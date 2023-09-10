// 2.使用结构和普通C函数定义对象

// Base.h // 通过文件来区分Base和Derived对象，不过本例过于简单，多个对象放在一个文件中，通过Base和Derived前缀区分也可以
struct Base;

typedef int(*FuncIsTrue)(struct Base *this);

typedef Base {
    int data;               // 结构里只有数据和虚函数，已经不是完整的对象了
    FuncIsTrue funcIsTrue;  // 虚函数也必须定义在结构中，但绝大多数的普通函数已经不需要使用函数指针了
};

// 嵌入式系统通常是固定分配的内存，所以额外增加了对象管理接口
extern int BaseAlloc();     // 装配Base对象的接口
extern void BaseFree(int id);
extern struct Base *BaseGet(int id);

extern void BaseSet(int id, int data); // Base对象的接口任然是C形式，通过id区分对象实例
extern int BaseIsTrue(int id); // 对虚函数接口进行了C形式的封装

// Base.c
struct Base g_base[10];

int BaseAllocId() {return 0;} // 对象管理功能，这里只是简单示例
void BaseFree(int id) {}
struct Base *BaseGet(int id){return &g_base[id];}

int BaseFuncIsTrue(struct Base *this) {return this->data != 0;} // Base的虚函数实现

int BaseAlloc() // Base对象的装配
{
    int id = BaseAllocId();
    struct Base *base = BaseGet(id);
    base->data = 0;
    base->funcIsTrue = BaseFuncIsTrue;
    return id;
}

void BaseSet(int id, int data) {g_base[id].data = data;} // C形式的普通函数接口
int BaseIsTrue(int id) {return g_base[id].funcIsTrue(&g_base[id]);} // C形式的虚函数接口

// Derived.h
extern int DerivedAlloc(); // 这里的子类对外接口只有装配Derived对象的接口

// Derived.h
int DerivedFuncIsTrue(struct Base *this) {return this->data > 0 && this->data < 10;} // 子类的虚函数实现

int DerivedAlloc() // 子类虚函数装配
{
    int id = BaseAlloc();
    struct Base *base = BaseGet(id);
    base->funcIsTrue = DerivedFuncIsTrue;
    return id;
}

// main.c
int main()
{
    int id = DerivedAlloc(); // 使用者看到的都是C形式的接口
    return BaseIsTrue(id);
}