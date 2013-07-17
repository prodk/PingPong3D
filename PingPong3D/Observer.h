#pragma once
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
	virtual ~Subject() {};
    void registerObserver(Observer* o)
    {
        observers.push_back(o);
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