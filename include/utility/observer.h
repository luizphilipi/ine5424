// EPOS Observer Utility Declarations

// Observation about the lack of virtual destructors in the following classes:
// Observed x Observer is used in mediators, so they appear very early in the system.
// To be more precise, they are used in SETUP, where we cannot yet handle a heap.
// Since the purpose of the destructors is only to trace the classes, we accepted to
// declare them as non-virtual. But it must be clear that this is one of the few uses
// for them.

#ifndef __observer_h
#define	__observer_h

#include <utility/list.h>

__BEGIN_SYS

// Observer x Observed
class Observer;

class Observed
{ 
    friend class Observer;

private:
    typedef Simple_List<Observer>::Element Element;

protected:
    Observed() {
        db<Observed>(TRC) << "Observed() => " << this << endl;
    }

public: 
    ~Observed() {
        db<Observed>(TRC) << "~Observed(this=" << this << ")" << endl;
    }

    virtual void attach(Observer * o);
    virtual void detach(Observer * o);
    virtual bool notify();

private: 
    Simple_List<Observer> _observers;
}; 

class Observer
{ 
    friend class Observed;

protected: 
    Observer(): _link(this) {
        db<Observer>(TRC) << "Observer() => " << this << endl;
    }

public: 
    ~Observer() {
        db<Observer>(TRC) << "~Observer(this=" << this << ")" << endl;
    }

    virtual void update(Observed * o) = 0;

private:
    Observed::Element _link;
};

inline void Observed::attach(Observer * o)
{
    db<Observed>(TRC) << "Observed::attach(obs=" << o << ")" << endl;

    _observers.insert(&o->_link);
}

inline void Observed::detach(Observer * o)
{
    db<Observed>(TRC) << "Observed::detach(obs=" << o << ")" << endl;

    _observers.remove(&o->_link);
}

inline bool Observed::notify()
{
    bool notified = false;

    db<Observed>(TRC) << "Observed::notify()" << endl;

    for(Element * e = _observers.head(); e; e = e->next()) {
        db<Observed>(INF) << "Observed::notify(this=" << this << ",obs=" << e->object() << ")" << endl;

        e->object()->update(this);
        notified = true;
    }

    return notified;
}


// Observer x Conditionally Observed
class Conditional_Observer;

class Conditionally_Observed
{
    friend class Conditional_Observer;

private:
    typedef Simple_Ordered_List<Conditional_Observer>::Element Element;

protected:
    Conditionally_Observed() {
        db<Observed>(TRC) << "Observed() => " << this << endl;
    }

public:
    ~Conditionally_Observed() {
        db<Observed>(TRC) << "~Observed(this=" << this << ")" << endl;
    }

    virtual void attach(Conditional_Observer * o, int c);
    virtual void detach(Conditional_Observer * o, int c);
    virtual bool notify(int c);

private: 
    Simple_Ordered_List<Conditional_Observer> _observers;
}; 

class Conditional_Observer
{
    friend class Conditionally_Observed;

public:
    Conditional_Observer(): _link(this) {
        db<Observer>(TRC) << "Observer() => " << this << endl;
    } 

    ~Conditional_Observer() {
        db<Observer>(TRC) << "~Observer(this=" << this << ")" << endl;
    }
    
    virtual void update(Conditionally_Observed * o, int c) = 0;

private:
    Conditionally_Observed::Element _link;
};

inline void Conditionally_Observed::attach(Conditional_Observer * o, int c)
{
    db<Observed>(TRC) << "Observed::attach(o=" << o << ",c=" << c << ")" << endl;

    o->_link = Element(o, c);
    _observers.insert(&o->_link);
}

inline void Conditionally_Observed::detach(Conditional_Observer * o, int c)
{
    db<Observed>(TRC) << "Observed::detach(obs=" << o << ",c=" << c << ")" << endl;

    _observers.remove(&o->_link);
}

inline bool Conditionally_Observed::notify(int c)
{
    bool notified = false;

    db<Observed>(TRC) << "Observed::notify(cond=" << hex << c << ")" << endl;

    for(Element * e = _observers.head(); e; e = e->next()) {
        if(e->rank() == c) {
            db<Observed>(INF) << "Observed::notify(this=" << this << ",obs=" << e->object() << ")" << endl;
            e->object()->update(this, c);
            notified = true;
        }
    }

    return notified;
}

__END_SYS
 
#endif
