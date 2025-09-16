
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
