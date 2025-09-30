## Simulador de Hidrômetro

![Simulador em Ação](SimuladorEmAção.png)

Projeto para a disciplina de Padrões de Projeto do curso de Engenharia de Computação do IFPB Campus Campina Grande.

O projeto simula um hidrômetro, desenvolvido em C++, onde simula o consumo de água tentando se aproximar da realidade. O usuário pode realizar a configuração do simulador para tentar se aproximar de uma situação desejada. Além de gerar mais de uma instância de SHA independente ao mesmo tempo, executando-os simultâneamente.

## Arquitetura do Sistema

```mermaid
classDiagram
    class IprovedorConfig {
        <<interface>>
        +enviar_configuracoes() configuracoes_t
        +montar_configuracoes() void
    }
    
    class provedorConfigArquivo {
        -configuracoes: configuracoes_t
        -file_name: string
        -file: ifstream
        +provedorConfigArquivo(name: string)
        +enviar_configuracoes() configuracoes_t
        +montar_configuracoes() void
    }
    
    class provedorConfigJSON {
        -configuracoes: configuracoes_t
        -file_name: string
        -instancia_id: int
        +provedorConfigJSON(name: string, id: int)
        +enviar_configuracoes() configuracoes_t
        +montar_configuracoes() void
        +carregar_todas_configuracoes(name: string)$ vector~configuracoes_t~
    }
    
    class provedorConfigDireto {
        -configuracoes: configuracoes_t
        +provedorConfigDireto(config: configuracoes_t)
        +enviar_configuracoes() configuracoes_t
        +montar_configuracoes() void
    }
    
    class SHA {
        -m_provedor: unique_ptr~IprovedorConfig~
        -m_gerador_imagem: unique_ptr~GeradorImagemPNG~
        -m_controladora: unique_ptr~controladora~
        -m_cronometro: unique_ptr~cronometro~
        -m_tela: unique_ptr~Tela~
        -m_mutex: mutex
        -m_id_instancia: int
        -m_nome_arquivo_imagem: string
        +SHA(caminho_config: string, id_instancia: int)
        +SHA(config: configuracoes_t, id_instancia: int)
        +run() void
        +iniciar() void
        +mostrar_interface() void
        +parar() void
        +aumentar_vazao_entrada(incremento: double) void
        +diminuir_vazao_entrada(decremento: double) void
        +get_vazao_entrada() double
    }
    
    class Icomando {
        <<interface>>
        +executar(sec: double, scale: double) void
    }
    
    class controladora {
        -sim: simuladorHidrometro*
        -ger_imagem: GeradorImagemPNG*
        -m_mutex: mutex*
        -m_nome_arquivo: string
        -m_id_instancia: int
        -m_ultimo_m3_salvo: double
        +controladora(prov: IprovedorConfig, gerador_png: GeradorImagemPNG*, mutex: mutex*, nome_arquivo: string, id_instancia: int)
        +gerar_imagem() void
        +executar(sec: double, scale: double) void
        +criar_simulador(prov: IprovedorConfig) void
        +aumentar_vazao_entrada(incremento: double) void
        +diminuir_vazao_entrada(decremento: double) void
        +get_vazao_entrada() double
        -verificar_e_salvar_m3() void
    }
    
    class simuladorHidrometro {
        -tubo_entrada: tubo*
        -tubo_saida: tubo*
        -consumo_centena_l: int
        -consumo_dezena_l: int
        -consumo_litro: int
        -consumo_m3: double
        +simuladorHidrometro(t_entrada: tubo*, t_saida: tubo*)
        +atualizar_info(sec: double, scale: double) void
        +get_real_consumo() double
        +get_consumo_total_m3() double
        +get_consumo_litro() int
        +get_consumo_dezena_litro() int
        +get_consumo_centena_litro() int
        +aumentar_vazao_entrada(incremento: double) void
        +diminuir_vazao_entrada(decremento: double) void
        +get_vazao_entrada() double
    }
    
    class tubo {
        -diametro: double
        -vazao_agua: double
        -vazao_ar: double
        +tubo(diametro: double, vazao_agua: double, vazao_ar: double)
        +consumo_em_dlitros() double
        +aumentar_vazao_agua(incremento: double) void
        +diminuir_vazao_agua(decremento: double) void
        +get_vazao_agua() double
        +set_vazao_agua(nova_vazao: double) void
    }
    
    class cronometro {
        -cmd: Icomando*
        -intervalo_ms: int
        -scala_simulador: double
        -ativo: bool
        -worker_thread: thread
        +cronometro(cmd: Icomando*, intervalo_em_ms: int, escala: double)
        +iniciar() void
        +parar() void
        -loop() void
    }
    
    class GeradorImagemPNG {
        -largura: int
        -altura: int
        -buffer_pixels: vector~unsigned char~
        +GeradorImagemPNG(largura_img: int, altura_img: int)
        +gerar(sim: simuladorHidrometro, nome_arquivo: string) void
        +gerar_jpeg(sim: simuladorHidrometro, nome_arquivo: string) void
        +gerar_buffer_apenas(sim: simuladorHidrometro, nome_arquivo: string) void
    }
    
    class Tela {
        -m_janela: RenderWindow
        -m_textura: Texture
        -m_sprite: unique_ptr~Sprite~
        -m_caminho_imagem: string
        -m_mutex: mutex*
        -m_sha: SHA*
        +Tela(largura: unsigned int, altura: unsigned int, titulo: string, caminho_imagem: string, mutex: mutex*, sha: SHA*)
        +executar() void
        +processarEventos() void
        +renderizar() void
    }
    
    class fabrica {
        <<static>>
        +criar_simulador(prov: IprovedorConfig)$ simuladorHidrometro*
    }
    
    IprovedorConfig <|-- provedorConfigArquivo
    IprovedorConfig <|-- provedorConfigJSON
    IprovedorConfig <|-- provedorConfigDireto
    Icomando <|-- controladora
    
    SHA *-- IprovedorConfig
    SHA *-- GeradorImagemPNG
    SHA *-- controladora
    SHA *-- cronometro
    SHA *-- Tela
    simuladorHidrometro *-- tubo : entrada
    simuladorHidrometro *-- tubo : saida
    
    controladora o-- simuladorHidrometro
    controladora o-- GeradorImagemPNG
    cronometro o-- Icomando
    Tela o-- SHA
    
    fabrica ..> simuladorHidrometro : create
    fabrica ..> tubo : create
    fabrica ..> IprovedorConfig : uses
```

