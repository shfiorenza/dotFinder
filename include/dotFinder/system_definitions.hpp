
#ifndef _DOTFINDER_SYSTEM_DEFINITIONS_HPP_
#define _DOTFINDER_SYSTEM_DEFINITIONS_HPP_
#include <cassert>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

// Use Sys namespace for variables/functions we'd like to access system-wide
namespace Sys {
/* System-wide variables */
inline std::string img_filename_{"NULL.jpg"}; // Name of image being analyzed

/* Stylistic stuff */
using Str = std::string;
using SysClock = std::chrono::steady_clock;
using SysTimepoint = SysClock::time_point;
template <typename DATA_T> using Vec = std::vector<DATA_T>;
template <typename DATA_T> using Vec2D = Vec<Vec<DATA_T>>;
template <typename DATA_T> using Vec3D = Vec<Vec<Vec<DATA_T>>>;
template <typename DATA_T> using Vec4D = Vec<Vec<Vec<Vec<DATA_T>>>>;
template <typename DATA_T> using Vec5D = Vec<Vec<Vec<Vec<Vec<DATA_T>>>>>;
template <typename DATA_T> using Fn = std::function<DATA_T>;
template <typename T1, typename T2> using Map = std::map<T1, T2>;
template <typename T1, typename T2> using UMap = std::unordered_map<T1, T2>;
template <typename T1, typename T2> using Pair = std::pair<T1, T2>;
} // namespace Sys

#endif