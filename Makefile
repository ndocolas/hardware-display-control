# Flags de compilação
CFLAGS = -Wall -fPIC

# Arquivos de objeto
OBJS = register.o hardware.o

# Arquivo executavel
EXE = exe

# Compiladores
COMP = register.c hardware.c

# Nome da biblioteca estática
STATIC_LIB = libhardware.a

# Nome da biblioteca dinâmica
DYNAMIC_LIB = libhardware.so

# Compilar
all: 
	gcc -o $(EXE) $(COMP)

# Compilar Bibliotecas
lib: 
	$(STATIC_LIB) $(DYNAMIC_LIB)


# Compila os arquivos de objeto
%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

# Cria a biblioteca estática
$(STATIC_LIB): $(OBJS)
	ar rcs $@ $^

# Cria a biblioteca dinâmica
$(DYNAMIC_LIB): $(OBJS)
	gcc -shared -o $@ $^

# Limpa os arquivos gerados
clean:
	rm -f $(OBJS) $(STATIC_LIB) $(DYNAMIC_LIB) $(EXE)

.PHONY: all clean

clear: clean