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

unsigned char image[HEIGHT][WIDTH][3];

// Define a cor de um pixel
void set_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        image[y][x][0] = r;
        image[y][x][1] = g;
        image[y][x][2] = b;
    }
}

// Algoritmo de Bresenham
void draw_line(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;

    while (1) {
        set_pixel(x0, y0, 0, 0, 0);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
    }
}

// Escreve imagem em arquivo PPM (binário)
void write_ppm(const char *filename) {
    FILE *f = fopen(filename, "wb");
    fprintf(f, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
    fwrite(image, 1, WIDTH * HEIGHT * 3, f);
    fclose(f);
}

// Lê o arquivo OBJ
int load_obj(const char *filename, Vertex *vertices, int *vcount, Face *faces, int *fcount) {
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
            if (sscanf(line + 2, "%f %f %f", &vertices[*vcount].x, &vertices[*vcount].y, &vertices[*vcount].z) == 3) {
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

// Normaliza e projeta os vértices
void render_faces(Vertex *vertices, Face *faces, int vcount, int fcount) {
    for (int i = 0; i < fcount; i++) {
        Face face = faces[i];
        for (int j = 0; j < face.n; j++) {
            Vertex v0 = vertices[face.verts[j] - 1];
            Vertex v1 = vertices[face.verts[(j + 1) % face.n] - 1];

            int x0 = (int)((v0.x + 1.0f) * WIDTH / 2.0f);
            int y0 = (int)((1.0f - v0.y) * HEIGHT / 2.0f);
            int x1 = (int)((v1.x + 1.0f) * WIDTH / 2.0f);
            int y1 = (int)((1.0f - v1.y) * HEIGHT / 2.0f);

            draw_line(x0, y0, x1, y1);
        }
    }
}

int main() {
    Vertex vertices[MAX_VERTICES];
    Face faces[MAX_FACES];
    int vcount, fcount;

    // Limpa a imagem (branco)
    memset(image, 255, sizeof(image));

    // Lê o arquivo OBJ enviado
    if (!load_obj("models/cranio.obj", vertices, &vcount, faces, &fcount)) {
        return 1;
    }

    // Renderiza as faces no framebuffer
    render_faces(vertices, faces, vcount, fcount);

    // Salva o PPM
    write_ppm("saida.ppm");

    printf("Imagem gerada: saida.ppm\n");
    return 0;
}
