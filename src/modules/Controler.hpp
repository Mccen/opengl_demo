#ifndef CONTROLER_HPP
#define CONTROLER_HPP

#include "../headerLists.hpp"
#include "../utils/Single.hpp"
#include "Camera.hpp"
struct Camera;
struct Controler : Single
{
    // 鼠标移动速度
    float mouseSpeed = 0.1f;
    // 鼠标滚轮速度
    float mouseWheelSpeed = 1.0f;
    // 是否捕获鼠标
    bool mouseGet = true;
    // 鼠标是否首次移动/被捕获
    bool mouse_is_first = true;
    // 是否首次按下
    bool keyFirst = true;
    // 鼠标最新位置
    double lastX = Window::width / 2, lastY = Window::height / 2;
    // 时间戳
    float deltaTime = 0.0f;
    float lastTime = 0.0f;

    // 获取鼠标移动事件
    static void getMouseMoveEvent(GLFWwindow *window, double xpos, double ypos);
    // 获取鼠标滚轮事件
    static void getMouseWheelEvent(GLFWwindow *window, double xoffset, double yoffset);
    // 更新相机视角
    void updateCamera(float yaw, float pitch);

    // 获取键盘事件
    static void getKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods);

    // 获取键盘持续事件
    void getKeyEventLongTime(GLFWwindow *window);
    // 更新相机矩阵
    void updateCameraMatrix();
    // 获取鼠标按钮事件
    static void getMouseButtonEvent(GLFWwindow *window, int button, int action, int mods){};

    // 获取鼠标按钮持续事件
    void getMouseButtonEventLongTime(GLFWwindow *window){};

    //更新时间戳
    void updateTime();

    // 获取控制器实例
    static Controler &getControler()
    {
        static Controler controler;
        return controler;
    }

private:
    Controler() = default;
};

#endif // CONTROLER_HPP