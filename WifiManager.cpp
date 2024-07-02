#include"WifiManager.h"
//#include"Configuracion.h"


// Constructor con configuración
WifiManager::WifiManager(Configuracion& conf,WebServer* server) : Manager(conf), server(server) {
    // Inicialización adicional si es necesaria
}

//         PUBLIC FUNCTIONS

void WifiManager::handleSetup(){
  
  if(!conf.getIsValid()){
    handleConfiguration();
  }
  connectWifi();
}

void WifiManager::handleRutineJob(){
  if(!conf.getIsValid()){
    server->handleClient();
    handleConfiguration();
    return;
  }
  else{
    connectWifi();
  } 

  if(conf.isConnected()){
    Serial.println(" --- Wifi is Connected --- ");
  }
  else{
    Serial.println(" --- Wifi is Unavailable --- ");
    delay(100);
  }
}

//         PRIVATE FUNCTIONS

bool WifiManager::isConnected(){
  return WiFi.status() == WL_CONNECTED;
}

void WifiManager::connectWifi(){

  //////// TEMPORAL FOR DEBBUGING ////////
//  conf.setSsid("Fibertel WiFi154 2.4G");
//  conf.setPassword("salem12345");
  conf.setSsid("ALUMNOSUTDT");
  conf.setPassword("Alumnos!Ditella");
  ////////////////////////////////////////
  
  // Si ya estas conectado a internet terminar la funcion
  if(conf.isConnected()){
    return;
  }
  if(!this->conf.getIsValid()){
    handleConfiguration();
  }
  // Intentar conectar al WiFi con las credenciales proporcionadas
  WiFi.begin(this->conf.getSsid().c_str(), this->conf.getPassword().c_str() );
  Serial.print("Conectando a WiFi...");

  // Esperar a que se conecte al WiFi
  unsigned long startTime = millis();
  while ( !isConnected() && millis() - startTime < Configuracion::CONNECT_DEADLINE ) { //  60segundos de tiempo de espera
    delay(500);
    Serial.print(".");
  }
  if (isConnected()) { // Se logro la conexion
    // Conexión exitosa
    Serial.println("\nConectado al WiFi.");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());
    // Desconectar el punto de acceso y cerrar el servidor para liberar recursos 
    // TODO: verificar si efectivamente el esp32 tiene el wifi local activo
    //WiFi.softAPdisconnect(true); // Que hace esto??

  } else { // Falló la conexión
    Serial.println("\nFallo en la conexión al WiFi.");
    showDisconnectionStatus();
    // Indicar al usuario que no se puedo conectar a internet
    showDisconnectionStatus();
  }
  
}

void WifiManager::handleReconnection(){
  connectWifi();
}

void WifiManager::showDisconnectionStatus(){
  // TODO
}


String WifiManager::generateWifiHTML() {

  String html = "<html>"
                "<head>"
                  "<title>Configuracion WiFi</title>"
                  "<script>"
                    "function validarFormulario() {"
                      "var ssid = document.forms['formularioWifi']['ssid'].value;"
                      "var password = document.forms['formularioWifi']['password'].value;"
                      "var phoneNumber = document.forms['formularioWifi']['phoneNumber'].value;"
                      "if (ssid == '') {"
                        "alert('Por favor, seleccione un SSID.');"
                        "return false;"
                      "}"
                      "if (password.length < 8) {"
                        "alert('La contraseña debe tener al menos 8 caracteres.');"
                        "return false;"
                      "}"
//                      "if (phoneNumber.length < 10 || isNaN(phoneNumber)) {"
//                        "alert('Ingrese un número de teléfono válido de al menos 10 dígitos.');"
//                        "return false;"
//                      "}"
                      "return true;"
                    "}"
                  "</script>"
                "</head>"
                "<body>"
                  "<h1>Configuracion WiFi ESP32</h1>"
                  "<form name='formularioWifi' action='/wifi_config' method='POST' onsubmit='return validarFormulario()'>"
                    "SSID:<br>"
                    "<select name='ssid'>";

  // Inicia el escaneo de redes WiFi de forma asíncrona
  WiFi.scanNetworks(true);
  // Espera a que el escaneo de redes se complete
  while (WiFi.scanComplete() < 0) {
    delay(10); // Pequeña pausa para evitar bloquear el CPU completamente
  }

  // Obtiene el número de redes encontradas
  int n = WiFi.scanComplete();
  // Genera las opciones del select para cada red encontrada
  for (int i = 0; i < n; i++) {
    html += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + "</option>";
  }

  html += "</select><br>Password:<br>"
          "<input type='password' name='password'><br><br>"
//          "Número de teléfono:<br>"
//          "<input type='text' name='phoneNumber'><br><br>"
          "<input type='submit' value='Conectar'>"
          "</form>"
          "</body>"
          "</html>";
  return html;
}

