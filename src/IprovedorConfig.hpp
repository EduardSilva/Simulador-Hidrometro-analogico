#ifndef IPROVCONF_H
#define IPROVCONF_H
#include "defs.hpp"
#include <string>
#include <fstream>
#include <vector>


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

class provedorConfigJSON : public IprovedorConfig{
    private:
        configuracoes_t configuracoes;
        std::string file_name;
        int instancia_id;
    public:
        provedorConfigJSON(const std::string & name, int id);
        virtual configuracoes_t enviar_configuracoes() const override;
        virtual void  montar_configuracoes() override;
        static std::vector<configuracoes_t> carregar_todas_configuracoes(const std::string & name);

};

class provedorConfigDireto : public IprovedorConfig{
    private:
        configuracoes_t configuracoes;
    public:
        provedorConfigDireto(const configuracoes_t& config);
        virtual configuracoes_t enviar_configuracoes() const override;
        virtual void  montar_configuracoes() override;

};



#endif