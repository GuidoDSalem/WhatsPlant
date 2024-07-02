#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include "Manager.h"
#include "Subject.h"
#include "Evento.h"
//#include "BotManager.h"
#include <time.h>
#include"DisplayManager.h"

#define PIN_LED BUILTIN_LED
#define PIN_DISPLAY 18
#define PIN_AGUA 33

enum DeviceMode{
  RUTINE_MODE,
  CHAT_MODE
};

class DeviceManager: Manager, Subject  {

  public:
  //CONSTRUCTOR
    DeviceManager(Configuracion& conf);

    static inline const long WAKE_UP_INTERVAL = 15;// 15 seg //60 * 1; //1min * cant_minutos
    static inline const long AWAKE_TOLERANCE = 30 * 4;  // 
    static inline const long OFFSET = 30;  // 
     
    
    void handleSetup() override;
    void handleRutineJob() override;

    void addObserver(Observer* o) override;
    void removeObserver() override;
    void notifyObservers(String msg) override;

//    void setBotManager(BotManager* botManager);

    time_t lastInteraction;
    void UpdateLastInteraction();

    void showMsg(String msg);
    void showQR(String url);
    
    void regar();
    void regarCant(int miliLitros);

    long timeUntilNextEvent();

    void evaluateAndEnterDeepSleep();
    
    void prenderLed();
    void apagarLed();

    Evento nextEvent();

    
    String getSensorsInfo(); // Para el futuro

  private:
    Observer* observador;
    DeviceMode currentMode;

    Display deviceDisplay;
    
    void setupPines();
    void displayStatus();
    void displayText(String text);
    void updateDeviceMode();
    void handleEvents();
    void deepSleep();
    void deepSleep(int segundos);
    void lightSleep(int segundos);
//    void deepSlpeep(int minutos,int horas);
};

#endif DEVICE_MANAGER_H
