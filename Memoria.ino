#include <esp_sleep.h>
#include <WebServer.h>
#include <WiFi.h>
#include "CTBot.h"
#include "Configuracion.h"
#include "DeviceManager.h"
#include "WifiManager.h"
#include "TimeManager.h"
#include "BotManager.h"


///
#include <nvs_flash.h>
///


// Variables Globales

Configuracion conf;
WifiManager* wifiManager = 0;
TimeManager* timeManager = 0;
BotManager* botManager = 0;
DeviceManager* deviceManager = 0;
WebServer* server = 0;
CTBot* bot = 0;


void setup() {
  Serial.begin(115200);
  // Espera a que la consola serial esté lista
  while (!Serial) continue;

  // RESTORE MEMORY //
//  nvs_flash_erase(); // erase the NVS partition and...
//  nvs_flash_init(); // initialize the NVS partition.
//  while(true);
  // END RESTORE MEMORY //

  // PRE-SETUP //
//  Configuracion configuracion = Configuracion();
//  configuracion.printState();
//  configuracion.saveInSystemWC(); 
  // END PRE-SETUP //

  delay(200);
  Serial.println("\n\n\n  --- Whats' Plant ---  ");

  conf = conf.getFromSystem();
  conf.printState();

  // DEVICE
  deviceManager = new DeviceManager(conf);
  deviceManager->handleSetup();

  // WIFI
  server = new WebServer(80);
  wifiManager = new WifiManager(conf,server);
  wifiManager->handleSetup();

  // TIME
  timeManager = new TimeManager(conf);
  timeManager->handleSetup();


  // TELEGRAM
  bot = new CTBot();
  botManager = new BotManager(conf,deviceManager,bot);
  botManager->handleSetup();

  deviceManager->addObserver(botManager);
   
  Serial.println("--- End SetUp ---");
  conf.printState();
  Serial.println(" --- Entre al Loop --- ");
}

void loop() {
//  conf.printState();
//  delay(10000);
  
  wifiManager->handleRutineJob();
  timeManager->handleRutineJob();
  botManager->handleRutineJob();
  deviceManager->handleRutineJob();

 
}
