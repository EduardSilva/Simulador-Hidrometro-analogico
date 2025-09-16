#include "fabrica.hpp"



simuladorHidrometro * fabrica::criar_simulador(const IprovedorConfig& prov){
    configuracoes_t conf = prov.enviar_configuracoes();

    tubo * te = new tubo(conf.diametro_entrada, conf.vazao_entrada, conf.ar_entrada);
    tubo * ts = new tubo(conf.diametro_saida, conf.vazao_saida, conf.ar_saida);

    simuladorHidrometro * sim = new simuladorHidrometro(te, ts);

    return sim;
}