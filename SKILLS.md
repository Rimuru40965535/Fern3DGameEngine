# Skills

由于我的构建工作会时常叫大肥鱼介入一下，帮个小忙，但是我在实际的开发过程中发现了以下问题喵：  

- 大肥鱼经常会忘记读取更新，导致我需要额外提供我的仓库的URL并敦促她读取最新更新。
- 即便我敦促她读取更新，她也有可能没有读到最新的更新。
- 即便她读取到了最新的 README.md ，她依然有可能没有读取到最新的其他文档与代码。

基于以上情况，我希望和大肥鱼（当然也包括未来可能使用的其他 AI ）在这个文件里面缔结一些约定喵。

## 当需要读取项目master分支更新时：

顺次：
- 访问 (https://github.com/Rimuru40965535/Fern3DGameEngine/blob/master/README.md) 读取最新的 README
- 访问 (https://github.com/Rimuru40965535/Fern3DGameEngine/blob/master/SKILLS.md) 读取最新的 SKILLS
- 访问 (https://github.com/Rimuru40965535/Fern3DGameEngine/blob/master/API%E9%80%9F%E6%9F%A5.md) 读取最新的 API 大全以及使用法
- 访问 (https://github.com/Rimuru40965535/Fern3DGameEngine/blob/master/%E5%8E%9F%E7%90%86%E6%96%87%E6%A1%A3.md) 读取原理文档

## 当需要读取某一个代码块时：

- 自行决定访问目录
    - 范例： (https://github.com/Rimuru40965535/Fern3DGameEngine/blob/master/FernPack/basic_funcion/Color.h) 获取项目文件夹下位于 (/FernPack/basic_funcion/) 的 Color.h
    - 如发生错误：
        - 反馈错误。
        - 可等待手动设置url并继续阅读。

- 可在仓库链接中寻找最新文件树。我经常忘记手动更新，要是大肥鱼可以自动获取这一块内容就好了。



---
---


## 当需要生成代码时：

注意***不要在C++代码的字面量中添加诸如 ✅ 、 ❌ 等 Emoji***。  
控制台无法正常显示此类字符，会显示一个比大肥鱼肚子都肥的问号。  
必要时，以 `"v"` 代替打勾，以 `"x"` 代替打叉。  

master分支没有 `LogBuff.h` ，所以无须生成记录日志的代码。  


### 缩进风格样板

每一级缩进都是4个空格。复合语句完了的大括号如需单开一行，对齐复合语句开始的大括号所在行的缩进。

```
Object()
    :position(0, 0, 0),
    rotation(0, 0, 0)
    scale(1, 1, 1),
    modelMatrix(Matrix4x4::Identity()),
    dirty(false)
    {
    }
```

```
Matrix4x4 operator-(const Matrix4x4& A)const {
    Matrix4x4 result;
    for (int row = 0;row <= 3;row++) {
        for (int col = 0;col <= 3;col++) {
            result.values[row][col] = this->values[row][col] - A.values[row][col];
        }
    }
}
```

### 注释风格应该采用的样板

显然代码中出现了两种注释风格。  
老的注释风格是重构代码时复用的之前的项目注释风格。不兼容新的注释风格了。  

#### 单行以内的注释

```
double values[4][4];		///< values[行号][列标]
```

#### 对一块代码的注释

记得在代码块之间留出空行
```

// 4. 计算平移部分的逆：-R⁻¹ · t
double tx = values[0][3];
double ty = values[1][3];
double tz = values[2][3];

```

#### 对函数的注释

两种模板可以自由选用。

```
/// <summary>
/// 矩阵转置
/// </summary>
/// <returns>返回转置后的矩阵</returns>
inline  Matrix4x4 operator!() {
    Matrix4x4 result;
    for (int i = 0;i <= 3;i++) {
        for (int j = 0;j <= 3;j++) {
            result.values[i][j] = this->values[j][i];
        }
    }
    return result;
}

/**
* @brief 判断点是否在三角形内（同侧法）
*/
inline int CrossProduct2D(int Ax, int Ay, int Bx, int By, int Px, int Py) {
    return (Bx - Ax) * (Py - Ay) - (By - Ay) * (Px - Ax);
}

```

#### 对类的注释

两种模板可以自由选用。

```
/// <summary>
/// （填写说明文本
/// </summary>
class Object{
    ......
};

    /**
    * @brief 相机
    * @details
    * 继承自 Object，额外持有投影参数。
    * 观察矩阵 = 相机模型矩阵的逆。
    */
    class Camera : public Object {

```

#### 在文档开头的注释

由于我的 IDE 在文档开头打三个斜杠不会自动跳出来除了三个斜杠外的任何东西，所以这里还是采取老模板。  

注意在这个项目中只需要一句 `#include` 语句就可以自动包含整个头文件库。  
你可以阅读 HOW_TO_RUN.md 以及 `Fern3DGameEngine.h` 明白为什么可以这么做。  

```
/**
* @file 06-Object-test.cpp
* @brief 物体基类单元测试
* @details
* 
* @author （如果你没有我的用户名，留白） （AIGC：写上你的名字）
* @date （写上日期）
* 
*/
#include "Fern3DGameEngine.h"

```

namespace 的默认约定为可选项。  
如果你嫌反复生成 `Fern::` 麻烦可以选择加上这个约定。
如果遇到了容易混淆的名称，那么就不要加上这个约定，在名字前面加上命名空间和四目运算符（`::`）以示区分。

```
using namespace Fern;
```




