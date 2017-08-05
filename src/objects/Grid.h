//
// Created by Haralambi Todorov on 27/07/2017.
//

#ifndef ELUCIDO_GRID_H
#define ELUCIDO_GRID_H


#include "TriangleMesh.h"

class Grid {

    struct triangle_description {
        // data members
        const TriangleMesh *m;          // pointer to the triangulated mesh to which the triangle is part of
        uint32_t ti;                    // index of the triangle in the triangulated mesh

        // constructors
        triangle_description(const TriangleMesh *tm, const uint32_t &_ti) : m(tm), ti(_ti) {}
    };

    struct cell {
        // data members
        std::vector<triangle_description> tris;

        // constructors
        cell() {}

        // methods
        void insert(const TriangleMesh *m, const uint32_t &ti) {
            tris.push_back(triangle_description(m, ti));
        }

        bool intersect(const Ray &r, const TriangleMesh *&im, isect_info &ii) {
            // iterate through all triangles in the cell
            for (uint32_t i = 0; i < tris.size(); i++) {

                // get the mesh to which the triangle belongs
                const TriangleMesh *mesh = tris[i].m;

                // check for intersection
                if (mesh->intersect(r, tris[i].ti, ii)) {
                    im = mesh;
                }
            }
            return (im != nullptr);
        }
    };

public:
    Grid(std::vector<std::unique_ptr<const TriangleMesh>> &m);

    ~Grid() {
        for (uint32_t i = 0; i < r.x*r.y*r.z; ++i) {
            if (cells[i] != NULL) delete cells[i];
        }
        delete [] cells;
    }

    cell **cells;
    AABBox bb;      // bounding box for the whole grid
    glm::vec3 r;    // resolution of grid
    glm::vec3 cd;   // dimensions of a cell
};


#endif //ELUCIDO_GRID_H
