#include "Configuracion.h"

//const int Configuracion::CANT_EVENTOS = 14;

Configuracion::Configuracion(){

  this->ssid = "";
  this->password = "";
  this->phoneNumber = "";
  this->telegramToken = "";
  this->senderId = 0;
  this->isValid = false;
  this->isAutoActive = true;
  this->areNotificationsOn = true;
  for(int i=0; i<CANT_EVENTOS; i++){
    this->eventos[i] = Evento();
  }
}

void Configuracion::setSsid(const String& newSsid) {
    ssid = newSsid;
}

void Configuracion::setPassword(const String& newPassword) {
    password = newPassword;
}

void Configuracion::setPhoneNumber(const String& newPhoneNumber) {
    phoneNumber = newPhoneNumber;
}

void Configuracion::setTelegramToken(const String& newToken){
    telegramToken = newToken;
}

void Configuracion::setSenderId(const int64_t id){
  senderId = id;
}

void Configuracion::setIsValid(const bool& newIsValid) {
  isValid = newIsValid;
}

void Configuracion::setIsAutoActive(const bool b){
  isAutoActive = b;
}

void Configuracion::setAreNotificationsOn(const bool b){
  Serial.print(" --- Conf.areNotificationsOn set to  ---");Serial.print(b);Serial.println(" ---");
  areNotificationsOn = b;
}

String Configuracion::getSsid() const {
    return ssid;
}

String Configuracion::getPassword() const {
    return password;
}

String Configuracion::getPhoneNumber() const {
    return phoneNumber;
}

String Configuracion::getTelegramToken() const {
    return telegramToken;
}

int64_t Configuracion::getSenderId() const {
  return senderId;
}

Evento* Configuracion::getEvento(int i){
  return &(this->eventos[i]);
}

bool Configuracion::getIsValid() const{
  return isValid;
}

bool Configuracion::getIsAutoActive() const{
  return isAutoActive;
}

bool Configuracion::isEventActive(int i){
  return this->eventos[i].getIsActive();
}

bool Configuracion::getAreNotificationsOn() const{
  return this->areNotificationsOn;
}

String Configuracion::eventKey(int i){
  return "evento" + String(i);
}

void Configuracion::saveInSystemWC(){
    Preferences preferences;
    preferences.begin("my-app", false); // Open Preferences with my-app namespace. RW-mode (second parameter) is false.

    // Save SSID
    preferences.putString("ssid", ssid);
    
    // Save Password
    preferences.putString("password", password);

    // Save PhoneNumber
    preferences.putString("phoneNumber", phoneNumber);

    // Save TelegramToken
    preferences.putString("telegramToken", telegramToken);

    // Save SenderId
    preferences.putLong64("senderId", senderId);
    
    // Save IsValid
    preferences.putBool("isValid", isValid);

    // Save IsAutoActive areNotificationsOn
    preferences.putBool("isAutoActive", isAutoActive);

    // Save areNotificationsOn
    preferences.putBool("notifications", areNotificationsOn);    

    // Save Eventos (assuming each Evento can be represented as a String or you have a method to serialize it)
    for (int i = 0; i < CANT_EVENTOS; i++) {
        String key = eventKey(i);
        String value = eventos[i].serialize();
        preferences.putString(key.c_str(), value);
    }

    preferences.end(); // Close the Preferences

    Serial.println(" --- Configuration saved in System --- ");

}

void Configuracion::saveInSystem() {
    bool realizeCambios = false;
    Preferences preferences;
    preferences.begin("my-app", false); // Open Preferences with my-app namespace. RW-mode (second parameter) is false.

    // Save SSID
    if(this->ssid != preferences.getString("ssid")){
      preferences.putString("ssid", ssid);
      realizeCambios = true;
    }
    
    // Save Password
    if(this->password != preferences.getString("password")){
      preferences.putString("password", password);
      realizeCambios = true;
    }
    

    // Save PhoneNumber
    if(this->phoneNumber != preferences.getString("phoneNumber")){
      preferences.putString("phoneNumber", phoneNumber);
      realizeCambios = true;
    }

    // Save TelegramToken
    if(this->telegramToken != preferences.getString("telegramToken")){
      preferences.putString("telegramToken", telegramToken);
      realizeCambios = true;
    }

    // Save SenderId
    if(this->senderId != preferences.getLong64("senderId")){
      preferences.putLong64("senderId", senderId);
      realizeCambios = true;
    }
     
    // Save IsValid
    preferences.putBool("isValid", isValid);

    // Save IsAutoActive
    if(this->isAutoActive != preferences.getBool("isAutoActive")){
      preferences.putBool("isAutoActive", isAutoActive);
      realizeCambios = true;
    }

    // Save areNotificationsOn
    Serial.print("Conf.notification: ");Serial.print(areNotificationsOn);Serial.print("  |  ");Serial.print("Memory notification: ");Serial.println(preferences.getBool("notifications"));
    if(this->areNotificationsOn != preferences.getBool("notifications")){
      preferences.putBool("notifications", areNotificationsOn);
      delay(100);
      Serial.print("Conf.notification: ");Serial.print(areNotificationsOn);Serial.print("  |  ");Serial.print("Memory notification: ");Serial.println(preferences.getBool("notifications"));
      realizeCambios = true;
    }

    // Save Eventos (assuming each Evento can be represented as a String or you have a method to serialize it)
    for (int i = 0; i < CANT_EVENTOS; i++) {
        String key = eventKey(i);
        String value = eventos[i].serialize();
        if(value != preferences.getString(key.c_str())){
          
          preferences.putString(key.c_str(), value);
          realizeCambios = true;
        }
    }

    preferences.end(); // Close the Preferences

    if(realizeCambios){
      Serial.println("\n --- SE REALIZARON CAMBIOS EN MEMORIA --- \n");
    }

    if(Serial){
      Serial.println(" --- Configuration saved in System --- ");
    }
}

