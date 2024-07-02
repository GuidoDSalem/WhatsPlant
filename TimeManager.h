#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include"Configuracion.h"
#include"Manager.h"
#include <WiFi.h>
#include <time.h>

//void setupTime();

class TimeManager: Manager {
  public:
    TimeManager(Configuracion& conf);
    
    void handleRutineJob() override; // Called In loop - TODO
    void handleSetup() override; //  Called in setup - TODO
    bool isSync();
    void showTime();
    
    

  private:
    bool sync;
    void syncClock(); 
    void setTimeInSystem(); // TODO
    
  
};

#endif