String WifiManager::generateTelegramHTML(){
    String html = "<html>"
                  "<head>"
                  "<title>Configurar Bot de Telegram</title>"
                  "</head>"
                  "<body>"
                  "<h1>Configuración del Bot de Telegram</h1>"
                  "<p>Sigue estos pasos para configurar tu bot en Telegram:</p>"
                  "<ol>"
                  "<li>Abre Telegram y busca a <a href='https://t.me/BotFather' target='_blank'>@BotFather</a>.</li>"
                  "<li>Envía el comando <code>/newbot</code> y sigue las instrucciones de @BotFather para crear un nuevo bot.</li>"
                  "<li>Una vez creado el bot, @BotFather te proporcionará un <b>token</b>.</li>"
                  "</ol>"
                  "<p>Ingresa el token de tu bot en el formulario a continuación:</p>"
                  "<form action='/telegram_config_post' method='POST'>"
                  "Token del Bot:<br>"
                  "<input type='text' name='token'><br><br>"
                  "<input type='submit' value='Guardar Configuración'>"
                  "</form>"
                  "</body>"
                  "</html>";

    return html;
}


void WifiManager::handleGetRoot(){
  Serial.println(" --- Entre a handle Root ---");
  this->server->send(200, "text/html", generateWifiHTML());
}

void WifiManager::handlePostConf(){ // <- Revisar esto
  Serial.println(" --- seteando Wifi --- ");
  String ssid = server->arg("ssid");
  String password = server->arg("password");
//  String phoneNumber = server->arg("phoneNumber");

  // Intentar conectar al WiFi con las credenciales proporcionadas
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.print("Conectando a WiFi: ");Serial.print(ssid);Serial.print(" - ");Serial.println(password);

  // Esperar a que se conecte al WiFi
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 150000) { // 150 segundos de tiempo de espera
    delay(1200);
    Serial.print(".");
  }

  if (conf.isConnected()) {
    // Conexión exitosa
    Serial.println("\n --- Conectado al WiFi --- ");
    Serial.print(" --- Dirección IP: ");Serial.print(WiFi.localIP()); Serial.println(" --- ");

    conf.setSsid(ssid);
    conf.setPassword(password);
//    conf.setPhoneNumber(phoneNumber);
    
    server->send(200,"text/html",generateTelegramHTML());
  } else {
    // Falló la conexión
    Serial.println("\nFallo en la conexión al WiFi.");
    this->server->send(200, "text/html", generateWifiHTML());
  }
  
}

void WifiManager::handleGetTelegram(){
  this->server->send(200, "text/html", generateTelegramHTML());
}

void WifiManager::handlePostTelegram(){
  Serial.println(" --- POST TELEGRAM ---");
  String token = server->arg("token");

  CTBot temporalBot = CTBot();
  temporalBot.setTelegramToken(token);

  unsigned long startTime = millis();
  while (!temporalBot.testConnection() && millis() - startTime < 150000) { // 150 segundos de tiempo de espera
    delay(500);
    Serial.print(".");
  }
  // check if all things are ok
  if (temporalBot.testConnection()){
    Serial.println("\n --- Bot connected Succesfully --- ");
    conf.setTelegramToken(token);

    userIsWorking = false;
    conf.setIsValid(true);
    
    conf.printState();
//    conf.saveInSystem();
  
    this->server->send(200, "text/plain", "Configuracion Termianda :)");
    delay(1000);
    
    WiFi.softAPdisconnect(true);
    server->close();

  delay(100);
    
  } 
  else{
    Serial.println(" --- Bot Connection failed --- ");
    server->send(200,"text/html",generateTelegramHTML());
  }
  
  
}

void WifiManager::handleConfiguration(){
  
  delay(100);

  if(userIsWorking){
    return;
  }
  
  if(conf.getIsValid()){
    return;
  }
  Serial.println(" --- Handle Configuracion Wifi --- ");
  
  // Configura el ESP32 como un punto de acceso
  WiFi.softAP(ESP_SSID, ESP_PASSWORD);//
  Serial.print("Punto de acces/o iniciado es: ");Serial.println(ESP_SSID);
  Serial.print("IP del punto de acceso: ");
  Serial.println(WiFi.softAPIP());

  // TODO: Mostrar qr con link al ip/root - http://192.168.4.1/

  // Manejadores de rutas para el servidor web
  server->on("/", HTTP_GET, [this]() { this->handleGetRoot(); });
  server->on("/wifi_config", HTTP_POST, [this]() { this->handlePostConf(); });
  server->on("/telegram_config", HTTP_GET, [this]() {this->handleGetTelegram(); });
  server->on("/telegram_config_post", HTTP_POST, [this]() {this->handlePostTelegram(); });

  this->server->begin();
//  CAPITAL
//  conf.setSsid("Fibertel WiFi154 2.4G");
//  conf.setPassword("salem12345");
//  COUNTRY
//  conf.setSsid("Fibertel WiFi995 2.4GHz");
//  conf.setPassword("0131026724");
  // DITELLA
  conf.setSsid("ALUMNOSUTDT");
  conf.setPassword("Alumnos!Ditella");

  conf.setTelegramToken("6916231665:AAHFLTSEfM9-1yq_1KqPXKixwF-A4yRP5ZY");
  conf.setIsValid(true);

  Evento event = Evento(15, 11, 100);
  conf.setNewEvent(event);
  event = Evento(30, 12, 200);
  conf.setNewEvent(event);
  event = Evento(45, 13, 300);
  conf.setNewEvent(event);

  conf.printState();

  userIsWorking = true;

  delay(200);

}
