#!/bin/bash
# Shell script atualizador dos arquivos asciidoc
# Author: Petrúcio Ricardo Tavares de Medeiros
# Date: October, 2015.

echo "Atualizando. . .";

# Gerar index.html para o diretorio de disciplinas
cd disciplinas/graphicsComputer
asciidoc -b html5 -a icons -a toc2 -a theme=flask graphicsComputer.txt
asciidoc -b html5 -a latexmath -a icons -a toc2 -a theme=flask rayTracingExercicios.txt
asciidoc -b html5 -a latexmath -a icons -a toc2 -a theme=flask povray.txt

# Gerar slides no diretório de disciplinas
asciidoc -a latexmath rayTracing.txt

cd ../robotic
asciidoc -b html5 -a latexmath -a icons -a toc2 -a theme=flask vrep.txt

