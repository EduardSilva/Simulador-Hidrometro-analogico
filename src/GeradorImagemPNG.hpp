#ifndef GERADOR_IMAGEM_PNG_HPP
#define GERADOR_IMAGEM_PNG_HPP

#include "simuladorHidrometro.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

struct Cor { unsigned char r, g, b; };

class GeradorImagemPNG {
private:
    const int largura;
    const int altura;
    const int canais = 3;
    std::vector<unsigned char> buffer_pixels;

    // --- Funções auxiliares de desenho (privadas) ---
    void desenhar_pixel(int x, int y, Cor cor);
    void desenhar_retangulo(int x, int y, int w, int h, Cor cor);
    void desenhar_circulo_preenchido(int centro_x, int centro_y, int raio, Cor cor_fundo_externo, Cor cor_circulo);
    void desenhar_digito(int digito, int x_offset, int y_offset, int escala, Cor cor);
    void desenhar_texto(const std::string& texto, int x_start, int y_start, int escala, Cor cor);
    void desenhar_imagem(int x_offset, int y_offset, const std::string& caminho_imagem);
    void desenhar_linha(int x0, int y0, int x1, int y1, int espessura, Cor cor); // --- MUDANÇA ---

public:
    explicit GeradorImagemPNG(int largura_img, int altura_img);
    void gerar(const simuladorHidrometro& sim, const std::string& nome_arquivo);
    void gerar_jpeg(const simuladorHidrometro& sim, const std::string& nome_arquivo, int qualidade = 90);
    void gerar_buffer_apenas(const simuladorHidrometro& sim); // Gera apenas o buffer, sem salvar arquivo
    
    // Métodos para acesso ao buffer
    const unsigned char* get_buffer_pixels() const { return buffer_pixels.data(); }
    int get_largura() const { return largura; }
    int get_altura() const { return altura; }
};

#endif // GERADOR_IMAGEM_PNG_HPP