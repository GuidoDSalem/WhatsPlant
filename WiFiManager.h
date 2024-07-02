#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include"Configuracion.h"
#include"Manager.h"
#include<WiFi.h>
#include<WebServer.h>
#include "CTBot.h"

#define ESP_SSID "ESP_WIFI_2.4"
#define ESP_PASSWORD "12345678"



class WifiManager: Manager{
  
  public:
    WifiManager(Configuracion& conf, WebServer* server);

    void handleSetup() override;
    void handleRutineJob() override;

    // TEMPORAL
    bool userIsWorking = false;
    
    
  private:
  // PROPIEDADES
    WebServer* server;


  // FUNCIONES
    bool isConnected();
    void connectWifi();
    void handleReconnection(); // Utilizada en loop en caso de que se haya perdido la coneccion
    void handleConfiguration(); // monta el server para que el usuario cargue los valores
    void showDisconnectionStatus();// Mostrar por pantalla TODO

    // FUNCIONES AUX
    static String generateWifiHTML();
    static String generateTelegramHTML();
    void handleGetRoot();
    void handlePostConf();
    void handleGetTelegram();
    void handlePostTelegram();
};

#endif // WIFIMANAGER_H
