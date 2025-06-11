/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#pragma once
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
/* `clang-tidy` off: NOLINTBEGIN(cppcoreguidelines-macro-usage); https://github.com/SwuduSusuwu/SusuLib/issues/3 is more simple with macros, plus some of the `constexpr` versions require `202002 <= __cplusplus` */
#ifndef SUSUWU_PREFER_C
#	ifdef __cplusplus
#		define SUSUWU_PREFER_C false
#	else
#		define SUSUWU_PREFER_C true
#	endif /* else !def __cplusplus */
#endif /* ndef SUSUWU_PREFER_C */
#if !defined(SUSUWU_SH_PREFER_STDIO)
#	if SUSUWU_PREFER_C
#		define SUSUWU_SH_PREFER_STDIO true
#	else
#		define SUSUWU_SH_PREFER_STDIO false
#	endif /* else !SUSUWU_PREFER_C */
#endif /* !defined(SUSUWU_SH_PREFER_STDIO) */
#if !defined(SUSUWU_SH_PREFER_CSTR)
#	if SUSUWU_PREFER_C
#		define SUSUWU_SH_PREFER_CSTR true
#	else
#		define SUSUWU_SH_PREFER_CSTR false
#	endif /* else !SUSUWU_PREFER_C */
#endif /* !defined(SUSUWU_SH_PREFER_CSTR) */
#if SUSUWU_SH_PREFER_STDIO /* `-DSUSUWU_SH_PREFER_STDIO` to force this. Replaces `std::cXXX << x << std::endl;` with `fprintf(stdXXX, "%s\n", x);` */
#	include <stdio.h> /* fprintf stderr stdout */ /* NOLINT (misc-include-cleaner): those are used if `SUSUWU_STDERR` is used */
#else
#	include <iostream> /* std::cerr std::cout std::endl */ /* NOLINT (misc-include-cleaner): those are used if `SUSUWU_CERR` is used */
#endif

#ifndef SUSUWU_DEFAULT_BRANCH
#	define SUSUWU_DEFAULT_BRANCH "trunk"
#endif /* ndef SUSUWU_DEFAULT_BRANCH */
#ifdef SUSUWU_EXPERIMENTAL
#	define SUSUWU_EXPERIMENTAL_ISSUES "was built with `-DSUSUWU_EXPERIMENTAL`; if you experience issues, execute `git switch trunk && ./build.sh` (as possible fixup), or [post the issue](https://github.com/SwuduSusuwu/SusuLib/issues/new)"
//#	define SUSUWU_EXPERIMENTAL_ISSUES "was built with `" SUSUWU_SH_GREEN " -DSUSUWU_EXPERIMENTAL " SUSUWU_SH_WHITE " `; if you experience issues, execute `" SUSUWU_SH_LIGHT_BROWN " git switch " SUSUWU_SH_LIGHT_PURPLE SUSUWU_DEFAULT_BRANCH SUSUWU_SH_WHITE " && ./build.sh" SUSUWU_SH_WHITE "` (as possible fixup), or [post the issue](https://github.com/SwuduSusuwu/SusuLib/issues/new)" /* TODO: fix `clang-tidy`'s "error: expected ')'" (which triggers on all text macros). */
#else /* def SUSUWU_EXPERIMENTAL else */
#	define SUSUWU_EXPERIMENTAL_ISSUES ""
#endif /* def SUSUWU_EXPERIMENTAL else */

#define SUSUWU_NOOP /* No-op. Skip. */
#ifndef SUSUWU_UNIT_TESTS /* if not set with `-DSUSUWU_UNIT_TESTS=true` (or `=false`) */
#	define SUSUWU_UNIT_TESTS true /* more stable future version could have default = `!defined(NDEBUG)` */
#endif /* ndef SUSUWU_UNIT_TESTS */

