/**
 * \file linesegment.c
 *
 * \brief Implementação do segmento de reta usando equacoes vetoriais.
 *
 * \author
 * Petrucio Ricardo Tavares de Medeiros \n
 * Universidade Federal Rural do Semi-Árido \n
 * Departamento de Engenharias e Tecnologia \n
 * petrucio at ufersa (dot) edu (dot) br
 *
 * \version 1.0
 * \date May 2025
 */

#include <stdio.h>

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

// Algoritmo do Segmento de Reta
void drawLine( int x0, int y0, int x1, int y1 ){
  for (float t = 0.0; t < 1.0; t = t + 0.001){
    putPixel((int)(x0 + (x1 - x0)*t), (int)(y0 + (y1 - y0)*t), 255, 0, 0 );
  }
}

int main(){

  // Limpar a imagem
  clearImage();

  // Executar o algoritmo
  drawLine(0, 0, 200, 200);
  
  // Salvar a imagem em PPM
  saveImage();
  
  return 0;
}
