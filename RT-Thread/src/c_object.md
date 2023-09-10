**用C语言实现面向对象**

用C语言来实现面向对象，并不是要把C语言打造成一个面向对象的语言，支持像其他面向对象语言一样的语言特性。而是为了能享受面向对象的一些福利，让程序**高度模块化**。

C语言是一门面向过程的语言，我们在用C语言做开发的时候会很自然的按照面向过程的思想来做设计和编码实现。而面向过程的思想在应对比较复杂的系统的时候存在一些先天不足：

* 把业务功能分解为一个个的过程、子过程，用函数来表达这些过程。在代码组织上很容易按照过程来组织代码。例如我们有一个激活小区的功能，我们把这个功能分解为一个个的过程，用函数来实现了，在组织代码的时候，我们很自然的用一个cell_active.c来把这些代码组织在一起。而激活小区这个功能很复杂，而且不同类型的小区要进行不同的处理，但是为了代码实现上的方便或者为了能更容易复用一些之前的实现，我们还是经常把这个复杂的功能组织在一个文件里面。不同类型的小区处理差异往往也不会进行隔离。这样就很容易导致上帝文件的产生。而且代码实现中有很多if else的分支判断。
* 在C语言中，函数是基本单位。如果要在函数之间共享数据的话，我们就很容易的想到使用全局变量。而在一个系统中，全局变量一多，使用上再不加控制，extern满天飞。那我们的系统很快就会陷入难以理解、难以维护的深渊。
* 由于函数是基本单位，那我们代码复用的粒度也只能是做到函数复用（C库提供给我们的功能就是通过一堆函数库）。而由于上面说的全局变量的问题，往往导致我们的函数在被复用的时候拔起萝卜带起泥。让人没有复用的欲望，我们自己重新实现一个好像要比复用这个还简单。

而面向对象在这方面就好很多，面向对象通过将关系紧密（高内聚）的数据以及在这些数据之上的操作封装在一起形成类。以类的粒度对外提供功能。我们在组织代码的时候基本上就是一个类一个头文件+源文件，而在复用上我们可以做到类粒度的复用。面向对象的多态可以更好的应对变化，提升代码的可维护性。因此现在很多使用C语言开发的项目也都在使用面向对象的方法做分析和设计。我们希望我们的代码实现和我们的设计能保持一致，那我们用面向对象的方法做的设计，用C语言怎么实现才能保证代码实现和软件设计（软件模型）的高度一致呢？我们用面向对象做设计，到能指导编码的地步的话，一般都是输出了类图，而用C语言实现面向对象其实就是用C语言把这个类图给表达出来，就是怎么表达类，怎么表达类之间的关系。



**实现类**

1. 用struct来实现类

2. 类的属性使用struct的字段来表达

3. 类的非抽象方法（C++中的非虚函数）使用普通函数来表达，函数的第一个参数为该struct的指针（对应C++中的this指针）

4. 类的抽象方法（C++中的虚函数）使用struct的函数指针类型的字段来表达，函数指针指向的函数类型第一个参数同样为该struct的指针。（后面讲多态的时候细讲）

5. 代码组织上使用一个.c和.h来表达一个类，文件名和类名对应

