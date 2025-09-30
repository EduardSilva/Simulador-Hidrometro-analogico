#ifndef ICOMANDO_H
#define ICOMANDO_H

#include "simuladorHidrometro.hpp"
#include "fabrica.hpp"
#include "IprovedorConfig.hpp"
#include "GeradorImagemPNG.hpp"
#include <mutex>

class Icomando{
    public:
    virtual ~Icomando() = default;

    virtual void executar(double sec, double scale) = 0;

};

class controladora : public Icomando{
    private:
        simuladorHidrometro * sim;
        GeradorImagemPNG *ger_imagem;
        std::mutex* m_mutex;
        std::string m_nome_arquivo;
        int m_id_instancia;
        double m_ultimo_m3_salvo;
    public:
    controladora(const IprovedorConfig & prov, GeradorImagemPNG * gerador_png, std::mutex* mutex, const std::string& nome_arquivo = "imagem.png", int id_instancia = 1);
    ~controladora();
    void gerar_imagem();
    void executar(double sec, double scale) override;
    void criar_simulador(const IprovedorConfig & prov);
    
    // Controle dinâmico de vazão
    void aumentar_vazao_entrada(double incremento = 0.1);
    void diminuir_vazao_entrada(double decremento = 0.1);
    double get_vazao_entrada() const;
    
private:
    void verificar_e_salvar_m3();
};

#endif