#ifndef MANAGER_H
#define MANAGER_H

#include "Configuracion.h"

class Manager {
protected:
    Configuracion& conf;
    bool isReady;

public:
    Manager(Configuracion& conf);

    virtual void handleSetup() = 0;
    virtual void handleRutineJob() = 0;
    bool isManagerReady();

    
};

#endif // MANAGER_H
