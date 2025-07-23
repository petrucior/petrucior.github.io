/**
 * \file raytracing3.c
 *
 * \brief Implementação do traçador de raios para criação de imagens
 * com renderização de uma esfera e um plano. Além disso, implementamos
 * a reflexão.
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

#define MAX_DEPTH 2

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
Vertex reflect(Vertex D, Vertex N){ return sub(D, scale(N, 2.0f * dot(D, N))); }

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

// Retorna t (distância) ou -1 se não há interseção
float intersecao_plano(Vertex O, Vertex D, Vertex P, Vertex N){
  float denominador = dot( N, D );
  if ( fabs(denominador) < 0.0 ) return -1.0;
  float t = dot( sub(P, O), N ) / denominador;
  return ( t > 0.0 ) ? t : -1.0;
}

Vertex trace(Vertex O, Vertex D, int depth){
  Vertex light_position = normalize( (Vertex){1, 1, 1} );

  // Parâmetros da esfera
  Vertex C = (Vertex){0, 0, -5};
  float r = 1.0;

  // Parâmetros do plano
  Vertex ponto_plano = (Vertex){0, -1, 0};
  Vertex normal_plano = (Vertex){0, 1, 0};

  float ts = intersecao_esfera(O, D, C, r);
  float tp = intersecao_plano(O, D, ponto_plano, normal_plano);

  if ( ts > 0.0 && (tp < 0 || ts < tp) ){
    Vertex hitPoint = add( O, scale( D, ts ));
    Vertex normal = normalize( sub(hitPoint, C) );
    float diff = fmaxf(0.0, dot(normal, light_position));

    // Cor difusa
    Vertex cor = scale((Vertex){255, 0, 0}, diff);

    // Reflexão
    if ( depth < MAX_DEPTH ){
      Vertex refl_dir = normalize(reflect(D, normal));
      Vertex refl_color = trace(add(hitPoint, scale(normal, 1e-4)), refl_dir, depth + 1);
      cor = add(scale(cor, 0.5f), scale(refl_color, 0.5f));
    }
    return cor;
  }
  else{
    if ( tp > 0 ){
      Vertex hitPoint = add( O, scale( D, tp ) );
      // Criar um xadrez                                                                                                                      
      float xadrez = ((int)(floor(hitPoint.x) + floor(hitPoint.z))) % 2;
      // Quando xadrez for par imprime mais claro e quando for ímpar mais escuro                                                              
      unsigned char c = xadrez ? 155 : 80;
      float diff = fmaxf(0.0, dot(normal_plano, light_position));
      return (Vertex){c, c, c};
    }
    else{
      return (Vertex){135, 206, 250}; // fundo azul claro
    }
  }
}

void render(){
  Vertex camera = (Vertex){0, 0, 0};

  for (int y = 0; y < HEIGHT; y++){
    for (int x = 0; x < WIDTH; x++){
      float fx = (2.0 * x / WIDTH - 1.0) * (float)WIDTH / HEIGHT;
      float fy = 1.0 - 2.0 * y / (float) HEIGHT;
      Vertex D = normalize( (Vertex){fx, fy, -1} );
      Vertex color = trace(camera, D, 0);
      img[x][y][0] = (unsigned char)color.x;
      img[x][y][1] = (unsigned char)color.y;
      img[x][y][2] = (unsigned char)color.z;
    }
  }
}

int main(){
  render();
  save();
  return 0;
}