Configuracion Configuracion::getFromSystem() {
    Configuracion configuracion;
    Preferences preferences;
    preferences.begin("my-app", true); // Open Preferences with my-app namespace. ReadOnly mode is true.

    // Load SSID
    configuracion.ssid = preferences.getString("ssid");

    // Load Password
    configuracion.password = preferences.getString("password");

    // Load PhoneNumber
    configuracion.phoneNumber = preferences.getString("phoneNumber");

    // Load TelegramToken
    configuracion.telegramToken = preferences.getString("telegramToken");

    // Loas SenderId
    configuracion.senderId = preferences.getLong64("senderId");

    // Load isValid
    configuracion.isValid = preferences.getBool("isValid");

    // Load isAutoActive areNotificationsOn
    configuracion.isAutoActive = preferences.getBool("isAutoActive");

    // Load areNotificationsOn
    configuracion.areNotificationsOn = preferences.getBool("notifications");

    // Load Eventos
    for (int i = 0; i < CANT_EVENTOS; i++) {
        String key = "evento" + String(i);
        String value = preferences.getString(key.c_str());
        configuracion.eventos[i].deserialize(value); 
    }

    preferences.end(); // Close the Preferences
    return configuracion;
}



void Configuracion::printState() {
  if(Serial){
    Serial.println("\n ----------------------------- Configuracion State ----------------------------- ");
    Serial.print(" ssid: ");Serial.println(this->ssid);
    Serial.print(" password: ");Serial.println(this->password);
    Serial.print(" phoneNumber: ");Serial.println(this->phoneNumber);
    Serial.print(" telegramToken: ");Serial.println(this->telegramToken);
    Serial.print(" senderId: ");Serial.println(this->senderId);
    Serial.print(" isValid: ");Serial.println(this->isValid);
    Serial.print(" isAutoActive: ");Serial.println(this->isAutoActive);
    Serial.print(" areNotificationsOn: ");Serial.println(this->areNotificationsOn);
    Serial.println("\tisActive \t-\t minuto \t-\t hora     -    cantidad   -   diasSemana");

    for(int i=0; i < CANT_EVENTOS /*- 14 + 5*/; i++){
      if(this->eventos[i].getIsActive()){
        Serial.print(" Evento "); Serial.print(i+1);Serial.print(": ");
        this->eventos[i].printState();
      }  
    }
    Serial.println("\n ----------------------------- END Configuracion State ----------------------------- ");
    Serial.println("\n\n");
    

//    Evento eventos[CANT_EVENTOS];
  } else{
    Serial.println(" --- Configuracion State Error --- ");
  }
}  

bool Configuracion::isConnected(){
  return WiFi.status() == WL_CONNECTED;
}
// Implementaciones de métodos para manejar eventos
// ...

String Configuracion::showActiveEvents(){
  String result;
  for(int i=0; i<NUM_EVENTOS; i++){
    if(eventos[i].getIsActive()){
      result += "Evento "+String(i)+": "+eventos[i].showEvent()+"\n";
    }
  }
  return result;
}

int Configuracion::getNumActiveEvents(){
  int cantEventos = 0;
  for(int i=0; i<CANT_EVENTOS; i++){
    if(this->eventos[i].getIsActive()){
      cantEventos++;
    }
  }
  Serial.println(" --- Cant de Eventos Activos: " + cantEventos);
  return cantEventos;
}

bool Configuracion::setNewEvent(Evento& event){
  int i=0;
  while(i<CANT_EVENTOS && this->eventos[i].getIsActive()){
    i++;
  }
  Serial.print("El indice donde se colocara el Evento es:");Serial.println(i);
  if(i == CANT_EVENTOS){
    Serial.println(" --- No hay lugar para guardar el Evento :( --- ");
    return false;
  }
  else{
    Serial.println(" --- Seteando el nuevo Evento ---");
    this->eventos[i].setActive(event.getIsActive());
    this->eventos[i].setMinuto(event.getMinuto());
    this->eventos[i].setHora(event.getHora());
    this->eventos[i].setCantidad(event.getCantidad());
    for(int j=0; j<7; j++){
      this->eventos[i].setDiaSemana(j,event.getDiaSemana(j));//
    }
    Serial.println(" --- FIN Seteando el nuevo Evento ---");
    delay(500);
    return true;
  }
}

void Configuracion::setEventActive(int i,bool b){
    int k=0;
    for(int j=0;j<CANT_EVENTOS; j++){
      if(k == i){
        this->eventos[j].setActive(b);
        return;
      }
      if(this->eventos[j].getIsActive()){
        k++;
      }
      
    }

  
}
