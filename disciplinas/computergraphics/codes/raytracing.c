/**
 * \file raytracing.c
 *
 * \brief Implementação do traçador de raios para criação de imagens
 * com renderização de uma esfera.
 *
 * \author
 * Petrucio Ricardo Tavares de Medeiros \n
 * Universidade Federal Rural do Semi-Arido \n
 * Departamento de Engenharias e Tecnologia \n
 * petrucio at ufersa (dot) edu (dot) br
 *
 * \version 1.0
 * \date Jul 2025
 */

#include <stdio.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600

unsigned char img[WIDTH][HEIGHT][3];

typedef struct{
  float x, y, z;
} Vertex;

Vertex add( Vertex a, Vertex b ){ return (Vertex){a.x + b.x, a.y + b.y, a.z + b.z}; }
Vertex sub( Vertex a, Vertex b ){ return (Vertex){a.x - b.x, a.y - b.y, a.z - b.z}; }
Vertex scale( Vertex v, float s ){ return (Vertex){s*v.x, s*v.y, s*v.z}; }
float dot(Vertex a, Vertex b){ return a.x*b.x + a.y*b.y + a.z*b.z; }
float length(Vertex v){ return sqrtf(dot(v, v)); }
Vertex normalize( Vertex v ){ return scale(v, 1.0f / length( v )); }

void save(){
  printf("P3\n %d \t %d\n 255\n", WIDTH, HEIGHT);
  for (int y = 0; y < HEIGHT; y++){
    for (int x = 0; x < WIDTH; x++){
      for (int c = 0; c < 3; c++){
	printf("%d \t", img[x][y][c]);
      }
      printf("\n");
    }
  }
}

// Retorna t (distância) ou -1 se não há interseção
float intersecao_esfera( Vertex O, Vertex D, Vertex C, float r ){
  Vertex L = sub( O, C );
  float a = dot( D, D );
  float b = 2.0 * dot(L, D);
  float c = dot(L, L) - r * r;
  float delta = b * b - 4 * a * c;
  if ( delta < 0 ) return -1.0;
  float sqrt_delta = sqrtf( delta );
  float t0 = (-b - sqrt_delta) / (2 * a);
  float t1 = (-b + sqrt_delta) / (2 * a);
  if ( t0 > 0.001 ) return t0;
  if ( t1 > 0.001 ) return t1;
  return -1.0;
}

void render(){
  Vertex camera = (Vertex){0, 0, 0};
  Vertex C = (Vertex){0, 0, -5};
  float r = 1.0;
  Vertex light_position = normalize( (Vertex){1, 1, 1} );

  for (int y = 0; y < HEIGHT; y++){
    for (int x = 0; x < WIDTH; x++){
      float fx = (2.0 * x / WIDTH - 1.0) * (float)WIDTH / HEIGHT;
      float fy = 1.0 - 2.0 * y / (float) HEIGHT;
      Vertex D = normalize( (Vertex){fx, fy, -1} );

      float t = intersecao_esfera(camera, D, C, r);
      if ( t > 0.0 ){
	Vertex hitPoint = add( camera, scale( D, t ));
	Vertex normal = normalize( sub(hitPoint, C) );
	float diff = fmaxf(0.0, dot(normal, light_position));
	img[x][y][0] = (unsigned char)(diff * 255);
	img[x][y][1] = 0;
	img[x][y][2] = 0;
      }
      else{
	img[x][y][0] = 135;
	img[x][y][1] = 206;
	img[x][y][2] = 250;
      }
    }
  }
}

int main(){
  render();
  save();
  return 0;
}
