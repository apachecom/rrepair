// #include <assert.h>
// #include <cmath>
// #include <map>
// #include <stack>
// #include <string>
// #include <unordered_map>
// #include <vector>

#define FATAL(message) std::cerr << "[FATAL] " << message << std::endl
#define ERROR(message) std::cerr << "[ERROR] " << message << std::endl
#define WARN(message) std::cerr << "[WARN] " << message << std::endl
#define INFO(message) std::cerr << "[INFO] " << message << std::endl
#ifdef NDEBUG
#define LOG(message) 0
#else
#define LOG(message) std::cerr << "[DEBUG] " << message << std::endl
#endif
#define TRACE(message)                                     \
    std::cerr << "[TRACE] " << __FILE__ << "/" << __LINE__ \
              << "/ " #message " : " << message << std::endl
#define COUT(message) std::cout << message << std::endl

// typedef char Char;
// typedef unsigned char Uchar;
// typedef int32_t Int;
// typedef uint32_t Uint;
// typedef int64_t Long;
// typedef u_int64_t Ulong;
