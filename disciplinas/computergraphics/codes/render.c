/**
 * \file render.c
 *
 * \brief Implementação do arquivo principal de renderização do modelo 3D, câmera
 * virtual e projeção em perspectiva.
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

#include "camera.h"

int main() {
    Vertex vertices[MAX_VERTICES];
    Face faces[MAX_FACES];
    int vcount, fcount;

    clr();

    // Lê o arquivo OBJ enviado
    if (!load_obj("models/wolf.obj", vertices, &vcount, faces, &fcount)) {
        return 1;
    }

    // Constroi uma câmera
    Camera cam = {
      .Peye = {0, 0, -0.5},
      .Pat = {0, 0, 0},
      .Vup = {0, 1, 0},
      .d = 5.0
    };
    
    // Renderiza as faces no framebuffer
    render_faces(vertices, faces, vcount, fcount, cam);
    
    save();
    
    return 0;
}
