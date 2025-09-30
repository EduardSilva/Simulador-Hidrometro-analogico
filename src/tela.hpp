#ifndef TELA_HPP
#define TELA_HPP

#include <string>
#include <SFML/Graphics.hpp>
#include <mutex>
#include <memory> 

// Forward declaration
class SHA;

class Tela {
private:
    sf::RenderWindow m_janela;
    sf::Texture m_textura;
    std::unique_ptr<sf::Sprite> m_sprite; 
    std::string m_caminho_imagem;
    std::mutex* m_mutex;
    SHA* m_sha; // Referência para controle de vazão

    void processarEventos();
    void renderizar();

public:
    Tela(unsigned int largura, unsigned int altura, const std::string& titulo, const std::string& caminho_imagem, std::mutex* mutex, SHA* sha = nullptr);
    void executar();
};

#endif