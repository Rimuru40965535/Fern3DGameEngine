# API 速查文档  

这里归档所有 API 的用法，方便快速查阅喵。  
如需要详细了解 API 的运作原理，可以查阅 API 原理文档喵。  
（当然在现在版本中，代码已经足够简洁，可以从代码中直接分析实现原理喵。）  

顺序是从基础到高级，写在前面的 API 不会依赖于写在后面的 API ，写在前面的文件也不会依赖写在后面的文件喵。

---
---

## MathUtils.h

这个文件是一个模块，主要功能是进行各种数学运算喵。
可以说，这个模块就是整个引擎的地基喵。

---

`isNearlyEqual()`
-
- 参数
| 名称 | 类型 | 默认值 | 含义 |
| --- | --- | --- | --- |
|`A`|`double`|  |待比较值|
|`B`|`double`|  |待比较值|
|`pricision`|`double`|`1e-6`|相等阈值|

- 返回值
    - bool
        - 如果两数相差的绝对值小于相等阈值，则认为两数相等，返回`true`。
        - 否则返回`false`。
  

`inline static double Q_rsqrt_double(double)` 
-
- 函数说明
    - 将一个数的倒数开平方，获取结果
- 参数
    - 待运算数。
- 返回值
    - 运算结果。

`inline double Factoriaal(int)` 
-
- 函数说明
    - 计算阶乘喵。采取硬编码模式取代计算喵。
    - 此处存在方案比选。
        - int可以表示的上限远远不如double，所以采用double类型以容纳更多的阶乘值。
        - 阶乘缓存算法虽然可行，但是终归有一个临界值会导致数据溢出，经查询得知 25！以上数据会失真。另外 24！带来的精度已经足够，考虑到这一点，故采取列表硬编码算法以求最快速的响应。
- 参数
    - 你要算几的阶乘
- 返回值
    - 算好了拿给你


`inline double Cosine(double x, int terms)` 
-
- 函数说明
    - 计算给定角度的余弦值喵。此处采取了泰勒展开式计算，依赖阶乘函数。
    - 此函数的具体实现由 AI 实现。
- 参数
| 名称 | 类型 | 默认值 | 含义 |
| :---: | :---: | :---: | :---: |
|`x`|`double`|  |角度值|
|`terms`|`int`| 6 |迭代项数|
- 返回值
    - 给定角度的余弦值
- 未来计划的更新
    - 未来计划采用CORDIC算法实现函数功能，将会完全替代该函数喵。

`inline double Sine(double x, int terms)` 
-
- 函数说明
    - 计算给定角度的正弦值喵。此处采取了泰勒展开式计算，依赖阶乘函数。
    - 此函数的具体实现由 AI 实现。
- 参数
| 名称 | 类型 | 默认值 | 含义 |
| :---: | :---: | :---: | :---: |
|`x`|`double`|  |角度值|
|`terms`|`int`| 6 |迭代项数|
- 返回值
    - 给定角度的正弦值
- 未来计划的更新
    - 未来计划采用CORDIC算法实现函数功能，将会完全替代该函数喵。

`inline double Tangent(double x, int terms)` 
-
- 函数说明
    - 计算给定角度的正切值喵。此处采取了三角关系式计算，依赖正弦函数和余弦函数。
    - 此函数的具体实现由 AI 实现。
    - 此函数是简单实现，没有考虑 $ cos(x) = 0 $ 的情况。
- 参数
| 名称 | 类型 | 默认值 | 含义 |
| :---: | :---: | :---: | :---: |
|`x`|`double`|  |角度值|
|`terms`|`int`| 6 |迭代项数|
- 返回值
    - 给定角度的正切值
- 未来计划的更新
    - 未来计划采用CORDIC算法实现函数功能，将会完全替代该函数喵。


`inline double DegToRad(double deg)`  
-

`inline double RadToDeg(double rad)`  
-
- 函数说明
    - 角度制弧度制转换函数  


---
---

## Color.h

这个文件存储颜色类统一接口。

以下是Color类的属性。
```
struct Fern::Color{
    public:
        union {
            struct {
                uint8_t b;	//blue
                uint8_t g;	//green
                uint8_t r;	//red
                uint8_t a;	//alpha
            };
            uint32_t argb;	//全色号
}
```

