#include "simuladorHidrometro.hpp"
#include <algorithm>
#include <cmath>

simuladorHidrometro::simuladorHidrometro(tubo * t_entrada, tubo * t_saida){
    this->tubo_entrada = t_entrada;
    this->tubo_saida = t_saida;
    this->consumo_centena_l = 0;
    this->consumo_dezena_l = 0;
    this->consumo_m3 = 0;
}

simuladorHidrometro::~simuladorHidrometro(){
    delete this->tubo_entrada;
    delete this->tubo_saida;
}

/// @brief calcula menor consumo entre os tubos de entrada e saida
/// @return um double com o menor consumo entre os tubos.
double simuladorHidrometro::get_real_consumo() const {
    return std::min(this->tubo_entrada->consumo_em_dlitros(), this->tubo_saida->consumo_em_dlitros()); 
}


/**
 * @brief Atualiza o estado do hidrômetro com base no tempo passado.
 * @param sec O tempo base em segundos que passou desde a última chamada.
 * @param scale Um fator de escala para acelerar/desacelerar a simulação.
 */
void simuladorHidrometro::atualizar_info(double sec, double scale) {

    double tempo_efetivo_segundos = sec * scale;

    double taxa_consumo_dezenas_litros_por_s = this->get_real_consumo();

    double consumo_neste_tick_dezenas_litros = taxa_consumo_dezenas_litros_por_s * tempo_efetivo_segundos;

    double consumo_neste_tick_m3 = consumo_neste_tick_dezenas_litros / 100.0;
    this->consumo_m3 += consumo_neste_tick_m3;

    long long total_litros = static_cast<long long>(std::floor(this->consumo_m3 * 1000.0));

    this->consumo_dezena_l = (total_litros / 10) % 10;

    this->consumo_centena_l = (total_litros / 100) % 10;
    this->consumo_litro = total_litros  % 10;
}


double simuladorHidrometro::get_consumo_total_m3() const {
    return this->consumo_m3;
}
int simuladorHidrometro::get_consumo_litro() const{
    return this->consumo_litro;
}

int simuladorHidrometro::get_consumo_dezena_litro() const {
    return this->consumo_dezena_l;
}

int simuladorHidrometro::get_consumo_centena_litro() const {
    return this->consumo_centena_l;
}
