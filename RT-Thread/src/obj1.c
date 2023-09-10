// 1.使用结构和函数指针模仿C++
/* 对应的C++实现
class Base {
    void Set(int data) {m_data = data;}
    virtual int IsTrue() {return m_data != 0;}
private:
    int m_data = 0;
};

Class Derived : pubilc Base {
pubilc:
    intIsTrue() override {retrue m_data > 0 && m_data < 10;}
};
*/

struct Base;
typedef void (*FunSet)(struct Base *this, int data); // 需要给每个函数额外增加this指针作为参数
typedef int(*FuncIsTrue)(struct Base *this);

typedef Base {              // 将类转换成结构
    int data;               // 无法区分private
    FuncSet funcSet;        // 普通函数成员也只能使用函数指针
    FuncIsTrue funcIsTrue;  // 虚函数也是函数指针，不易区分
};

struct Derived {        // 子类也是结构
    struct Base base;   // 子类以父类结构开始，可以通过替换base中函数指针的内容，实现虚函数功能
                        // 后面可以增加子类特有内容
};

void BaseFuncSet(struct Base *this, int data) {this->data = data}
int BaseFuncIsTrue(struct Base *this) {return this->data > 0 && this->data < 10;}

int main()
{
    struct Base base = (0, BaseFuncSet, BaseFuncIsTrue); //使用前需要进行装配
    struct Derived derived = {{base.data, base.funcSet, DerivedFuncIsTrue}}; // 装配时可以替换虚函数指针

    return derived.base.funcIsTrue((struct Base*)&derived);
}