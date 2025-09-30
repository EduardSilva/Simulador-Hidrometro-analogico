#include "tubo.hpp"
#include <cmath>

/// @brief Calcula o consumo total em dezenas de litros para um intervalo de 1 segundo
/// @return Volume total em dezenas de litros
double tubo::consumo_em_dlitros() const {
    double vazao_total = this->vazao_agua + this->vazao_ar;
    double raio = this->diametro / 2.0;
    double area_tubo = M_PI * std::pow(raio, 2);
    double vazao_m3_por_segundo = area_tubo * vazao_total;
    double volume_em_litros = vazao_m3_por_segundo * 1000.0;

    return volume_em_litros / 10.0;
}

void tubo::aumentar_vazao_agua(double incremento) {
    if (incremento > 0) {
        this->vazao_agua += incremento;
    }
}

void tubo::diminuir_vazao_agua(double decremento) {
    if (decremento > 0 && this->vazao_agua >= decremento) {
        this->vazao_agua -= decremento;
    } else if (decremento > 0) {
        this->vazao_agua = 0.0;
    }
}

double tubo::get_vazao_agua() const {
    return this->vazao_agua;
}

void tubo::set_vazao_agua(double nova_vazao) {
    if (nova_vazao >= 0) {
        this->vazao_agua = nova_vazao;
    }
}
