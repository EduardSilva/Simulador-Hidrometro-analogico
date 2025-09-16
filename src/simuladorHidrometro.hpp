#ifndef SIMULADORHIDROMETRO_H
#define SIMULADORHIDROMETRO_H

#include "tubo.hpp"

class simuladorHidrometro
{
private:
    double consumo_m3;
    int consumo_centena_l;
    int consumo_dezena_l;
    int consumo_litro;
    tubo * tubo_entrada;
    tubo * tubo_saida;

public:
    simuladorHidrometro(tubo * t_entrada, tubo * t_saida);
    ~simuladorHidrometro();

    void atualizar_info(double sec, double scale);

    double get_consumo_total_m3() const;
    int get_consumo_centena_litro() const;
    int get_consumo_dezena_litro() const;
    int get_consumo_litro() const;
    double get_real_consumo() const;
};


#endif
