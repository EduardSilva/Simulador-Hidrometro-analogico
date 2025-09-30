#include "Icomando.hpp"
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <sstream>
#include <cmath>

/// @brief Cria um simulador usando a classe fabrica
/// @param prov Provedor de configurações para o simulador
void controladora::criar_simulador(const IprovedorConfig & prov){
    this->sim = fabrica::criar_simulador(prov);
};

/// @brief Executa a atualização do simulador e gera imagem
/// @param sec Segundos de simulação
/// @param scale Escala de tempo
void controladora::executar(double sec, double scale) {
    this->sim->atualizar_info(sec, scale);
    this->gerar_imagem();
}

/// @brief Imprime dados do simulador no terminal
/// @param sim Simulador de hidrômetro
void imprimir_status(const simuladorHidrometro& sim) {
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "  Consumo Total Acumulado: " << sim.get_consumo_total_m3() << " m3\n";
    std::cout << "  Ponteiro Centenas de Litros: " << sim.get_consumo_centena_litro() << "\n";
    std::cout << "  Ponteiro Dezenas de Litros:  " << sim.get_consumo_dezena_litro() << "\n";
    std::cout << "  Ponteiro de Litros:  " << sim.get_consumo_litro() << "\n\n";
}

void controladora::gerar_imagem(){
    std::lock_guard<std::mutex> lock(*m_mutex);
    imprimir_status(*this->sim);
    
    std::filesystem::create_directories("temp");
    
    std::string temp_filename = "temp/" + m_nome_arquivo;
    this->ger_imagem->gerar(*this->sim, temp_filename);
    
    verificar_e_salvar_m3();
}

controladora::~controladora(){
    delete this->sim;
}
controladora::controladora(const IprovedorConfig & prov, GeradorImagemPNG * gerador_png, std::mutex* mutex, const std::string& nome_arquivo, int id_instancia){
    this->criar_simulador(prov);
    this->ger_imagem =  gerador_png;
    this->m_mutex = mutex;
    this->m_nome_arquivo = nome_arquivo;
    this->m_id_instancia = id_instancia;
    this->m_ultimo_m3_salvo = 0.0;
}

// Controle dinâmico de vazão
void controladora::aumentar_vazao_entrada(double incremento) {
    if (this->sim) {
        this->sim->aumentar_vazao_entrada(incremento);
    }
}

void controladora::diminuir_vazao_entrada(double decremento) {
    if (this->sim) {
        this->sim->diminuir_vazao_entrada(decremento);
    }
}

double controladora::get_vazao_entrada() const {
    if (this->sim) {
        return this->sim->get_vazao_entrada();
    }
    return 0.0;
}

void controladora::verificar_e_salvar_m3() {
    if (!this->sim) return;
    
    double m3_atual = this->sim->get_consumo_total_m3();
    int m3_completos = static_cast<int>(std::floor(m3_atual));
    int m3_salvos = static_cast<int>(std::floor(m_ultimo_m3_salvo));
    
    if (m3_completos > m3_salvos) {
        std::string diretorio_principal = "Medições_202311250036";
        std::string diretorio_instancia = diretorio_principal + "/Instancia_" + 
                                        std::to_string(m_id_instancia).insert(0, 2 - std::to_string(m_id_instancia).length(), '0');
        
        try {
            std::filesystem::create_directories(diretorio_instancia);
            
            int numero_arquivo = (m3_completos % 100);
            if (numero_arquivo == 0) numero_arquivo = 100;
            
            std::ostringstream nome_arquivo;
            nome_arquivo << diretorio_instancia << "/" 
                        << std::setw(2) << std::setfill('0') << numero_arquivo 
                        << ".jpeg";
            
            this->ger_imagem->gerar_jpeg(*this->sim, nome_arquivo.str());
            
            std::cout << "Instancia " << m_id_instancia << ": m³ " << m3_completos 
                     << " completado! Salvo em: " << nome_arquivo.str() << std::endl;
            
            m_ultimo_m3_salvo = m3_atual;
            
        } catch (const std::exception& e) {
            std::cerr << "Erro ao salvar JPEG para instancia " << m_id_instancia 
                     << ": " << e.what() << std::endl;
        }
    }
}

