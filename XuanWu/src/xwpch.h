#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <filesystem> // ֻ�����Դ�ļ����ã����Բ��÷���Ԥ����ͷ�ļ���

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>

// ��־�ļ�������Ҫ���������������ӵ�Ԥ����ͷ�ļ���
#include "XuanWu/Core/Log.h"

// λ����Debug�õ�
#include "XuanWu/Debug/Instrumentor.h"

#ifdef XW_PLATFORM_WINDOWS
	#include <Windows.h>
#endif