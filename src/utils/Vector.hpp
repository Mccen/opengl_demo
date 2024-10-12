#ifndef VECTOR_HPP
#define VECTOR_HPP
/**
 * Vector是一个模仿std::vector的结构体,用以实现动态存储和按索引快速读取
 * @class Vector
 * @tparam C 存储的类型
 * @tparam N 存储的最大长度
 */
template <class C, int N>
struct Vector
{
    // 指向动态数组的指针
    C *m_v;

    // 默认构造函数，初始化动态数组
    Vector()
    {
        // 为动态数组分配内存
        this->m_v = new C[N];
        // 记录数组的长度
        this->len = N;
    }

    // 重载下标运算符，用于访问和修改数组元素
    C &operator[](const size_t& index)
    {
        // 如果索引超出当前数组长度
        if (index > this->len)
        {
            // 保存旧数组指针
            C *temp = this->m_v;
            // 重新分配更大的内存空间
            this->m_v = new C[index + 1];
            // 复制旧数组元素到新数组
            for (int i = 0; i < this->len; i++)
            {
                this->m_v[i] = temp[i];
            }
            // 释放旧数组内存
            delete[] temp;
            // 更新数组长度
            this->len = index + 1;
        }
        // 返回索引位置的元素引用
        return this->m_v[index];
    }

    // 析构函数，释放动态数组内存
    ~Vector() = default;

    // 清除动态数组的内容并释放内存
    void clear()
    {
        // 释放动态数组内存
        delete[] this->m_v;
    }

    // 获取动态数组的长度
    GLuint getSize(){
        return this->len;
    }

private:
    // 记录动态数组的总长度
    GLuint len;
};

#endif