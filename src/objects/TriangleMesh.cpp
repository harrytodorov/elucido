//
// Created by Haralambi Todorov on 05/07/2017.
//

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include "TriangleMesh.h"

loading_info TriangleMesh::load_mesh(const char *f) {
    std::string type;               // type of the object read on the current line
    std::string line;               // one line of the file
    std::vector<glm::vec4> tva;     // temporary array to store vertices
    std::vector<glm::vec4> tvna;    // temporary array to store vertex normals
    std::vector<uint32_t> vi;       // vertex index array
    std::vector<uint32_t > ni;      // normal index array
    uint32_t nf(0);                 // number of faces
    uint32_t nt(0);                 // number of triangles
    loading_info ret;               // holder for the loading information

    // load file in memory
    std::ifstream fs(f);

    // if we fail to load file from memory, exit the function
    if (fs.fail())
        return ret;

    // read the file line by line
    while (std::getline(fs, line)) {
        // convert the line into tokenizable object
        std::stringstream tokens(line);

        // read what is the type of the object that had to be initialized
        // v:   vector
        // vn:  vector normal
        // f:   face
        tokens >> type;

        // in case we have a vertex
        if (type == vertex) {
            // x, y and z components of the vertex
            float_t vx, vy, vz;
            tokens >> vx >> vy >> vz;

            // add vertex to temporary vertex array
            tva.push_back(glm::vec4(vx, vy, vz, 1));

        // in case we have a vertex normal
        } else if (type == vertex_normal) {
            float_t vnx, vny, vnz;          // x, y and z components of the vertex normal
            tokens >> vnx >> vny >> vnz;

            // add vertex normal to temporary vertex normal array
            tvna.push_back(glm::vec4(vnx, vny, vnz, 0));

        // in case we have a face
        } else if (type == face) {
            std::string fp;                 // temporary holder for face information
            std::string d = "/";            // d: d
            std::string vd, cv;             // vd: vertex dummy, used to store information for the extracted vertex,
                                            // vertex normal or vertex texture
                                            // cv: current vertex
            std::vector<uint32_t> vertex_index, vertexnormal_index;
            uint32_t nv(0);                 // number of vertices in the face

            // extract  information
            while (tokens >> fp) {
                // extract vertex index information
                cv = fp;
                vd = cv.substr(0, cv.find(d));
                vertex_index.push_back((unsigned int) std::stoi(vd));

                // extract vertex texture information
                // TODO: for now we just skip this information, if any
                cv.erase(0, cv.find(d) + d.length());
                vd = cv.substr(0, cv.find(d));

                // extract vertex normal information
                cv.erase(0, cv.find(d) + d.length());
                vd = cv.substr(0, cv.find(d));
                vertexnormal_index.push_back((unsigned int) std::stoi(vd));

                nv++;
            }

            // generate 1 or multiple triangles for each face; depending on vertices number
            // each face has #_of_vertices - 2 triangles
            for (int t = 0; t < (nv - 2); t++) {
                // vertex indices for the triangle
                uint32_t v0, v1, v2;
                v0 = vertex_index.at(0);
                v1 = vertex_index.at(t + 1);
                v2 = vertex_index.at(t + 2);
                vi.push_back(v0);
                vi.push_back(v1);
                vi.push_back(v2);

                // vertex normal indices for the triangle
                uint32_t vn0, vn1, vn2;
                vn0 = vertexnormal_index.at(0);
                vn1 = vertexnormal_index.at(t + 2);
                vn2 = vertexnormal_index.at(t + 1);
                ni.push_back(vn0);
                ni.push_back(vn1);
                ni.push_back(vn2);

                nt++;
            }

            nf++;
        }
    }

    ret.num_vertices = (uint32_t) tva.size();
    ret.num_of_vn = (uint32_t) tvna.size();
    ret.num_of_faces = nf;
    ret.num_of_triangles = nt;

    // initialize a triangle for each face
    for (int ti = 0; ti < nt; ti++) {
        // get vertices
        glm::vec4 v0(tva[vi[3 * ti] - 1]);
        glm::vec4 v1(tva[vi[3 * ti + 1] - 1]);
        glm::vec4 v2(tva[vi[3 * ti + 2] - 1]);

        // get vertex normals
        glm::vec4 vn0(tvna[ni[3 * ti] - 1]);
        glm::vec4 vn1(tvna[ni[3 * ti + 1] - 1]);
        glm::vec4 vn2(tvna[ni[3 * ti + 2] - 1]);

        ts.push_back(Triangle(v0, v1, v2, vn0, vn1, vn2, om));
    }

    return ret;
}

bool TriangleMesh::intersect(const Ray &r, float_t &t, glm::vec4 &p_hit, uint32_t &ti) {
    bool intersected = false;

    // intersect with all triangles in the triangulated mesh
    for (std::vector<Triangle>::iterator it = ts.begin(); it != ts.end(); ++it) {
        if (it->intersect(r, t, p_hit, ti)) {
            intersected = true;
            ti = (uint32_t) (it - ts.begin());
        }
    }

    return intersected;
}

void TriangleMesh::get_surface_properties(const glm::vec4 &hit_point, const glm::vec4 &view_direction, const uint32_t &triangle_index,
                                          glm::vec4 &hit_normal) {
    ts.at(triangle_index).get_surface_properties(hit_point, view_direction, triangle_index, hit_normal);
}

void TriangleMesh::apply_camera_transformation(glm::mat4 &t) {
    for (std::vector<Triangle>::iterator it = ts.begin(); it != ts.end(); ++it) {
        it->apply_camera_transformation(t);
    }

}

void TriangleMesh::apply_transformations() {
    for (std::vector<Triangle>::iterator it = ts.begin(); it != ts.end(); ++it) {
        it->apply_transformations();
    }
}

void TriangleMesh::translate(const float_t &translation, const uint32_t &axes_of_translation) {
    for (std::vector<Triangle>::iterator it = ts.begin(); it != ts.end(); ++it) {
        it->translate(translation, axes_of_translation);
    }
}

void TriangleMesh::rotate(const float_t &angle_of_rotation, const uint32_t &axes_of_rotation) {
    for (std::vector<Triangle>::iterator it = ts.begin(); it != ts.end(); ++it) {
        it->rotate(angle_of_rotation, axes_of_rotation);
    }
}

void TriangleMesh::scale(const float_t &scaling_factor, const uint32_t &axes_of_scale) {
    for (std::vector<Triangle>::iterator it = ts.begin(); it != ts.end(); ++it) {
        it->scale(scaling_factor, axes_of_scale);
    }
}
