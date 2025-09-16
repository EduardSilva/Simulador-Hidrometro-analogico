#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "GeradorImagemPNG.hpp"
#include <cmath>
#include <string>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <vector>
#include <filesystem>

// --- Definição da Fonte Bitmap 5x7 ---
const int DIGITO_LARGURA = 5; const int DIGITO_ALTURA = 7;
const bool FONT_MAP[10][DIGITO_ALTURA][DIGITO_LARGURA] = {
    {{1,1,1,1,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,1}}, // 0
    {{0,0,1,0,0},{0,1,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,1,1,1,0}}, // 1
    {{1,1,1,1,1},{0,0,0,0,1},{0,0,0,0,1},{1,1,1,1,1},{1,0,0,0,0},{1,0,0,0,0},{1,1,1,1,1}}, // 2
    {{1,1,1,1,1},{0,0,0,0,1},{0,0,0,0,1},{1,1,1,1,1},{0,0,0,0,1},{0,0,0,0,1},{1,1,1,1,1}}, // 3
    {{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,1},{0,0,0,0,1},{0,0,0,0,1},{0,0,0,0,1}}, // 4
    {{1,1,1,1,1},{1,0,0,0,0},{1,0,0,0,0},{1,1,1,1,1},{0,0,0,0,1},{0,0,0,0,1},{1,1,1,1,1}}, // 5
    {{1,1,1,1,1},{1,0,0,0,0},{1,0,0,0,0},{1,1,1,1,1},{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,1}}, // 6
    {{1,1,1,1,1},{0,0,0,0,1},{0,0,0,0,1},{0,0,0,0,1},{0,0,0,0,1},{0,0,0,0,1},{0,0,0,0,1}}, // 7
    {{1,1,1,1,1},{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,1},{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,1}}, // 8
    {{1,1,1,1,1},{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,1},{0,0,0,0,1},{0,0,0,0,1},{1,1,1,1,1}}  // 9
};

GeradorImagemPNG::GeradorImagemPNG(int largura_img, int altura_img) : largura(largura_img), altura(altura_img), buffer_pixels(largura_img * altura_img * canais) {}
void GeradorImagemPNG::desenhar_pixel(int x, int y, Cor cor) { if (x < 0 || x >= largura || y < 0 || y >= altura) return; int index = (y * largura + x) * canais; buffer_pixels[index] = cor.r; buffer_pixels[index + 1] = cor.g; buffer_pixels[index + 2] = cor.b; }
void GeradorImagemPNG::desenhar_circulo_preenchido(int cx, int cy, int r, Cor c_fundo, Cor c_circ) { for (int y = cy - r - 1; y < cy + r + 1; y++) { for (int x = cx - r - 1; x < cx + r + 1; x++) { double d = std::sqrt(std::pow(x - cx, 2) + std::pow(y - cy, 2)); if (d <= r + 0.5) { double s = std::clamp(r + 0.5 - d, 0.0, 1.0); unsigned char R = s*c_circ.r+(1-s)*c_fundo.r, G = s*c_circ.g+(1-s)*c_fundo.g, B = s*c_circ.b+(1-s)*c_fundo.b; desenhar_pixel(x,y,{R,G,B}); } } } }
void GeradorImagemPNG::desenhar_retangulo(int x, int y, int w, int h, Cor cor) { for (int j = y; j < y + h; ++j) for (int i = x; i < x + w; ++i) desenhar_pixel(i, j, cor); }
void GeradorImagemPNG::desenhar_digito(int d, int xo, int yo, int s, Cor c) { if (d < 0 || d > 9) return; for (int y = 0; y < DIGITO_ALTURA; ++y) for (int x = 0; x < DIGITO_LARGURA; ++x) if (FONT_MAP[d][y][x]) desenhar_retangulo(xo+x*s, yo+y*s, s, s, c); }
void GeradorImagemPNG::desenhar_texto(const std::string& t, int xs, int ys, int s, Cor c) { for (size_t i = 0; i < t.length(); ++i) if (t[i] >= '0' && t[i] <= '9') desenhar_digito(t[i]-'0', xs+i*(DIGITO_LARGURA*s+s*2), ys, s, c); }
void GeradorImagemPNG::desenhar_linha(int x0, int y0, int x1, int y1, int esp, Cor c) { for (int i = -esp/2; i <= esp/2; ++i) for (int j = -esp/2; j <= esp/2; ++j) { int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1; int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1; int err = dx + dy, e2; int curX = x0+i, curY = y0+j; while(true) { desenhar_pixel(curX, curY, c); if (curX == (x1+i) && curY == (y1+j)) break; e2 = 2*err; if (e2 >= dy) { err += dy; curX += sx; } if (e2 <= dx) { err += dx; curY += sy; } } } }
// Função para carregar imagem foi removida pois não é mais necessária.

