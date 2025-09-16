#include "Icomando.hpp"
#include <iostream>
#include <iomanip>
/// @brief Cria um simulador usando a classe fabrica e 
/// @param prov um provedor deconfigurações, cujo será usado para receber as configurações do simulador
void controladora::criar_simulador(const IprovedorConfig & prov){
    this->sim = fabrica::criar_simulador(prov);
};

void controladora::executar(double sec, double scale) {
    this->sim->atualizar_info(sec, scale);
    this->gerar_imagem();
}
void imprimir_status(const simuladorHidrometro& sim) {
    std::cout << std::fixed << std::setprecision(4); // Formata para 4 casas decimais
    std::cout << "  Consumo Total Acumulado: " << sim.get_consumo_total_m3() << " m3\n";
    std::cout << "  Ponteiro Centenas de Litros: " << sim.get_consumo_centena_litro() << "\n";
    std::cout << "  Ponteiro Dezenas de Litros:  " << sim.get_consumo_dezena_litro() << "\n";
    std::cout << "  Ponteiro de Litros:  " << sim.get_consumo_litro() << "\n\n";
}

void controladora::gerar_imagem(){
    std::lock_guard<std::mutex> lock(*m_mutex);
    imprimir_status(*this->sim);
    this->ger_imagem->gerar(*this->sim, "imagem.png");
}

controladora::~controladora(){
    delete this->sim;
}
controladora::controladora(const IprovedorConfig & prov, GeradorImagemPNG * gerador_png, std::mutex* mutex){
    this->criar_simulador(prov);
    this->ger_imagem =  gerador_png;
    this->m_mutex = mutex;
}

