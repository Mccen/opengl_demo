## 使用cmake构建的opengl环境

### 构建

暂未进行windows之外的构建支持

构建工具：

cmake 3.10+

mingw64 gcc

拉取仓库  `git clone https://github.com/Mccen/opengl_demo.git`

进入项目目录

创建并进入构建目录

`mkdir build`

`cd build ..`

`cmake  -G "MinGW Makefiles" ..`

`make`

`make install`

`cd ../output`

`demo.exe`


### 操作说明
1.使用wasd进行四向移动，使用space和c进行上下移动（相对于相机目视方向）

2.移动鼠标控制相机视角，拨动滚轮改变视野范围

3，按下esc可关闭窗口，按住左ctl+m可释放/锁定鼠标

4.按住左shift可加快移动速度（注意输入法模式）