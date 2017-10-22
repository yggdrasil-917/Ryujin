#ifndef MACROS_HPP
#define MACROS_HPP



#if _WIN32 && _MSC_VER < 1900
#define alignof(type) __alignof(type)
#define constexpr
#endif

#ifdef _MSC_VER
#define FORCEINLINE __forceinline
#define ALIGNMENT(x) __declspec(align(x))
#else
#define FORCEINLINE inline
#define ALIGNMENT(x) __attribute__ ((aligned(x)))
#endif

#if _WIN32 || _WIN64

	#define PLATFORM_WINDOWS 1
	#define RYUJIN_MAX_PATH MAX_PATH
    #define RYUJIN_TLS thread_local

    #ifdef RYUJINCORE_EXPORTS
        #define CORE_API __declspec(dllexport)
    #else
        #define CORE_API __declspec(dllimport)
    #endif

#elif defined(__APPLE__) || defined(__APPLE_CC__)

	#define PLATFORM_MAC 1
	#define RYUJIN_MAX_PATH 1024
    #define RYUJIN_TLS __thread
    #define CORE_API __attribute__ ((visibility ("default")))

#else

    #define PLATFORM_LINUX 1
    #define RYUJIN_MAX_PATH 1024
    #define RYUJIN_TLS thread_local
    #define CORE_API __attribute__ ((visibility ("default")))

#endif

#if ALLOW_DEBUG_ALLOCATORS
    #define USE_DEBUG_ALLOCATORS 1
#endif

#define OVERRIDE override
#define VIRTUAL virtual
#define FINAL final

#define StdMove std::move

// stringizes a string, even macros
#define STRINGIZE_HELPER(token)    #token
#define STRINGIZE(str)             STRINGIZE_HELPER(str)

// concatenates two strings, even when the strings are macros themselves
#define CONCAT(x, y)                    CONCAT_HELPER(x, y)
#define CONCAT_HELPER(x, y)             CONCAT_HELPER_HELPER(x, y)
#define CONCAT_HELPER_HELPER(x, y)      x##y
#define CONCAT_STR_WITH_SPACE(str1, str2) #str1 " " #str2

// VA_NUM_ARGS() is a very nifty macro to retrieve the number of arguments handed to a variable-argument macro
// unfortunately, VS 2010 still has this compiler bug which treats a __VA_ARGS__ argument as being one single parameter:
// https://connect.microsoft.com/VisualStudio/feedback/details/521844/variadic-macro-treating-va-args-as-a-single-parameter-for-other-macros#details
#if _MSC_VER >= 1400
#define VA_NUM_ARGS_HELPER(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...)    N
#define VA_NUM_ARGS_REVERSE_SEQUENCE            10, 9, 8, 7, 6, 5, 4, 3, 2, 1
#define LEFT_PARENTHESIS (
#define RIGHT_PARENTHESIS )
// a macro for evaluating how many parameters have been passed to a variable argument macro
#define VA_NUM_ARGS(...)                        VA_NUM_ARGS_HELPER LEFT_PARENTHESIS __VA_ARGS__, VA_NUM_ARGS_REVERSE_SEQUENCE RIGHT_PARENTHESIS
#else
#define VA_NUM_ARGS(...)                        VA_NUM_ARGS_HELPER(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define VA_NUM_ARGS_HELPER(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...)    N
#endif

// PASS_VA passes __VA_ARGS__ as multiple parameters to another macro, working around the above-mentioned bug
#if _MSC_VER >= 1400
#define PASS_VA(...)                            LEFT_PARENTHESIS __VA_ARGS__ RIGHT_PARENTHESIS
#else
#define PASS_VA(...)                            (__VA_ARGS__)
#endif

#define SAFE_DELETE(x) if (x) { delete x; x = nullptr; }
#define SAFE_DELETE_ARRAY(x) if (x) { delete[] x; x = nullptr; }

#endif
