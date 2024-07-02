#ifndef BOT_MANAGER_H
#define BOT_MANAGER_H

#include"Manager.h"
#include "CTBot.h"
#include"Evento.h"
#include"Configuracion.h"
#include"DeviceManager.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include<regex.h>

#define REGAR_CALLBACK "regar"
#define SETTINGS_CALLBACK "settings" // Para un futuro baquero
#define CONF_EVENTOS_CALLBACK "configurarEventos"
#define AGREGAR_EVENTO_CALLBACK "agregarEvento"
#define ELIMINAR_EVENTO_CALLBACK "eliminarEvento"
#define DESACTIVAR_EVENTO_CALLBACK "desactivarAutomatico"
#define ACTIVAR_EVENTO_CALLBACK "activarAutomatico"
#define DESACTIVAR_NOTIFICACIONES_CALLBACK "desactivarNotificaciones"
#define ACTIVAR_NOTIFICACIONES_CALLBACK "activarNotificaciones"

class BotManager: Manager, public Observer {

  public:
  // CONSTRUCTORES
  BotManager(Configuracion& conf,DeviceManager* device, CTBot* bot);

  void handleSetup() override;
  void handleRutineJob() override;

  void notify(String msg) override;

  bool getIsAvailable();
  bool isSettingEvent;

  private:
  DeviceManager* deviceManager;
  bool isTelegramConnected;
  CTBotInlineKeyboard helloKbd;  // custom inline keyboard object helper
//  CTBotInlineKeyboard EventKbd;  // 
  
  CTBotReplyKeyboard HoraKbd;
  CTBotReplyKeyboard MinKbd;
  CTBotReplyKeyboard DiasKbd;
  CTBotReplyKeyboard DiasEventKbd;  // 
  
  CTBot* bot;

  void connectBot();
  void handleMessages();
  void buttonsSetup();
  bool verifyHourFormat(const String& input);
  bool verifyMinFormat(const String& input);
  bool verifyWeekFormat(const String& input);
  bool verifyEliminateFormat(const String& input);
//  bool verifyEventIsActive(const int num);
  int getNumEvent(const String& input);

  bool testTelegramConnection();
  void wrapper();
  
};


#endif //BOT_MANAGER_H
