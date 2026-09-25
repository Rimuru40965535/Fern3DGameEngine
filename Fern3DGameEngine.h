/***
* @file Fern3DGameEngine.h
* @brief A simple 3D game engine for C++	
* 
* @author 半人马座beta星
* 
*/

#pragma once

/***
* @brief 此命名空间用于封装Fern3DGameEngine的所有功能和类。我说菲伦是十七岁的妈妈有没有懂的。
* 
*/
namespace Fern{}

///< std dependency || 标准库依赖		||依赖文件
#include <iostream>						
#include <vector>
#include <fstream>
#include <queue>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <iomanip>
#include <filesystem>
#ifdef _WIN32
#include <Windows.h>
#endif
#include <cstdint>


#include <easyx.h>


///#include "FernPack/basic_function/LogBuff.h"

#include "FernPack/basic_funcion/MathUtils.h"
#include "FernPack/basic_funcion/Color.h"

#include "FernPack/math/Vector3D.h"