以下是Color类的方法：

|                方法                 |            作用            |
| :-------------------------------: | :----------------------: |
|             `Color()`             |   构造Color结构体，并初始化为白色。    |
|     `Color(red, green, blue)`     | 构造Color结构体，并按给定的颜色分量初始化。 |
| ` Color(red, green, blue, alpha)` | 构造Color结构体，并按给定的颜色分量初始化。 |
|       `Color(rgbaValue)  `        |  构造Color结构体，并按给定的色号初始化。  |
|          `  getARGB() `           |           获取色号           |
|`ToEasyxColor()`                   | 获取EazyX格式的颜色|

以及预定义颜色：
|                 | 颜色名称 |
| :-------------: | :--: |
|     `Red()`     |  红   |
|    `Green()`    |  绿   |
|    `Blue()`     |  蓝   |
|    `White()`    |  白   |
|    `Black()`    |  黑   |
|   `Yellow()`    |  黄   |
|    `Cyan()`     |  青   |
|   `Orange()`    |  橙   |
|   `Magenta()`   |  品红  |
| `RimuruBlue()`  | 利姆露蓝 |
| `Transparent()` |  透明  |

---
---
## Vector3D.h

这个文件定义三维向量。包括点和法向量。

### Vector3D类的属性如下：
```
typedef class Vector3D {
    public:
        union {
            struct {
                double x;                   ///<x坐标
                double y;                   ///<y坐标
                double z;                   ///<z坐标
            };
            struct {
                double objectx;             ///<出现这几种写法的原因是便于追溯此时点所在的空间是哪一个空间
                double objecty;
                double objectz;
            };
            struct {
                double worldx;
                double worldy;
                double worldz;
            };
            struct {
                double screenx;
                double screeny;
                double screendepth;
            };
        };
    };
```

### 类提供以下方法：

`Vector3D()`
-
构造方法。根据传入的参数不同，返回不同的向量。

| 参数列表 | 返回向量 |
| :---: | :---: |
| 留空 | 零向量 |
|`double, double, double` | 由三个double数据指定的向量|
|`Vector3D`|与给定的向量相等的向量|
|`Vectir3D A, Vector3D B`| 向量 $ B-A $ |

`=`
-
重载的赋值方法，成员函数  
- 参数
    - Vector3D
- 返回值
    - `*this`

`==`
-
重载的判等方法，成员函数  
- 参数
    - Vector3D
- 返回值
    - 如果两向量任意一个轴的分量的差在默认误差`1e-6`以上时，返回 `false`
    - 其余情况返回 `true`


`+` `+=`
-
重载的向量加法，区别仅仅是有无赋值环节。成员函数。
- 参数
    - Vector3D 加向量
- 返回值
    - Vector3D 和向量


`-` `-=`
-
重载的向量减法，区别仅仅是有无赋值环节。成员函数。
- 参数
    - Vector3D 减向量
- 返回值
    - Vector3D 差向量


`*` `*=`
-
重载的向量数乘，区别仅仅是有无赋值环节。成员函数。
- 参数
    - double 乘数
- 返回值
    - Vector3D 积向量


`*`
-
重载的向量内积。成员函数。
- 参数
    - Vector3D 乘向量
- 返回值
    - double 内积

`%` `%=`
-
重载的向量外积，区别仅仅是有无赋值环节。成员函数。
- 参数
    - Vector3D 乘向量
- 返回值
    - Vector3D 积向量

`inline double getLengthSquared()`
-
获取模长的平方。成员函数。
- 参数
    - 无
- 返回值
    - double 模长平方

`getLength()` 
-
获取模长
- 参数
    - 无
- 返回值
    - double 模长

`setPos()`
-
重设点。成员函数。
不同的参数对应不同的功能
| 参数列表 | 执行功能 | 返回值 |
| :------: | :------: | :----: |
| `double, double, double` | 指定重设向量的三个分量 | `*this` |
| `Vector3D` | 指定一个与重设向量相等的向量 | `*this` |


`makeLengthUnit()`
-
将一个向量的模长设定为 1 ，且保持方向不变。成员函数。
- 参数
    - 无
- 返回值
    - `*this`


