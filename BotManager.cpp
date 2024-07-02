#include "BotManager.h"
// Constructor con configuración
BotManager::BotManager(Configuracion& conf, DeviceManager* device, CTBot* botPtr) : Manager(conf), bot(botPtr){
    isSettingEvent = false;
    this->deviceManager = device;
    isTelegramConnected = false;
    
    buttonsSetup();

}

// PUBLIC FUNCTION
void BotManager::handleSetup(){
  
  connectBot();
  
}

void BotManager::handleRutineJob(){
  
  // a variable to store telegram message data
  if(!this->getIsAvailable()){
    Serial.println(" --- Bot NOT is avaialable --- ");
    return;
  }
//  if(!isTelegramConnected){
//    connectBot();
//  }
  connectBot();
  handleMessages();
}




void BotManager::notify(String msg){
    Serial.println(" --- Enviando Mensaje Automatico por Evento Activado ---");
    bot->sendMessage(conf.getSenderId(),msg);
  }





bool BotManager::getIsAvailable(){
  return conf.getIsValid();
}

void BotManager::connectBot(){
  
  if(!conf.getIsValid()){
    Serial.println(" --- Bot is NOT Avaialable! --- ");
    return;
  }
  if(!this->conf.isConnected()){
    return;
  }
  if(this->isTelegramConnected){
    return;
  }
    
  bot->setTelegramToken(conf.getTelegramToken());
  
  delay(100);

  // Esperar a que se conecte al WiFi
  Serial.print("Conectando a Telegram...");

  if (bot->testConnection()){
    Serial.println("\n --- Bot is Connected --- ");
    this->isTelegramConnected = true;
  } 
  else{
    Serial.println(" --- Bot Connection failed --- ");
    this->isTelegramConnected = false;
  }
  
}

void BotManager::wrapper(){
    // Llamada a la función de la biblioteca.
    bot->testConnection();
    vTaskDelete(NULL); // Asegura que la tarea se elimine una vez completada la ejecución.

}

bool BotManager::testTelegramConnection(){
//    TaskHandle_t taskHandle = NULL;
//
//    xTaskCreate(wrapper, "LongRunFunc", 2048, NULL, 5, &taskHandle);
//
//    if (taskHandle != NULL) {
//        // Espera por la tarea a finalizar o por el timeout.
//        if (xTaskNotifyWait(0, 0, NULL, pdMS_TO_TICKS(timeoutMs)) == pdFALSE) {
//            // Timeout alcanzado, la tarea aún está en ejecución.
//            vTaskDelete(taskHandle); // Forzamos la terminación de la tarea.
//            return false;
//        }
//        // Si llegamos aquí, la tarea ha terminado por sí misma o ha sido eliminada.
//        return true;
//    }
}

bool BotManager::verifyHourFormat(const String& s){
    // Verificar la longitud de la cadena
    Serial.println(" --- a ver la longitud... ---");
    if (s.length() != 3) return false;
    Serial.println(" --- a ver los : ... ---");
    // Verificar que el primer carácter sea ':'
    if (s.charAt(2) != ':') return false;
    
    // Verificar que los dos últimos caracteres sean dígitos
    for (int i = 0; i < 2; i++) {
      Serial.print(" --- supuesto num: "+i);Serial.print(s.charAt(i));Serial.println(" --- ");
      if (!isDigit(s.charAt(i))) return false;
    }
    
    // Si todas las comprobaciones son verdaderas, el formato es correcto
    return true;
}

bool BotManager::verifyMinFormat(const String& s){
    // Verificar la longitud de la cadena
    if (s.length() != 3) return false;
    
    // Verificar que el primer carácter sea ':'
    if (s.charAt(0) != ':') return false;
    
    // Verificar que los dos últimos caracteres sean dígitos
    for (int i = 1; i < 3; i++) {
      if (!isDigit(s.charAt(i))) return false;
    }
    
    // Si todas las comprobaciones son verdaderas, el formato es correcto
    return true;
}

bool BotManager::verifyWeekFormat(const String& s){
      // Lista de días válidos
    const String diasValidos[] = {"Lu", "Ma", "Mi", "Ju", "Vi", "Sa", "Do"};
    
    // Tamaño del arreglo
    int numDias = sizeof(diasValidos) / sizeof(diasValidos[0]);
    
    // Recorrer el arreglo buscando una coincidencia
    for (int i = 0; i < numDias; i++) {
        if (s == diasValidos[i]) {
            return true; // El día pertenece al conjunto
        }
    }
    
    // Si se recorre todo el arreglo y no se encuentra el día, no pertenece al conjunto
    return false;
}

