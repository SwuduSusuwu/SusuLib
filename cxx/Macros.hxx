/* Licenses: allows all uses ("Creative Commons"/"Apache 2") */
#ifndef INCLUDES_cxx_Macros_hxx
#define INCLUDES_cxx_Macros_hxx
/* Miscellaneous macros */
/* To printout default preprocessor definitions:
 * for X={clang, clang++, gcc, g++, hipcc, icc}: `$X -dM -E -x c++ /dev/null`
 * replace `/dev/null` with a file (such as `cxx/Macros.hxx`) to printout actual preprocessor definitions
 * for MSVC: `git clone --depth 1 https://github.com/MicrosoftDocs/cpp-docs.git && vim cpp-docs/blob/main/docs/preprocessor/predefined-macros.md` or browse to https://learn.microsoft.com/en-us/cpp/preprocessor/predefined-macros 
 * for others: `git clone https://github.com/cpredef/predef.git && vim predef/Compilers.md`
 */ /* To pass new preprocessor definitions (example is `#define USE_CONTRACTS true`):
 * to `clang`/`clang++`/`gcc`/`g++`/Intel(`icc`): `-DUSE_CONTRACTS=true`
 * to MSVC(`cl`): `\DUSE_CONTRACTS=true`
 */
#ifdef __cplusplus
#	include <cassert> /* assert static_assert */
/* `clang-tidy` off: NOLINTBEGIN(cppcoreguidelines-macro-usage); https://github.com/SwuduSusuwu/SubStack/issues/3 more simple with macros, plus some of the `constexpr` versions require `__cplusplus >= 202002` */
#	define IF_SUSUWU_CPLUSPLUS(TRUE, FALSE) TRUE
#else /* !(defined __cplusplus */
#	include <assert.h> /* assert */
#	define IF_SUSUWU_CPLUSPLUS(TRUE, FALSE) FALSE
#	define SUSUWU_SH_PREFER_STDIO
#endif /* !(defined __cplusplus */
#ifdef SUSUWU_SH_PREFER_STDIO /* `-DSUSUWU_SH_PREFER_STDIO` to force this. Replaces `std::cXXX << x << std::endl;` with `fprintf(stdXXX, "%s\n", x);` */
#	include <stdio.h> /* fprintf stderr stdout */
#else
#	include <iostream> /* std::cerr std::cout std::endl */
#endif
namespace Susuwu { /* namespaces do not affect macros. Is just standard practice to wrap all of a project's contents with namespaces. */
#define SUSUWU_GLUE2(S, U) S##U /* concatanates 2 macro constants */
#define SUSUWU_GLUE(S, U) SUSUWU_GLUE2(S, U) /* concatanates 2 macro functions or constants */
#define SUSUWU_COMMA , /* to pass to macro functions whose `__VA_ARGS__` is conditional */
#if (!defined _POSIX_VERSION) && (defined _POSIX_C_SOURCE)
#	define _POSIX_VERSION _POSIX_C_SOURCE /* "Error: ... ndef _POSIX_VERSION" fix. Now, you can just do `#ifdef _POSIX_VERSION` for POSIX code paths */
#endif /* (!defined _POSIX_VERSION) && (defined _POSIX_C_SOURCE) */

/* `#pragma S` in macro functions is `_Pragma(S)`, but without this wrap gives `error: _Pragma takes a parenthesized string literal`/`expected string literal in pragma message`. Use as `SUSUWU_PRAGMA(message("Message"))` */
#define SUSUWU_PRAGMA(S) _Pragma(#S)

#if !defined(NDEBUG) && !defined(SUSUWU_SH_VERBOSE)
# define SUSUWU_SH_VERBOSE true /* diagnostic logs to `cerr`/`stderr`; can enable on `--release` with `-DSUSUWU_SH_VERBOSE=true` */
#else
# define SUSUWU_SH_VERBOSE false /* can disable on `--debug` with `-DSUSUWU_SH_VERBOSE=false` */
#endif

#if !defined(SUSUWU_SH_SKIP_BRACKETS) || SUSUWU_SH_SKIP_BRACKETS == false /* overridable with `-DSUSUWU_SH_SKIP_BRACKETS true` (which you can set to mimic `g++`/`clang++` syntax for outputs) */
#	define IF_SUSUWU_SH_BRACKETS(TRUE, FALSE) TRUE
#else
#	define IF_SUSUWU_SH_BRACKETS(TRUE, FALSE) FALSE
#endif

#if (!defined(SUSUWU_SH_FILE) && SUSUWU_SH_VERBOSE) || SUSUWU_SH_FILE /* overridable with `-DSUSUWU_SH_FILE true/false` */
#	define SUSUWU_SH_USE_FILE /* affix `__FILE__ ":"` to `stderr`/`cerr` printout */
#endif
#if (!defined(SUSUWU_SH_LINE) && SUSUWU_SH_VERBOSE) || SUSUWU_SH_LINE /* overridable with `-DSUSUWU_SH_LINE true/false` */
#	define SUSUWU_SH_USE_LINE /* affix `__LINE__ ":"` to `stderr`/`cerr` printout */
#endif
#if defined(SUSUWU_SH_FUNC) && SUSUWU_SH_FUNC /* overridable with `-DSUSUWU_SH_FUNC true/false` */
#	define SUSUWU_SH_USE_FUNC /* affix `__func__ ":"` to `stderr`/`cerr` printout */
#endif
#ifdef SUSUWU_SH_USE_FILE
#	define IF_SUSUWU_SH_FILE(U /* wrap clauses which print __FILE__ to `cerr`/`cout` */) U /* printout */
#else
#	define IF_SUSUWU_SH_FILE(U) /* don't printout */
#endif
#ifdef SUSUWU_SH_USE_LINE
#	define IF_SUSUWU_SH_LINE(U /* wrap clauses which print __LINE__ to `cerr`/`cout` */) U /* printout */
#else
#	define IF_SUSUWU_SH_LINE(U) /* don't printout */
#endif
#ifdef SUSUWU_SH_USE_FUNC
#	define IF_SUSUWU_SH_FUNC(U /* wrap clauses which print __func__ to `cerr`/`cout` */) U /* printout */
#else
#	define IF_SUSUWU_SH_FUNC(U) /* don't printout */
#endif
#if defined(SUSUWU_SH_USE_FILE) || defined(SUSUWU_SH_USE_LINE) || defined(SUSUWU_SH_USE_FUNC)
#	define IF_SUSUWU_SH_FILE_LINE_OR_FUNC(U /* wrap clauses common to `__FILE__`, `__LINE__`, `__func__` use */) U /* printout */
#else
#	define IF_SUSUWU_SH_FILE_LINE_OR_FUNC(U) /* don't printout */
#endif
#ifdef SUSUWU_SH_RUNTIME_COLORS
#	pragma message("[Notice: SUSUWU_SH_RUNTIME_COLORS is TODO; https://github.com/SwuduSusuwu/SubStack/issues/17 to contribute]")
#endif /* #elif !defined(_POSIX_VERSION) TODO */
#ifdef SUSUWU_SH_RUNTIME_OSC
#	pragma message("[Notice: SUSUWU_SH_RUNTIME_OSC is TODO; https://github.com/SwuduSusuwu/SubStack/issues/17 to contribute]")
#endif /* #elif !defined(_POSIX_VERSION) TODO */
#if !defined(_POSIX_VERSION)
#	define SUSUWU_SH_COLORS_UNSUPPORTED /* assume "dumb terminals" (such as __WIN32__ or __MINGW32__ often has) if built without runtime tests for color attributes, on non-POSIX systems */
#	define SUSUWU_SH_OSC_UNSUPPORTED /* assume "dumb terminals" (such as __WIN32__ or __MINGW32__ often has) if built without runtime tests for Operating System Commands, on non-POSIX systems */
#endif
#if defined(SUSUWU_SH_OSC_UNSUPPORTED) && !defined(SUSUWU_SH_SKIP_OSC)
#	define SUSUWU_SH_SKIP_OSC true /* you can use `-DSUSUWU_SH_SKIP_OSC=false` to force unsupported Operating System Command use (such as if build is __WIN32__ but you assume Win10+ `xterm` support) */
#endif /* SUSUWU_SH_OSC_UNSUPPORTED && !defined SUSUWU_SH_SKIP_OSC */
#ifdef SUSUWU_SH_SKIP_OSC /* `-DSUSUWU_SH_SKIP_OSC=true` to turn Operating System Commands off */
#	define SUSUWU_SH_TO_CLIPBOARD_PREFIX ""
#	define SUSUWU_SH_TO_CLIPBOARD_SUFFIX ""
#	define SUSUWU_SH_TO_CLIPBOARD(base64_str) (true)/* skip */
#else /* def SUSUWU_SH_SKIP_OSC else */
#	define SUSUWU_SH_TO_CLIPBOARD_PREFIX SUSUWU_SH_OSC "52;c;" /* Command to put BASE64 str into clipboard */
#	define SUSUWU_SH_TO_CLIPBOARD_SUFFIX "\a"
#	define SUSUWU_SH_TO_CLIPBOARD(base64_str) fprintf(stdout, SUSUWU_SH_TO_CLIPBOARD_PREFIX "%s" SUSUWU_SH_TO_CLIPBOARD_SUFFIX, IF_SUSUWU_CPLUSPLUS(std::string(base64_str).c_str(), base64_str)); /* TODO: `std::cout` version of this */
#endif /* def SUSUWU_SH_SKIP_OSC else */
#if defined(SUSUWU_SH_COLORS_UNSUPPORTED) && !defined(SUSUWU_SH_SKIP_COLORS)
#	define SUSUWU_SH_SKIP_COLORS true /* you can use `-DSUSUWU_SH_SKIP_COLORS=false` to force unsupported color use (such as if build is for __WIN32__ but you assume Win10+ `xterm` support) */
#endif /* SUSUWU_SH_COLORS_UNSUPPORTED && !defined SUSUWU_SH_SKIP_COLORS */
#	define SUSUWU_SH_BEL "\07" /* Bell sound */
#	define SUSUWU_SH_ESC "\033" /* Escape */
#	define SUSUWU_SH_OSC SUSUWU_SH_ESC "]" /* Operating System Command */
#	define SUSUWU_SH_ST SUSUWU_SH_ESC "\\" /* String Terminator of commands */ /* `BEL` reduces this to 1 byte but just functions as `ST` on `xterm` */
#	define SUSUWU_SH_CSI SUSUWU_SH_ESC "[" /* Control Sequence Introducer */
#if SUSUWU_SH_SKIP_COLORS /* `-DSUSUWU_SH_SKIP_COLORS=true` to turn colors off */
#	define SUSUWU_SH_DEFAULT ""
#	define SUSUWU_SH_BLACK ""
#	define SUSUWU_SH_DARK_GRAY ""
#	define SUSUWU_SH_RED ""
#	define SUSUWU_SH_LIGHT_RED ""
#	define SUSUWU_SH_GREEN ""
#	define SUSUWU_SH_LIGHT_GREEN ""
#	define SUSUWU_SH_BROWN ""
#	define SUSUWU_SH_YELLOW ""
#	define SUSUWU_SH_BLUE ""
#	define SUSUWU_SH_LIGHT_BLUE ""
#	define SUSUWU_SH_PURPLE ""
#	define SUSUWU_SH_LIGHT_PURPLE ""
#	define SUSUWU_SH_CYAN ""
#	define SUSUWU_SH_LIGHT_CYAN ""
#	define SUSUWU_SH_LIGHT_GRAY ""
#	define SUSUWU_SH_WHITE ""
#else /* !SUSUWU_SH_SKIP_COLORS */
#	define SUSUWU_SH_DEFAULT	SUSUWU_SH_CSI "0m"
#	define SUSUWU_SH_BLACK	SUSUWU_SH_CSI "0;30m"
#	define SUSUWU_SH_DARK_GRAY	SUSUWU_SH_CSI "1;30m"
#	define SUSUWU_SH_RED	SUSUWU_SH_CSI "0;31m"
#	define SUSUWU_SH_LIGHT_RED	SUSUWU_SH_CSI "1;31m"
#	define SUSUWU_SH_GREEN	SUSUWU_SH_CSI "0;32m"
#	define SUSUWU_SH_LIGHT_GREEN	SUSUWU_SH_CSI "1;32m"
#	define SUSUWU_SH_BROWN	SUSUWU_SH_CSI "0;33m"
#	define SUSUWU_SH_YELLOW	SUSUWU_SH_CSI "1;33m"
#	define SUSUWU_SH_BLUE	SUSUWU_SH_CSI "0;34m"
#	define SUSUWU_SH_LIGHT_BLUE	SUSUWU_SH_CSI "1;34m"
#	define SUSUWU_SH_PURPLE	SUSUWU_SH_CSI "0;35m"
#	define SUSUWU_SH_LIGHT_PURPLE	SUSUWU_SH_CSI "1;35m"
#	define SUSUWU_SH_CYAN	SUSUWU_SH_CSI "0;36m"
#	define SUSUWU_SH_LIGHT_CYAN	SUSUWU_SH_CSI "1;36m"
#	define SUSUWU_SH_LIGHT_GRAY	SUSUWU_SH_CSI "0;37m"
#	define SUSUWU_SH_WHITE	SUSUWU_SH_CSI "1;37m"
#endif /* !SUSUWU_SH_SKIP_COLORS */
#define SUSUWU_SH_FILE __FILE__ ":"
#define SUSUWU_SH_PREFIX IF_SUSUWU_SH_BRACKETS("[", "") SUSUWU_SH_WHITE
#define SUSUWU_SH_ERROR SUSUWU_SH_RED "Error: " SUSUWU_SH_WHITE
#define SUSUWU_SH_WARNING SUSUWU_SH_PURPLE "Warning: " SUSUWU_SH_WHITE
#define SUSUWU_SH_INFO SUSUWU_SH_CYAN "Info: " SUSUWU_SH_WHITE
#define SUSUWU_SH_SUCCESS SUSUWU_SH_GREEN "Success: " SUSUWU_SH_WHITE
#define SUSUWU_SH_NOTICE SUSUWU_SH_BLUE "Notice: " SUSUWU_SH_WHITE
#define SUSUWU_SH_DEBUG SUSUWU_SH_BLUE "Debug: " SUSUWU_SH_WHITE
#define SUSUWU_SH_POSTFIX IF_SUSUWU_SH_BRACKETS("]", "")

#define SUSUWU_ERRSTR_IMP(WARN_LEVEL, x) std::string(SUSUWU_GLUE2(SUSUWU_SH_, WARN_LEVEL)) + std::string(x) + std::string(SUSUWU_SH_DEFAULT)
#define SUSUWU_CERR_IMP(WARN_LEVEL, x) SUSUWU_GLUE2(SUSUWU_SH_, WARN_LEVEL) << (x) << SUSUWU_SH_DEFAULT
#define SUSUWU_STDERR_IMP(WARN_LEVEL, prefix, postfix, x, ... /* must pass SUSUWU_COMMA after __VA_ARGS__ params */) fprintf(stderr, prefix SUSUWU_GLUE2(SUSUWU_SH_, WARN_LEVEL) "%s" SUSUWU_SH_DEFAULT postfix, __VA_ARGS__ IF_SUSUWU_CPLUSPLUS(std::string(x).c_str(), x))

/* WARN_LEVEL = {ERROR, WARNING, INFO, SUCCESS, NOTICE, DEBUG} */
#define SUSUWU_ERRSTR(WARN_LEVEL, x) std::string(SUSUWU_SH_PREFIX) IF_SUSUWU_SH_FILE(+ SUSUWU_SH_FILE) IF_SUSUWU_SH_LINE(+ std::to_string(__LINE__) + ':') IF_SUSUWU_SH_FUNC(+ std::string(__func__) + ':') IF_SUSUWU_SH_FILE_LINE_OR_FUNC(+ ' ') + SUSUWU_ERRSTR_IMP(WARN_LEVEL, x) + SUSUWU_SH_POSTFIX
#define SUSUWU_CERR(WARN_LEVEL, x) std::cerr << SUSUWU_SH_PREFIX IF_SUSUWU_SH_FILE(<< std::string(SUSUWU_SH_FILE)) IF_SUSUWU_SH_LINE(<< std::to_string(__LINE__) << ":") IF_SUSUWU_SH_FUNC(<< std::string(__func__) << ":") IF_SUSUWU_SH_FILE_LINE_OR_FUNC(<< ' ') << SUSUWU_CERR_IMP(WARN_LEVEL, x) << SUSUWU_SH_POSTFIX << std::endl
#define SUSUWU_STDERR(WARN_LEVEL, x) SUSUWU_STDERR_IMP(WARN_LEVEL, SUSUWU_SH_PREFIX IF_SUSUWU_SH_FILE(SUSUWU_SH_FILE) IF_SUSUWU_SH_LINE("%i:") IF_SUSUWU_SH_FUNC("%s:") IF_SUSUWU_SH_FILE_LINE_OR_FUNC(" "), SUSUWU_SH_POSTFIX "\n", x, IF_SUSUWU_SH_LINE(__LINE__ SUSUWU_COMMA) IF_SUSUWU_SH_FUNC(__func__ SUSUWU_COMMA))
/* Use this to do C versus C++ agnostic code */
#ifdef SUSUWU_SH_PREFER_STDIO
#	define SUSUWU_PRINT(LEVEL, x) SUSUWU_STDERR(LEVEL, x)
#else
#	define SUSUWU_PRINT(LEVEL, x) SUSUWU_CERR(LEVEL, x)
#endif
#define SUSUWU_ERROR(x) SUSUWU_PRINT(ERROR, x)
#define SUSUWU_WARNING(x) SUSUWU_PRINT(WARNING, x)
#define SUSUWU_INFO(x) SUSUWU_PRINT(INFO, x)
#define SUSUWU_SUCCESS(x) SUSUWU_PRINT(SUCESS, x)

/* Use this to just print debug/notices to `--debug` builds (+ do conditional execution) */
#if SUSUWU_SH_VERBOSE
#	define SUSUWU_NOTICE(x) SUSUWU_PRINT(NOTICE, x)
#	define SUSUWU_DEBUG(x) SUSUWU_PRINT(DEBUG, x)
#	define SUSUWU_EXECUTEVERBOSE(x) x /* about side-effects; do not assume that `--debug` was used. `--release -DSUSUWU_SH_VERBOSE=true` will execute this. */
#else /* else SUSUWU_SH_VERBOSE */
#	define SUSUWU_NOTICE(x) (true)/* skip */
#	define SUSUWU_DEBUG(x) (true)/* skip */
#	define SUSUWU_EXECUTEVERBOSE(x) (true)/*skip*/ /* about side-effects; do not assume that just `--release` was used. `--debug -DSUSUWU_SH_VERBOSE=false` will skip. */
#endif /* else SUSUWU_SH_VERBOSE */

/* Use this to reduce print (NOTICE/DEBUG is conditional) + (unconditional) execute into single statement */
#define SUSUWU_ERROR_EXECUTE(x) ((SUSUWU_ERROR(#x)), (x))
#define SUSUWU_WARNING_EXECUTE(x) ((SUSUWU_WARNING(#x)), (x))
#define SUSUWU_INFO_EXECUTE(x) ((SUSUWU_INFO(#x)), (x))
#define SUSUWU_SUCCESS_EXECUTE(x) ((SUSUWU_SUCCESS(#x)), (x))
#define SUSUWU_NOTICE_EXECUTE(x) ((SUSUWU_NOTICE(#x)), (x))
#define SUSUWU_DEBUG_EXECUTE(x) ((SUSUWU_DEBUG(#x)), (x))

/* Use this to reduce (conditional) print + (conditional) execute into single statement */
#define SUSUWU_NOTICE_EXECUTEVERBOSE(x) ((SUSUWU_NOTICE(#x)), SUSUWU_EXECUTEVERBOSE(x))
#define SUSUWU_DEBUG_EXECUTEVERBOSE(x) ((SUSUWU_DEBUG(#x)), SUSUWU_EXECUTEVERBOSE(x))

#ifndef __has_feature
#	define __has_feature(X) false /* `gcc` "error: missing binary operator before token \"(\"" fix */
#endif /* ndef __has_feature */
#if (!defined _POSIX_VERSION) && (_POSIX_C_SOURCE)
#	define _POSIX_VERSION _POSIX_C_SOURCE /* "Error: ... ndef _POSIX_VERSION" fix */
#endif /* (!defined _POSIX_VERSION) && (_POSIX_C_SOURCE) */
#if (defined __cplusplus && 201102 < __cplusplus)
#	define SUSUWU_CXX11
#endif /* if (defined __cplusplus && 201402 <= __cplusplus) */
#if (defined __cplusplus && 201402 <= __cplusplus)
#	define SUSUWU_CXX14
#endif /* if (defined __cplusplus && 201402 < __cplusplus) */
#if (defined __cplusplus && 201702 < __cplusplus)
#	define SUSUWU_CXX17
#endif /* if (defined __cplusplus && 201702 < __cplusplus) */
#if (defined __cplusplus && 202002 <= __cplusplus)
#	define SUSUWU_CXX20
#endif /* if (defined __cplusplus && 202002 <= __cplusplus) */

#if (!defined __WIN32__) && (defined _WIN32 /* || defined __CYGWIN__ should use "#ifdef _POSIX_VERSION" path */ || __MSC_VER)
#	define __WIN32__ /* https://stackoverflow.com/questions/430424/are-there-any-macros-to-determine-if-my-code-is-being-compiled-to-windows/430435#430435 says that __WIN32__ is not always defined on Windows targets */
#endif

/* `UNREACHABLE` is close to `ASSUME(false)` */
#if (!defined NDEBUG) && (defined(SUSUWU_CXX11))
/* [https://stackoverflow.com/questions/2249282/c-c-portable-way-to-detect-debug-release] [https://stackoverflow.com/questions/2290509/debug-vs-ndebug] */
/* Debug: Promises unreachable, for static analysis */
#	define UNREACHABLE static_assert(false)
#elif (!defined NDEBUG)
#	define UNREACHABLE assert(false)
#else
}; /* namespace Susuwu */
#	include <version> /* __cpp_lib_unreachable */ /* [https://en.cppreference.com/w/cpp/feature_test] */
#	if defined(__cpp_lib_unreachable) && __cpp_lib_unreachable
/* Release: Promises executable can not reach this spot, for compiler which optimizes this. Warning: `UNREACHABLE && UB (undefined behaviour)` */
#		include <utility> /* std::unreachable() */
#		define UNREACHABLE std::unreachable()
#	elif (defined __GNUC__) && ((4 <= __GNUC__ && 4 < __GNUC_MINOR__) || 4 < __GNUC__) /* `~ $ g++` */
#		define UNREACHABLE __builtin_unreachable()
#	else /* else (!def NDEBUG) && (!supports unreachable) */
#		define UNREACHABLE /* No-op */
#	endif /* __cpp_lib_unreachable elif IS_GCC ...*/
namespace Susuwu {
#endif  /* #elif (!defined NDEBUG) ... #else */

#ifdef USE_CONTRACTS /* Pass `-DUSE_CONTRACTS` once compiler has C++26 (Contracts) */
/* `EXPECTS(X)` is close to `@pre @code X @endcode` or `ASSUME(X)` but is for headers; https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p2521r2.html */
/* Promises `(true == (X))`, for static analysis, or for compiler which optimizes this. Warning: `if(!(X)) {UB (undefined behaviour)}` */
#	define EXPECTS(X) [[expects: X]] /* Usage: `void pushf(std::deque<float> &x, float f) EXPECTS(!x.full());` */
#	define ENSURES(X) [[ensures: X]] /* Usage: `void pushf(std::deque<float> &x, float f) ENSURES(0 != x.size());` */
#else /* else !def USE_CONTRACTS */
#	define EXPECTS(X) /* `@pre @code X @endcode` */
#	define ENSURES(X) /* `@post @code X @encode` */
#endif /* else !def USE_CONTRACTS */

/* `ASSUME(X)` is close to `@pre @code X @endcode` or `[[expects: x]]` */
/* TODO: choose best of [various possible ASSUME macros](https://stackoverflow.com/questions/44054078/how-to-guide-gcc-optimizations-based-on-assertions-without-runtime-cost) */
#ifndef NDEBUG
/* Debug: Promises `(true == (X))`, for static analysis */
#	ifdef SUSUWU_CXX11
#		define ASSUME(X) static_assert(X)
#	else /* def SUSUWU_CXX11 else */
#		define ASSUME(X) assert(X)
#	endif /* def SUSUWU_CXX11 else */
#elif (!defined USE_ASSUME) || USE_ASSUME /* Default: if(!NDEBUG) USE_ASSUME=true; pass `-DUSE_ASSUME=false` to disable this */ 
/* Release: Promises `(true == (X))`, for compiler which optimizes this. Warning: `if(!(X)) {UB (undefined behaviour)}` */
#	ifdef IS_MSVC
#		define ASSUME(X) __assume(X)
#	elif __clang__ /* `~ $ clang++` */
#		define ASSUME(X) __builtin_assume(X)
#	else /* (!def IS_MSVC) && (!def __clang__) */
#		define ASSUME(X) ((X) ? static_cast<void>(0) : UNREACHABLE)
#	endif /* !def IS_MSVC */
#else /* !def USE_ASSUME */
#	define ASSUME(X)
#endif /* !def USE_ASSUME */
/* `clang-tidy` on: NOLINTEND(cppcoreguidelines-macro-usage) */

#if defined(SUSUWU_CXX11) || (defined(__clang__) && __has_feature(cxx_noexcept)) || (defined(__GXX_EXPERIMENTAL_CXX0X__) && __GNUC__ * 10 + __GNUC_MINOR__ >= 46) || (defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 180021114) /* [Other `noexcept` tests](https://stackoverflow.com/questions/18387640/how-to-deal-with-noexcept-in-visual-studio) */
#	define NOEXCEPT noexcept /* Usage: `void info() NOEXCEPT; ... {info();}` is close to `void versionInfo() [[ensures: true]]; ... {info();}` or `{try {versionInfo();} catch(...) {UNREACHABLE;}} */
		/* Usage 2: `void versionInfo() NOEXCEPT(std::is_nothrow_constructible<U>::value); {versionInfo();}` is close to `{try {versionInfo();} catch(...) {if(std::is_nothrow_constructible<U>::value) {UNREACHABLE;}}}` */
#else /* C++11 else */
#	define NOEXCEPT /* old `g++`/`clang++` "error: expected function body after function declarator" fix */
#endif /* else no `noexcept` */
#if defined(SUSUWU_CXX11) || ((defined __has_cpp_attribute) && __has_cpp_attribute(noreturn)) /* TODO: [Cmake test for `\[\[noreturn\]\]`](https://stackoverflow.com/a/33517293/24473928) */
#	define NORETURN [[noreturn]] /* Usage: `NORETURN void exit();` is close to `void exit() [[ensures:: false]];` or `exit(); UNREACHABLE;*/
#else /* C++11 else */
#	define NORETURN /* old `g++` "error: 'NORETURN' does not name a type" / old `clang++` "error: unknown type name 'NORETURN'" fix */
#endif /* else no `[[noreturn]]` */
}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_Macros_hxx */

