#ifndef MESH_HPP
#define MESH_HPP

#include "../headerLists.hpp"
#include "Vector.hpp"

struct Mesh
{
    Vector<GLfloat, 12> positions;
    Vector<GLfloat, 8> texCoords;
    Vector<GLfloat, 3> normals;
};

#endif