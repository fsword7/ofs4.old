#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <array>
#include <vector>
#include <optional>
#include <cassert>

#define APP_NAME    "Orbital Flight Simulator"
#define APP_SHORT   "OFS"

// Default window size for 4K monitors
#define OFSW_WIDTH   1920
#define OFSW_HEIGHT  1080

typedef const char          cchar_t;
typedef std::string         str_t;
typedef const std::string   cstr_t;

namespace fs = std::filesystem;
