#include "tubo.hpp"
#include <cmath>

/**
 * @brief Calcula o consumo total em DEZENAS DE LITROS, para um intervalo de 1 segundo.
 * @return double O volume total em dezenas de litros.
 */
double tubo::consumo_em_dlitros() const {

    double vazao_total = this->vazao_agua + this->vazao_ar;

    double raio = this->diametro / 2.0;
    double area_tubo = M_PI * std::pow(raio, 2);


    double vazao_m3_por_segundo = area_tubo * vazao_total;
    

    double volume_em_litros = vazao_m3_por_segundo * 1000.0;

    return volume_em_litros / 10.0;
}
