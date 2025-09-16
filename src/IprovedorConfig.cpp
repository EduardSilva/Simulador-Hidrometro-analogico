#include "IprovedorConfig.hpp"
#include <iostream>
#include <map>
#include <stdexcept>
namespace {

    enum class campos  {
        VAZAO_ENTRADA,
        DIAMETRO_ENTRADA,
        AR_ENTRADA,
        VAZAO_SAIDA,
        DIAMETRO_SAIDA,
        AR_SAIDA,
        TEMPO,
        SCALE
    };

    campos string_to_enum (const std::string & s){
        static const std::map<std::string, campos> string_to_campo{
        {"vazao_entrada", campos::VAZAO_ENTRADA},
        {"diametro_entrada", campos::DIAMETRO_ENTRADA},
        {"ar_entrada", campos::AR_ENTRADA},
        {"vazao_saida",      campos::VAZAO_SAIDA},
        {"diametro_saida",   campos::DIAMETRO_SAIDA},
        {"ar_saida",         campos::AR_SAIDA},
        {"tempo",            campos::TEMPO},
        {"scale",            campos::SCALE}
        };
        //faz a busca no map acima pelo parametro procurado.
        std::map<std::string, campos>::const_iterator it = string_to_campo.find(s);
        if(it != string_to_campo.end()){
            return it->second;
        }
        else {
            throw std::invalid_argument("Parametro desconhecido :" + s);
        }
    };
};



provedorConfigArquivo::provedorConfigArquivo(const std::string & name)    : file_name(name), file(name) // Inicializa o ifstream com o arquivo.
{
    if (!file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo de configuracao: " << file_name << std::endl;
    } else {
        // Já que abrimos o arquivo, podemos montar as configs aqui mesmo.
        montar_configuracoes();
        this->file.close();
    }
}
/// @brief monta as configurações de acordo com o arquivo que recebeu.
void provedorConfigArquivo::montar_configuracoes(){
    //pra chegar aqui o arquivo já deve estar aberto.
    std::string linha;
    int nlinha = 1;
    while (std::getline(file,linha)){
        if(linha.empty() || (linha[0] == '#')){ //skip caso esteja vazia ou comentario.
            nlinha++;
            continue;
        }
        size_t equal_pos = linha.find('=');
        if(equal_pos == std::string::npos){
            std::cerr << "Linha nº:" << nlinha << " sem '='" << std::endl;
            nlinha++;
            continue; //skipa linha mal formada, mandando aviso
        }
        try{
            switch (string_to_enum(linha.substr(0, equal_pos))){
                case campos::VAZAO_ENTRADA:
                    this->configuracoes.vazao_entrada = std::stod(linha.substr(equal_pos+1));
                break;
                case campos::DIAMETRO_ENTRADA:
                    this->configuracoes.diametro_entrada = std::stod(linha.substr(equal_pos+1));
                break;
                case campos::AR_ENTRADA:
                    this->configuracoes.ar_entrada = std::stod(linha.substr(equal_pos+1));
                break;
                case campos::VAZAO_SAIDA:
                    this->configuracoes.vazao_saida = std::stod(linha.substr(equal_pos+1));
                break;
                case campos::DIAMETRO_SAIDA:
                    this->configuracoes.diametro_saida = std::stod(linha.substr(equal_pos+1));
                break;
                case campos::AR_SAIDA:
                    this->configuracoes.ar_saida = std::stod(linha.substr(equal_pos+1));
                break;
                case campos::TEMPO:
                    this->configuracoes.tempo = std::stod(linha.substr(equal_pos+1));
                break;
                case campos::SCALE:
                    this->configuracoes.scale = std::stod(linha.substr(equal_pos+1));
                break;
            
                default:
                break;
            }
        }
        catch(const std::exception& e){
            std::cerr << "Erro ao processar linha " << nlinha << ": " << e.what() << std::endl;
        }
        nlinha++;
    }
    
}

/// @brief retorna as configurações obtidas do arquivo ou seus valores padrões
/// @return retorna um configuracoes_t
configuracoes_t provedorConfigArquivo::enviar_configuracoes() const {
    return this->configuracoes;
}