#ifndef SUSUWU_OPENMP /* if no environment flag to use */
#	if defined(_OPENMP /* `-fopenmp` */) || (defined(__has_include) && __has_include(<omp.h>) /* `clang++` */)
#		define SUSUWU_OPENMP true /* supports `#pragma omp <directive>` */
#	else /* !(defined(_OPENMP) || (defined(__has_include) && __has_include(<omp.h>))) */
#		define SUSUWU_OPENMP false /* `#pragma omp <directive>` can trigger `[-Wunknown-pragma]` */
#	endif /* !(defined(_OPENMP) || (defined(__has_include) && __has_include(<omp.h>))) */
#endif /* ndef SUSUWU_OPENMP */
#if SUSUWU_OPENMP
#	define SUSUWU_OMP_PRAGMA(x) _Pragma(#x)
#else /* else !SUSUWU_OPENMP */
#	define SUSUWU_OMP_PRAGMA(x) SUSUWU_NOOP
#endif /* else !SUSUWU_OPENMP */

#ifdef __cplusplus
#	include <cassert> /* assert static_assert */
#	define SUSUWU_IF_CPLUSPLUS(TRUE, FALSE) TRUE
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
#	define SUSUWU_IF_CPLUSPLUS(TRUE, FALSE) FALSE /* [Issue #3 (`CC` support) uses this](https://github.com/SwuduSusuwu/SusuLib/issues/3) */
#	if (199901 <= __STDC_VERSION__)
#		define SUSUWU_C99
#	endif /* (199901 <= __STDC_VERSION__) */
#	if (201112 <= __STDC_VERSION__)
#		define SUSUWU_C11
#	endif /* (201112 <= __STDC_VERSION__) */
#endif /* !(defined __cplusplus */

#ifndef SUSUWU_INHERIT_GSL_OWNER /* If have no `-DSUSUWU_INHERIT_GSL_OWNER` value */
#	ifdef __cpp_lib_gsl /* If `gsl::owner` is included */
#		define SUSUWU_INHERIT_GSL_OWNER true
#	else /* else !def __cpp_lib_gsl */
#		define SUSUWU_INHERIT_GSL_OWNER false
#	endif /* !def __cpp_lib_gsl */
#endif /* ndef SUSUWU_INHERIT_GSL_OWNER */

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
#	define SUSUWU_NORETURN [[noreturn]] /* Usage: `SUSUWU_NORETURN void exit();` is close to `void exit() [[ensures:: false]];` or `exit(); SUSUWU_UNREACHABLE;` */ /* TODO? || SUSUWU_HAS_ATTRIBUTE(noreturn) or [Cmake test for `\[\[noreturn\]\]`](https://stackoverflow.com/a/33517293/24473928) */
#	define SUSUWU_CONSTEXPR constexpr /* Usage: `SUSUWU_CONSTEXPR bool passes(); SUSUWU_STATIC_ASSERT(passes());` is close to `#define PASSES\nSUSUWU_STATIC_ASSERT(PASSES)` */
#else
#	define SUSUWU_NORETURN /* No-op; old `g++` "error: 'SUSUWU_NORETURN' does not name a type" / old `clang++` "error: unknown type name 'SUSUWU_NORETURN'" fix */
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
#	define SUSUWU_DEFAULT = default; /* Usage: `Macros.cxx` has tests for this */
#	define SUSUWU_DELETE = delete; /* Usage: `Macros.cxx` has tests for this */
#	define SUSUWU_FINAL final /* Usage: `Macros.cxx` has tests for this */
#	define SUSUWU_NULLPTR nullptr /* Usage: `Macros.cxx` has tests for this */
#	define SUSUWU_OVERRIDE override /* Usage: `Macros.cxx` has tests for this */
		/* Usage 2: `void versionInfo() SUSUWU_NOEXCEPT(std::is_nothrow_constructible<U>::value); {versionInfo();}` is close to `{try {versionInfo();} catch(...) {if(std::is_nothrow_constructible<U>::value) {SUSUWU_UNREACHABLE;}}}` */
