# Makefile para o Simulador de Hidrômetro

# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O2
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Diretórios
SRC_DIR = src
BUILD_DIR = build
OUTPUT_DIR = outputs

# Nome do executável
TARGET = simulador_hidrometro

# Arquivos fonte
SOURCES = main.cpp \
          $(SRC_DIR)/SHA.cpp \
          $(SRC_DIR)/IprovedorConfig.cpp \
          $(SRC_DIR)/tubo.cpp \
          $(SRC_DIR)/tela.cpp \
          $(SRC_DIR)/Icomando.cpp \
          $(SRC_DIR)/fabrica.cpp \
          $(SRC_DIR)/GeradorImagemPNG.cpp \
          $(SRC_DIR)/cronometro.cpp \
          $(SRC_DIR)/simuladorHidrometro.cpp

# Arquivos objeto (derivados dos sources, colocados na pasta outputs)
OBJECTS = $(patsubst %.cpp,$(OUTPUT_DIR)/%.o,$(SOURCES))

# Regra principal
all: $(TARGET)

# Compilação do executável
$(TARGET): $(OBJECTS)
	@echo "Linkando o executável..."
	$(CXX) $(OBJECTS) -o $(TARGET) $(SFML_LIBS)
	@echo "Executável $(TARGET) criado com sucesso!"

# Criação da pasta outputs se não existir
$(OUTPUT_DIR):
	@echo "Criando pasta $(OUTPUT_DIR)..."
	@mkdir -p $(OUTPUT_DIR)
	@mkdir -p $(OUTPUT_DIR)/$(SRC_DIR)

# Compilação dos arquivos objeto
$(OUTPUT_DIR)/%.o: %.cpp | $(OUTPUT_DIR)
	@echo "Compilando $< -> $@..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza
clean:
	@echo "Limpando arquivos temporários..."
	rm -rf $(OUTPUT_DIR) $(TARGET) *.png
	@echo "Limpeza concluída!"

# Rebuild completo
rebuild: clean all

# Execução
run: $(TARGET)
	@echo "Executando o simulador..."
	./$(TARGET)

# Teste de compilação (só compila, não executa)
test-build: $(TARGET)
	@echo "Teste de compilação bem-sucedido!"

# Debug build
debug: CXXFLAGS += -DDEBUG -g3
debug: $(TARGET)

# Release build
release: CXXFLAGS += -DNDEBUG -O3
release: clean $(TARGET)

# Verificação de dependências
check-deps:
	@echo "Verificando dependências..."
	@echo "Verificando g++..."
	@which g++ > /dev/null && echo "g++ encontrado" || echo "g++ não encontrado"
	@echo "Verificando SFML..."
	@pkg-config --exists sfml-all && echo "SFML encontrado" || echo "SFML pode não estar instalado"

# Informações sobre o projeto
info:
	@echo "Informações do projeto:"
	@echo "   Compilador: $(CXX)"
	@echo "   Flags: $(CXXFLAGS)"
	@echo "   Bibliotecas: $(SFML_LIBS)"
	@echo "   Arquivos fonte: $(words $(SOURCES)) arquivos"
	@echo "   Executável: $(TARGET)"
	@echo "   Pasta de objetos: $(OUTPUT_DIR)"

# Ajuda
help:
	@echo "Makefile do Simulador de Hidrômetro"
	@echo ""
	@echo "Comandos disponíveis:"
	@echo "  make all        - Compila o projeto completo"
	@echo "  make clean      - Remove arquivos temporários"
	@echo "  make rebuild    - Limpa e recompila tudo"
	@echo "  make run        - Compila e executa o simulador"
	@echo "  make test-build - Testa a compilação sem executar"
	@echo "  make debug      - Compila versão de debug"
	@echo "  make release    - Compila versão otimizada"
	@echo "  make check-deps - Verifica dependências"
	@echo "  make info       - Mostra informações do projeto"
	@echo "  make help       - Mostra esta ajuda"

# Alvos que não geram arquivos
.PHONY: all clean rebuild run test-build debug release check-deps info help

# Dependências implícitas dos headers
$(OUTPUT_DIR)/main.o: $(SRC_DIR)/SHA.hpp
$(OUTPUT_DIR)/$(SRC_DIR)/SHA.o: $(SRC_DIR)/SHA.hpp $(SRC_DIR)/IprovedorConfig.hpp $(SRC_DIR)/GeradorImagemPNG.hpp $(SRC_DIR)/Icomando.hpp $(SRC_DIR)/cronometro.hpp $(SRC_DIR)/tela.hpp
$(OUTPUT_DIR)/$(SRC_DIR)/Icomando.o: $(SRC_DIR)/Icomando.hpp $(SRC_DIR)/simuladorHidrometro.hpp $(SRC_DIR)/fabrica.hpp $(SRC_DIR)/IprovedorConfig.hpp $(SRC_DIR)/GeradorImagemPNG.hpp
$(OUTPUT_DIR)/$(SRC_DIR)/cronometro.o: $(SRC_DIR)/cronometro.hpp $(SRC_DIR)/Icomando.hpp
$(OUTPUT_DIR)/$(SRC_DIR)/tela.o: $(SRC_DIR)/tela.hpp
$(OUTPUT_DIR)/$(SRC_DIR)/simuladorHidrometro.o: $(SRC_DIR)/simuladorHidrometro.hpp $(SRC_DIR)/tubo.hpp
$(OUTPUT_DIR)/$(SRC_DIR)/tubo.o: $(SRC_DIR)/tubo.hpp
$(OUTPUT_DIR)/$(SRC_DIR)/fabrica.o: $(SRC_DIR)/fabrica.hpp $(SRC_DIR)/IprovedorConfig.hpp $(SRC_DIR)/simuladorHidrometro.hpp
$(OUTPUT_DIR)/$(SRC_DIR)/IprovedorConfig.o: $(SRC_DIR)/IprovedorConfig.hpp
$(OUTPUT_DIR)/$(SRC_DIR)/GeradorImagemPNG.o: $(SRC_DIR)/GeradorImagemPNG.hpp $(SRC_DIR)/simuladorHidrometro.hpp