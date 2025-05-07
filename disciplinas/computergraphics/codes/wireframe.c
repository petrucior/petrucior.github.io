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
