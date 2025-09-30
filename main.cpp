#include "src/SHA.hpp"
#include "src/IprovedorConfig.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <memory>

int main() {
    try {
        std::vector<configuracoes_t> configs = provedorConfigJSON::carregar_todas_configuracoes("config.json");
        
        if (configs.empty()) {
            std::cerr << "Erro: Nenhuma configuração encontrada no arquivo config.json" << std::endl;
            return 1;
        }
        
        const int NUM_INSTANCIAS = configs.size();
        std::vector<std::unique_ptr<SHA>> hidrotmetros;
        std::vector<std::thread> threads;

        std::cout << "Criando " << NUM_INSTANCIAS << " instancias de hidrometros...\n";

        for (int i = 0; i < NUM_INSTANCIAS; ++i) {
            hidrotmetros.push_back(std::make_unique<SHA>(configs[i], i + 1));
            hidrotmetros.back()->iniciar();
        }

        std::cout << "Iniciando interfaces graficas em threads separadas...\n";

        for (int i = 0; i < NUM_INSTANCIAS; ++i) {
            threads.emplace_back([&hidrotmetros, i]() {
                hidrotmetros[i]->mostrar_interface();
            });
        }

        std::cout << "Aguardando threads terminarem...\n";

        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }

        std::cout << "Parando todas as simulacoes...\n";

        for (auto& hidrometro : hidrotmetros) {
            hidrometro->parar();
        }

        std::cout << "Todas as instancias foram encerradas com sucesso.\n";

    } catch (const std::exception& e) {
        std::cerr << "Uma excecao nao tratada ocorreu: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Ocorreu um erro desconhecido." << std::endl;
        return 1;
    }

    return 0;
}
