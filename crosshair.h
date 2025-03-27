//
// Created by pknadimp on 3/26/25.
//

#ifndef CROSSHAIR_H
#define CROSSHAIR_H

#include "mesh_generator.h"

namespace renderer {
    class CrossHair {
    public:
        CrossHair() = default;

        ~CrossHair() {
            delete_mesh();
        }

        void initialize();

        const Mesh &get_mesh() const;

    private:
        void create_mesh();

        void delete_mesh() const;

    private:
        Mesh m_mesh;
    };
}

#endif //CROSSHAIR_H
