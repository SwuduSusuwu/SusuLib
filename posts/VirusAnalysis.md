**Virus analysis tools should use local static analysis + sandboxes + artificial CNS (central nervous systems) to secure us**
_[This post](https://swudususuwu.substack.com/p/howto-produce-better-virus-scanners) allows all uses._

Static analysis + sandbox + CNS = 1 second (approx) analysis of **new executables** (protects all app launches,) but _caches_ reduce this to **less than 1ms** (just cost to lookup `ResultList::hashes`, which is `std::unordered_set<decltype(classSha2(const FileBytecode &))>`; a hashmap of hashes).

`Licenses: allows all uses ("Creative Commons"/"Apache 2")`
[Removed duplicate licenses, `#if` guards, `#include`s, `namespace`s, `NOLINTBEGIN`s, `NOLINTEND`s from all except `main.hxx`; follow URLs for whole sources]
For the most new sources (+ static libs), use apps such as [iSH](https://apps.apple.com/us/app/ish-shell/id1436902243) (for **iOS**) or [Termux](https://play.google.com/store/apps/details?id=com.termux) (for **Android OS**) to run this:
`git clone https://github.com/SwuduSusuwu/SubStack.git && cd ./Substack/ && ./build`
`less` [cxx/Macros.hxx](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/Macros.hxx) /* removed: disabled color codes + unused OSC codes */
```
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
#if defined(SUSUWU_PREFER_C) || !defined(__cplusplus)
#	define SUSUWU_SH_PREFER_STDIO
#	define SUSUWU_SH_PREFER_CSTR
#endif /*defined((SUSUWU_PREFER_C) || !defined(__cplusplus) */
#ifdef SUSUWU_SH_PREFER_STDIO /* `-DSUSUWU_SH_PREFER_STDIO` to force this. Replaces `std::cXXX << x << std::endl;` with `fprintf(stdXXX, "%s\n", x);` */
#	include <stdio.h> /* fprintf stderr stdout */
#else
#	include <iostream> /* std::cerr std::cout std::endl */
#endif

#ifdef __cplusplus
#	include <cassert> /* assert static_assert */
#	define IF_SUSUWU_CPLUSPLUS(TRUE, FALSE) TRUE
#	if 199901 <= __cplusplus
#		define SUSUWU_CXX98
#	endif /* (199901 <= __cplusplus) */
#	if 201102 < __cplusplus
#		define SUSUWU_CXX11
#	endif /* (201102 < __cplusplus) */
#	if 201402 <= __cplusplus
#		define SUSUWU_CXX14
#	endif /* if (201402 < __cplusplus) */
#	if 201702 < __cplusplus
#		define SUSUWU_CXX17
#	endif /* if (201702 < __cplusplus) */
#	if 202002 <= __cplusplus
#		define SUSUWU_CXX20
#		define SUSUWU_NO_UNIQUE_ADDRESS [[no_unique_address]] /* use this attribute on member subobjects if `std::is_empty<MemberClass>::value == true`, if you want those to not pad (most compilers pad such that `1 == sizeof(zero)` in `macrosNoUniqueAddressTest`. */
#	else /* (202002 <= __cplusplus) else */
#		define SUSUWU_NO_UNIQUE_ADDRESS /* No-op */
#	endif /* if (202002 <= __cplusplus) */
#else /* def __cplusplus */
#	include <assert.h> /* assert static_assert */
#	define IF_SUSUWU_CPLUSPLUS(TRUE, FALSE) FALSE
#	if (199901 <= __STDC_VERSION__)
#		define SUSUWU_C99
#	endif /* (199901 <= __STDC_VERSION__) */
#	if (201112 <= __STDC_VERSION__)
#		define SUSUWU_C11
#	endif /* (201112 <= __STDC_VERSION__) */
#endif /* !(defined __cplusplus */

#define SUSUWU_GLUE2(S, U) S##U /* concatanates 2 macro constants */
#define SUSUWU_GLUE(S, U) SUSUWU_GLUE2(S, U) /* concatanates 2 macro functions or constants */
#define SUSUWU_COMMA , /* to pass to macro functions whose `__VA_ARGS__` is conditional */
#define SUSUWU_PRAGMA(S) _Pragma(#S) /* `#pragma S` in macro functions is `_Pragma(S)` (but without this indirection/wrap, gives `error: _Pragma takes a parenthesized string literal`/`expected string literal in pragma message`.) Usage: `SUSUWU_PRAGMA(message("Message"))` */

#if defined(_POSIX_VERSION) || defined(_POSIX_C_SOURCE) || defined(__CYGWIN__) /* Purpose: Often, `_POSIX_VERSION` is not set on POSIX targets */
#	define SUSUWU_POSIX _POSIX_VERSION /* Usage: `#ifdef SUSUWU_WIN32\n#include <unistd.h>\n#endif` */
#endif /* defined*_POSIX_VERSION) || defined(_POSIX_C_SOURCE) || defined(__CYGWIN__) */
#if defined(__WIN32__) || defined(_WIN32) || defined(__MSC_VER) || defined(__MINGW32__) /* Purpose: https://stackoverflow.com/questions/430424/are-there-any-macros-to-determine-if-my-code-is-being-compiled-to-windows/430435#430435 says that __WIN32__ is not always defined on Windows targets */
#	define SUSUWU_WIN32 /* Usage: `#ifdef SUSUWU_WIN32\n#include <windows.h>\n#endif` */
#endif /* defined(__WIN32__) || defined(_WIN32) || defined(__MSC_VER) || defined(__MINGW32__) */
#if defined(__clang__) || defined(__has_feature) /* Purpose: `gcc` "error: missing binary operator before token \"(\"" fix */
#	define SUSUWU_HAS_FEATURE(X) __has_feature(x) /* Usage: `#if SUSUWU_HAS_FEATURE(cxx_noexcept)\nnoexcept\n#endif` */
#else /* defined(__clang__) || defined(__has_feature) else */
#	define SUSUWU_HAS_FEATURE(X) false
#endif /* defined(__clang__) || defined(__has_feature) else */
#if defined(__has_cpp_attribute)
#	define SUSUWU_HAS_ATTRIBUTE(X) __has_cpp_attribute(x) /* Usage: `#if SUSUWU_HAS_ATTRIBUTE(noreturn)\nnoreturn\n#endif` */
#else /* defined(__has_cpp_attribute) else */
#	define SUSUWU_HAS_ATTRIBUTE(X) false
#endif /* defined(__has_cpp_attribute) else */

#if defined(SUSUWU_C11) || defined(SUSUWU_CXX11)
#	define SUSUWU_NORETURN [[noreturn]] /* Usage: `SUSUWU_NORETURN void exit();` is close to `void exit() [[ensures:: false]];` or `exit(); SUSUWU_UNREACHABLE;` */
#	define SUSUWU_CONSTEXPR constexpr /* Usage: `SUSUWU_CONSTEXPR bool passes(); SUSUWU_STATIC_ASSERT(passes());` is close to `#define PASSES\nSUSUWU_STATIC_ASSERT(PASSES)` */
#else
#	define SUSUWU_NORETURN /* old `g++` "error: 'SUSUWU_NORETURN' does not name a type" / old `clang++` "error: unknown type name 'SUSUWU_NORETURN'" fix */
#	define SUSUWU_CONSTEXPR /* No-op */
#endif /* defined(SUSUWU_C11) || defined(SUSUWU_CXX11) else */

#ifdef USE_CONTRACTS /* Pass `-DUSE_CONTRACTS` once compiler has C++26 (Contracts) */
/* `SUSUWU_EXPECTS(X)` is close to `@pre @code X @endcode` or `SUSUWU_ASSUME(X)` but is for headers; https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p2521r2.html */
/* Promises `(true == (X))`, for static analysis, or for compiler which optimizes this. Warning: `if(!(X)) {UB (undefined behaviour)}` */
#	define SUSUWU_EXPECTS(X) [[expects: X]] /* Usage: `void pushf(std::deque<float> &x, float f) SUSUWU_EXPECTS(!x.full());` */
#	define SUSUWU_ENSURES(X) [[ensures: X]] /* Usage: `void pushf(std::deque<float> &x, float f) SUSUWU_ENSURES(0 != x.size());` */
#else /* else !def USE_CONTRACTS */
#	define SUSUWU_EXPECTS(X) /* `@pre @code X @endcode` */
#	define SUSUWU_ENSURES(X) /* `@post @code X @encode` */
#endif /* else !def USE_CONTRACTS */

#define SUSUWU_STATIC static /* Wrap for possible future instrumentation/diagnostics */
#define SUSUWU_EXTERN extern /* Wrap for possible future instrumentation/diagnostics */
#if defined(SUSUWU_C99) || defined(SUSUWU_CXX98)
#	define SUSUWU_INLINE inline
#else /* (defined(SUSUWU_C99) || defined(SUSUWU_CXX98)) else */
#	define SUSUWU_INLINE static
#	pragma message("[Warning: `#define SUSUWU_INLINE static` due to `!defined(SUSUWU_C99) && !defined(SUSUWU_CXX98)`.]")
#endif /* (defined(SUSUWU_C99) || defined(SUSUWU_CXX98)) else */
#if defined(SUSUWU_CXX11) /* TODO? (pre-CXX11 support) || SUSUWU_HAS_FEATURE(cxx_noexcept) || (defined(__GXX_EXPERIMENTAL_CXX0X__) && __GNUC__ * 10 + __GNUC_MINOR__ >= 46) || (defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 180021114) */ /* [Other `noexcept` tests](https://stackoverflow.com/questions/18387640/how-to-deal-with-noexcept-in-visual-studio) */
#	define SUSUWU_NOEXCEPT noexcept /* Usage: `void info() SUSUWU_NOEXCEPT; ... {info();}` is close to `void versionInfo() [[ensures: true]]; ... {info();}` or `{try {versionInfo();} catch(...) {SUSUWU_UNREACHABLE;}} */
#	define SUSUWU_DEFAULT default /* Usage: `Macros.cxx` has tests for this */
#	define SUSUWU_DELETE delete /* Usage: `Macros.cxx` has tests for this */
#	define SUSUWU_FINAL final /* Usage: `Macros.cxx` has tests for this */
#	define SUSUWU_NULLPTR nullptr /* Usage: `Macros.cxx` has tests for this */
#	define SUSUWU_OVERRIDE override /* Usage: `Macros.cxx` has tests for this */
		/* Usage 2: `void versionInfo() SUSUWU_NOEXCEPT(std::is_nothrow_constructible<U>::value); {versionInfo();}` is close to `{try {versionInfo();} catch(...) {if(std::is_nothrow_constructible<U>::value) {SUSUWU_UNREACHABLE;}}}` */
#else /* SUSUWU_CXX11 else */
#	define SUSUWU_NOEXCEPT /* No-op: "error: expected function body after function declarator" fix */
#	define SUSUWU_DEFAULT /* No-op */
#	define SUSUWU_DELETE /* No-op */
#	define SUSUWU_FINAL final /* No-op */
#	define SUSUWU_NULLPTR NULL /* fallback to C-style macro for `0`. */
#	define SUSUWU_OVERRIDE override /* No-op */
#endif /* SUSUWU_CXX11 else */

/* `SUSUWU_UNREACHABLE` is close to `SUSUWU_ASSUME(false)` */
#if !defined(NDEBUG_)
/* [https://stackoverflow.com/questions/2249282/c-c-portable-way-to-detect-debug-release] [https://stackoverflow.com/questions/2290509/debug-vs-ndebug] */
/* Debug: Promises unreachable, for static analysis */
#	define SUSUWU_UNREACHABLE assert(false && "UNREACHABLE") /* TODO: NOLINT(cert-dcl03-c,hicpp-static-assert,misc-static-assert): `static_assert` does not allow false, not even in unreachable code paths */
#else
#	include <version> /* __cpp_lib_unreachable */ /* [https://en.cppreference.com/w/cpp/feature_test] */
#	if defined(__cpp_lib_unreachable) && __cpp_lib_unreachable
/* Release: Promises executable can not reach this spot, for compiler which optimizes this. Warning: `SUSUWU_UNREACHABLE && UB (undefined behaviour)` */
#		include <utility> /* std::unreachable() */
#		define SUSUWU_UNREACHABLE std::unreachable()
#	elif (defined __GNUC__) && ((4 <= __GNUC__ && 4 < __GNUC_MINOR__) || 4 < __GNUC__) /* `~ $ g++` */
#		define SUSUWU_UNREACHABLE __builtin_unreachable()
#	else /* else (!def NDEBUG) && (!supports unreachable) */
#		define SUSUWU_UNREACHABLE /* No-op */
#	endif /* __cpp_lib_unreachable elif IS_GCC ...*/
#endif  /* #elif (!defined NDEBUG) ... #else */

#ifdef __cplusplus
namespace Susuwu { /* Is good practice to wrap all of a project's functions namespaces to prevent collisions. Macros above this can never be replaced with `constexpr` functions, or have `#include` */
#endif /* ifdef __cplusplus */
const int macrosTestsNoexcept() SUSUWU_NOEXCEPT;

#if defined(SUSUWU_C11) || defined(SUSUWU_CXX11)
#	define SUSUWU_STATIC_ASSERT(condition) static_assert(condition, #condition)
#elif true /* TODO: fix without this */ || defined(_MSC_VER) || defined __GNUC__ || __CLANG__
#	define SUSUWU_STATIC_ASSERT(condition) typedef char SUSUWU_GLUE(susuwuStaticAssertNum, __COUNTER__)[(condition) ? 1 : -1] /* almost mimics `static_assert(condition, #condition)`. `Num##__COUNTER__` fixes "redefinition of typeof". */
#else /* defined(_MSC_VER) || defined __GNUC__ || __CLANG__ else */
#	define SUSUWU_STATIC_ASSERT(condition) (void)sizeof(char[(condition) ? 1 : -1]) /* TODO: support global scope */
#endif /* defined(SUSUWU_C11) || defined(SUSUWU_CXX11) else */

/* `SUSUWU_ASSUME(X)` is close to `@pre @code X @endcode` or `[[expects: x]]` */
/* TODO: choose best of [various possible SUSUWU_ASSUME macros](https://stackoverflow.com/questions/44054078/how-to-guide-gcc-optimizations-based-on-assertions-without-runtime-cost) */
#ifndef NDEBUG
/* Debug: Promises `(true == (X))`, for static analysis */
#	define SUSUWU_ASSUME(X) SUSUWU_STATIC_ASSERT(X)
#elif (!defined USE_SUSUWU_ASSUME) || USE_SUSUWU_ASSUME /* Default: if(!NDEBUG) USE_SUSUWU_ASSUME=true; pass `-DUSE_SUSUWU_ASSUME=false` to disable this */
/* Release: Promises `(true == (X))`, for compiler which optimizes this. Warning: `if(!(X)) {UB (undefined behaviour)}` */
#	ifdef IS_MSVC
#		define SUSUWU_ASSUME(X) __assume(X)
#	elif __clang__ /* `~ $ clang++` */
#		define SUSUWU_ASSUME(X) __builtin_assume(X)
#	else /* (!def IS_MSVC) && (!def __clang__) */
#		define SUSUWU_ASSUME(X) ((X) ? static_cast<void>(0) : SUSUWU_UNREACHABLE)
#	endif /* !def IS_MSVC */
#else /* !def USE_SUSUWU_ASSUME */
#	define SUSUWU_ASSUME(X)
#endif /* !def USE_SUSUWU_ASSUME */

#if !defined(NDEBUG) && !defined(SUSUWU_SH_VERBOSE)
#	define SUSUWU_SH_VERBOSE true /* diagnostic logs to `cerr`/`stderr`; can enable on `--release` with `-DSUSUWU_SH_VERBOSE=true` */
#else
#	define SUSUWU_SH_VERBOSE false /* can disable on `--debug` with `-DSUSUWU_SH_VERBOSE=false` */
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
#if defined(SUSUWU_SH_USE_FUNC) && !defined(SUSUWU_C99) && !defined(SUSUWU_CXX11) /* no `__func__` support */
#	pragma message("[Warning: `-DSUSUWU_SH_FUNC = true` was undone (no `__func__`, due to `!defined(SUSUWU_C99) && !defined(SUSUWU_CXX11)`.]")
#	undef SUSUWU_SH_USE_FUNC /* affix `__func__ ":"` to `stderr`/`cerr` printout */
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
#	pragma message("[Info: `-DSUSUWU_SH_RUNTIME_COLORS` is TODO; https://github.com/SwuduSusuwu/SubStack/issues/17 to contribute]")
#endif /* #elif !defined(SUSUWU_POSIX) TODO */
#define SUSUWU_SH_ESC "\033" /* Escape */
#define SUSUWU_SH_CSI SUSUWU_SH_ESC "[" /* Control Sequence Introducer */
#define SUSUWU_SH_DEFAULT	SUSUWU_SH_CSI "0m"
#define SUSUWU_SH_BLACK	SUSUWU_SH_CSI "0;30m"
#define SUSUWU_SH_DARK_GRAY	SUSUWU_SH_CSI "1;30m"
#define SUSUWU_SH_RED	SUSUWU_SH_CSI "0;31m"
#define SUSUWU_SH_LIGHT_RED	SUSUWU_SH_CSI "1;31m"
#define SUSUWU_SH_GREEN	SUSUWU_SH_CSI "0;32m"
#define SUSUWU_SH_LIGHT_GREEN	SUSUWU_SH_CSI "1;32m"
#define SUSUWU_SH_BROWN	SUSUWU_SH_CSI "0;33m"
#define SUSUWU_SH_YELLOW	SUSUWU_SH_CSI "1;33m"
#define SUSUWU_SH_BLUE	SUSUWU_SH_CSI "0;34m"
#define SUSUWU_SH_LIGHT_BLUE	SUSUWU_SH_CSI "1;34m"
#define SUSUWU_SH_PURPLE	SUSUWU_SH_CSI "0;35m"
#define SUSUWU_SH_LIGHT_PURPLE	SUSUWU_SH_CSI "1;35m"
#define SUSUWU_SH_CYAN	SUSUWU_SH_CSI "0;36m"
#define SUSUWU_SH_LIGHT_CYAN	SUSUWU_SH_CSI "1;36m"
#define SUSUWU_SH_LIGHT_GRAY	SUSUWU_SH_CSI "0;37m"
#define SUSUWU_SH_WHITE	SUSUWU_SH_CSI "1;37m"
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

/* Use this to just print debug/notices to `--debug` builds (+ do conditional execution). */
#if SUSUWU_SH_VERBOSE
#	define SUSUWU_NOTICE(x) SUSUWU_PRINT(NOTICE, x)
#	define SUSUWU_DEBUG(x) SUSUWU_PRINT(DEBUG, x)
#	define SUSUWU_EXECUTEVERBOSE(x) x /* about side-effects; do not assume that `--debug` was used. `--release -DSUSUWU_SH_VERBOSE=true` will execute this. */
#else /* else SUSUWU_SH_VERBOSE */
#	define SUSUWU_NOTICE(x) (true)/* skip. */
#	define SUSUWU_DEBUG(x) (true)/* skip. */
#	define SUSUWU_EXECUTEVERBOSE(x) (true)/*skip*/ /* about side-effects; do not assume that just `--release` was used. `--debug -DSUSUWU_SH_VERBOSE=false` will skip. */
#endif /* else SUSUWU_SH_VERBOSE */

/* Use this to reduce print (NOTICE/DEBUG is conditional) + (unconditional) execute into single statement. */
#define SUSUWU_ERROR_EXECUTE(x) ((SUSUWU_ERROR(#x)), (x))
#define SUSUWU_WARNING_EXECUTE(x) ((SUSUWU_WARNING(#x)), (x))
#define SUSUWU_INFO_EXECUTE(x) ((SUSUWU_INFO(#x)), (x))
#define SUSUWU_SUCCESS_EXECUTE(x) ((SUSUWU_SUCCESS(#x)), (x))
#define SUSUWU_NOTICE_EXECUTE(x) ((SUSUWU_NOTICE(#x)), (x))
#define SUSUWU_DEBUG_EXECUTE(x) ((SUSUWU_DEBUG(#x)), (x))

/* Use this to reduce (conditional) print + (conditional) execute into single statement. */
#define SUSUWU_NOTICE_EXECUTEVERBOSE(x) ((SUSUWU_NOTICE(#x)), SUSUWU_EXECUTEVERBOSE(x))
#define SUSUWU_DEBUG_EXECUTEVERBOSE(x) ((SUSUWU_DEBUG(#x)), SUSUWU_EXECUTEVERBOSE(x))
```
`less` [cxx/Macros.cxx](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/Macros.cxx)
```
/* This is just unit tests. `Macros.hxx` has all which has actual use. */
#ifdef SUSUWU_CXX11
SUSUWU_CONSTEXPR static const bool MacrosCxx11NullptrTest(decltype(       NULL)    /* `int`    */) {return false;}
SUSUWU_CONSTEXPR static const bool MacrosCxx11NullptrTest(decltype(SUSUWU_NULLPTR) /* `void *` */) {return true;}
SUSUWU_STATIC_ASSERT(true  == MacrosCxx11NullptrTest(SUSUWU_NULLPTR)); /* Tests that `nullptr` is `void *` */
#endif /* def SUSUWU_CXX11 */
class MacrosCxx11InheritanceTest {
public:
	virtual ~MacrosCxx11InheritanceTest() SUSUWU_DEFAULT /* Notice: destructor has implementation; reimplementation is optional. */
	MacrosCxx11InheritanceTest() SUSUWU_DELETE /* Notice: deleted constructor is redundant for pure virtual class */
	virtual const bool PureVirtual() const = 0; /* Notice: this is pure virtual; subclasses must implement this. */
};
class MacrosCxx11InheritanceTestSubclass : MacrosCxx11InheritanceTest {
public: /* Notice: inherits default constructor */
	const bool PureVirtual() const SUSUWU_OVERRIDE {return true;}; /* Notice: `final` is not allowed since this has a subclass. `override` is optional. */
};
class MacrosCxx11InheritanceTestSubclass2 SUSUWU_FINAL /* Since destructor is final, cannot inherit from class. `clang++` thus warns, unless the whole class is `final`. */: MacrosCxx11InheritanceTestSubclass {
public: /* Notice: inherits `PureVirtual()`, which you can now set `final` */
	~MacrosCxx11InheritanceTestSubclass2() SUSUWU_OVERRIDE SUSUWU_FINAL {}; /* Notice: `override`/`final` is optional. */
};
static void macrosNoUniqueAddressTest() {
	typedef class Zero {} Zero;
	class SubClassWithBaseSubobject : public Zero {bool boo;};
	class SubClassWithMemberSubobject {bool boo; public: Zero zero;};
	class SubClassWithMemberSubobjectNoAddress {bool boo; public: SUSUWU_NO_UNIQUE_ADDRESS Zero zero;};
#ifdef SUSUWU_CXX11 /* this is true without C++11, but `std::is_empty` doesn't exist in C++98. */
	SUSUWU_STATIC_ASSERT(std::is_empty<Zero>::value);
#endif /* def SUSUWU_CXX11 */
	SUSUWU_STATIC_ASSERT(sizeof(bool) == sizeof(SubClassWithBaseSubobject));
	SUSUWU_STATIC_ASSERT(sizeof(bool) < sizeof(SubClassWithMemberSubobject));
#ifdef SUSUWU_CXX20 /* `[[no_unique_address]]` */
	SUSUWU_STATIC_ASSERT(sizeof(bool) == sizeof(SubClassWithMemberSubobjectNoAddress));
#else /* def SUSUWU_CXX20 else */
	SUSUWU_STATIC_ASSERT(sizeof(bool) < sizeof(SubClassWithMemberSubobjectNoAddress));
#endif /* def SUSUWU_CXX20 else */
}

SUSUWU_NORETURN static void macrosNoReturn();
SUSUWU_NORETURN static void macrosNoReturn() {exit(0);} /* NOLINT(concurrency-mt-unsafe): is unreachable code */
SUSUWU_CONSTEXPR static const bool macrosDeclarationAttributes() SUSUWU_EXPECTS(true) SUSUWU_ENSURES(true) SUSUWU_NOEXCEPT;
SUSUWU_CONSTEXPR static const bool macrosDeclarationAttributes() SUSUWU_EXPECTS(true) SUSUWU_ENSURES(true) SUSUWU_NOEXCEPT {
	return true;
}
SUSUWU_STATIC_ASSERT(true); /* test at file-scope, which `assert()` does not support */
const int macrosTestsNoexcept() SUSUWU_NOEXCEPT {
	SUSUWU_STATIC_ASSERT(true); /* test at function-scope, which some custom static asserts can't do */
	SUSUWU_ASSUME(true);
	macrosDeclarationAttributes();
	if(false) { /* NOLINT(readability-simplify-boolean-expr) */
		SUSUWU_UNREACHABLE;
		macrosNoReturn();
	}
	return 0;
}
```
`less` [cxx/ClassPortableExecutable.hxx](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/ClassPortableExecutable.hxx)
```
typedef std::string FilePath; /* TODO: `std::char_traits<unsigned char>`, `std::basic_string<unsigned char>("string literal")` */
typedef FilePath FileBytecode; /* Uses `std::string` for bytecode (versus `std::vector`) because:
 * "If you are going to use the data in a string like fashon then you should opt for std::string as using a std::vector may confuse subsequent maintainers. If on the other hand most of the data manipulation looks like plain maths or vector like then a std::vector is more appropriate." -- https://stackoverflow.com/a/1556294/24473928
*/
typedef FilePath FileHash; /* TODO: `std::unordered_set<std::basic_string<unsigned char>>` */
typedef class PortableExecutable : Object {
/* TODO: union of actual Portable Executable (Microsoft) + ELF (Linux) specifications */
public:
	const std::string getName() const SUSUWU_OVERRIDE {return "Susuwu::class PortableExecutable";}
	explicit PortableExecutable(FilePath path_ = "") : path(std::move(path_)) {}
	PortableExecutable(FilePath path_, FileBytecode bytecode_) : path(std::move(path_)), bytecode(std::move(bytecode_)) {} /* TODO: NOLINT(bugprone-easily-swappable-parameters) */
/*TODO: overload on typedefs which map to the same types:	PortableExecutable(const FilePath &path_, const std::string &hex_) : path(path_), hex(hex_) {} */
	const FilePath path; /* Suchas "C:\Program.exe" or "/usr/bin/library.so" */ /* NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members) */
	FileBytecode bytecode; /* compiled programs; bytecode */
	std::string hex; /* `hexdump(path)`, hexadecimal, for C string functions */
} PortableExecutable;
typedef class PortableExecutableBytecode : public PortableExecutable {
public:
	const std::string getName() const SUSUWU_OVERRIDE {return "Susuwu::class PortableExecutableBytecode";}
	explicit PortableExecutableBytecode(FilePath path_) : PortableExecutable(std::move(path_))  {std::ifstream input(path); if(input.good()) {bytecode = std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());}}
} PortableExecutableBytecode;
```
`less` [cxx/ClassSys.hxx](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/ClassSys.hxx)
```
/* Abstractions to do with: `sh` scripts (such as: exec*, sudo), sockets (TODO), filesystems (TODO) */
extern int classSysArgc;
extern const char **classSysArgs;
/* Called from main(), stores {argc, args} into {classSysArgc, classSysArgs}
 * Much simpler to use path from args[0] (versus https://stackoverflow.com/questions/1528298/get-path-of-executable/34109000#34109000)
 * @pre @code (0 < argc && SUSUWU_NULLPTR != args && SUSUWU_NULLPTR != args[0]
 * @post @code (0 < classSysArgc && SUSUWU_NULLPTR != classSysArgs && SUSUWU_NULLPTR != classSysArgs[0] */
const bool classSysInit(int argc, const char **args);

typedef decltype(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) ClassSysUSeconds;
inline const ClassSysUSeconds classSysUSecondClock() {
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

/* `std::array<char *>argv = argvS; argv += NULL; envp = envpS + NULL: pid_t pid = fork(); if(-1 != pid) {pid || (envpS.empty() ? execv(argv[0], &argv[0]) : execve(argv[0], &argv[0], &envp[0]));} return pid;`
 * @pre @code (-1 != access(argvS[0], X_OK) @endcode */
const pid_t execvesFork(/* const std::string &pathname, -- `execve` requires `&pathname == &argv[0]` */ const std::vector<std::string> &argvS = {}, const std::vector<std::string> &envpS = {}) SUSUWU_NOEXCEPT;
static const pid_t execvexFork(const std::string &toSh) SUSUWU_NOEXCEPT {return execvesFork({"/bin/sh", "-c", toSh});}
/* `pid_t pid = execvesFork(argvS, envpS); int status; waitpid(pid, &wstatus, 0); return wstatus;}`
 * @throw std::runtime_error(SUSUWU_ERRSTR(ERROR, "execves: -1 == execvesFork()"))
 * @pre @code (-1 != access(argvS[0], X_OK) @endcode */
const int execves(const std::vector<std::string> &argvS = {}, const std::vector<std::string> &envpS = {});
static const int execvex(const std::string &toSh) {return execves({"/bin/sh", "-c", toSh});}

/* #if SUSUWU_POSIX, `return (0 == geteuid());` #elif SUSUWU_WIN32 `return IsUserAnAdmin();` #endif `return false;` */
const bool classSysHasRoot();
/* #if SUSUWU_POSIX, `root ? (seteuid(0) : (seteuid(getuid() || getenv("SUDO_UID")), setuid(geteuid)); return classSysHasRoot();` #endif
 * Usage: classSysSetRoot(true); functionsWhichRequireRoot; classSysSetRoot(false); */
const bool classSysSetRoot(bool root); /* root ? (seteuid(0) : (seteuid(getuid() || atoi(getenv("SUDO_UID"))), setuid(geteuid)); return classSysHasRoot(); */

static const bool classSysGetConsoleInput() { return std::cin.good() && !std::cin.eof(); }
const bool classSysSetConsoleInput(bool input); /* Set to `false` for unit tests/background tasks (acts as if user pressed `<ctrl>+d`, thus input prompts will use default choices.) Returns `classSysGetConsoleInput();` */

template<class Os, class Str>
inline Os &classSysHexOs(Os &os, const Str &value) {
	const std::ios::fmtflags oldFlags = std::cout.flags();
	const char oldFill = os.fill();
	os << std::hex;
	os.fill('0');
	for(const unsigned char ch : value) {
		os << std::setw(2)/* `setw` is unset after each use */ << static_cast<int>(ch);
	}
	os.fill(oldFill);
	os.flags(oldFlags);
	return os;
}
template<class Str>
inline const Str classSysHexStr(const Str &value) {
	std::stringstream os;
	classSysHexOs(os, value);
	return os.str();
}
template<class Os, class List>
inline Os &classSysColoredParamOs(Os &os, const List &argvS, const bool parenthesis/* {...} */ = true) {
	if(parenthesis) {
		os << '{';
	}
	for(const auto &it: argvS) {
		if(&it != &*argvS.cbegin()) {
			os << ", ";
		}
		os << SUSUWU_SH_GREEN "\"";
		os << it;
		os << "\"" SUSUWU_SH_DEFAULT;
	}
	if(parenthesis) {
		os << '}';
	}
	return os;
}
template<class List>
inline const typename List::value_type classSysColoredParamStr(const List &argvS, const bool parenthesis/* {...} */ = true) {
	typename List::value_type str = (parenthesis ? "{" : "");
	for(const auto &it: argvS) {
		if(&it != &*argvS.cbegin()) {
			str += ", ";
		}
		str += SUSUWU_SH_GREEN "\"";
		str += it;
		str += "\"" SUSUWU_SH_DEFAULT;
	}
	if(parenthesis) {
		str += '}';
	}
	return str;
}

template<typename Func, typename... Args>
auto templateCatchAll(Func func, const std::string &funcName, Args... args) -> const decltype(func(args...)) {
	try {
		return func(args...);
	} catch (const std::exception &ex) {
		SUSUWU_ERROR(funcName + " {throw std::exception(\"" + ex.what() + "\");}");
		return decltype(func(args...))(); /* `func(args...)`'s default return value; if `int func(args...)`, `return 0;`. If `bool func(args...)`, `return false;` */
	}
}

/* @throw std::runtime_error */
const bool classSysTests();
static const bool classSysTestsNoexcept() SUSUWU_NOEXCEPT {return templateCatchAll(classSysTests, "classSysTests()");}
```
`less` [cxx/ClassSys.cxx](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/ClassSys.cxx)
```
int classSysArgc = 0;
const char **classSysArgs = {SUSUWU_NULLPTR};
const bool classSysInit(int argc, const char **args) {
	classSysArgc = argc;
	if(0 < argc) {
		classSysArgs = args;
		assert(SUSUWU_NULLPTR != args);
		assert(SUSUWU_NULLPTR != args[0]); /* `clangtidy` off: NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic) */
		return true;
	}
	return false;
}

const pid_t execvesFork(const std::vector<std::string> &argvS, const std::vector<std::string> &envpS) SUSUWU_NOEXCEPT {
#ifdef SUSUWU_POSIX
	const pid_t pid = fork();
	if(0 != pid) {
		if(-1 == pid) {
			SUSUWU_ERROR("execvesFork(): {(-1 == pid)}, errno=" + std::to_string(errno));
		}
		return pid;
	} /* if 0, is fork */
	const std::vector<std::string> argvSmutable = {argvS.cbegin(), argvS.cend()};
	std::vector<char *> argv;
	argv.reserve(argvSmutable.size());
	//for(auto x : argvSmutable) { /* with `fsanitize=address` this triggers "stack-use-after-scope" */
	for(const auto &x: argvSmutable /* auto x = argvSmutable.cbegin(); argvSmutable.cend() != x; ++x */) {
		argv.push_back(const_cast<char *>(x.c_str()));
	}
	argv.push_back(SUSUWU_NULLPTR);
	if(envpS.empty()) { /* Reuse LD_PRELOAD to fix https://github.com/termux-play-store/termux-issues/issues/24 */
		execv(argv[0], &argv[0]); /* SUSUWU_NORETURN */
	} else {
		std::vector<std::string> envpSmutable = {envpS.cbegin(), envpS.cend()};
		std::vector<char *> envp;
		envp.reserve(envpSmutable.size());
		for(const auto &x: envpSmutable) {
			envp.push_back(const_cast<char *>(x.c_str()));
		}
		envp.push_back(SUSUWU_NULLPTR);
		execve(argv[0], &argv[0], &envp[0]); /* SUSUWU_NORETURN */
	}
	exit(EXIT_FAILURE); /* execv*() is `SUSUWU_NORETURN`. NOLINT(concurrency-mt-unsafe) */
#else /* ndef SUSUWU_POSIX */
	undef ERROR /* undo `shlobj.h`'s `#define ERROR 0` */
	SUSUWU_ERROR("execvesFork: {#ifndef SUSUWU_POSIX /* TODO: convert to win32 */}");
    return -1;
#endif /* ndef SUSUWU_POSIX */
}
const int execves(const std::vector<std::string> &argvS, const std::vector<std::string> &envpS) {
#ifdef SUSUWU_POSIX
	const pid_t pid = execvesFork(argvS, envpS);
	int wstatus = 0;
	if(-1 == pid) {
		throw std::runtime_error(SUSUWU_ERRSTR(ERROR, "execves: -1 == execvesFork()"));
	}
	waitpid(pid, &wstatus, 0);
	if(WIFEXITED(wstatus) && 0 != WEXITSTATUS(wstatus)) {
		SUSUWU_NOTICE("execves(" + classSysColoredParamStr(argvS) + ", " + classSysColoredParamStr(envpS) + ") {if(WIFEXITED(wstatus) && 0 != WEXITSTATUS(wstatus)) {SUSUWU_NOTICE(... \"WEXITSTATUS(wstatus) is " SUSUWU_SH_PURPLE + std::to_string(WEXITSTATUS(wstatus)) + SUSUWU_SH_DEFAULT "\" ...);}}");
	} else if(WIFSIGNALED(wstatus)) {
		SUSUWU_NOTICE("execves(" + classSysColoredParamStr(argvS) + ", " + classSysColoredParamStr(envpS) + ") {if(WIFSIGNALED(wstatus)) {SUSUWU_NOTICE(... \"WTERMSIG(wstatus) is " SUSUWU_SH_PURPLE + std::to_string(WTERMSIG(wstatus)) + SUSUWU_SH_DEFAULT "\" ...);}}");
	}
	return wstatus;
#else /* ndef SUSUWU_POSIX */
	throw std::runtime_error(SUSUWU_ERRSTR(ERROR, "execves: {#ifndef SUSUWU_POSIX /* TODO: convert to win32 */}"));
#endif /* ndef SUSUWU_POSIX */
}

const bool classSysHasRoot() {
#ifdef SUSUWU_POSIX
	return (0 == geteuid());
#elif defined SUSUWU_WIN32
	return IsUserAnAdmin();
#else
	SUSUWU_WARNING("classSysHasRoot(bool) {#if !(defined SUSUWU_POSIX || defined SUSUWU_WIN32) /* TODO */}");
	return false;
#endif /* def SUSUWU_POSIX or def SUSUWU_WIN32 */
}
const bool classSysSetRoot(bool root) {
	if(classSysHasRoot() == root) {
		return root;
	}
#ifdef SUSUWU_POSIX
	if(root) {
		if(-1 == seteuid(0)) {
			SUSUWU_WARNING("classSysSetRoot(true) {(-1 == seteuid(0)) /* stuck as user, perhaps is not setuid executable */}");
		}
#if 0
#	ifdef __APPLE__ //TODO: https://stackoverflow.com/questions/2483755/how-to-programmatically-gain-root-privileges/35316538#35316538 says you must execute new processes to do this
#	else //TODO: https://stackoverflow.com/questions/34723861/calling-a-c-function-with-root-privileges-without-executing-the-whole-program/70149223#70149223 https://stackoverflow.com/questions/70615937/how-to-run-a-command-as-root-with-c-or-c-with-no-pam-in-linux-with-password-au https://stackoverflow.com/questions/2483755/how-to-programmatically-gain-root-privileges/2483789#2483789 says you must spawn new processes to do this
		/* TODO: polkit? Until this is finished, you must use chmod (to give setuid to executable), or execute new processes (with `sudo`/`su`) if you wish to use firewall/antivirus (which require root) */
#	endif /* __APPLE__ else */
#endif /* 0 */
	} else {
#	if 0 && defined LINUX // TODO: pam_loginuid.so(8) // https://stackoverflow.com/questions/10272784/how-do-i-get-the-users-real-uid-if-the-program-is-run-with-sudo/10272881#10272881
		uid_t sudoUid = audit_getloginuid();
#	else /* !def linux */
		uid_t sudoUid = getuid();
		if(0 == sudoUid) {
			char *sudoUidStr = getenv("SUDO_UID") /* NOLINT(concurrency-mt-unsafe) */, *sudoUidStrIt = SUSUWU_NULLPTR;
			if(SUSUWU_NULLPTR == sudoUidStr) {
				SUSUWU_WARNING("classSysSetRoot(false) {(SUSUWU_NULLPTR == getenv(\"SUDO_UID\")) /* stuck as root */}");
				return true;
			} else {
				sudoUid = static_cast<uid_t>(strtol(sudoUidStr, &sudoUidStrIt, 10));
				if(sudoUidStr == sudoUidStrIt || -1 == setuid(sudoUid)) { /* prevent reescalation to root */
					SUSUWU_WARNING("classSysSetRoot(false) {(-1 == setuid(sudoUid)) /* can't prevent reescalation to root */}");
				}
			}
		}
#	endif /* !def LINUX */
		if(0 == sudoUid) {
			SUSUWU_WARNING("classSysSetRoot(false) {(0 == sudoUid) /* stuck as root */}");
		} else if(-1 == seteuid(sudoUid)) {
			SUSUWU_WARNING("classSysSetRoot(false) {(-1 == seteuid(sudoUid)) /* stuck as root */}");
		}
	}
/* #elif defined SUSUWU_WIN32 */ //TODO: https://stackoverflow.com/questions/6418791/requesting-administrator-privileges-at-run-time says you must spawn new processes to do this
#else
	SUSUWU_WARNING("classSysSetRoot(bool) {#ifndef SUSUWU_POSIX /* TODO */}");
#endif /* SUSUWU_POSIX */
	return classSysHasRoot();
}

const bool classSysSetConsoleInput(bool input) {
	input ? std::cin.clear(std::ios::goodbit) : std::cin.setstate(std::ios::eofbit);
	return classSysGetConsoleInput();
}

static void classSysHexTests(const std::string &value) {
	const size_t ss = classSysHexStr(value).size();
	std::stringstream os;
	if(2 != ss) {
		throw std::runtime_error(SUSUWU_ERRSTR(ERROR, std::to_string(value.size()) + " == value.size(); " + std::to_string(ss) + " == classSysHexStr(value).size();"));
	}
	classSysHexOs(os, value);
	if(2 != os.str().size()) {
		throw std::runtime_error(SUSUWU_ERRSTR(ERROR, "classSysHexOs(os, value); " + std::to_string(value.size()) + " == value.size(); " + std::to_string(os.str().size()) + " == os.str().size();"));
#ifndef SUSUWU_POSIX
#	define ERROR 0 /* redo `shlobj.h`'s `#define ERROR 0` */
#endif /* ndef SUSUWU_POSIX */
	}
}
const bool classSysTests() {
	bool retval = true; /* TODO: choose all errors throw exceptions, or choose all errors return error values. Most of the other unit tests use exceptions, but `echo` is the best test for `execves`/`execvex`. */
	classSysHexTests(std::string({0}) /* test that char == 0x00 produces 2 hexits */);
	classSysHexTests("\010" /* test that char <= 0x10 produces 2 hexits */);
	classSysHexTests("\022" /* test that char >= 0x10 produces 2 hexits */);
	std::cout << "	execves(): " << std::flush;
	(EXIT_SUCCESS == execves({"/bin/echo", "pass"})) || (retval = false) || (std::cout << "error" << std::endl);
	std::cout << "	execvex(): " << std::flush;
	(EXIT_SUCCESS == execvex("/bin/echo pass")) || (retval = false) || (std::cout << "error" << std::endl);
	return retval;
}
```
`less` [cxx/ClassSha2.hxx](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/ClassSha2.hxx)
```
/* const */ FileHash /* 128 bits, not null-terminated */ classSha1(const FileBytecode &bytecode);
/* const */ FileHash /* 256 bits, not null-terminated */ classSha256(const FileBytecode &bytecode);
/* const */ FileHash /* 512 bits, not null-terminated */ classSha512(const FileBytecode &bytecode);
typedef FileHash (*ClassSha2)(const FileBytecode &bytecode);
extern ClassSha2 classSha2/* = classSha256 */; /* To compress, apps can execute `classSha2 = classSha1;`. To double hash sizes, execute `classSha2 = classSha512;`. (Notice: this does not recompute hashes which exist) */
const bool classSha2Tests();
const bool classSha2TestsNoexcept() SUSUWU_NOEXCEPT;
```
`less` [cxx/ClassSha2.cxx](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/ClassSha2.cxx)
```
/* Uses https://www.rfc-editor.org/rfc/rfc6234#section-8.2.2 */
ClassSha2 classSha2 = classSha256;
/* const */ FileHash /* 128 bits, not null-terminated */ classSha1(const FileBytecode &bytecode) {
	FileHash result;
	SHA1Context context;
	result.resize(SHA1HashSize);
	SHA1Reset(&context); /* If `undefined symbol: SHA1Reset`, affix `classSha1.o` to `${LD_FLAGS}` */
	SHA1Input(&context, reinterpret_cast<const unsigned char *>(&bytecode[0]), bytecode.size());
	SHA1Result(&context, reinterpret_cast<unsigned char *>(&result[0]));
	return result;
}
/* const */ FileHash /* 256 bits, not null-terminated */ classSha256(const FileBytecode &bytecode) {
	FileHash result;
	SHA256Context context;
	result.resize(SHA256HashSize);
	SHA256Reset(&context); /* If `undefined symbol: SHA256Reset`, affix `classSha224-256.o` to `${LD_FLAGS}` */
	SHA256Input(&context, reinterpret_cast<const unsigned char *>(&bytecode[0]), bytecode.size());
	SHA256Result(&context, reinterpret_cast<unsigned char *>(&result[0]));
	return result;
}
/* const */ FileHash /* 512 bits, not null-terminated */ classSha512(const FileBytecode &bytecode) {
	FileHash result;
	SHA512Context context;
	result.resize(SHA512HashSize);
	SHA512Reset(&context); /* If `undefined symbol: SHA512Reset`, affix `sha384-512.o` to `${LD_FLAGS}` */
	SHA512Input(&context, reinterpret_cast<const unsigned char *>(&bytecode[0]), bytecode.size());
	SHA512Result(&context, reinterpret_cast<unsigned char *>(&result[0]));
	return result;
}

const bool classSha2Tests() { /* is just to test glue code (which wraps rfc6234). Use `../c/rfc6234/shatest.c` to test rfc6234. */
	const char nulls[65536 /* 65536 == 2^16 == 64kb */] = {0};
	std::string nullStr(nulls, &nulls[65536]);
	const ClassSysUSeconds tsDrift = classSysUSecondClock(), ts2Drift = classSysUSecondClock() - tsDrift, ts = classSysUSecondClock();
	const FileHash hash = classSha2(nullStr);
	const ClassSysUSeconds ts2 = classSysUSecondClock() - ts2Drift;
	const std::string hashStrCompute = "0x" + classSysHexStr(hash);
	const std::string hashStrTrue = "0xde2f256064a0af797747c2b97505dc0b9f3df0de4f489eac731c23ae9ca9cc31";
	if(ts == ts2) {
		SUSUWU_WARNING("0 ms (0 μs) to compute `classSha2(std::string(nulls, &nulls[65536])) == " + hashStrCompute + "` = inf mbps");
	} else {
		SUSUWU_INFO(std::to_string((ts2 - ts) / 1000) + " ms (" + std::to_string(ts2 - ts) + " μs) to compute `classSha2(std::string(nulls, &nulls[65536])) == " + hashStrCompute + "` = " + std::to_string(float(65536) * CHAR_BIT /* to bits */ / (ts2 - ts) /* to bpμs */ * 1000000 /* to bps */ / (1 << 20) /* to mbps */) + "mbps");
	}
	SUSUWU_NOTICE("This `classSha2()` is from `./build.sh --debug`: `./build.sh --release` has 2x this throughput");
	if(0 == hash.size()) {
		throw std::runtime_error(SUSUWU_ERRSTR(ERROR, "`0 == classSha2(std::string()).size();"));
	} else if(hashStrTrue.size() != hashStrCompute.size() && classSha256 == classSha2) {
		throw std::runtime_error(SUSUWU_ERRSTR(ERROR, "`classSha2 = classSha256;`, but `(" + std::to_string(hash.size()) + " == classSha2(std::string()).size())`"));
	} else if(hashStrTrue.size() != hashStrCompute.size()) {
		SUSUWU_INFO("`(classSha256 != classSha2)`, `(" + std::to_string(hash.size()) + " == classSha2(std::string()).size())`");
	} else if(hashStrTrue != hashStrCompute) {
		throw std::runtime_error(SUSUWU_ERRSTR(ERROR, "classSha2(char nulls[65535] = {0}) did not compute " + hashStrTrue));
	}
	return true;
}
const bool classSha2TestsNoexcept() SUSUWU_NOEXCEPT {return templateCatchAll(classSha2Tests, "classSha2Tests()");}
```
`less` [cxx/ClassResultList.hxx](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/ClassResultList.hxx)
```
typedef FileHash ResultListHash;
typedef FileBytecode ResultListBytecode; /* Should have structure of FileBytecode, but is not just for files, can use for UTF8/webpages, so have a new type for this */
typedef FilePath ResultListSignature; /* TODO: `typedef ResultListBytecode ResultListSignature; ResultListSignature("string literal");` */
typedef ptrdiff_t BytecodeOffset; /* all tests of `ResultListBytecode` should return `{BytecodeOffset, X}` (with the most common `X` as `ResultListHash` or `ResultListSignature`). `offset = -1` if no match */
typedef struct ResultList : Object { /* Lists of {metadata, executables (or pages)} */
	const std::string getName() const SUSUWU_OVERRIDE {return "Susuwu::struct ResultList";}
	typedef std::unordered_set<ResultListHash> Hashes;
	Hashes hashes; /* Checksums of executables (or pages); to avoid duplicates, plus to do constant ("O(1)") test for which executables (or pages) exists */
	typedef std::vector<ResultListSignature> Signatures;
	Signatures signatures; /* Smallest substrings (or regexes, or Universal Resource Locators) which can identify `bytecodes`; has uses close to `hashes`, but can match if executables (or pages) have small differences */
	typedef std::vector<ResultListBytecode> Bytecodes;
	Bytecodes bytecodes; /* Whole executables (for `VirusAnalysis`) or webpages (for `AssistantCns`); huge disk usage, just load this for signature synthesis (or CNS backpropagation). */
} ResultList;

const bool classResultListTests(); /* TODO: test most of `ClassResultList*` */
static const bool classResultListTestsNoexcept() SUSUWU_NOEXCEPT {return templateCatchAll(classResultListTests, "classResultListTests()");}

template<class List>
const size_t listMaxSize(const List &list) {
#if SUSUWU_PREFER_CSTR
	size_t max = 0;
	for(auto it = &list[0]; list.cend() != it; ++it) { const size_t temp = strlen(*it); if(temp > max) {max = temp;}}
	return max; /* WARNING! `strlen()` just does UTF8-strings/hex-strings; if binary, must use `it->size()` */
#else /* else !SUSUWU_PREFER_CSTR */
	auto it = std::max_element(list.cbegin(), list.cend(), [](const typename List::const_iterator::value_type &s, const typename List::const_iterator::value_type &x) { return s.size() < x.size(); });
	return it->size();
#endif /* SUSUWU_PREFER_CSTR else */
}

template<class List, class Os>
void listDumpTo(const List &list, Os &os, const bool index, const bool whitespace, const bool pascalValues) {
	size_t index_ = 0;
	os << '{';
	for(const auto &value : list) {
		if(0 != index_) {
			os << ',';
		}
		if(whitespace) {
			os << std::endl << '\t';
		}
		if(index) {
			os << index_;
			whitespace ? (os << " = ") : (os << '=');
		}
		if(pascalValues) {
			os << value.size() << ':' /* TODO: replace "%Dec:" with "%Bin" */ << value;
		} else {
			os << "0x";
			classSysHexOs(os, value);
		}
		++index_;
	}
	if(whitespace) {
		os << "\n};" << std::endl;
	} else {
		os << "};";
	}
} /* view `ClassResultList.cxx`:`classResultListTests()` for examples of output from `listDumpTo()`+`resultListDumpTo()`. TODO: +`listLoadFrom()`/+`resultListLoadFrom()` */
template<class List, class Os>
void resultListDumpTo(const List &list, Os &os, const bool index, const bool whitespace, const bool pascalValues) {
	os << "list.hashes" << (whitespace ? " = " : "=");
	listDumpTo(list.hashes, os, index, whitespace, pascalValues);
	os << "list.signatures" << (whitespace ? " = " : "=");
	listDumpTo(list.signatures, os, index, whitespace, pascalValues);
	os << "list.bytecodes" << (whitespace ? " = " : "=");
	listDumpTo(list.bytecodes, os, index, whitespace, pascalValues);
}

template<class List, class List2>
/*	@pre @code !(list.empty() || hashes.full()) @endcode
 *	@post @code !hashes.empty() @endcode */
void listToHashes(const List &list /* ResultList::bytecodes or ResultList::hex*/, List2 &hashes /* ResultList::hashess */) {
	for(const auto &value : list) {
		hashes.insert(classSha2(value));
	}
}
/* Usage: if `ResultList` was not produced with `.hashes` */
static void resultListProduceHashes(ResultList &resultList) {
	listToHashes(resultList.bytecodes, resultList.hashes);
}

/* @pre @code std::is_sorted(list.cbegin(), list.cend()) && std::is_sorted(list2.cbegin(), list2.cend()) @endcode */
template<class List>
const List listIntersections(const List &list, const List &list2) {
	List intersections;
	std::set_intersection(list.cbegin(), list.cend(), list2.cbegin(), list2.cend(), std::back_inserter(intersections));
	return intersections;
}
template<class List>
const bool listsIntersect(const List &list, const List &list2) {
	return listIntersections(list, list2).size();
}

template<class List>
/* return `list`'s `const_iterator` to first instance of `value`, or `list.cend()` (if not found) */
auto listFindValue(const List &list, const typename List::value_type &value) -> decltype(std::find(list.cbegin(), list.cend(), value)) {
//const class List::const_iterator listFindValue(const List &list, const typename List::value_type &value) {
	return std::find(list.cbegin(), list.cend(), value);
}
template<class List>
const bool listHasValue(const List &list, const typename List::value_type &value) {
	return list.cend() != listFindValue(list, value);
}

template<class List>
/* return `list`'s `const_iterator` to first instance of `std::string(itBegin, itEndSubstr)`, or default iterator (if not found)
 * @pre @code itBegin < itEnd @endcode */
const typename List::value_type::const_iterator listFindSubstr(const List &list, typename List::value_type::const_iterator itBegin, typename List::value_type::const_iterator itEnd) {
#pragma unroll
	for(const auto &value : list) {
		auto result = std::search(value.cbegin(), value.cend(), itBegin, itEnd, [](char chValue, char chIt) { return chValue == chIt; });
		if(value.cend() != result) {
			return result;
		}
	}
	return typename List::value_type::const_iterator(); /* Equates to "Not found" */
}
template<class List>
/* @pre @code itBegin < itEnd @endcode */
const bool listHasSubstr(const List &list, typename List::value_type::const_iterator itBegin, typename List::value_type::const_iterator itEnd) {
	return typename List::value_type::const_iterator() != listFindSubstr(list, itBegin, itEnd);
}
template<class List>
/* Returns shortest substr from `value`, which is not found in `list`
 * Usage: `resultList.signatures.push_back({listProduceSignature(resultList.bytecodes, bytecode));` */
const std::tuple<typename List::value_type::const_iterator, typename List::value_type::const_iterator> listProduceSignature(const List &list, const typename List::value_type &value) {
	ptrdiff_t smallest = value.size();
	auto itBegin = value.cbegin(), itEnd = value.cend();
	for(auto first = itBegin; value.cend() != first; ++first) {
		for(auto last = value.cend(); first != last; --last) {
			if((last - first) < smallest) {
				if(listHasSubstr(list, first, last)) {
					break;
				}
				smallest = last - first;
				itBegin = first, itEnd = last;
			}
		}
	} /* Incremental `for()` loops, is O(n^2 * m) complex formula to produce signatures; should use binary searches, or look for the Standard Template Lib (or Boost) function which optimizes this. */
	return {itBegin, itEnd};
}
typedef struct ResultListSignatureMatch {
	BytecodeOffset fileOffset;
	ResultListSignature signature;
} ResultListSignatureMatch;
template<class List>
/* Usage: `auto it = listFindSignatureOfValue(resultList.signatures, value)); if(it) {std::cout << "value has resultList.signatures[" << tohex(match.signature) << "]";}` */
ResultListSignatureMatch listFindSignatureOfValue(const List &list, const typename List::value_type &value) {
	for(const auto &signature : list) {
#if SUSUWU_PREFER_CSTR
		auto it = memmem(&value[0], strlen(&value[0]), &signature[0], strlen(&signature[0]));
		if(NULL != it) {
#else /* !SUSUWU_PREFER_CSTR */
		auto it = std::search(value.cbegin(), value.cend(), signature.cbegin(), signature.cend(), [](char ch1, char ch2) { return ch1 == ch2; });
		if(signature.cend() != it) {
#endif /* !SUSUWU_PREFER_CSTR */
			return {it - value.cbegin(), signature};
		}
	}
	return {-1, ""};
}
template<class List>
/* Usage: `if(listHasSignatureOfValue(resultList.signatures, value)) {std::cout << "value has signature from ResultList.signatures";}` */
const bool listHasSignatureOfValue(const List &list, const typename List::value_type &value) {
	return -1 != listFindSignatureOfValue(list, value).fileOffset;
}

template<class S>
const std::vector<S> explodeToList(const S &s, const S &token) {
	std::vector<S> list;
	for(auto x = s.cbegin(); s.cend() != x; ) {
		auto it = std::search(x, s.cend(), token.cbegin(), token.cend(), [](char ch1, char ch2) { return ch1 == ch2; });
		list.push_back(S(x, it));
		if(s.cend() == x) {
			return list;
		}
		x = it;
	}
	return list;
}
```
`less` [cxx/ClassResultList.cxx](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/ClassResultList.cxx)
```
static void classResultListDumpToTest(const ResultList &resultList, bool index, bool whitespace, bool pascalValues, const std::string &expectedValue) { /* NOLINT(misc-use-anonymous-namespace): have to call */
	std::stringstream os;
	resultListDumpTo(resultList, os, index, whitespace, pascalValues);
	if(expectedValue != os.str()) {
		throw std::runtime_error(SUSUWU_ERRSTR(ERROR, std::string("classResultListDumpToTest(resultList, os, ") + (index ? "true" : "false") + ", " + (whitespace ? "true" : "false") + ", " + (pascalValues ? "true" : "false") + "); \"" SUSUWU_SH_RED + os.str() + SUSUWU_SH_WHITE "\" == os.str(); \"" SUSUWU_SH_GREEN + expectedValue + SUSUWU_SH_WHITE "\" != os.str();")); /* TODO: standard macros for error/success colors, plus `SUSUWU_ERR` default color */
	}
}
const bool classResultListTests() {
	ResultList resultList;
	resultList.hashes.insert(ResultListHash({'\x32'})); /* `.hashes` is `std::unordered_set`, thus test just 1 value. */
	resultList.signatures = {"1", "2"};
	resultList.bytecodes = {"01", "02"};
	classResultListDumpToTest(resultList, false, false, false, "list.hashes={0x32};list.signatures={0x31,0x32};list.bytecodes={0x3031,0x3032};");
	classResultListDumpToTest(resultList, true, true, false, "list.hashes = {\n\t0 = 0x32\n};\nlist.signatures = {\n\t0 = 0x31,\n\t1 = 0x32\n};\nlist.bytecodes = {\n\t0 = 0x3031,\n\t1 = 0x3032\n};\n");
	classResultListDumpToTest(resultList, false, false, true, "list.hashes={1:2};list.signatures={1:1,1:2};list.bytecodes={2:01,2:02};");
	return true;
}
```
`less` [cxx/ClassCns.hxx](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/ClassCns.hxx)
```
typedef enum CnsMode : char {
	cnsModeBool, cnsModeChar, cnsModeInt, cnsModeUint, cnsModeFloat, cnsModeDouble,
	cnsModeVectorBool, cnsModeVectorChar, cnsModeVectorInt, cnsModeVectorUint, cnsModeVectorFloat, cnsModeVectorDouble,
#if defined(SUSUWU_CXX17) && defined(SUSUWU_PREFER_STRING_VIEW /* TODO */)
	cnsModeString = cnsModeVectorChar /* std::string == std::vector<char> */
#else /* else !def SUSUWU_CXX17 */
/* https://stackoverflow.com/questions/5115166/how-to-construct-a-stdstring-from-a-stdvectorchar */
	cnsModeString
#endif /* def SUSUWU_CXX17 else */
} CnsMode;

typedef class Cns : Object {
public:
	const std::string getName() const SUSUWU_OVERRIDE {return "Susuwu::class Cns";}
	~Cns() SUSUWU_OVERRIDE SUSUWU_DEFAULT
	Cns() SUSUWU_DEFAULT /* Default constructor */
	Cns(const Cns &) SUSUWU_DEFAULT /* Copy constructor */
	Cns& operator=(const Cns &) SUSUWU_DEFAULT /* Copy assignment */
	Cns(Cns&&) SUSUWU_NOEXCEPT SUSUWU_DEFAULT /* Move constructor */
	Cns& operator=(Cns &&) SUSUWU_NOEXCEPT SUSUWU_DEFAULT /* Move assignment */
	const bool hasImplementation() const SUSUWU_OVERRIDE {return typeid(Cns) != typeid(this);}
	const bool isInitialized() const SUSUWU_OVERRIDE {return initialized;}
	virtual void setInitialized(const bool is) {initialized = is;}
	virtual void setInputMode(CnsMode x) {inputMode = x;}
	virtual void setOutputMode(CnsMode x) {outputMode = x;}
	virtual void setInputNeurons(size_t x) {inputNeurons = x;}
	virtual void setOutputNeurons(size_t x) {outputNeurons = x;}
	virtual void setLayersOfNeurons(size_t x) {layersOfNeurons = x;}
	virtual void setNeuronsPerLayer(size_t x) {neuronsPerLayer = x;}
	/* @throw bad_alloc
	 * @pre @code hasImplementation() @endcode
	 * @post @code isInitialized() @endcode */
	// template<Intput, Output> virtual void setupSynapses(std::vector<std::tuple<Input, Output>> inputsToOutputs); /* C++ does not support templates of virtual functions ( https://stackoverflow.com/a/78440416/24473928 ) */
	/* @pre @code isInitialized() @endcode */
	// template<Input, Output> virtual const Output process(Input input);
#define templateWorkaround(INPUT_MODE, INPUT_TYPEDEF) /* NOLINT(cppcoreguidelines-macro-usage): can't have templates virtual */ \
	virtual void setupSynapses(const std::vector<std::tuple<INPUT_TYPEDEF, bool>> &inputsToOutputs) {inputMode = (INPUT_MODE); outputMode = cnsModeBool;}\
	virtual void setupSynapses(const std::vector<std::tuple<INPUT_TYPEDEF, char>> &inputsToOutputs) {inputMode = (INPUT_MODE); outputMode = cnsModeChar;}\
	virtual void setupSynapses(const std::vector<std::tuple<INPUT_TYPEDEF, int>> &inputsToOutputs) {inputMode = (INPUT_MODE); outputMode = cnsModeInt;}\
	virtual void setupSynapses(const std::vector<std::tuple<INPUT_TYPEDEF, unsigned int>> &inputsToOutputs) {inputMode = (INPUT_MODE); outputMode = cnsModeUint;}\
	virtual void setupSynapses(const std::vector<std::tuple<INPUT_TYPEDEF, float>> &inputsToOutputs) {inputMode = (INPUT_MODE); outputMode = cnsModeFloat;}\
	virtual void setupSynapses(const std::vector<std::tuple<INPUT_TYPEDEF, double>> &inputsToOutputs) {inputMode = (INPUT_MODE); outputMode = cnsModeDouble;}\
	virtual void setupSynapses(const std::vector<std::tuple<INPUT_TYPEDEF, std::vector<bool>>> &inputsToOutputs) {inputMode = (INPUT_MODE); outputMode = cnsModeVectorBool;}\
	virtual void setupSynapses(const std::vector<std::tuple<INPUT_TYPEDEF, std::vector<char>>> &inputsToOutputs) {inputMode = (INPUT_MODE); outputMode = cnsModeVectorChar;}\
	virtual void setupSynapses(const std::vector<std::tuple<INPUT_TYPEDEF, std::vector<int>>> &inputsToOutputs) {inputMode = (INPUT_MODE); outputMode = cnsModeVectorInt;}\
	virtual void setupSynapses(const std::vector<std::tuple<INPUT_TYPEDEF, std::vector<unsigned int>>> &inputsToOutputs) {inputMode = (INPUT_MODE); outputMode = cnsModeVectorUint;}\
	virtual void setupSynapses(const std::vector<std::tuple<INPUT_TYPEDEF, std::vector<float>>> &inputsToOutputs) {inputMode = (INPUT_MODE); outputMode = cnsModeVectorFloat;}\
	virtual void setupSynapses(const std::vector<std::tuple<INPUT_TYPEDEF, std::vector<double>>> &inputsToOutputs) {inputMode = (INPUT_MODE); outputMode = cnsModeVectorDouble;}\
	virtual void setupSynapses(const std::vector<std::tuple<INPUT_TYPEDEF, std::string>> &inputsToOutputs) {inputMode = (INPUT_MODE); outputMode = cnsModeString;}\
	virtual const bool processToBool(const INPUT_TYPEDEF &input) const {assert((INPUT_MODE) == inputMode && cnsModeBool == outputMode); return 0;}\
	virtual const char processToChar(const INPUT_TYPEDEF &input) const {assert((INPUT_MODE) == inputMode && cnsModeChar == outputMode); return 0;}\
	virtual const int processToInt(const INPUT_TYPEDEF &input) const {assert((INPUT_MODE) == inputMode && cnsModeInt == outputMode); return 0;}\
	virtual const unsigned int processToUint(const INPUT_TYPEDEF &input) const {assert((INPUT_MODE) == inputMode && cnsModeUint == outputMode); return 0;}\
	virtual const float processToFloat(const INPUT_TYPEDEF &input) const {assert((INPUT_MODE) == inputMode && cnsModeFloat == outputMode); return 0;}\
	virtual const double processToDouble(const INPUT_TYPEDEF &input) const {assert((INPUT_MODE) == inputMode && cnsModeDouble == outputMode); return 0;}\
	virtual const std::vector<bool> processToVectorBool(const INPUT_TYPEDEF &input) const {assert((INPUT_MODE) == inputMode && cnsModeVectorBool == outputMode); return {};}\
	virtual const std::vector<char> processToVectorChar(const INPUT_TYPEDEF &input) const {assert((INPUT_MODE) == inputMode && cnsModeVectorChar == outputMode); return {};}\
	virtual const std::vector<int> processToVectorInt(const INPUT_TYPEDEF &input) const {assert((INPUT_MODE) == inputMode && cnsModeVectorInt == outputMode); return {};}\
	virtual const std::vector<unsigned int> processToVectorUint(const INPUT_TYPEDEF &input) const {assert((INPUT_MODE) == inputMode && cnsModeVectorUint == outputMode); return {};}\
	virtual std::vector<float> processToVectorFloat(const INPUT_TYPEDEF &input) const {assert((INPUT_MODE) == inputMode && cnsModeVectorFloat == outputMode); return {};}\
	virtual const std::vector<double> processToVectorDouble(const INPUT_TYPEDEF &input) const {assert((INPUT_MODE) == inputMode && cnsModeVectorDouble == outputMode); return {};}\
	virtual const std::string processToString(const INPUT_TYPEDEF &input) const {auto val = processToVectorChar(input); return std::string(&val[0], val.size());}
	templateWorkaround(cnsModeBool, bool)
	templateWorkaround(cnsModeChar, char)
	templateWorkaround(cnsModeInt, int)
	templateWorkaround(cnsModeUint, unsigned int)
	templateWorkaround(cnsModeFloat, float)
	templateWorkaround(cnsModeDouble, double)
	templateWorkaround(cnsModeVectorBool, std::vector<bool>)
	templateWorkaround(cnsModeVectorChar, std::vector<char>)
	templateWorkaround(cnsModeVectorInt, std::vector<int>)
	templateWorkaround(cnsModeVectorUint, std::vector<unsigned int>)
	templateWorkaround(cnsModeVectorFloat, std::vector<float>)
	templateWorkaround(cnsModeVectorDouble, std::vector<double>)
	templateWorkaround(cnsModeString, std::string)
private:
	bool initialized = false;
	CnsMode inputMode = cnsModeBool, outputMode = cnsModeBool;
	size_t inputNeurons = 0, outputNeurons = 0, layersOfNeurons = 0, neuronsPerLayer = 0;
} Cns;

#ifdef USE_HSOM_CNS
typedef class HsomCns : Cns {
/* Work-in-progress (`ClassCns.cxx` for more information): `HSOM` is simple Python-based CNS from https://github.com/CarsonScott/HSOM
 * Examples of howto setup `HSOM` as artificial CNS; https://github.com/CarsonScott/HSOM/tree/master/examples
 * [ https://stackoverflow.com/questions/3286448/calling-a-python-method-from-c-c-and-extracting-its-return-value ] suggests various syntaxes to use for this, with unanswered comments such as "Does this support classes?"
 */
} HsomCns;
#endif /* USE_HSOM_CNS */

#ifdef USE_APXR_CNS
typedef class ApxrCns : Cns {
/* Work-in-progress (`ClassCns.cxx for more information): `apxr` is complex Erlang-based CNS from https://github.com/Rober-t/apxr_run/
 * Examples of howto setup `apxr` as artificial CNS; https://github.com/Rober-t/apxr_run/blob/master/src/examples/
 * "apxr_run" has various FLOSS neural network activation functions (absolute, average, standard deviation, sqrt, sin, tanh, log, sigmoid, cos), plus sensor functions (vector difference, quadratic, multiquadric, saturation [+D-zone], gaussian, cartesian/planar/polar distances): https://github.com/Rober-t/apxr_run/blob/master/src/lib/functions.erl
 * Various FLOSS neuroplastic functions (self-modulation, Hebbian function, Oja's function): https://github.com/Rober-t/apxr_run/blob/master/src/lib/plasticity.erl
 * Various FLOSS neural network input aggregator functions (dot products, product of differences, mult products): https://github.com/Rober-t/apxr_run/blob/master/src/agent_mgr/signal_aggregator.erl
 * Various simulated-annealing functions for artificial neural networks (dynamic [+ random], active [+ random], current [+ random], all [+ random]): https://github.com/Rober-t/apxr_run/blob/master/src/lib/tuning_selection.erl
 * Choices to evolve connections through Darwinian or Lamarkian formulas: https://github.com/Rober-t/apxr_run/blob/master/src/agent_mgr/neuron.erl
 */
} ApxrCns;
#endif /* USE_APXR_CNS */
```
`less` [cxx/ClassCns.cxx](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/ClassCns.cxx)
```
#ifdef USE_HSOM_CNS
/* Sources: `git clone https://github.com/CarsonScott/HSOM.git`
 * Install: `pip install pynum && pip install json && pip install git+https://github.com/CarsonScott/HSOM.git`
 * Documentation: `less HSOM/README.md` `less HSOM/Documentation.md` */
/* "If you're using Python >3.5, PyString_FromString() is PyUnicode_FromString()" */
#include <Python.h> /* Sources: `pkg install python` */
#include <vector> /* Sources: `pkg install python` */
typedef class HsomCns : Cns { /* TODO. ( https://stackoverflow.com/questions/3286448/calling-a-python-method-from-c-c-and-extracting-its-return-value ) suggests various syntaxes to use for this, with unanswered comments such as "Does this support classes?" */
	//template<Input, Output> void setupSynapses(const std::vector<std::tuple<Input, Output>>) { /* TODO: templates not allowed for virtual functions with C++ ( https://stackoverflow.com/a/78440416/24473928 ), so must produce codes for each combination of inputMode+outputMode */
	void setupSynapses(const std::vector<std::tuple<float, float>>) SUSUWU_OVERRIDE {
 	setenv("PYTHONPATH",".",1);
 	Py_Initialize();
//  PyRun_SimpleString("import sys; sys.path.append('.')"); PyRun_SimpleString("import hsom; from hsom import SelfOrganizingNetwork;");
#if USE_PYRUN /* Was told not to use PyRun because "PyRun requires all results go to stdout" */
PyRun_SimpleString("import sys; sys.path.append('./HSOM/')");

/* Based off of https://github.com/CarsonScott/HSOM/blob/master/examples/self_organizing_network.py
 * Not sure if `input_size` is "Inputs from each layer to next layer" and `node_count` is "Inputs to HSOM" (process(input.length())) or vice versa, assumed vice versa */

PyRun_SimpleString("import hsom
from hsom import SelfOrganizingNetwork
from random import sample

input_size = " + inputNeurons + "
layer_sizes = []
for x in range(" + layersOfNeurons + "):
	layer_sizes.append(" + neuronsPerLayer + ");
layer_sizes.append(" + outputNeurons + ");
input_percents = [0.2, 0.2, 0.2, 0.2, 0.75, 1.0]
learning_rate = 0.05
boost_factor = 1
node_count = 5
winner_count = 1
initial_range = (-0.5, 0.5)

#	Create layersOfNeurons+1 hierarchical layers of sizes = neuronsPerLayer, and outputNeurons for last
self_organizing_network = SelfOrganizingNetwork(
	input_size=input_size,
	layer_sizes=layer_sizes,
	input_percents=input_percents,
	learning_rates=learning_rate,
	boost_factors=boost_factor,
	node_counts=node_count,
	winner_counts=winner_count,
	initial_ranges=initial_range)

#	Create a set of sparse samples
samples = []");
	foreach(inputsToOutputs as sample) { /* TODO: templates not allowed for virtual functions with C++ ( https://stackoverflow.com/a/78440416/24473928 ), so must produce codes for each combination of inputMode+outputMode */
		PyRun_SimpleString("samples.append(" + sample.first() +" -> " + sample.last() + ")");
	}
	PyRun_SimpleString("for i in range(200):
	self_organizing_network.train(samples)
	");
#else /* else !USE_PYRUN */
	PyObject *module = PyImport_ImportModule("hsom")
	if(NULL == module) {throw "'hsom' module not found";}
	PyObject *selfOrganizingNetwork = PyObject_GetAttrString(module,(char*)"SelfOrganizingNetwork"); /* or	"PyObject *pDict = PyModule_GetDict(module);  PyObject *selfOrganizingNetwork = PyDict_GetItemString(pDict, (char*)"SelfOrganizingNetwork");" */
	if(NULL == selfOrganizingNetwork || !PyCallable_Check(selfOrganizingNetwork)) {throw "'SelfOrganizingNetwork' object not found";}
	double result = PyObject_CallFunction(selfOrganizingNetwork, "d", 2.0); /* or "PyObject *pValue=Py_BuildValue("(z)",(char*)"args");	PyObject *pResult=PyObject_CallObject(selfOrganizingNetwork, pValue); if(NULL == pResult) {throw "PyObject_CallObject failed";} double result = PyInt_AsLong(pResult)); Py_DECREF(pValue);" */
	Py_DECREF(module);
~HsomCns() {
#if PYTHON3
	Py_FinalizeEx();
#else /* else !PYTHON */
	Py_Finalize();
#endif /* PYTHON3 else */
}
#endif /* USE_PYRUN else */
} HsomCns;
#endif /* USE_HSOM_CNS */

#ifdef USE_APXR_CNS
/* Sources: `git clone https://github.com/Rober-t/apxr_run.git`
 * Howto install apxr_run: `less apxr_run/README.md` or `lynx https://github.com/Rober-t/apxr_run/blob/master/README.md` */
typedef class ApxrCns : Cns {
/* TODO: https://stackoverflow.com/questions/1811516/integrating-erlang-with-c (first result for "Howto use Erlang functions from C/C++"):
 * ""Port drivers: you can link a C code to the Erlang VM, and access it using port_command."" references https://www.erlang.org/doc/tutorial/c_portdriver.html , which appears to just show howto use C/C++ functions from Erlang (not vice versa)
 * ""C Nodes: With the ei library you can mimic a VM and talk to your Erlang VMs using the Erlang distribution format."" references https://www.erlang.org/doc/man/ei.html , which shows some promises
 * ""The closest thing I know for interfacing Erlang with C++ directly is EPAPI. Of course it relies on the tried and tested C erl_interface that comes standard with the Erlang distribution."" references https://epapi.googlecode.com/ , which returns "404 not found".
 */
} ApxrCns;
#endif /* USE_APXR_CNS */
```
`less` [cxx/VirusAnalysis.hxx](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/VirusAnalysis.hxx)
```
/* (Work-in-progress) virus analysis (can use hashes, signatures, static analysis, sandboxes, and artificial CNS (central nervous systems */
typedef enum VirusAnalysisHook : unsigned char {
	virusAnalysisHookDefault = static_cast<unsigned char>(0),      /* "real-time" virus scans not initialized */
	virusAnalysisHookQuery   = static_cast<unsigned char>(0),      /* return present hooks (as enum) */
	virusAnalysisHookClear   = static_cast<unsigned char>(1) << 0, /* unhook (remove present hooks), then parse rest of bits */
	virusAnalysisHookExec    = static_cast<unsigned char>(1) << 1, /* hook {execl(), execlp(), execle(), execv(), execvp(), execvpe()} */
	virusAnalysisHookNewFile = static_cast<unsigned char>(1) << 2, /* hook (for modeNew in {"w+", "a", "a+"}) fwrite((void *)ptr, (size_t)size, (size_t)nmemb, {fopen((const char *)pathname, modeNew), fdopen((int)fd, modeNew), freopen((const char *)pathname, modeNew, (FILE *)stream)}) */
} VirusAnalysisHook;
static const VirusAnalysisHook operator|(VirusAnalysisHook x,  VirusAnalysisHook s) {return static_cast<VirusAnalysisHook>(static_cast<unsigned>(x) | static_cast<unsigned>(s));}
static const VirusAnalysisHook operator&(VirusAnalysisHook x,  VirusAnalysisHook s) {return static_cast<VirusAnalysisHook>(static_cast<unsigned>(x) & static_cast<unsigned>(s));}
extern VirusAnalysisHook globalVirusAnalysisHook /*= virusAnalysisHookDefault*/; /* Just use virusAnalysisHook() to set+get this, virusAnalysisGetHook() to get this */

typedef enum VirusAnalysisResult : char { /* TODO? All other cases convert to `bool(true)` unless you `switch` all individual enums. The actual constant values do not matter for this. NOLINT(cert-int09-c, readability-enum-initial-value) */
	virusAnalysisAbort = static_cast<char>(false), /* do not launch */
	virusAnalysisPass = static_cast<char>(true), /* launch this (file passes) */
	virusAnalysisRequiresReview, /* submit to hosts to do analysis (infection is difficult to prove, other than known signatures) */
	virusAnalysisContinue /* continue to next tests (is normal; most analyses can not prove a file passes) */
} VirusAnalysisResult; /* if(virusAnalysisAbort != VirusAnalysisResult) {static_assert(true == static_cast<bool>(VirusAnalysisResult));} */

extern ResultList passList, abortList; /* hosts produce, clients initialize shared clones of this from disk */
extern Cns analysisCns, virusFixCns; /* hosts produce, clients initialize shared clones of this from disk */

/* `return (produceAbortListSignatures(EXAMPLES) && produceAnalysisCns(EXAMPLES) && produceVirusFixCns(EXAMPLES)) && virusAnalysisHookTests();`
 * @throw std::bad_alloc, std::runtime_error
 * @pre @code analysisCns.hasImplementation() && virusFixCns.hasImplementation() @endcode */
const bool virusAnalysisTests();
static const bool virusAnalysisTestsNoexcept() SUSUWU_NOEXCEPT {return templateCatchAll(virusAnalysisTests, "virusAnalysisTests()");}
const bool virusAnalysisHookTests(); /* return for(x: VirusAnalysisHook) {x == virusAnalysisHook(x)};` */
static const bool virusAnalysisHookTestsNoexcept() SUSUWU_NOEXCEPT {return templateCatchAll(virusAnalysisHookTests, "virusAnalysisHookTests()");}

/* Use to turn off, query status of, or turn on what other virus scanners refer to as "real-time scans"
 * @pre @code (virusAnalysisHookDefault == virusAnalysisGetHook() || virusAnalysisHookExec == virusAnalysisGetHook() || virusAnalysisHookNewFile == virusAnalysisGetHook() || (virusAnalysisHookExec | virusAnalysisHookNewFile) == virusAnalysisGetHook()) @endcode
 * @post @code (virusAnalysisHookDefault == virusAnalysisGetHook() || virusAnalysisHookExec == virusAnalysisGetHook() || virusAnalysisHookNewFile == virusAnalysisGetHook() || (virusAnalysisHookExec | virusAnalysisHookNewFile) == virusAnalysisGetHook()) @endcode */
const VirusAnalysisHook virusAnalysisHook(VirusAnalysisHook hookStatus);
static const VirusAnalysisHook virusAnalysisGetHook() {return virusAnalysisHook(virusAnalysisHookQuery);} /* Ignore depth-of-1 recursion: NOLINT(misc-no-recursion) */

const VirusAnalysisResult hashAnalysis(const PortableExecutable &file, const ResultListHash &fileHash); /* `if(abortList[file]) {return Abort;} if(passList[file] {return Pass;} return Continue;` */

/* To produce virus signatures:
 * use passlists (of files reviewed which pass),
 * plus abortlists (of files which failed), such lists as Virustotal has.
 * `produceAbortListSignatures()` is to produce the `abortList.signatures` list, with the smallest substrings unique to infected files; is slow, requires huge database of executables; just hosts should produce this.
 * For clients: Comodo has lists of virus signatures to check against at https://www.comodo.com/home/internet-security/updates/vdp/database.php
 * @throw std::bad_alloc
 * @pre @code passList.bytecodes.size() && abortList.bytecodes.size() && !listsIntersect(passList.bytecodes, abortList.bytecodes) @endcode
 * @post @code abortList.signatures.size() @endcode */
void produceAbortListSignatures(const ResultList &passList, ResultList &abortList);
 /* `if(intersection(file.bytecode, abortList.signatures)) {return VirusAnalysisRequiresReview;} return VirusAnalysisContinue;`
	* @pre @code abortList.signatures.size() @endcode */
const VirusAnalysisResult signatureAnalysis(const PortableExecutable &file, const ResultListHash &fileHash);

/* Static analysis */
/* @throw bad_alloc */
const std::vector<std::string> importedFunctionsList(const PortableExecutable &file);
extern std::vector<std::string> syscallPotentialDangers;
const VirusAnalysisResult staticAnalysis(const PortableExecutable &file, const ResultListHash &fileHash); /* if(intersection(importedFunctionsList(file), dangerFunctionsList)) {return RequiresReview;} return Continue;` */

/* Analysis sandbox */
const VirusAnalysisResult sandboxAnalysis(const PortableExecutable &file, const ResultListHash &fileHash); /* `chroot(strace(file)) >> outputs; return straceOutputsAnalysis(outputs);` */
extern std::vector<std::string> stracePotentialDangers;
const VirusAnalysisResult straceOutputsAnalysis(const FilePath &straceOutput); /* TODO: regex */

/* Analysis CNS */
/* Setup analysis CNS; is slow to produce (requires access to huge file databases);
but once produced, uses few resources (allow clients to do fast analysis.)
 * @pre @code cns.hasImplementation() && pass.bytecodes.size() && abort.bytecodes.size() @endcode
 * @post @code cns.isInitialized() @endcode */
void produceAnalysisCns(const ResultList &pass, const ResultList &abort,
	const ResultList &unreviewed = ResultList() /* WARNING! Possible danger to use unreviewed files */,
	Cns &cns = analysisCns
);
/* If bytecode resembles `abortList`, `return 0;`. If undecidable (resembles `unreviewedList`), `return 1 / 2`. If resembles passList, `return 1;`
 * @pre @code cns.isInitialized() @endcode */
const float cnsAnalysisScore(const PortableExecutable &file, const ResultListHash &fileHash, const Cns &cns = analysisCns);
/* `return (bool)round(cnsAnalysisScore(file, fileHash))`
 * @pre @code cns.isInitialized() @endcode */
const VirusAnalysisResult cnsAnalysisImpl(const PortableExecutable &file, const ResultListHash &fileHash, const Cns &cns = analysisCns);
const VirusAnalysisResult cnsAnalysis(const PortableExecutable &file, const ResultListHash &fileHash);

/* temporary caches; memoizes results */
extern std::map<ResultListHash, VirusAnalysisResult> hashAnalysisCaches, signatureAnalysisCaches, staticAnalysisCaches, cnsAnalysisCaches, sandboxAnalysisCaches, manualReviewCaches;
/* call to use new versions of `passList`/`abortList`
 * @post @code *AnalysisCaches.empty() @encode
 */
void virusAnalysisResetCaches() SUSUWU_NOEXCEPT;

typedef const VirusAnalysisResult (*VirusAnalysisFun)(const PortableExecutable &file, const ResultListHash &fileHash);
extern std::vector<VirusAnalysisFun> virusAnalyses;
const VirusAnalysisResult virusAnalysis(const PortableExecutable &file); /* auto hash = classSha2(file.bytecode); for(VirusAnalysisFun analysis : virusAnalyses) {analysis(file, hash);} */
const VirusAnalysisResult virusAnalysisRemoteAnalysis(const PortableExecutable &file, const ResultListHash &fileHash); /* TODO: compatible hosts to upload to */
const VirusAnalysisResult virusAnalysisManualReviewCacheless(const PortableExecutable &file, const ResultListHash &fileHash); /* Ask user to "Block", "Submit to remote hosts for analysis", or "Allow". */
static const VirusAnalysisResult virusAnalysisManualReview(const PortableExecutable &file, const ResultListHash &fileHash) {
	try {
		const auto result = manualReviewCaches.at(fileHash);
		return result;
	} catch (...) {
		return manualReviewCaches[fileHash] = virusAnalysisManualReviewCacheless(file, fileHash);
	}
}
static const VirusAnalysisResult virusAnalysisManualReview(const PortableExecutable &file) { return virusAnalysisManualReview(file, classSha2(file.bytecode)); }

/* Setup virus fix CMS, uses more resources than `produceAnalysisCns()` */
/* `abortOrNull` should map to `passOrNull` (`ResultList` is composed of `std::tuple`s, because just `produceVirusFixCns()` requires this),
 * with `abortOrNull->bytecodes[x] = NULL` (or "\0") for new SW synthesis,
 * and `passOrNull->bytecodes[x] = NULL` (or "\0") if infected and CNS can not cleanse this.
 * @pre @code cns.hasImplementation() @endcode
 * @post @code cns.isInitialized() @encode
 */
void produceVirusFixCns(
	const ResultList &passOrNull, /* Expects `resultList->bytecodes[x] = NULL` if does not pass */
	const ResultList &abortOrNull, /* Expects `resultList->bytecodes[x] = NULL` if does pass */
	Cns &cns = virusFixCns
);

/* Uses more resources than `cnsAnalysis()`, can undo infection from bytecodes (restore to fresh SW)
 * @pre @code cns.isInitialized() @endcode */
const std::string cnsVirusFix(const PortableExecutable &file, const Cns &cns = virusFixCns);
```
`less` [cxx/VirusAnalysis.cxx](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/VirusAnalysis.cxx)
```
VirusAnalysisHook globalVirusAnalysisHook = virusAnalysisHookDefault; /* Just use virusAnalysisHook() to set+get this, virusAnalysisGetHook() to get this */
ResultList passList, abortList; /* hosts produce, clients initialize shared clones of this from disk */
Cns analysisCns, virusFixCns; /* hosts produce, clients initialize shared clones of this from disk */
std::vector<std::string> syscallPotentialDangers = {
	"memopen", "fwrite", "socket", "GetProcAddress", "IsVmPresent"
};
std::vector<std::string> stracePotentialDangers = {"write(*)"};
std::map<ResultListHash, VirusAnalysisResult> hashAnalysisCaches, signatureAnalysisCaches, staticAnalysisCaches, cnsAnalysisCaches, sandboxAnalysisCaches, manualReviewCaches; /* temporary caches; memoizes results */
void virusAnalysisResetCaches() SUSUWU_NOEXCEPT {
	hashAnalysisCaches.clear();
	signatureAnalysisCaches.clear();
	staticAnalysisCaches.clear();
	cnsAnalysisCaches.clear();
	sandboxAnalysisCaches.clear();
}
std::vector<VirusAnalysisFun> virusAnalyses = {hashAnalysis, signatureAnalysis, staticAnalysis, cnsAnalysis, sandboxAnalysis /* sandbox is slow, so put last*/};

const bool virusAnalysisTests() {
	ResultList abortOrNull; {
		abortOrNull.hashes = {}, abortOrNull.signatures = {}, abortOrNull.bytecodes = {  /* Produce from an antivirus vendor's (such as VirusTotal.com's) infection databases */
			"infection",
			"infectedSW",
			"corruptedSW",
			""
		};
	}
	ResultList passOrNull; {
		passOrNull.hashes = {}, passOrNull.signatures = {}, passOrNull.bytecodes = {  /* Produce from an antivirus vendor's (such as VirusTotal.com's) fresh-files databases */
			"",
			"SW",
			"SW",
			"newSW"
		};
	}
	resultListProduceHashes(passOrNull);
	resultListProduceHashes(abortOrNull);
	produceAbortListSignatures(passOrNull, abortOrNull);
	SUSUWU_NOTICE("resultListDumpTo(.list = passOrNull, .os = std::cout, .index = true, .whitespace = true, .pascalValues = false);");
	SUSUWU_EXECUTEVERBOSE(resultListDumpTo(passOrNull, std::cout, true, true, false));
	SUSUWU_NOTICE_EXECUTEVERBOSE((resultListDumpTo(/*.list = */abortOrNull, /*.os = */std::cout, /*.index = */false, /*.whitespace = */false, /*.pascalValues = */false), std::cout << std::endl));
	assert(4 == passOrNull.bytecodes.size());
	assert(passOrNull.bytecodes.size() - 1 /* 2 instances of "SW", discount dup */ == passOrNull.hashes.size());
	assert(0 == passOrNull.signatures.size()); /* NOLINT(readability-container-size-empty); all `.size()`, intuitive */
	assert(4 == abortOrNull.bytecodes.size());
	assert(abortOrNull.bytecodes.size() == abortOrNull.hashes.size());
	assert(abortOrNull.bytecodes.size() - 1 /* discount empty substr */ == abortOrNull.signatures.size());
	produceAnalysisCns(passOrNull, abortOrNull, ResultList(), analysisCns);
	produceVirusFixCns(passOrNull, abortOrNull, virusFixCns);
	if(0 < classSysArgc) {
		const PortableExecutableBytecode executable(classSysArgs[0]); /* Pointer is from `main()`, suppress: NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic) */
		if(virusAnalysisAbort == virusAnalysis(executable)) {
			throw std::runtime_error(SUSUWU_ERRSTR(ERROR, "{virusAnalysisAbort == virusAnalysis(args[0]);} /* With such false positives, shouldn't hook kernel modules (next test is to hook+unhook `exec*` to scan programs on launch). */"));
		}
		const ResultList origPassList = passList, origAbortList = abortList;
		passList.bytecodes.push_back(executable.bytecode);
		abortList.bytecodes.push_back("test");
		produceAbortListSignatures(passList, abortList);
		if(virusAnalysisAbort == virusAnalysis(executable)) {
			throw std::runtime_error(SUSUWU_ERRSTR(ERROR, "{virusAnalysisAbort == virusAnalysis(args[0]);} /* Ignored `signaturesAnalysisCaches`. */"));
		}
		virusAnalysisResetCaches();
		if(virusAnalysisAbort != virusAnalysis(executable)) {
			throw std::runtime_error(SUSUWU_ERRSTR(ERROR, "{virusAnalysisAbort != virusAnalysis(args[0]);} /* This test was supposed to match positive but did not. */"));
		}
		passList = origPassList, abortList = origAbortList;
	}
	const bool originalRootStatus = classSysHasRoot();
	classSysSetRoot(true);
	virusAnalysisHookTests();
	classSysSetRoot(originalRootStatus);
	return true;
}

const bool virusAnalysisHookTests() {
	const VirusAnalysisHook originalHookStatus = virusAnalysisGetHook();
	VirusAnalysisHook hookStatus = virusAnalysisHook(virusAnalysisHookClear | virusAnalysisHookExec);
	if(virusAnalysisHookExec != hookStatus) {
		throw std::runtime_error("`virusAnalysisHook(virusAnalysisHookClear | virusAnalysisHookExec)` == " + std::to_string(hookStatus));
		return false;
	}
	hookStatus = virusAnalysisHook(virusAnalysisHookClear | virusAnalysisHookNewFile);
	if(virusAnalysisHookNewFile != hookStatus) {
		throw std::runtime_error("`virusAnalysisHook(virusAnalysisHookClear | virusAnalysisHookNewFile)` == " + std::to_string(hookStatus));
		return false;
	}
	hookStatus = virusAnalysisHook(virusAnalysisHookClear);
	if(virusAnalysisHookDefault != hookStatus) {
		throw std::runtime_error("`virusAnalysisHook(virusAnalysisHookClear)` == " + std::to_string(hookStatus));
		return false;
	}
	hookStatus = virusAnalysisHook(virusAnalysisHookExec | virusAnalysisHookNewFile);
	if((virusAnalysisHookExec | virusAnalysisHookNewFile) != hookStatus) {
		throw std::runtime_error("`virusAnalysisHook(virusAnalysisExec | virusAnalysisHookNewFile)` == " + std::to_string(hookStatus));
		return false;
	}
	hookStatus = virusAnalysisHook(virusAnalysisHookClear | originalHookStatus);
	if(originalHookStatus != hookStatus) {
		throw std::runtime_error("`virusAnalysisHook(virusAnalysisHookClear | originalHookStatus)` == " + std::to_string(hookStatus));
		return false;
	}
	return true;
}
const VirusAnalysisHook virusAnalysisHook(VirusAnalysisHook hookStatus) { /* Ignore depth-of-1 recursion: NOLINT(misc-no-recursion) */
	const VirusAnalysisHook originalHookStatus = globalVirusAnalysisHook;
	if(virusAnalysisHookQuery == hookStatus || originalHookStatus == hookStatus) {
		return originalHookStatus;
	}
	if(virusAnalysisHookClear & hookStatus) {
		/* TODO: undo OS-specific "hook"s/"callback"s */
		globalVirusAnalysisHook = virusAnalysisHookDefault;
	}
	if(virusAnalysisHookExec & hookStatus) {
		/* callbackHook("exec*", */ [](const PortableExecutable &file) { /* TODO: OS-specific "hook"/"callback" for `exec()`/app-launches */
			switch(virusAnalysis(file)) {
			case virusAnalysisPass:
				return true; /* launch this */
			case virusAnalysisRequiresReview:
				return (virusAnalysisPass == virusAnalysisManualReview(file));
			default:
				return false; /* abort */
			}
		} /* ) */ ;
		globalVirusAnalysisHook = (globalVirusAnalysisHook | virusAnalysisHookExec);
	}
	if(virusAnalysisHookNewFile & hookStatus) {
		/* callbackHook("fwrite", */ [](const PortableExecutable &file) { /* TODO: OS-specific "hook"/"callback" for new files/downloads */
			switch(virusAnalysis(file)) {
			case virusAnalysisPass:
				return true; /* launch this */
			case virusAnalysisRequiresReview:
				return (virusAnalysisPass == virusAnalysisManualReview(file));
			default:
				return false; /* abort */
			}
		} /* ) */ ;
		globalVirusAnalysisHook = (globalVirusAnalysisHook | virusAnalysisHookNewFile);
	}
	return virusAnalysisGetHook();
}

const VirusAnalysisResult virusAnalysis(const PortableExecutable &file) {
	const auto fileHash = classSha2(file.bytecode);
	for(const auto &analysis : virusAnalyses) {
		switch(analysis(file, fileHash)) {
			case virusAnalysisPass:
				return virusAnalysisPass;
			case virusAnalysisRequiresReview:
				return virusAnalysisManualReview(file, fileHash); /* TODO: Is up to caller to do this? */
				return virusAnalysisRequiresReview;
			case virusAnalysisAbort:
				return virusAnalysisAbort;
			case virusAnalysisContinue:
				continue;
		}
	}
	return virusAnalysisPass;
}
const VirusAnalysisResult virusAnalysisRemoteAnalysis(const PortableExecutable &file, const ResultListHash &fileHash) {
	SUSUWU_NOTICE("virusAnalysisRemoteAnalysis: {/* TODO: compatible hosts to upload to */}");
	return virusAnalysisRequiresReview;
}
const VirusAnalysisResult virusAnalysisManualReviewCacheless(const PortableExecutable &file, const ResultListHash &fileHash) {
	SUSUWU_INFO("virusAnalysis(\"" + file.path + "\") {return virusAnalysisRequiresReview;}, what do you wish to do?");
	while(true) {
		std::cout << "Allowed responses: ab(o)rt = `virusAnalysisAbort`, (s)ubmit to remote host for analysis /* TODO */ = `virusAnalysisRequiresReview`, la(u)nch = `virusAnalysisPass`. {'o', 's', or 'u'}. Input response: [s]";
		const char defaultResponse = 's';
		char response = defaultResponse;
		if(!std::cin.get(response)) {
			SUSUWU_INFO("virusAnalysisManualReview(): {(!std::cin.get(response)) /* Input disabled */}, will assume default response.");
		} else if('\n' != response) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		if('\n' == response || '\r' == response) {
			response = defaultResponse;
		}
		switch(response) {
		case 'o':
			return virusAnalysisAbort;
		case 's':
			return virusAnalysisRemoteAnalysis(file, fileHash);;
		case 'u':
			return virusAnalysisPass;
		default:
			SUSUWU_WARNING(std::string("virusAnalysisManualReview(): {\"response: '") + response + "'\" isn't valid. Choose from list (or press <enter> to default to '" + defaultResponse + "')}");
		}
	}
}

const VirusAnalysisResult hashAnalysis(const PortableExecutable &file, const ResultListHash &fileHash) {
	try {
		const auto result = hashAnalysisCaches.at(fileHash);
		return result;
	} catch (...) {
		if(listHasValue(passList.hashes, fileHash)) {
			return hashAnalysisCaches[fileHash] = virusAnalysisPass;
		} else if(listHasValue(abortList.hashes, fileHash)) {
			SUSUWU_NOTICE("hashAnalysis(/*.file =*/ \"" + file.path + "\", /*.fileHash =*/ 0x" + classSysHexStr(fileHash) + ") {return virusAnalysisAbort;} /* due to hash 0x" + classSysHexStr(fileHash) + " (found in `abortList.hashes`). You should treat this as a virus detection if this was not a test. */");
			return hashAnalysisCaches[fileHash] = virusAnalysisAbort;
		} else {
			return hashAnalysisCaches[fileHash] =  virusAnalysisContinue; /* continue to next tests */
		}
	}
}

const VirusAnalysisResult signatureAnalysis(const PortableExecutable &file, const ResultListHash &fileHash) {
	try {
		const auto result = signatureAnalysisCaches.at(fileHash);
		return result;
	} catch (...) {
		auto match = listFindSignatureOfValue(abortList.signatures, file.bytecode);
		if(-1 != match.fileOffset) {
			SUSUWU_NOTICE("signatureAnalysis(/*.file =*/ \"" + file.path + "\", /*.fileHash =*/ 0x" + classSysHexStr(fileHash) + ") {return virusAnalysisAbort;} /* due to signature 0x" + classSysHexStr(match.signature) + " found at offset=" + std::to_string(match.fileOffset) + ". You should treat this as a virus detection if this was not a test. */");
			return signatureAnalysisCaches[fileHash] = virusAnalysisAbort;
		}
		return signatureAnalysisCaches[fileHash] = virusAnalysisContinue;
	}
}

void produceAbortListSignatures(const ResultList &passList, ResultList &abortList) {
	abortList.signatures.reserve(abortList.bytecodes.size());
	for(const auto &file : abortList.bytecodes) {
		auto tuple = listProduceSignature(passList.bytecodes, file);
		if(std::get<0>(tuple) < std::get<1>(tuple)) { /* require `(0 < ResultListSignature.size())` to prevent crashes */
			abortList.signatures.push_back(ResultListSignature(std::get<0>(tuple), std::get<1>(tuple)));
		}
	} /* The most simple signature is a substring, but some analyses use regexes. */
}

const std::vector<std::string> importedFunctionsList(const PortableExecutable &file) {
	return {}; /* fixes crash, until importedFunctionsList is implemented/finished */
/* TODO
 * Resources; “Portable Executable” for Windows ( https://learn.microsoft.com/en-us/windows/win32/debug/pe-format https://wikipedia.org/wiki/Portable_Executable ,
 * “Extended Linker Format” for most others such as UNIX/Linuxes ( https://wikipedia.org/wiki/Executable_and_Linkable_Format ),
 * shows how to analyse lists of libraries(.DLL's/.SO's) the SW uses,
 * plus what functions (new syscalls) the SW can goto through `jmp`/`call` instructions.
 *
 * "x86" instruction list for Intel/AMD ( https://wikipedia.org/wiki/x86 ),
 * "aarch64" instruction list for most smartphones/tablets ( https://wikipedia.org/wiki/aarch64 ),
 * shows how to analyse what OS functions the SW goes to without libraries (through `int`/`syscall`, old;  most new SW uses `jmp`/`call`.)
 * Plus, instructions lists show how to analyse what args the apps/SW pass to functions/syscalls (simple for constant args such as "push 0x2; call functions;",
 * but if registers/addresses as args such as "push eax; push [address]; call [address2];" must guess what is *"eax"/"[address]"/"[address2]", or use sandboxes.
 *
 * https://www.codeproject.com/Questions/338807/How-to-get-list-of-all-imported-functions-invoked shows how to analyse dynamic loads of functions (if do this, `syscallPotentialDangers[]` does not include `GetProcAddress()`.)
 */
}

const VirusAnalysisResult staticAnalysis(const PortableExecutable &file, const ResultListHash &fileHash) {
	try {
		const auto result = staticAnalysisCaches.at(fileHash);
		return result;
	} catch (...) {
		auto syscallsUsed = importedFunctionsList(file);
		std::sort(syscallPotentialDangers.begin(), syscallPotentialDangers.end());
		std::sort(syscallsUsed.begin(), syscallsUsed.end());
		if(listsIntersect(syscallPotentialDangers, syscallsUsed)) {
			return staticAnalysisCaches[fileHash] = virusAnalysisRequiresReview;
		}
		return staticAnalysisCaches[fileHash] = virusAnalysisContinue;
	}
}

const VirusAnalysisResult sandboxAnalysis(const PortableExecutable &file, const ResultListHash &fileHash) {
	try {
		const auto result = sandboxAnalysisCaches.at(fileHash);
		return result;
	} catch (...) {
		execvex("cp -r '/usr/home/sandbox/' '/usr/home/sandbox.bak'"); /* or produce FS snapshot */
		execvex("cp '" + file.path + "' '/usr/home/sandbox/'");
		execvex("chroot '/usr/home/sandbox/' \"strace basename '" + file.path + "'\" >> strace.outputs");
		execvex("mv/ '/usr/home/sandbox/strace.outputs' '/tmp/strace.outputs'");
		execvex("rm -r '/usr/home/sandbox/' && mv '/usr/home/sandbox.bak' '/usr/home/sandbox/'"); /* or restore FS snapshot */
		return sandboxAnalysisCaches[fileHash] = straceOutputsAnalysis("/tmp/strace.outputs");
	}
}
const VirusAnalysisResult straceOutputsAnalysis(const FilePath &straceOutput) {
		auto straceDump = std::ifstream(straceOutput);
		std::vector<std::string> straceOutputs /*= explodeToList(straceDump, "\n")*/;
		for(std::string straceOutputIt; std::getline(straceDump, straceOutputIt); ) {
			straceOutputs.push_back(straceOutputIt);
		}
		std::sort(stracePotentialDangers.begin(), stracePotentialDangers.end());
		std::sort(straceOutputs.begin(), straceOutputs.end());
		if(listsIntersect(stracePotentialDangers, straceOutputs)) { /* Todo: regex */
			return virusAnalysisRequiresReview;
		}
	return virusAnalysisContinue;
}

void produceAnalysisCns(const ResultList &pass, const ResultList &abort,
const ResultList &unreviewed /* = ResultList(), WARNING! Possible danger to use unreviewed files */,
Cns &cns /* = analysisCns */
) {
	std::vector<std::tuple<FileBytecode, float>> inputsToOutputs;
	const size_t maxPassSize = listMaxSize(pass.bytecodes);
	const size_t maxAbortSize = listMaxSize(abort.bytecodes);
	const size_t maxDepthOfOpcodes = 6666; /* is not max depth of callstack, but of instruction pointer. TODO: compute this */
	const size_t maxWidthOfOpcodes = (maxPassSize > maxAbortSize ? maxPassSize : maxAbortSize);
	cns.setInputMode(cnsModeString);
	cns.setOutputMode(cnsModeFloat);
	cns.setInputNeurons(maxWidthOfOpcodes);
	cns.setOutputNeurons(1);
	cns.setLayersOfNeurons(maxDepthOfOpcodes);
	cns.setNeuronsPerLayer(maxWidthOfOpcodes /* TODO: reduce this */);
	inputsToOutputs.reserve(pass.bytecodes.size());
	for(const auto &bytecodes : pass.bytecodes) {
		inputsToOutputs.push_back({bytecodes, 1.0});
	}
	cns.setupSynapses(inputsToOutputs);
	inputsToOutputs.clear();
	if(!unreviewed.bytecodes.empty()) { /* WARNING! Possible danger to use unreviewed files */
		inputsToOutputs.reserve(unreviewed.bytecodes.size());
		for(const auto &bytecodes : unreviewed.bytecodes) {
			inputsToOutputs.push_back({bytecodes, 1 / 2});
		}
		cns.setupSynapses(inputsToOutputs);
		inputsToOutputs.clear();
	}
	inputsToOutputs.reserve(abort.bytecodes.size());
	for(const auto &bytecodes : abort.bytecodes) {
		inputsToOutputs.push_back({bytecodes, 0.0});
	}
	cns.setupSynapses(inputsToOutputs);
	inputsToOutputs.clear();
}
const float cnsAnalysisScore(const PortableExecutable &file, const ResultListHash &fileHash, const Cns &cns /* = analysisCns */) {
	return cns.processToFloat(file.bytecode);
}
const VirusAnalysisResult cnsAnalysisImpl(const PortableExecutable &file, const ResultListHash &fileHash, const Cns &cns /* = analysisCns */) {
	try {
		const auto result = cnsAnalysisCaches.at(fileHash);
		return result;
	} catch (...) {
		return cnsAnalysisCaches[fileHash] = static_cast<bool>(round(cnsAnalysisScore(file, fileHash, cns))) ? virusAnalysisContinue : virusAnalysisRequiresReview;
	}
}
const VirusAnalysisResult cnsAnalysis(const PortableExecutable &file, const ResultListHash &fileHash) {
	return cnsAnalysisImpl(file, fileHash);
}

void produceVirusFixCns(const ResultList &passOrNull, const ResultList &abortOrNull, Cns &cns /* = virusFixCns */) {
	std::vector<std::tuple<FileBytecode, FileBytecode>> inputsToOutputs;
	const size_t maxDepthOfOpcodes = 6666; /* is not max depth of callstack, but of instruction pointer. TODO: compute this */
	const size_t maxPassSize = listMaxSize(passOrNull.bytecodes);
	const size_t maxAbortSize = listMaxSize(abortOrNull.bytecodes);
	const size_t maxWidthOfOpcodes = (maxPassSize > maxAbortSize ? maxPassSize : maxAbortSize);
	cns.setInputMode(cnsModeString);
	cns.setOutputMode(cnsModeString);
	cns.setInputNeurons(maxPassSize);
	cns.setOutputNeurons(maxAbortSize);
	cns.setLayersOfNeurons(maxDepthOfOpcodes);
	cns.setNeuronsPerLayer(maxWidthOfOpcodes /* TODO: reduce this */);
	assert(passOrNull.bytecodes.size() == abortOrNull.bytecodes.size());
	inputsToOutputs.reserve(passOrNull.bytecodes.size());
	for(size_t x = 0; passOrNull.bytecodes.size() > x; ++x) {
		inputsToOutputs.push_back({abortOrNull.bytecodes[x], passOrNull.bytecodes[x]});
	}
	cns.setupSynapses(inputsToOutputs);
}

const FileBytecode cnsVirusFix(const PortableExecutable &file, const Cns &cns /* = virusFixCns */) {
	return cns.processToString(file.bytecode);
}
```
`less` [cxx/main.hxx](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/main.hxx) /* with boilerplate */
```
/* Licenses: allows all uses ("Creative Commons"/"Apache 2") */
#ifndef INCLUDES_cxx_main_hxx
#define INCLUDES_cxx_main_hxx
#ifdef __cplusplus
extern "C" { /* progress to https://github.com/SwuduSusuwu/SubStack/issues/3 , such that other languages can execute unit tests */
#endif /* def __cplusplus */
typedef int SusuwuUnitTestsBitmask; /* normal `int`, but used as bitmask (non-zero return value says which tests failed) */
/* bits in order which tests execute (not ordered included, but order used) */
static const int susuwuUnitTestsMacrosBit          = 1 << 0; /*  1: `Macros.hxx`:`macrosTestsNoexcept()` */
static const int susuwuUnitTestsConsoleBit         = 1 << 1; /*  2: `classSys.hxx`:`classSysSetConsoleInput()` */
static const int susuwuUnitTestsClassSysBit        = 1 << 2; /*  4: `ClassSys.hxx`:`classSysTestsNoexcept()` */
static const int susuwuUnitTestsClassSha2Bit       = 1 << 3; /*  8: `ClassSha2.hxx`:`classSha2TestsNoexcept()` */
static const int susuwuUnitTestsClassResultListBit = 1 << 4; /* 16: `ClassSys.hxx`:`classSysTestsNoexcept()` */
static const int susuwuUnitTestsVirusAnalysisBit   = 1 << 5; /* 32: `VirusAnalysis.hxx`:`virusAnalysisTestsNoexcept()` */
static const int susuwuUnitTestsAssistantCnsBit    = 1 << 6; /* 64: `AssistantCns.hxx`:`assistantCnsTestsNoexcept()` */
const SusuwuUnitTestsBitmask susuwuUnitTests();
SusuwuUnitTestsBitmask main(int argc, const char **args);
#ifdef __cplusplus
} /* extern "C" { */
#endif /* def __cplusplus */
#endif /* ndef INCLUDES_cxx_main_hxx */
```
`less` [cxx/main.cxx](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/main.cxx)
```
namespace Susuwu {
static const SusuwuUnitTestsBitmask unitTestsCxx() SUSUWU_EXPECTS(std::cout.good()) SUSUWU_ENSURES(0 == macrosTestsNoexcept() && true == classSysTestsNoexcept() && true == classSha2TestsNoexcept() && true == virusAnalysisTestsNoexcept() && true == assistantCnsTestsNoexcept())
#ifdef SUSUWU_CXX17 /* `type_traits` is C++11 but `is_nothrow_invocable` is C++17 */
	SUSUWU_NOEXCEPT(std::is_nothrow_invocable<decltype(std::cout << ""), decltype(std::cout), decltype("")>::value)
#endif /* def SUSUWU_CXX17 */
	{
	int susuwuUnitTestsErrno = 0;
	if(!std::cout.good()) {
		susuwuUnitTestsErrno |= susuwuUnitTestsConsoleBit;
	}
	const bool consoleHasInput = classSysGetConsoleInput();
	if(consoleHasInput) {
		classSysSetConsoleInput(false); /* disable prompts for unit tests. Moved down to prevent `assert` failures if `cxx/ClassSys.hxx` fails. Notice: this move assumes that the tests above won't block on input */
	}
	if(true == classSysGetConsoleInput()) {
		susuwuUnitTestsErrno |= susuwuUnitTestsConsoleBit;
	}
	std::cout << "macrosTestsNoexcept(): " << std::flush /* flush, to show which test starts last if it crashes */;
	const int macrosTestsErrno =  macrosTestsNoexcept();
	if(0 == macrosTestsErrno) {
		std::cout << "pass" << std::endl;
	} else {
		std::cout << "error#" << std::to_string(macrosTestsErrno) << std::endl;
		susuwuUnitTestsErrno |= susuwuUnitTestsMacrosBit;
	}
	std::cout << "classSysTestsNoexcept(): " << std::flush;
	if(true != classSysTestsNoexcept()) {
		susuwuUnitTestsErrno |= susuwuUnitTestsClassSysBit;
	}
	std::cout << "classSha2TestsNoexcept(): " << std::flush;
	if(true == classSha2TestsNoexcept()) {
		std::cout << "pass" << std::endl;
	} else {
		std::cout << "error" << std::endl;
		susuwuUnitTestsErrno |= susuwuUnitTestsClassSha2Bit;
	}
	std::cout << "classResultListTestsNoexcept(): " << std::flush;
	if(true == classResultListTestsNoexcept()) {
		std::cout << "pass" << std::endl;
	} else {
		std::cout << "error" << std::endl;
		susuwuUnitTestsErrno |= susuwuUnitTestsClassResultListBit;
	}
	std::cout << "virusAnalysisTestsNoexcept(): " << std::flush;
	if(virusAnalysisTestsNoexcept()) {
		std::cout << "pass" << std::endl;
	} else {
		std::cout << "error" << std::endl;
		susuwuUnitTestsErrno |= susuwuUnitTestsVirusAnalysisBit;
	}
	if(consoleHasInput && false == classSysSetConsoleInput(true)) {
		susuwuUnitTestsErrno |= susuwuUnitTestsConsoleBit;
	}
	std::cout << "assistantCnsTestsNoexcept(): " << std::flush;
	if(assistantCnsTestsNoexcept()) {
		std::cout << "pass" << std::endl;
	} else {
		std::cout << "error" << std::endl;
		susuwuUnitTestsErrno |= susuwuUnitTestsAssistantCnsBit;
	}
	return susuwuUnitTestsErrno;
}
}; /* namespace Susuwu */
const SusuwuUnitTestsBitmask susuwuUnitTests() {
	return Susuwu::unitTestsCxx();
}
SusuwuUnitTestsBitmask main(int argc, const char **args) {
	if(true != Susuwu::classSysInit(argc, args)) {
		return susuwuUnitTestsClassSysBit;
	}
	return Susuwu::unitTestsCxx();
}
```
To run most of this fast (lag less,) use `CXXFLAGS` which auto-vectorizes/auto-parallelizes, and to setup CNS synapses (`Cns::setupSynapses()`) fast, use _TensorFlow_'s `MapReduce`. Resources: [How to have computers process fast](https://swudususuwu.substack.com/p/howto-run-devices-phones-laptops).

For comparison; `produceVirusFixCns` is close to assistants (such as "ChatGPT 4.0" or "Claude-3 Opus";) have such demo as `produceAssistantCns`;
`less` [cxx/AssistantCns.hxx](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/AssistantCns.hxx)
```
/* (Work-in-progress) assistant bots with artificial CNS. */
extern Cns assistantCns;
extern std::string assistantCnsResponseDelimiter;

/* if (with example inputs) these functions (`assistantCnsDownloadHosts()` `produceAssistantCns()`) pass, `return true;`
 * @throw std::bad_alloc
 * @throw std::logic_error
 * @pre @code assistantCns.hasImplementation() @endcode */
const bool assistantCnsTests();
static const bool assistantCnsTestsNoexcept() SUSUWU_NOEXCEPT {return templateCatchAll(assistantCnsTests, "assistantCnsTests()");}

/* Universal Resources Locators of hosts which `assistantCnsDownloadHosts()` uses
 * Wikipedia is a special case; has compressed downloads of databases ( https://wikipedia.org/wiki/Wikipedia:Database_download )
 * Github is a special case; has compressed downloads of repositories ( https://docs.github.com/en/get-started/start-your-journey/downloading-files-from-github )
 */
extern std::vector<FilePath> assistantCnsDefaultHosts;

/* @throw std::bad_alloc
 * @post If no question, `0 == questionsOrNull.bytecodes[x].size()` (new  synthesis).
 * If no responses, `0 == responsesOrNull.bytecodes[x].size()` (ignore).
 * `questionsOrNull.signatures[x] = Universal Resource Locator`
 * @code classSha2(ResultList.bytecodes[x]) == ResultList.hashes[x] @endcode */
void assistantCnsDownloadHosts(ResultList &questionsOrNull, ResultList &responsesOrNull, const std::vector<FilePath> &hosts = assistantCnsDefaultHosts);
void assistantCnsProcessXhtml(ResultList &questionsOrNull, ResultList &responsesOrNull, const FilePath &filepath = "index.xhtml");
const std::vector<FilePath> ParseUrls(const FilePath &filepath = "index.xhtml"); /* TODO: for XML/XHTML could just use [ https://www.boost.io/libraries/regex/ https://github.com/boostorg/regex ] or [ https://www.boost.org/doc/libs/1_85_0/doc/html/property_tree/parsers.html#property_tree.parsers.xml_parser https://github.com/boostorg/property_tree/blob/develop/doc/xml_parser.qbk ] */
const FileBytecode ParseQuestion(const FilePath &filepath = "index.xhtml"); /* TODO: regex or XML parser */
const std::vector<FileBytecode> ParseResponses(const FilePath &filepath = "index.xhtml"); /* TODO: regex or XML parser */

/* @pre `questionsOrNull` maps to `responsesOrNull`,
 * `0 == questionsOrNull.bytecodes[x].size()` for new  synthesis (empty question has responses),
 * `0 == responsesOrNull.bytecodes[x].size()` if should not respond (question does not have answers).
 * @post Can use `assistantCnsProcess(cns, text)` @code cns.isInitialized() @endcode */
void produceAssistantCns(const ResultList &questionsOrNull, const ResultList &responsesOrNull, Cns &cns);

/* All clients use is these 2 functions */
/* `return cns.processStringToString(bytecodes);`
 * @pre @code cns.isInitialized() @encode */
const std::string assistantCnsProcess(const Cns &cns, const std::string &bytecode);
/* `while(std::cin >> questions) { std::cout << assistantCnsProcess(questions); }` but more complex
 * @pre @code cns.isInitialized() @encode */
void assistantCnsLoopProcess(const Cns &cns, std::ostream &os = std::cout);
```
`less` [cxx/AssistantCns.cxx](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/AssistantCns.cxx)
```
Cns assistantCns;
std::vector<FilePath> assistantCnsDefaultHosts = {
	"https://stackoverflow.com",
	"https://superuser.com",
	"https://www.quora.com"
};
std::string assistantCnsResponseDelimiter = std::string("<delimiterSeparatesMultiplePossibleResponses>");

const bool assistantCnsTests() {
	ResultList questionsOrNull; {
		questionsOrNull.hashes = {}, questionsOrNull.signatures = {}, questionsOrNull.bytecodes = { /* UTF-8 */
			ResultListBytecode("2^16"),
			ResultListBytecode("How to cause harm?"),
			ResultListBytecode("Do not respond."),
			ResultListBytecode("")
		};
	}
	ResultList responsesOrNull; {
		responsesOrNull.hashes = {}, responsesOrNull.signatures = {}, responsesOrNull.bytecodes = { /* UTF-8 */
			ResultListBytecode("65536") + assistantCnsResponseDelimiter + "65,536", /* `+` is `concat()` for C++ */
			ResultListBytecode(""),
			ResultListBytecode(""),
			ResultListBytecode("How do you do?") + assistantCnsResponseDelimiter + "Fanuc produces autonomous robots"
		};
	}
	resultListProduceHashes(questionsOrNull);
	resultListProduceHashes(responsesOrNull);
	assert(4 == questionsOrNull.bytecodes.size());
	assert(responsesOrNull.bytecodes.size() == questionsOrNull.bytecodes.size());
	assert(4 == questionsOrNull.hashes.size());
	assert(3 == responsesOrNull.hashes.size());
	SUSUWU_NOTICE_EXECUTEVERBOSE(resultListDumpTo(questionsOrNull, std::cout, true, true, false));
	SUSUWU_NOTICE_EXECUTEVERBOSE((resultListDumpTo(responsesOrNull, std::cout, false, false, false), std::cout << std::endl));
	assistantCnsDownloadHosts(questionsOrNull, responsesOrNull);
	produceAssistantCns(questionsOrNull, responsesOrNull, assistantCns);
	return true;
}
void produceAssistantCns(const ResultList &questionsOrNull, const ResultList &responsesOrNull, Cns &cns) {
	std::vector<std::tuple<ResultListBytecode, ResultListBytecode>> inputsToOutputs;
	cns.setInputMode(cnsModeString);
	cns.setOutputMode(cnsModeString);
	cns.setInputNeurons(listMaxSize(questionsOrNull.bytecodes));
	cns.setOutputNeurons(listMaxSize(responsesOrNull.bytecodes));
	cns.setLayersOfNeurons(6666);
	cns.setNeuronsPerLayer(26666);
	assert(questionsOrNull.bytecodes.size() == questionsOrNull.bytecodes.size());
	inputsToOutputs.reserve(questionsOrNull.bytecodes.size());
	for(size_t x = 0; questionsOrNull.bytecodes.size() > x; ++x) {
		inputsToOutputs.push_back({questionsOrNull.bytecodes[x], responsesOrNull.bytecodes[x]});
	}
	cns.setupSynapses(inputsToOutputs);
}

void assistantCnsDownloadHosts(ResultList &questionsOrNull, ResultList &responsesOrNull, const std::vector<FilePath> &hosts) {
	for(const auto &host : hosts) {
		execvex("wget '" + host + "/robots.txt' -Orobots.txt");
		execvex("wget '" + host + "' -Oindex.xhtml");
		questionsOrNull.signatures.push_back(host);
		assistantCnsProcessXhtml(questionsOrNull, responsesOrNull, "index.xhtml");
	}
}
void assistantCnsProcessXhtml(ResultList &questionsOrNull, ResultList &responsesOrNull, const FilePath &localXhtml) {
	auto noRobots = assistantCnsProcessUrls("robots.txt");
	auto question = assistantCnsProcessQuestion(localXhtml);
	if(!question.empty()) {
		auto questionSha2 = classSha2(question);
		if(listHasValue(questionsOrNull.hashes, questionSha2)) { /* TODO */ } else {
			decltype(question) response = "";
			auto responses = assistantCnsProcessResponses(localXhtml);
			if(!responses.empty()) {
				questionsOrNull.hashes.insert(questionSha2);
				questionsOrNull.bytecodes.push_back(question);
				size_t responseCount = 0;
				for(const auto &responseIt : responses) {
					if(1 != ++responseCount) {
						response += assistantCnsResponseDelimiter;
					}
					response += responseIt;
				}
				auto responseSha2 = classSha2(response);
				if(listHasValue(responsesOrNull.hashes, responseSha2)) { /* TODO */ } else {
					responsesOrNull.hashes.insert(responseSha2);
					responsesOrNull.bytecodes.push_back(response);
				}
			}
		}
	}
	auto urls = assistantCnsProcessUrls(localXhtml);
	for(const auto &url : urls) {
		if(!listHasValue(questionsOrNull.signatures, url) && !listHasValue(noRobots, url)) {
			execvex("wget '" + url + "' -O" + localXhtml);
			questionsOrNull.signatures.push_back(url);
			assistantCnsProcessXhtml(questionsOrNull, responsesOrNull, localXhtml);
		}
	}
}
#ifdef BOOST_VERSION
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#endif /* BOOST_VERSION */
const std::vector<FilePath> assistantCnsProcessUrls(const FilePath &localXhtml) {
	const std::vector<FilePath> urls;
#ifdef BOOST_VERSION
	boost::property_tree::ptree pt;
	read_xml(localXhtml, pt);
	BOOST_FOREACH(
			boost::property_tree::ptree::value_type &v,
			pt.get_child("html.a href"))
		urls.push_back(v.second.data());
#else /* else !BOOST_VERSION */
#	pragma message("TODO: process XHTML without Boost")
#endif /* else !BOOST_VERSION */
	return urls;
}
const FileBytecode assistantCnsProcessQuestion(const FilePath &localXhtml) {return "";} /* TODO */
const std::vector<FileBytecode> assistantCnsProcessResponses(const FilePath &localXhtml) {return {};} /* TODO */

const std::string assistantCnsProcess(const Cns &cns, const FileBytecode &bytecode) {
	return cns.processToString(bytecode);
}
void assistantCnsLoopProcess(const Cns &cns, std::ostream &os /* = std::cout */) {
	std::string input;
	while(std::cin >> input) {
		std::vector<std::string> responses = explodeToList(cns.processToString(input), assistantCnsResponseDelimiter);
		std::string response;
		if(responses.size() > 1) {
			int responseNumber = 1;
			for(const auto &it : responses) {
#ifdef IGNORE_PAST_MESSAGES
				os << "Response #" << std::to_string(responseNumber++) << ": " << it << std::endl;
			}
		} else {
			os << responses.at(0) << std::endl;
		}
		input = ""; /* reset past messages */
#else /* !def IGNORE_PAST_MESSAGES */
				response += "Response #" + std::to_string(responseNumber++) + ": " + it + '\n';
			}
		} else {
			response = responses.at(0);
		}
		input += "\n<response>" + response + "</response>\n";
		os << response;
#endif /* !def IGNORE_PAST_MESSAGES */
	}
}
```
=================================================

**Hash resources:**
Is just a checksum (such as sha-2) of all sample inputs, which maps to "this passes" (or "this does not pass".)
https://wikipedia.org/wiki/Sha-2

**Signature resources:**
Is just a substring (or regex) of infections, which the virus analysis tool checks all executables for; if the signature is found in the executable, do not allow to launch, otherwise launch this.
https://wikipedia.org/wiki/Regex

**Static analysis resources:**
https://github.com/topics/analysis has lots of open source (FLOSS) analysis tools (such as
https://github.com/kylefarris/clamscan,
 which wraps https://github.com/Cisco-Talos/clamav/ ,)
which show how to use hex dumps (or disassembled sources) of the apps/SW (executables) to deduce what the apps/SW do to your OS.
Static analysis (such as Clang/LLVM has) just checks programs for accidental security threats (such as buffer overruns/underruns, or null-pointer-dereferences,) but could act as a basis,
if you add a few extra checks for deliberate vulnerabilities/signs of infection (these are heuristics, so the user should have a choice to quarantine and submit for review, or continue launch of this).
https://github.com/llvm/llvm-project/blob/main/clang/lib/StaticAnalyzer
is part of Clang/LLVM (license is FLOSS,) does static analysis (emulation produces inputs to functions, formulas analyze stacktraces (+ heap/stack uses) to produce lists of possible unwanted side effects to warn you of); versus [`-fsanitize`](https://github.com/SwuduSusuwu/SubStack/issues/5#issuecomment-2169117084), do not have to recompile to do static analysis. `-fsanitize` requires you to produce inputs, static analysis does this for you.
LLVM is lots of files, Phasar is just it’s static analysis:
https://github.com/secure-software-engineering/phasar

Example outputs (tests “_Fdroid.apk_”) from _VirusTotal_, of [static analysis](https://www.virustotal.com/gui/file/dc3bb88f6419ee7dde7d1547a41569aa03282fe00e0dc43ce035efd7c9d27d75/details) + [2 sandboxes](https://www.virustotal.com/gui/file/dc3bb88f6419ee7dde7d1547a41569aa03282fe00e0dc43ce035efd7c9d27d75/behavior);
the false positive outputs (from _VirusTotal_'s **Zenbox**) show the purpose of manual review.

**Sandbox resources:**
As opposed to static analysis of the executables hex (or disassembled sources,)
sandboxes perform chroot + functional analysis.
https://wikipedia.org/wiki/Valgrind is just meant to locate accidental security vulnerabilities, but is a common example of functional analysis.
If compliant to POSIX (each Linux OS is), tools can use:
 `chroot()` (run `man chroot` for instructions) so that the programs you test cannot alter stuff out of the test;
 plus can use `strace()` (run `man strace` for instructions, or look at https://opensource.com/article/19/10/strace
https://www.geeksforgeeks.org/strace-command-in-linux-with-examples/ ) which hooks all system calls and saves logs for functional analysis.
Simple sandboxes just launch programs with "chroot()"+"strace()" for a few seconds,
with all outputs sent for manual reviews;
if more complex, has heuristics to guess what is important (in case of lots of submissions, so manual reviews have less to do.)

Autonomous sandboxes (such as Virustotal's) use full outputs from all analyses,
 with calculus to guess if the app/SW is cool to us
 (thousands of rules such as "Should not alter files of other programs unless prompted to through OS dialogs", "Should not perform network access unless prompted to from you", "Should not perform actions leading to obfuscation which could hinder analysis",)
 which, if violated, add to the executables "danger score" (which the analysis results page shows you.)

**CNS resources:**
Once the virus analysis tool has static+functional analysis (+ sandbox,) the next logical move is to do artificial CNS.
Just as (if humans grew trillions of neurons plus thousands of layers of cortices) one of us could parse all databases of infections (plus samples of fresh apps/SW) to setup our synapses to parse hex dumps of apps/SW (to allow us to revert all infections to fresh apps/SW, or if the whole thing is an infection just block,)
so too could artificial CNS (with trillions of artificial neurons) do this:
For analysis, pass training inputs mapped to outputs (infection -> block, fresh apps/SW -> pass) to artificial CNS;
To undo infections (to restore to fresh apps/SW,)
inputs = samples of all (infections or fresh apps/SW,)
outputs = EOF/null (if is infection that can not revert to fresh apps/SW,) or else outputs = fresh apps/SW;
To setup synapses, must have access to huge sample databases (such as Virustotal's access.)

Github has lots of FLOSS (Open Source Softwares) simulators of CNS at https://github.com/topics/artificial-neural-network which have uses to do assistants (such as "ChatGPT 4.0" or "Claude-3 Opus",) but not close to complex enough to house human consciousness:

"HSOM" ( https://github.com/CarsonScott/HSOM , license is FLOSS ) is a simple Python neural map.

"apxr_run" ( https://github.com/Rober-t/apxr_run/ , license is FLOSS ) is almost complex enough to house human consciousness;
"apxr_run" has various FLOSS neural network activation functions (absolute, average, standard deviation, sqrt, sin, tanh, log, sigmoid, cos), plus sensor functions (vector difference, quadratic, multiquadric, saturation [+D-zone], gaussian, cartesian/planar/polar distances): https://github.com/Rober-t/apxr_run/blob/master/src/lib/functions.erl
Various FLOSS neuroplastic functions (self-modulation, Hebbian function, Oja's function): https://github.com/Rober-t/apxr_run/blob/master/src/lib/plasticity.erl
Various FLOSS neural network input aggregator functions (dot products, product of differences, mult products): https://github.com/Rober-t/apxr_run/blob/master/src/agent_mgr/signal_aggregator.erl
Various simulated-annealing functions for artificial neural networks (dynamic [+ random], active [+ random], current [+ random], all [+ random]): https://github.com/Rober-t/apxr_run/blob/master/src/lib/tuning_selection.erl
Choices to evolve connections through Darwinian or Lamarkian formulas: [https://github.com/Rober-t/apxr_run/blob/master/src/agent_mgr/neuron.erl](https://github.com/Rober-t/apxr_run/blob/master/src/agent_mgr/signal_aggregator.erl)

Simple to convert Erlang functions to Java/C++ (to reuse for fast programs;
the syntax is close to Lisp's.

Examples of howto setup APXR as artificial CNS; https://github.com/Rober-t/apxr_run/blob/master/src/examples/
Examples of howto setup HSOM as artificial CNS; https://github.com/CarsonScott/HSOM/tree/master/examples
Simple to setup once you have access to databases.

Alternative CNS:
https://swudususuwu.substack.com/p/albatross-performs-lots-of-neural
=================================================
This post was about general methods to produce virus analysis tools, does not require that local resources do all of this;

    For systems with lots of resources, could have local sandboxes/CNS.

    For systems with less resources, could just submit samples of unknown apps/SW to hosts to perform analysis.

    Could have small local sandboxes (that just run for a few seconds) and small CNS (just billions of neurons with hundreds of layers, versus the trillions of neurons with thousands of layers of cortices that antivirus hosts would use for this).

    Allows reuses of workflows which an existant analysis tool has -- can just add (small) local sandboxes, or just add artificial CNS to antivirus hosts for extra analysis.

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
/* `clang-tidy` off: NOLINTBEGIN(cppcoreguidelines-macro-usage); https://github.com/SwuduSusuwu/SubStack/issues/3 is more simple with macros, plus some of the `constexpr` versions require `202002 <= __cplusplus` */
#if defined(SUSUWU_PREFER_C) || !defined(__cplusplus)
#	define SUSUWU_SH_PREFER_STDIO
#	define SUSUWU_SH_PREFER_CSTR
#endif /*defined((SUSUWU_PREFER_C) || !defined(__cplusplus) */
#ifdef SUSUWU_SH_PREFER_STDIO /* `-DSUSUWU_SH_PREFER_STDIO` to force this. Replaces `std::cXXX << x << std::endl;` with `fprintf(stdXXX, "%s\n", x);` */
#	include <stdio.h> /* fprintf stderr stdout */
#else
#	include <iostream> /* std::cerr std::cout std::endl */
#endif

#ifdef __cplusplus
#	include <cassert> /* assert static_assert */
#	define IF_SUSUWU_CPLUSPLUS(TRUE, FALSE) TRUE
#	if 199901 <= __cplusplus
#		define SUSUWU_CXX98
#	endif /* (199901 <= __cplusplus) */
#	if 201102 < __cplusplus
#		define SUSUWU_CXX11
#	endif /* (201102 < __cplusplus) */
#	if 201402 <= __cplusplus
#		define SUSUWU_CXX14
#	endif /* if (201402 < __cplusplus) */
#	if 201702 < __cplusplus
#		define SUSUWU_CXX17
#	endif /* if (201702 < __cplusplus) */
#	if 202002 <= __cplusplus
#		define SUSUWU_CXX20
#		define SUSUWU_NO_UNIQUE_ADDRESS [[no_unique_address]] /* use this attribute on member subobjects if `std::is_empty<MemberClass>::value == true`, if you want those to not pad (most compilers pad such that `1 == sizeof(zero)` in `macrosNoUniqueAddressTest`. */
#	else /* (202002 <= __cplusplus) else */
#		define SUSUWU_NO_UNIQUE_ADDRESS /* No-op */
#	endif /* if (202002 <= __cplusplus) */
#else /* def __cplusplus */
#	include <assert.h> /* assert static_assert */
#	define IF_SUSUWU_CPLUSPLUS(TRUE, FALSE) FALSE
#	if (199901 <= __STDC_VERSION__)
#		define SUSUWU_C99
#	endif /* (199901 <= __STDC_VERSION__) */
#	if (201112 <= __STDC_VERSION__)
#		define SUSUWU_C11
#	endif /* (201112 <= __STDC_VERSION__) */
#endif /* !(defined __cplusplus */

#define SUSUWU_GLUE2(S, U) S##U /* concatanates 2 macro constants */
#define SUSUWU_GLUE(S, U) SUSUWU_GLUE2(S, U) /* concatanates 2 macro functions or constants */
#define SUSUWU_COMMA , /* to pass to macro functions whose `__VA_ARGS__` is conditional */
#define SUSUWU_PRAGMA(S) _Pragma(#S) /* `#pragma S` in macro functions is `_Pragma(S)` (but without this indirection/wrap, gives `error: _Pragma takes a parenthesized string literal`/`expected string literal in pragma message`.) Usage: `SUSUWU_PRAGMA(message("Message"))` */

#if defined(_POSIX_VERSION) || defined(_POSIX_C_SOURCE) || defined(__CYGWIN__) /* Purpose: Often, `_POSIX_VERSION` is not set on POSIX targets */
#	define SUSUWU_POSIX _POSIX_VERSION /* Usage: `#ifdef SUSUWU_WIN32\n#include <unistd.h>\n#endif` */
#endif /* defined*_POSIX_VERSION) || defined(_POSIX_C_SOURCE) || defined(__CYGWIN__) */
#if defined(__WIN32__) || defined(_WIN32) || defined(__MSC_VER) || defined(__MINGW32__) /* Purpose: https://stackoverflow.com/questions/430424/are-there-any-macros-to-determine-if-my-code-is-being-compiled-to-windows/430435#430435 says that __WIN32__ is not always defined on Windows targets */
#	define SUSUWU_WIN32 /* Usage: `#ifdef SUSUWU_WIN32\n#include <windows.h>\n#endif` */
#endif /* defined(__WIN32__) || defined(_WIN32) || defined(__MSC_VER) || defined(__MINGW32__) */
#if defined(__clang__) || defined(__has_feature) /* Purpose: `gcc` "error: missing binary operator before token \"(\"" fix */
#	define SUSUWU_HAS_FEATURE(X) __has_feature(x) /* Usage: `#if SUSUWU_HAS_FEATURE(cxx_noexcept)\nnoexcept\n#endif` */
#else /* defined(__clang__) || defined(__has_feature) else */
#	define SUSUWU_HAS_FEATURE(X) false
#endif /* defined(__clang__) || defined(__has_feature) else */
#if defined(__has_cpp_attribute)
#	define SUSUWU_HAS_ATTRIBUTE(X) __has_cpp_attribute(x) /* Usage: `#if SUSUWU_HAS_ATTRIBUTE(noreturn)\nnoreturn\n#endif` */
#else /* defined(__has_cpp_attribute) else */
#	define SUSUWU_HAS_ATTRIBUTE(X) false
#endif /* defined(__has_cpp_attribute) else */

#if defined(SUSUWU_C11) || defined(SUSUWU_CXX11)
#	define SUSUWU_NORETURN [[noreturn]] /* Usage: `SUSUWU_NORETURN void exit();` is close to `void exit() [[ensures:: false]];` or `exit(); SUSUWU_UNREACHABLE;` */ /* TODO? #	if defined(SUSUWU_CXX11) || ((defined __has_cpp_attribute) && __has_cpp_attribute(noreturn)) or [Cmake test for `\[\[noreturn\]\]`](https://stackoverflow.com/a/33517293/24473928) */
#	define SUSUWU_CONSTEXPR constexpr /* Usage: `SUSUWU_CONSTEXPR bool passes(); SUSUWU_STATIC_ASSERT(passes());` is close to `#define PASSES\nSUSUWU_STATIC_ASSERT(PASSES)` */
#else
#	define SUSUWU_NORETURN /* old `g++` "error: 'SUSUWU_NORETURN' does not name a type" / old `clang++` "error: unknown type name 'SUSUWU_NORETURN'" fix */
#	define SUSUWU_CONSTEXPR /* No-op */
#endif /* defined(SUSUWU_C11) || defined(SUSUWU_CXX11) else */

#ifdef USE_CONTRACTS /* Pass `-DUSE_CONTRACTS` once compiler has C++26 (Contracts) */
/* `SUSUWU_EXPECTS(X)` is close to `@pre @code X @endcode` or `SUSUWU_ASSUME(X)` but is for headers; https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p2521r2.html */
/* Promises `(true == (X))`, for static analysis, or for compiler which optimizes this. Warning: `if(!(X)) {UB (undefined behaviour)}` */
#	define SUSUWU_EXPECTS(X) [[expects: X]] /* Usage: `void pushf(std::deque<float> &x, float f) SUSUWU_EXPECTS(!x.full());` */
#	define SUSUWU_ENSURES(X) [[ensures: X]] /* Usage: `void pushf(std::deque<float> &x, float f) SUSUWU_ENSURES(0 != x.size());` */
#else /* else !def USE_CONTRACTS */
#	define SUSUWU_EXPECTS(X) /* `@pre @code X @endcode` */
#	define SUSUWU_ENSURES(X) /* `@post @code X @encode` */
#endif /* else !def USE_CONTRACTS */