#else /* SUSUWU_CXX11 else */
#	define SUSUWU_NOEXCEPT /* No-op: "error: expected function body after function declarator" fix */
#	define SUSUWU_DEFAULT {} /* allows default constructors/destructors. TODO: default operators? */
#	define SUSUWU_DELETE ; /* causes linker error if DELETEd function is called. */
#	define SUSUWU_FINAL /* No-op */
#	define SUSUWU_NULLPTR NULL /* fallback to C-style macro for `0`. */
#	define SUSUWU_OVERRIDE /* No-op */
#endif /* SUSUWU_CXX11 else */

/* `SUSUWU_UNREACHABLE` is close to `SUSUWU_ASSUME(false)` */
#if !defined(NDEBUG_)
/* <https://stackoverflow.com/questions/2249282/c-c-portable-way-to-detect-debug-release> <https://stackoverflow.com/questions/2290509/debug-vs-ndebug> */
/* Debug: Promises unreachable, for static analysis */
#	define SUSUWU_UNREACHABLE assert(false && "UNREACHABLE") /* TODO: NOLINT(cert-dcl03-c,hicpp-static-assert,misc-static-assert): `static_assert` does not allow false, not even in unreachable code paths */
#else
#	include <version> /* __cpp_lib_unreachable */ /* <https://en.cppreference.com/w/cpp/feature_test> */
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
#if SUSUWU_UNIT_TESTS
const int macrosTestsNoexcept() SUSUWU_NOEXCEPT;
#endif /* SUSUWU_UNIT_TESTS */

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
#elif !defined(SUSUWU_SH_VERBOSE)
#	define SUSUWU_SH_VERBOSE false /* can disable on `--debug` with `-DSUSUWU_SH_VERBOSE=false` */
#endif /* else defined(SUSUWU_SH_VERBOSE) */

#if !defined(SUSUWU_SH_SKIP_BRACKETS) || SUSUWU_SH_SKIP_BRACKETS == false /* overridable with `-DSUSUWU_SH_SKIP_BRACKETS=true` (which you can set to mimic `g++`/`clang++` syntax for outputs) */
#	define IF_SUSUWU_SH_BRACKETS(TRUE, FALSE) TRUE
#else
#	define IF_SUSUWU_SH_BRACKETS(TRUE, FALSE) FALSE
#endif

