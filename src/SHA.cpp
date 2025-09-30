#include "SHA.hpp"
#include <iostream>

SHA::SHA(const std::string& caminho_config, int id_instancia) : m_id_instancia(id_instancia) {
    std::cout << "Inicializando a Aplicacao SHA (Instancia " << m_id_instancia << ")...\n";

    m_provedor = std::make_unique<provedorConfigArquivo>(caminho_config);
    m_gerador_imagem = std::make_unique<GeradorImagemPNG>(400, 400);
    
    configuracoes_t configs = m_provedor->enviar_configuracoes();

    m_nome_arquivo_imagem = "imagem_" + std::to_string(m_id_instancia) + ".png";
    m_controladora = std::make_unique<controladora>(*m_provedor, m_gerador_imagem.get(), &m_mutex, m_nome_arquivo_imagem, m_id_instancia);
    
    m_cronometro = std::make_unique<cronometro>(m_controladora.get(), configs.tempo, configs.scale);

    m_controladora->gerar_imagem();

    std::string titulo_janela = "Simulador de Hidrometro (SHA) - Instancia " + std::to_string(m_id_instancia);
    std::string caminho_temp = "temp/" + m_nome_arquivo_imagem;
    m_tela = std::make_unique<Tela>(400, 400, titulo_janela, caminho_temp, &m_mutex, this);
}

SHA::SHA(const configuracoes_t& config, int id_instancia) : m_id_instancia(id_instancia) {
    std::cout << "Inicializando a Aplicacao SHA (Instancia " << m_id_instancia << ")...\n";

    m_provedor = std::make_unique<provedorConfigDireto>(config);
    m_gerador_imagem = std::make_unique<GeradorImagemPNG>(400, 400);
    
    configuracoes_t configs = m_provedor->enviar_configuracoes();

    m_nome_arquivo_imagem = "imagem_" + std::to_string(m_id_instancia) + ".png";
    m_controladora = std::make_unique<controladora>(*m_provedor, m_gerador_imagem.get(), &m_mutex, m_nome_arquivo_imagem, m_id_instancia);
    
    m_cronometro = std::make_unique<cronometro>(m_controladora.get(), configs.tempo, configs.scale);

    m_controladora->gerar_imagem();

    std::string titulo_janela = "Simulador de Hidrometro (SHA) - Instancia " + std::to_string(m_id_instancia);
    std::string caminho_temp = "temp/" + m_nome_arquivo_imagem;
    m_tela = std::make_unique<Tela>(400, 400, titulo_janela, caminho_temp, &m_mutex, this);
}

SHA::~SHA() {
    std::cout << "Aplicacao SHA (Instancia " << m_id_instancia << ") encerrada. Recursos liberados.\n";
}

/// @brief Inicia a simulação completa
void SHA::run() {
    if (!m_cronometro || !m_tela) {
        std::cerr << "ERRO FATAL: Falha na inicializacao dos componentes da aplicacao.\n";
        return;
    }

    std::cout << "Iniciando a simulacao em segundo plano (Instancia " << m_id_instancia << ")...\n";
    m_cronometro->iniciar();
    
    std::cout << "Iniciando a interface grafica (Instancia " << m_id_instancia << ")...\n";
    m_tela->executar();

    std::cout << "Parando a simulacao em segundo plano (Instancia " << m_id_instancia << ")...\n";
    m_cronometro->parar();
}

/// @brief Inicia apenas o cronômetro em background
void SHA::iniciar() {
    if (!m_cronometro) {
        std::cerr << "ERRO FATAL: Cronometro nao inicializado (Instancia " << m_id_instancia << ").\n";
        return;
    }
    
    std::cout << "Iniciando simulacao em segundo plano (Instancia " << m_id_instancia << ")...\n";
    m_cronometro->iniciar();
}

/// @brief Mostra interface gráfica (método bloqueante)
void SHA::mostrar_interface() {
    if (!m_tela) {
        std::cerr << "ERRO FATAL: Tela nao inicializada (Instancia " << m_id_instancia << ").\n";
        return;
    }
    
    std::cout << "Mostrando interface grafica (Instancia " << m_id_instancia << ")...\n";
    m_tela->executar();
}

/// @brief Para o cronômetro
void SHA::parar() {
    if (!m_cronometro) {
        std::cerr << "ERRO FATAL: Cronometro nao inicializado (Instancia " << m_id_instancia << ").\n";
        return;
    }
    
    std::cout << "Parando simulacao (Instancia " << m_id_instancia << ")...\n";
    m_cronometro->parar();
}
void SHA::aumentar_vazao_entrada(double incremento) {
    if (m_controladora) {
        m_controladora->aumentar_vazao_entrada(incremento);
    }
}

void SHA::diminuir_vazao_entrada(double decremento) {
    if (m_controladora) {
        m_controladora->diminuir_vazao_entrada(decremento);
    }
}

double SHA::get_vazao_entrada() const {
    if (m_controladora) {
        return m_controladora->get_vazao_entrada();
    }
    return 0.0;
}
