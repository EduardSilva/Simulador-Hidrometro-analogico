#ifndef IPROVCONF_H
#define IPROVCONF_H
#include "defs.hpp"
#include <string>
#include <fstream>


class IprovedorConfig{
    public:
        virtual configuracoes_t enviar_configuracoes() const = 0;
        virtual void  montar_configuracoes() = 0;
        virtual ~IprovedorConfig() = default;

};

class provedorConfigArquivo : public IprovedorConfig{
    private:
        configuracoes_t configuracoes;
        std::string file_name;
        std::ifstream file;
    public:
        provedorConfigArquivo(const std::string & name);
        virtual configuracoes_t enviar_configuracoes() const override;
        virtual void  montar_configuracoes() override;

};



#endif