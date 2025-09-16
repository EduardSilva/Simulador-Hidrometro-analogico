/* #include <iostream>

#include "src/IprovedorConfig.hpp" // Inclui sua classe

void imprimir_configs(const configuracoes_t& configs) {
    std::cout << "\n--- Configuracoes Carregadas ---" << std::endl;
    std::cout << "Vazao de Entrada: " << configs.vazao_entrada << std::endl;
    std::cout << "Diametro de Entrada: " << configs.diametro_entrada << std::endl;
    std::cout << "Ar na Entrada: " << configs.ar_entrada << std::endl;
    std::cout << "Vazao de Saida: " << configs.vazao_saida << std::endl;
    std::cout << "Diametro de Saida: " << configs.diametro_saida << std::endl;
    std::cout << "Ar na Saida: " << configs.ar_saida << " (valor padrao)" << std::endl;
    std::cout << "Tempo: " << configs.tempo << std::endl;
    std::cout << "--------------------------------\n";
}

int main() {
    std::cout << "Iniciando teste do provedor de configuracao..." << std::endl;

    // 1. Cria o provedor. O construtor vai abrir e ler o arquivo "config.txt".
    provedorConfigArquivo provedor("config.txt");

    // 2. Pede as configurações que foram carregadas.
    configuracoes_t configs_carregadas = provedor.enviar_configuracoes();

    // 3. Imprime os valores para verificação.
    imprimir_configs(configs_carregadas);

    std::cout << "\nTeste finalizado." << std::endl;

    return 0;
}
*/
/*
#include <iostream>
#include <iomanip>
#include "src/tubo.hpp"

int main() {
    // PARÂMETROS DO TESTE
    // Diâmetro padrão de 25mm, convertido para metros.
    double diametro_cano = 0.025; // 25mm = 0.025m

    // Mantendo as mesmas velocidades de exemplo
    double velocidade_agua = 2.0; // m/s
    double velocidade_ar = 0.5;   // m/s

    // 1. Cria o tubo com as novas especificações
    tubo meu_tubo(diametro_cano, velocidade_agua, velocidade_ar);

    // 2. Calcula o consumo (volume que passa em 1 segundo)
    double consumo = meu_tubo.consumo_em_dlitros();

    // --- Cálculo esperado para verificação ---
    // Velocidade total = 2.0 + 0.5 = 2.5 m/s
    // Raio = 0.025 / 2 = 0.0125 m
    // Área = pi * (0.0125)^2 ~= 0.00049087 m^2
    // Vazão = 0.00049087 m^2 * 2.5 m/s ~= 0.001227 m^3/s
    // Volume em Litros = 0.001227 m^3 * 1000 ~= 1.227 Litros
    // Consumo em dezenas de litros = 1.227 / 10 ~= 0.1227

    std::cout << "--- Teste da Classe Tubo (Cano Padrao 25mm) ---" << std::endl;
    std::cout << std::fixed << std::setprecision(4); // Formata a saída
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "Consumo em 1 segundo: " << consumo << " dezenas de litros." << std::endl;

    return 0;
}
*/
/*
#include <iostream>
#include <iomanip>
#include <vector>
#include "src/defs.hpp"
#include "src/tubo.hpp"
#include "src/simuladorHidrometro.hpp"

void imprimir_status(int tempo_total, const simuladorHidrometro& sim) {
    std::cout << "--- Tempo de Simulacao: " << tempo_total << "s ---\n";
    std::cout << std::fixed << std::setprecision(4); // Formata para 4 casas decimais
    std::cout << "  Consumo Total Acumulado: " << sim.get_consumo_total_m3() << " m3\n";
    std::cout << "  Ponteiro Centenas de Litros: " << sim.get_consumo_centena_litro() << "\n";
    std::cout << "  Ponteiro Dezenas de Litros:  " << sim.get_consumo_dezena_litro() << "\n";
    std::cout << "  Ponteiro de Litros:  " << sim.get_consumo_litro() << "\n\n";
}

int main() {

    std::cout << "Configurando o cenario de teste..." << std::endl;

    tubo* tubo_de_entrada = new tubo(0.025, 2.0, 0.0);

    tubo* tubo_de_saida = new tubo(0.025, 2.0, 0.0);

    // Cria o simulador, transferindo a posse dos tubos para ele.
    simuladorHidrometro simulador(tubo_de_entrada, tubo_de_saida);

    std::cout << "Taxa de consumo atual (gargalo): " 
              << simulador.get_real_consumo() << " dezenas de litros/s\n" << std::endl;

    // --- FASE 2: SIMULAÇÃO PASSO A PASSO ---
    int tempo_simulado = 0;
    
    std::cout << "=== Iniciando Simulacao em Tempo Real (5 segundos) ===\n";
    for (int i = 0; i < 5; ++i) {
        simulador.atualizar_info(1.0, 10.0); // 1 segundo real, escala 1x
        tempo_simulado += 10;
        imprimir_status(tempo_simulado, simulador);
    }

    std::cout << "=== Simulacao Acelerada (2s reais = 10s de consumo) ===\n";
    for (int i = 0; i < 2; ++i) {
        simulador.atualizar_info(1.0, 50.0); // 1 segundo real, escala 5x (simula 5 segundos de consumo)
        tempo_simulado += 50;
        imprimir_status(tempo_simulado, simulador);
    }
    
    std::cout << "Teste finalizado." << std::endl;

    return 0;
}
*/
/*
#include <iostream>
#include "src/IprovedorConfig.hpp"
#include "src/fabrica.hpp" // Inclui a nova fábrica

int main() {
    std::cout << "Iniciando sistema..." << std::endl;

    // 1. Cria o provedor de configurações (como antes).
    provedorConfigArquivo provedor("config.txt");

    // 2. USA A FÁBRICA para criar o simulador. Toda a complexidade está escondida.
    simuladorHidrometro * simulador = fabrica::criar_simulador(provedor); 

    std::cout << "Simulador criado com sucesso pela fábrica." << std::endl;
    std::cout << "Taxa de consumo inicial: " << simulador->get_real_consumo() << " dezenas de litros/s\n\n";

    // 3. O resto do seu loop de simulação pode continuar aqui...
    for (int i = 0; i < 5; ++i) {
        simulador->atualizar_info(1.0, 1.0);
        // ... imprimir status ...
    }

    std::cout << "Simulação finalizada." << std::endl;
    // A memória é liberada automaticamente quando 'simulador' (unique_ptr) sai de escopo.

    return 0;
}
*/
/*
#include "src/IprovedorConfig.hpp"
#include "src/Icomando.hpp"
#include "src/cronometro.hpp"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    std::cout << "Iniciando sistema de simulacao completo...\n\n";

    // 1. Cria o provedor de configurações
    provedorConfigArquivo provedor("config.txt");

    // 2. Cria a controladora (que também é o nosso IComando)
    controladora minha_controladora(provedor);

    // 4. Cria o cronômetro.
    // Ele vai executar a controladora a cada 1000ms (1 segundo), com escala de tempo 1x
    cronometro meu_cronometro(&minha_controladora, 1000, 100.0);

    // 5. Inicia a simulação (em uma thread separada)
    meu_cronometro.iniciar();
    std::cout << "\n=== CRONOMETRO INICIADO (rodando por 5 segundos) ===\n";
    std::cout << "A saida de status sera impressa a cada segundo:\n\n";

    // A thread principal (main) dorme por 5 segundos enquanto o cronômetro trabalha
    std::this_thread::sleep_for(std::chrono::seconds(50));

    // 6. Para a simulação, finalizando a thread do cronômetro de forma segura
    meu_cronometro.parar();
    std::cout << "\n=== CRONOMETRO PARADO ===\n\n";

    std::cout << "Simulacao finalizada." << std::endl;
    // O destrutor da 'minha_controladora' será chamado aqui, liberando a memória do 'sim'.

    return 0;
}
*/
/*
#include "src/IprovedorConfig.hpp"
#include "src/Icomando.hpp"
#include "src/cronometro.hpp"
#include "src/GeradorImagemPNG.hpp"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    std::cout << "Iniciando sistema de simulacao...\n\n";

    // 1. Cria as peças principais que não mudam
    provedorConfigArquivo provedor("config.txt");
    GeradorImagemPNG gerador_de_imagem(400, 400);

    // 2. Cria a controladora, injetando a dependência do gerador de imagem
    controladora minha_controladora(provedor,&gerador_de_imagem);
    std::cout << "Simulador criado e pronto.\n";

    // 4. Cria o cronômetro, que irá executar a controladora a cada segundo
    cronometro meu_cronometro(&minha_controladora, 1000, 500.0);

    // 5. Inicia o cronômetro (que agora faz tudo: atualiza e gera imagem)
    meu_cronometro.iniciar();
    std::cout << "\n=== CRONOMETRO INICIADO (rodando por 10 segundos) ===\n";

    // A thread 'main' apenas espera a simulação acontecer
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // 6. Para a simulação
    meu_cronometro.parar();
    std::cout << "\n=== CRONOMETRO PARADO ===\n\n";

    std::cout << "Simulacao finalizada." << std::endl;
    
    return 0;
}

*/
/*#include "src/IprovedorConfig.hpp"
#include "src/Icomando.hpp"
#include "src/cronometro.hpp"
#include "src/GeradorImagemPNG.hpp"
#include "src/tela.hpp"
#include <iostream>
#include <mutex>
#include <chrono>
#include <thread>

int main() {
    // 1. Cria o mutex que será partilhado entre as threads
    std::mutex imagem_mutex;

    // 2. Configura os componentes do back-end
    provedorConfigArquivo provedor("config.txt");
    GeradorImagemPNG gerador_de_imagem(400, 400);
    
    // Cria a controladora, passando o endereço do mutex
    controladora minha_controladora(provedor, &gerador_de_imagem, &imagem_mutex);
    
    cronometro meu_cronometro(&minha_controladora, 1000, 1.0);
    
    // --- MUDANÇA CRÍTICA: GARANTIR O PRIMEIRO FRAME ---
    // A thread 'main' gera a primeira imagem de forma segura antes de qualquer outra coisa acontecer.
    // Isto garante que o ficheiro "imagem.png" existe quando a Tela for criada.
    std::cout << "Gerando frame inicial para a tela...\n";
    minha_controladora.gerar_imagem();
    // 3. Configura o front-end
    // A tela é criada e o seu construtor irá carregar com sucesso o frame inicial.
    // O mesmo mutex é passado para a tela para proteger as leituras futuras.
    const std::string NOME_FICHEIRO_FRAME = "imagem.png";
    Tela minha_tela(400, 400, "Simulador de Hidrometro", NOME_FICHEIRO_FRAME, &imagem_mutex);
    
    // 4. Inicia a simulação em segundo plano
    // A thread do cronómetro começará a sobrescrever o ficheiro "imagem.png" de forma segura.
    meu_cronometro.iniciar();
    std::cout << "Thread de simulacao iniciada.\n";

    // 5. Executa a tela
    // O loop de renderização da tela irá recarregar o ficheiro de forma segura.
    minha_tela.executar();

    // 6. Finaliza o programa de forma limpa
    meu_cronometro.parar();
    std::cout << "\nJanela fechada. Simulacao finalizada." << std::endl;
    
    return 0;
}
*/

#include "src/SHA.hpp"
#include <iostream>

int main() {
    try {
        // 1. Cria a aplicação. O construtor faz todo o trabalho pesado.
        SHA simulador_hidrometro_app("config.txt");
        
        // 2. Roda a aplicação.
        simulador_hidrometro_app.run();

    } catch (const std::exception& e) {
        std::cerr << "Uma excecao nao tratada ocorreu: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Ocorreu um erro desconhecido." << std::endl;
        return 1;
    }

    return 0;
}
