// EPOS Thread Abstraction Implementation

#include <system/kmalloc.h>
#include <machine.h>
#include <thread.h>
#include <alarm.h>

__BEGIN_SYS

// Class attributes
Scheduler_Timer * Thread::_timer;

Thread* volatile Thread::_running;
Thread::Queue Thread::_ready;
Thread::Queue Thread::_suspended;

// This_Thread class attributes
bool This_Thread::_not_booting;

// Methods
void Thread::common_constructor(Log_Addr entry, unsigned int stack_size) {
	db<Thread>(TRC) << "Thread(entry=" << entry << ",state=" << _state
			<< ",priority=" << _link.rank() << ",stack={b=" << _stack << ",s="
			<< stack_size << "},context={b=" << _context << "," << *_context
			<< "}) => " << this << endl;

	switch (_state) {
	case RUNNING:
		break;
	case SUSPENDED:
		_suspended.insert(&_link);
		break;
	default:
		_ready.insert(&_link);
	}

	unlock();
}

Thread::~Thread() {
	lock();

	db<Thread>(TRC) << "~Thread(this=" << this << ",state=" << _state
			<< ",stack={b=" << _stack << ",context={b=" << _context << ","
			<< *_context << "})\n";

	_ready.remove(this);
	_suspended.remove(this);

	if (_refSleeping) {
		_refSleeping->remove(this);
	}

	if (_threadJoined) {
		_threadJoined->resume();
	}

	unlock();

	kfree(_stack);
}

int Thread::join() {
	lock();

	db<Thread>(TRC) << "Thread::join(this=" << this << ",state=" << _state
			<< ")" << endl;

	if (_state != FINISHING) {
		_threadJoined = running();
		_threadJoined->suspend();
	}
	unlock();

	return *static_cast<int *>(_stack);
}

void Thread::pass() {
	lock();

	db<Thread>(TRC) << "Thread::pass(this=" << this << ")" << endl;

	Thread * prev = _running;
	prev->_state = READY;
	_ready.insert(&prev->_link);

	_ready.remove(this);
	_state = RUNNING;
	_running = this;

	dispatch(prev, this);

	unlock();
}

void Thread::suspend() {
	lock();

	db<Thread>(TRC) << "Thread::suspend(this=" << this << ")" << endl;

	if (_running != this)
		_ready.remove(this);

	_state = SUSPENDED;
	_suspended.insert(&_link);

	if ((_running == this)) {
		_running = _ready.remove()->object();
		_running->_state = RUNNING;

		dispatch(this, _running);
	}

	unlock();
}

void Thread::resume() {
	lock();

	db<Thread>(TRC) << "Thread::resume(this=" << this << ")" << endl;

	_suspended.remove(this);
	_state = READY;
	_ready.insert(&_link);

	unlock();
}

void Thread::sleep(Queue *sleepingQ) {
	db<Thread>(TRC) << "Thread::sleep(this=" << this << ")" << endl;

	assert(locked());

//	while (_ready.empty()) {
//			idle();
//	}

	_refSleeping = sleepingQ;
	_state = WAITING;
	sleepingQ->insert(&_link);

	_running = _ready.remove()->object();
	_running->_state = RUNNING;

	dispatch(this, _running);

	unlock();
}

void Thread::wakeup(Queue* _sleeping) {
	assert(locked());

	if (!_sleeping->empty()) {
		Thread * wkup_thread = _sleeping->remove()->object();
		wkup_thread->_refSleeping = 0;
		wkup_thread->_state = READY;
		_ready.insert(&wkup_thread->_link);
	}

	unlock();

	if (preemptive)
		reschedule();
}

// Class methods
void Thread::yield() {
	lock();

	db<Thread>(TRC) << "Thread::yield(running=" << _running << ")" << endl;

	Thread * prev = _running;
	prev->_state = READY;
	_ready.insert(&prev->_link);

	_running = _ready.remove()->object();
	_running->_state = RUNNING;

	dispatch(prev, _running);

	unlock();
}

void Thread::exit(int status) {
	lock();

	db<Thread>(TRC) << "Thread::wakeup_all(running=" << running() << ")"
			<< endl;

	if (_running->_threadJoined) {
		_running->_threadJoined->resume();
		_running->_threadJoined = 0;
	}

	lock();

	Thread * prev = _running;
	prev->_state = FINISHING;
	*static_cast<int *>(prev->_stack) = status;

	_running = _ready.remove()->object();
	_running->_state = RUNNING;

	dispatch(prev, _running);

	unlock();
}

void Thread::reschedule() {
	yield();
}

void Thread::implicit_exit() {
	exit(CPU::fr());
}

int Thread::idle() {
	db<Thread>(TRC) << "Thread::idle()" << endl;

	while (true) {
		if (_suspended.empty() && !running()->_refSleeping) {
			db<Thread>(WRN) << "The last thread in the system has exited!\n";
			if (reboot) {
				db<Thread>(WRN) << "Rebooting the machine ...\n";
				Machine::reboot();
			} else {
				db<Thread>(INF) << "Halting the CPU ..." << endl;
				CPU::int_disable();
				CPU::halt();
			}
		} else {
			db<Thread>(INF) << "There are no runnable threads at the moment!"
					<< endl;
			db<Thread>(INF) << "Halting the CPU ..." << endl;
			CPU::int_enable();
			CPU::halt();
		}
	}
	return 0;
}

// Id forwarder to the spin lock
unsigned int This_Thread::id() {
	return _not_booting ?
			reinterpret_cast<volatile unsigned int>(Thread::self()) :
			Machine::cpu_id() + 1;
}

__END_SYS