bool BotManager::verifyEliminateFormat(const String& input){
  if (input.length() == 8 || input.length() == 9) {
    // Comprobar si los primeros 7 caracteres son "Evento "
    if (input.substring(0, 7) == "Evento ") {
      // Comprobar si el siguiente caracter es un dígito
      if (input[7] >= '0' && input[7] <= '9') {
        // Si la longitud es 8, es un "Evento D" válido
        if (input.length() == 8) return true;
        // Si la longitud es 9, asegurarse de que el último caracter también sea un dígito
        if (input[8] >= '0' && input[8] <= '9') return true;
      }
    }
  }
  // Si no cumple con los criterios anteriores, no es un evento válido
  return false;
}

int BotManager::getNumEvent(const String& input){
  int num = -1;
  if(input.length() == 8){
    Serial.println(" --- Entre a 8 ---");
    num = input.substring(7,8).toInt();
    Serial.print("El evento en String: ");Serial.println(input.substring(9));
    Serial.print("El evento en Int: ");Serial.println(num);
  }
  else if(input.length() == 9){
    Serial.println(" --- Entre a 9 ---");
    num = input.substring(8,9).toInt(); 
    Serial.print("El evento en String: ");Serial.println(input.substring(7,9));
    Serial.print("El evento en Int: ");Serial.println(num);
  }
  Serial.print("El evento a eliminar es el: ");Serial.println(num);
  return num;
}




