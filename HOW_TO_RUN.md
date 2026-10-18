# 关于如何使用这一个库
只需在项目中include这个库的头文件即可使用。  
	` #include "Fern3DGameEngine.h" `  
开箱即食。

我的开发环境是 Windows 10 + VS2026。可以作为参考。

---

## Fern3DGameEngine.h
头文件中包含所有的外部依赖，以及按照分区组织的类和函数声明。

关于怎么使用API，参见 *API速查*  
关于数学原理，参见 *原理文档*  


---

---

## 运行单元测试

当前解决方案的构建目标是 `Matrix4x4` 的单元测试，源文件为 `Ext02-Matrix4x4_fix_test.cpp`。
在 Visual Studio 中打开 `Fern3DGameEngine_Rebuild.slnx` 后，直接按 F5 即可编译并运行。

判定方式：测试逐组打印结果，结尾输出"通过 N 项, 失败 M 项"。
程序退出码为 0 表示全部通过，非 0 表示存在失败项，便于后续接入自动化。

该测试只依赖 `FernPack/math` 与 `FernPack/basic_funcion` 下的头文件，
不依赖 EasyX 与窗口，因此可以独立编译运行。

### 切换回渲染演示

渲染演示 `Pipe03.cpp` 已收纳至 `FernPack/outdated_sources/`。
如需运行，把 `Fern3DGameEngine_Rebuild.vcxproj` 中的

    <ClCompile Include="Ext02-Matrix4x4_fix_test.cpp" />

改回

    <ClCompile Include="FernPack/outdated_sources/Pipe03.cpp" />

即可。`FernLearnOpenGL` 仓库中有更完整的渲染开发。
