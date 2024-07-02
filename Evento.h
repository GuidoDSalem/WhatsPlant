// Evento.h
#ifndef EVENTO_H
#define EVENTO_H
#include<Arduino.h>
#include <ArduinoJson.h>
#include<time.h>


class Evento {
public:
  // Getters & Setters
  void setActive(bool isActive);
  void setMinuto(int minuto);
  void setHora(int hora);
  void setCantidad(int cant);
  void setDiaSemana(String& dia);
  void setDiaSemana(int i,bool b);


  Evento(); // Constructor
  Evento(int minuto,int hora,int cantidad);
  void printState();
  void setDias(char* diasSemana);
  String showEvent();
  long cuantoFalta();
  String serialize() const;
  void deserialize(const String& data);
  
  bool getIsActive();
  int getMinuto();
  int getHora();
  int getCantidad();
  bool getDiaSemana(int i);

private:
  bool isActive;
  int minuto;
  int hora;
  int cantidad;
  bool diasSemana[7];
};

#endif // EVENTO_H
