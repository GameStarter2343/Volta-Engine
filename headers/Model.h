#pragma once

#include "../external/obj/tiny_obj_loader.h"
#include <string>
#include <vector>

class Model {
public:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int vertexCount = 0;

    Model(const std::vector<float>& verts);
    Model(const std::string& path);
    ~Model();

    void SetupMesh(const std::vector<float>& verts);
    void Draw() const;

    unsigned int VertexCount() const;
};
