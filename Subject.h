#ifndef SUBJECT_H
#define SUBJECT_H

#include "Observer.h"

class Subject{
  public:
    virtual void addObserver(Observer* o);
    virtual void removeObserver();
    virtual void notifyObservers(String msg);
};
#endif //SUBJECT_H
