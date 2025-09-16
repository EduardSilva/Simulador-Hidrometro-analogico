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
    public:
    controladora(const IprovedorConfig & prov, GeradorImagemPNG * gerador_png, std::mutex* mutex);
    ~controladora();
    void gerar_imagem();
    void executar(double sec, double scale) override;
    void criar_simulador(const IprovedorConfig & prov);
};

#endif