// EPOS Synchronizer Abstractions Common Package

#ifndef __synchronizer_h
#define __synchronizer_h

#include <cpu.h>
#include <thread.h>

__BEGIN_SYS

class Synchronizer_Common {
protected:
	Synchronizer_Common() {
	}
	~Synchronizer_Common() {
		begin_atomic();
		wakeup_all();
	}

	// Atomic operations
	bool tsl(volatile bool & lock) {
		return CPU::tsl(lock);
	}
	int finc(volatile int & number) {
		return CPU::finc(number);
	}
	int fdec(volatile int & number) {
		return CPU::fdec(number);
	}

	// Thread operations
	void begin_atomic() {
		Thread::lock();
	}
	void end_atomic() {
		Thread::unlock();
	}

	void sleep() {
		begin_atomic();

		Thread * running = Thread::running();
		running->sleep(&_sleeping);

		end_atomic();
	}
	void wakeup() {
		Thread::wakeup(&_sleeping);
	}
	void wakeup_all() {
		while (!_sleeping.empty()) {
			Thread::wakeup(&_sleeping);
		}
	}
private:
	Thread::Queue _sleeping;
};

__END_SYS

#endif
