#include "cronometro.hpp"

cronometro::cronometro(Icomando* cmd, int intervalo_em_ms, double escala)
    : cmd(cmd),
      intervalo_ms(intervalo_em_ms),
      scala_simulador(escala),
      ativo(false) // O cronômetro começa desligado.
{}


cronometro::~cronometro() {
    parar();
}
/// @brief inicia o funcionamento do cronometro
void cronometro::iniciar() {
    if (ativo) return; // Se já estiver ativo, não faz nada.

    ativo = true;
    worker_thread = std::thread(&cronometro::loop, this);
}

/// @brief Para a execução em segundo plano de forma segura
void cronometro::parar() {
    ativo = false; 
    if (worker_thread.joinable()) {
        worker_thread.join();
    }
}

/// @brief função responsável por executar o Icomando de x em x ficando em sleep durante o periodo inativo
void cronometro::loop() {

    double intervalo_sec = static_cast<double>(intervalo_ms) / 1000.0;


    while (this->ativo) {

        if (this->cmd) {

            cmd->executar(intervalo_sec, scala_simulador);
        }


        std::this_thread::sleep_for(std::chrono::milliseconds(intervalo_ms));
    }
}