`makeLengthUnitCopy()` 
-
将一个向量的模长设定为 1 ，且保持方向不变，不改变自身向量的值，生成一个副本。成员函数。
- 参数
    - 无
- 返回值
    - Vector3D 重设后向量

`friend Vector3D operator*()`
-
保证向量数乘写反不崩溃的友元函数。*参见：opreator\**

`friend std::ostream& operator<<`
-
输出至控制台登场景，获取字符串。

### 预定义的工厂方法
| 写法（按轴分） | 写法（按方向分） | 实际返回 |
| :------------: | :--------------: | :------: |
| `xPositive()`  | `East()`         | `Vector3D(1, 0, 0)`|
| `xNegative()`  | `West()`         | `Vector3D(-1, 0, 0)`|
| `yPositive()`  | `Up()`           | `Vector3D(0, 1, 0)`|
| `yNegative()`  | `Down()`         | `Vector3D(0, -1, 0)`|
| `zPositive()`  | `South()`        | `Vector3D(0, 0, 1)`|
| `zNegative()`  | `North()`        | `Vector3D(0, 0, -1)`|

---
---

## Matrix4x4.h

这个文件提供矩阵的定义以及运算法则。

### Matrix4x4 类的属性如下
```
class Matrix4x4 {
	public:
		double values[4][4];		///values[行号][列标]
    };
```
需要注意的是，矩阵采用行主序存储。  
也就是说，如果写在纸上的话，这个矩阵会是这样的形式：
```
values[0][0]    values[0][1]    values[0][2]    values[0][3]
values[1][0]    values[1][1]    values[1][2]    values[1][3]
values[2][0]    values[2][1]    values[2][2]    values[2][3]
values[3][0]    values[3][1]    values[3][2]    values[3][3]
```

### Matrix4x4 类的方法如下

`Matrix4x4()`
-
重载的构造方法。
- 参数
    - (int = 1) 矩阵生成初始化填充样板
| 宏 | 值 | 矩阵填充效果 |
| :---: | :---: | :----: |
| `MATRIX_INIT_WITH_UNIT_MATRIX` | 1 | 矩阵初始化为四维单位矩阵 | 
| `MATRIX_INIT_WITH_ALL_0`       | 2 | 矩阵初始化为全0矩阵      |
| `MATRIX_INIT_WITH_ALL_1`       | 3 | 矩阵初始化为全1矩阵      |
| `MATRIX_INIT_WITH_TEST`        | 4 | 矩阵初始化为测试矩阵     |

- 参数
    - double[4][4] 按二维数组填充矩阵，生成的矩阵的值与数组中的一致。

- 参数
    - Matrix4x4 按矩阵填充矩阵，生成的矩阵的值与矩阵中的一致。

- 注
    - 单位测试矩阵的填充如下
| 0 | 1 | 2 | 3 |
|:-:|:-:|:-:|:-:|
| 4 | 5 | 6 | 7 |
| 8 | 9 | 10| 11|
| 12| 13| 14| 15|

`=`
-
重载的赋值函数。将一个矩阵的值赋值给另一个矩阵。
- 参数
    - Matrix4x4 
- 返回值
    - `*this`

`+` `+=`
-
矩阵加法。区别仅限有没有赋值操作。成员函数。
- 参数
    - Matrix4x4 加矩阵
- 返回值
    - Matrix4x4 和矩阵


`-` `-=`
-
矩阵减法。区别仅限有没有赋值操作。成员函数。
- 参数
    - Matrix4x4 减矩阵
- 返回值
    - Matrix4x4 差矩阵

`*` `*=`
-
重载的矩阵左乘。区别仅限有没有赋值操作。成员函数。
- 参数
    - Matrix4x4 左矩阵
- 返回值
    - 积矩阵

`*`
-
重载的向量左乘矩阵。成员函数。
- 参数
    - Vector3D 待变换向量
- 返回值
    - 变换结果


`^` 
-
矩阵右乘。区别仅限有没有赋值操作。成员函数。
- 参数
    - Matrix4x4 右矩阵
- 返回值
    - 积矩阵

`~`
-
矩阵求逆。专门用于仿射矩阵求逆。如果不是仿射矩阵，或者矩阵不可逆，返回单位矩阵。
- 参数
    - 无
