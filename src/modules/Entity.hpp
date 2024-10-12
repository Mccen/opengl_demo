#ifndef ENTITY_HPP
#define ENTITY_HPP
#include"../headerLists.hpp"
// 定义一个名为 Entity 的结构体，用于表示一个实体的基本属性
struct Entity{
    // 实体的位置，使用 glm::vec3 表示三维向量
    glm::vec3 e_position;
    // 实体的目标位置，使用 glm::vec3 表示三维向量
    glm::vec3 e_target;
    // 实体的向上方向，使用 glm::vec3 表示三维向量
    glm::vec3 e_up;
    // 实体的移动速度，默认为 0.1f
    float e_speed=0.1f;
};


#endif // ENTITY_HPP