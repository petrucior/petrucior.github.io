/**
 * \file dda.c
 *
 * \brief Implementação do algoritmo DDA.
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

// Algoritmo DDA
void drawDDA( int x0, int y0, int x1, int y1 ){
  // calcula a variacao
  int dx = x1 - x0;
  int dy = y1 - y0;

  // calcula o número de passos de incremento
  int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

  // define o incremento em cada eixo
  float x_inc = (float) dx / steps;
  float y_inc = (float) dy / steps;

  float x = x0;
  float y = y0;

  for (int i = 0; i <= steps; i++){
    putPixel((int)x, (int)y, 255, 0, 0);
    x += x_inc;
    y += y_inc;
  }
}

int main(){

  // Limpar a imagem
  clearImage();

  // Executar o algoritmo DDA
  drawDDA(0, 0, 200, 200);
  
  // Salvar a imagem em PPM
  saveImage();
  
  return 0;
}