void GeradorImagemPNG::gerar(const simuladorHidrometro& sim, const std::string& nome_arquivo) {
    // --- CORES e Constantes ---
    const Cor PRETO = {30, 30, 30};
    const Cor CINZA_FUNDO_EXTERNO = {220, 220, 220};
    const Cor CINZA_MIOLO = {245, 245, 245};
    const Cor CINZA_BORDA = {80, 80, 80};
    const Cor CINZA_DISPLAY = {50, 50, 50};
    const Cor BRANCO_TEXTO = {240, 240, 240};
    const Cor VERMELHO_TEXTO = {220, 50, 50};
    
    int centro_x = largura / 2;
    int centro_y = altura / 2;

    std::fill(buffer_pixels.begin(), buffer_pixels.end(), 220);

    // 1. Desenha o miolo circular
    desenhar_circulo_preenchido(centro_x, centro_y, 195, CINZA_FUNDO_EXTERNO, CINZA_BORDA);
    desenhar_circulo_preenchido(centro_x, centro_y, 185, CINZA_BORDA, CINZA_MIOLO);

    // --- MUDANÇA --- Lógica de desenho do odômetro com janelas individuais ---
    
    // Pega todos os valores de uma vez
    long long consumo_m3 = static_cast<long long>(sim.get_consumo_total_m3());
    int centena_l = sim.get_consumo_centena_litro();
    int dezena_l = sim.get_consumo_dezena_litro();
    int litro_l = sim.get_consumo_litro(); 

    // Formata os números como strings
    std::stringstream ss_m3, ss_litros;
    ss_m3 << std::setfill('0') << std::setw(4) << (consumo_m3 % 10000);
    ss_litros << centena_l << dezena_l; // Os 2 dígitos vermelhos
    
    std::string m3_str = ss_m3.str();
    std::string litros_str = ss_litros.str();

    // Constantes de layout para o odômetro
    int escala_fonte = 4;
    int digito_w = DIGITO_LARGURA * escala_fonte;
    int digito_h = DIGITO_ALTURA * escala_fonte;
    int espacamento = 8;
    int janela_w = digito_w + 4;
    int janela_h = digito_h + 4;
    int y_display = 130;
    
    // Calcula a largura total para centralizar
    int total_w_m3 = 4 * (janela_w + espacamento);
    int total_w_litros = 2 * (janela_w + espacamento);
    int total_odometro_w = total_w_m3 + total_w_litros;
    int x_inicial = centro_x - total_odometro_w / 2;

    // Desenha as janelas e os dígitos para m³ (pretos)
    for (int i = 0; i < 4; ++i) {
        int x_pos = x_inicial + i * (janela_w + espacamento);
        desenhar_retangulo(x_pos, y_display, janela_w, janela_h, CINZA_DISPLAY);
        desenhar_texto(std::string(1, m3_str[i]), x_pos + 2, y_display + 2, escala_fonte, BRANCO_TEXTO);
    }
    
    // Desenha a vírgula separadora
    desenhar_retangulo(x_inicial + total_w_m3 - espacamento/2, y_display + janela_h - 10, 6, 6, BRANCO_TEXTO);

    // Desenha as janelas e os dígitos para litros (vermelhos)
    for (int i = 0; i < 2; ++i) {
        int x_pos = x_inicial + total_w_m3 + i * (janela_w + espacamento);
        desenhar_retangulo(x_pos, y_display, janela_w, janela_h, CINZA_DISPLAY);
        desenhar_texto(std::string(1, litros_str[i]), x_pos + 2, y_display + 2, escala_fonte, VERMELHO_TEXTO);
    }
    
    // Adiciona a etiqueta "m³" ao final
    desenhar_texto("m3", x_inicial + total_odometro_w + 5, y_display + 10, 2, PRETO);

    // Desenha o relógio analógico de litros
    int relogio_cx = centro_x;
    int relogio_cy = 260;
    int relogio_r = 60;
    for (int i = 0; i < 10; ++i) {
        double ang = (i * 36 - 90) * (M_PI / 180.0);
        int nx = relogio_cx + (relogio_r - 12) * cos(ang) - (DIGITO_LARGURA);
        int ny = relogio_cy + (relogio_r - 12) * sin(ang) - (DIGITO_ALTURA);
        desenhar_texto(std::to_string(i), nx, ny, 2, PRETO);
    }
    double ang_ponteiro = (litro_l * 36 - 90) * (M_PI / 180.0);
    int px = relogio_cx + (relogio_r - 8) * cos(ang_ponteiro);
    int py = relogio_cy + (relogio_r - 8) * sin(ang_ponteiro);
    desenhar_linha(relogio_cx, relogio_cy, px, py, 3, VERMELHO_TEXTO);
    desenhar_circulo_preenchido(relogio_cx, relogio_cy, 5, VERMELHO_TEXTO, VERMELHO_TEXTO);
    
    // --- REMOVIDO --- A chamada para desenhar o logo do INMETRO foi removida.
    
    stbi_write_png(nome_arquivo.c_str(), largura, altura, canais, buffer_pixels.data(), largura * canais);
}
