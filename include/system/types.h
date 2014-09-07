// EPOS Internal Type Management System

typedef __SIZE_TYPE__ size_t;

#ifndef __types_h
#define __types_h

inline void * operator new(size_t s, void * a) { return a; }
inline void * operator new[](size_t s, void * a) { return a; }

__BEGIN_SYS

// Dummy class for incomplete architectures and machines 
template<int>
class Dummy;

// Utilities
class Debug;
class Lists;
class Spin;
class Heap;

// System parts
class Build;
class Boot;
class Setup;
class Init;
class System;
class Application;

// Hardware Mediators - CPU
class IA32;

// Hardware Mediators - Time-Stamp Counter
class IA32_TSC;

// Hardware Mediators - Memory Management Unit
class IA32_MMU;

// Hardware Mediators - Machine
class PC;

// Hardware Mediators - Bus
class PC_PCI;

// Hardware Mediators - Interrupt Controller
class PC_IC;

// Hardware Mediators - Timer
class PC_Timer;

// Hardware Mediators - Real-time Clock
class PC_RTC;

// Hardware Mediators - EEPROM
class PC_EEPROM;

// Hardware Mediators - UART
class PC_UART;

// Hardware Mediators - Display
class Serial_Display;
class PC_Display;

// Abstractions	- Process
class Thread;

// Abstractions	- Synchronization
class Synchronizer;
class Mutex;
class Semaphore;
class Condition;

// Abstractions	- Time
class Clock;
class Alarm;
class Chronometer;

// Abstractions - Network
class Ethernet;


// System Components IDs
// The order in this enumeration defines many things in the system (e.g. init)
typedef unsigned int Type_Id;
enum 
{
    CPU_ID,
    TSC_ID,
    MMU_ID,

    MACHINE_ID,
    PCI_ID,
    IC_ID,
    TIMER_ID,
    RTC_ID,
    EEPROM_ID,
    UART_ID,
    DISPLAY_ID,

    THREAD_ID,

    MUTEX_ID,
    SEMAPHORE_ID,
    CONDITION_ID,

    CLOCK_ID,
    ALARM_ID,
    CHRONOMETER_ID,

    UNKNOWN_TYPE_ID,
    LAST_TYPE_ID = UNKNOWN_TYPE_ID - 1
};

// Type IDs for system components
template<typename T> struct Type { static const Type_Id ID = UNKNOWN_TYPE_ID; };

template<> struct Type<IA32> { static const Type_Id ID = CPU_ID; };
template<> struct Type<IA32_TSC> { static const Type_Id ID = TSC_ID; };
template<> struct Type<IA32_MMU> { static const Type_Id ID = MMU_ID; };

template<> struct Type<PC> { static const Type_Id ID = MACHINE_ID; };
template<> struct Type<PC_IC> { static const Type_Id ID = IC_ID; };
template<> struct Type<PC_Timer> { static const Type_Id ID = TIMER_ID; };
template<> struct Type<PC_UART> { static const Type_Id ID = UART_ID; };
template<> struct Type<PC_RTC> { static const Type_Id ID = RTC_ID; };
template<> struct Type<PC_PCI> { static const Type_Id ID = PCI_ID; };
template<> struct Type<PC_Display> { static const Type_Id ID = DISPLAY_ID; };

template<> struct Type<Thread> { static const Type_Id ID = THREAD_ID; };

__END_SYS

#endif
