#ifndef RENDER_HPP
#define RENDER_HPP
#include"../utils/Mesh.hpp"
#include"../utils/Vector.hpp"
struct Renserder{
    //要渲染的顶点数据
    Vector<GLfloat, 1024> vertices;
    //EBO索引顺序
    float ebo_index[6] = {0,1,2,2,3,0};

};

#endif