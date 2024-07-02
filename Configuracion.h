#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <Preferences.h>
#include<WiFi.h>
#include "Evento.h"


#define NUM_EVENTOS 14

class Configuracion {
  
  public:

    static inline const long CONNECT_DEADLINE = 60 * 1000;  //60 seg
  
    Configuracion();
    static inline const int CANT_EVENTOS = 14;
  // Getters
    String getSsid() const;
    String getPassword() const;
    String getPhoneNumber() const;
    String getTelegramToken() const;
    int64_t getSenderId() const;
    Evento* getEvento(int i);
    bool getIsValid() const;
    bool getIsAutoActive() const;
    bool getAreNotificationsOn() const;
    bool isEventActive(int i);
    
  // Setters
    void setSsid(const String& newSsid);
    void setPassword(const String& newPassword);
    void setPhoneNumber(const String& newPhoneNumber);
    void setTelegramToken(const String& newToken);
    void setSenderId(const int64_t id);
    void setIsValid(const bool& newIsValid);
    void setIsAutoActive(const bool b);
    void setAreNotificationsOn(const bool b);
    void setEventActive(int i,bool b);
    
  // System
    void saveInSystem();
    void saveInSystemWC();// WC == Without Check -> Utilizado para restaurar la memoria... ni idea
    static Configuracion getFromSystem();
  // Otros
  
    void printState();
    bool isConnected();
    String showActiveEvents();

  //TODO
    int getNumActiveEvents();
    bool setNewEvent(Evento& event);// Es bool por si por alguna razon fall ej: ya hay 14 eventos activos

  private:
    // Datos Persistentes
    String ssid;
    String password;
    String phoneNumber;
    String telegramToken;
    int64_t senderId;
    Evento eventos[CANT_EVENTOS];
    bool isValid;
    bool isAutoActive;
    bool areNotificationsOn; 

    // Datos de Sesion

    // Funciones
    bool isWifiConnected();
    String eventKey(int i);// for saving an event
    
};
#endif // CONFIGURACION_H
