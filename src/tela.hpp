#ifndef TELA_HPP
#define TELA_HPP

#include <string>
#include <SFML/Graphics.hpp>
#include <mutex>
#include <memory> 

class Tela {
private:
    sf::RenderWindow m_janela;
    sf::Texture m_textura;
    std::unique_ptr<sf::Sprite> m_sprite; 
    std::string m_caminho_imagem;
    std::mutex* m_mutex;

    void processarEventos();
    void renderizar();

public:
    Tela(unsigned int largura, unsigned int altura, const std::string& titulo, const std::string& caminho_imagem, std::mutex* mutex);
    void executar();
};

#endif