void BotManager::handleMessages(){

  if(!this->conf.isConnected()){
  return;
  }
  TBMessage msg; 

  Serial.println(" --- Operacion pesada? ---");

  // if there is an incoming message...
  Serial.flush();
  delay(150);
  if (bot->getNewMessage(msg,true)){
    this->deviceManager->UpdateLastInteraction();
    this->conf.setSenderId(msg.sender.id);

    Serial.println("--- Ya agarro el mensaje ---");
    
  // TEXT
    if(msg.messageType == CTBotMessageText){
      bot->sendMessage(msg.sender.id,"Bienvenido a tu Planta\n Que accion desea realizar:",helloKbd);
    }

// QUERY
    else if(msg.messageType == CTBotMessageQuery){
      Serial.println(" --- Entre a Query ---");

// REGAR
      if (msg.callbackQueryData.equals(REGAR_CALLBACK)){
        Serial.println(" --- Enviando Regar ---");
        delay(50);
        this->deviceManager->regar();
        bot->sendMessage(msg.sender.id,"Ya regamos tu Planta :)");
      }
            
// Eventos
      if(msg.callbackQueryData.equals(CONF_EVENTOS_CALLBACK)){
        CTBotInlineKeyboard EventKbd;
        EventKbd.addButton("Agregar Evento",AGREGAR_EVENTO_CALLBACK,CTBotKeyboardButtonQuery);
        EventKbd.addButton("Eliminar Evento",ELIMINAR_EVENTO_CALLBACK,CTBotKeyboardButtonQuery);
        EventKbd.addRow();
        if(conf.getIsAutoActive()){
          EventKbd.addButton("Desactivar riego automatico",DESACTIVAR_EVENTO_CALLBACK,CTBotKeyboardButtonQuery);
        }else{
          EventKbd.addButton("Activar riego automatico",ACTIVAR_EVENTO_CALLBACK,CTBotKeyboardButtonQuery);
        }
        EventKbd.addRow();
        if(conf.getAreNotificationsOn()){
          EventKbd.addButton("Desactivar notificaciones",DESACTIVAR_NOTIFICACIONES_CALLBACK,CTBotKeyboardButtonQuery);
        }else{
          EventKbd.addButton("Activar notificaciones",ACTIVAR_NOTIFICACIONES_CALLBACK,CTBotKeyboardButtonQuery);
        }
//        EventKbd.enableResize();
        

        
        Serial.println(" --- Entre a Evento ---");
        delay(50);
        String respond = this->conf.showActiveEvents();
        delay(50);
        bot->sendMessage(msg.sender.id,"  --- Eventos --- \n"+respond,EventKbd);
      }

// Eventos -> Agregar
      if(msg.callbackQueryData.equals(AGREGAR_EVENTO_CALLBACK)){
        Serial.println(" --- Entre a Evento -> Agregar ---");

        Evento newEvent = Evento();
        newEvent.setActive(true);
        
        unsigned long startTime;
        unsigned long timeout;
        
        
        startTime = millis();
        timeout = 30 * 5 *1000; // Tiempo de espera en milisegundos (10 segundos)
        delay(50);
        bot->sendMessage(msg.sender.id, "Elija a que hora quiere programar el riego", HoraKbd);
        Serial.println(" --- Esperando Respuesta Hora ---");

        while(!bot->getNewMessage(msg) && millis() - startTime < timeout){
          Serial.print(".");
          delay(500);
        }
        
        if(verifyHourFormat(msg.text)){
            this->deviceManager->UpdateLastInteraction();
            int hora = msg.text.substring(0, 2).toInt();
            Serial.print(" --- Llegamos al Numero:");Serial.print(hora); Serial.println(" --- ");
            newEvent.setHora(hora);
        }
        else{
          bot->sendMessage(msg.sender.id,"Perdon hubo un error con tu respuesta, Intentelo de nuevo :)");  
          this->deviceManager->UpdateLastInteraction();
          return;
        }
        delay(20);

        startTime = millis();
        delay(50);
        bot->sendMessage(msg.sender.id, "Elija en que minuto quiere programar el riego", MinKbd);
        Serial.println(" --- Esperando Respuesta Minuto ---");

        while(!bot->getNewMessage(msg) && millis() - startTime < timeout){
          Serial.print(".");
          delay(500);
        }
        if(verifyMinFormat(msg.text)){
            this->deviceManager->UpdateLastInteraction();
            int minuto = msg.text.substring(1, 3).toInt();
            Serial.print(" --- Llegamos al Numero:");Serial.print(minuto); Serial.println(" --- ");
            newEvent.setMinuto(minuto);
        }
        else{
          bot->sendMessage(msg.sender.id,"Perdon hubo un error con tu respuesta, Intentelo de nuevo :)"); 
          this->deviceManager->UpdateLastInteraction();
          return; 
        }
        
        delay(50);

        startTime = millis();
        delay(50);
        bot->sendMessage(msg.sender.id, "Elija el dia de la semana:", DiasKbd);
        Serial.println(" --- Esperando Respuesta Semana ---");
        while(!bot->getNewMessage(msg) && millis() - startTime < timeout){
          Serial.print(".");
          delay(500);
        }
        if(verifyWeekFormat(msg.text)){
            this->deviceManager->UpdateLastInteraction();
            Serial.print(" ---Dia de la semana:");Serial.print(msg.text); Serial.println(" --- ");
            newEvent.setDiaSemana(msg.text);
        }
        else{
          bot->sendMessage(msg.sender.id,"Perdon hubo un error con tu respuesta, Intentelo de nuevo :)");
          this->deviceManager->UpdateLastInteraction(); 
          return;
        }
        // LINEA CLAVE EN REVISION -- Arreglar la Memoria - Para despues...
        Serial.println(" --- Guardar Event ---");
        bool saveEvent = this->conf.setNewEvent(newEvent);
        Serial.println(" --- FIN Guardar Event ---");
        if(saveEvent){
          Serial.println(" --- Entre Al ---");
          this->conf.printState();
        }
        
        Serial.println(" --- SALIO PERFECTO ---");
        delay(300);
        bot->removeReplyKeyboard(msg.sender.id, "Felicitaciones, ya agregamos el evento a la maceta");
        
      }
// Eventos -> Eliminar
      if(msg.callbackQueryData.equals(ELIMINAR_EVENTO_CALLBACK)){

        CTBotReplyKeyboard DeleteKbd;
        

        for(int i=0;i<Configuracion::CANT_EVENTOS; i++){
          if(this->conf.isEventActive(i)){
            if( (i&7==0) && (i!= 0) ){
              DeleteKbd.addRow();
            }
            DeleteKbd.addButton("Evento " + String(i));
          }
        }
        DeleteKbd.enableOneTime();
        DeleteKbd.enableResize();
        unsigned long startTime;
        unsigned long timeout;
     
        startTime = millis();
        timeout = 30 * 5 *1000; // Tiempo de espera en milisegundos (10 segundos)
        delay(150);
        bot->sendMessage(msg.sender.id, "Elija que Evento quiere eliminar", DeleteKbd);
        this->deviceManager->UpdateLastInteraction();
        
        Serial.println(" --- Esperando Respuesta Eliminacion ---");

        while(!bot->getNewMessage(msg) && millis() - startTime < timeout){
          Serial.print(".");
          delay(500);
        }
        
        if(verifyEliminateFormat(msg.text)){
          this->deviceManager->UpdateLastInteraction();
          int num = getNumEvent(msg.text);
          if(conf.isEventActive(num)){
            conf.setEventActive(num,false);
          }
         
          bot->removeReplyKeyboard(msg.sender.id, "Evento eliminado satisfactoriamente :)");
          this->deviceManager->UpdateLastInteraction();
          conf.printState();
          delay(50);
          
        }
        else{
          bot->removeReplyKeyboard(msg.sender.id, "Lo sentimos, Ocurrion un error con la eliminacion. Intentelo de nuevo :(");
          return;
        } 
      }
// Eventos -> Activar Modo automatico
      if(msg.callbackQueryData.equals(ACTIVAR_EVENTO_CALLBACK)){
        conf.setIsAutoActive(true);
        bot->sendMessage(msg.sender.id, "Riego Automatico Activado :)");
        this->deviceManager->UpdateLastInteraction();
      }

// Eventos -> Desactivar Modo automatico
      if(msg.callbackQueryData.equals(DESACTIVAR_EVENTO_CALLBACK)){
        conf.setIsAutoActive(false);
        bot->sendMessage(msg.sender.id, "Riego Automatico Desactivado :)");
        this->deviceManager->UpdateLastInteraction();
      }
      
// Eventos -> Activar notificaciones
      if(msg.callbackQueryData.equals(ACTIVAR_NOTIFICACIONES_CALLBACK)){
        conf.setAreNotificationsOn(true);
        bot->sendMessage(msg.sender.id, "Notificaciones Activadas :)");
        this->deviceManager->UpdateLastInteraction();
      }
      
// Eventos -> Desactivar notificaciones
      if(msg.callbackQueryData.equals(DESACTIVAR_NOTIFICACIONES_CALLBACK)){
        conf.setAreNotificationsOn(false);
        bot->sendMessage(msg.sender.id, "Notificaciones Desactivadas :)");
        this->deviceManager->UpdateLastInteraction();
      }

    }// END_QUERY

    // LINEA PELIGROSA
    conf.saveInSystem();
  }

  Serial.println(" --- FIN Operacion pesada? ---");
}

