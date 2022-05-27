# Nome do nosso compilador (C Compiler)
CC = arm-none-eabi-gcc
# Definindo as flags de compilação
CFLAGS = -c -mcpu=cortex-m4 -mthumb

# Aqui CC é o nome do nosso compilador, CFLAGS são as flags que serão utilizadas e $< representa o primeiro elemento da lista de 
# pré requisitos, e a variável $@ representa o alvo da regra (nosso target)
main.o:main.c
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -f *.o
# A linha acima representa o comando que nomeamos de "clean" para automatizar a remoção de todos os arquivos com extensão ".o"
# para que, antes de compilar, removamos todos os arquivos ".o" e seja uma compilação limpa