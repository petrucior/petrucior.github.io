/**
 * \file wireframe.c
 *
 * \brief Implementação do arquivo principal de renderização do modelo 3D.
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

#include "model.h"

int main() {
    Vertex vertices[MAX_VERTICES];
    Face faces[MAX_FACES];
    int vcount, fcount;

    clr();

    // Lê o arquivo OBJ enviado
    if (!load_obj("models/cranio.obj", vertices, &vcount, faces, &fcount)) {
        return 1;
    }

    // Renderiza as faces no framebuffer
    render_faces(vertices, faces, vcount, fcount);

    save();
    
    return 0;
}