#if (!defined(SUSUWU_SH_FILE) && SUSUWU_SH_VERBOSE) || SUSUWU_SH_FILE /* overridable with `-DSUSUWU_SH_FILE=[true/false]` */
#	define SUSUWU_SH_USE_FILE /* affix `__FILE__ ":"` to `stderr`/`cerr` printout */
#endif
#if (!defined(SUSUWU_SH_LINE) && SUSUWU_SH_VERBOSE) || SUSUWU_SH_LINE /* overridable with `-DSUSUWU_SH_LINE=[true/false]` */
#	define SUSUWU_SH_USE_LINE /* affix `__LINE__ ":"` to `stderr`/`cerr` printout */
#endif
#if defined(SUSUWU_SH_FUNC) && SUSUWU_SH_FUNC /* overridable with `-DSUSUWU_SH_FUNC=[true|false]` */
#	define SUSUWU_SH_USE_FUNC /* affix `__func__ ":"` to `stderr`/`cerr` printout */
#endif
#if defined(SUSUWU_SH_USE_FUNC) && !defined(SUSUWU_C99) && !defined(SUSUWU_CXX11) /* no `__func__` support */
#	pragma message("[Warning: `-DSUSUWU_SH_FUNC=true` was undone (no `__func__`, due to `!defined(SUSUWU_C99) && !defined(SUSUWU_CXX11)`.]")
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
#	pragma message("[Info: `-DSUSUWU_SH_RUNTIME_COLORS` is TODO; https://github.com/SwuduSusuwu/SusuLib/issues/17 to contribute]")
#endif /* #elif !defined(SUSUWU_POSIX) TODO */
#ifdef SUSUWU_SH_RUNTIME_OSC
#	pragma message("[Info: `-DSUSUWU_SH_RUNTIME_OSC` is TODO; https://github.com/SwuduSusuwu/SusuLib/issues/17 to contribute]")
#endif /* #elif !defined(SUSUWU_POSIX) TODO */
#if !defined(SUSUWU_POSIX)
#	define SUSUWU_SH_COLORS_UNSUPPORTED /* assume "dumb terminals" (such as SUSUWU_WIN32 often has) if built without runtime tests for color attributes, on non-POSIX systems */
#	define SUSUWU_SH_OSC_UNSUPPORTED /* assume "dumb terminals" (such as SUSUWU_WIN32 often has) if built without runtime tests for Operating System Commands, on non-POSIX systems */
#endif
#if defined(SUSUWU_SH_OSC_UNSUPPORTED) && !defined(SUSUWU_SH_SKIP_OSC)
#	define SUSUWU_SH_SKIP_OSC true /* you can use `-DSUSUWU_SH_SKIP_OSC=false` to force unsupported Operating System Command use (such as if build is SUSUWU_WIN32 but you assume Win10+ `xterm` support) */
#endif /* SUSUWU_SH_OSC_UNSUPPORTED && !defined SUSUWU_SH_SKIP_OSC */
#ifdef SUSUWU_SH_SKIP_OSC /* `-DSUSUWU_SH_SKIP_OSC=true` to turn Operating System Commands off */
#	define SUSUWU_SH_TO_CLIPBOARD_PREFIX ""
#	define SUSUWU_SH_TO_CLIPBOARD_SUFFIX ""
#	define SUSUWU_SH_TO_CLIPBOARD(base64_str) SUSUWU_NOOP
#else /* def SUSUWU_SH_SKIP_OSC else */
#	define SUSUWU_SH_TO_CLIPBOARD_PREFIX SUSUWU_SH_OSC "52;c;" /* Command to put BASE64 str into clipboard */
#	define SUSUWU_SH_TO_CLIPBOARD_SUFFIX "\a"
#	define SUSUWU_SH_TO_CLIPBOARD(base64_str) fprintf(stdout, SUSUWU_SH_TO_CLIPBOARD_PREFIX "%s" SUSUWU_SH_TO_CLIPBOARD_SUFFIX, SUSUWU_IF_CPLUSPLUS(std::string(base64_str).c_str(), base64_str)); /* TODO: `std::cout` version of this */
#endif /* def SUSUWU_SH_SKIP_OSC else */
#if defined(SUSUWU_SH_COLORS_UNSUPPORTED) && !defined(SUSUWU_SH_SKIP_COLORS)
#	define SUSUWU_SH_SKIP_COLORS true /* you can use `-DSUSUWU_SH_SKIP_COLORS=false` to force unsupported color use (such as if build is for SUSUWU_WIN32 but you assume Win10+ `xterm` support) */
#endif /* SUSUWU_SH_COLORS_UNSUPPORTED && !defined SUSUWU_SH_SKIP_COLORS */

#define SUSUWU_SH_BEL "\07" /* Bell sound */
#define SUSUWU_SH_ESC "\033" /* Escape */
#define SUSUWU_SH_OSC SUSUWU_SH_ESC "]" /* [Operating System Command](https://wikipedia.org/wiki/ANSI_escape_code#Operating_System_Command_sequences) */
#define SUSUWU_SH_ST SUSUWU_SH_ESC "\\" /* String Terminator of commands */ /* `BEL` reduces this to 1 byte, but just functions as `ST` on `xterm`. */
#define SUSUWU_SH_CSI SUSUWU_SH_ESC "[" /* [Control Sequence Introducer](https://wikipedia.org/wiki/ANSI_escape_code#Control_Sequence_Introducer_commands) */
/* `SUSUWU_SH_<color>`. Notice: update [sh/Macros.sh](sh/Macros.sh) if you update those. */
/* Usage: `SUSUWU_PRINT(SUSUWU_SH_<warn-level>, ${SUSUWU_SH_<color>} "<message>" ${SUSUWU_SH_DEFAULT})`. */
#if SUSUWU_SH_SKIP_COLORS /* `-DSUSUWU_SH_SKIP_COLORS=true` to turn colors off. */
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
#define SUSUWU_SH_POSTFIX IF_SUSUWU_SH_BRACKETS("]", "")