![image-20220608223533780](https://user-images.githubusercontent.com/80186561/173372371-27cbfee3-bbba-480b-a79b-707cf03714aa.png)

对应上面的Person类，我们使用C语言的实现代码如下：

```c
// person.h
typedef struct Person {
	const char *name;
	int age;
} Person;

void Person_Introduce(const Person *self);
```

```c
// person.c
#include "person.h"

void Person_Introduce(const Person *self)
{
	printf("My name is %s. I am %d years old.", self->name, self->age);
}
```

头文件中我们放置类的定义以及类对外提供的功能（对应C++中的public方法），源文件中放置方法的实现。



**使用类**

如下是一段使用Person类的代码

```c
// client.c
#include "person.h"

void UsePerson(void)
{
	Person person = (.name = "Tom", .age = 20);
	Person_Introduce(&person);
}
```

其实使用类很简单，就是获取/定义类实例，然后调用类提供的方法。在C++等面向对象的语言中，一个类有构造函数的，用户通过构造函数可以很容易生产出来一个类的实例。但是在C语言中没有构造函数这个特性，我们要生产出来一个类的实例，比C++里面要麻烦一些。



**提供构造宏**

像上面的代码中由于Person类很简单，我们在定义person的时候直接对各个字段进行初始化。如果是这种比较简单的类，我们可以提供一个宏来替代构造函数的功能。让代码的表达力更强一点。代码如下：

```c
// person.h 提供构造Person的宏
#define MAKE_PERSON(name_, age_) {.name = (name_), .age = (age_)};

// client.c
#include "person.h"

void UsePerson(void)
{
	Person person = MAKE_PERSON("Tom", 20);
	Person_Introduce(&person);
}
```



**提供初始化操作**

有的类的初始化过程就比较复杂（例如需要申请内存、类里面嵌套的有其他类等等），没办法像上面那样简单提供一个宏来构造一个类实例。这时候我们可以提供一个Init函数来完成初始化操作。

```c
// person.h 提供初始化操作
void Person_Init(Person *self);

// client.c
#include "person.h"

void UsePerson(void)
{
    Person preson;
    Person_Init(&person);
    
    Person_Introduce(&person);
}
```

提供Init操作，我们就要考虑是否需要对应的提供DeInit的操作（对应于C++的析构函数，用于释放资源等），如果需要，我们就提供配套的DeInit操作。



**提供Factory或者Creation方法**

有点时候我们不想让用户自己来生产一个类的实例，而是由Factory或者类本身提供一个Creation方法的形式来给用户提供一个类实例的指针。如下代码展示的是由Person类来提供Creation方法给用户。

```c
// person.h
typedef struct Person Person;

Person* Person_Create(const char *name, int age);
void Person_Destroy(Person *self);

void Person_Introduce(const Person *self);

// person.c
#include "person.h"

typedef struct Person {
    const char *name;
    int age;
} Person;

Person* Person_Create(const char *name, int age)
{
    Person *ins = (Person *)malloc(sizeof(Person));
    if (ins == NULL) {
        return NULL;
    }
    
    ins->name = name;
    ins->age = age;
    
    return ins;
}

void Person_Destroy(Person *self)
{
    if (self == NULL) {
        return;
    }
    
    free(self);
}

void Person_Introduce(const Person *self)
{
    printf("My name is %s. I am %d years old.", slef->name, self->age);
}

// client.c
#include "person.h"

void UsePerson(void)
{
    Person *person = Person_Create("Tom", 20);
    
    Person_Introduce(person);
    
    Person_Destroy(person);
}
```

上面的代码我们看到struct Person的定义移到了person.c中，client在使用Person的时候根本看不到Person的定义，不知道Person里面有什么内容，也不知道其具体实现。只用关注person.h中提供的方法都是提供的什么功能就可以了（这就是Bob大叔在《整洁架构》中讲到的完美封装）。为什么可以这样实现呢？这是因为Person提供的方法的参数都是Person指针，而我们提供了Peson_Create让用户获取到一个具体的Person指针。在使用指针类型的时候，是不需要知道其类型的具体定义的（因为不要分配内存），只要有其申明，知道Person是一个结构体类型就够了。也就是上面代码中的`typedef struct Person Person;`，这就是我们常说的前置说明。通过前置说明我们可以在某些场景下进一步提升代码的封装性，做到更好的信息隐藏，让服务使用者和服务提供者之间的耦合更低。

Create方法的实现往往是通过动态申请的方式，因此我们往往需要提供对应的Destory操作。我们利用GCC提供的cleanup属性可以做到不提供Destory操作，类似于C++中的智能指针，在变量申明周期结束的时候自动调用一个用户提供的函数。感兴趣的同学可以去了解下。

嵌入式系统中我们往往不建议使用动态内存申请的方式（频繁的动态内存申请会产生内存碎片，效率也低）。可以利用预先申请一块内存+池化技术来代替动态申，这里就不展开讲了。



**实现类之间的关系**

![image-20220608223741369](https://user-images.githubusercontent.com/80186561/173372604-d9dabafe-c810-41c7-a65a-45862fe4fa91.png)


**依赖(use a)**

依赖一般是一种使用的关系，即一个类的实现需要另一个类的协助。我们知道类A依赖于类B，体现在代码上往往表现为：

1. A的某个方法的参数是B类型
2. A的某个方法实现中使用了B类型的局部变量
3. A的某个方法的返回值是B类型
4. A调用B的静态方法

上面的而类图中Teacher类就依赖于student类（Teacher类的Introduce方法需要Student类型的参数）。代码如下：

```
void Teacher_Introduce(const Teacher *self, canst Student *student)
{
	Person_Introduce(&self->base);
	printf("I am Teacher. I %steach %s", Klasss_IsEqual(self->klass, Student_GetKlass(student)) ? "" : "don't", Student_GetName(student));
}
```



**关联 聚合 组合 (has)**

* 关联：对象间的结构关系，说明一个关联的实例中有多少个相互连接的对象。简单关联表示两个同等地位类之间的结构关系。关联可以是单向的也可以是双向的。
* 聚合：一种更强关系的关联，表示关联的两个类之间是整体和部分的关系。
* 组合：最强关系的关联，表示关联的两个类之间是整体和部分的关系，且整体和部分共存（生命周期相同，同生共死）。

表示在代码上一般都是被关联类作为关联类的属性，即has a或者has many的关系。如果是简单关联和聚合，我们一般使用指针类型（生命周期可以不同）。如果是组合关系，一般使用值类型（生命周期相同，同生共死）。

上面的类图中Teacher和Klass是单项关联关系（老师教哪个班级）。Student和Klass是双向关联关系（学生属于哪个班级，班级的班长是哪个学生）。

C语言中我们实现关联就是结构体中包含结构即可。

```c
// Teacher和Klass的单向关联关系
typedef struct Teacher {
    Person base;
    Klass *klass; // 表达关联关系
};
```

```c
// Student和Klass的双向关联关系

// student.h
typedef struct Klass Klass;

typedef struct Student {
    Person base;
    Klass *klass; // 学生属于哪个班级
};

// klass.h
typedef struct Student Student;

typedef struct Klass {
    int klassNo;
    Student *leader; // 班级的班长是哪个学生
};
```

由于双向关联是双向的依赖关系，如果我们在struct.h中include klass.h，而在klass.h中也需要include student.h，这就引入了循环依赖，导致编译问题。解决这个问题同样可以利用前置申明，由于Student中使用Klass以及Klass中使用Student都是通过指针类型，我们不用看到彼此的定义，有申明就可以了。Student中使用Klass的具体功能是在student.c中，因此在student.c中include klass即可。Klass亦然。这样就解决了循环依赖问题。

聚合的实现跟关联是一样的，只不过两个类之间有整体和部分的含义，在代码上没有区别。

组合的实现把指针类型换成值类型即可。



**泛化(ls a)**

泛化指的是一般事物（父类）和具体事物（子类）之间的关系。也就是我们常说的继承，表达的is a 的关系。继承是面向对象的三大特征之一，使用继承可以带来以下好处：

1. 表达两个类之间is a的关系
2. 代码复用
3. 实现多态

在面向对象语言中对继承都有语法上的支持，但是C语言是没有的。使用 C语言实现继承还是通过结构体包含来实现。上面类图中Student和Teacher都是继承自Person，代码实现如下：

```c
// person.h
typedef struct Person {
    const char *name;
    int age;
} Person;

void Person_Introduce(const Person *self);

//person.c
#include "person.h"

void Person_Introduce(const Person *self)
{
    printf("My name is %s. I am %d years old.", self->name, self->age);
}

//student.h
#include "person.h"

typedef struct Student {
    Person base; // 表达Student is a Person
    Klass *klass;
} Student;

void Student_Introduce(const Student *self);

// student.c
#include"student.h"

void Student_Introduce(const Student *self)
{
    Person_Introuduce(&self->base); //子类对象向上转型为父类对象，复用父类实现
    
    printf("I am student. I am Class %d.", Klass_GetNo(self->klass));
}
```

我们把父类当作子类的第一个成员，可以很容易的做向上转型，向下转型（实现多态的时候会用到）。子类可以很容易的复用父类的实现。



**实现**

实现本质上还是继承，实现上没什么差别，不再展开讲了。



**多态**

在面向对象编程思想中，多态表达的是具有共性的类型，在执行相同的行为时，会体现出不同的实现方式。可以简称为：**相同的行为，不同的实现**。比如：同样看到对面过来一个美女，男人和女人的想法是不一样的。

![image](https://user-images.githubusercontent.com/80186561/173372136-6d42fb90-08f0-4e8a-bf71-1991dcf8b4cf.png)


多态是面向对象三大特性之一，表达的是“变化”。面向丰富和不断变化的问题域，多态可以让我们的程序能更好的去应对这些变化，恰当的使用多态技术，可以显著提升程序的可扩展和可维护性。

C++中实现多态是通过虚函数，用C语言怎么实现多态呢？假设有这样一个场景：有一个教育为主的交流会议，参加的有学生、教师、记者、教育局领导不冲突的话也会与会。每个参会者都要上台做自我介绍，每种职业自我介绍

的内容不一样。我们要用代码表达以下这个过程。

![image-20220608230531443](https://user-images.githubusercontent.com/80186561/173372722-15b62f78-318d-4b7f-a34e-240cbca58d7a.png)


```c
// attendee.h
typedef struct Attendee Attendee;

typedef struct AttendeeOps {
    void (*Introduce)(const Attendee *self);
    void (*Relase)(Attendee *self);
} AttendeeOps;

typedef struct Attendee {
    const char *name;
    int age;
    const AttendeeOps *ops;
} Attendee;

static inline void Attendee_Introduce(const Attendee *self)
{
    self->ops->Introduce(self);
}

static inline void Attendee_Release(Attendee *self)
{
    self->ops->Release(self);
}

// attendee.c
#include "attendee.h"
#include <stdio.h>

void Attendee_IntroduceInner(const Attendee *self)
{
    printf("My name is %s, I am %d years old.", self->name, self->age);
}

void Attendee_Init(Attendee *self, const char *name, int age, const AttendeeOps *ops)
{
    self->name = name;
    self->age = age;
    self->ops = ops;
}

// attendee_inner.h
#include "attendee.h"

typedef struct Student {
    Attendee base;
    int classNo;
} Student;

Attendee* Student_Create(const char *name, int age, int classNo);

// student.c
#include "student.h"
#include "attendee_inner.h"
#include <stdio.h>
#include <stdlib.h>

static void Student_Introduce(const Attendee *attendee)
{
    Attendee_IntroduceInner(attendee);
    
    const Student *self = (const Student *)attendee; // 向下转型
    printf("I am a student. I am in Class %d.\n", self->classNo);
}

static void Student_Release(Attendee *attendee)
{
    free(attendee);
}

static AttendeeOps studentOps = {
    .Introduce = Student_Introduce,
    .Relase = Student_Relase
};

Attendee* Student_Create(const char *name, int age, int classNo)
{
    Student *ins = (Student*)malloc(sizeof(Student));
    if (ins == NULL) {
        return NULL;
    }
    
    Attdee_Init(&ins->base, name, age, &studentOps);
    ins->classNo = classNo;
    
    return &ins->base;
}

// teacher.h
#include "attendee.h"

typedef struct Teacher {
    Attendee base;
    int classNo;
} Teacher;

Attendee* Teacher_Create(const char *name, int age, int classNo);

// teacher.c
#include "teacher.h"
#include "attendee_inner.h"
#include <stdio.h>
#include <stdlib.h>

static void Teacher_Introduce(const Attendee *attendee)
{
    Attendee_IntroduceInner(attendee);
    
    const Teacher *self = (const Teacher *)attendee; // 向下转型
    printf("I am a teacher. I teach Class %d.\n", self->classNo);
}

static void Teacher_Release(Attendee *attendee)
{
    free(attendee);
}

static AttendeeOps teacherOps = {
    .Introduce = Teacher_Introduce,
    .Release = Teache_Release
};

Attendee* Teacher_Create(const char *name, int age, int classNo)
{
    Teacher *ins = (Teacher*)malloc(sizeof(Teacher));
    if (ins == NULL) {
        return NULL;
    }
    
    Attdee_Init(&ins->base, name, age, &teacherOps);
    ins->classNo = classNo;
    
    return &ins->base;
}

// attendee_factory.h
#include "attendee.h"
#include <stdio.h>

typedef enum {
    STUDENT,
    TEACHER
} Occupation;

Attendee* AttendttFactory_Create(Occupation occupation, const char *name, int age, int classNo);

#define AttendttFactory_Destroy(attendee) \
	do { \
		if (attendee != NULL) { \
			Attendee_Release(attendee); \
			attendee = NULL; \
	} \
} while(0)

// attendee_factory.c
#include "attendee_factory.h"
#include "student.h"
#include "teacher.h"

Attendee* AttendttFactory_Create(Occupation occupation, const char *name, int age, int classNo);
{
    switch (occupation) {
        case STUDENT:
            return Student_Create(name, age, classNo);
        case TEACHER:
            return Teacher_Create(name, age, classNo);
        default:
            return NULL;
    }
}

// main.c
#include "attendee_factory.h"

int main(void)
{
    Attendee* attendees[] = {
        AttendttFactory_Create(STUDENT, "MeiChaofeng", 30, 1),
        AttendttFactory_Create(TEACHER, "HuangYaoshi", 50, 1),
        AttendttFactory_Create(STUDENT, "Guojing", 18, 2),
        AttendttFactory_Create(TEACHER, "HongQigong", 50, 2)
    };
    
    for (int i=0; i<sizeof(attendees)/sizeof(attendees[0]); i++) {
        Attendee *attendee = attendees[i];
        Attrndee_Introduce(attendee);
        
        AttendttFactory_Destroy(attendee);
    }
    
    return 0;
}
```

运行结果

```c
My name is MeiChaofeng, I am 30 years old. I am a student, I am in Class 1.
My name is HuangYaoshi, I am 50 years old. I am a teacher, I teach Class 1.
My name is Guojing, I am 30 years old. I am a student, I am in Class 2.
My name is HongQigong, I am 50 years old. I am a teacher, I teach Class 2.
```

attendees数组中每个元素都是Attendee*，但是在运行时对于自我介绍这个行为，表现出了不同的实现。这个就是多态。教育局领导如果不冲突可以与会的话，我们新增一个教育局领导的类就可以支持，满足开闭原则。

C语言中实现多态的诀窍就是用函数指针类型的成员来表达具有多态的方法（对应C++的虚函数），函数指针在不同的子类中指向了不同的函数实现，因此行为表现出了多态性。在代码实现上有几点需要澄清下：

* 为什么把Attendee支持的具有多态行为的方法聚合到AttendeeOps中，而不是直接写在Attendee里面？

1. 节省内存：如果是直接写在Attendee中，那么每个Attendee的实例都需要内存存储这几个函数指针成员。而这些操作往往是一种类型的子类的操作都是一样，不需要每个实例持有一份。所以我们为每种子类定义一个static的操作表，Attendee里面通过指针指向表就可以了，这样每个实例只需要一个指针的内存空间。（操作表相当于C++中的虚表指针）。
2. 对标C++虚函数实现，给每个子类挂表的操作简洁易懂。

* 代码中为什么有一个attendee_inner.h，为什么不把该文件的内容直接放到attendee.h中？

  attendee_inner.h中的操作是提供给子类，为了让子类复用实现的。而attendee.h中的内容是提供给Attendee类用户的。根据接口隔离原则，不要强迫客户依赖不需要的东西，我们将头文件进行拆分。

上面的代码中向下转型（父类类型转化为子类类型）都是通过强制类型转换实现，是因为父类是子类结构体的第一个成员。如果是多重继承的话，怎么向下转型？

答案是通过container_of（linux内核实现的一个宏，作用是在知道结构体成员地址的前提下得到结构体的地址，可以参考linux内核的实现自己实现一个），实例代码：

![image-20220608230945096](https://user-images.githubusercontent.com/80186561/173372865-2d9499f5-3cd3-41e9-9356-410c222fe7bd.png)

```c
#define offset_of(type, member) ((size_t)&((type*)0)->member)
#define container_of(ptr, type, member) \
		({ \
			const typeof(((type*)0)->member0)*_mptr = (ptr); \
			(type*)((char*)_mptr - offset_of(type, member)); \
		})
		

// father.h
typedef struct Father Father;
typedef struct Father {
    void (*PlayWithSon)(Father *self);
} Father;

//employee.h
typedef struct Employee Employee;
typedef struct Employee {
    void (*Work)(Employee *self);
} Employee;

// man.h
#include "father.h"
#include "employee.h"
typedef struct Man {
    Father father;
    Employee employee;
    
    const char *name;
} Man;

// man.c
#include "man.h"
#include <stdio.h>

static void Man_PlayWithSon(Father *father)
{
    Man *self = container_of(father, Man, father);
    printf("Man %s play with main.\n", self->name);
}

static void Man_Work(Employee *employee)
{
    self->father.PlayWithSon = Man_PlayWithSon;
    self->employee.Work = Man_Work;
    
    self->name = name;
}

// main.c
int main(void)
{
    Man man;
    Man_Init("ZhangSan");
    
    Father *father = &man.father;
    father->PlayWithSon(father);
    
    Employee *employee = &man.employee;
    employee->Work(employee);
    
    return 0;
}
```

运行结果：

```c
Man zhangSan play with son.
Man ZhangSan work.
```



**总结**

1. 用C语言实现面向对象并不是要完全实现面向对象语言的特性，而是为了让我们用面向对象方法做的软件设计可以和我们的代码实现保持高度的一致。享受到面向对象的福利，让程序高度的模块化。
2. 物理组织上用一个.c加.h来表达一个类，文件名和类名保持一致。
3. 用struct实现类，用struct包含来实现has-a和is-a关系。指针类型表达生命周期不同，值类型表达生命周期相同。
4. 尽可能利用static、前置申明提升程序的封装性，降低耦合。
5. 尽可能利用const提升程序的健壮性。


[UML六种关系](https://www.jianshu.com/p/f35fab1640c6)

[UML类图几种关系的总结](http://uml.org.cn/oobject/201609062.asp)
