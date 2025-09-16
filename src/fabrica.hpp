#ifndef FABRICA_H
#define FABRICA_H

#include "IprovedorConfig.hpp"
#include "simuladorHidrometro.hpp"
class fabrica{
    public:
        static simuladorHidrometro * criar_simulador(const IprovedorConfig& prov);

};



#endif