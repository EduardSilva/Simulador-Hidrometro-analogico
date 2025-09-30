#include "tela.hpp"
#include "SHA.hpp"
#include <iostream>

Tela::Tela(unsigned int largura, unsigned int altura, const std::string& titulo, const std::string& caminho_imagem, std::mutex* mutex, SHA* sha)
    : m_janela(sf::VideoMode({largura, altura}), titulo),
      m_caminho_imagem(caminho_imagem),
      m_mutex(mutex),
      m_sha(sha)
{
    m_janela.setFramerateLimit(60);

    std::lock_guard<std::mutex> lock(*m_mutex);
    
    if (!m_textura.loadFromFile(m_caminho_imagem)) {
        std::cerr << "AVISO: Nao foi possivel carregar o frame inicial da imagem '" << m_caminho_imagem << "'. Sera criada durante execucao.\n";
        if (!m_textura.create(400, 400)) {
            std::cerr << "ERRO FATAL: Nao foi possivel criar textura vazia.\n";
        }
    }

    m_sprite = std::make_unique<sf::Sprite>(m_textura);
}

/// @brief Mantém a janela sendo renderizada enquanto não for fechada
void Tela::executar() {
    while (m_janela.isOpen()) {
        processarEventos();
        renderizar();
    }
}

/// @brief Processa os eventos na janela, incluindo controle de vazão por teclado
void Tela::processarEventos() {
    sf::Event event;
    while (m_janela.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_janela.close();
        }
        else if (event.type == sf::Event::KeyPressed && m_sha) {
            switch (event.key.code) {
                case sf::Keyboard::Up:
                case sf::Keyboard::Add:
                    m_sha->aumentar_vazao_entrada(0.1);
                    std::cout << "Vazao aumentada para: " << m_sha->get_vazao_entrada() << std::endl;
                    break;
                case sf::Keyboard::Down:
                case sf::Keyboard::Subtract:
                    m_sha->diminuir_vazao_entrada(0.1);
                    std::cout << "Vazao diminuida para: " << m_sha->get_vazao_entrada() << std::endl;
                    break;
                default:
                    break;
            }
        }
    }
}
/// @brief Atualiza a janela com a imagem do simulador
void Tela::renderizar() {
    if (!m_mutex || !m_sprite) return;

    {
        std::lock_guard<std::mutex> lock(*m_mutex);
        m_textura.loadFromFile(m_caminho_imagem); 
    }
    
    m_janela.clear(sf::Color(220, 220, 220));
    m_janela.draw(*m_sprite); 
    m_janela.display();
}