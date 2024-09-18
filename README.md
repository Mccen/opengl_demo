# 使用cmake构建的opengl环境

### 构建

#### 特别说明：
在linux下，glfw需要一些依赖才能编译，以下来自glfw官网：

On Debian and derivatives like Ubuntu and Linux Mint you will need the libwayland-dev and libxkbcommon-dev packages to compile for Wayland and the xorg-dev meta-package to compile for X11. These will pull in all other dependencies.

`sudo apt install libwayland-dev libxkbcommon-dev xorg-dev`

On Fedora and derivatives like Red Hat you will need the wayland-devel and libxkbcommon-devel packages to compile for Wayland and the libXcursor-devel, libXi-devel, libXinerama-devel and libXrandr-devel packages to compile for X11. These will pull in all other dependencies.

`sudo dnf install wayland-devel libxkbcommon-devel libXcursor-devel libXi-devel libXinerama-devel libXrandr-devel`

On FreeBSD you will need the wayland, libxkbcommon and evdev-proto packages to compile for Wayland. The X11 headers are installed along the end-user X11 packages, so if you have an X server running you should have the headers as well. If not, install the xorgproto package to compile for X11.

`pkg install wayland libxkbcommon evdev-proto xorgproto`

On Cygwin Wayland is not supported but you will need the libXcursor-devel, libXi-devel, libXinerama-devel, libXrandr-devel and libXrender-devel packages to compile for X11. These can be found in the Libs section of the GUI installer and will pull in all other dependencies.

#### 构建工具：
cmake 3.10+<br>
make/ninja<br>
mingw64/gcc 10.0+（若使用vs直接打开文件夹即可）

进入项目文件夹

创建构建文件夹：
`mkdir build`

进入构建文件夹：
`cd build`

cmake构建（视目标平台和构建工具而定）：
`cmake -G "MinGW Makefiles" ..`

make构建：
`make`

make 安装：
`make install`

进入可执行文件文件夹：
`cd output`

运行：
`./demo`

# 操作说明
1.使用wasd进行四向移动，使用space和c进行上下移动（相对于相机目视方向）

2.移动鼠标控制相机视角，拨动滚轮改变视野范围

3，按下esc可关闭窗口，按住左ctl+m可释放/锁定鼠标

4.按住左shift可加快移动速度（注意输入法模式）

### 本项目用到的第三方库如下
