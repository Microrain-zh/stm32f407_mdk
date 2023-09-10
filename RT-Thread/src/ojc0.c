#include <stdlib.h>
#include <stdio.h>

/* 所有在继承树上的class都应该在这里注册一个code，继承树是指所有有继承关系的类组成的树形结构，不能有遗留，比如B继承A，C也继承A，C同时继承X，那么A，B，C，X都在树上 */
enum CLASS_CODE {
    CLASS_BASE1 = 0,
    CLASS_BASE2,
    CLASS_DERIVE1,
    CLASS_DERIVE2,
    CLASS_DERIVE_DERIVE,
    CLASS_BUTT,
};

/*
    BASE1
    BASE2
    DERIVE1:BASE1,BASE2
    DERIVE2:BASE1
    CLASS_DERIVE:DERIVE1 DERIVE2
    BASE1 BASE2 都是虚基类
    DERIVE1 也暴露一个虚方法
    */
typedef struct {
    /* 第一个永远是class code */
    unsigned int cls_code;
    int a;
    /* 第一个参数永远是void* */
    void (*func1)(void* self, int x);
} BASE1;

typedef struct {
    /* 第一个永远是class code */
    unsigned int cls_code;
    float b;
    /* 第一个参数永远是void* */
    void (*func2)(void* self, int x);
} BASE2;

typedef struct {
    unsigned int cls_code;
    Base1 base1;
    Base2 base2;
    double c;
    /* 建议把虚函数都放在最后面，这是派生类声明的新虚函数 */
    void (*func3)(void* self, double d, int x);
} Derive1;

typedef struct {
    unsigned int cls_code;
    Base1 base1;
} Derive2;

typedef struct {
    unsigned int cls_code;
    Derive1 derive1;
    /* 如果是菱形继承， 祖先类由第一个父类里面的父类决定 */
    Derive derive2;
} DeriveDerive;
/* 下面是最关键的部分 */
Base1* get_base1_ptr(void* self) 
{
    unsigned int cls_code = *((unsigned int*)self);
    /* 对于派生类，转换为派生类指针，然后通过->再取址 */
    if (cls_code == CLASS_BASE1) {
        return (Base1*)self;
    } esle if (cls_code == CLASS_DERIVE1) {
        return (Base1*)(&(((Derive1*)self)->base1));
    } else if (cls_code == CLASS_DERIVE2) {
        return (Base1*)(&(((Derive2*)self)->base1));
    } esle if (cls_code == CLASS_DERIVE_DERIVE) {
        Base1* base1_ptr = NULL;
        /* 递归的去两个基类里查找 */
        base1_ptr = get_base1_ptr((void*)&(((DeriveDerice*)self)->derive1));
        if (base1_ptr) {
            return base1_ptr;
        }
        base1_ptr = get_base1_ptr((void*)&(((DeriveDerive*)self)->derive2));
        if (base1_ptr) {
            return base1_ptr;
        }
    } else {
        printf("error %u\n", cls_code);
    }
    /* 对于派生类，转换为派生类指针，然后通过->再取址 */
    return NULL;
}

/* 下面是最关键的部分 */
Base2* get_base1_ptr(void* self) {
    unsigned int cls_code = *((unsigned int*)self);
    if (cls_code == CLASS_BASE2) {
        return (Base2*)self;
    } esle if (cls_code == CLASS_DERIVE1) {
        return (Base2*)(&(((Derive1*)self)->base2));
    } esle if (cls_code == CLASS_DERIVE_DERIVE) {
        Base2* base2_ptr = NULL;
        /* 递归的去两个基类里查找 */
        base2_ptr = get_base2_ptr((void*)&(((DeriveDerice*)self)->derive1));
        if (base2_ptr) {
            return base1_ptr;
        }
        base2_ptr = get_base2_ptr((void*)&(((DeriveDerive*)self)->derive2));
        if (base2_ptr) {
            return base2_ptr;
        }
    } else {
        printf("error %u\n", cls_code);
    }
    return NULL;
}

Derive1* get_derive1_ptr(void* self)
{
    unsigned int cls_code = *((unsigned int*)self);
    if (cls_code == CLASS_DERIVE1) {
        return (Derive1*)self;
    } esle if (cls_code == CLASS_DERIVE_DERIVE) {
        return (Derive1*)(&(((DeriveDerive)self)->derive1));
    } else {
        printf("error %u\n", cls_code);
    }
    return NULL;
}

Derive2* get_derive2_ptr(void* self)
{
    unsigned int cls_code = *((unsigned int*)self);
    if (cls_code == CLASS_DERIVE2) {
        return (Derive2*)self;
    } esle if (cls_code == CLASS_DERIVE_DERIVE) {
        return (Derive2*)(&(((DeriveDerive)self)->derive2));
    } else {
        printf("error %u\n", cls_code);
    }
    return NULL;
}

