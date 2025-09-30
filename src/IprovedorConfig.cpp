#include "IprovedorConfig.hpp"
#include <iostream>
#include <map>
#include <stdexcept>
#include <sstream>
#include <regex>

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
    
    /// @brief Mapeia uma string para um campo da enumeração
    /// @param s String a ser mapeada
    /// @return Campo correspondente da enumeração
    campos string_to_enum(const std::string & s){
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
        
        std::map<std::string, campos>::const_iterator it = string_to_campo.find(s);
        if(it != string_to_campo.end()){
            return it->second;
        }
        else {
            throw std::invalid_argument("Parametro desconhecido :" + s);
        }
    };
};



provedorConfigArquivo::provedorConfigArquivo(const std::string & name) : file_name(name), file(name)
{
    if (!file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo de configuracao: " << file_name << std::endl;
    } else {
        montar_configuracoes();
        this->file.close();
    }
}

/// @brief Monta as configurações de acordo com o arquivo recebido
void provedorConfigArquivo::montar_configuracoes(){
    std::string linha;
    int nlinha = 1;
    while (std::getline(file,linha)){
        if(linha.empty() || (linha[0] == '#')){
            nlinha++;
            continue;
        }
        size_t equal_pos = linha.find('=');
        if(equal_pos == std::string::npos){
            std::cerr << "Linha nº:" << nlinha << " sem '='" << std::endl;
            nlinha++;
            continue;
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

/// @brief Retorna as configurações obtidas do arquivo
/// @return Estrutura configuracoes_t com os valores carregados
configuracoes_t provedorConfigArquivo::enviar_configuracoes() const {
    return this->configuracoes;
}

namespace {
    /// @brief Extrai valor numérico de uma string JSON
    /// @param linha String JSON contendo o campo
    /// @param campo Nome do campo a ser extraído
    /// @return Valor numérico extraído ou 0.0 se não encontrado
    double extrair_valor_json(const std::string& linha, const std::string& campo) {
        std::regex regex("\"" + campo + "\"\\s*:\\s*([0-9]*\\.?[0-9]+)");
        std::smatch match;
        if (std::regex_search(linha, match, regex)) {
            return std::stod(match[1].str());
        }
        return 0.0;
    }
    
    /// @brief Extrai valor inteiro de uma string JSON
    /// @param linha String JSON contendo o campo
    /// @param campo Nome do campo a ser extraído
    /// @return Valor inteiro extraído ou 0 se não encontrado
    int extrair_id_json(const std::string& linha, const std::string& campo) {
        std::regex regex("\"" + campo + "\"\\s*:\\s*([0-9]+)");
        std::smatch match;
        if (std::regex_search(linha, match, regex)) {
            return std::stoi(match[1].str());
        }
        return 0;
    }
}

provedorConfigJSON::provedorConfigJSON(const std::string & name, int id) 
    : file_name(name), instancia_id(id) {
    montar_configuracoes();
}

void provedorConfigJSON::montar_configuracoes() {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo JSON: " << file_name << std::endl;
        return;
    }
    
    std::string linha;
    std::string json_content;
    
    while (std::getline(file, linha)) {
        json_content += linha + "\n";
    }
    file.close();
    
    std::regex instancia_regex(R"(\{[^}]*"id"\s*:\s*)" + std::to_string(instancia_id) + R"([^}]*\})");
    std::smatch match;
    
    if (std::regex_search(json_content, match, instancia_regex)) {
        std::string instancia_str = match[0].str();
        
        configuracoes.vazao_entrada    = extrair_valor_json(instancia_str, "vazao_entrada");
        configuracoes.diametro_entrada = extrair_valor_json(instancia_str, "diametro_entrada");
        configuracoes.ar_entrada       = extrair_valor_json(instancia_str, "ar_entrada");
        configuracoes.vazao_saida      = extrair_valor_json(instancia_str, "vazao_saida");
        configuracoes.diametro_saida   = extrair_valor_json(instancia_str, "diametro_saida");
        configuracoes.ar_saida         = extrair_valor_json(instancia_str, "ar_saida");
        configuracoes.tempo            = extrair_valor_json(instancia_str, "tempo");
        configuracoes.scale            = extrair_valor_json(instancia_str, "scale");
        
        std::cout << "Configurações carregadas para instância " << instancia_id << std::endl;
    } else {
        std::cerr << "Erro: Instância " << instancia_id << " não encontrada no arquivo JSON" << std::endl;
    }
}

configuracoes_t provedorConfigJSON::enviar_configuracoes() const {
    return this->configuracoes;
}

/// @brief Carrega todas as configurações do arquivo JSON
/// @param name Caminho para o arquivo JSON
/// @return Vetor com todas as configurações encontradas
std::vector<configuracoes_t> provedorConfigJSON::carregar_todas_configuracoes(const std::string & name) {
    std::vector<configuracoes_t> configs;
    std::ifstream file(name);
    
    if (!file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo JSON: " << name << std::endl;
        return configs;
    }
    
    std::string linha;
    std::string json_content;
    
    while (std::getline(file, linha)) {
        json_content += linha + "\n";
    }
    file.close();
    
    std::regex instancia_regex(R"(\{[^}]*"id"\s*:\s*[0-9]+[^}]*\})");
    std::sregex_iterator iter(json_content.begin(), json_content.end(), instancia_regex);
    std::sregex_iterator end;
    
    for (; iter != end; ++iter) {
        std::string instancia_str = iter->str();
        configuracoes_t config;
        
        config.vazao_entrada    = extrair_valor_json(instancia_str, "vazao_entrada");
        config.diametro_entrada = extrair_valor_json(instancia_str, "diametro_entrada");
        config.ar_entrada       = extrair_valor_json(instancia_str, "ar_entrada");
        config.vazao_saida      = extrair_valor_json(instancia_str, "vazao_saida");
        config.diametro_saida   = extrair_valor_json(instancia_str, "diametro_saida");
        config.ar_saida         = extrair_valor_json(instancia_str, "ar_saida");
        config.tempo            = extrair_valor_json(instancia_str, "tempo");
        config.scale            = extrair_valor_json(instancia_str, "scale");
        
        configs.push_back(config);
    }
    
    std::cout << "Carregadas " << configs.size() << " configurações do arquivo JSON" << std::endl;
    return configs;
}

provedorConfigDireto::provedorConfigDireto(const configuracoes_t& config) 
    : configuracoes(config) {
}

void provedorConfigDireto::montar_configuracoes() {
}

configuracoes_t provedorConfigDireto::enviar_configuracoes() const {
    return this->configuracoes;
}