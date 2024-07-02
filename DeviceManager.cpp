#include "DeviceManager.h"


DeviceManager::DeviceManager(Configuracion& conf) : Manager(conf){
  this->currentMode = RUTINE_MODE;
  this->lastInteraction = -1; // Cuando despierta el dispositivo, si no hubo ningun mensage lo indico con un time_t negativo
  this->deviceDisplay = Display();
  
};

void DeviceManager::setupPines(){
  // LED
  pinMode(PIN_LED,OUTPUT);
  digitalWrite(PIN_LED, LOW);// HIGH - LOW

  // DISPLAY
  pinMode(PIN_DISPLAY,OUTPUT);
  digitalWrite(PIN_DISPLAY,LOW);

  //BOMBA AGUA
  pinMode(PIN_AGUA,OUTPUT);
  digitalWrite(PIN_AGUA, LOW);// HIGH - LOW

}

// SETUP
void DeviceManager::handleSetup(){
  setupPines();
  displayStatus();
  

}

// RUTINE JOB
void DeviceManager::handleRutineJob() {
  long nextEventTime = timeUntilNextEvent();
  updateDeviceMode();
  Serial.println(" --- Device is Working ---\n");
  evaluateAndEnterDeepSleep();
}

void DeviceManager::displayStatus(){
  digitalWrite(PIN_DISPLAY,HIGH);
  deviceDisplay.displayText("HOLAMUNDO");
  delay(5000);
  digitalWrite(PIN_DISPLAY,LOW);
}

void DeviceManager::displayText(String text){
  digitalWrite(PIN_DISPLAY,HIGH);
  deviceDisplay.displayText(text);
  delay(5000);
  digitalWrite(PIN_DISPLAY,LOW);
}

void DeviceManager::evaluateAndEnterDeepSleep(){

//  long timeNextEvent = timeUntilNextEvent();
  long ne = timeUntilNextEvent();               // ---
  long wui = DeviceManager::WAKE_UP_INTERVAL;   // ___
  long of = DeviceManager::OFFSET;             // ooo
  Serial.print(" --- Segundo para el siguiente Evento: ");Serial.print(ne);Serial.println(" ---");
  switch(currentMode){
    case RUTINE_MODE:
        //  ______________
        //  -----------------oooo
        if(ne + of - wui > of){
          deepSleep();
        }
       //  ______________
       //  -------------oooo
        else if( (ne + of > wui) && (ne + of - wui < of) ){
//          deepSleep(ne - of); Elejir
          lightSleep(ne);
          regar();
          if(conf.getAreNotificationsOn()){
            notifyObservers("Evento Activado: Ya regamos  tu Planta :)");// OPCIONAL -> TODO CAMBIAR MENSAGE
          }
          
        }
        //  ______________
        //  ---------oooo
        else if( ne + of < wui ){
          lightSleep(ne);
          regar();
          if(conf.getAreNotificationsOn()){
            notifyObservers("Evento Activado: Ya regamos  tu Planta :)");// OPCIONAL -> TODO CAMBIAR MENSAGE
          }
        }
      break;

    case CHAT_MODE:
      if(ne < wui){
        lightSleep(ne);
        regar();
        if(conf.getAreNotificationsOn()){
            notifyObservers("Evento Activado: Ya regamos  tu Planta :)");// OPCIONAL -> TODO CAMBIAR MENSAGE
        }
      }
      else{
//        lightSleep(3);
      }
      Serial.println(" --- ChatMode: no sleep ---");
      break; 
  }
}


void DeviceManager::addObserver(Observer* o){
  this->observador = o;
}
void DeviceManager::removeObserver(){
  this->observador = 0;
}
void DeviceManager::notifyObservers(String msg){
  Serial.println(" --- avisando a los observadores... ---");
  this->observador->notify(msg);
}

