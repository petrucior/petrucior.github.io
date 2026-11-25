/**
 * \file zbuffer.c
 *
 * \brief Implementacao do zbuffer na renderizacao 3D.
 *
 * \author
 * Petrucio Ricardo Tavares de Medeiros \n
 * Universidade Federal Rural do Semi-Arido \n
 * Departamento de Engenharias e Tecnologia \n
 * petrucio at ufersa (dot) edu (dot) br
 *
 * \version 1.0
 * \date Nov 2026
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
  int x, y;
  float z; // profundidade (distancia ate a camera)
}Point2D;

typedef struct {
  float x, y, z;
} Vertex;

typedef struct {
  int verts[MAX_FACE_VERTS];
  int n;
} Face;

// Estrutura da câmera                                                                                                                                          
typedef struct {
  Vertex Peye; // Posicao da camera no mundo
  Vertex Pat;  // Ponto para onde a camera está olhando
  Vertex Vup;  // Vetor up (0, 1, 0)
  float d;     // Distância entre o centro de projecao
} Camera;


unsigned char image[WIDTH][HEIGHT][3];
float zbuffer[WIDTH][HEIGHT]; // z-buffer: menor z, entao mais proximo da camera

void set_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
  if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
    image[y][x][0] = r;
    image[y][x][1] = g;
    image[y][x][2] = b;
  }
}

// desenha o pixel apenas se ele estiver mais proximo da camera
void set_pixel_z(int x, int y, float z, unsigned char r, unsigned char g, unsigned char b) {
  if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && z > zbuffer[y][x]) {
    zbuffer[y][x] = z;
    set_pixel( x, y, r, g, b );
  }
}

void draw_line(int x0, int y0, int x1, int y1) {
  for (float t = 0.0; t < 1.0; t = t + 0.0001)
    set_pixel((int)x0+(x1-x0)*t, (int)y0+(y1-y0)*t, 0, 0, 0);
}

void clr(){
  for(int i = 0; i < WIDTH; i++){
    for(int j = 0; j < HEIGHT; j++){
      zbuffer[i][j] = 0.0; // iniciando com menor valor
      for(int c = 0; c < 3; c++){
	image[i][j][c] = 255;
      }
    }
  }
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

void save_zbuffer_ppm(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Erro ao criar imagem do z-buffer");
        return;
    }

    float min_z = 1e30f;
    float max_z = -1e30f;

    // Coleta o intervalo real dos valores preenchidos (z > 0.0f)
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            float z = zbuffer[y][x];
            if (z > 0.0f) {
                if (z < min_z) min_z = z;
                if (z > max_z) max_z = z;
            }
        }
    }

    fprintf(fp, "P3\n%d %d\n255\n", WIDTH, HEIGHT);

    float range = max_z - min_z;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            float z = zbuffer[y][x];
            int gray = 0; // fundo sem geometria fica preto

            if (z > 0.0f) {
                float norm = (range > 1e-6f) ? ((z - min_z) / range) : 1.0f;
                // Como z = 1/w é maior quanto mais próximo da câmera:
                // norm próximo de 1 => mais próximo (mais claro)
                gray = (int)(norm * 255.0f);
                if (gray < 0) gray = 0;
                if (gray > 255) gray = 255;
            }

            fprintf(fp, "%d %d %d\n", gray, gray, gray);
        }
    }

    fclose(fp);
}

int load_obj(const char *filename, Vertex *vertices, int *vcount,
	     Face *faces, int *fcount) {
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

// funcoes auxiliares
static Vertex sub( Vertex a, Vertex b ){
  return (Vertex) {a.x - b.x, a.y - b.y, a.z - b.z};
}

static float dot( Vertex a, Vertex b ){
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

static Vertex cross( Vertex a, Vertex b ){
  return (Vertex){
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x
  };
}

static Vertex normalize( Vertex v ){
  float tam = sqrt( dot( v, v ) );
  if ( tam == 0.0f ) return (Vertex){0, 0, 0};
  else return (Vertex){ v.x/tam, v.y/tam, v.z/tam };
}

Vertex scalar( float x, Vertex v ){
  return (Vertex){ x*v.x, x*v.y, x*v.z };
}

// Transformação de camera e projeção
void resizing( Vertex p, Camera cam, Point2D *out ){
  
  // Obtencao da base ortonormal n, u e v
  Vertex n = normalize( sub( cam.Peye, cam.Pat ) );
  Vertex u = normalize( cross( cam.Vup, n ) );
  Vertex v = cross( n, u );

  // Aplicacao da Mview
  // p_cam = Mview * p
  Vertex p_eye = sub( p, cam.Peye );
  float x_cam = dot( u, p_eye );
  float y_cam = dot( v, p_eye );
  float z_cam = dot( n, p_eye );

  float w = z_cam + cam.d;
  if ( w <= 0.1f ) return ;
  
  // divisor da projeção em perspectiva
  out->z = 1.0f / w;

  float x_proj = (x_cam * cam.d) / w;
  float y_proj = (y_cam * cam.d) / w;

  out->x = (int)((x_proj + 1.0f) * WIDTH / 2.0f);
  out->y = (int)((1.0f - y_proj) * HEIGHT / 2.0f);

}

void barycentric_coordinate( Point2D a, Point2D b, Point2D c, unsigned char red,
			     unsigned char green, unsigned char blue ){
  // calculando o bounding box
  int x_min = (int)fminf(fminf(a.x, b.x), c.x);
  int x_max = (int)fmaxf(fmaxf(a.x, b.x), c.x);
  int y_min = (int)fminf(fminf(a.y, b.y), c.y);
  int y_max = (int)fmaxf(fmaxf(a.y, b.y), c.y);

  // Encontrando a Area do triangulo abc
  double area_abc = 0.5 * (a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y));
  
  if ( fabs(area_abc) < 1.0 ) return;
  
  for (int y = y_min; y <= y_max; y++){
    for (int x = x_min; x <= x_max; x++){
      Vertex p = {x, y, 0};
      // Encontrando a Area dos triangulos
      double area_pbc = 0.5 * (p.x*(b.y - c.y) + b.x*(c.y - p.y) + c.x*(p.y - b.y));
      double area_apc = 0.5 * (a.x*(p.y - c.y) + p.x*(c.y - a.y) + c.x*(a.y - p.y));
      double area_abp = 0.5 * (a.x*(b.y - p.y) + b.x*(p.y - a.y) + p.x*(a.y - b.y));
      double alfa = area_pbc / area_abc;
      double beta = area_apc / area_abc;
      double gamma = area_abp / area_abc;
      if ( alfa >= 0.0 && beta >= 0.0 && gamma >= 0.0 ){
	// interpolacao da profundidade com as coordenadas baricentricas
	float z = alfa * a.z + beta * b.z + gamma * c.z;
	set_pixel_z( x, y, z, red, green, blue );
      }
    }
  }
}


void render_faces_filled( Vertex *vertices, Face *faces, int vcount, int fcount, Camera cam ){

  Vertex light = {0.25, 0.0, -0.75}; //{0.25, 0.0, -0.75};
  Vertex view_dir = {0.0, 0.0, -1.0}; // Camera olhando para -z
  
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
    float ka = 0.3; // ambiente
    float kd = 0.6; // difusa
    float ks = 0.4; // especular
    int brilho = 80;

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

    Point2D p0, p1, p2;
    resizing( v0, cam, &p0 );
    resizing( v1, cam, &p1 );
    resizing( v2, cam, &p2 );

    barycentric_coordinate( p0, p1, p2, 255*intensity, 255*intensity, 255*intensity );
  }
}

void render_faces(Vertex *vertices, Face *faces, int vcount, int fcount, Camera cam) {
    for (int i = 0; i < fcount; i++) {
        Face face = faces[i];

	Vertex v0 = vertices[face.verts[0] - 1];
	Vertex v1 = vertices[face.verts[1] - 1];
	Vertex v2 = vertices[face.verts[2] - 1];
	
	Point2D p0, p1, p2;
	resizing( v0, cam, &p0 );
	resizing( v1, cam, &p1 );
	resizing( v2, cam, &p2 );
	
	barycentric_coordinate( p0, p1, p2, rand()%255, rand()%255, rand()%255 );
	//draw_line(p0.x, p0.y, p1.x, p1.y);
    }
}


int main(){

  Vertex vertices[MAX_VERTICES];
  Face faces[MAX_FACES];
  int vcount, fcount;
  
  clr();
  
  // Le o arquivo OBJ enviado
  if (!load_obj("models/wolf.obj", vertices, &vcount, faces, &fcount)) {
    return 1;
  }

  Camera cam = {
    .Peye = {0, 0, -0.8},
    .Pat = {0, 0, 0},
    .Vup = {0, 1, 0},
    .d = 7.0
  };
  
  // Renderiza as faces no framebuffer
  //render_faces(vertices, faces, vcount, fcount, cam);
  render_faces_filled(vertices, faces, vcount, fcount, cam);
  
  save();
  save_zbuffer_ppm("output_buffer.ppm");
  
  return 0;
}