## Como Executar o Projeto

### Pré-requisitos

Certifique-se de ter instalado:
- **Compilador C++17 ou superior** (g++)
- **SFML 2.5+** (Simple and Fast Multimedia Library)
- **Make** (para utilizar o Makefile)

#### Instalação do SFML no Ubuntu/Debian:
```bash
sudo apt update
sudo apt install libsfml-dev
```

#### Instalação do SFML no Arch Linux:
```bash
sudo pacman -S sfml
```

### Compilação

O projeto utiliza um Makefile completo com várias opções de compilação:

#### Comandos Principais:

```bash
# Compilação padrão
make all
# ou simplesmente
make

# Limpeza de arquivos temporários
make clean

# Limpeza e recompilação completa
make rebuild

# Compilação e execução
make run

# Compilação para debug (com símbolos de depuração)
make debug

# Compilação otimizada para release
make release

# Teste de compilação (compila mas não executa)
make test-build
```

#### Comandos Utilitários:

```bash
# Verificar dependências
make check-deps

# Informações sobre o projeto
make info

# Mostrar ajuda
make help
```

### Configuração

O simulador utiliza um arquivo `config.json` para configurar múltiplas instâncias:

```json
{
  "instancias": [
    {
      "id": 1,
      "nome": "Hidrometro_Principal",
      "vazao_entrada": 2.0,
      "diametro_entrada": 0.075,
      "ar_entrada": 1.5,
      "vazao_saida": 2.0,
      "diametro_saida": 0.075,
      "ar_saida": 0.0,
      "tempo": 1000,
      "scale": 10
    }
  ]
}
```

### Execução

Após a compilação, execute:

```bash
./simulador_hidrometro
```

O programa irá:
1. Carregar as configurações do `config.json`
2. Criar múltiplas instâncias de hidrômetros simultaneamente
3. Abrir janelas gráficas para cada instância
4. Salvar automaticamente imagens JPEG a cada m³ consumido

### Controles

Durante a execução, você pode:
- **Seta para cima (↑)** ou **+**: Aumentar vazão de entrada
- **Seta para baixo (↓)** ou **-**: Diminuir vazão de entrada
- **Fechar janela**: Encerrar a instância específica

### Estrutura de Arquivos

```
projeto/
├── main.cpp              # Ponto de entrada do programa
├── config.json           # Configurações das instâncias
├── Makefile              # Sistema de compilação
├── src/                  # Código fonte
│   ├── SHA.cpp/hpp       # Classe principal do simulador
│   ├── simuladorHidrometro.cpp/hpp
│   ├── IprovedorConfig.cpp/hpp
│   ├── GeradorImagemPNG.cpp/hpp
│   ├── cronometro.cpp/hpp
│   ├── tela.cpp/hpp
│   ├── tubo.cpp/hpp
│   ├── fabrica.cpp/hpp
│   └── Icomando.cpp/hpp
├── outputs/              # Arquivos objeto (gerados)
├── temp/                 # Arquivos PNG temporários
└── Medições_*/           # Imagens JPEG salvas automaticamente
```