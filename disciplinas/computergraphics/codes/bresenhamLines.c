/**
 * \file bresenhamLines.c
 *
 * \brief Implementação do algoritmo bresenham para linhas.
 *
 * \author
 * Petrucio Ricardo Tavares de Medeiros \n
 * Universidade Federal Rural do Semi-Árido \n
 * Departamento de Engenharias e Tecnologia \n
 * petrucio at ufersa (dot) edu (dot) br
 *
 * \version 1.0
 * \date April 2025
 */

#include <stdio.h>
#include <stdlib.h> // abs

#define width 200
#define height 200

unsigned char image[height][width][3];

// Funcao para preencher um pixel na imagem
void putPixel( int x, int y, unsigned char r, unsigned char g, unsigned char b){
  if (( x >= 0 ) && ( x <= width )  && ( y >= 0 ) && ( y <= height )){
    image[x][y][0] = r;
    image[x][y][1] = g;
    image[x][y][2] = b;
  }
}

// Funcao para limpar a imagem
void clearImage(){
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
      putPixel(x, y, 255, 255, 255); 
}

// Funcao para salvar imagem no formato PPM
void saveImage(){
  // Configurando o header do formato PPM
  printf("P3\n %d \t %d\n 255\n", width, height);

  for (int y = 0; y < height; y++){ // Altura
    for (int x = 0; x < width; x++){ // Largura
      for (int c = 0; c < 3; c++){ // Canal: r, g, b
	printf("%d \t", image[x][y][c]);
      }
      printf("\n");
    }
  }
}

// Algoritmo Bresenham (1º quadrante)
void drawBresenhamLinesIncomplete( int x0, int y0, int x1, int y1 ){
  
  // calcula a variacao
  int dx = x1 - x0;
  int dy = y1 - y0;
  
  // calcula o erro
  int D = (2 * dy) - dx;

  int x = x0;
  int y = y0;

  putPixel(x, y, 255, 0, 0);

  while (x < x1){
    x = x + 1;
    if ( D < 0 ){
      D = D + (2 * dy);
    }
    else{
      y = y + 1;
      D = D + (2 * ( dy - dx ));
    }
    putPixel(x, y, 255, 0, 0);
  }

}

// Algoritmo Bresenham ( todos os quadrantes )
void drawBresenhamLines( int x0, int y0, int x1, int y1 ){
  // calcula a variacao absoluta
  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);

  // indica a direcao da reta
  int sx = (x0 < x1) ? 1 : -1; // direção de x
  int sy = (y0 < y1) ? 1 : -1; // direção de y

  // calcula o erro
  int erro = dx - dy;
  
  while (1) {
    putPixel(x0, y0, 250, 0, 0);

    // Quando a condicao for alcancada finaliza o loop
    if (x0 == x1 && y0 == y1) break;

    // atualiza o erro
    int erro2 = 2 * erro;
    
    if (erro2 > -dy) { // verifica se o erro esta acima
      erro -= dy;
      x0 += sx;
    }
    
    if (erro2 < dx) { // verifica se o erro esta abaixo
      erro += dx;
      y0 += sy;
    }
  }
}



int main(){

  // Limpar a imagem
  clearImage();

  // Executar o algoritmo DDA
  //drawBresenhamLinesIncomplete(0, 0, 200, 200);

  drawBresenhamLines(20, 20, 80, 20);
  drawBresenhamLines(20, 20, 20, 80);
  drawBresenhamLines(20, 80, 80, 80);
  drawBresenhamLines(80, 20, 80, 80);
  
  // Salvar a imagem em PPM
  saveImage();
  
  return 0;
}
