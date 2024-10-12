#ifndef SINGLE_HPP
#define SINGLE_HPP
// 定义一个名为 Single 的结构体, 定义单例模式
struct Single{
    // 禁用拷贝构造函数，防止拷贝实例
    Single(const Single&) = delete;
    // 禁用赋值操作符，防止赋值实例
    Single& operator=(const Single&) = delete;
protected:
    // 定义一个保护的构造函数，防止外部创建实例
    Single()=default;
    // 定义一个保护的析构函数，防止外部销毁实例
    ~Single()=default;
};

#endif // SINGLE_HPP