void BotManager::buttonsSetup(){
  // AddEvent - ShowStatus - Show Events
  helloKbd.addButton("Regar",REGAR_CALLBACK,CTBotKeyboardButtonQuery);
  helloKbd.addRow();
  helloKbd.addButton("Configurar Eventos",CONF_EVENTOS_CALLBACK,CTBotKeyboardButtonQuery);
  helloKbd.addRow();
  helloKbd.addButton("Settings",SETTINGS_CALLBACK,CTBotKeyboardButtonQuery);

//  EventKbd.addButton("Agregar Evento",AGREGAR_EVENTO_CALLBACK,CTBotKeyboardButtonQuery);
//  EventKbd.addButton("Eliminar Evento",ELIMINAR_EVENTO_CALLBACK,CTBotKeyboardButtonQuery);


  HoraKbd.addButton("00:");
  HoraKbd.addButton("01:");
  HoraKbd.addButton("02:");
  HoraKbd.addButton("03:");
  HoraKbd.addButton("04:");
  HoraKbd.addButton("05:");
  HoraKbd.addButton("06:");
  HoraKbd.addButton("07:");
  HoraKbd.addRow();
  HoraKbd.addButton("08:");
  HoraKbd.addButton("09:");
  HoraKbd.addButton("10:");
  HoraKbd.addButton("11:");
  HoraKbd.addButton("12:");
  HoraKbd.addButton("13:");
  HoraKbd.addButton("14:");
  HoraKbd.addButton("15:");
  HoraKbd.addRow();
  HoraKbd.addButton("16:");
  HoraKbd.addButton("17:");
  HoraKbd.addButton("18:");
  HoraKbd.addButton("19:");
  HoraKbd.addButton("20:");
  HoraKbd.addButton("21:");
  HoraKbd.addButton("22:");
  HoraKbd.addButton("23:");
  HoraKbd.enableResize();
  HoraKbd.enableOneTime();

  MinKbd.addButton(":00");
  MinKbd.addButton(":15");
  MinKbd.addButton(":30");
  MinKbd.addButton(":45");
  MinKbd.enableOneTime();
  MinKbd.enableResize();

  DiasKbd.addButton("Lu");
  DiasKbd.addButton("Ma");
  DiasKbd.addButton("Mi");
  DiasKbd.addButton("Ju");
  DiasKbd.addButton("Vi");
  DiasKbd.addButton("Sa");
  DiasKbd.addButton("Do");
  DiasKbd.enableOneTime();
  DiasKbd.enableResize();
//  DiasEventKbd
 
}
