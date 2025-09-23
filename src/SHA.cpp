#include "SHA.hpp"
#include <iostream>

SHA::SHA(const std::string& caminho_config) {
    std::cout << "Inicializando a Aplicacao SHA...\n";

    m_provedor = std::make_unique<provedorConfigArquivo>(caminho_config);
    m_gerador_imagem = std::make_unique<GeradorImagemPNG>(400, 400);
    
     configuracoes_t configs = m_provedor->enviar_configuracoes();

    m_controladora = std::make_unique<controladora>(*m_provedor, m_gerador_imagem.get(), &m_mutex);
    
    m_cronometro = std::make_unique<cronometro>(m_controladora.get(), configs.tempo, configs.scale);

    m_controladora->gerar_imagem();

    const std::string NOME_FICHEIRO_FRAME = "imagem.png";
    m_tela = std::make_unique<Tela>(400, 400, "Simulador de Hidrometro (SHA)", NOME_FICHEIRO_FRAME, &m_mutex);
}

SHA::~SHA() {
    std::cout << "Aplicacao SHA encerrada. Recursos liberados.\n";
}

/// @brief Inicia a simlação, 
void SHA::run() {
    if (!m_cronometro || !m_tela) {
        std::cerr << "ERRO FATAL: Falha na inicializacao dos componentes da aplicacao.\n";
        return;
    }

    std::cout << "Iniciando a simulacao em segundo plano...\n";
    m_cronometro->iniciar();
    
    std::cout << "Iniciando a interface grafica...\n";
    m_tela->executar();

    std::cout << "Parando a simulacao em segundo plano...\n";
    m_cronometro->parar();
}
