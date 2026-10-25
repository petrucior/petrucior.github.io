/**
 * \file camera.c
 *
 * \brief Implementação das funções de manipulação do modelo 3D, câmera
 * e projeção em perspectiva.
 *
 * \author
 * Petrucio Ricardo Tavares de Medeiros \n
 * Universidade Federal Rural do Semi-Árido \n
 * Departamento de Engenharias e Tecnologia \n
 * petrucio at ufersa (dot) edu (dot) br
 *
 * \version 1.0
 * \date Oct 2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "camera.h"

unsigned char image[HEIGHT][WIDTH][3];

void set_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
  if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
    image[y][x][0] = r;
    image[y][x][1] = g;
    image[y][x][2] = b;
  }
}

void draw_line(int x0, int y0, int x1, int y1) {
  for (float t = 0.0; t < 1.0; t = t + 0.0001)
    set_pixel((int)x0+(x1-x0)*t, (int)y0+(y1-y0)*t, 0, 0, 0);
}

void clr(){
  for(int i = 0; i < WIDTH; i++)
    for(int j = 0; j < HEIGHT; j++)
      for(int c = 0; c < 3; c++)
	image[i][j][c] = 255;
}

void save(){
  printf("P3\n %d \t %d\n 255\n", WIDTH, HEIGHT);
  for(int i = 0; i < WIDTH; i++){
    for(int j = 0; j < HEIGHT; j++){
      for(int c = 0; c < 3; c++){
	printf("%d \t", image[i][j][c]);
      }
      printf("\n");
    }
  }
}

int load_obj(const char *filename, Vertex *vertices, int *vcount, Face *faces,
	     int *fcount) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    char line[512];
    *vcount = 0;
    *fcount = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            if (sscanf(line + 2, "%f %f %f", &vertices[*vcount].x,
		       &vertices[*vcount].y, &vertices[*vcount].z) == 3) {
                (*vcount)++;
            }
        } else if (strncmp(line, "f ", 2) == 0) {
            Face face = {.n = 0};
            char *token = strtok(line + 2, " \n");
            while (token && face.n < MAX_FACE_VERTS) {
                int index;
                if (sscanf(token, "%d", &index) == 1) {
                    face.verts[face.n++] = index;
                }
                token = strtok(NULL, " \n");
            }
            faces[(*fcount)++] = face;
        }
    }

    fclose(file);
    return 1;
}

static Vertex sub( Vertex a, Vertex b ){
  return (Vertex) {a.x - b.x, a.y - b.y, a.z - b.z};
}
static float dot( Vertex a, Vertex b ){
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}
static Vertex cross( Vertex a, Vertex b ){
  return (Vertex){
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x
  };
}
static Vertex normalize( Vertex v ){
  float tam = sqrt( dot(v, v) );
  if ( tam > 0.0 ) {
    v.x = v.x / tam;
    v.y = v.y / tam;
    v.z = v.z / tam;
  }
  return v;
}

void resizing( Vertex p, Camera cam, Point2D *out ){

  // Calcular os vetores ortonormais u, v e n
  Vertex n = normalize( sub( cam.Peye, cam.Pat ) );
  Vertex u = normalize( cross( cam.Vup, n ) );
  Vertex v = cross( n, u );

  // Aplicar na matriz view
  Vertex p_eye = sub( p, cam.Peye );
  float x_cam = dot( u, p_eye );
  float y_cam = dot( v, p_eye );
  float z_cam = dot( n, p_eye );

  float w = z_cam + cam.d;
  if ( w < 0.1f ) return;

  float x_proj = x_cam * cam.d / w;
  float y_proj = y_cam * cam.d / w;

  out->x = (int)((x_proj + 1.0f) * WIDTH / 2.0f);
  out->y = (int)((1.0f - y_proj) * HEIGHT / 2.0f);
}


void render_faces(Vertex *vertices, Face *faces, int vcount, int fcount, Camera cam) {
    for (int i = 0; i < fcount; i++) {
        Face face = faces[i];
        for (int j = 0; j < face.n; j++) {
            Vertex v0 = vertices[face.verts[j] - 1];
            Vertex v1 = vertices[face.verts[(j + 1) % face.n] - 1];
	    Point2D p0, p1;
	    resizing( v0, cam, &p0);
	    resizing( v1, cam, &p1);
	    draw_line(p0.x, p0.y, p1.x, p1.y);
        }
    }
}

