// Observer.h - declaration of the Observer class.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#ifndef OBSERVER_H
#define OBSERVER_H

#include <vector>

class Subject;			// Forward declaration.

class Observer
{
public:
	Observer(void) {};
	virtual ~Observer(void) {};
	virtual void notify(Subject* s) = 0;
};

class Subject
{
public:
	Subject() {};
	virtual ~Subject() {};
    void registerObserver(Observer* o)
    {
        observers.push_back(o);
    }

	void unregisterObserver()
    {
        observers.pop_back();
    }

	void notifyObservers()
    {
        std::vector<Observer*>::iterator iter;
        for (iter = observers.begin(); iter != observers.end(); ++iter)
            (*iter)->notify(this);
    }

private:
	std::vector<Observer*> observers;
};

#endif // OBSERVER_H