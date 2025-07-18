/**
 * \file lambert.c
 *
 * \brief Implementação da iluminação de Lambert
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
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 800
#define MAX_VERTICES 50000
#define MAX_FACES 50000
#define MAX_FACE_VERTS 32

typedef struct {
  float x, y, z;
} Vertex;

typedef struct {
  int verts[MAX_FACE_VERTS];
  int n;
} Face;

unsigned char image[WIDTH][HEIGHT][3];	    

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

void rotate_z(Vertex *v, float angle_rad) {
    float x = v->x;
    float y = v->y;
    v->x = x * cosf(angle_rad) - y * sinf(angle_rad);
    v->y = x * sinf(angle_rad) + y * cosf(angle_rad);
}

void project_3dto2d(Vertex *v) {
    v->x = (v->x + 1.0f) * (WIDTH  / 2.0f);
    v->y = (1.0f + v->y) * (HEIGHT / 2.0f);
}

void barycentric_coordinate( Vertex a, Vertex b, Vertex c, float red, float green, float blue ){
  // calculando o bounding box
  int x_min = floorf(fminf(fminf(a.x, b.x), c.x));
  int x_max = ceilf(fmaxf(fmaxf(a.x, b.x), c.x));
  int y_min = floorf(fminf(fminf(a.y, b.y), c.y));
  int y_max = ceilf(fmaxf(fmaxf(a.y, b.y), c.y));

  // Encontrando a área do triangulo abc
  float area_abc = 0.5 * fabsf(a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y));
  
  for (int y = y_min; y <= y_max; y++){
    for (int x = x_min; x <= x_max; x++){
      Vertex p = {x, y, 0};
      // Encontrando a área dos triangulos
      float area_pbc = 0.5 * (p.x*(b.y - c.y) + b.x*(c.y - p.y) + c.x*(p.y - b.y));
      float area_apc = 0.5 * (a.x*(p.y - c.y) + p.x*(c.y - a.y) + c.x*(a.y - p.y));
      float area_abp = 0.5 * (a.x*(b.y - p.y) + b.x*(p.y - a.y) + p.x*(a.y - b.y));
      float alfa = area_pbc / area_abc;
      float beta = area_apc / area_abc;
      float gamma = area_abp / area_abc;
      if ( alfa >= 0.0 && beta >= 0.0 && gamma >= 0.0 ){
	set_pixel( x, y, red, green, blue );
      }
    }
  }
}

Vertex sub( Vertex a, Vertex b ){
  return (Vertex) {a.x - b.x, a.y - b.y, a.z - b.z};
}

float dot( Vertex a, Vertex b ){
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vertex cross( Vertex a, Vertex b ){
  return (Vertex) {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
}

Vertex normalize( Vertex v ){
  float len = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
  if ( len == 0 ) return (Vertex){0, 0, 0};
  return (Vertex){v.x/len, v.y/len, v.z/len};
}

Vertex scalar( float x, Vertex v ){
  return (Vertex){ x*v.x, x*v.y, x*v.z };
}

void render_faces_filled( Vertex *vertices, Face *faces, int vcount, int fcount, Vertex light, Vertex view_dir ){
  for (int i = 0; i < fcount; i++){
    Face face = faces[i];
    
    Vertex v0 = vertices[face.verts[0] - 1];
    Vertex v1 = vertices[face.verts[1] - 1];
    Vertex v2 = vertices[face.verts[2] - 1];

    // Vetor normal
    Vertex v1_v0 = sub( v1, v0 );
    Vertex v2_v0 = sub( v2, v0 );
    Vertex normal = normalize( cross( v2_v0, v1_v0 ) );

    // Coeficientes de phong
    float ka = 0.2; // ambiente
    float kd = 0.6; // difusa
    float ks = 0.4; // especular
    int brilho = 32;

    // Normalizar luz e direcao da camera
    Vertex L = normalize(light);
    Vertex V = normalize(view_dir);
    
    // Iluminacao difusa
    float diff = fmaxf(0, dot( normal, L ));
    
    // Iluminacao especular
    Vertex R = sub( scalar(2.0 * dot(normal,L), normal), L); // R = 2(N.L)N - L
    float spec = powf(fmaxf(0, dot(R, V)), brilho);
    
    // Intensidade final
    float intensity = ka + kd * diff + ks * spec;
    if ( intensity > 1.0 ) intensity = 1.0;
    
    // Rotacione os vertices (180 graus)
    rotate_z(&v0, M_PI);
    rotate_z(&v1, M_PI);
    rotate_z(&v2, M_PI);
    
    // Projecao 3D -> 2D
    project_3dto2d(&v0);
    project_3dto2d(&v1);
    project_3dto2d(&v2);

    barycentric_coordinate( v0, v1, v2, intensity*255, intensity*255, intensity*255 );
  }
}

int main(){

  Vertex vertices[MAX_VERTICES];
  Face faces[MAX_FACES];
  int vcount, fcount;
  
  clr();
  
  // Lê o arquivo OBJ enviado
  if (!load_obj("models/wolf.obj", vertices, &vcount, faces, &fcount)) {
    return 1;
  }
  
  Vertex light = {0, 0, -1}; //{0.25, 0.0, -0.75};
  Vertex view_dir = {0, 0, 1}; // Camera olhando para -z
  
  // Renderiza as faces no framebuffer
  render_faces_filled( vertices, faces, vcount, fcount, light, view_dir );
  
  save();
  
  return 0;
}