/* `SUSUWU_SH_<warn-level>`. Notice: update [sh/Macros.sh](sh/Macros.sh) if you update those. */
/* Usage: `SUSUWU_PRINT(SUSUWU_SH_<warn-level>, "<message>")`. */
#define SUSUWU_SH_ERROR SUSUWU_SH_RED "Error: " SUSUWU_SH_WHITE
#define SUSUWU_SH_WARNING SUSUWU_SH_PURPLE "Warning: " SUSUWU_SH_WHITE
#define SUSUWU_SH_INFO SUSUWU_SH_CYAN "Info: " SUSUWU_SH_WHITE
#define SUSUWU_SH_SUCCESS SUSUWU_SH_GREEN "Success: " SUSUWU_SH_WHITE
#define SUSUWU_SH_NOTICE SUSUWU_SH_BLUE "Notice: " SUSUWU_SH_WHITE
#define SUSUWU_SH_DEBUG SUSUWU_SH_BLUE "Debug: " SUSUWU_SH_WHITE

#define SUSUWU_ERRSTR_IMP(WARN_LEVEL, x) std::string(WARN_LEVEL) + std::string(x) + std::string(SUSUWU_SH_DEFAULT)
#define SUSUWU_CERR_IMP(WARN_LEVEL, x) WARN_LEVEL << (x) << SUSUWU_SH_DEFAULT
#define SUSUWU_STDERR_IMP(WARN_LEVEL, prefix, postfix, x, ... /* must pass SUSUWU_COMMA after __VA_ARGS__ params */) fprintf(stderr, prefix WARN_LEVEL "%s" SUSUWU_SH_DEFAULT postfix, __VA_ARGS__ SUSUWU_IF_CPLUSPLUS(std::string(x).c_str(), x))

/* Usage: `throw std::logic_error(SUSUWU_ERRSTR(SUSUWU_SH_<warn-level>, "<message>"));`. */
#define SUSUWU_ERRSTR(WARN_LEVEL, x) std::string(SUSUWU_SH_PREFIX) IF_SUSUWU_SH_FILE(+ SUSUWU_SH_FILE) IF_SUSUWU_SH_LINE(+ std::to_string(__LINE__) + ':') IF_SUSUWU_SH_FUNC(+ std::string(__func__) + ':') IF_SUSUWU_SH_FILE_LINE_OR_FUNC(+ ' ') + SUSUWU_ERRSTR_IMP(WARN_LEVEL, x) + SUSUWU_SH_POSTFIX
/* Usage: `SUSUWU_CERR(SUSUWU_SH_<warn-level>, "<message>");`. */
#define SUSUWU_CERR(WARN_LEVEL, x) std::cerr << SUSUWU_SH_PREFIX IF_SUSUWU_SH_FILE(<< std::string(SUSUWU_SH_FILE)) IF_SUSUWU_SH_LINE(<< std::to_string(__LINE__) << ":") IF_SUSUWU_SH_FUNC(<< std::string(__func__) << ":") IF_SUSUWU_SH_FILE_LINE_OR_FUNC(<< ' ') << SUSUWU_CERR_IMP(WARN_LEVEL, x) << SUSUWU_SH_POSTFIX << std::endl
/* Usage: `SUSUWU_STDERR(SUSUWU_SH_<warn-level>, "<message>");`. */
#define SUSUWU_STDERR(WARN_LEVEL, x) SUSUWU_STDERR_IMP(WARN_LEVEL, SUSUWU_SH_PREFIX IF_SUSUWU_SH_FILE(SUSUWU_SH_FILE) IF_SUSUWU_SH_LINE("%i:") IF_SUSUWU_SH_FUNC("%s:") IF_SUSUWU_SH_FILE_LINE_OR_FUNC(" "), SUSUWU_SH_POSTFIX "\n", x, IF_SUSUWU_SH_LINE(__LINE__ SUSUWU_COMMA) IF_SUSUWU_SH_FUNC(__func__ SUSUWU_COMMA))