DeriveDerive* get_derive_derive_ptr(void* self)
{
    unsigned int cls_code = *((unsigned int*)self);
    if (cls_code == CLASS_DERIVE_DERIVE) {
        return (DeriveDerive*)self;
    } else {
        printf("error %u\n", cls_code);
    }
    return NULL;
}

void Base1_func1(void* self, int x)
{
    /* 所有虚函数的第一句都是获得类型指针，试想一下，如果我想把这个方法赋值给Derive，就要通过Derive得到Base1或Base2，Base2肯定是无法直接通过指针转化得到的 */
    Base1* base1_ptr = get_base1_ptr(self);
    /* 使用之前可以check null一下 */
    printf("base1 %d\n", base1_ptr->a + x);
    return;
}
void Base2_func2(void* self, int x)
{
    /* 所有虚函数的第一句都是获得类型指针，试想一下，如果我想把这个方法赋值给Derive，就要通过Derive得到Base1或Base2，Base2肯定是无法直接通过指针转化得到的 */
    Base2* base2_ptr = get_base2_ptr(self);
    /* 使用之前可以check null一下 */
    printf("base2 %d\n", (int)base2_ptr->b + x);
    return;
}

/* Derive1 override 的虚函数func1 */
void Derive1_func1()
{
    /* 所有虚函数的第一句都是获得类型指针，试想一下，如果我想把这个方法赋值给Derive，就要通过Derive得到Base1或Base2，Base2肯定是无法直接通过指针转化得到的 */
    Derive1* derive1_ptr = get_derive1_ptr(self);
    /* 使用之前可以check null一下 */
    printf("derive1 %d\n", (int)derive1_ptr->c + x);
    return;
}

/* Derive2 override 的虚函数func1 */
void Derive2_func1()
{
    /* 所有虚函数的第一句都是获得类型指针，试想一下，如果我想把这个方法赋值给Derive，就要通过Derive得到Base1或Base2，Base2肯定是无法直接通过指针转化得到的 */
    Derive2* derive2_ptr = get_derive2_ptr(self);
    /* 使用之前可以check null一下 */
    printf("derive2 %d\n", x);
    return;
}

/* DeriveDerive override 的虚函数func1 */
void DeriveDerive_func1()
{
    /* 所有虚函数的第一句都是获得类型指针，试想一下，如果我想把这个方法赋值给Derive，就要通过Derive得到Base1或Base2，Base2肯定是无法直接通过指针转化得到的 */
    DeriveDerive* derivederive_ptr = get_derivederive_ptr(self);
    /* 使用之前可以check null一下 */
    printf("derive derive %d\n", x);
    return;
}

/* 下面是构造函数 */
void Base1_ctor(Base1* base1)
{
    base1->cls_code = CLASS_BASE1;
    base1->a = 10;
    base1->func1 = Base1_func1;
}

Base1* Base1_new(void)
{
    Base1* base1_ptr = (Base1*)malloc(sizeof(Base1));
    if (base1_ptr == NULL) {
        return base1_ptr;
    }
    Base1_ctor(base1_ptr);
    return base1_ptr;
}

void Base2_ctor(Base2* base2)
{
    base2->cls_code = CLASS_BASE2;
    base2->b = 1.f;
    base2->func2 = Base2_func2;
}

Base2* Base2_new(void)
{
    Base2* base2_ptr = (Base2*)malloc(sizeof(Base2));
    if (base2_ptr == NULL) {
        return base2_ptr;
    }
    Base1_ctor(base2_ptr);
    return base2_ptr;
}

/* 下面非常重要， 注意如何让Derive覆盖func1 */
void Derive1_ctor(Derive1* derive1)
{
    derive1->cls_code = CLASS_DERIVE1;
    derive1->c = 3.14;
    Base1* base1_ptr = get_base1_ptr(derive1);
    Base1_ctor(base1_ptr);
    Base2* base2_ptr = get_base2_ptr(derive1);
    Base2_ctor(base2_ptr);
    base1_ptr->func1 = Derive1_func1;
}

Derive1* Derive_new(void)
{
    Derive1* derive1_ptr = (Derive1*)malloc(sizeof(Derive1));
    if (derive1_ptr == NULL) {
        return derive1_ptr;
    }
    Derive1_ctor(derive1_ptr);
    return derive1_ptr;
}

void Derive2_ctor(Derive2* derive1)
{
    derive2->cls_code = CLASS_DERIVE2;
    Base1* base1_ptr = get_base1_ptr(derive2);
    Base1_ctor(base1_ptr);
    base1_ptr->func1 = Derive2_func1;
}

Derive2* Derive2_new(void)
{
    
}