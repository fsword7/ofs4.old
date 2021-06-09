#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <array>
#include <vector>
#include <optional>
#include <cassert>
#include <cmath>

#define APP_NAME    "Orbital Flight Simulator"
#define APP_SHORT   "OFS"

// Default window size for 4K monitors
#define OFSW_WIDTH   1920
#define OFSW_HEIGHT  1080

typedef const char          cchar_t;
typedef std::string         str_t;
typedef const std::string   cstr_t;

namespace fs = std::filesystem;

// Math libraru section
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

// 32-bit single precision floating
typedef glm::tvec2<float> vec2f_t;
typedef glm::tvec3<float> vec3f_t;
typedef glm::tvec4<float> vec4f_t;
typedef glm::tquat<float> quatf_t;

typedef glm::tmat3x3<float> mat3f_t;
typedef glm::tmat4x4<float> mat4f_t;

// 64-bit double precision floating
typedef glm::tvec2<double> vec2d_t;
typedef glm::tvec3<double> vec3d_t;
typedef glm::tvec4<double> vec4d_t;
typedef glm::tquat<double> quatd_t;

typedef glm::tmat3x3<double> mat3d_t;
typedef glm::tmat4x4<double> mat4d_t;

// 80-bit extended precision floating 
typedef glm::tvec2<long double> vec2e_t;
typedef glm::tvec3<long double> vec3e_t;
typedef glm::tvec4<long double> vec4e_t;
typedef glm::tquat<long double> quate_t;

typedef glm::tmat3x3<long double> mat3e_t;
typedef glm::tmat4x4<long double> mat4e_t;

