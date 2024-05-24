# Flags de compilação
CFLAGS = -Wall -fPIC

# Arquivos de objeto
OBJS = register.o hardware.o

# Nome da biblioteca estática
STATIC_LIB = libhardware.a

# Nome da biblioteca dinâmica
DYNAMIC_LIB = libhardware.so

# Alvo padrão
all: $(STATIC_LIB) $(DYNAMIC_LIB)

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
	rm -f $(OBJS) $(STATIC_LIB) $(DYNAMIC_LIB)

.PHONY: all clean
