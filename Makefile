# Flags de compilação
CFLAGS = -Wall -fPIC

# Arquivos de objeto
OBJS = register.o hardware.o

# Arquivo executável
EXE = exe

# Compiladores
COMP = register.c hardware.c

# Nome da biblioteca estática
STATIC_LIB = libhardware.a

# Nome da biblioteca dinâmica
DYNAMIC_LIB = libhardware.so

# Compilar apenas o executável por padrão
all: $(EXE)

# Compilar o executável
$(EXE): $(COMP)
	gcc -o $(EXE) $(COMP)

# Regra para compilar as bibliotecas
lib: $(STATIC_LIB) $(DYNAMIC_LIB)

# Compila os arquivos de objeto
%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

# Cria a biblioteca estática
$(STATIC_LIB): $(OBJS)
	ar rcs $@ $^

# Cria a biblioteca dinâmica
$(DYNAMIC_LIB): $(OBJS)
	gcc -shared -o $@ $^

# Limpa os arquivos gerados, exceto os originais
clean:
	rm -f $(OBJS) $(EXE)

# Limpa todos os arquivos gerados, incluindo bibliotecas
clear: clean
	rm -f $(STATIC_LIB) $(DYNAMIC_LIB)

.PHONY: all clean clear lib
