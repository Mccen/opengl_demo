#ifndef AABB_HPP
#define AABB_HPP
#include"../headerLists.hpp"
struct AABB{

    AABB() = default;
    AABB(glm::vec3 min, glm::vec3 max) : min(min), max(max) {}
    bool intestine(const AABB &aabb){
        return !(aabb.min.x > max.x || aabb.max.x < min.x || aabb.min.y > max.y || aabb.max.y < min.y || aabb.min.z > max.z || aabb.max.z < min.z);
    }
    bool containPoint(const glm::vec3& point)
{
    return point.x >= min.x && point.x <= max.x &&
           point.y >= min.y && point.y <= max.y &&
           point.z >= min.z && point.z <= max.z;
}
    void transformAABB(glm::mat4 transform){
        glm::vec3 newMin = transform * glm::vec4(min, 1.0f);
        glm::vec3 newMax = transform * glm::vec4(max, 1.0f);
        min = glm::min(newMin, newMax);
        max = glm::max(newMin, newMax);
    }
    glm::vec3 getMin(){
        return min;
    }
    glm::vec3 getMax(){
        return max;
    }
private:
    glm::vec3 min, max;
};

#endif