- 返回值
    - 求逆结果

### 可能会在未来的版本废弃的函数
 
`friend std::ostream& operator<<()`
-
重载的流式输出友元函数。

`std::string toString()`
-
获取字符串形式矩阵。曾用于调试矩阵运算，输出运算步骤与结果。

### 预定义的工厂方法

以下均为 `Matrix4x4` 类的成员函数：
| 函数名 | 函数参数列表 | 函数返回值类型 | 函数作用 |
| :----: | :----------: | :------------: | :------: |
| `Identity()` | | `Matrix4x4` | 生成一个单位矩阵 |
| `Translation()` | `double x, double y, double z` | `Matrix4x4` | 生成按指定分量平移矩阵 |
| `Translation()` | `Vector3D` | `Matrix4x4` | 生成按指定向量平移矩阵 |
| `Scale()` | `double x, double y, double z` | `Matrix4x4` | 生成按指定三轴缩放比例缩放矩阵 |
| `Scale()` | `Vector3D` | `Matrix4x4` | 生成按指定向量三分量三轴缩放比例缩放矩阵 |
| `RotationX()` | `double angle` | `Matrix4x4` | 生成绕x轴正向逆时针转动`angle`弧度的矩阵，旋转基点为原点 |
| `RotationY()` | `double angle` | `Matrix4x4` | 生成绕y轴正向逆时针转动`angle`弧度的矩阵，旋转基点为原点 |
| `RotationZ()` | `double angle` | `Matrix4x4` | 生成绕z轴正向逆时针转动`angle`弧度的矩阵，旋转基点为原点 |
| `RotationAroundOriginX()` | `double angle, Vector3D origin` | `Matrix4x4` | 生成绕x轴正向逆时针转动`angle`弧度的矩阵，旋转基点为`origin` |
| `RotationAroundOriginY()` | `double angle, Vector3D origin` | `Matrix4x4` | 生成绕y轴正向逆时针转动`angle`弧度的矩阵，旋转基点为`origin` |
| `RotationAroundOriginZ()` | `double angle, Vector3D origin` | `Matrix4x4` | 生成绕z轴正向逆时针转动`angle`弧度的矩阵，旋转基点为`origin` |


---
--- 

## `Window.h`

文件存储一个管理窗口行为的类。
这个类是程序与EazyX通讯的桥梁。

### Window类的属性如下
```
 class Window {
    public:
        int width;             ///<窗口宽度
        int height;            ///<窗口高度
        std::string title;     ///<窗口标题
        bool isOpen;           ///<是否打开

        HWND windowHwnd;       ///<窗口句柄

        Matrix4x4 windowMatrix; ///<视窗变换矩阵
    };
```

### Window类的方法如下

`Window()`
-
构造方法。
指定宽高，自动创建窗口，初始化视窗变换矩阵，同时自动开启批量绘图功能。
- 参数
    - int w 窗口宽度
    - int h 窗口高度
    - std::string& t 窗口标题
    - int flag EasyX窗口创建标志位  
    
***没有拷贝构造函数以及赋值函数。***

`Clear()`
-
清除屏幕，填充背景色。
- 参数
    - 无
- 返回值
    - 无

`Present()`
-
交换缓冲区，显示绘图内容。
- 参数
    - 无
- 返回值
    - 无
    
`PullEvents()`
-
处理窗口消息函数。  
***空实现***  

`ShouldClose()`
-
检测窗口是否应该被关闭。
- 参数
    - 无
- 返回值
    - `!isOpen`

`Width()` `Height()`
-
获取窗口宽度、高度。
- 参数
    - 无
- 返回值
    - int 窗口宽度（或高度）


`DrawPixel()`
-
逐像素绘制。  
内部已经内置剔除逻辑，将超过视窗范围的像素剔除跳过绘制。
- 参数
    - int x
    - int y 绘制像素所在坐标
    - Color 绘制颜色
- 返回值
    - int 函数执行状态代码
    | 值 | 状态 | 
    | :---: | :---: | 
    | 1 | 绘制成功 |
    | -1 | x轴超过范围 |
    | -2 | 在x轴不超过范围的前提下y轴超过范围 |

---
---





