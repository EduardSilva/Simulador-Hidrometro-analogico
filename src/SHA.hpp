#ifndef SHA_HPP
#define SHA_HPP

#include "IprovedorConfig.hpp"
#include "GeradorImagemPNG.hpp"
#include "Icomando.hpp"
#include "cronometro.hpp"
#include "tela.hpp"
#include <string>
#include <memory>
#include <mutex>

class SHA {
private:
    // A classe SHA é "dona" de todos os componentes principais.
    std::unique_ptr<IprovedorConfig> m_provedor;
    std::unique_ptr<GeradorImagemPNG> m_gerador_imagem;
    std::unique_ptr<controladora> m_controladora;
    std::unique_ptr<cronometro> m_cronometro;
    std::unique_ptr<Tela> m_tela;
    
    std::mutex m_mutex;

public:

    explicit SHA(const std::string& caminho_config);
    ~SHA();

    // Proíbe cópias para garantir que só existe uma instância da aplicação.
    SHA(const SHA&) = delete;
    SHA& operator=(const SHA&) = delete;

    void run();
};

#endif // SHA_HPP