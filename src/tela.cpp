#include "tela.hpp"
#include <iostream>

Tela::Tela(unsigned int largura, unsigned int altura, const std::string& titulo, const std::string& caminho_imagem, std::mutex* mutex)
    : m_janela(sf::VideoMode({largura, altura}), titulo),
      m_caminho_imagem(caminho_imagem),
      m_mutex(mutex)
{
    m_janela.setFramerateLimit(60);

    // 1. Trava o mutex para garantir a leitura segura do ficheiro inicial.
    std::lock_guard<std::mutex> lock(*m_mutex);
    
    // 2. Carrega a textura. O 'main' já garantiu que o ficheiro existe.
    if (!m_textura.loadFromFile(m_caminho_imagem)) {
        std::cerr << "ERRO FATAL: Nao foi possivel carregar o frame inicial da imagem '" << m_caminho_imagem << "'.\n";
    }

    // 3. AGORA, com a textura carregada, criamos o sprite.
    m_sprite = std::make_unique<sf::Sprite>(m_textura);
}

void Tela::executar() {
    while (m_janela.isOpen()) {
        processarEventos();
        renderizar();
    }
}

void Tela::processarEventos() {
    while (const auto event = m_janela.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_janela.close();
        }
    }
}

void Tela::renderizar() {
    if (!m_mutex || !m_sprite) return;

    // Trava o mutex para proteger a leitura do ficheiro
    {
        std::lock_guard<std::mutex> lock(*m_mutex);
        m_textura.loadFromFile(m_caminho_imagem); 
    }
    
    m_janela.clear(sf::Color(220, 220, 220));
    m_janela.draw(*m_sprite); 
    m_janela.display();
}