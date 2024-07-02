#include "TimeManager.h"


const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -3 * 3600; // Ajustar según tu zona horaria
const int daylightOffset_sec = 0; // Ajustar para el horario de verano si es necesario


// CONSTRUCTORES



TimeManager::TimeManager(Configuracion& conf) : Manager(conf) {
    syncClock();
 }

// FUNCIONES

void TimeManager::handleSetup(){
  if(conf.isConnected()){
    syncClock();
  }
}

void TimeManager::handleRutineJob(){
  syncClock();
  if(isSync()){
     showTime();
  }
  else{
    Serial.println(" --- Hora no Syncronizada ---");
  }
  
}

void TimeManager::syncClock() {
    // Verifico conexion a Internet
    if(!conf.isConnected()){
      return;
    }
    
    // Configuración de la zona horaria
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    // Espera hasta que la hora sea sincronizada

    unsigned long startTime = millis();
    
    while (time(nullptr) < 24*3600 && millis() - startTime < 150000) {
        Serial.print(".");
        delay(1000);
    }
    if(time(nullptr) >= 24*3600 && millis()){
      Serial.println(" --- Hora sincronizada --- ");
      this->sync = true;
    }
    else{
      Serial.println(" --- No se pudo sincronizar la hora --- ");
      this->sync = false;
    }
    
}

void TimeManager::showTime(){
  // Verifico conexion a Internet
    if(!conf.isConnected()){
      return;
    }
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println(" --- Failed to obtain time ---");
        return;
    }
    Serial.println(&timeinfo, "Local Time: %A, %B %d %Y %H:%M:%S");
}

bool TimeManager::isSync(){
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        // No se pudo obtener la hora, por lo tanto, el reloj no está sincronizado
        return false;
    }
    // Se obtuvo la hora exitosamente, el reloj está sincronizado
    return true;
}
