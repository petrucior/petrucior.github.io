#!/bin/bash
# Shell script atualizador dos arquivos asciidoc
# Author: Petrúcio Ricardo Tavares de Medeiros
# Date: October, 2015.

echo "Atualizando. . .";

# Gerar index.html para o diretorio de disciplinas
cd disciplinas/graphicsComputer
asciidoc -b html5 -a icons -a toc2 -a theme=flask graphicsComputer.txt

# Gerar slides no diretório de disciplinas
asciidoc rayTracing.txt
