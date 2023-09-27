#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <filesystem> // 只有这个源文件在用，所以不用放在预编译头文件中

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>

// 日志文件，不需要经常变更，所以添加到预编译头文件中
#include "XuanWu/Core/Log.h"

// 位引擎Debug用的
#include "XuanWu/Debug/Instrumentor.h"

#ifdef XW_PLATFORM_WINDOWS
	#include <Windows.h>
#endif