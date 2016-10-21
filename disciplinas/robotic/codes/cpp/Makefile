#
#	Makefile do cliente.cpp
#

#
# A variável CC indica que compilador que será utilizado
#
CC= g++

#
# A variável CFLAGS indica que opção de compilação queremos
#
CFLAGS= -Wall -pedantic -ansi -g -c -DBUILD_SHARED_LIBS=OFF

#
# A variável LFLAGS indica que opção de compilação queremos
#
LFLAGS= -Wall -g -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255

#
# A variável DIR indica o diretório dos arquivos de cabeçalho do projeto
#
DIR= ~/petrucior.github.io/disciplinas/robotic/codes/cpp

#
# A variável INCS indica o caminho dos arquivos de cabeçalho
#
INCS= -I../ -I/usr/local/include/opencv2 -I/usr/local/include

#
# A variável LIBS indica o caminho das bibliotecas e as bibliotecas usadas na ligação
#
LIBS= $(DIR)/include

#
# A variável OBJS indica os arquivos objetos comuns a todos os executáveis
#
OBJS=

#
# A variável ALL indica os arquivos executáveis
#
all: cliente

#
# Regra de compilação e ligação do executáveis
#

#
# Regra de compilação e ligação do executáveis
# 
multi: cliente.cpp $(DIR)/removeApi/extApi.h
	$(CC) $(LFLAGS) $(CFLAGS) cliente.cpp -o cliente $(LIBS) -O3


#
# Regra que elimina arquivos intermediários
# 
clean:
	rm -f *.o *.cpp~

#
# Regra que elimina arquivos intermediários e executável
# 
realclean:
	rm -f *.o *.cpp~ cliente

