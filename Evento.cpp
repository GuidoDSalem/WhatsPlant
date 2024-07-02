// Evento.cpp
#include "Evento.h"
//#include "ESP32TIME_H"



Evento::Evento() {
  this->minuto = 0;
  this->hora = 0;
  this->cantidad = 0;
  this->isActive = false;
  for (int i = 0; i < 7; ++i){
    this->diasSemana[i] = false;
  }
}

Evento::Evento(int minuto, int hora, int cantidad){
  this->minuto = minuto;
  this->hora = hora;
  this->cantidad = cantidad;
  this->isActive = true;
  for (int i = 0; i < 7; ++i){
    this->diasSemana[i] = false;
  }
  this->diasSemana[5] = true;
}

int Evento::getMinuto(){
  return this->minuto;
  
}
int Evento::getHora(){
  return this->hora;
}
int Evento::getCantidad(){
  return this->cantidad;
}

void Evento::printState(){

  if(Serial){
    Serial.print("\t");Serial.print(isActive);Serial.print("\t-\t ");Serial.print(minuto);Serial.print("\t - \t");Serial.print(hora);Serial.print("\t - \t");Serial.print(cantidad);Serial.print("\t - \t");
    for(int i=0; i<7; i++){
      if(diasSemana[i]){
        Serial.print("o");
      }
      else{
        Serial.print("-");
      }
    } 
    Serial.println();
    
  }  
}

void Evento::setDias(char* diasSemana){
  for (int i = 0; diasSemana[i] != '\0'; i++) {
        switch (diasSemana[i]) {
            case 'L': this->diasSemana[0] = true; break;
            case 'M': this->diasSemana[1] = true; break;
            case 'X': this->diasSemana[2] = true; break;
            case 'J': this->diasSemana[3] = true; break;
            case 'V': this->diasSemana[4] = true; break;
            case 'S': this->diasSemana[5] = true; break;
            case 'D': this->diasSemana[6] = true; break;
        }
    }
}

String Evento::showEvent(){
  String event = String(this->hora) + ":" + String(this->minuto) + " ";
  for(int i=0; i<7; i++){
    if(this->diasSemana[i]){
      switch (i) {
        case 0: event += "Lu"; break;
        case 1: event += "Ma"; break;
        case 2: event += "Mi"; break;
        case 3: event += "Ju"; break;
        case 4: event += "Vi"; break;
        case 5: event += "Sa"; break;
        case 6: event += "Do"; break;
      }
    }
    
  }

  event += " " + String(this->cantidad) + "mL";

  return event;
  
}

// Cuantos segundos faltan para que se produzca el Evento
long Evento::cuantoFalta() {
    // Código para hacer algo
  if (!isActive) return 92233720368547758; // Indicar que no hay evento activo (numero alto)

  time_t ahora = time(NULL);
  struct tm tiempoLocal = *localtime(&ahora);

  int diaActual = tiempoLocal.tm_wday; // 0=Domingo, 6=Sábado (Orifginal
  
  int diasParaEvento = 0;
  // Encontrar el próximo día activo
  while (diasParaEvento < 7) {
    if (diasSemana[((tiempoLocal.tm_wday - 1) % 7 + diasParaEvento) % 7]) break;
    diasParaEvento++;
  }

  struct tm proximoEvento = tiempoLocal;
  proximoEvento.tm_hour = hora;
  proximoEvento.tm_min = minuto;
  proximoEvento.tm_sec = 0;
  // Ajustar la fecha al próximo evento
  proximoEvento.tm_mday += diasParaEvento;

  // Convertir tm a time_t para calcular la diferencia
  time_t tiempoEvento = mktime(&proximoEvento);
  long segundosHastaEvento = difftime(tiempoEvento, ahora);

  // Si el evento ya pasó en el día actual, calcular para la próxima semana
  if (segundosHastaEvento < 0) {

    
    
    proximoEvento.tm_mday += 7;
    tiempoEvento = mktime(&proximoEvento);
    segundosHastaEvento = difftime(tiempoEvento, ahora);
  }

  return segundosHastaEvento;
}




String Evento::serialize() const {
    StaticJsonDocument<256> doc;
    doc["isActive"] = isActive;
    doc["minuto"] = minuto;
    doc["hora"] = hora;
    doc["cantidad"] = cantidad;
    for (int i = 0; i < 7; ++i) {
        doc["diasSemana"][i] = diasSemana[i];
    }
    String output;
    serializeJson(doc, output);
    return output;
}

void Evento::deserialize(const String& data) {
    StaticJsonDocument<256> doc;
    deserializeJson(doc, data);

    isActive = doc["isActive"];
    minuto = doc["minuto"];
    hora = doc["hora"];
    cantidad = doc["cantidad"];
    for (int i = 0; i < 7; ++i) {
        diasSemana[i] = doc["diasSemana"][i];
    }
}
bool Evento::getIsActive(){
  return this->isActive;
}

void Evento::setActive(bool isActive){
  this->isActive = isActive;
}
void Evento::setMinuto(int minuto){
  this->minuto = minuto;
}
void Evento::setHora(int hora){
  this->hora = hora;  
}
void Evento::setCantidad(int cant){
  this->cantidad = cant;
}
void Evento::setDiaSemana(int i,bool b){
  this->diasSemana[i] = b;
  
}
void Evento::setDiaSemana(String& dia){
  if(dia == "Lu"){
    this->diasSemana[0] = true;
  }
  else if(dia == "Ma"){
    this->diasSemana[1] = true;
  }
  else if(dia == "Mi"){
    this->diasSemana[2] = true;
  }
  else if(dia == "Ju"){
    this->diasSemana[3] = true;
  }
  else if(dia == "Vi"){
    this->diasSemana[4] = true;
  }
  else if(dia == "Sa"){
    this->diasSemana[5] = true;
  }
  else if(dia == "Do"){
    this->diasSemana[6] = true;
  }
  else{
    // Do Nothing
  }
 
}

bool Evento::getDiaSemana(int i){
  return this->diasSemana[i];
}