/* Usage: `SUSUWU_PRINT(SUSUWU_SH_<warn-level>, "<message>")`, to do C versus C++ agnostic code. */
#if SUSUWU_SH_PREFER_STDIO
#	define SUSUWU_PRINT(WARN_LEVEL, x) SUSUWU_STDERR(WARN_LEVEL, x)
#else
#	define SUSUWU_PRINT(WARN_LEVEL, x) SUSUWU_CERR(WARN_LEVEL, x)
#endif /* else !SUSUWU_SH_PREFER_STDIO */

#ifdef SUSUWU_EXPERIMENTAL
#	define SUSUWU_ERROR(x) {SUSUWU_PRINT(SUSUWU_SH_ERROR, x); SUSUWU_WARNING("`$0` " SUSUWU_EXPERIMENTAL_ISSUES);}
#else /* SUSUWU_EXPERIMENTAL else */
#	define SUSUWU_ERROR(x) SUSUWU_PRINT(SUSUWU_SH_ERROR, x)
#endif /* SUSUWU_EXPERIMENTAL else */
#define SUSUWU_WARNING(x) SUSUWU_PRINT(SUSUWU_SH_WARNING, x)
#define SUSUWU_INFO(x) SUSUWU_PRINT(SUSUWU_SH_INFO, x)
#define SUSUWU_SUCCESS(x) SUSUWU_PRINT(SUSUWU_SH_SUCCESS, x)

/* Use this to just print debug/notices to `--debug` builds (+ do conditional execution). */
#if SUSUWU_SH_VERBOSE
#	define SUSUWU_NOTICE(x) SUSUWU_PRINT(SUSUWU_SH_NOTICE, x)
#	define SUSUWU_DEBUG(x) SUSUWU_PRINT(SUSUWU_SH_DEBUG, x)
#	define SUSUWU_EXECUTEVERBOSE(x) x /* about side-effects; do not assume that `--debug` was used. `--release -DSUSUWU_SH_VERBOSE=true` will execute this. */
#else /* else SUSUWU_SH_VERBOSE */
#	define SUSUWU_NOTICE(x) SUSUWU_NOOP
#	define SUSUWU_DEBUG(x) SUSUWU_NOOP
#	define SUSUWU_EXECUTEVERBOSE(x) SUSUWU_NOOP /* about side-effects; do not assume that just `--release` reaches this; `--debug -DSUSUWU_SH_VERBOSE=false` uses this too. */
#endif /* else SUSUWU_SH_VERBOSE */

/* Use this to reduce print (NOTICE/DEBUG is conditional) + (unconditional) execute into single statement. */
#define SUSUWU_ERROR_EXECUTE(x) ((SUSUWU_ERROR(#x)), (x))
#define SUSUWU_WARNING_EXECUTE(x) ((SUSUWU_WARNING(#x)), (x))
#define SUSUWU_INFO_EXECUTE(x) ((SUSUWU_INFO(#x)), (x))
#define SUSUWU_SUCCESS_EXECUTE(x) ((SUSUWU_SUCCESS(#x)), (x))
#define SUSUWU_NOTICE_EXECUTE(x) ((SUSUWU_NOTICE(#x)), (x))
#define SUSUWU_DEBUG_EXECUTE(x) ((SUSUWU_DEBUG(#x)), (x))

/* Use this to reduce (conditional) print + (conditional) execute into single statement. */
#define SUSUWU_NOTICE_EXECUTEVERBOSE(x) SUSUWU_EXECUTEVERBOSE(((SUSUWU_NOTICE(#x)), x))
#define SUSUWU_DEBUG_EXECUTEVERBOSE(x) SUSUWU_EXECUTEVERBOSE((SUSUWU_DEBUG(#x), x))
#ifdef __cplusplus
}; /* namespace Susuwu */
#endif /* ifdef __cplusplus */
/* `clang-tidy` on: NOLINTEND(cppcoreguidelines-macro-usage) */
#endif /* ndef INCLUDES_cxx_Macros_hxx */