void DeviceManager::updateDeviceMode(){
  
  time_t now = time(NULL);
  
  switch(this->currentMode){

    case RUTINE_MODE:
      // Check If there was any Message
      if( lastInteraction > 0 ){ // se produjo un mensaje
        currentMode = CHAT_MODE;
      }
      break;

    case CHAT_MODE:
      if(difftime(now,lastInteraction) > DeviceManager::AWAKE_TOLERANCE){
        Serial.println(" --- Awake tolerance achived, going to sleep ---");
        currentMode = RUTINE_MODE;
        notifyObservers("Bueno me voy a dormir... cualquier cosa escribime :)");
      }
      break;
  }
}

void DeviceManager::UpdateLastInteraction(){
  this->lastInteraction = time(NULL);
}

void DeviceManager::showMsg(String msg){}
void DeviceManager::showQR(String url){}

void DeviceManager::regar(){
  digitalWrite(PIN_LED,HIGH);
  digitalWrite(PIN_AGUA,HIGH);
  displayText("Regando...");
  delay(5000);
  digitalWrite(PIN_AGUA,LOW);
  digitalWrite(PIN_LED,LOW);
};
//Suponer: 100mliliLitros == 100miliSeg, Despues hay que fijarse posta
void DeviceManager::regarCant(int miliLitros){
  digitalWrite(PIN_LED,HIGH);
  delay(miliLitros);
  digitalWrite(PIN_LED,LOW);
}

void DeviceManager::prenderLed(){}
void DeviceManager::apagarLed(){}

//void  DeviceManager::setBotManager(BotManager* botManager){
//  this->botManager = botManager;
//}

String DeviceManager::getSensorsInfo(){} // Para el futuro

long DeviceManager::timeUntilNextEvent(){

  long tiempos[Configuracion::CANT_EVENTOS];
  for(int i=0; i<Configuracion::CANT_EVENTOS; i++){
    tiempos[i] = (conf.getEvento(i))->cuantoFalta();
  }
  long tiempoMenor = 92233720368547758;// nuemro muy grande
  for(int i=0;i<Configuracion::CANT_EVENTOS; i++){
//    Serial.print("Tiempo restante ");Serial.print(i);Serial.print(": "); Serial.println(tiempos[i]);
    if(tiempos[i] < tiempoMenor){
      
      tiempoMenor = tiempos[i];

//      Serial.print("\n Tiempo cambio a :"); Serial.println(tiempoMenor);
      delay(50);
    }
  }

//  Serial.print("\n Tiempo MENOR :"); Serial.println(tiempoMenor);

  return tiempoMenor;

}
  
  

void DeviceManager::lightSleep(int segundos){
  // Configura un temporizador para despertar el ESP32
  esp_sleep_enable_timer_wakeup(segundos * 1000000); // Despierta después de 5 seg
  Serial.print(" --- Light Sleep: ");Serial.print(segundos);Serial.println("seg ---");
  Serial.flush();
  esp_light_sleep_start();
}

void DeviceManager::deepSleep(){
  Serial.print(" --- Deep Sleep: ");Serial.print(DeviceManager::WAKE_UP_INTERVAL);Serial.println("seg ---");
  // Configure the timer to be the wakeup source
  esp_sleep_enable_timer_wakeup( (DeviceManager::WAKE_UP_INTERVAL - DeviceManager::OFFSET) * 1000000); // Time in microseconds

  // Enter deep sleep
  Serial.flush();
  esp_deep_sleep_start();
}

void DeviceManager::deepSleep(int segundos){
  Serial.print(" --- Deep Sleep: ");Serial.print(segundos);Serial.println("seg ---");
  
  // Configure the timer to be the wakeup source
  esp_sleep_enable_timer_wakeup( (segundos - DeviceManager::OFFSET) * 1000000); // Time in microseconds
  
    // Desactiva todos los dominios de energía que no sean esenciales para reducir el consumo de energía
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF); // Desactiva los periféricos RTC
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF); // Desactiva la memoria lenta RTC
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF); // Desactiva la memoria rápida RTC
  esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF); // Desactiva otros dominios de energía, si es aplicable


  // Enter deep sleep
  Serial.flush();
  esp_deep_sleep_start();
}

void DeviceManager::handleEvents(){
  
}

//void DeviceManager::deepSlpeep(int minutos,int horas){}
