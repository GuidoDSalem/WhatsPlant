#include "Manager.h"

Manager::Manager(Configuracion& conf): conf(conf){
    // Puedes añadir código de inicialización aquí si es necesario
    this->isReady = true;
}


bool Manager::isManagerReady(){
  return this->isReady;
}
