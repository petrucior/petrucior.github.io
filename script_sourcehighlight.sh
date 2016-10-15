#!/bin/bash
# Shell script instalador do Source-Highlight
# Author: Petrúcio Ricardo Tavares de Medeiros
# Date: October, 2016.

echo "Instalacao do gnuplot";

# Atualizando os pacotes
sudo apt-get update

# Instalando as dependências
sudo apt-get install libtool automake1.4 autoconf libboost-all-dev

# Baixando versao do source-highlight
wget git clone http://git.savannah.gnu.org/cgit/src-highlite.git

# Acessando o diretório
\cd src-highlite/

# Reorganizar o configurador
autoreconf -i

# Criando Build e acessando
mkdir build
\cd build

# Configurando a instalação
../configure

# Compilando
make

# Instalando
sudo make install

# Retirando um error
ldconfig /usr/local/lib

# Retornando ao diretorio raiz
\cd ~

# Informando termino de instalacao do gnuplot
echo "Gnuplot instalado";
