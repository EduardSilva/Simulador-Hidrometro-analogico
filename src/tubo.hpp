#ifndef TUBO_H
#define TUBO_H

class tubo {
    private:

        double diametro;
        double vazao_agua;
        double vazao_ar;

    public:
        /// @brief cria um tubo,-,)
        /// @param diametro_inicial 
        /// @param vazao_agua_inicial 
        /// @param vazao_ar_inicial 
        explicit tubo(double diametro_inicial, double vazao_agua_inicial, double vazao_ar_inicial) : 
            diametro(diametro_inicial), 
            vazao_agua(vazao_agua_inicial), 
            vazao_ar(vazao_ar_inicial){

        }

        double consumo_em_dlitros() const;

        // Métodos para controle dinâmico de vazão
        void aumentar_vazao_agua(double incremento);
        void diminuir_vazao_agua(double decremento);
        double get_vazao_agua() const;
        void set_vazao_agua(double nova_vazao);

};

#endif
