# Definindo o nome do nosso arquivo (programa executável)
PROG = blinky
# Nome do nosso compilador (C Compiler)
CC = arm-none-eabi-gcc
# Definindo as flags de compilação
CFLAGS = -c -mcpu=cortex-m4 -mthumb -Wall -O0
# Armazenando o nome do linker e as opções de linkedição
LD = arm-none-eabi-gcc
CP = arm-none-eabi-objcopy
LDFLAGS = -nostdlib -T stm32f411-rom.ld

# Definindo o os arquivos pré-requisitos para nosso executável
OBJS = startup.o main.o

# Aqui estamos criando o comando "all" no qual ao chamarmos "make all", ele irá compilar todos os nomes que se encontram abaixo
# de uma só vez, sem haver a necessidade de compilarmos cada um separadamente.
#all: startup.o main.o

all: $(PROG).elf

$(PROG).elf: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@
	$(CP) $(PROG).elf -0 binary $(PROG).bin

# Aqui CC é o nome do nosso compilador, CFLAGS são as flags que serão utilizadas e $< representa o primeiro elemento da lista de 
# pré requisitos, e a variável $@ representa o alvo da regra (nosso target)
# main.o:main.c
# 	$(CC) $(CFLAGS) -o $@ $^


# startup.o:startup.c
# 	$(CC) $(CFLAGS) -o $@ $^

# Adicionando regra única para o make com o caracter "%", que automaticamente substitui cada um dos itens em "all" e executa a regra
# de compilação abaixo
%.o:%.c
	$(CC) $(CFLAGS) -o $@ $^


# As linhas abaixo epresentam o comando que nomeamos de "clean" para automatizar a remoção de todos os arquivos com extensão ".o"
# para que, antes de compilar, removamos todos os arquivos ".o" e seja uma compilação limpa
clean:
	rm -f $(OBJS) $(PROG).elf