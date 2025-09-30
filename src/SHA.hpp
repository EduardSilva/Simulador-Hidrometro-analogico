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
    std::unique_ptr<IprovedorConfig> m_provedor;
    std::unique_ptr<GeradorImagemPNG> m_gerador_imagem;
    std::unique_ptr<controladora> m_controladora;
    std::unique_ptr<cronometro> m_cronometro;
    std::unique_ptr<Tela> m_tela;
    
    std::mutex m_mutex;
    int m_id_instancia;
    std::string m_nome_arquivo_imagem;

public:

    explicit SHA(const std::string& caminho_config, int id_instancia = 1);
    explicit SHA(const configuracoes_t& config, int id_instancia = 1);
    ~SHA();

    SHA(const SHA&) = delete;
    SHA& operator=(const SHA&) = delete;

    void run();
    void iniciar();
    void mostrar_interface();
    void parar();
    
    void aumentar_vazao_entrada(double incremento = 0.1);
    void diminuir_vazao_entrada(double decremento = 0.1);
    double get_vazao_entrada() const;
};

#endif