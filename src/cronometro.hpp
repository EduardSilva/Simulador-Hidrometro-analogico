#ifndef CRONOMETRO_H
#define CRONOMETRO_H

#include "Icomando.hpp"
#include <thread> 
#include <chrono>
#include <atomic>

class cronometro{
    private:
        Icomando * cmd;
        int intervalo_ms;
        double scala_simulador;
        std::thread worker_thread;
        std::atomic<bool> ativo;

        void loop();

    public:
        cronometro(Icomando* cmd, int intervalo, double scala);
        void iniciar();
        void parar();
        ~cronometro();


};



#endif