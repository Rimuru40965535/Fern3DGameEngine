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



