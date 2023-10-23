#ifndef _XWPCH_H
#define _XWPCH_H
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <filesystem>
#include <functional>
namespace fs = std::filesystem;

// LLVM
#include <clang-c/Index.h>

// ¹¤¾ß
#include "Utils/Utils.h"
#include "Meta/meta_data_config.h"

// Mustache
#include <mustache.hpp>
namespace Mustache = kainjow::mustache;

constexpr const char* const REFLECTION_PATH = "/../../../Intermediate/_generated/reflection";
constexpr const char* const TEMPLATE_PATH = "/../../../Template/";

#endif