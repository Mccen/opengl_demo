#ifndef WINDOW_HPP
#define WINDOW_HPP
#include"../headerLists.hpp"
#include "../utils/Single.hpp"
struct Window : Single{
    // 窗口宽高
    static int width, height;
    //opengl版本
    unsigned int openglMajor, openglMinor;
    const char* title;
    GLFWwindow* window;
    void createWindow(unsigned int width, unsigned int height, const char* title, unsigned int openglMajor, unsigned int openglMinor);
    static Window& getWindow(){
        static Window window;
        return window;
    }
    static void reshapeCallback(GLFWwindow* window, int newwidth, int newheight);
private:
    Window()=default;
};

#endif // WINDOW_HPP