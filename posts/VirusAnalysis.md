**Howto: use local static analysis + heuristics + sandboxes + artificial CNS (central nervous systems) to secure computers; virus analysis tools improved**
\[This post [from _SubStack_](https://swudususuwu.substack.com/p/howto-improve-virus-analysis) allows [all uses](https://creativecommons.org/licenses/by/2.0/).\]

# Table of Contents
- [Intro](#intro)
- [Source code](#source-code)
- [Comparison to assistants](#comparison-to-assistants)
- [Post, with resources](#post-with-resources)
  - [Neural resources](#neural-resources)
- [Synopsis + related posts](#synopsis--related-posts)
- [Sponsor this](../README.md#sponsor)

# Intro
Static analysis + sandbox + CNS = 1 second (approx) analysis of **new executables** (secures all app launches,) but after first launch: **caches** reduce this to **less than 1ms** (just cost to compute `caches.at(classSha2(ClassIoBytecode()))`, where `caches` is `std::map<ResultListHash, VirusAnalysisResult>` or `ResultList::hashes`).

[`../README.md`](../README.md) has how to use this (what follows is more of a book of source code).

(Removed duplicate licenses, `#if` guards, `#include`s, `namespace`s (except files with >=2 `namespace`s), `NOLINTBEGIN`s, `NOLINTEND`s from all except `main.*xx`; follow URLs for whole sources.)

For the most new sources (+ static libs), use apps such as [_iSH_](https://apps.apple.com/us/app/ish-shell/id1436902243) (for _iOS_) or [_Termux_](https://play.google.com/store/apps/details?id=com.termux) (for _Android OS_) to run this:
`git clone https://github.com/SwuduSusuwu/SusuLib.git && cd ./SusuLib/ && ./build.sh`

To test certificates, view [this post](https://swudususuwu.substack.com/p/githubcomswudususuwusubstack-certificate-new).

To improve how fast the whole program executes; [set relevent environment flags (such as `CXXFLAGS`) to enable SIMD](./SimdGpgpuTpu.md#simd-single-instruction-multiple-data) and _OpenMP_.

To improve how fast backpropagation (ergo "training"; such as `Cns::setupSynapses()`, which {`produceAnalysisCns()`, `produceVirusFixCns()`} use) and forwardpropagation (ergo "inference"; such as `Cns::processTo*()`, which {`cnsAnalysisScore()` and `cnsVirusFix()`} use) executes, [use **TPU**s](./SimdGpgpuTpu.md#tpus-tensor-processor-units) and/or implement `class Cns` with [_TensorFlow_'s `MapReduce`](./SimdGpgpuTpu.md#synopsis--related-posts).

# Source code
(C) 2024 Swudu Susuwu, dual licenses: choose [_GPLv2_](../LICENSE_GPLv2) or [_Apache 2_](../LICENSE), allows all uses.

`less `[`cxx/Macros.hxx`](../cxx/Macros.hxx) #Removed: disabled [color codes](https://en.wikipedia.org/wiki/C0_and_C1_control_codes#C0_controls) + unused [**OSC**](https://en.wikipedia.org/wiki/C0_and_C1_control_codes#C1_controls) codes
```c++
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
#	include <stdio.h> /* fprintf stderr stdout */
#else
#	include <iostream> /* std::cerr std::cout std::endl */
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
#	include SUSUWU_IF_CPLUSPLUS(<cstdint>, <stdint.h>) /* intptr_t */ /* NOLINT(misc-include-cleaner): this is used if `SUSUWU_INTPTR` is used */
#	define SUSUWU_INTPTR intptr_t
#else
#	define SUSUWU_NORETURN /* old `g++` "error: 'SUSUWU_NORETURN' does not name a type" / old `clang++` "error: unknown type name 'SUSUWU_NORETURN'" fix */
#	define SUSUWU_CONSTEXPR /* No-op */
#	include SUSUWU_IF_CPLUSPLUS(<cstddef>, <stddef.h>) /* size_t */
#	define SUSUWU_INTPTR size_t /* is supposed to hold a positive pointer (a memory address), and `size_t` can hold all positive memory address offsets. */
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
#	define SUSUWU_NOEXCEPT noexcept /* Usage: `void info() SUSUWU_NOEXCEPT; ... { info(); }` is close to `void versionInfo() [[ensures: true]]; ... { info(); }` or `{ try { versionInfo(); } catch(...) { SUSUWU_UNREACHABLE; } } */
#	define SUSUWU_DEFAULT = default; /* Usage: `Macros.cxx` has tests for this */
#	define SUSUWU_DELETE = delete; /* Usage: `Macros.cxx` has tests for this */
#	define SUSUWU_FINAL final /* Usage: `Macros.cxx` has tests for this */
#	define SUSUWU_NULLPTR nullptr /* Usage: `Macros.cxx` has tests for this */
#	define SUSUWU_OVERRIDE override /* Usage: `Macros.cxx` has tests for this */
		/* Usage 2: `void versionInfo() SUSUWU_NOEXCEPT(std::is_nothrow_constructible<U>::value); { versionInfo(); }` is close to `{try { versionInfo(); } catch(...) {if(std::is_nothrow_constructible<U>::value) { SUSUWU_UNREACHABLE; } } }` */
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

#if (!defined(SUSUWU_SH_FILE) && SUSUWU_SH_VERBOSE) || SUSUWU_SH_FILE /* overridable with `-DSUSUWU_SH_FILE=[true|false]` */
#	define SUSUWU_SH_USE_FILE /* affix `__FILE__ ":"` to `stderr`/`cerr` printout */
#endif
#if (!defined(SUSUWU_SH_LINE) && SUSUWU_SH_VERBOSE) || SUSUWU_SH_LINE /* overridable with `-DSUSUWU_SH_LINE=[true|false]` */
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

#define SUSUWU_SH_ESC "\033" /* Escape */
#define SUSUWU_SH_OSC SUSUWU_SH_ESC "]" /* [Operating System Command](https://wikipedia.org/wiki/ANSI_escape_code#Operating_System_Command_sequences) */
#define SUSUWU_SH_ST SUSUWU_SH_ESC "\\" /* String Terminator of commands */ /* `BEL` reduces this to 1 byte, but just functions as `ST` on `xterm`. */
#define SUSUWU_SH_CSI SUSUWU_SH_ESC "[" /* [Control Sequence Introducer](https://wikipedia.org/wiki/ANSI_escape_code#Control_Sequence_Introducer_commands) */
/* `SUSUWU_SH_<color>`. Notice: update [sh/Macros.sh](sh/Macros.sh) if you update those. */
/* Usage: `SUSUWU_PRINT(SUSUWU_SH_<warn-level>, ${SUSUWU_SH_<color>} "<message>" ${SUSUWU_SH_DEFAULT})`. */
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

/* Usage: `SUSUWU_PRINT(SUSUWU_SH_<warn-level>, "<message>");`, to do C versus C++ agnostic code. */
#if SUSUWU_SH_PREFER_STDIO
#	define SUSUWU_PRINT(LEVEL, x) SUSUWU_STDERR(LEVEL, x)
#else
#	define SUSUWU_PRINT(LEVEL, x) SUSUWU_CERR(LEVEL, x)
#endif /* else !SUSUWU_SH_PREFER_STDIO */

#ifdef SUSUWU_EXPERIMENTAL
#	define SUSUWU_ERROR(x) { SUSUWU_PRINT(SUSUWU_SH_ERROR, x); SUSUWU_WARNING("`$0` " SUSUWU_EXPERIMENTAL_ISSUES); }
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
```

`less `[`cxx/Macros.cxx`](../cxx/Macros.cxx)
```c++
#if SUSUWU_UNIT_TESTS /* `cxx/Macros.cxx` is just unit tests. `Macros.hxx` has all which has actual use. */
#ifdef SUSUWU_CXX11
SUSUWU_CONSTEXPR static const bool MacrosCxx11NullptrTest(decltype(       NULL)    /* `int`    */) { return false; }
SUSUWU_CONSTEXPR static const bool MacrosCxx11NullptrTest(decltype(SUSUWU_NULLPTR) /* `void *` */) { return true; }
SUSUWU_STATIC_ASSERT(true  == MacrosCxx11NullptrTest(SUSUWU_NULLPTR)); /* Tests that `nullptr` is `void *` */
#endif /* def SUSUWU_CXX11 */
class MacrosCxx11InheritanceTest {
public:
	MacrosCxx11InheritanceTest() SUSUWU_DELETE /* Notice: deleted constructor is redundant for pure virtual class */
	MacrosCxx11InheritanceTest(const MacrosCxx11InheritanceTest &) SUSUWU_DEFAULT /* Default clone constructor */
	virtual ~MacrosCxx11InheritanceTest() SUSUWU_DEFAULT /* Notice: destructor has implementation; reimplementation is optional. */
	virtual const bool PureVirtual() const = 0; /* Notice: this is pure virtual; subclasses must implement this. */
};
class MacrosCxx11InheritanceTestSubclass : MacrosCxx11InheritanceTest {
public: /* Notice: inherits default constructor */
	const bool PureVirtual() const SUSUWU_OVERRIDE { return true; }; /* Notice: `final` is not allowed since this has a subclass. `override` is optional. */
};
class MacrosCxx11InheritanceTestSubclass2 SUSUWU_FINAL /* Since destructor is final, cannot inherit from class. `clang++` thus warns, unless the whole class is `final`. */: MacrosCxx11InheritanceTestSubclass {
public: /* Notice: inherits `PureVirtual()`, which you can now set `final` */
	~MacrosCxx11InheritanceTestSubclass2() SUSUWU_FINAL SUSUWU_DEFAULT /* Notice: `override`/`final` is optional. */
};
static void macrosNoUniqueAddressTest() {
	typedef class Zero {} Zero;
	class SubClassWithBaseSubobject : public Zero { public: bool boo = true; };
	class SubClassWithMemberSubobject { public: bool boo = true; Zero zero; };
	class SubClassWithMemberSubobjectNoAddress { public: bool boo = true; SUSUWU_NO_UNIQUE_ADDRESS Zero zero; };
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
#endif /* SUSUWU_UNIT_TESTS */
```

`less `[`cxx/ClassObject.hxx`](../cxx/ClassObject.hxx)
```c++
/* Gives: `Susuwu::Class` (a C++ port of [`java.lang.Class`](https://docs.oracle.com/javase/8/docs/api/java/lang/Class.html)),
 * plus `Susuwu::Object` (a C++ port of [Java's `Object`](https://docs.oracle.com/javase/8/docs/api/java/lang/Object.html) [superclass](https://docs.oracle.com/javase%2Ftutorial%2F/java/IandI/objectclass.html)),
* to [assist future Java ports](https://github.com/SwuduSusuwu/SusuLib/issues/10) */
/* Susuwu::Instrumentation` is somewhat analogous to [`java.lang.instrument.Instrumentation` interface](https://docs.oracle.com/javase/8/docs/api/java/lang/instrument/Instrumentation.html). */
namespace Susuwu {
#if SUSUWU_UNIT_TESTS
const bool classObjectTests();
const bool classObjectTestsNoexcept() SUSUWU_NOEXCEPT;
#endif /* SUSUWU_UNIT_TESTS */

typedef enum ObjectMode : char {
	objectModeBool /* binary classification */, objectModeChar, objectModeEnum /* multi-class indices */, objectModeInt, objectModeUint, objectModeFloat, objectModeDouble,
	objectModeVectorBool /* one-hot binary-classification */, objectModeVectorChar, objectModeVectorEnum /* multi-label multi-class indices */, objectModeVectorInt, objectModeVectorUint, objectModeVectorFloat, objectModeVectorDouble,
#if defined(SUSUWU_CXX17) && defined(SUSUWU_PREFER_STRING_VIEW /* TODO */)
	objectModeString = objectModeVectorChar /* std::string == std::vector<char> */
#else /* else !def SUSUWU_CXX17 */
/* https://stackoverflow.com/questions/5115166/how-to-construct-a-stdstring-from-a-stdvectorchar */
	objectModeString
#endif /* def SUSUWU_CXX17 else */
} ObjectMode;
template<class Q>
struct ToObjectMode; /* Usage: `template<class Type>\nObjectMode objectMode = ToObjectMode<Type>::value;` */
// typedef struct ToObjectMode { static ObjectMode value; } ToObjectMode; /* TODO: fix `error: expected ';' after struct` */
template<>
struct ToObjectMode<bool> { static SUSUWU_CONSTEXPR ObjectMode value = objectModeBool; };
template<>
struct ToObjectMode<char> { static SUSUWU_CONSTEXPR ObjectMode value = objectModeChar; };
enum ObjectEnum {};
template<>
struct ToObjectMode<enum ObjectEnum> { static SUSUWU_CONSTEXPR ObjectMode value = objectModeEnum; };
template<>
struct ToObjectMode<int> { static SUSUWU_CONSTEXPR ObjectMode value = objectModeInt; };
template<>
struct ToObjectMode<unsigned int> { static SUSUWU_CONSTEXPR ObjectMode value = objectModeUint; };
template<>
struct ToObjectMode<float> { static SUSUWU_CONSTEXPR ObjectMode value = objectModeFloat; };
template<>
struct ToObjectMode<double> { static SUSUWU_CONSTEXPR ObjectMode value = objectModeDouble; };
template<>
struct ToObjectMode<std::vector<bool>> { static SUSUWU_CONSTEXPR ObjectMode value = objectModeVectorBool; };
template<>
struct ToObjectMode<std::vector<char>> { static SUSUWU_CONSTEXPR ObjectMode value = objectModeVectorChar; };
template<>
struct ToObjectMode<std::vector<enum ObjectEnum>> { static SUSUWU_CONSTEXPR ObjectMode value = objectModeVectorEnum; };
template<>
struct ToObjectMode<std::vector<int>> { static SUSUWU_CONSTEXPR ObjectMode value = objectModeVectorInt; };
template<>
struct ToObjectMode<std::vector<unsigned int>> { static SUSUWU_CONSTEXPR ObjectMode value = objectModeVectorUint; };
template<>
struct ToObjectMode<std::vector<float>> { static SUSUWU_CONSTEXPR ObjectMode value = objectModeVectorFloat; };
template<>
struct ToObjectMode<std::vector<double>> { static SUSUWU_CONSTEXPR ObjectMode value = objectModeVectorDouble; };
template<>
struct ToObjectMode<std::string> { static SUSUWU_CONSTEXPR ObjectMode value = objectModeString; };

typedef class Instrumentation { /* Produced this unaware of `Instrumentation`. TODO: match `Instrumentation` protocols (as `getObjectSize()` does). For now, this is just whatever run-time type information/reflection which does not map to `java.lang.Class`. */
public:
	virtual ~Instrumentation() SUSUWU_DEFAULT /* allows subclasses to release resources */
	virtual const size_t getDynamicSize() const { return 0; /* sum({`S` for `malloc(S)`, `sizeof(S)` for `new S`}) */ }
	virtual const size_t getObjectSize() const { return sizeof(*this); /* sum(sizeof({vptr, data members})) */ }
	virtual const bool hasExtraVirtuals() const { /* if(has `virtual`s other than `Object`'s) { return true; }*/ return false; }
#define SUSUWU_INSTRUMENTATION_ISPUREVIRTUAL(SUBCLASS) const bool isPureVirtual() const SUSUWU_OVERRIDE { return false; }
#define SUSUWU_INSTRUMENTATION_ISPUREVIRTUAL_PURE_VIRTUAL(SUBCLASS) const bool isPureVirtual() const SUSUWU_OVERRIDE { return typeid(SUBCLASS) == typeid(this); }
	virtual const bool isPureVirtual() const { return false; } /* in case the compiler does not recognize such classes as "pure virtual", use `SUSUWU_INSTRUMENTATION_ISPUREVIRTUAL_PURE_VIRTUAL(SubClass)` for `SubClass` which does not implement all virtuals (or which has broken implementations of virtuals) */
	virtual const bool isInitialized() const { return true; } /* override this if the constructor does not produce objects ready to use */
	virtual const bool isPlainOldData() const { return !(usesDynamicAllocations() || usesFilesystem() || usesNetwork() || usesThreads()); } /* whether or not to allow shallow clones */
	virtual const bool usesDynamicAllocations() const { /* if(uses functions suchas {`malloc`, `new`}) { return true; } */ return false; }
	virtual const bool usesFilesystem() const { /* if(uses functions suchas {`fopen`, `new`}) { return true; } */ return false; }
	virtual const bool usesNetwork() const { /* if(uses functions suchas {`socket`}) { return true; } */ return false; }
	virtual const bool usesThreads() const { /* if(uses functions suchas {`pthread`}) { return true; } */ return false; }
} Instrumentation;

#ifndef SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES /* override with `-DSUSUWU_VIRTUAL_EQUALS_USE_ADDRESES=false` */
#	define SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES true /* Default is: Interpret `Java`'s `Object::equals` standard as "Addresses must match". */
#endif /* ndef SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES */
#define SUSUWU_VIRTUAL_OPERATORS_USE_ADDRESSES false /* TODO: inherit from `SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES`? */
#define SUSUWU_VPTR_SZ sizeof(SUSUWU_NULLPTR) /* TODO: use documented constant for `vptr` byte count. */
#define SUSUWU_VPTR_OFFSET SUSUWU_VPTR_SZ /* Notice: assumes that first data member is `vptr`. */
#ifndef SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS /* override with `-DSUSUWU_VIRTUAL_VPTR_COMPARISON=false` to use `hasLayoutOf()` && data members comparison */
#	define SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS true /* Default: `Class::operator==` does `typeid` && data members comparison */
#endif /* ndef SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS */

#define SUSUWU_VIRTUAL_OPERATOREQUALTO_WITH_VPTR { return typeid(*this) == typeid(obj) && 0 == memcmp(reinterpret_cast<const void *>(this), reinterpret_cast<const void *>(&obj), this->getObjectSize()); } /* cast silences "warning: first operand of this 'memcmp' call is a pointer to dynamic class 'Object'; vtable pointer will be compared [-Wdynamic-class-memaccess]" */
#define SUSUWU_VIRTUAL_OPERATOREQUALTO_WITHOUT_VPTR { return (this->hasLayoutOf(obj) && 0 == memcmp(reinterpret_cast<const char *>(this) + SUSUWU_VPTR_OFFSET, reinterpret_cast<const char *>(&obj) + SUSUWU_VPTR_OFFSET, this->getObjectSize() - SUSUWU_VPTR_SZ)); } /* NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic) */

#ifdef SUSUWU_CXX20
#	define SUSUWU_VIRTUAL_OPERATOREQUALTO_DEFAULT SUSUWU_DEFAULT
#else /* !C++20 */
#	define SUSUWU_VIRTUAL_OPERATOREQUALTO_DEFAULT SUSUWU_VIRTUAL_OPERATOREQUALTO_WITH_VPTR
#endif /* !C++20 */
#define SUSUWU_SUBCLASS_OPERATOREQUALTO(SUBCLASS) bool operator==(const SUBCLASS &obj) const SUSUWU_VIRTUAL_OPERATOREQUALTO_DEFAULT /* Usage: `SUSUWU_SUBCLASS_OPERATOREQUALTO(SubClass);`, for all `SubClass : public Class` or `SubClass : public Object` which do not have their own `operator==(const SubClass &)` */

#define SUSUWU_VIRTUAL_ virtual /* Is used to mark base class functions `virtual`. */
#define SUSUWU_CLASS_OVERRIDE /* For base class functions is no-op. */
#if SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS
#	define SUSUWU_CLASS_OPERATOREQUALTO(SUBCLASS) SUSUWU_VIRTUAL_ bool operator==(const Class &obj) const SUSUWU_CLASS_OVERRIDE SUSUWU_VIRTUAL_OPERATOREQUALTO_WITH_VPTR
#else /* else !SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS */
#	define SUSUWU_CLASS_OPERATOREQUALTO(SUBCLASS) SUSUWU_VIRTUAL_ bool operator==(const Class &obj) const SUSUWU_CLASS_OVERRIDE SUSUWU_VIRTUAL_OPERATOREQUALTO_WITHOUT_VPTR
#endif /* else !SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS */

/* NOLINTBEGIN(hicpp-uppercase-literal-suffix,readability-uppercase-literal-suffix,google-runtime-int) */
SUSUWU_INLINE const unsigned long long classObjectHashcodeVo64(const unsigned char *data, const size_t objectSz) { /* [Fowler-Noll-Vo hashcode](https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1a_hash) */
	const unsigned long long fowlerNollVo64OffsetBasis = 14695981039346656037ull;
	const unsigned long long fowlerNollVo64Prime = 1099511628211ull; /* NOLINT(hicpp-uppercase-literal-suffix,readability-uppercase-literal-suffix) */
	unsigned long long hash = fowlerNollVo64OffsetBasis;
	for (const unsigned char *const dataEnd = &data[objectSz]; data != dataEnd; ++data) { /* NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic): TODO, replace pointers with some sort of `_view`? */
	    hash ^= *data; /* the "1a" version does exclusive-or first, so that even if `1 == objectSz`, single-bit-input-differences still flip half the output-bits (cause avalanches) */
	    hash *= fowlerNollVo64Prime;
	}
	return hash;
} /* NOLINTEND(hicpp-uppercase-literal-suffix,readability-uppercase-literal-suffix,google-runtime-int) */
#define SUSUWU_VIRTUAL_HASHCODE { return classObjectHashcodeVo64(reinterpret_cast<const unsigned char *>(this), this->getObjectSize()); }

#define SUSUWU_CLASS_GETNAME(SUBCLASS) SUSUWU_VIRTUAL_ const std::string /* returns as value so subclasses can return dynamic values */ getName() const SUSUWU_CLASS_OVERRIDE { return #SUBCLASS; }
#define SUSUWU_CLASS_GETOBJECTSIZE(SUBCLASS) SUSUWU_VIRTUAL_ const size_t getObjectSize() const SUSUWU_OVERRIDE { return sizeof(SUBCLASS); } /* Run-time type information */
#define SUSUWU_CLASS_ISINSTANCE(SUBCLASS) SUSUWU_VIRTUAL_ const bool isInstance(const Class &obj) const SUSUWU_CLASS_OVERRIDE { auto ptr = dynamic_cast<const SUBCLASS *>(&obj); return SUSUWU_NULLPTR != ptr; } /* port of Java's */
#define SUSUWU_CLASS_DEFAULTS(SUBCLASS) /* Usage: `class SubClass : public Class { SUSUWU_CLASS_DEFAULTS(SubClass) };` */ \
	SUSUWU_CLASS_GETNAME(SUBCLASS) \
	SUSUWU_CLASS_GETOBJECTSIZE(SUBCLASS)\
	SUSUWU_CLASS_OPERATOREQUALTO(SUBCLASS) /* If you don't want it to override `operator==(const Class &)` for you; do `#define SUSUWU_CLASS_OPERATOREQUALTO(noop) ;` */\
	SUSUWU_CLASS_ISINSTANCE(SUBCLASS)
typedef class Class : public Instrumentation { /* Port of `java.lang.Class`. */ /* NOLINT(cppcoreguidelines-special-member-functions, hicpp-special-member-functions): suppress `clang-tidy`'s suggestions of constructors. */
public:
	~Class() SUSUWU_OVERRIDE SUSUWU_DEFAULT /* allows subclasses to release resources */
	SUSUWU_CLASS_DEFAULTS(Susuwu::Class)
	virtual const Class *getSuperclass() const { return SUSUWU_NULLPTR; /* no base for root class */ }
	SUSUWU_INLINE const Class &getSuperclassRef() const { const auto *superclass = getSuperclass(); return SUSUWU_NULLPTR != superclass ? *superclass : *this; /* for promises not to `return nullptr`, use references */ }
	SUSUWU_INLINE const bool hasLayoutOf(const Class &obj) const { return this->isRelatedTo(obj) && this->getObjectSize() == obj.getObjectSize(); }
	SUSUWU_INLINE const bool isAssignableFrom(const Class &obj) const { return this->isInstance(obj); } /* TODO: template to test types without obj */
	SUSUWU_INLINE const bool isRelatedTo(const Class &obj) const { return (this->isInstance(obj) || obj.isInstance(*this)); }
	virtual bool operator!=(const Class &obj) const SUSUWU_FINAL { return !this->operator==(obj); } /* do not override this, but `operator==(const Class &)` */
} Class;
SUSUWU_INLINE const bool instanceof(const Class &obj, const Class &thiso) { return thiso.isInstance(obj); } /* Just as Java's `instanceof`, has operands reversed compared to `isInstance`. */
#undef SUSUWU_CLASS_OVERRIDE                  /*     Was no-op for base functions, ... */
#define SUSUWU_CLASS_OVERRIDE SUSUWU_OVERRIDE /* ... but is `override` for subclasses. */

#define SUSUWU_OBJECT_OVERRIDE /* For base class functions is no-op. */
#define SUSUWU_VIRTUAL_DEFAULTS(SUBCLASS) /* Usage: `class SubClass : public Object { SUSUWU_VIRTUAL_DEFAULTS(SubClass) };` */ \
	SUSUWU_CLASS_DEFAULTS(SUBCLASS) /* If you don't want it to override `operator==(const Class &)` for you; do `#define SUSUWU_CLASS_OPERATOREQUALTO(Q) ;` */\
	SUSUWU_INSTRUMENTATION_ISPUREVIRTUAL(SUBCLASS)

#define SUSUWU_PURE_VIRTUAL_DEFAULTS(SUBCLASS) /* Usage: `class PureVirtualSubClass : public Object { SUSUWU_PURE_VIRTUAL_DEFAULTS(PureVirtualSubClass) };` */ \
	SUSUWU_CLASS_DEFAULTS(SUBCLASS) \
	SUSUWU_INSTRUMENTATION_ISPUREVIRTUAL_PURE_VIRTUAL(SUBCLASS)
typedef enum ObjectCloneAs : unsigned char { /* Is extra (not part of Java); accomodates the numerous types of C++ clones. */
	objectCloneAsNone           = 0, /* `!isCloneabble()` */
	objectCloneAsDeep           = 1, /* Recursive `new` */ /* [If overload `operator new`, set `-flifetime-dse=1](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#index-fno-lifetime-dse) */
	objectCloneAsShallow        = 2, /* `memcpy` */
	objectCloneAsReferenceCount = 4, /* `std::shared_ptr` */ /* TODO: `objectCloneAsReferenceCount` with const `clone` */
	objectCloneAsCoW            = 8  /* [Copy-on-Write](https://wikipedia.org/wiki/Copy-on-write)  */
} ObjectCloneAs;
typedef class Object : public Class { /* Port of `java.lang.Object`. */
public:
	SUSUWU_VIRTUAL_DEFAULTS(Susuwu::Object) /* use `SUSUWU_PURE_VIRTUAL_DEFAULTS(SubClass)` if `SubClass` is pure virtual */
	virtual const bool isCloneable() const { return objectCloneAsNone != cloneableAs(); }
	virtual const ObjectCloneAs cloneableAs() const { /* returns preferred/default clone mode */
		return isPlainOldData() ? objectCloneAsShallow : objectCloneAsNone; /* crude heuristic, override for complex classes */
	}
	virtual const bool isCloneableAs(ObjectCloneAs cloneAs) const { return cloneableAs() == cloneAs; /* must override if multiple cloneable types */ }
	virtual const Object stackClone() const {
		if(!isCloneableAs(objectCloneAsShallow)) { throw std::runtime_error("`" + getName() + "::stackClone()`: unsupported default use."); }
		return Object(*this);
	}
	virtual gsl::owner<Object *> clone() const {
//		return &(*(new Object) = stackClone());
		return cloneAs(cloneableAs());
	}
	virtual const Object stackCloneAs(ObjectCloneAs cloneAs) const {
		if(!isCloneableAs(objectCloneAsShallow)) { throw std::runtime_error("`" + getName() + "::stackCloneAs(" + std::to_string(cloneAs) + ")`: unsupported default use."); }
		return Object(*this);
	}
	virtual gsl::owner<Object *> cloneAs(ObjectCloneAs cloneAs) const {
//		return &(*(new Object) = stackCloneAs(cloneAs));
		if(!isCloneableAs(objectCloneAsShallow)) { throw std::runtime_error("`" + getName() + "::cloneAs(" + std::to_string(cloneAs) + ")`: unsupported default use."); }
		auto clonePtr = ::operator new(getObjectSize()); /* NOLINT(cppcoreguidelines-owning-memory) */
		memcpy(clonePtr, static_cast<const void *>(this), getObjectSize());
		return static_cast<Object *>(clonePtr);
	}
#if SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES /* If you interpret `Java`'s standard as "Addresses must match". */
	virtual const bool equals(const Object &obj) const { return this == &obj; } /* Java's contract requires you to override this version of `equals` */
#else /* SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES else */
	virtual const bool equals(const Object &obj) const { return this->getClass().operator==(obj); } /* if you want to override `equals`, just override `operator==(const Class *)`, which will do both */
#endif /* SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES else */
	virtual void finalize() {
		this->~Object();
	}
	const Class &getClass() const { return *this; }
	virtual const SUSUWU_INTPTR hashCode() const { return reinterpret_cast<SUSUWU_INTPTR>(this); }
	virtual const std::string toString() const {
		return getName() + '@' + classIoHexStr(hashCode()); /* TODO: if `SUSUWU_HEX_DOES_PREFIX`, remove "0x"? */
	}
	virtual void notify() {}
	virtual void notifyAll() {}
	virtual void wait() {}
} Object;
#undef SUSUWU_VIRTUAL_  /* Was used to mark base functions `virtual` ... */
#define SUSUWU_VIRTUAL_ /*            ... , but is no-op for subclasses. */
#undef SUSUWU_OBJECT_OVERRIDE                  /*     Was no-op for base functions, ... */
#define SUSUWU_OBJECT_OVERRIDE SUSUWU_OVERRIDE /* ... but is `override` for subclasses. */

/* Is some slowdown to use inheritance+polymorphism with all classes;
 * `--release` sets `-o2`, [`-o2` sets `-devirtualize`, which convert some polymorphic calls into constant calls](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#index-fdevirtualize)
 * [`-devirtualize` is improved if functions use `final` (which requires C++11)](https://stackoverflow.com/questions/8824587/what-is-the-purpose-of-the-final-keyword-in-c11-for-functions/78680754#78680754]. If >=C++11, `SUSUWU_FINAL` is `final`, if <C++11, is no-op. */
}; /* namespace Susuwu */
```

`less `[`cxx/ClassObject.cxx`](../cxx/ClassObject.cxx) #This is just unit tests. `ClassObject.hxx` has all which has actual use.

`less `[`cxx/ClassIo.hxx`](../cxx/ClassIo.hxx)
```c++
#if !SUSUWU_INHERIT_GSL_OWNER /* If `gsl::owner` was not included */
namespace gsl {
	template <typename Resource>
	using owner = Resource; /* Wraps pointers; [cppcoreguidelines-owning-memory] fix. */
}; /* namespace gsl */
#endif /* !SUSUWU_INHERIT_GSL_OWNER */

/* Abstractions to do with filesystems (such as `procfs`) */
namespace Susuwu {
#ifndef SUSUWU_HEX_DOES_PREFIX
#	define SUSUWU_HEX_DOES_PREFIX false
#endif /* ndef SUSUWU_HEX_DOES_PREFIX */
#define SUSUWU_HEX_PREFIX_SZ (SUSUWU_HEX_DOES_PREFIX ? 2 : 0)
#ifndef SUSUWU_DASHOS
#	define SUSUWU_DASHOS false /* `-Os` param */
#endif /* ndef SUSUWU_DASHOS */
#ifndef SUSUWU_IO_THROW
#	define SUSUWU_IO_THROW false /* `if(SUSUWU_IO_THROW) { classIoCheckStr(__func__, expectedValue, value); } else { if(expectedValue != value) { is.setstate(std::ios::failbit); } }` */
#endif /* ndef SUSUWU_IO_THROW */
#ifndef SUSUWU_USE_STD_HEX
#	define SUSUWU_USE_STD_HEX false /* prefer `std::hex` for conversions */
#endif /* ndef SUSUWU_STD_HEX */
#ifndef SUSUWU_IO_WHITESPACE
#	define SUSUWU_IO_WHITESPACE false /* true if `getline` includes characters such as '\n' */
#endif /* ndef SUSUWU_IO_WHITESPACE */
typedef std::string ClassIoPath; /* TODO: `std::char_traits<unsigned char>`, `std::basic_string<unsigned char>("string literal")` */
typedef ClassIoPath ClassIoBytecode; /* Uses `std::string` for bytecode (versus `std::vector`) because:
 * "If you are going to use the data in a string like fashon then you should opt for std::string as using a std::vector may confuse subsequent maintainers. If on the other hand most of the data manipulation looks like plain maths or vector like then a std::vector is more appropriate." -- https://stackoverflow.com/a/1556294/24473928
*/
typedef ClassIoPath ClassIoHash; /* TODO: `std::unordered_set<std::basic_string<unsigned char>>` */

/* Usage: for Linux (or Windows,) if you don't trust `argv[0]`, replace it with `classIoGetOwnPath()`.
 * Error values: `return ClassIoPath();` */
const ClassIoPath classIoGetOwnPath() /* TODO: SUSUWU_NOEXCEPT(std::is_nothrow_constructible<ClassIoPath>::value) */;
const gsl::owner<FILE *> classIoFopenOwnPath() /* TODO: SUSUWU_NOEXCEPT(std::is_nothrow_invocable<classIoGetClassIoPath()>::value) */;

static const bool classIoGetConsoleInput() { return std::cin.good() && !std::cin.eof(); }
const bool classIoSetConsoleInput(bool input); /* Set to `false` for unit tests/background tasks (acts as if user pressed `<ctrl>+d`, thus input prompts will use default choices.) Returns `classIoGetConsoleInput();` */
const unsigned char classIoGetConsoleAttributes(); /* if(_WIN32 || ) { return (background * 16) + foreground color; } else if(_POSIX_SOURCE) { return "\033[%1;%2m" -> (%1 * 16) + %2 ; } else { return 0; } */
const bool classIoConsoleHasAnsiColors();

template<class Os, class Int,
	typename std::enable_if<std::is_integral<Int>::value, int>::type = 0>
inline Os &classIoHexOs(Os &os, const Int &value) {
#if SUSUWU_HEX_DOES_PREFIX
	os << "0x";
#endif /* SUSUWU_HEX_DOES_PREFIX */
	const std::ios::fmtflags oldFlags = os.flags();
	os << std::hex << value;
	os.flags(oldFlags);
	return os;
}
template<class Int,
	typename std::enable_if<std::is_integral<Int>::value, int>::type = 0>
inline const std::string classIoHexStr(const Int &value) {
	std::stringstream os;
	classIoHexOs(os, value);
	return os.str();
}
template<class Os, class Str,
	typename std::enable_if<!std::is_integral<Str>::value, int>::type = 0>
inline Os &classIoHexOs(Os &os, const Str &value) {
#if SUSUWU_HEX_DOES_PREFIX
	os << "0x";
#endif /* SUSUWU_HEX_DOES_PREFIX */
	const std::ios::fmtflags oldFlags = os.flags();
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
template<class Str,
	typename std::enable_if<!std::is_integral<Str>::value, int>::type = 0>
inline const Str classIoHexStr(const Str &value) {
	std::stringstream os;
	classIoHexOs(os, value);
	return os.str();
}
template<class Os, class List>
inline Os &classIoColoredParamOs(Os &os, const List &argvS, const bool parenthesis/* {...} */ = true) {
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
inline const typename List::value_type classIoColoredParamStr(const List &argvS, const bool parenthesis/* {...} */ = true) {
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
} /* TODO: move into `ClassStr.hxx`? */
static const size_t classIoAscii7Bit = (1 << 7);
SUSUWU_STATIC_ASSERT(0x80 == classIoAscii7Bit); /* ASCII, 7bit*/
#if SUSUWU_HEX_TABLE
static const std::array<signed char,
#	if SUSUWU_DASHOS
						 classIoAscii7Bit
#	else /* SUSUWU_DASHOS else */
						 1 << CHAR_BIT
#	endif /* SUSUWU_DASHOS else */
						 > classIoHex2Nib = {
	/* memset(&classIoHex2Nib[0x00], -1, 48) */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* [0x30] = '0' */ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, /* memset(&classIoHex2Nib[0x3A], -1, 7) */ -1, -1, -1, -1, -1, -1, -1, /* [0x41] = 'A' */ 10, 11, 12, 13, 14, 15, /* memset(&classIoHex2Nib[0x48], -1, 26) */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* [0x61] = 'a' */ 10, 11, 12, 13, 14, 15, /* memset(&classIoHex2Nib[0x67], -1, 25) */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
#	if 8 < CHAR_BIT
#		error "TODO: 8 < CHAR_BIT"
#	elif 7 < CHAR_BIT && !SUSUWU_DASHOS
	, /* memset(&classIoHex2Nib[classIoAscii7Bit], -1, (1 << CHAR_BIT) - classIoAscii7Bit) */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
#	endif /* 7 < CHAR_BIT */
}; /* TODO: move into `ClassStr.hxx`? */
#endif /* SUSUWU_HEX_TABLE */
inline const bool classIoIsXdigit(const unsigned char char_) {
#if SUSUWU_HEX_TABLE
#	if SUSUWU_DASHOS
	return (-1 != classIoHex2Nib.at(char_));
#	else /* SUSUWU_DASHOS else */
	return (-1 != classIoHex2Nib[char_]); /* NOLINT(cppcoreguidelines-pro-bounds-constant-array-index) */
#	endif /* SUSUWU_DASHOS else */
#else /* SUSUWU_HEX_TABLE else */
	return (0 != isxdigit(char_));
#endif /* SUSUWU_HEX_TABLE */
} /* TODO: move into `ClassStr.hxx`? */
inline unsigned char classIoHexitToNibble(const unsigned char hexit) {
#if SUSUWU_HEX_TABLE
	assert(-1 != classIoHex2Nib.at(hexit)); /* Min valid = 0x30, max valid = 0x66 */
#	if SUSUWU_DASHOS
	return classIoHex2Nib.at(hexit);
#	else /* SUSUWU_DASHOS else */
	return classIoHex2Nib[hexit]; /* NOLINT(cppcoreguidelines-pro-bounds-constant-array-index) */
#	endif /* SUSUWU_DASHOS else */
#else /* SUSUWU_HEX_TABLE else */
	static const size_t aToHex = 10;
	assert(0 != isxdigit(hexit));
	if(0 == isupper(hexit)) {
		return hexit - '0';
	} else {
		return hexit - 'A' + aToHex;
	}
#endif /* SUSUWU_HEX_TABLE else */
} /* TODO: move into `ClassStr.hxx`? */
inline unsigned char classIoHex2Char(unsigned char hexit1, unsigned char hexit2) {
		hexit1 = classIoHexitToNibble(hexit1);
		hexit2 = classIoHexitToNibble(hexit2);
		return static_cast<unsigned char>((hexit1 << 4) | hexit2);
} /* TODO: move into `ClassStr.hxx`? */
template<class Is>
/* Usage: @code classIoDebugIs(__func__, is); @endcode */
static void classIoDebugIs(const std::string &func, Is &is) {
#ifdef NDEBUG
	(void)func; (void)is; /* suppress `clang-tidy`/`lint` notices */
#else /* ndef NDEBUG */
	const std::streampos pos = is.tellg();
	std::string token;
	if(!is.good()) {
		SUSUWU_DEBUG("(!is.good())");
		return;
	} /* auto isState = is.getstate(); */
	if(is >> token) {
		SUSUWU_DEBUG(func + ": is == \"" + token + "\"");
	} else if(is.fail() && !(is.bad() || is.eof())) {
		is.clear(std::ios::goodbit); /* is.setstate(isState); */
	}
	is.seekg(pos);
#endif /* ndef NDEBUG */
}
template<class Is, class Int,
	typename std::enable_if<std::is_integral<Int>::value, int>::type = 0>
/* @pre @code is.good(); @endcode */
inline Is &classIoHexIs(Is &is, Int &value) {
	const std::ios::fmtflags oldFlags = is.flags();
	const char oldFill = is.fill();
	is << std::hex;
	is.fill('0');
	/* classIoDebugIs(std::string(__func__) + "(pre)", is); */
	if(!is.good()) {
		SUSUWU_NOTICE("(!is.good())");
		return is;
	} /* auto isState = is.getstate(); */
	std::streampos pos = is.tellg();
	if(is >> std::hex >> std::setw(2)/* `setw` is unset after each use */ >> value) {
		const std::streampos newPos = is.tellg();
		/* SUSUWU_DEBUG("value == 0x" + classIoHexStr(value) + ", pos += " + std::to_string(newPos - pos)); */
		pos = newPos;
	} else if(is.fail() && !(is.bad() || is.eof())) {
		is.seekg(pos); /* TODO: prove if this can be removed for all flags*/
		is.clear(std::ios::goodbit); /* is.setstate(isState); */
	}
	/* classIoDebugIs(std::string(__func__) + "(post)", is); */
	is.fill(oldFill);
	is.flags(oldFlags);
	return is;
} /* TODO: refactor this and the `Str` version, to reuse common code */
template<class Is, class Str,
	typename std::enable_if<!std::is_integral<Str>::value, int>::type = 0>
/* @pre @code is.good(); @endcode */
inline Is &classIoHexIs(Is &is, Str &value) {
	value.clear(); /* TODO; move after `if(!is.good()) { ... return is; }` (to preserve `value` if precondition fails)? */
	if(!is.good()) {
		SUSUWU_DEBUG("(!is.good())"); /* TODO; remove (since caller should check `is.good()`)? */
		return is;
	} /* auto isState = is.getstate(); */
	std::streampos pos = is.tellg();
#if SUSUWU_USE_CXX_HEX /* TODO: fix or remove this code path */
	const std::ios::fmtflags oldFlags = is.flags();
	const char oldFill = is.fill();
	is << std::hex;
	is.fill('0');
	/* classIoDebugIs(std::string(__func__) + "(pre)", is); */
	for(unsigned int ch; is.good() && is >> std::hex >> std::setw(2)/* `setw` is unset after each use */ >> ch; ) {
		const std::streampos newPos = is.tellg();
		/* SUSUWU_DEBUG("ch == 0x" + classIoHexStr(ch) + ", pos += " + std::to_string(newPos - pos)); */
		for(auto i = ((newPos - pos) >> 1) - (SUSUWU_HEX_DOES_PREFIX ? 1 : 0); i--; ) {
			value += reinterpret_cast<unsigned char *>(&ch)[i];
		}
		pos = newPos;
	}
	is.fill(oldFill);
	is.flags(oldFlags);
	if(is.fail() && !(is.bad() || is.eof())) { /* If stopped due to non-hex input, */
		is.seekg(pos); /* restore non-hex input (which was consumed). */
		is.clear(std::ios::goodbit); /* `is.setstate(isState);` */
	}
#else /* SUSUWU_USE_CXX_HEX else */
#	if SUSUWU_HEX_DOES_PREFIX
#		if SUSUWU_PREFER_GETLINE
	std::string prefix;
	std::getline(is, prefix, 'x'); /* used `std::getline` so 'x' is swallowed */
	if("0" != prefix) {
#		else /* SUSUWU_PREFER_GETLINE else */
	char prefix[2] = {'\0', '\0'};
	is >> prefix[0];
	if(!('0' == prefix[0] && is >> prefix[1] && 'x' == prefix[1])) {
#		endif /* SUSUWU_PREFER_GETLINE else */
		is.seekg(pos);
#		if SUSUWU_IO_THROW
		classIoCheckStr(__func__, "0x", prefix); /* TODO: choose `std::ios::failbit` or `throw` */
#		else /* SUSUWU_IO_THROW else */
		SUSUWU_DEBUG("(SUSUWU_HEX_DOES_PREFIX && (prefix != \"" SUSUWU_SH_GREEN "0x" SUSUWU_SH_DEFAULT "\") && (prefix == \"" SUSUWU_SH_RED + std::string(prefix) + SUSUWU_SH_DEFAULT "\"))");
		is.setstate(std::ios::failbit);
#		endif /* SUSUWU_IO_THROW else */
		return is;
	}
	pos = is.tellg();
#	endif /* SUSUWU_HEX_DOES_PREFIX */
	for(unsigned char hexit1 = '\0', hexit2 = '\0'; is.good() && is >> hexit1; ) {
		if(0 == classIoIsXdigit(hexit1) || (is >> hexit2 && 0 == classIoIsXdigit(hexit2))) {
			if(!is.bad()) {
				is.seekg(pos);
				is.clear(std::ios::goodbit); /* is.setstate(isState); */
			}
			break;
		}
		pos = is.tellg();
		/* SUSUWU_DEBUG("hexit1 == \"" + classIoHexStr(hexit1) + "\", hexit2 == \"" + classIoHexStr(hexit2) + '"'); */
		value += classIoHex2Char(hexit1, hexit2);
	}
#endif /* SUSUWU_USE_CXX_HEX else */
	/* classIoDebugIs(std::string(__func__) + "(post)", is); */
	return is;
}
template<class Is, class Value>
/* usage: @code U value = classIoHexIs<U>(is); @endcode
 * @pre @code is.good(); @endcode */
inline const Value classIoHexIs(Is &is) {
	Value value;
	classIoHexIs(is, &value);
	return value;
}
template <class CharT, class Traits, class Allocator>
/* Usage: is as `std::getline` except that `delim` is not consumed
 * @pre @code is.good(); @endcode */
static std::basic_istream<CharT, Traits>& classIoGetline(std::basic_istream<CharT, Traits>& is, std::basic_string<CharT, Traits, Allocator>& str, CharT delim = '\n') {
	str.clear();
	for(char token = delim; is.good(); str += token) {
		const std::streampos pos = is.tellg();
		is >> token;
		if(delim == token) {
			is.seekg(pos);
			return is;
		}
	}
	return is;
}
/* Usage: @code is >> value; classIoCheckChar(__func__, '{', value); @endcode */
inline void classIoCheckChar(const std::string &func, const char expected, const char got) {
	if(expected != got) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, func + ": expected '" SUSUWU_SH_GREEN + expected + SUSUWU_SH_DEFAULT "', got '" SUSUWU_SH_RED + got + SUSUWU_SH_DEFAULT "'"));
	} /* classIoCheckStr(func, std::string(expected), got); */
}
/* Usage: @code is >> value; classIoCheckSz(__func__, 42, value); @endcode */
inline void classIoCheckSz(const std::string &func, const size_t expected, const size_t got) {
	if(expected != got) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, func + ": expected '" SUSUWU_SH_GREEN + std::to_string(expected) + SUSUWU_SH_DEFAULT "', got '" SUSUWU_SH_RED + std::to_string(got) + SUSUWU_SH_DEFAULT "'"));
	} /* classIoCheckStr(func, std::to_string(expected), std::to_string(got)); */
}
/* Usage: @code if(rand() % 2) {is >> value;} else {classIoGetline(is, value);} classIoCheckStr(__func__, "};", value); @endcode */
static void classIoCheckStr(const std::string &func, const std::string &expected, const std::string &got) {
	if(expected != got) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, func + ": expected '" SUSUWU_SH_GREEN + expected + SUSUWU_SH_DEFAULT "', got '" + SUSUWU_SH_RED + got + SUSUWU_SH_DEFAULT "'"));
	}
}

#if SUSUWU_UNIT_TESTS
/* @throw std::runtime_error */
const bool classIoTests();
const bool classIoTestsNoexcept() SUSUWU_NOEXCEPT;
#endif /* SUSUWU_UNIT_TESTS */

}; /* namespace Susuwu */
```

`less `[`cxx/ClassIo.cxx`](../cxx/ClassIo.cxx)
```c++
const gsl::owner<FILE *> classIoFopenOwnPath() {
	return fopen(classIoGetOwnPath().c_str(), "r");
}
const ClassIoPath classIoGetOwnPath() {
#ifdef __linux__
	char path[PATH_MAX]; /* NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays) */
	const ssize_t len = readlink("/proc/self/exe", static_cast<char *>(path), sizeof(path) - 1);
	if (len == -1) {
		SUSUWU_ERROR("classIoGetOwnPath(): { if(-1 == readlink(\"/proc/self/exe\", path, sizeof(path) - 1)) { errno == " + std::to_string(errno) + "; } }");
		return ClassIoPath(); /* return EXIT_FAILURE; */
	}
	path[len] = '\0'; /* NOLINT(cppcoreguidelines-pro-bounds-constant-array-index) */
//	`return "/proc/self/exe"; /* if _Termux_, causes `PortableExecutableBytecode(classIoGetOwnPath())` to act as `PortableExecutableBytecode("/apex/com.android.runtime/bin/linker64")` */
	return static_cast<ClassIoPath>(&path[0]); /* causes `PortableExecutableBytecode(classIoGetOwnPath())` to act as `PortableExecutableBytecode(argv[0])` */
#elif defined SUSUWU_WIN32
	const HMODULE hModule = GetModuleHandle(SUSUWU_NULLPTR);
	const size_t nSize = GetModuleFileName(hModule, SUSUWU_NULLPTR, 0);
	static const std::string getModuleFileNameReturn = "classIoGetOwnPath(): { HMODULE hModule = GetModuleHandle(nullptr); size_t nSize = GetModuleFileName(hModule, nullptr, 0); (nSize == " SUSUWU_SH_PURPLE;
	if(0 < nSize) {
		char *const lpFilename = new char[nSize];
		const size_t result = GetModuleFileName(hModule, lpFilename, nSize);
		if(nSize == result) {
			return ClassIoPath(lpFilename);
		} else {
			SUSUWU_ERROR(getModuleFileNameReturn + std::to_string(nSize) + SUSUWU_SH_DEFAULT "); char *const lpFilename = new char[nSize]; (GetModuleFileName(hModule, lpFileName, nSize) == " SUSUWU_SH_PURPLE + std::to_string(result) + SUSUWU_SH_DEFAULT " /* expected `== nSize` */); (GetLastError() == " SUSUWU_SH_PURPLE + std::to_string(GetLastError()) + SUSUWU_SH_DEFAULT "); }");
			return ClassIoPath(); /* return EXIT_FAILURE; */
		}
	} else {
		SUSUWU_ERROR(getModuleFileNameReturn + "0" SUSUWU_SH_DEFAULT " /* expected `> 0` */); (GetLastError() == " SUSUWU_SH_PURPLE + std::to_string(GetLastError()) + SUSUWU_SH_DEFAULT "); }");
		return ClassIoPath(); /* return EXIT_FAILURE; */
	}
#else /* def SUSUWU_WIN32 else */
	if(SUSUWU_NULLPTR == classSysArgs) {
		SUSUWU_ERROR("classIoGetOwnPath(): { if(SUSUWU_NULLPTR == classSysArgs) {/* `classSysInit()` was not used? */} }");
		return ClassIoPath(); /* return EXIT_FAILURE; */
	} else if(SUSUWU_NULLPTR == classSysArgs[0]) { /* NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic) */
		SUSUWU_ERROR("classIoGetOwnPath(): { if(SUSUWU_NULLPTR == classSysArgs[0]) {/* `classSysInit()` was not used? */} }");
		return ClassIoPath(); /* return EXIT_FAILURE; */
	}
	return ClassIoPath(classSysArgs[0]); /* NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic) */
#endif /* def SUSUWU_WIN32 else */
}

const bool classIoSetConsoleInput(bool input) {
	input ? std::cin.clear(std::ios::goodbit) : std::cin.setstate(std::ios::eofbit);
	return classIoGetConsoleInput();
}
const unsigned char classIoGetConsoleAttributes() {
#ifdef SUSUWU_WIN32
	CONSOLE_SCREEN_BUFFER_INFO info;
	if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) {
		SUSUWU_WARNING("classIoGetConsoleAttributes() {/* TODO: [decode response from `GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)`](https://github.com/SwuduSusuwu/SubStack/issues/17)");
		return info.wAttributes;
	} else {
		SUSUWU_ERROR("classIoGetConsoleAttributes() {!GetConsoleScreenBufferInfo() && GetLastError() == " SUSUWU_SH_PURPLE + std::to_string(GetLastError()) + SUSUWU_SH_DEFAULT "}");
	}
#elif defined SUSUWU_POSIX
	std::cout << "\033[?6;1;1t" /* Request console attributes */ << std::flush;
	const size_t bufferLen = 32;
	char buffer[bufferLen];
	std::cin.read/*non-blocking*/(static_cast<char *>(buffer), sizeof(buffer)); /* read request response from console */ /* TODO: have it portable, support all consoles */
	const size_t bytesRead = strlen(static_cast<const char *>(buffer));
	if(0 < bytesRead) {
		SUSUWU_WARNING("classIoGetConsoleAttributes() {/* TODO: [decode response from `\\033[?6;1;1t`](https://github.com/SwuduSusuwu/SubStack/issues/17)");
# ifdef SUSUWU_DEBUG2
		std::cerr << "Current color settings: ";
		for(size_t i = 0; i < bytesRead; ++i) {
			std::cerr << buffer[i]; /* TODO: decode this response (Termux doesn't have this) */
		}
		std::cout << std::endl;
# endif /* def SUSUWU_DEBUG2 */
	} else {
# ifndef NDEBUG
		SUSUWU_WARNING("classIoGetConsoleAttributes() {std::cout << \"\\033[?6;1;1t\" << std::flush; char buffer[32]; (" + std::to_string(bytesRead) + " == std::cin.readsome(buffer, sizeof(buffer));)");
# endif /* ndef NDEBUG */
	}
#else /* elif defined SUSUWU_POSIX else */
	SUSUWU_NOTICE("classIoGetConsoleAttributes() { /* [TODO](https://github.com/SwuduSusuwu/SubStack/issues/17): `#if !defined(SUSUWU_WIN32) && !defined(SUSUWU_POSIX)`. Hardcoded to `errno = ENOTTY; return 0;`. */ }");
#endif /* elif defined SUSUWU_POSIX else */
	errno = ENOTTY;
	return 0;
}
const bool classIoConsoleHasAnsiColors() {
#if defined(SUSUWU_SH_SKIP_COLORS) && SUSUWU_SH_SKIP_COLORS
	return false;
#elif defined(__WIN32__)
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if(INVALID_HANDLE_VALUE == hConsole) {
		SUSUWU_PRINT(SUSUWU_SH_WARNING, "classIoConsoleHasAnsiColors() {(!GetConsoleScreenBufferInfo()} && GetLastError() == " SUSUWU_SH_PURPLE + std::to_string(GetLastError()) + SUSUWU_SH_DEFAULT "}");
		return false;
	}
	DWORD mode;
	if(!GetConsoleMode(hConsole, &mode)) {
		return false;
	}
	if(!SetConsoleMode(hConsole, mode | ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING /* virtual mode allows CSI colors */)) {
		SUSUWU_PRINT(SUSUWU_SH_WARNING, "classIoConsoleHasAnsiColors() {(!SetConsoleMode(hConsole, mode | ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) && GetLastError() == " SUSUWU_SH_PURPLE + std::to_string(GetLastError()) + SUSUWU_SH_DEFAULT "}");
		return false;
	}
	return true;
#elif defined _POSIX_VERSION
	return true;
#else /* ndef _POSIX_VERSION */
	const char *term = getenv("TERM");
#	if defined(SUSUWU_SH_SKIP_COLORS) && !SUSUWU_SH_SKIP_COLORS
	static const char *uncolored[] = {"dumb", SUSUWU_NULLPTR}; /* blacklist */
	for(const char **it = uncolored; SUSUWU_NULLPTR != *it; ++*it) {
		if(0 == strcmp(term, *it)) {
			return false;
		}
	}
	return true;
#	else /* ndef SUSUWU_SH_SKIP_COLORS */
	static const char *colored[] = {"screen", "screen-256color", "vt100", "xterm", "xterm-256color", SUSUWU_NULLPTR}; /* whitelist */
	for(const char **it = colored; SUSUWU_NULLPTR != *it; ++*it) {
		if(0 == strcmp(term, *it)) {
			return true;
		}
	}
	return false;
#	endif /* ndef SUSUWU_SH_SKIP_COLORS_BLACKLIST */
#endif /* ndef _POSIX_VERSION */
}

#if SUSUWU_UNIT_TESTS
static void classIoHexOsSzTest(const std::string &value, const size_t hexSz, const bool printable) {
	const size_t ss = classIoHexStr(value).size();
	const std::string escapedValue = (printable ? ('"' + value + '"') : "value");
	if(hexSz + SUSUWU_HEX_PREFIX_SZ != ss) {
		throw std::logic_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "classIoHexStr(" + escapedValue + ").size() == " SUSUWU_SH_RED + std::to_string(value.size()) + SUSUWU_SH_DEFAULT "; classIoHexStr(" + escapedValue + ").size() != " SUSUWU_SH_GREEN + std::to_string(hexSz) + SUSUWU_SH_DEFAULT ";"));
	}
	std::stringstream os;
	classIoHexOs(os, value);
	if(ss != os.str().size()) {
		throw std::logic_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "classIoHexOs(os, " + escapedValue + "); " SUSUWU_SH_RED + std::to_string(value.size()) + SUSUWU_SH_DEFAULT " /* value.size() */ != " SUSUWU_SH_GREEN + std::to_string(os.str().size()) + SUSUWU_SH_DEFAULT " /* os.str().size() */;"));
	}
}
static void classIoHexSsNumTest(const long &num) {
	std::stringstream os;
	classIoHexOs(os, num);
	long newNum = 0;
	classIoHexIs(os, newNum);
	if(num != newNum) {
		throw std::logic_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, std::string("classIoHexOs(os, " SUSUWU_SH_GREEN) + std::to_string(num) + SUSUWU_SH_DEFAULT "\"); long newNum; classIoHexIs(os, newNum); newNum == " SUSUWU_SH_RED + std::to_string(newNum) + SUSUWU_SH_DEFAULT ";"));
	}
}
static void classIoHexSsStrTest(const std::string &value, const bool printable) {
	std::stringstream os;
	classIoHexOs(os, value);
	std::string newValue;
	classIoHexIs(os, newValue);
	if(value != newValue) {
		const std::string escapedValue = (printable ? ("\"" SUSUWU_SH_GREEN + value + SUSUWU_SH_DEFAULT "\"") : (SUSUWU_SH_GREEN + classIoHexStr(value) + SUSUWU_SH_DEFAULT));
		const std::string escapedNewValue = (newValue.empty() ? "\"\"" : (printable ? ("\"" SUSUWU_SH_RED + newValue + SUSUWU_SH_DEFAULT "\"") : SUSUWU_SH_RED + classIoHexStr(newValue) + SUSUWU_SH_DEFAULT)); /* NOLINT(readability-avoid-nested-conditional-operator): alternatives are much more verbose */
		throw std::logic_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "std::string value = " + escapedValue + ", newValue; classIoHexOs(os, value); classIoHexIs(os, newValue); newValue != value; newValue == " + escapedNewValue + ';'));
	}
}
const bool classIoTests() {
	/* test `classIoHexStr()` and `classIoHexOs()`'s output lengths */
	classIoHexOsSzTest("", (sizeof("") - sizeof('\0')) * 2, true /* test that `value.empty()` produces 0 hexits */);
	classIoHexOsSzTest("22", (sizeof("22") - sizeof('\0')) * 2, true /* test that 2 chars produces 4 hexits */);
	classIoHexOsSzTest("uwu", (sizeof("uwu") - sizeof('\0')) * 2, true /* test that 3 chars produces 6 hexits */);
	classIoHexOsSzTest(std::string({'\0'}), sizeof('\0') * 2, false /* test that char == 0x00 produces 2 hexits */);
	classIoHexOsSzTest("\010", sizeof('\010') * 2, false /* test that char <= 0x10 produces 2 hexits */);
	classIoHexOsSzTest("\022", sizeof('\022') * 2, false /* test that char >= 0x10 produces 2 hexits */);

	/* test that `classIoHexIs()` undoes `classIoHexOs()` */
	for(long q = 0; (1 << CHAR_BIT) >= q /* test all `char` + first `short` */; ++q) {
		classIoHexSsNumTest(q);
	}
	classIoHexSsStrTest("", true);
	classIoHexSsStrTest("2", true);
	classIoHexSsStrTest("22", true);
	classIoHexSsStrTest("\010", false);
	classIoHexSsStrTest("\022", false);
	classIoHexSsStrTest(std::string({'\0'}), false);

	return true;
}
const bool classIoTestsNoexcept() SUSUWU_NOEXCEPT { return templateCatchAll(classIoTests, "classIoTests()"); } /* cppcheck-suppress throwInNoexceptFunction */
#endif /* SUSUWU_UNIT_TESTS */
```

`less `[`cxx/ClassPortableExecutable.hxx`](../cxx/ClassPortableExecutable.hxx)
```c++
typedef class PortableExecutable : public Object {
/* TODO: union of actual Portable Executable (Microsoft) + ELF (Linux) specifications */
public:
	SUSUWU_VIRTUAL_DEFAULTS(Susuwu::PortableExecutable) /* `getName()`, `isPureVirtual()`, `operator==`()`, ... */
	explicit PortableExecutable(ClassIoPath path_ = "") : path(std::move(path_)) {}
	PortableExecutable(ClassIoPath path_, ClassIoBytecode bytecode_) : path(std::move(path_)), bytecode(std::move(bytecode_)) {} /* TODO: NOLINT(bugprone-easily-swappable-parameters) */
/*TODO: overload on typedefs which map to the same types:	PortableExecutable(const ClassIoPath &path_, const std::string &hex_) : path(path_), hex(hex_) {} */
	const ClassIoPath path; /* Suchas "C:\Program.exe" or "/usr/bin/library.so" */ /* NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members) */
	ClassIoBytecode bytecode; /* compiled programs; bytecode */
	std::string hex; /* `hexdump(path)`, hexadecimal, for C string functions */
} PortableExecutable;
typedef class PortableExecutableBytecode : public PortableExecutable {
public:
	SUSUWU_VIRTUAL_DEFAULTS(Susuwu::PortableExecutableBytecode) /* `getName()`, `isPureVirtual()`, `operator==`()`, ... */
	explicit PortableExecutableBytecode(ClassIoPath path_) : PortableExecutable(std::move(path_)) { std::ifstream input(path); if(input.good()) { bytecode = std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()); } }
} PortableExecutableBytecode;
```

`less `[`cxx/ClassSys.hxx`](../cxx/ClassSys.hxx)
```c++
/* Abstractions to do with: `sh` scripts (such as: `exec*`, `sudo`), sockets (such as `socket`, `WinSock2`) */
#ifdef SUSUWU_CXX20
extern std::span<const char *> classSysArgs; /* [cppcoreguidelines-pro-bounds-pointer-arithmetic] fix */
#else
extern int classSysArgc;
extern const char **classSysArgs;
#endif
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
 * @throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "execves: -1 == execvesFork()"))
 * @throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "execves: if(1 != argvS.size()) // TODO: non-POSIX systems with multiple commands
 * @pre @code (-1 != access(argvS[0], X_OK) @endcode */
const int execves(const std::vector<std::string> &argvS = {}, const std::vector<std::string> &envpS = {});
static const int execvex(const std::string &toSh) {return execves({"/bin/sh", "-c", toSh});}

/* #if SUSUWU_POSIX, `return (0 == geteuid());` #elif SUSUWU_WIN32 `return IsUserAnAdmin();` #endif `return false;` */
const bool classSysHasRoot();
/* #if SUSUWU_POSIX, `root ? (seteuid(0) : (seteuid(getuid() || getenv("SUDO_UID")), setuid(geteuid)); return classSysHasRoot();` #endif
 * Usage: classSysSetRoot(true); classSysKernelSetHook(...); classSysSetRoot(false); */
const bool classSysSetRoot(bool root); /* root ? (seteuid(0) : (seteuid(getuid() || atoi(getenv("SUDO_UID"))), setuid(geteuid)); return classSysHasRoot(); */

/* Effect: `(callback(args...) ? func(args...) : decltype(func(args...))())` */
template<typename func, typename callback, typename... Args>
auto classSysKernelCallback(Args... args) -> decltype(func(args...)) {
	const auto ret = callback(args...);
	return static_cast<bool>(ret) ? func(args...) : ret;
}
/* Usage: `classSysKernelSetHook(download, classSysKernelCallback<download, virusAnalysisDownloadCallback>);`
 * Effect: `:%s/func(/classSysKernelCallback<func, callback>(/`
 * @pre @code classSysHasRoot() @endof */
template<typename Func, typename Lambda>
const bool classSysKernelSetHook(Func func, Lambda callback) {
	if(classSysHasRoot()) { /* cppcheck-suppress knownConditionTrueFalse */
		SUSUWU_WARNING("classSysKernelSetHook: TODO");
//		return true; /* TODO: hook `func` */
	} else {
		SUSUWU_ERROR("classSysKernelSetHook: if(!classSysHasRoot()) {/* kernel hook impossible to use */}");
	}
	return false;
}

template<typename Func, typename... Args>
auto templateCatchAll(Func func, const std::string &funcName, Args... args) SUSUWU_NOEXCEPT -> const decltype(func(args...)) { /* cppcheck-suppress throwInNoexceptFunction */
	try {
		return func(args...);
	} catch (const std::exception &w) {
		SUSUWU_ERROR(funcName + " { throw std::exception(\"" + w.what() + "\"); }");
	} catch (...) { /* "error: Exception thrown in function declared not to throw exceptions. [throwInNoexceptFunction]" fix */
		SUSUWU_ERROR(funcName + " { throw ??? /* unknown; not derivative of std::exception */; }");
	}
	return decltype(func(args...))(); /* `func(args...)`'s default return value; if `int func(args...)`, `return 0;`. If `bool func(args...)`, `return false;` */
}

#if SUSUWU_UNIT_TESTS
const bool classSysTests();
static const bool classSysTestsNoexcept() SUSUWU_NOEXCEPT { return templateCatchAll(classSysTests, "classSysTests()"); } /* cppcheck-suppress throwInNoexceptFunction */
#endif /* SUSUWU_UNIT_TESTS */
```

`less `[`cxx/ClassSys.cxx`](../cxx/ClassSys.cxx)
```c++
#ifdef SUSUWU_CXX20
std::span<const char *> classSysArgs({}); /* [cppcoreguidelines-pro-bounds-pointer-arithmetic] fix */
#else
int classSysArgc = 0;
const char **classSysArgs = {SUSUWU_NULLPTR};
#endif
const bool classSysInit(int argc, const char **args) {
#ifdef SUSUWU_CXX20
	classSysArgs = {args, static_cast<size_t>(argc)}; /* [cppcoreguidelines-pro-bounds-pointer-arithmetic] fix */
#else
	classSysArgc = argc;
	classSysArgs = args;
#endif
	if(0 < argc) {
		assert(SUSUWU_NULLPTR != args);
		assert(SUSUWU_NULLPTR != args[0]); /* `clangtidy` off: NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic) */
		return true;
	}
#ifndef SUSUWU_SH_SKIP_COLORS
	if(!classIoConsoleHasAnsiColors()) {
		SUSUWU_WARNING("classSysInit() {(!classIoConsoleHasAnsiColors()) /* Command Sequence Introducers disabled */}");
	}
#endif /* ndef SUSUWU_SH_SKIP_COLORS */
	return false;
}

const pid_t execvesForkThrow(const std::vector<std::string> &argvS, const std::vector<std::string> &envpS) SUSUWU_NOEXCEPT {
	std::vector<char *> argv;
	const std::vector<std::string> argvSmutable = {argvS.cbegin(), argvS.cend()};
	argv.reserve(argvSmutable.size() + 1);
	/* Do not remove `&`. `for(auto x : argvSmutable)` with `fsanitize=address` triggers `stack-use-after-scope` */
	for(const auto &x: argvSmutable /* auto x = argvSmutable.cbegin(); argvSmutable.cend() != x; ++x */) {
		argv.push_back(const_cast<char *>(x.c_str()));
	}
	argv.push_back(SUSUWU_NULLPTR);
#ifdef SUSUWU_POSIX
	const pid_t pid = fork();
	if(0 != pid) {
		if(-1 == pid) {
			SUSUWU_ERROR("execvesFork(): {(-1 == pid)}, errno=" + std::to_string(errno));
		}
		return pid;
	} /* if 0, is fork */
	if(envpS.empty()) { /* Reuse LD_PRELOAD to fix https://github.com/termux-play-store/termux-issues/issues/24 */
		execv(argv[0], &argv[0]); /* has `noreturn` */
	} else {
		std::vector<char *> envp;
		std::vector<std::string> envpSmutable = {envpS.cbegin(), envpS.cend()};
		envp.reserve(envpSmutable.size() + 1);
		for(const auto &x: envpSmutable) {
			envp.push_back(const_cast<char *>(x.c_str()));
		}
		envp.push_back(SUSUWU_NULLPTR);
		execve(argv[0], &argv[0], &envp[0]); /* has `noreturn` */
	}
	exit(EXIT_FAILURE); /* execv*() has `noreturn`. NOLINT(concurrency-mt-unsafe) */
#else /* ndef SUSUWU_POSIX */
	if(1 != argvS.size()) {
		SUSUWU_ERROR("if(1 != argvS.size()) { /* TODO: non-POSIX systems (such as Win32) with multiple commands */ }");
		return -1;
	}
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	if(CreateProcess(
		NULL,    /* No module name (use command line) */
		argv[0], /* Command line */
		NULL,    /* Process handle not inheritable */
		NULL,    /* Thread handle not inheritable */
		FALSE,   /* Set handle inheritance to FALSE */
		0,       /* No creation flags */
		NULL,    /* Use parent's environment block */
		NULL,    /* Use parent's working directory */
		&si,     /* Pointer to STARTUPINFO structure */
		&pi)     /* Pointer to PROCESS_INFORMATION structure */
	) {
		SUSUWU_NOTICE("execvesFork(" + classIoColoredParamStr(argvS) + ", " + classIoColoredParamStr(envpS) + ") { if(WIFEXITED(wstatus) && 0 != WEXITSTATUS(wstatus)) { /* (preview) Win32 code */ if(CreateProcess(...)) { /* started, non-blocking */ }}}");
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	} else {
		SUSUWU_NOTICE("execvesFork(" + classIoColoredParamStr(argvS) + ", " + classIoColoredParamStr(envpS) + ") { if(WIFEXITED(wstatus) && 0 != WEXITSTATUS(wstatus)) { /* (preview) Win32 code */ if(!CreateProcess(...)) { /* failed to launch */ \"GetLastError()\" == \"" SUSUWU_SH_PURPLE + std::to_string(GetLastError()) + SUSUWU_SH_DEFAULT "\" ...); }}}");
	}
	return 0;
#endif /* ndef SUSUWU_POSIX */
}
const pid_t execvesFork(const std::vector<std::string> &argvS, const std::vector<std::string> &envpS) SUSUWU_NOEXCEPT {
	try {
		return execvesForkThrow(argvS, envpS);
	} catch (const std::bad_alloc &e) {
		errno = ENOMEM;
		return -1;
	} catch (const std::exception &e) { /* [`clang-tidy` gives "warning: an exception may be thrown in function 'execvesFork' which should not throw exceptions [bugprone-exception-escape]"](https://stackoverflow.com/questions/49426460/) fix. */
		errno = EFAULT;
		return -1;
	}
}
const int execves(const std::vector<std::string> &argvS, const std::vector<std::string> &envpS) {
#ifdef SUSUWU_POSIX
	const pid_t pid = execvesFork(argvS, envpS);
	int wstatus = 0;
	if(-1 == pid) {
		throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "execves: -1 == execvesFork()"));
	}
	waitpid(pid, &wstatus, 0);
/* NOLINTBEGIN(misc-include-cleaner): `clang-tidy` can't detect `sys/wait.h` definitions of macros */
	if(WIFEXITED(wstatus) && 0 != WEXITSTATUS(wstatus)) {
		SUSUWU_NOTICE("execves(" + classIoColoredParamStr(argvS) + ", " + classIoColoredParamStr(envpS) + ") {if(WIFEXITED(wstatus) && 0 != WEXITSTATUS(wstatus)) {SUSUWU_NOTICE(... \"WEXITSTATUS(wstatus) is " SUSUWU_SH_PURPLE + std::to_string(WEXITSTATUS(wstatus)) + SUSUWU_SH_DEFAULT "\" ...);}}");
	} else if(WIFSIGNALED(wstatus)) {
		SUSUWU_NOTICE("execves(" + classIoColoredParamStr(argvS) + ", " + classIoColoredParamStr(envpS) + ") {if(WIFSIGNALED(wstatus)) {SUSUWU_NOTICE(... \"WTERMSIG(wstatus) is " SUSUWU_SH_PURPLE + std::to_string(WTERMSIG(wstatus)) + SUSUWU_SH_DEFAULT "\" ...);}}");
	} /* NOLINTEND(misc-include-cleaner): `clang-tidy` on */
	return wstatus;
#else /* ndef SUSUWU_POSIX */
	if(1 != argvS.size()) {
		throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "execves: if(1 != argvS.size()) { /* TODO: non-POSIX systems (such as Win32) with multiple commands */ }"));
	}
	const int status = system(argvS[0].c_str());
	if(status) { /* cppcheck-suppress duplicateBranch */
		SUSUWU_NOTICE("execves(" + classIoColoredParamStr(argvS) + ", " + classIoColoredParamStr(envpS) + ") { if(WIFEXITED(wstatus) && 0 != WEXITSTATUS(wstatus)) { /* (preview) Win32 code */ if(!CreateProcess(...)) { /* failed to launch */ \"GetLastError()\" == \"" SUSUWU_SH_PURPLE + std::to_string(GetLastError()) + SUSUWU_SH_DEFAULT "\" ...); }}}");
	} else {
		SUSUWU_NOTICE("execves(" + classIoColoredParamStr(argvS) + ", " + classIoColoredParamStr(envpS) + ") { if(WIFEXITED(wstatus) && 0 != WEXITSTATUS(wstatus)) { /* (preview) Win32 code */ if(CreateProcess(...)) { /* started, blocking */ }}}");
	}
	return status;
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
#	ifdef __APPLE__ //TODO: [execute new process to do this?](https://stackoverflow.com/questions/2483755/how-to-programmatically-gain-root-privileges/35316538#35316538)
#	else //TODO: spawn new process to do this? https://stackoverflow.com/questions/34723861/calling-a-c-function-with-root-privileges-without-executing-the-whole-program/70149223#70149223 https://stackoverflow.com/questions/70615937/how-to-run-a-command-as-root-with-c-or-c-with-no-pam-in-linux-with-password-au https://stackoverflow.com/questions/2483755/how-to-programmatically-gain-root-privileges/2483789#2483789
		/* TODO: polkit? Until this is finished, you must use chmod (to give setuid to executable), or execute new processes (with `sudo`/`su`) if you wish to use firewall/antivirus (which require root) */
#	endif /* __APPLE__ else */
#endif /* 0 */
	} else {
#	if 0 && defined LINUX /* TODO: [use `pam_loginuid.so(8)` to do this?](https://stackoverflow.com/questions/10272784/how-do-i-get-the-users-real-uid-if-the-program-is-run-with-sudo/10272881#10272881) */
		uid_t sudoUid = audit_getloginuid();
#	else /* !def linux */
		uid_t sudoUid = getuid();
		if(0 == sudoUid) {
			const char *sudoUidStr = getenv("SUDO_UID") /* NOLINT(concurrency-mt-unsafe) */;
			char *sudoUidStrIt = SUSUWU_NULLPTR;
			if(SUSUWU_NULLPTR == sudoUidStr) {
				SUSUWU_WARNING("classSysSetRoot(false) {(SUSUWU_NULLPTR == getenv(\"SUDO_UID\")) /* stuck as root */}");
				return true;
			} else {
				static const int baseDecimal = 10;
				sudoUid = static_cast<uid_t>(strtol(sudoUidStr, &sudoUidStrIt, baseDecimal));
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
// #elif defined SUSUWU_WIN32 /* TODO: [spawn new process to do this?](https://stackoverflow.com/questions/6418791/requesting-administrator-privileges-at-run-time) */
#else
	SUSUWU_WARNING("classSysSetRoot(bool) {#ifndef SUSUWU_POSIX /* TODO */}");
#endif /* SUSUWU_POSIX */
	return classSysHasRoot();
}

#if SUSUWU_UNIT_TESTS
const bool classSysTests() {
	bool retval = true; /* TODO: choose all errors throw exceptions, or choose all errors return error values. Most of the other unit tests use exceptions, but `echo` is the best test for `execves`/`execvex`. */
	std::cout << "	execves(): " << std::flush;
	(EXIT_SUCCESS == execves({"/bin/echo", "pass"})) || (retval = false) || (std::cout << "error" << std::endl);
	std::cout << "	execvex(): " << std::flush;
	(EXIT_SUCCESS == execvex("/bin/echo pass")) || (retval = false) || (std::cout << "error" << std::endl);
	return retval;
}
#endif /* SUSUWU_UNIT_TESTS */
```

`less `[`cxx/ClassNumeral.hxx`](../cxx/ClassNumeral.hxx)
```c++
/* Scalar functions of numerals. */
template<typename Numeral>
inline const Numeral numeralRamp(const Numeral num, const Numeral ramp = std::numeric_limits<Numeral>::epsilon()) {
	if(!(num >= ramp)) { /* the `!` form is required to catch `inf` and `nan` */
		return ramp; /* `ramp = epsilon` to prevent "division-by-error" errors */
	}
	return num;
}

/* Vector / tensor functions of numerals. */ /* TODO: containers as templates, to allow all containers which have random access (such as `tensorflow::Tensor`) */
typedef long double NumeralAccum;
template<typename Numeral>
const NumeralAccum numeralAverage(const std::vector<Numeral> &nums) {
	NumeralAccum total = 0;
	for(const auto &num : nums) {
		total += num; /* TODO: catch overflows and recompute with per-iteration division? */
	}
	return total / nums.size(); /* single division formula uses less CPU */
}
template<std::size_t tupleIndex, typename Numeral>
const NumeralAccum numeralAverage(const std::vector<std::tuple<Numeral, Numeral>> &nums) {
	NumeralAccum total = 0;
	for(const auto &num : nums) {
		total += std::get<tupleIndex>(num); /* TODO: catch overflows and recompute with per-iteration division? */
	}
	return total / nums.size();
}
template<typename Numeral>
const Numeral numeralMagnitude(const std::vector<Numeral> &nums) {
	Numeral found = 0;
	for(const auto &num : nums) {
		found = std::max(std::abs(num), found);
	}
	return numeralRamp(found);
}
template<std::size_t tupleIndex, typename Numeral>
const Numeral numeralMagnitude(const std::vector<std::tuple<Numeral, Numeral>> &nums) {
	Numeral found = 0;
	for(const auto &numTuple : nums) {
		const Numeral num = std::get<tupleIndex>(numTuple);
		found = std::max(std::abs(num), found);
	}
	return numeralRamp(found);
}
template<typename Numeral>
const Numeral numeralBalancedMagnitude(const std::vector<Numeral> &nums, const NumeralAccum average) {
	Numeral found = 0;
	for(const auto &num : nums) {
		const Numeral balancedNum = num - average;
		found = std::max(std::abs(balancedNum), found);
	}
	return numeralRamp(found);
}
template<std::size_t tupleIndex, typename Numeral>
const Numeral numeralBalancedMagnitude(const std::vector<std::tuple<Numeral, Numeral>> &nums, const NumeralAccum average) {
	Numeral found = 0;
	for(const auto &numTuple : nums) {
		const Numeral balancedNum = std::get<tupleIndex>(numTuple) - average;
		found = std::max(std::abs(balancedNum), found);
	}
	return numeralRamp(found);
}

/* Structures which hold values used for normalization-functions upon numerals */
/* NOLINTBEGIN(google-explicit-constructor,hicpp-explicit-conversions) */
typedef struct NumeralNormalizers : public Object {
	SUSUWU_VIRTUAL_DEFAULTS(Susuwu::NumeralNormalizers) /* `getName()`, `isPureVirtual()`, `operator==`()`, ... */
	NumeralNormalizers(const NumeralAccum avg = 0.0, const NumeralAccum mag = 1.0) : average(avg), magnitude(mag) {} /* NOLINT(bugprone-easily-swappable-parameters); overzealous heuristic */ /* cppcheck-suppress noExplicitConstructor */
	template<typename Numeral>
	NumeralNormalizers(const std::vector<Numeral> &nums) : average(numeralAverage(nums)), magnitude(numeralBalancedMagnitude(nums, average)) {} /* cppcheck-suppress noExplicitConstructor */
	template<std::size_t tupleIndex, typename Numeral>
	static const NumeralNormalizers fromTuple(const std::vector<std::tuple<Numeral, Numeral>> &nums) {
		const NumeralAccum avg = numeralAverage<tupleIndex>(nums);
		return NumeralNormalizers(avg, numeralBalancedMagnitude<tupleIndex>(nums, avg));
	}
	NumeralAccum average, magnitude;
} NumeralNormalizers;
typedef struct NumeralNormalizersReciprocal : public Object { /* is as `NumeralNormalizers`, except `magnitude` is replaced with `magnitudeReciprocal` */
	SUSUWU_VIRTUAL_DEFAULTS(Susuwu::NumeralNormalizersReciprocal) /* `getName()`, `isPureVirtual()`, `operator==`()`, ... */
	NumeralAccum average, magnitudeReciprocal;
	NumeralNormalizersReciprocal(const NumeralAccum avg = 0.0, const NumeralAccum magRecip = 1.0) : average(avg), magnitudeReciprocal(magRecip) {} /* NOLINT(bugprone-easily-swappable-parameters); overzealous heuristic */ /* cppcheck-suppress noExplicitConstructor */
	template<typename Numeral>
	NumeralNormalizersReciprocal(const std::vector<Numeral> &nums) : average(numeralAverage(nums)), magnitudeReciprocal(1 / numeralBalancedMagnitude(nums, average)) {} /* cppcheck-suppress noExplicitConstructor */
	template<std::size_t tupleIndex, typename Numeral>
	static const NumeralNormalizersReciprocal fromTuple(const std::vector<std::tuple<Numeral, Numeral>> &nums) {
		const NumeralAccum avg = numeralAverage<tupleIndex>(nums);
		return NumeralNormalizersReciprocal(avg, 1 / numeralBalancedMagnitude<tupleIndex>(nums, avg));
	}
	NumeralNormalizersReciprocal(const NumeralNormalizers &normalizers) : average(normalizers.average), magnitudeReciprocal(1 / normalizers.magnitude) {} /* cppcheck-suppress noExplicitConstructor */
	operator NumeralNormalizers() const { return { average, 1 / magnitudeReciprocal }; }
} NumeralNormalizersReciprocal;
/* NOLINTEND(google-explicit-constructor,hicpp-explicit-conversions) */

/* Normalization functions which map vectors / tensors to `[-1, 1]` to suit all activation functions (which include transcendentals such as hyperbolic `tan`) */
template<typename Numeral>
inline const Numeral numeralNormalization(const Numeral num, const NumeralNormalizersReciprocal &normalizers) {
	return (num - normalizers.average) * normalizers.magnitudeReciprocal;
}
template<typename Numeral>
void numeralNormalization(std::vector<Numeral> &nums, const NumeralNormalizersReciprocal &normalizers) {
	for(auto &num : nums) {
		num = numeralNormalization(num, normalizers);
	}
}
template<typename Numeral>
const NumeralNormalizersReciprocal numeralNormalization(std::vector<Numeral> &nums) {
	const NumeralNormalizersReciprocal normalizers(nums);
	numeralNormalization(nums, normalizers);
	return normalizers;
}
#if SUSUWU_CLASSNUMERAL_REDUNDANT /* denormalization is fastest with `magnitudeReciprocal`; implicit conversion makes `magnitude` versions redundant */
template<typename Numeral>
inline const Numeral numeralNormalization(const Numeral num, const NumeralNormalizers &normalizers) {
	return (num - normalizers.average) / normalizers.magnitude;
}
template<typename Numeral>
void numeralNormalization(std::vector<Numeral> &nums, const NumeralNormalizers &normalizers) {
	numeralNormalization(nums, NumeralNormalizersReciprocal(normalizers));
}
#endif /* SUSUWU_CLASSNUMERAL_REDUNDANT */

/* Denormalization functions which map `[-1, 1]` back to original input values */
template<typename Numeral>
inline const Numeral numeralDenormalization(const Numeral num, const NumeralNormalizers &normalizers) {
	return (num * normalizers.magnitude) + normalizers.average;
}
template<typename Numeral>
void numeralDenormalization(std::vector<Numeral> &nums, const NumeralNormalizers &normalizers) {
	for(auto &num : nums) {
		num = numeralDenormalization(num, normalizers);
	}
}
template<typename Numeral>
const NumeralNormalizers numeralDenormalization(std::vector<Numeral> &nums) {
	const NumeralNormalizers normalizers(nums);
	numeralDenormalization(nums, normalizers);
	return normalizers;
}
#if SUSUWU_CLASSNUMERAL_REDUNDANT /* denormalization is fastest with `magnitude`; implicit conversion makes `magnitudeReciprocal` versions redundant */
template<typename Numeral>
inline const Numeral numeralDenormalization(const Numeral num, const NumeralNormalizersReciprocal &normalizers) {
	return (num / normalizers.magnitudeReciprocal) + normalizers.average;
}
template<typename Numeral>
void numeralDenormalization(std::vector<Numeral> &nums, const NumeralNormalizersReciprocal &normalizers) {
	numeralDenormalization(nums, NumeralNormalizers(normalizers));
}
#endif /* SUSUWU_CLASSNUMERAL_REDUNDANT */
```

`less `[`cxx/ClassSha2.hxx`](../cxx/ClassSha2.hxx)
```c++
/* const */ ClassIoHash /* 128 bits, not null-terminated */ classSha1(const ClassIoBytecode &bytecode);
/* const */ ClassIoHash /* 256 bits, not null-terminated */ classSha256(const ClassIoBytecode &bytecode);
/* const */ ClassIoHash /* 512 bits, not null-terminated */ classSha512(const ClassIoBytecode &bytecode);
typedef ClassIoHash (*ClassSha2)(const ClassIoBytecode &bytecode);
extern ClassSha2 classSha2/* = classSha256 */; /* To compress, apps can execute `classSha2 = classSha1;`. To double hash sizes, execute `classSha2 = classSha512;`. (Notice: this does not recompute hashes which exist) */
#if SUSUWU_UNIT_TESTS
const bool classSha2Tests();
const bool classSha2TestsNoexcept() SUSUWU_NOEXCEPT;
#endif /* SUSUWU_UNIT_TESTS */
```

`less `[`cxx/ClassSha2.cxx`](../cxx/ClassSha2.cxx)
```c++
/* Uses https://www.rfc-editor.org/rfc/rfc6234#section-8.2.2 */
ClassSha2 classSha2 = classSha256;
/* const */ ClassIoHash /* 128 bits, not null-terminated */ classSha1(const ClassIoBytecode &bytecode) {
	ClassIoHash result;
	SHA1Context context;
	result.resize(SHA1HashSize);
	SHA1Reset(&context); /* If `undefined symbol: SHA1Reset`, affix `classSha1.o` to `${LD_FLAGS}` */
	SHA1Input(&context, reinterpret_cast<const unsigned char *>(&bytecode[0]), bytecode.size());
	SHA1Result(&context, reinterpret_cast<unsigned char *>(&result[0]));
	return result;
}
/* const */ ClassIoHash /* 256 bits, not null-terminated */ classSha256(const ClassIoBytecode &bytecode) {
	ClassIoHash result;
	SHA256Context context;
	result.resize(SHA256HashSize);
	SHA256Reset(&context); /* If `undefined symbol: SHA256Reset`, affix `classSha224-256.o` to `${LD_FLAGS}` */
	SHA256Input(&context, reinterpret_cast<const unsigned char *>(&bytecode[0]), bytecode.size());
	SHA256Result(&context, reinterpret_cast<unsigned char *>(&result[0]));
	return result;
}
/* const */ ClassIoHash /* 512 bits, not null-terminated */ classSha512(const ClassIoBytecode &bytecode) {
	ClassIoHash result;
	SHA512Context context;
	result.resize(SHA512HashSize);
	SHA512Reset(&context); /* If `undefined symbol: SHA512Reset`, affix `sha384-512.o` to `${LD_FLAGS}` */
	SHA512Input(&context, reinterpret_cast<const unsigned char *>(&bytecode[0]), bytecode.size());
	SHA512Result(&context, reinterpret_cast<unsigned char *>(&result[0]));
	return result;
}

#if SUSUWU_UNIT_TESTS
const bool classSha2Tests() { /* is just to test glue code (which wraps rfc6234). Use `../c/rfc6234/shatest.c` to test rfc6234. */
	const size_t benchmarkSz = 65536; /* 65536 == 2^16 == 64kb */
	const char nulls[benchmarkSz] = {0}; /* NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays); this was used to allow zero-initialism semantics with no runtime cost */
	const std::string nullStr(static_cast<const char *>(nulls), &nulls[benchmarkSz]);
	const ClassSysUSeconds tsDrift = classSysUSecondClock(), ts2Drift = classSysUSecondClock() - tsDrift, ts = classSysUSecondClock();
	const ClassIoHash hash = classSha2(nullStr);
	const ClassSysUSeconds ts2 = classSysUSecondClock() - ts2Drift;
	const std::string hashStrCompute = (SUSUWU_HEX_DOES_PREFIX ? "" : "0x") + classIoHexStr(hash);
	const std::string hashStrTrue = "0xde2f256064a0af797747c2b97505dc0b9f3df0de4f489eac731c23ae9ca9cc31";
	if(ts == ts2) {
		SUSUWU_WARNING("0 ms (0 μs) to compute `classSha2(std::string(nulls, &nulls[" + std::to_string(benchmarkSz) + "])) == " + hashStrCompute + "` = inf mbps");
	} else {
		SUSUWU_INFO(std::to_string((ts2 - ts) / 1000) + " ms (" + std::to_string(ts2 - ts) + " μs) to compute `classSha2(std::string(nulls, &nulls[" + std::to_string(benchmarkSz) + "])) == " + hashStrCompute + "` = " + std::to_string(float(benchmarkSz) * CHAR_BIT /* to bits */ / (ts2 - ts) /* to bpμs */ * 1000000 /* to bps */ / (1 << 20) /* to mbps */) + "mbps");
	}
	SUSUWU_NOTICE("This `classSha2()` is from `./build.sh --debug`: `./build.sh --release` has 2x this throughput");
	if(0 == hash.size()) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "`0 == classSha2(std::string()).size();"));
	} else if(hashStrTrue.size() != hashStrCompute.size() && classSha256 == classSha2) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "`classSha2 = classSha256;`, but `(" + std::to_string(hash.size()) + " == sha2(std::string()).size())`"));
	} else if(hashStrTrue.size() != hashStrCompute.size()) {
		SUSUWU_INFO("`(classSha256 != classSha2)`, `(" + std::to_string(hash.size()) + " == sha2(std::string()).size())`");
	} else if(hashStrTrue != hashStrCompute) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "classSha2(char nulls[65535] = {0}) did not compute " + hashStrTrue));
	}
	return true;
}
const bool classSha2TestsNoexcept() SUSUWU_NOEXCEPT { return templateCatchAll(classSha2Tests, "classSha2Tests()"); } /* cppcheck-suppress throwInNoexceptFunction */
#endif /* SUSUWU_UNIT_TESTS */
```

`less `[`cxx/ClassResultList.hxx`](../cxx/ClassResultList.hxx)
```c++
typedef ClassIoHash ResultListHash;
typedef ClassIoBytecode ResultListBytecode; /* Should have structure of ClassIoBytecode, but is not just for files, can use for UTF8/webpages, so have a new type for this */
typedef ClassIoPath ResultListSignature; /* TODO: `typedef ResultListBytecode ResultListSignature; ResultListSignature("string literal");` */
typedef ptrdiff_t BytecodeOffset; /* all tests of `ResultListBytecode` should return `{BytecodeOffset, X}` (with the most common `X` as `ResultListHash` or `ResultListSignature`). `offset = -1` if no match */
typedef struct ResultList : public Object { /* Lists of {metadata, executables (or pages)} */
	SUSUWU_VIRTUAL_DEFAULTS(Susuwu::ResultList) /* `getName()`, `isPureVirtual()`, `operator==`()`, ... */
	typedef std::unordered_set<ResultListHash> Hashes;
	Hashes hashes; /* Checksums of executables (or pages); to avoid duplicates, plus to do constant ("O(1)") test for which executables (or pages) exists */
	typedef std::vector<ResultListSignature> Signatures;
	Signatures signatures; /* Smallest substrings (or regexes, or Universal Resource Locators) which can identify `bytecodes`; has uses close to `hashes`, but can match if executables (or pages) have small differences */
	typedef std::vector<ResultListBytecode> Bytecodes;
	Bytecodes bytecodes; /* Whole executables (for `VirusAnalysis`) or webpages (for `AssistantCns`); huge disk usage, just load this for signature synthesis (or CNS backpropagation). */
} ResultList;

#if SUSUWU_UNIT_TESTS
const bool classResultListTests(); /* TODO: test most of `ClassResultList*` */
static const bool classResultListTestsNoexcept() SUSUWU_NOEXCEPT { return templateCatchAll(classResultListTests, "classResultListTests()"); } /* cppcheck-suppress throwInNoexceptFunction */
#endif /* SUSUWU_UNIT_TESTS */

template<class List>
const size_t listMaxSize(const List &list) {
#if SUSUWU_PREFER_CSTR
	size_t max = 0;
	for(auto it = &list[0]; list.cend() != it; ++it) { const size_t temp = strlen(*it); if(temp > max) { max = temp; } }
	return max; /* WARNING! `strlen()` just does UTF8-strings/hex-strings; if binary, must use `it->size()` */
#else /* else !SUSUWU_PREFER_CSTR */
	auto it = std::max_element(list.cbegin(), list.cend(), [](const typename List::const_iterator::value_type &s, const typename List::const_iterator::value_type &x) { return s.size() < x.size(); });
	return it->size();
#endif /* SUSUWU_PREFER_CSTR else */
}

typedef enum ListFormat : char {
	listFormatInitializer, /* style: C or C++ */
	listFormatJson /* style: Java or JavaScript */
} ListFormat;
template<class List, class Os>
void listDumpTo(const List &list, Os &os, const bool index, const bool whitespace, const bool pascalValues, const bool finalList = true, const ListFormat listFormat = listFormatInitializer) {
	bool quoteValues = false;
	size_t index_ = 0;
	switch(listFormat) {
		case listFormatInitializer:
			os << '{';
		break;
		case listFormatJson:
			os << '[';
			quoteValues = true;
		break;
	}
#ifdef SUSUWU_LIST_COUNT
	os << list.size() << ':';
#endif /* def SUSUWU_LIST_COUNT */
	const std::string assignment = whitespace ? " = " : "=";
	for(const auto &value : list) {
		if(0 != index_) {
			os << ',';
		}
		if(whitespace) {
			os << std::endl << '\t';
		}
		if(index) {
			os << index_ << assignment;
		}
		if(quoteValues) {
			os << '"';
		}
		if(pascalValues) {
			os << value.size() << ':' /* TODO: replace "%Dec:" with "%Bin" */ << value;
		} else {
#if !SUSUWU_HEX_DOES_PREFIX
			os << "0x";
#endif /* !SUSUWU_HEX_DOES_PREFIX */
			classIoHexOs(os, value);
		}
		if(quoteValues) {
			os << '"';
		}
		++index_;
	}
	if(whitespace) {
		os << '\n';
	}
	switch(listFormat) {
		case listFormatInitializer:
			os << "};";
		break;
		case listFormatJson:
			os << (finalList ? "]" : "],");
		break;
	}
	if(whitespace) {
		os << std::endl;
	}
} /* view `ClassResultList.cxx:classResultListTests()` for examples of output from `listDumpTo()`+`resultListDumpTo()`. */
template<class Os>
void resultListDumpToAssignment(const std::string &listStr, Os &os, const bool whitespace, const ListFormat listFormat = listFormatInitializer) {
	switch(listFormat) {
		case listFormatInitializer:
			os << listStr << (whitespace ? " = " : "=");
		break;
		case listFormatJson:
			os << '"' << listStr << '"' << (whitespace ? ": " : ":");
		break;
	}
}
template<class List, class Os>
void resultListDumpTo(const List &list, Os &os, const bool index, const bool whitespace, const bool pascalValues, const ListFormat listFormat = listFormatInitializer) {
	if(listFormatJson == listFormat) {
		os << '{';
		if(whitespace) {
			os << '\n';
		}
	}
	resultListDumpToAssignment("list.hashes", os, whitespace, listFormat);
	listDumpTo(list.hashes, os, index, whitespace, pascalValues, false, listFormat);
	resultListDumpToAssignment("list.signatures", os, whitespace, listFormat);
	listDumpTo(list.signatures, os, index, whitespace, pascalValues, false, listFormat);
	resultListDumpToAssignment("list.bytecodes", os, whitespace, listFormat);
	listDumpTo(list.bytecodes, os, index, whitespace, pascalValues, true, listFormat);
	if(listFormatJson == listFormat) {
		os << '}';
	}
}
template<class List, class Is>
void listLoadFrom(List &list, Is &is, const bool index, const bool whitespace, const bool pascalValues, const bool finalList = true, const ListFormat listFormat = listFormatInitializer) { /* NOLINT(readability-function-cognitive-complexity) TODO: inner loops depend on shared outer variables, how to factor those out? */
	const bool whitespaceFrom = SUSUWU_IO_WHITESPACE && whitespace;
	bool quoteValues = false;
	char delim = '\0';
	char terminator = '\0';
	is >> delim;
	switch(listFormat) {
		case listFormatInitializer:
			classIoCheckChar(__func__, '{', delim);
			terminator = '}';
		break;
		case listFormatJson:
			classIoCheckChar(__func__, '[', delim);
			terminator = ']';
			quoteValues = true;
		break;
		default:
			SUSUWU_UNREACHABLE;
	}
//	decltype(list.front()) value;
#ifdef SUSUWU_LIST_COUNT
	size_t count = 0;
	is >> count;
	is >> delim;
	classIoCheckChar(__func__, ':', delim);
	list.reserve(count);
#endif /* def SUSUWU_LIST_COUNT */
	const std::string assignment = whitespaceFrom ? " = " : "=";
	const std::string hexPrefix = (whitespace && !quoteValues) ? " 0" : "0";
	for(size_t index_ = 0; is.good(); ++index_) {
		std::string token;
		const std::streampos pos = is.tellg();
		if(whitespaceFrom) {
			is >> delim;
			classIoCheckChar(__func__, '\n' /* TODO: support '\r'? */, delim);
		}
		is >> delim;
		if(terminator == delim) {
#ifdef SUSUWU_LIST_COUNT
			classIoCheckSz(__func__, count, index_);
#endif /* def SUSUWU_LIST_COUNT */
			break;
		}
		if(0 == index_) {
			is.seekg(pos);
		} else {
			classIoCheckChar(__func__, ',', delim);
		}
		if(whitespaceFrom) {
			is >> delim;
			classIoCheckChar(__func__, '\t', delim); /* is >> token; classIoCheckStr(__func__, "\n\t", token); */
		}
		if(index) {
			is >> token;
			classIoCheckStr(__func__, std::to_string(index_), token);
			is >> token;
			classIoCheckStr(__func__, assignment, token);
		}
		typename List::value_type value;
		if(quoteValues) {
			is >> delim;
			classIoCheckChar(__func__, '"', delim);
		}
		if(pascalValues) {
			std::streamsize tokenSz = 0;
			is >> tokenSz;
			is >> delim;
			classIoCheckChar(__func__, ':' /* TODO: replace "%Dec:" with "%Bin" */, delim);
			value.resize(tokenSz);
			if(0 < tokenSz) {
				is.read(&value[0], tokenSz); /* `is >> value;` won't do binary code */
				if(is.gcount() < tokenSz) {
					value.resize(is.gcount());
				}
				classIoCheckSz(__func__, is.gcount(), tokenSz);
			}
		} else {
#if !SUSUWU_HEX_DOES_PREFIX
//			is >> token; classIoCheckStr(__func__, "0x", token);
			std::getline(is, token, 'x'); /* used `std::getline` so 'x' is swallowed */
			classIoCheckStr(__func__, hexPrefix, token);
#endif /* !SUSUWU_HEX_DOES_PREFIX */
			classIoHexIs(is, value); /* can do `is >> std::hex >> tokenSz;` but not `>> token` (or `>> value`) */
		}
		if(quoteValues) {
			is >> delim;
			classIoCheckChar(__func__, '"', delim);
		}
		list.insert(list.end(), value); /* list.push_back(value); */
	}
	classIoCheckChar(__func__, terminator, delim);
	switch(listFormat) {
		case listFormatInitializer:
			is >> delim;
			classIoCheckChar(__func__, ';', delim);
		break;
		case listFormatJson:
			if(!finalList) {
				is >> delim;
				classIoCheckChar(__func__, ',', delim);
			}
		break;
	}
	if(whitespaceFrom) {
		is >> delim;
		classIoCheckChar(__func__, '\n', delim);
	}
} /* view `ClassResultList.cxx:classResultListTests()` for examples of input from `listLoadFrom()`+`resultListLoadFrom`. */
template<class Is>
void resultListLoadFromAssignment(const std::string &listStr, Is &is, const bool whitespace, const ListFormat listFormat = listFormatInitializer) {
	const bool whitespaceFrom = SUSUWU_IO_WHITESPACE && whitespace;
	std::string token;
	switch(listFormat) {
		case listFormatInitializer:
			classIoGetline(is, token, '{');
			classIoCheckStr(__func__, listStr + (whitespaceFrom ? " = " : "="), token);
		break;
		case listFormatJson:
			classIoGetline(is, token, '[');
			classIoCheckStr(__func__, '"' + listStr + '"' + (whitespaceFrom ? ": " : ":"), token);
		break;
	}
}
template<class List, class Is>
void resultListLoadFrom(List &list, Is &is, const bool index, const bool whitespace, const bool pascalValues, const ListFormat listFormat = listFormatInitializer) {
	const bool whitespaceFrom = SUSUWU_IO_WHITESPACE && whitespace;
	char delim = '\0';
	if(listFormatJson == listFormat) {
		is >> delim;
		classIoCheckChar(__func__, '{', delim);
		if(whitespaceFrom) {
			is >> delim;
			classIoCheckChar(__func__, '\n', delim);
		}
	}
	resultListLoadFromAssignment("list.hashes", is, whitespace, listFormat);
	listLoadFrom(list.hashes, is, index, whitespace, pascalValues, false, listFormat);
	resultListLoadFromAssignment("list.signatures", is, whitespace, listFormat);
	listLoadFrom(list.signatures, is, index, whitespace, pascalValues, false, listFormat);
	resultListLoadFromAssignment("list.bytecodes", is, whitespace, listFormat);
	listLoadFrom(list.bytecodes, is, index, whitespace, pascalValues, true, listFormat);
	if(listFormatJson == listFormat) {
		is >> delim;
		classIoCheckChar(__func__, '}', delim);
	}
}

template<class List, class List2>
/*	@pre @code !(list.empty() || hashes.full()) @endcode
 *	@post @code !hashes.empty() @endcode */
void listToHashes(const List &list /* ResultList::bytecodes or ResultList::hex*/, List2 &hashes /* ResultList::hashess */) {
	SUSUWU_OMP_PRAGMA(omp parallel for) /* TODO: ensure OpenMP won't cause problems if `List` has dependence on previous values (such as if `List`: is sorted, is a binary tree or heap, or is a hashmap) */
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
//	SUSUWU_OMP_PRAGMA(omp simd) /* "error: statement after '#pragma omp simd' must be a for loop"
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
//	SUSUWU_OMP_PRAGMA(omp parallel for) /* "error: cannot return from OpenMP region". `break;` version gives "error: 'break' statement cannot be used in OpenMP for loop" */
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
 * Usage: `auto tuple = listProduceSignature(resultList.bytecodes, bytecode); resultList.signatures.push_back({std::get<0>(tuple), std::get<1>(tuple)});` */
const std::tuple<typename List::value_type::const_iterator, typename List::value_type::const_iterator> listProduceSignature(const List &list, const typename List::value_type &value) {
	ptrdiff_t smallest = value.size();
	auto itBegin = value.cbegin(), itEnd = value.cend();
	SUSUWU_OMP_PRAGMA(omp parallel for) /* TODO: ensure OpenMP won't cause problems if `List` has dependence on previous values (such as if `List`: is sorted, is a binary tree or heap, or is a hashmap) */
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
/* Usage: `auto it = listFindSignatureOfValue(resultList.signatures, value)); if(it) { std::cout << "value has resultList.signatures[" << tohex(match.signature) << "]"; }` */
ResultListSignatureMatch listFindSignatureOfValue(const List &list, const typename List::value_type &value) {
//	SUSUWU_OMP_PRAGMA(omp parallel for) /* "error: cannot return from OpenMP region". `break;` version gives "error: 'break' statement cannot be used in OpenMP for loop" */
	for(const auto &signature : list) {
//		SUSUWU_OMP_PRAGMA(omp simd) /* new compilers know when to use SIMD opcodes. If the compiler chose not to, and the pragma causes it to, the overhead of SIMD can cause slow downs */ /* "error: statement after '#pragma omp simd' must be a for loop" */
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
/* Usage: `if(listHasSignatureOfValue(resultList.signatures, value)) { std::cout << "value has signature from ResultList.signatures"; }` */
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

`less `[`cxx/ClassResultList.cxx`](../cxx/ClassResultList.cxx)
```c++
#if SUSUWU_UNIT_TESTS
static void classResultListLoadFromTest(std::stringstream &is, const bool index, const bool whitespace, const bool pascalValues, const ListFormat listFormat, const std::string &expectedValue) {
//	std::cout << is.str();
	try {
		ResultList resultList;
		resultListLoadFrom(resultList, is, index, whitespace, pascalValues, listFormat);
		std::stringstream os2;
		resultListDumpTo(resultList, os2, index, whitespace, pascalValues, listFormat);
		if(expectedValue != os2.str()) {
			throw std::logic_error(SUSUWU_SH_RED + os2.str() + SUSUWU_SH_WHITE " == os2.str(); " SUSUWU_SH_GREEN + expectedValue + SUSUWU_SH_WHITE " != os2.str();");
		}
	} catch (std::exception &w) {
		throw std::logic_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, std::string("classResultListLoadFromTest(os, ") + (index ? "true" : "false") + ", " + (whitespace ? "true" : "false") + ", " + (pascalValues ? "true" : "false") + ", " + (listFormatInitializer == listFormat ? "listFormatInitializer" : "listFormatJson") + "); caught `std::exception::what() == \"" + w.what() + "\"`"));
	}
}
static void classResultListDumpToTest(const ResultList &resultList, const bool index, const bool whitespace, const bool pascalValues, const ListFormat listFormat, const std::string &expectedValue) {
	std::stringstream os;
	resultListDumpTo(resultList, os, index, whitespace, pascalValues, listFormat);
	if(expectedValue != os.str()) {
		throw std::logic_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, std::string("classResultListDumpToTest(resultList, ") + (index ? "true" : "false") + ", " + (whitespace ? "true" : "false") + ", " + (pascalValues ? "true" : "false") + ", " + (listFormatInitializer == listFormat ? "listFormatInitializer" : "listFormatJson") + "); " SUSUWU_SH_RED + os.str() + SUSUWU_SH_WHITE " == os.str(); " SUSUWU_SH_GREEN + expectedValue + SUSUWU_SH_WHITE " != os.str();")); /* TODO: standard macros for error/success colors, plus `SUSUWU_ERR` default color */
	}
	classResultListLoadFromTest(os, index, whitespace, pascalValues, listFormat, expectedValue);
}
const bool classResultListTests() {
	ResultList resultList;
	resultList.hashes.insert(ResultListHash({'\x32'})); /* `.hashes` is `std::unordered_set`, thus test just 1 value. */
	resultList.signatures = {"", "2"};
	resultList.bytecodes = {"", "02"};
#ifdef SUSUWU_LIST_COUNT
	const std::string listHashesSz = "1:", listSignaturesSz = "2:", listBytecodesSz = "2:";
#else /* def SUSUWU_LIST_COUNT else */
	const std::string listHashesSz = "", listSignaturesSz = "", listBytecodesSz = ""; /* NOLINT(readability-redundant-string-init): define that those are "". */
#endif /* ndef SUSUWU_LIST_COUNT */
	classResultListDumpToTest(resultList, false, false, false, listFormatInitializer, "list.hashes={" + listHashesSz + "0x32};list.signatures={" + listSignaturesSz + "0x,0x32};list.bytecodes={" + listBytecodesSz + "0x,0x3032};");
	classResultListDumpToTest(resultList, false, false, false, listFormatJson, "{\"list.hashes\":[" + listHashesSz + "\"0x32\"],\"list.signatures\":[" + listSignaturesSz + "\"0x\",\"0x32\"],\"list.bytecodes\":[" + listBytecodesSz + "\"0x\",\"0x3032\"]}");
	classResultListDumpToTest(resultList, true, true, false, listFormatInitializer, "list.hashes = {" + listHashesSz + "\n\t0 = 0x32\n};\nlist.signatures = {" + listSignaturesSz + "\n\t0 = 0x,\n\t1 = 0x32\n};\nlist.bytecodes = {" + listBytecodesSz + "\n\t0 = 0x,\n\t1 = 0x3032\n};\n");
	classResultListDumpToTest(resultList, true, true, false, listFormatJson, "{\n\"list.hashes\": [" + listHashesSz + "\n\t0 = \"0x32\"\n],\n\"list.signatures\": [" + listSignaturesSz + "\n\t0 = \"0x\",\n\t1 = \"0x32\"\n],\n\"list.bytecodes\": [" + listBytecodesSz + "\n\t0 = \"0x\",\n\t1 = \"0x3032\"\n]\n}");
	classResultListDumpToTest(resultList, false, false, true, listFormatInitializer, "list.hashes={" + listHashesSz + "1:2};list.signatures={" + listSignaturesSz + "0:,1:2};list.bytecodes={" + listBytecodesSz + "0:,2:02};");
	classResultListDumpToTest(resultList, false, false, true, listFormatJson, "{\"list.hashes\":[" + listHashesSz + "\"1:2\"],\"list.signatures\":[" + listSignaturesSz + "\"0:\",\"1:2\"],\"list.bytecodes\":[" + listBytecodesSz + "\"0:\",\"2:02\"]}");
	resultList.bytecodes = {"\001", "\002"};
	classResultListDumpToTest(resultList, false, false, true, listFormatInitializer, "list.hashes={" + listHashesSz + "1:2};list.signatures={" + listSignaturesSz + "0:,1:2};list.bytecodes={" + listBytecodesSz + "1:\001,1:\002};");
	classResultListDumpToTest(resultList, false, false, true, listFormatJson, "{\"list.hashes\":[" + listHashesSz + "\"1:2\"],\"list.signatures\":[" + listSignaturesSz + "\"0:\",\"1:2\"],\"list.bytecodes\":[" + listBytecodesSz + "\"1:\001\",\"1:\002\"]}");
	return true;
}
#endif /* SUSUWU_UNIT_TESTS */
```

`less `[`cxx/ClassCns.hxx`](../cxx/ClassCns.hxx)
```c++
#ifndef SUSUWU_CNS_VALUE_SEMANTICS
#	define SUSUWU_CNS_VALUE_SEMANTICS true
#endif /* ndef SUSUWU_CNS_VALUE_SEMANTICS */
#define SUSUWU_CNS_MODE_ENUM /* use `enum`s for multi-class indices */
typedef class Cns : public Object {
public:
	Cns() SUSUWU_DEFAULT /* Default constructor */
	Cns(const Cns &) SUSUWU_DEFAULT /* Copy constructor */
	Cns& operator=(const Cns &) SUSUWU_DEFAULT /* Copy assignment */
	Cns(Cns&&) SUSUWU_NOEXCEPT SUSUWU_DEFAULT /* Move constructor */
	Cns& operator=(Cns &&) SUSUWU_NOEXCEPT SUSUWU_DEFAULT /* Move assignment */
	~Cns() SUSUWU_OVERRIDE SUSUWU_DEFAULT
#if SUSUWU_CNS_VALUE_SEMANTICS
#	undef SUSUWU_CLASS_OPERATOREQUALTO
#	define SUSUWU_CLASS_OPERATOREQUALTO(noop) ; /* NOLINT(cppcoreguidelines-macro-usage) */
	bool operator==(const Class &obj) const SUSUWU_OVERRIDE { /* NOLINT(fuchsia-overloaded-operator) */
//		SUSUWU_VIRTUAL_OPERATOREQUALTO_WITH_VPTR /* shallow comparison, with virtual pointer included */
//		return this->hashCode() == obj.hashCode();
		const Cns *thisFlow = dynamic_cast<const Cns *>(this);
		const Cns *objFlow = dynamic_cast<const Cns *>(&obj);
		return typeid(*this) == typeid(obj) &&
			thisFlow->inputMode == objFlow->inputMode &&
			thisFlow->outputMode == objFlow->outputMode &&
			thisFlow->inputNeurons == objFlow->inputNeurons &&
			thisFlow->outputNeurons == objFlow->outputNeurons &&
			thisFlow->neuronsPerLayer == objFlow->neuronsPerLayer &&
			thisFlow->layersOfNeurons == objFlow->layersOfNeurons;
	}
	const bool equals(const Object &obj) const SUSUWU_OVERRIDE { return this->getClass().operator==(obj); }
	const SUSUWU_INTPTR hashCode() const SUSUWU_OVERRIDE SUSUWU_VIRTUAL_HASHCODE /* Shallow hash code. TODO: ensure that derivatives of `class Cns` override this to hash container values */
#endif /* SUSUWU_CNS_VALUE_SEMANTICS */
	SUSUWU_PURE_VIRTUAL_DEFAULTS(Susuwu::Cns) /* `getName()`, `isPureVirtual()`, `operator==()`, ... */
	const bool isInitialized() const SUSUWU_OVERRIDE { return initialized; } /* if can do "inference" (ergo "forwardpropagation"; `process*`) */
	virtual void setInitialized(const bool is) { initialized = is; } /* after "training" (ergo "backpropagation") finishes, set to `true` */

	/* Topological values; methods which set the "shape" of `Cns.synapses` (or of whatever the derived class uses to store the connectome) */
	virtual const bool isSquareConnectome() const { return false; } /* returns `true` if the implementation requires `inputNeurons == outputNeurons == neuronsPerLayer` (square layout does not support all uses, but is most simple to code and allows the most [SIMD](../posts/SimdGpgpuTpu.md) */
//	virtual bool setSquareConnectome(const bool is) { return false; } /* returns `true` (and sets the squareness to `is`) if the implementation has customizable squareness (dynamic code paths for square versus non-square connectomes) */
	virtual void setSquareConnectome(const bool is) { throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::setSquareConnectome(" + std::to_string(is) + ") { /* implementation does not support setting this */ }")); } /* if the implementation has customizable squareness (dynamic code paths for square versus non-square connectomes), sets the squareness to `is`. */
	virtual void restructureConnectome() { setInitialized(false); /* stub, since root class has no `synapses` tensor */ } /* updates (or rebuilds) connectome (tensor of coefficients and/or biases) to match new topological layout */
	virtual void setInputMode(ObjectMode x) { inputMode = x; } /* sets type of input */
	virtual void setOutputMode(ObjectMode x) { outputMode = x; } /* sets type of output (notice: some implementations require `inputMode == outputMode`) */
	virtual void setInputNeurons(size_t x) { /* sets connectome input count */
		if(x != inputNeurons) {
			if(isSquareConnectome() && 0 != neuronsPerLayer && x != neuronsPerLayer) {
				throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::setInputNeurons(.x = " + std::to_string(x) + ") { isSquareConnectome() && 0 != neuronsPerLayer && x != neuronsPerLayer }"));
			}
			inputNeurons = x;
			restructureConnectome();
		}
	}
	virtual void setOutputNeurons(size_t x) { /* sets connectome output count */
		if(x != outputNeurons) {
			if(isSquareConnectome() && 0 != neuronsPerLayer && x != neuronsPerLayer) {
				throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::setOutputNeurons(.x = " + std::to_string(x) + ") { isSquareConnectome() && 0 != neuronsPerLayer && x != neuronsPerLayer }"));
			}
			outputNeurons = x;
			restructureConnectome();
		}
	}
	virtual void setLayersOfNeurons(size_t x) { /* sets connectome "hidden layer" count */
		if(x != layersOfNeurons) {
			layersOfNeurons = x;
			restructureConnectome();
		}
	}
	virtual void setNeuronsPerLayer(size_t x) { /* sets connectome coefficients-per-"hidden layer" */
		if(x != neuronsPerLayer) {
			if(isSquareConnectome() && 0 != inputNeurons && x != inputNeurons) {
				throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::setNeuronsPerLayer(.x = " + std::to_string(x) + ") { isSquareConnectome() && 0 != inputNeurons && x != inputNeurons }"));
			}
			neuronsPerLayer = x;
			restructureConnectome();
		}
	}
	virtual const size_t getParameterCount() { /* Notice: must `override` if architecture is sparse. Must `override` to count constant biases. */
		if(!isSquareConnectome()) {
			/*throw std::runtime_error*/SUSUWU_WARNING(getName() + "::getParameterCount() { isSquareConnectome() == false; /*Is not simple dense, square connectome. Must `override` for sparse connectomes. */ }")/*) TODO: `throw`? */;
		}
		return neuronsPerLayer * neuronsPerLayer * layersOfNeurons; /* default is; counts 1 coefficient per input to next layer of neurons */
	}

/* NOLINTBEGIN(google-default-arguments): derivative classes use our default values */
	/* Compute values; setters of training momentum (and similar arguments) which the derivative classes use */
	virtual void setLearningFactor(ObjectMode x) {
		if(0 > learningFactor || 1 < learningFactor) {
			throw std::invalid_argument("ClassCns::setLearningFactor(\"" + std::to_string(x) + "\") out of bounds");
		}
		learningFactor = x; /* how much coefficients move (converge) per step of `setupSynapses` training loop. `0.0` is static, `1.0` is straight to the first reciprocal (or whatever form of gradient the optimizer uses) */
	}

	/* dump the connectome (the `tensorflow::Tensor` of synapse coefficients)
	 * @throw std::runtime_error */
	virtual void dumpTo(const ClassIoPath &modelPath) const { throw std::runtime_error("ClassCns::dumpTo(\"" + modelPath + "\") pure virtual call"); }
	/* load the connectome (the `tensorflow::Tensor` of synapse coefficients)
	 * @throw std::runtime_error */
	virtual void loadFrom(const ClassIoPath &modelPath) { throw std::runtime_error("ClassCns::loadFrom(\"" + modelPath + "\") pure virtual call"); }

	/* Internal function, which outros of `setupSynapses()` use.
	 * @throw std::bad_alloc std::runtime_error
	 * @pre @code !isPureVirtual() @endcode
	 * @post @code isInitialized() @endcode */
	virtual void setupSynapsesPostProcess() { setInitialized(true); } /* `override` with code common to all `setupSynapses`. */
#if SUSUWU_VIRTUAL_MEMBER_FUNCTION_TEMPLATES /* C++ does not support templates of virtual functions ( https://stackoverflow.com/a/78440416/24473928 ) */
	/* Initialize `Cns.synapses` with pseudorandom (uses `rand()` or recipricals of `inputsToOutputs`) values
	 * @throw std::runtime_error */
	template<class InputsToOutputs>
	virtual void pseudoRandomSynapses(const InputsToOutputs &inputsToOutputs) { throw std::runtime_error("ClassCns::pseudoRandomSynapses() pure virtual call"); }
	/* @throw bad_alloc
	 * @pre @code !isPureVirtual() @endcode */
	template<typename Input, typename Output>
	virtual void setupSynapses(std::vector<std::tuple<Input, Output>> inputsToOutputs, size_t trainingIterations = 0 /* if 0, guesses suitable loop count */); /* { restructureConnectome(); inputMode = ToObjectMode<Input>::value; outMode = ToObjectMode<Output>::value; setupSynapsesPostProcess(); throw std::runtime_error("ClassCns::setupSynapses() pure virtual call"); } */
	/* @pre @code isInitialized() @endcode */
	template<typename Input, typename Output>
	virtual const Output process(const Input input) const {
		assert(ToObjectMode<Input>::value == inputMode && ToObjectMode<Output>::value == outputMode);
		throw std::runtime_error("ClassCns::process() pure virtual call");
	}
#else /* !SUSUWU_VIRTUAL_MEMBER_FUNCTION_TEMPLATES */
#	define SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, OUTPUT_TYPEDEF) \
	/* Initialize `Cns.synapses` with pseudorandom (uses `rand()` or recipricals of `inputsToOutputs`) values \
	 * @throw std::runtime_error */ \
	virtual void pseudoRandomSynapses(const std::vector<std::tuple<INPUT_TYPEDEF, OUTPUT_TYPEDEF>> &inputsToOutputs) { throw std::runtime_error("ClassCns::pseudoRandomSynapses() pure virtual call"); } /* NOLINT(bugprone-macro-parentheses): parentheses cause "error: expected expression [clang-diagnostic-error]" */ \
	/* @throw bad_alloc \
	 * @pre @code !isPureVirtual() @endcode \
	 * @post @code isInitialized() @endcode */\
	virtual void setupSynapses(const std::vector<std::tuple<INPUT_TYPEDEF, OUTPUT_TYPEDEF>> &inputsToOutputs, size_t trainingIterations = 0 /* if 0, guesses suitable loop count */) { restructureConnectome(); inputMode = ToObjectMode<INPUT_TYPEDEF>::value; outputMode = ToObjectMode<OUTPUT_TYPEDEF>::value; setupSynapsesPostProcess(); /* stub so unit tests pass */ } /* NOLINT(bugprone-macro-parentheses) */
#	define SUSUWU_TEMPLATE_WORKAROUND(INPUT_TYPEDEF) \
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, bool)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, char)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, int)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, unsigned int)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, float)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, double)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, std::vector<bool>)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, std::vector<char>)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, std::vector<int>)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, std::vector<unsigned int>)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, std::vector<float>)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, std::vector<double>)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, std::string)\
	\
	/* @pre @code isInitialized() @endcode */\
	virtual const bool processToBool(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeBool == outputMode); return 0; }\
	virtual const char processToChar(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeChar == outputMode); return 0; }\
	virtual const int processToInt(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeInt == outputMode); return 0; }\
	virtual const unsigned int processToUint(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeUint == outputMode); return 0; }\
	virtual const float processToFloat(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeFloat == outputMode); return 0; }\
	virtual const double processToDouble(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeDouble == outputMode); return 0; }\
	virtual const std::vector<bool> processToVectorBool(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeVectorBool == outputMode); return {}; }\
	virtual const std::vector<char> processToVectorChar(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeVectorChar == outputMode); return {}; }\
	virtual const std::vector<int> processToVectorInt(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeVectorInt == outputMode); return {}; }\
	virtual const std::vector<unsigned int> processToVectorUint(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeVectorUint == outputMode); return {}; }\
	virtual std::vector<float> processToVectorFloat(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeVectorFloat == outputMode); return {}; }\
	virtual const std::vector<double> processToVectorDouble(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeVectorDouble == outputMode); return {}; }\
	virtual const std::string processToString(const INPUT_TYPEDEF &input) const { auto val = processToVectorChar(input); return std::string(&val[0], val.size()); }
	SUSUWU_TEMPLATE_WORKAROUND(bool)
	SUSUWU_TEMPLATE_WORKAROUND(char)
	SUSUWU_TEMPLATE_WORKAROUND(int)
	SUSUWU_TEMPLATE_WORKAROUND(unsigned int)
	SUSUWU_TEMPLATE_WORKAROUND(float)
	SUSUWU_TEMPLATE_WORKAROUND(double)
	SUSUWU_TEMPLATE_WORKAROUND(std::vector<bool>)
	SUSUWU_TEMPLATE_WORKAROUND(std::vector<char>)
	SUSUWU_TEMPLATE_WORKAROUND(std::vector<int>)
	SUSUWU_TEMPLATE_WORKAROUND(std::vector<unsigned int>)
	SUSUWU_TEMPLATE_WORKAROUND(std::vector<float>)
	SUSUWU_TEMPLATE_WORKAROUND(std::vector<double>)
	SUSUWU_TEMPLATE_WORKAROUND(std::string)
#	undef SUSUWU_TEMPLATE_WORKAROUND
#endif /* !SUSUWU_VIRTUAL_MEMBER_FUNCTION_TEMPLATES */
/* NOLINTEND(google-default-arguments) */
	const NumeralNormalizers &inputNorms() const {
		return inputNormsStorage;
	}
	const NumeralNormalizers &outputNorms() const {
#if SUSUWU_CNS_SEPARATE_NORMS
		return outputNormsStorage;
#else /* else !SUSUWU_CNS_SEPARATE_NORMS */
		return inputNormsStorage;
#endif /* !SUSUWU_CNS_SEPARATE_NORMS */
	}
protected: /* NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers) */
	NumeralNormalizers inputNormsStorage; /* store inputNorms which `setupSynapses` ("training" / backpropagation) used, so that `processTo*` ("inference" / forwardpropagation) can reuse those */
#if SUSUWU_CNS_SEPARATE_NORMS
	NumeralNormalizers outputNormsStorage; /* separate normalization factors for "labels" (for expected output values) */
#endif /* !SUSUWU_CNS_SEPARATE_NORMS */
	ObjectMode inputMode = objectModeBool, outputMode = objectModeBool;
	size_t inputNeurons = 0, outputNeurons = 0, layersOfNeurons = 0, neuronsPerLayer = 0;
	size_t patience = 10; float minLossDelta = 0.001; /* `setupSynapses()`'s minimum per-iterations-improvement; abort (undefined if this is "success" or "failure") if the loss function is less than `minLossDelta` for `patience` iterations in a row */ /* TODO: include `setMinLossDelta()`, or turn `patience` and `minLossDelta` into arguments to `setupSynapses()` */
	float desiredLossThreshold = 0.01; /* return (success) from `setupSynapses()` before `trainingIterations` if the loss function goes below this value */ /* TODO: include `setDesiredLossThreshold()`, or turn `desiredLossThreshold` into an argument to `setupSynapses()` */
	float validationFactor = 0.2; /* so overfitting (convergence on non-generalizable patterns) is prevented, reserve this much of `inputsToOutputs` for validation (to compute true loss) */
	float learningFactor = 0.02;
	bool initialized = false;
/* NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers) */
} Cns;

#ifdef USE_HSOM_CNS
typedef class HsomCns : public Cns {
/* Work-in-progress (`ClassCns.cxx` for more information): `HSOM` is simple Python-based CNS from https://github.com/CarsonScott/HSOM
 * Examples of howto setup `HSOM` as artificial CNS; https://github.com/CarsonScott/HSOM/tree/master/examples
 * [ https://stackoverflow.com/questions/3286448/calling-a-python-method-from-c-c-and-extracting-its-return-value ] suggests various syntaxes to use for this, with unanswered comments such as "Does this support classes?"
 */
} HsomCns;
#endif /* USE_HSOM_CNS */

#ifdef USE_APXR_CNS
typedef class ApxrCns : public Cns {
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

`less `[`cxx/ClassCns.cxx`](../cxx/ClassCns.cxx)
```c++
#ifdef USE_HSOM_CNS
/* Sources: `git clone https://github.com/CarsonScott/HSOM.git`
 * Install: `pip install pynum && pip install json && pip install git+https://github.com/CarsonScott/HSOM.git`
 * Documentation: `less HSOM/README.md` `less HSOM/Documentation.md` */
/* "If you're using Python >3.5, PyString_FromString() is PyUnicode_FromString()" */
#include <Python.h> /* Sources: `pkg install python` */
#include <vector> /* std::vector */
typedef class HsomCns : Cns { /* TODO. ( https://stackoverflow.com/questions/3286448/calling-a-python-method-from-c-c-and-extracting-its-return-value ) suggests various syntaxes to use for this, with unanswered comments such as "Does this support classes?" */
	//template<Input, Output> void setupSynapses(const std::vector<std::tuple<Input, Output>>) { /* TODO: templates not allowed for virtual functions with C++ ( https://stackoverflow.com/a/78440416/24473928 ), so must produce codes for each combination of inputMode+outputMode */
	void setupSynapses(const std::vector<std::tuple<float, float>>) SUSUWU_OVERRIDE {
		setenv("PYTHONPATH", ".",1);
		Py_Initialize();
//		PyRun_SimpleString("import sys; sys.path.append('.')"); PyRun_SimpleString("import hsom; from hsom import SelfOrganizingNetwork;");
#if USE_PYRUN /* Was told not to use PyRun because "PyRun requires all results go to stdout" */
		PyRun_SimpleString("import sys; sys.path.append('./HSOM/')");

/* Based off of https://github.com/CarsonScott/HSOM/blob/master/examples/self_organizing_network.py
 * Not sure if `input_size` is "Inputs from each layer to next layer" and `node_count` is "Inputs to HSOM" (process(input.length())) or vice versa, assumed vice versa */

		PyRun_SimpleString("import hsom\n\
from hsom import SelfOrganizingNetwork\n\
from random import sample\n\
\
input_size = " + inputNeurons + "\n\
layer_sizes = []\n\
for x in range(" + layersOfNeurons + "):\n\
	layer_sizes.append(" + neuronsPerLayer + ");\n\
layer_sizes.append(" + outputNeurons + ");\n\
input_percents = [0.2, 0.2, 0.2, 0.2, 0.75, 1.0]\n\
learning_rate = 0.05\n\
boost_factor = 1\n\
node_count = 5\n\
winner_count = 1\n\
initial_range = (-0.5, 0.5)\n\
\
# Create layersOfNeurons+1 hierarchical layers of sizes = neuronsPerLayer, and outputNeurons for last\n\
self_organizing_network = SelfOrganizingNetwork(\
	input_size=input_size,\
	layer_sizes=layer_sizes,\
	input_percents=input_percents,\
	learning_rates=learning_rate,\
	boost_factors=boost_factor,\
	node_counts=node_count,\
	winner_counts=winner_count,\
	initial_ranges=initial_range)\n\
\
# Create a set of sparse samples\n\
samples = []");
		foreach(inputsToOutputs as sample) { /* TODO: templates not allowed for virtual functions with C++ ( https://stackoverflow.com/a/78440416/24473928 ), so must produce codes for each combination of inputMode+outputMode */
			PyRun_SimpleString("samples.append(" + sample.first() +" -> " + sample.last() + ")");
		}
		PyRun_SimpleString("for i in range(200):\n\
	self_organizing_network.train(samples)");
#else /* else !USE_PYRUN */
		PyObject *module = PyImport_ImportModule("hsom")
		if(NULL == module) { throw "'hsom' module not found"; }
		PyObject *selfOrganizingNetwork = PyObject_GetAttrString(module, (char*)"SelfOrganizingNetwork"); /* or	"PyObject *pDict = PyModule_GetDict(module);  PyObject *selfOrganizingNetwork = PyDict_GetItemString(pDict, (char*)"SelfOrganizingNetwork");" */
		if(NULL == selfOrganizingNetwork || !PyCallable_Check(selfOrganizingNetwork)) { throw "'SelfOrganizingNetwork' object not found"; }
		double result = PyObject_CallFunction(selfOrganizingNetwork, "d", 2.0); /* or "PyObject *pValue = Py_BuildValue("(z)",(char*)"args");	PyObject *pResult = PyObject_CallObject(selfOrganizingNetwork, pValue); if(NULL == pResult) { throw "PyObject_CallObject failed"; } double result = PyInt_AsLong(pResult)); Py_DECREF(pValue);" */
		Py_DECREF(module);
#endif /* USE_PYRUN else */
	}
	~HsomCns() {
#if PYTHON3
		Py_FinalizeEx();
#else /* else !PYTHON */
		Py_Finalize();
#endif /* PYTHON3 else */
	}
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
```

`less `[`cxx/ClassTensorFlowCns.hxx`](../cxx/ClassTensorFlowCns.hxx)
```c++
#define SUSUWU_TENSORFLOWCNS_PROTOBUF_FS /* The is the most close to fit-for-use */
#ifndef SUSUWU_CNS_LOCAL_COEFFICIENTS
#	define SUSUWU_CNS_LOCAL_COEFFICIENTS false
#endif /* ndef SUSUWU_CNS_LOCAL_COEFFICIENTS */
#ifndef SUSUWU_TENSORFLOW_HAS_DATATYPETOENUM
#	define SUSUWU_TENSORFLOW_HAS_DATATYPETOENUM false
#endif /* ndef SUSUWU_TENSORFLOW_HAS_DATATYPETOENUM */
#ifndef SUSUWU_CNS_USE_MLP
#	define SUSUWU_CNS_USE_MLP false /* Multiple-Layer-Perceptron mode. No reason to disable this (if `1 == layersOfNeurons`, `setupSynapses()` and `processTo*()` act as Single-Layer-Perceptrons), but for now is TODO */
#endif /* ndef SUSUWU_CNS_USE_MLP */
#if SUSUWU_CNS_IF_MLP
# define SUSUWU_CNS_IF_MLP(THEN, ELSE) THEN
#else /* else !SUSUWU_CNS_USE_MLP */
# define SUSUWU_CNS_IF_MLP(THEN, ELSE) ELSE
#endif /* else !SUSUWU_CNS_USE_MLP */
#ifndef SUSUWU_CNS_USE_BIAS
#	define SUSUWU_CNS_USE_BIAS true
#endif /* ndef SUSUWU_CNS_USE_BIAS */
#if SUSUWU_CNS_USE_BIAS
# define SUSUWU_CNS_IF_BIAS(THEN) THEN
#else /* else !SUSUWU_CNS_USE_BIAS */
# define SUSUWU_CNS_IF_BIAS(THEN) SUSUWU_NOOP
#endif /* else !SUSUWU_CNS_USE_BIAS */

namespace Susuwu {
typedef struct TensorFlowCnsLoss { tensorflow::Output loss, backprop; } /* `decltype(tensorflow::ops::SoftmaxCrossEntropyWithLogits(tensorflow::Scope::NewRootScope(), tensorflow::ops::Add, tensorflow::ops::Placeholder))`? */ TensorFlowCnsLoss;

/* For use until [`tensorflow::ops::SigmoidCrossEntropyWithLogits`](https://www.tensorflow.org/api_docs/cc/class/tensorflow/ops/sigmoid-cross-entropy-with-logits) (which GitHub's assistant suggests, but is "404" now) is found or `tensorflow::ops::BinaryCrossEntropyWithLogits` (which Claude-3-Haiku says to use from `tensorflow/c/experimental/ops/nn_ops.h`, but is also not found) is found.
 * BCE = max(logits, 0) - logits * labels + log(1 + exp(-abs(logits))) */
static const TensorFlowCnsLoss SigmoidCrossEntropyWithLogits(const tensorflow::Scope &root, const tensorflow::Input &logits, const tensorflow::Input &labels) {
	//return tensorflow::ops::Sigmoid(root, tensorflow::ops::SparseCrossEntropyWithLogits(root, logits, labels));
	auto zeros = tensorflow::ops::ZerosLike(root, logits); /* `tensorflow::ops::Const(root, 0.0f, logits.shape());` may not support placeholders? */
	auto cond = tensorflow::ops::Maximum(root, logits, zeros);
	auto negAbs = tensorflow::ops::Neg(root, tensorflow::ops::Abs(root, logits));
	auto logExp = tensorflow::ops::Log1p(root, tensorflow::ops::Exp(root, negAbs));
	auto mul = tensorflow::ops::Multiply(root, logits, labels);
	auto bce = tensorflow::ops::Add(root, tensorflow::ops::Subtract(root, cond, mul), logExp);
	/* TODO: `bce = { tensorflow::ops::Mean(root, bce, 0) };`, or is the caller supposed to do this? */
	auto sigmoid = tensorflow::ops::Sigmoid(root, logits);
	auto grad = tensorflow::ops::Subtract(root, sigmoid, labels);
	return { bce, grad };
}
template<> /* specialization continued from `cxx/ClassObject.hxx` */
struct ToObjectMode<tensorflow::tstring> { static SUSUWU_CONSTEXPR ObjectMode value = objectModeString; }; /* `error: implicit instantiation of undefined template 'Susuwu::ToObjectMode<tsl::tstring>'` fix */

template<class Q>
#if SUSUWU_TENSORFLOW_HAS_DATATYPETOENUM
struct DataTypeToEnum { static SUSUWU_CONSTEXPR tensorflow::DataType value = tensorflow::DataTypeToEnum<Q>::v(); };
#else /* else !SUSUWU_TENSORFLOW_HAS_DATATYPETOENUM */ /* `error: no template named 'DataTypeToEnum' in namespace 'tensorflow';` fix. */
struct DataTypeToEnum; /* The template specializations (which follow) require forward declaration of this. */
// typedef struct DataTypeToEnum { static tensorflow::DataType value; } DataTypeToEnum; /* TODO: fix `error: expected ';' after struct` */
template<>
struct DataTypeToEnum<float> { static SUSUWU_CONSTEXPR tensorflow::DataType value = tensorflow::DT_FLOAT; };
template<>
struct DataTypeToEnum<int> { static SUSUWU_CONSTEXPR tensorflow::DataType value = tensorflow::DT_INT32; };
template<>
struct DataTypeToEnum<tensorflow::int64> { static SUSUWU_CONSTEXPR tensorflow::DataType value = tensorflow::DT_INT64; };
template<>
struct DataTypeToEnum<std::string> { static SUSUWU_CONSTEXPR tensorflow::DataType value = tensorflow::DT_STRING; };
template<>
struct DataTypeToEnum<tensorflow::tstring> { static SUSUWU_CONSTEXPR tensorflow::DataType value = tensorflow::DT_STRING; };
#endif /* else !SUSUWU_TENSORFLOW_HAS_DATATYPETOENUM */


class TensorFlowCns : public Cns {
public:
	typedef tensorflow::int64 /* decltype(tensorflow::TensorShapeRep({}).num_elements()) */ DimSz; /* `[-Wnarrowing]` fix. `tensorflow/core/framework/tensor_shape.h` has "We use `int64` and not `size_t` to be compatible with `Eigen::Tensor` which uses `ptrdiff_t`" */
	typedef float CoefficientDefaultType; /* `float` is the most common (due to: is fast and accurate). This `typedef` allows to set a new default for the whole `TensorFlowCns` class. */

	/* @throw std::runtime_error, tensorflow::errors::Internal, tensorflow::errors::Unavailable */
	TensorFlowCns() : root(tensorflow::Scope::NewRootScope()) {
		const tensorflow::SessionOptions sessionOptions;
		const tensorflow::Status status = tensorflow::NewSession(sessionOptions, &session);
		if(!status.ok()) {
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, this->getName() + "() { (!tensorflow::NewSession(sessionOptions, &session).ok()) { status.ToString() == \"" + status.ToString() + "\"; } }"));
		}
	}
	TensorFlowCns(const TensorFlowCns &obj) : Cns(obj), root(tensorflow::Scope::NewRootScope()) {
		const tensorflow::SessionOptions sessionOptions;
		const tensorflow::Status status = tensorflow::NewSession(sessionOptions, &session);
		if(!status.ok()) {
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, this->getName() + "() { (!tensorflow::NewSession(sessionOptions, &session).ok()) { status.ToString() == \"" + status.ToString() + "\"; } }"));
		}
#if SUSUWU_CNS_LOCAL_COEFFICIENTS
		this->coefficients = obj.coefficients;
#	if SUSUWU_CNS_USE_BIAS
		this->biases = obj.biases;
#	endif /* SUSUWU_CNS_USE_BIAS */
#endif /* SUSUWU_CNS_LOCAL_COEFFICIENTS */
		this->graphDef = obj.graphDef;
	}

	~TensorFlowCns() SUSUWU_OVERRIDE {
		if(SUSUWU_NULLPTR != session) {
			TF_CHECK_OK(session->Close());
			delete session;
		}
	}
#if SUSUWU_CNS_VALUE_SEMANTICS
#	define SUSUWU_CLASS_OPERATOREQUALTO(noop) ; /* NOLINT(cppcoreguidelines-macro-usage) */
	bool operator==(const Class &obj) const SUSUWU_OVERRIDE { /* NOLINT(fuchsia-overloaded-operator) */
		const auto *thisFlow = dynamic_cast<const TensorFlowCns *>(this);
		const auto *objFlow = dynamic_cast<const TensorFlowCns *>(&obj);
		return Cns::operator==(obj) &&
#if SUSUWU_CNS_LOCAL_COEFFICIENTS
			thisFlow->coefficients == objFlow->coefficients \
#	if SUSUWU_CNS_USE_BIAS
			&& thisFlow->biases == objFlow->biases \
#	endif /* SUSUWU_CNS_USE_BIAS */
			;
#else /* else !SUSUWU_CNS_LOCAL_COEFFICIENTS */
			thisFlow->graphDef.SerializeAsString() == objFlow->graphDef.SerializeAsString();
/* TODO: `#include <google/protobuf/util/message_differencer.h>` `bool areEqual = google::protobuf::util::MessageDifferencer::Equals(thisFlow->graphDef, objFlow->graphDef);`. This development environment does not have `message_differencer.h` to test those. */
#endif /* else !SUSUWU_CNS_LOCAL_COEFFICIENTS */
//		return this->hashCode() == obj.hashCode();
	}
	const SUSUWU_INTPTR hashCode() const SUSUWU_OVERRIDE SUSUWU_VIRTUAL_HASHCODE /* Shallow hash code. TODO: hash container values */
#endif /* SUSUWU_CNS_VALUE_SEMANTICS */
	SUSUWU_PURE_VIRTUAL_DEFAULTS(Susuwu::TensorFlowCns) /* `getName()`, `isPureVirtual()`, `operator==()`, ... */

	/* Topological values; sets the "shape" of `Cns.coefficients` (or of whatever the derived class uses to store the connectome) */
	const bool isSquareConnectome() const SUSUWU_OVERRIDE { return true; } /* TODO: don't hard code */
	void setSquareConnectome(const bool is) SUSUWU_OVERRIDE { throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::setSquareConnectome(" + std::to_string(is) + ") { /* implementation does not support setting this */ }")); } /* TODO: allow to set this */
//	void setInputMode(ObjectMode x) SUSUWU_OVERRIDE { inputMode = x; } /* sets type of input */
//	void setOutputMode(ObjectMode x) SUSUWU_OVERRIDE { outputMode = x; } /* sets type of output (notice: some implementations require `inputMode == outputMode`) */
	template<class CoefficientType>
	void restructureConnectomeImpl() { /* CRTP backend, for internal use. */
		setInitialized(false);
#if SUSUWU_CNS_LOCAL_COEFFICIENTS
		coefficients = tensorflow::Tensor(DataTypeToEnum<CoefficientType>::value, tensorflow::TensorShape({static_cast<DimSz>(inputNeurons), static_cast<DimSz>(neuronsPerLayer)}));
#	if SUSUWU_CNS_USE_BIAS
		biases = tensorflow::Tensor(DataTypeToEnum<CoefficientType>::value, tensorflow::TensorShape({static_cast<DimSz>(neuronsPerLayer)}));
#	endif /* SUSUWU_CNS_USE_BIAS */
#endif /* SUSUWU_CNS_LOCAL_COEFFICIENTS */
	}
	void restructureConnectome() SUSUWU_OVERRIDE {
		restructureConnectomeImpl<CoefficientDefaultType>();
	}
	void setInputNeurons(size_t x) SUSUWU_OVERRIDE { /* sets connectome input count */
		if(x != inputNeurons) {
			if(isSquareConnectome() && 0 != neuronsPerLayer && x != neuronsPerLayer) {
				throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::setInputNeurons(.x = " + std::to_string(x) + ") { isSquareConnectome() && 0 != neuronsPerLayer && x != neuronsPerLayer }"));
			}
			inputNeurons = x;
			restructureConnectome();
		}
	}
	void setOutputNeurons(size_t x) SUSUWU_OVERRIDE { /* sets connectome output count */
		if(x != outputNeurons) {
			if(isSquareConnectome() && 0 != neuronsPerLayer && x != neuronsPerLayer) {
				throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::setOutputNeurons(.x = " + std::to_string(x) + ") { isSquareConnectome() && 0 != neuronsPerLayer && x != neuronsPerLayer }"));
			}
			outputNeurons = x;
			restructureConnectome();
		}
	}
	void setLayersOfNeurons(size_t x) SUSUWU_OVERRIDE { /* sets connectome "hidden layer" count */
		if(x != layersOfNeurons) {
			layersOfNeurons = x;
			restructureConnectome();
		}
	}
	void setNeuronsPerLayer(size_t x) SUSUWU_OVERRIDE { /* sets connectome coefficients-per-"hidden layer" */
		if(x != neuronsPerLayer) {
			if(isSquareConnectome() && 0 != inputNeurons && x != inputNeurons) {
				throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::setNeuronsPerLayer(.x = " + std::to_string(x) + ") { isSquareConnectome() && 0 != inputNeurons && x != inputNeurons }"));
			}
			neuronsPerLayer = x;
			restructureConnectome();
		}
	}
	const size_t getParameterCount() SUSUWU_OVERRIDE {
		if(!isSquareConnectome()) {
			/*throw std::runtime_error*/SUSUWU_WARNING(getName() + "::getParameterCount() { isSquareConnectome() == false; /*Is not simple dense, square connectome. Must `override` for sparse connectomes. */ }")/*) TODO: `throw`? */;
		}
		return (neuronsPerLayer * neuronsPerLayer * layersOfNeurons) SUSUWU_CNS_IF_BIAS(+ (neuronsPerLayer * layersOfNeurons));
	}

	template<class CoefficientType,	class InputIt>
	void initCoefficient(CoefficientType &coefficientRef, InputIt inputIt, const float scale) {
#if defined(SUSUWU_CNS_TRUE_RANDOM_INIT) && SUSUWU_CNS_TRUE_RANDOM_INIT
		coefficientRef = static_cast<CoefficientType>(rand()) / static_cast<CoefficientType>(RAND_MAX) /* start with true random values. */
#elif defined(SUSUWU_CNS_HE_INIT) && SUSUWU_CNS_HE_INIT /* He initialization prevents saturation of `ReLU` functions. */
		coefficientRef = (static_cast<CoefficientType>(rand()) / static_cast<CoefficientType>(RAND_MAX) * 2 - 1);
#elif defined(SUSUWU_CNS_XAVIER_INIT) && SUSUWU_CNS_XAVIER_INIT /* Also known as "Glorot initialization", prevents saturation of `sigmoid` and `tanh` functions. */
		coefficientRef = (static_cast<CoefficientType>(rand()) / static_cast<CoefficientType>(RAND_MAX) * 2 - 1);
#elif defined(SUSUWU_CNS_BIAS_INIT) && SUSUWU_CNS_BIAS_INIT /* Biases are good approximations of constant functions */
		coefficientRef = std::get<1>(*inputIt) - std::get<0>(*inputIt) /* start with simple biases. */;
#else /* Fallback mode. Reciprocals are good approximations of linear functions. */
		coefficientRef = std::get<1>(*inputIt) / std::get<0>(*inputIt) /* start with reciprocals. */;
#endif /* !(defined(SUSUWU_CNS_TRUE_RANDOM_INIT) && SUSUWU_CNS_TRUE_RANDOM_INIT) */
		coefficientRef *= scale;
	}
	template<class CoefficientType, class InputsToOutputs>
	void pseudoRandomSynapses(const InputsToOutputs &inputsToOutputs)
#if SUSUWU_VIRTUAL_MEMBER_FUNCTION_TEMPLATES
	SUSUWU_OVERRIDE
#endif /* SUSUWU_VIRTUAL_MEMBER_FUNCTION_TEMPLATES */
	{ /* Initialize `coefficients` with pseudorandom (uses `rand()` or recipricals of `inputsToOutputs`) values */
#if !SUSUWU_CNS_LOCAL_COEFFICIENTS
		tensorflow::Tensor coefficients(tensorflow::Tensor(DataTypeToEnum<CoefficientType>::value, tensorflow::TensorShape({static_cast<DimSz>(inputNeurons), static_cast<DimSz>(neuronsPerLayer)}))); /* connectome coefficients ("synaptic strengths") */
		tensorflow::Tensor biases(tensorflow::Tensor(DataTypeToEnum<CoefficientType>::value, tensorflow::TensorShape({static_cast<DimSz>(neuronsPerLayer)}))); /* connectome biases (membrane potentials) */
#endif /* !SUSUWU_CNS_LOCAL_COEFFICIENTS */
		const auto inputBegin = inputsToOutputs.cbegin(), inputEnd = inputsToOutputs.cend();
		auto inputIt = inputBegin;
#if defined(SUSUWU_CNS_HE_INIT) && SUSUWU_CNS_HE_INIT /* He initialization prevents saturation of `ReLU` functions. */
		const float scale = sqrt(2.0f / (float)neuronsPerLayer);
#elif defined(SUSUWU_CNS_XAVIER_INIT) && SUSUWU_CNS_XAVIER_INIT /* Also known as "Glorot initialization", prevents saturation of `sigmoid` and `tanh` functions. */
		const float scale = sqrt(6.0f / (float)(neuronsPerLayer + neuronsPerLayer));
#else
		const float scale = 1.0; /* placeholder */
#endif /* !(defined(SUSUWU_CNS_HE_INIT) && SUSUWU_CNS_HE_INIT) */
		for(int i = 0; i < coefficients.dim_size(0); ++i) {
			for(int j = 0; j < coefficients.dim_size(1); ++j) {
				if(inputEnd != inputIt) {
					++inputIt;
				} else {
					inputIt = inputBegin;
				}
				initCoefficient(coefficients.tensor<CoefficientType, 2>()(i, j), inputIt, scale);
			}
		}
		TF_CHECK_OK(session->Run({
					{"randomCoefficients", coefficients}, {"randomBiases", biases}
					}, {}, {"assignCoefficients", "assignBiases"},
					SUSUWU_NULLPTR)); /* "LoadError: Tensorflow error: Status: Attempting to use uninitialized value" fix */
	}

#if SUSUWU_CNS_IF_MLP
#	pragma message("TODO: `SUSUWU_TENSORFLOWCNS_LOGITS` loop for `SUSUWU_CNS_USE_MLP`")
//	for(long w = 0; this->layersOfNeurons > w; ++w) { /* TODO */ }
#endif /* SUSUWU_CNS_IF_MLP */
#if SUSUWU_CNS_USE_BIAS
#	define SUSUWU_TENSORFLOWCNS_LOGITS tensorflow::ops::Add(root.WithOpName("logits"), tensorflow::ops::MatMul(root, input, coefficientsVar), biasesVar)
#else /* !SUSUWU_CNS_USE_BIAS */
#	define SUSUWU_TENSORFLOWCNS_LOGITS tensorflow::ops::MatMul(root.WithOpName("logits"), input, coefficientsVar)
#endif /* !SUSUWU_CNS_USE_BIAS */
#define SUSUWU_TENSORFLOWCNS_INIT_SCOPE /* Simple (1 dense layer) inference model. TODO: [choose how to implement `layersOfNeurons`](https://github.com/copilot/share/427f408e-08e0-8c80-9151-f24920212857) */\
		const auto inputDim = static_cast<DimSz>(inputNeurons), outputDim = static_cast<DimSz>(outputNeurons); /* TODO: assert `1` for base types? */ \
		root = tensorflow::Scope::NewRootScope(); \
		auto input = tensorflow::ops::Placeholder(root.WithOpName("input"), /* `DataTypeToEnum<Input>::value` gives "INVALID_ARGUMENT: Inconsistent values" */coefficientFormat/*, Placeholder::Shape({-1, inputDim})*/); \
		auto coefficientsVar = tensorflow::ops::Variable(root.WithOpName("coefficients"), tensorflow::TensorShape({inputDim, outputDim}), coefficientFormat); \
		auto randomCoefficientsVar = tensorflow::ops::Variable(root.WithOpName("randomCoefficients"), tensorflow::TensorShape({inputDim, outputDim}), coefficientFormat); \
		SUSUWU_CNS_IF_BIAS(auto biasesVar = tensorflow::ops::Variable(root.WithOpName("biases"), tensorflow::TensorShape({outputDim}), coefficientFormat);) \
		SUSUWU_CNS_IF_BIAS(auto randomBiasesVar = tensorflow::ops::Variable(root.WithOpName("randomBiases"), tensorflow::TensorShape({outputDim}), coefficientFormat);) \
		auto logits = SUSUWU_TENSORFLOWCNS_LOGITS; /* TODO: ensure that `"logits"` is usable as-is for `process*()`. For classification use `"relu"`? */ \
		auto relu = tensorflow::ops::Relu(root.WithOpName("relu"), logits); /* TODO: benchmark test how accurate this is? [Rectified Linear Unit activation function](https://www.tensorflow.org/api_docs/cc/class/tensorflow/ops/relu) does; scaling, cutoff and `abs`. Gives non-linear version of `logits` (can use as input to hidden-layer `loss`). [Python version has better documents](https://www.tensorflow.org/api_docs/python/tf/keras/layers/ReLU). */
	template<class Input, class Output>
	void initScopeRootForward(tensorflow::DataType coefficientFormat) { /* Reusable graph logic (for `process*()`, `initScopeRootBack()`) */
		SUSUWU_TENSORFLOWCNS_INIT_SCOPE /* TODO: merge macro into `initScopeRootBack` */
//		auto output = tensorflow::ops::Variable(root.WithOpName("output"), {outputDim}, coefficientFormat);
//		auto process = tensorflow::ops::Assign(root.WithOpName("process"), output, logits); /* `{"output"}, {"process"}, &oTensors);` gives `status.ToString() == "INVALID_ARGUMENT: Tensor output:0, specified in either feed_devices or fetch_devices was not found in the Graph";` */

		/* Create the graph (`this->graphDef`) */
		TF_CHECK_OK(root.ToGraphDef(&graphDef)); /* produce `tensorflow::GraphDef` */
		TF_CHECK_OK(session->Create(graphDef));
	}
	template<class Input, class Output>
	void initScopeRootBack(tensorflow::DataType coefficientFormat) { /* Reusable graph logic (for `setupSynapses()`) */
		SUSUWU_TENSORFLOWCNS_INIT_SCOPE /* TODO: replace with `initScopeRootForward(coefficientFormat);`. But how to capture all local variables from functions? */

		/* Define the loss function (ergo, softmax cross-entropy) */
		auto labels = tensorflow::ops::Placeholder(root.WithOpName("labels"), /* `DataTypeToEnum<Output>::value` gives "INVALID_ARGUMENT: Inconsistent values" */coefficientFormat/*, Placeholder::Shape({-1, outputDim})*/);
#ifdef TENSORFLOW_HAS_GRADIENTTAPE
		/* Record the operations for the loss computation */
		tensorflow::GradientTape tape;
		tape.Watch({coefficientsVar, biasesVar});
#endif /* def TENSORFLOW_HAS_GRADIENTTAPE */
		tensorflow::Output loss, grad;
		auto scaleNum = tensorflow::ops::Const(root, 1.0f);
		if(objectModeBool == outputMode) { /* binary classification loop */
			auto unkClass = SigmoidCrossEntropyWithLogits(root, logits, labels);
			loss = unkClass.loss;
			grad = unkClass.backprop;
		} else if(objectModeVectorBool == outputMode) { /* multi-class (categorical) one-hot classification loop */
			auto unkClass = tensorflow::ops::SoftmaxCrossEntropyWithLogits(root, logits, labels);
			loss = unkClass.loss;
			grad = unkClass.backprop;
#ifdef SUSUWU_CNS_MODE_ENUM /* TODO? Is `objectModeEnum` (or `objectModeVectorEnum`) required for class indices? */
		} else if(objectModeEnum == outputMode || objectModeVectorEnum == outputMode) { /* multi-label multi-class classification loop */
			auto unkClass = tensorflow::ops::SparseSoftmaxCrossEntropyWithLogits(root, logits, labels);
			loss = unkClass.loss;
			grad = unkClass.backprop;
#endif /* !def SUSUWU_CNS_MODE_ENUM */
		} else { /* common regression loop */
			loss = tensorflow::ops::SquaredDifference(root, logits, labels); /* squared error */
//			loss = tensorflow::ops::Multiply(root, squared, tensorflow::ops::Const(root, 1.0f / 2.0f));
			grad = tensorflow::ops::Subtract(root, logits /* TODO: should this use `relu`? */, labels); /* shape: [batchSize, outputDim] */
			scaleNum = tensorflow::ops::Const(root, 2.0f); /* `.loss` is `^ 2`, so `.backprop` is `* 1/2`. This is so `scale` does not require the factor of `2` (which would not suit other formulas). */
		}
		loss = tensorflow::ops::Mean(root.WithOpName("loss"), loss, 0);

		/* Gradients (last argument, `delta`, to `tensorflow::ops::ApplyGradientDescent` */
//		auto gradients = tensorflow::ops::Gradients(root, {loss}, {coefficients SUSUWU_CNS_IF_BIAS(SUSUWU_COMMA biases}); /* gives `error: no member named 'Gradients' in namespace 'tensorflow::ops'` */

#ifdef TENSORFLOW_HAS_GRADIENTTAPE
		/* Compute the gradients */
		std::vector<tensorflow::Output> gradientOutputs = tape.Gradient({loss}, {coefficientsVar, biasesVar});
		tensorflow::Output gradCoefficientsScaled = gradientOutputs[0],
			SUSUWU_CNS_IF_BIAS(gradBiasesScaled = gradientOutputs[1]);
		static_cast<void>(grad); /* "style: Variable 'grad' is assigned a value that is never used. [unreadVariable]" fix. */
#else /* ! def TENSORFLOW_HAS_GRADIENTTAPE */
		auto inputTranspose = tensorflow::ops::Transpose(root, input, {1, 0}); /* Transpose input: shape [input_dim, batch_size] */
		auto gradCoefficients = tensorflow::ops::MatMul(root, inputTranspose, grad); /* MatMul: shape [input_dim, output_dim] */
		SUSUWU_CNS_IF_BIAS(auto gradBiases = tensorflow::ops::Sum(root, grad, {0});) /* Sum over batch axis */ /* Notice: swapped order so dependency of `MatMul` on `inputTranspose` is masked with overhead of `Sum` */

		/* Scalar batch size (to divide by) */
		auto batchSize = tensorflow::ops::Shape(root, input /* With `inputDim`, does not converge. With `input`, diverges. TODO: improve */); /* shape: [batch_size, input_dim] */
		auto batchSizeScalar = tensorflow::ops::Slice(root, batchSize, {0}, {1}); /* [batch_size] */
		auto batchSizeFloat = tensorflow::ops::Cast(root, batchSizeScalar, coefficientFormat);
		auto batchSizeReduced = tensorflow::ops::Squeeze(root, batchSizeFloat); /* Reduce batchSize to scalar */

		auto scale = tensorflow::ops::Div(root, scaleNum, batchSizeReduced);

		/* Scaled grads (`ApplyGradientDescent`'s `delta` = `grad / scale`) */
		auto gradCoefficientsScaled = tensorflow::ops::Multiply(root, gradCoefficients, scale);
		SUSUWU_CNS_IF_BIAS(auto gradBiasesScaled = tensorflow::ops::Multiply(root, gradBiases, scale);) /* use same scale as above */
#endif /* def TENSORFLOW_HAS_GRADIENTTAPE */

#if SUSUWU_CNS_IF_MLP
#	pragma message("TODO: `ApplyGradientDescent` loop for `SUSUWU_CNS_USE_MLP`")
//	for(long w = 0; this->layersOfNeurons > w; ++w) { /* TODO */ }
#endif /* SUSUWU_CNS_IF_MLP */
	//		auto optimizer = tensorflow::ops::ApplyAdam(root, coefficientsVar, SUSUWU_CNS_IF_BIAS(biasesVar SUSUWU_COMMA), learningFactor); /* TODO? Heard this is just part of the Python TensorFlow */
//		auto optimizer = tensorflow::ops::AdamOptimizer(learningFactor); /* TODO? */
		auto optimizerCoefficients = tensorflow::ops::ApplyGradientDescent(root.WithOpName("optimizerCoefficients"), coefficientsVar, learningFactor, gradCoefficientsScaled /* gradients[0] */); /* TODO: allow to configure this? Default is `SGD` (Stochastic Gradient Descent). */
		SUSUWU_CNS_IF_BIAS(auto optimizerBiases = tensorflow::ops::ApplyGradientDescent(root.WithOpName("optimizerBiases"), biasesVar, learningFactor, gradBiasesScaled /* gradients[1] */);) /* TODO: the derivative of `coefficients` is `n`, but the derivative of `biases` is `1`; use low-order optimizer for `biases`? */

		auto initCoefficients = tensorflow::ops::Assign(root.WithOpName("initCoefficients"), coefficientsVar, tensorflow::ops::Const(root, 0.2f /* gradients are all `0` if initial values are `0` */, tensorflow::TensorShape({inputDim, outputDim})) /* std::vector<std::vector<CoefficientType>>(inputDim, std::vector<CoefficientType>(outputDim, 0.0f)))*/ );
		auto assignCoefficients = tensorflow::ops::Assign(root.WithOpName("assignCoefficients"), coefficientsVar, randomCoefficientsVar);
		SUSUWU_CNS_IF_BIAS(auto initBiases = tensorflow::ops::Assign(root.WithOpName("initBiases"), biasesVar, tensorflow::ops::Const(root, 0.2f, tensorflow::TensorShape({outputDim})));)
		SUSUWU_CNS_IF_BIAS(auto assignBiases = tensorflow::ops::Assign(root.WithOpName("assignBiases"), biasesVar, randomBiasesVar);)
//		auto init = tensorflow::ops::InitializeVariables(root.WithOpName("init")); /* gives `error: no member named 'InitializeVariables' in namespace 'tensorflow::ops'` */

		/* Create and init the graph (`this->graphDef`) */
		TF_CHECK_OK(root.ToGraphDef(&graphDef)); /* produce `tensorflow::GraphDef` */
		TF_CHECK_OK(session->Create(graphDef));
		TF_CHECK_OK(session->Run({}, {}, {"initCoefficients" SUSUWU_CNS_IF_BIAS(SUSUWU_COMMA "initBiases")}, SUSUWU_NULLPTR)); /* Prevents "LoadError: Tensorflow error: Status: Attempting to use uninitialized value" in case `pseudoRandomSynapses` is not used. */
	}
	void printGraphNodes() const {
		SUSUWU_INFO(getName() + "::" + __func__ + "() { for(const auto &node : graphDef.node()) { ");
		for(const auto &node : graphDef.node()) {
			SUSUWU_INFO("    node.name() == \"" + node.name() + "\"; node.op == \"" + node.op() + "\";");
		}
		SUSUWU_INFO("} }");
	}

	template<class CoefficientType, class Input, class Output>
	void setupSynapsesImpl(const std::vector<std::tuple<Input /* "input" */, Output /* "label" */>> &inputsToOutputs, size_t trainingIterations) {
		const size_t inputCount = inputsToOutputs.size();
		const size_t validationCount = (1 < inputCount) ? (inputCount * validationFactor) : 0;
		const size_t trainingCount = inputCount - validationCount;
		const auto inputDim = static_cast<DimSz>(inputNeurons); /* TODO: assert `1` for base types? */
		const auto outputDim = static_cast<DimSz>(outputNeurons); /* TODO: assert `1` for base types? */
		assert(ToObjectMode<Input>::value == inputMode);
		setInitialized(false);
		tensorflow::Tensor inputTensor(DataTypeToEnum<CoefficientType /* implicit conversion from `Input` to prevent "INVALID_ARGUMENT: Inconsistent values" */>::value, tensorflow::TensorShape({static_cast<DimSz>(trainingCount), static_cast<DimSz>(inputDim)})) /* backpropagation's `x` axis, which TensorFlow calls "input" */,
			inputTensor2(DataTypeToEnum<CoefficientType /* implicit conversion from `Input` */>::value, tensorflow::TensorShape({static_cast<DimSz>(validationCount), static_cast<DimSz>(inputDim)})) /* validation holdout values */,
			expectedOutputTensor(DataTypeToEnum<CoefficientType /* implicit conversion from `Output` */>::value, tensorflow::TensorShape({static_cast<DimSz>(trainingCount), static_cast<DimSz>(inputDim)})) /* backpropagation's `y` axis, which TensorFlow calls "label" */,
			expectedOutputTensor2(DataTypeToEnum<CoefficientType /* implicit conversion from `Output` */>::value, tensorflow::TensorShape({static_cast<DimSz>(validationCount), static_cast<DimSz>(inputDim)})) /* validation holdout values */;

		auto inputTensorMapped = inputTensor.matrix<CoefficientType /* implicit conversion from `Input` */>(),
			inputTensorMapped2 = inputTensor2.matrix<CoefficientType /* implicit conversion from `Input` */>();
		auto expectedOutputTensorMapped = expectedOutputTensor.matrix<CoefficientType /* implicit conversion from `Output` */>(),
			expectedOutputTensorMapped2 = expectedOutputTensor2.matrix<CoefficientType /* implicit conversion from `Output` */>();
#ifdef SUSUWU_CNS_SHUFFLE
		auto shuffledInputs = inputsToOutputs; /* TODO: shuffle without duplicate, since some training sets use more than half of RAM resources */
		std::shuffle(shuffledInputs.begin(), shuffledInputs.end(), std::default_random_engine(/* `std::random_devices()` or constant (for reproducible results) */));
		auto inputsToOutputsIt = shuffledInputs.cbegin();
#else /* else !def SUSUWU_CNS_SHUFFLE */
		auto inputsToOutputsIt = inputsToOutputs.cbegin(); /* assume that `inputsToOutputs` is pre-shuffled, or that `inputsToOutputs` was produced to not have clustered data (not have 1 type of samples in the first half and another type of samples in the second, which would make the training samples unmatched to the validation samples) */
#endif /* !def SUSUWU_CNS_SHUFFLE */

		inputNormsStorage = NumeralNormalizers::fromTuple<0>(inputsToOutputs);
#if SUSUWU_CNS_SEPARATE_NORMS
		outputNormsStorage = NumeralNormalizers::fromTuple<1>(inputsToOutputs);
#endif /* !SUSUWU_CNS_SEPARATE_NORMS */
		NumeralNormalizersReciprocal inputNormsRecip(inputNorms()),
			outputNormsRecip(outputNorms());
		for(size_t i = 0; i < trainingCount; ++i, ++inputsToOutputsIt) {
			inputTensorMapped(i, 0 /* TODO: for versions of `setupSynapses` where `std::tuple` holds `std::vector`s, replace 0 with `std::vector` index, and loop */) = numeralNormalization(std::get<0>(*inputsToOutputsIt), inputNormsRecip); /* Training input */
			expectedOutputTensorMapped(i, 0) = numeralNormalization(std::get<1>(*inputsToOutputsIt), outputNormsRecip); /* Expected training output */
		}
		for(size_t i = 0; i < validationCount; ++i, ++inputsToOutputsIt) {
			inputTensorMapped2(i, 0) = numeralNormalization(std::get<0>(*inputsToOutputsIt), inputNormsRecip); /* Validation input */
			expectedOutputTensorMapped2(i, 0) = numeralNormalization(std::get<1>(*inputsToOutputsIt), outputNormsRecip); /* Expected validation output */
		} /* TODO: use `std::shuffle` (or similar formula to shuffle `inputsToOutputs`) if overfitting results from localized clusters */

		/* Define the neural network architecture */
		restructureConnectomeImpl<CoefficientType>(); /* TODO: move `Scope` construction into this? Move `GraphDef` construction into this? */
//		auto coefficientsMapped = coefficients.matrix<CoefficientType>(); /* TODO? https://poe.com/s/kMLk4ogWhaNWvpIsHruCoefficients */
		initScopeRootBack<Input, Output>(DataTypeToEnum<CoefficientType>::value);
		pseudoRandomSynapses<CoefficientType>(inputsToOutputs); /* Initialize `"coefficients"` with pseudorandom (uses `rand()` or recipricals of `inputsToOutputs`) values */

		if(0 == trainingIterations) {
			trainingIterations = 1000; /* TODO: use input specifics (and available host resources?) to compute best value */
		}
		const std::vector<std::string> outputTensors = {"loss"};
		float bestLoss = std::numeric_limits<float>::max(),
			bestLossAbs = std::abs(bestLoss);
		size_t patienceCounter = 0;
		for(size_t epoch = 0; epoch < trainingIterations; ++epoch) {
			std::vector<tensorflow::Tensor> outputs;
			tensorflow::Status status = session->Run(
				{
					{"input", inputTensor},
					{"labels", expectedOutputTensor}
				},
				(0 == validationCount) ? outputTensors : std::vector<std::string>(),
				{"optimizerCoefficients" SUSUWU_CNS_IF_BIAS(SUSUWU_COMMA "optimizerBiases")},
				(0 == validationCount) ? &outputs : SUSUWU_NULLPTR
			);
			if(!status.ok()) {
				throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::" + __func__ + "() { const tensorflow::Status status = session->Run({{\"input\", inputTensor}, {\"labels\", expectedOutputTensor}, {" + ((0 == validationCount) ? "\"loss\"" : "") + "}, {\"optimizerCoefficients\" " + "" SUSUWU_CNS_IF_BIAS(", \"optimizerBiases\"") + "}, &outputs); (!status.ok()) { epoch == " + std::to_string(epoch) + "; status.ToString() == \"" + status.ToString() + "\"; } } }"));
			}
			float lossVal;
			if(0 == validationCount) {
				lossVal = outputs[0].scalar<float>()();
			} else {
				outputs.clear();
				status = session->Run(
					{
						{"input", inputTensor2},
						{"labels", expectedOutputTensor2}
					},
					outputTensors, {}, &outputs);
				if(!status.ok()) {
					throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::setupSynapses() { const tensorflow::Status status = session->Run({{\"inputs\", inputTensor2}, {\"labels\", expectedOutputTensor2}}, {\"loss\"}, {}, &outputs); (!status.ok()) { epoch == " + std::to_string(epoch) + "; status.ToString() == \"" + status.ToString() + "\"; } }"));
				}
				lossVal = outputs[0].scalar<float>()();
			}
			static const size_t printoutAmount = 4; /* minimumAbs desired printouts */
			const float lossValAbs = std::abs(lossVal);
			bool printout = false;
			if(printoutAmount > epoch /* logarithmic backoff, to limit amount of debug messages */
					|| (128 > epoch && !(0xF /* 15 */ & epoch))
					|| (32768 > epoch && !(0x7F /* 127 */ & epoch))
					|| ((1UL << 23) > epoch && !(0x7FFF /* ((1 << 15) - 1) */ & epoch))
					|| ((1UL << 31) > epoch && !(0x7FFFFF /* ((1 << 23) - 1) */ & epoch))
			) {
				printout = true;
			}
			if(lossValAbs < desiredLossThreshold) { break; }
			std::string printBestLoss = ""; /* don't print, unless pathological */ /* cppcheck-suppress variableScope */
			const float currentDelta = bestLossAbs - lossValAbs;
			if(currentDelta > minLossDelta) {
				bestLoss = lossVal;
				bestLossAbs = lossValAbs;
				patienceCounter = 0;
			} else { /* Pathological */
				patienceCounter++;
				if(printoutAmount + patienceCounter > patience) { /* If stalled, printout the last `printoutAmount` loops. */
					printout = true;
					if(lossVal > bestLoss) { /* If diverging, then ... */
						printBestLoss = "; bestLoss == " + std::to_string(bestLoss); /* ... print pre-divergence value. */
						printBestLoss += "; currentDelta == " + std::to_string(currentDelta);
						printBestLoss += "; minLossDelta == " + std::to_string(minLossDelta);
					}
				}
				if(patienceCounter >= patience) { break; }
			}
			if(printout) {
				SUSUWU_DEBUG(getName() + "::setupSynapses() { maxEpoch == " + std::to_string(trainingIterations) + "; epoch == " + std::to_string(epoch) + "; lossVal == " + std::to_string(lossVal) + printBestLoss + "; status.ToString() == \"" + status.ToString() + "\"; ... } }");
			}
		}
		setupSynapsesPostProcess();
	}
	void setupSynapsesPostProcess() SUSUWU_OVERRIDE {
		if(0 == graphDef.node_size()) {
			SUSUWU_WARNING(getName() + "::" + __func__ + "() { if(0 == graphDef.node_size()) { /* This path is unreachable unless `setupSynapsesPostProcess()` is called by user code, or is overridden but `setupSynapses()` is not overridden (such as if an unimplemented type of `setupSynapses()` was used) */ } } ");
		} else {
#if SUSUWU_CNS_LOCAL_COEFFICIENTS /* Store model values into `class TensorFlowCns`. */
			std::vector<tensorflow::Tensor> outputs;
			const tensorflow::Status status = session->Run({}, {"coefficients", "biases"}, {}, &outputs);
			if(!status.ok()) {
				printGraphNodes();
				throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::" + __func__ + "() { const tensorflow::Status status = session->Run({}, {\"coefficients\" SUSUWU_CNS_IF_BIAS(SUSUWU_COMMA \"biases\")}, {}, &outputs); (!status.ok()) { outputs.size() == " + std::to_string(outputs.size()) + "; status.ToString() == \"" + status.ToString() + "\"; } }"));
			}
			this->coefficients = outputs[0];
			SUSUWU_CNS_IF_BIAS(this->biases = outputs[1];)
#endif /* SUSUWU_CNS_LOCAL_COEFFICIENTS */
			setInitialized(true);
		}
	}

	/* Also known as "backpropagation" or "gradient descent". `float` to `float` version.
	 * @throw std::runtime_error, tensorflow::errors::Internal, tensorflow::errors::Unavailable */
	void setupSynapses(const std::vector<std::tuple<float /* "input" */, float /* "label" */>> &inputsToOutputs, size_t trainingIterations = 0 /* if 0, guesses suitable loop count */) SUSUWU_OVERRIDE { /* NOLINT(google-default-arguments): is 0 for base and derivatives */
		setupSynapsesImpl<CoefficientDefaultType, float, float>(inputsToOutputs, trainingIterations);
	}

	/* Also known as "backpropagation" or "gradient descent". `int` to `int` version.
	 * @throw std::runtime_error, tensorflow::errors::Internal, tensorflow::errors::Unavailable */
	void setupSynapses(const std::vector<std::tuple<int /* "input" */, int /* "label" */>> &inputsToOutputs, size_t trainingIterations = 0 /* if 0, guesses suitable loop count */) SUSUWU_OVERRIDE { /* NOLINT(google-default-arguments): is 0 for base and derivatives */
		setupSynapsesImpl<CoefficientDefaultType, int, int>(inputsToOutputs, trainingIterations);
	}

	/* Helper function for `processTo*()`. For internal use.
	 * @throw std::runtime_error, tensorflow::errors::Internal, tensorflow::errors::Unavailable */
	template<class CoefficientType, class Output>
	const std::vector<tensorflow::Tensor> processToImpl(const tensorflow::Tensor &inputTensor, const std::string &func) const {
		if(ToObjectMode<Output>::value != outputMode && "processToInt" != func) {
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::" + func + "() { outputMode == " + std::to_string(outputMode) + "; outputMode != " + std::to_string(ToObjectMode<Output>::value) + " /* ToObjectMode<Output>::value */; }"));
		}
//		initScopeRootForward<Input, Output>(DataTypeToEnum<CoefficientType>::value); /* TODO: uncomment if `root` is set to `mutable` */
		std::vector<tensorflow::Tensor> oTensors;
		const tensorflow::Status status = session->Run(
			{{"input", inputTensor}/*, {"coefficients", coefficients} SUSUWU_CNS_IF_BIAS(SUSUWU_COMMA {"biases", biases}) */},
			{"logits"}, {"logits"}, &oTensors);
		if(!status.ok()) {
			const std::string coefficientStr = "/*, {\"coefficients\", coefficients}" SUSUWU_CNS_IF_BIAS(+ std::string(", {\"biases\", biases}")) + "*/"; /* Notice: can set to "" */
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::" + func + "() { const tensorflow::Status status = session->Run({{\"input\", inputTensor}" + coefficientStr + "}, {\"output\"}, {}, &oTensors); (!status.ok()) { status.ToString() == \"" + status.ToString() + "\"; } }"));
		}
		return oTensors; //return static_cast<Output /* cannot cast to `std::string` */>(oTensors[0].matrix<CoefficientType>()(0, 0));
	}

	/* Also known as "forwardpropagation" or "inference". `float` version.
	 * @throw std::runtime_error, tensorflow::errors::Internal, tensorflow::errors::Unavailable */
	const float processToFloat(const float &input) const SUSUWU_OVERRIDE {
		tensorflow::Tensor inputTensor(DataTypeToEnum<CoefficientDefaultType /* If `CoefficientDefaultType` switches to `double`, `float` could give "INVALID_ARGUMENT: Inconsistent values" */>::value, tensorflow::TensorShape({1, 1}));
		inputTensor.matrix<CoefficientDefaultType>()(0, 0) = numeralNormalization(input, inputNorms());
		auto oTensors = processToImpl<CoefficientDefaultType, float>(inputTensor, __func__);
		return static_cast<float>(numeralDenormalization(oTensors[0].matrix<CoefficientDefaultType /* `float` */>()(0, 0), outputNorms()));
	}

	/* Also known as "forwardpropagation" or "inference". `int` version.
	 * @throw std::runtime_error, tensorflow::errors::Internal, tensorflow::errors::Unavailable */
	const int processToInt(const int &input) const SUSUWU_OVERRIDE {
		return static_cast<int>(processToFloat(static_cast<float>(input))); /* TODO: figure out the reason that the code below has outputs stuck, plus how to improve */
		tensorflow::Tensor inputTensor(DataTypeToEnum<CoefficientDefaultType /* `int` gives "INVALID_ARGUMENT: Inconsistent values" */>::value, tensorflow::TensorShape({1, 1}));
		inputTensor.matrix<CoefficientDefaultType>()(0, 0) = static_cast<CoefficientDefaultType>(numeralNormalization(input, inputNorms()));
		auto oTensors = processToImpl<CoefficientDefaultType, int>(inputTensor, __func__);
		return static_cast<int>(numeralDenormalization(oTensors[0].matrix<CoefficientDefaultType /* `int` */>()(0, 0), outputNorms()));
	}

	/* Also known as "forwardpropagation" or "inference". `std::string` version. */
	const std::string processToString(const std::string &input) const SUSUWU_OVERRIDE {
//		tensorflow::Tensor inputTensor(tensorflow::DT_STRING, tensorflow::TensorShape({1, 1})); /* Alternative? [`tensorflow::ops::DecodeRaw`](https://www.tensorflow.org/api_docs/cc/class/tensorflow/ops/decode-raw) into tensor of `char`s */
//		inputTensor.matrix<tensorflow::tstring>()(0, 0) = input; /* `<std::string>` gives `tensorflow/core/framework/types.h:336:3: error: static_assert failed due to requirement 'IsValidDataType<std::basic_string<char, std::char_traits<char>, std::allocator<char>>>::value' "Specified Data Type not supported"` */
		tensorflow::Tensor inputTensor(DataTypeToEnum<CoefficientDefaultType>::value, tensorflow::TensorShape({1, 1} /* TODO: map sentences into tokens */));
		inputTensor.matrix<CoefficientDefaultType>()(0, 0) = std::stof(input /* map text number into `float` */);
		auto oTensors = processToImpl<CoefficientDefaultType, tensorflow::tstring>(inputTensor, __func__);
		return std::to_string(oTensors[0].matrix<CoefficientDefaultType >()(0, 0 /* TODO: tokens, which map into sentences */));
	}

	/* store the connectome (the synapse coefficients and biases); serialize from `TensorFlowCns::coefficients` (or `TensorFlowCns::graphDef`), into `modelPath`.
	 * @throw std::runtime_error */
	void dumpTo(const ClassIoPath &modelPath) const SUSUWU_OVERRIDE;
	/* load the connectome (the synapse coefficients and biases); deserialize into `TensorFlowCns::const` (or `TensorFlowCns::graphDef`), from `modelPath`.
	 * @throw std::runtime_error, tensorflow::errors::Internal, tensorflow::errors::Unavailable */
	void loadFrom(const ClassIoPath &modelPath) SUSUWU_OVERRIDE;

protected: /* NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes) */
	tensorflow::Session* session = SUSUWU_NULLPTR;
	tensorflow::Scope root; /* TODO: If not used after init, move this into `setupRootScope()` */
	tensorflow::GraphDef graphDef; /* TODO: If not used after init, move this into `setupRootScope()`. Can `dumpTo` use? */
#if SUSUWU_CNS_LOCAL_COEFFICIENTS
	tensorflow::Tensor coefficients = tensorflow::Tensor(tensorflow::DT_FLOAT, tensorflow::TensorShape({static_cast<DimSz>(inputNeurons), static_cast<DimSz>(neuronsPerLayer)})); /* connectome coefficients ("synaptic strengths") */
#	if SUSUWU_CNS_USE_BIAS
	tensorflow::Tensor biases = tensorflow::Tensor(tensorflow::DT_FLOAT, tensorflow::TensorShape({static_cast<DimSz>(neuronsPerLayer)})); /* connectome biases ("membrane potentials") */
#	endif /* SUSUWU_CNS_USE_BIAS */
#endif /* SUSUWU_CNS_LOCAL_COEFFICIENTS */
/* NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes) */
};
const bool classTensorFlowCnsTests();
static const bool classTensorFlowCnsTestsNoexcept() SUSUWU_NOEXCEPT {return templateCatchAll(classTensorFlowCnsTests, "classTensorFlowCnsTests()");}

}; /* namespace Susuwu */
```

`less `[`cxx/ClassTensorFlowCns.cxx`](../cxx/ClassTensorFlowCns.cxx)
```c++
/* store the connectome (the synapse coefficients and biases); serialize from `TensorFlowCns::coefficients` (or `TensorFlowCns::graphDef`), into `modelPath`.
 * @throw std::runtime_error */
void TensorFlowCns::dumpTo(const ClassIoPath &modelPath) const { /* TODO: the implementation is in the header to allow future `template<>` use; If `dumpTo` will not use C++ `template<>`s, implement in `ClassTensorFlowCns.cxx` to reduce `./build.sh` resource use */
#if !SUSUWU_IN_MEMORY_COEFFICIENTS
	std::vector<tensorflow::Tensor> outputs;
	TF_CHECK_OK(session->Run({}, {"coefficients", "biases"}, {}, &outputs));
	const auto &coefficients = outputs[0];
	const auto &biases = outputs[1];
#endif /* !SUSUWU_IN_MEMORY_COEFFICIENTS */
#ifdef SUSUWU_TENSORFLOWCNS_PROTOBUF_FS
	tensorflow::TensorProto tensorProto;
#ifdef SUSUWU_TENSORFLOWCNS_BACKUP_TENSORS
	coefficients.AsProtoField(&tensorProto);
	biases.AsProtoField(&tensorProto);
	const tensorflow::Status status = tensorflow::WriteBinaryProto(tensorflow::Env::Default(), modelPath, tensorProto);
#else /* !def SUSUWU_TENSORFLOWCNS_BACKUP_TENSORS */
	const tensorflow::Status status = tensorflow::WriteBinaryProto(tensorflow::Env::Default(), modelPath, graphDef);
#endif /* !def SUSUWU_TENSORFLOWCNS_BACKUP_TENSORS */
	if (!status.ok()) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::dumpTo(.modelPath = \""+ modelPath + "\") { tensorflow::WriteBinaryProto(tensorflow::Env::Default(), modelPath, graphDef).ToString() == \"" + status.ToString() + "\" }"));
	}
#elif defined(SUSUWU_TENSORFLOWCNS_ROCM_FS)
	const tensorflow::Status status = tensorflow::SaveTensorToFile(modelPath, coefficients); /* TODO: Assistant suggested to use this function, but don't know what to include. [All Google found was ROCm](https://rocm.docs.amd.com/projects/rocPyDecode/en/latest/reference/decoderClass.html#savetensortofile-output-file-path-frame-adrs-width-height-rgb-format-surface-info) */ /* TODO: biases */
	if(!status.ok()) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::dumpTo(.modelPath = \""+ modelPath + "\") { !tensorflow::SaveTensorToFile(modelPath, coefficients).ok() }"));
	}
#elif defined(SUSUWU_TENSORFLOWCNS_SAVEDMODEL_FS)
	throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::dumpTo(.modelPath = \""+ modelPath + "\") { /* TODO; `SUSUWU_TENSORFLOWCNS_SAVEDMODEL_FS`. Numerous assistants suggest functions (such as `LoadSessionFromSavedModel`) which are not found, and results of related searches (such as for \"SavedModelBundle\") amount to \"TensorFlow's C / C++ API's do not have this for now\" */"));
#else /* else SUSUWU_TENSORFLOWCNS_MANUAL_FS */
	throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::dumpTo(.modelPath = \""+ modelPath + "\") { /* TODO; `SUSUWU_TENSORFLOWCNS_MANUAL_FS`. Perhaps just dump binary `coefficients` into `modelPath`? */"));
#endif /* SUSUWU_TENSORFLOWCNS_MANUAL_FS */
}

/* load the connectome (the synapse coefficients and biases); deserialize into `TensorFlowCns::const` (or `TensorFlowCns::graphDef`), from `modelPath`.
 * @throw std::runtime_error, tensorflow::errors::Internal, tensorflow::errors::Unavailable */
void TensorFlowCns::loadFrom(const ClassIoPath &modelPath) { /* TODO: the implementation is in the header to allow future `template<>` use; If `loadFrom` will not use C++ `template<>`s, implement in `ClassTensorFlowCns.cxx` to reduce `./build.sh` resource use */
	switch(inputMode) { /* TODO: support more types, or replace `template<class Input>` on `initScopeRootForward` with an argument (such as `ObjectMode input`) */
		case objectModeFloat:	initScopeRootForward<float, float>(DataTypeToEnum<float>::value); break;
		case objectModeInt:	initScopeRootForward<int, int>(DataTypeToEnum<int>::value); break;
		default:	throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(.modelPath = \""+ modelPath + "\") { inputMode == " + std::to_string(inputMode) + "; /* unsupported `ObjectMode` */ }"));
	}
#ifdef SUSUWU_TENSORFLOWCNS_PROTOBUF_FS
//	tensorflow::GraphDef graphDef; /* TODO: uncomment (use temp `tensorflow::GraphDef`) or remove (settle on `TensorFlowCns::graphDef`) */
#	ifdef SUSUWU_TENSORFLOWCNS_BACKUP_TENSORS
	throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(.modelPath = \""+ modelPath + "\") { /* TODO; `SUSUWU_TENSORFLOWCNS_BACKUP_TENSORS`. Solution: unset that macro for now. */ }"));
#	else /* !def SUSUWU_TENSORFLOWCNS_BACKUP_TENSORS */
	const tensorflow::Status status = ReadBinaryProto(tensorflow::Env::Default(), modelPath, &graphDef);
	if(!status.ok()) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(.modelPath = \""+ modelPath + "\") { !tensorflow::ReadBinaryProto(tensorflow::Env::Default(), modelPath, &graphDef).ok() }"));
	}
//		TF_CHECK_OK(root.ToGraphDef(&graphDef)); /* uncomment if `ReadBinaryProto` loads `tensorflow::Scope root` */
//		TF_CHECK_OK(session->Create(graphDef)); /* uncomment to reload `tensorflow::GraphDef` */
#	endif /* !def SUSUWU_TENSORFLOWCNS_BACKUP_TENSORS */
#elif defined(SUSUWU_TENSORFLOWCNS_ROCM_FS)
	const tensorflow::Status status = tensorflow::ReadTensorFromFile(modelPath, &coefficients); /* Assistant suggested to use this function, but don't know what to include. All Google found was [ShuffleNetV2Plus from MindX SDK](https://zhuanlan.zhihu.com/p/558676663) */ /* TODO: biases */
#elif defined(SUSUWU_TENSORFLOWCNS_SAVEDMODEL_FS)
	tensorflow::SavedModelBundle bundle;
	tensorflow::SessionOptions sessionOptions;
	tensorflow::RunOptions runOptions;
	const tensorflow::Status status = tensorflow::LoadSessionFromSavedModel(sessionOptions, runOptions, modelPath, {tensorflow::kSavedModelTagServe}, &bundle); /* TODO: GitHub's assistant suggests this function, but don't know what to `#include` */
	if(!status.ok()) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(.modelPath = \""+ modelPath + "\") { tensorflow::LoadSessionFromSavedModel(sessionOptions, runOptions, modelPath, {tensorflow::kSavedModelTagServe}, &bundle).ToString() == \"" + status.ToString() + "\" }"));
	} /* Use `tensorflow::SavedModelBundle`? */
	session = bundle.session.get();
#else /* SUSUWU_TENSORFLOWCNS_MANUAL_FS */
	auto env = tensorflow::Env::Default(); /* OS / platform environment handle */

	auto status = env->FileExists(modelPath);
	if(status.ok() == false) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(\""+ modelPath + "\") { (!env->FileExists(modelPath).ok()) }"));
	}

	tensorflow::uint64 fileSize;
	status = env->GetFileSize(modelPath, &fileSize);

	if(status.ok() == false) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(\""+ modelPath + "\") { tensorflow::uint64 fileSize); (!env->GetFileSize(modelPath, &fileSize).ok()) }"));
	}

	SUSUWU_DEBUG(getName() + "::loadFrom(modelPath = \""+ modelPath + "\") { tensorflow::uint64 fileSize; env->GetFileSize(modelPath, &fileSize); (fileSize == " + std::to_string(fileSize) + ") }");

	std::unique_ptr<tensorflow::RandomAccessFile> fileHandle;
	status = env->NewRandomAccessFile(modelPath, &fileHandle);

	if(status.ok() == false) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(modelPath = \""+ modelPath + "\") { std::unique_ptr<tensorflow::RandomAccessFile> fileHandle; (!env->NewRandomAccessFile(modelPath, &fileHandle).ok()) }"));
	}

	std::string content;
	content.resize(fileSize);

	tensorflow::StringPiece sp;

	status = fileHandle->Read(0, fileSize, &sp, &(content)[0]);

	if(status.ok() == false) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(\""+ modelPath + "\") { std::unique_ptr<tensorflow::RandomAccessFile> fileHandle; (!fileHandle->Read(0, fileSize, &sp, &(content)[0]).ok()) }"));
	}

//	coefficients = /* TODO */
	throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(\""+ modelPath + "\") { /* TODO; deserialize `sp` into `tensorflow::Tensor`. Use `tensorflow::SavedModelBundle` or `TF_LoadSessionFromSavedModel`? */"));
#endif /* SUSUWU_TENSORFLOWCNS_MANUAL_FS */
}

template <typename Numeral, class Input, class Output, typename Process>
static const bool classTensorFlowCnsTestLinear(const Input min, const Input max, const Input step, const Numeral epsilon /* For now, `epsilon` is best result known (so that regressions trigger debug messages) */, Process process) {
	bool success = true;
	std::vector<std::tuple<Input, Output>> inputsToOutputs;
//	 epsilon = std::numeric_limits<Numeral>::epsilon /* TODO: [compute most accurate possible values](https://github.com/copilot/share/c056538e-08c0-8822-9001-720924696114) */
	for(Input input = min; max >= input; input += step) {
		inputsToOutputs.push_back({input, input * 2});
	}
	TensorFlowCns tensorFlowCns;
	Cns &cns = tensorFlowCns;
	cns.setInputNeurons(1);
	cns.setInputMode(ToObjectMode<Input>::value);
	cns.setNeuronsPerLayer(1);
	cns.setLayersOfNeurons(1);
	cns.setOutputNeurons(1);
	cns.setOutputMode(ToObjectMode<Output>::value);
#if SUSUWU_TENSORFLOW_EXCEPTIONS
	try {
#endif /* SUSUWU_TENSORFLOW_EXCEPTIONS */
		const size_t inputsCount = inputsToOutputs.size();
		cns.setupSynapses(inputsToOutputs);
		{
#if SUSUWU_CNS_IS_VALUE_OBJECT /* if comparison is not limited to object addresses */
			TensorFlowCns tensorFlowCns2 = tensorFlowCns;
			Cns &cns2 = tensorFlowCns2;
#endif /* SUSUWU_CNS_IS_VALUE_OBJECT */
			assert(inputsCount == inputsToOutputs.size());
			Output worstLoss = 0;
			for(unsigned index = 0; inputsToOutputs.size() > index; ++index) {
				const Output label = std::get<1>(inputsToOutputs.at(index)),
					output = process(cns, std::get<0>(inputsToOutputs.at(index))),
					loss = label - output,
					absLoss = abs(loss);
				if(absLoss > worstLoss) {
					worstLoss = absLoss;
				}
				if(!(epsilon >= absLoss) /* inverted test (with `!`) catches `nan` */) {
					/* throw std::runtime_error */SUSUWU_WARNING("classTensorFlowCnsTests(.min = " + std::to_string(min) + ", .max = " + std::to_string(max) + ", .step = " + std::to_string(step) + ", .epsilon == " + std::to_string(epsilon) + ") { label[" + std::to_string(index) + "] == " + std::to_string(label) + "; output[" + std::to_string(index) + "] == " + std::to_string(output) + "; loss /* label - output */ == " + std::to_string(loss) + "; if(!(abs(loss) <= epsilon)) { throw std::runtime_error; } }");
//					return false; /* abort on first warning */
					success = false; /* so all warnings show */
				}
			}
			if(epsilon > (1 + worstLoss)) {
				SUSUWU_DEBUG("classTensorFlowCnsTests(.min = " + std::to_string(min) + ", .max = " + std::to_string(max) + ", .step = " + std::to_string(step) + ", .epsilon == " + std::to_string(epsilon) + ") { worstLoss == " + std::to_string(worstLoss) + "; /* `(epsilon > (1 + worstLoss))`, set `epsilon = worstLoss` to detect future regressions */; }");
			}

#if SUSUWU_CNS_IS_VALUE_OBJECT /* test that `process()` follows `const` method rules, and that `equals()` and `hashCode` do what those are supposed to do */
			assert(cns.equals(cns2));
			assert(cns.hashCode() == cns2.hashCode()); /* cppcheck-suppress knownConditionTrueFalse */
#endif /* SUSUWU_CNS_IS_VALUE_OBJECT */
		}

#ifdef SUSUWU_TENSORFLOWCNS_HAS_DUMPTO /* TODO: implement `dumpTo()` and `loadFrom()`, remove `cppcheck-suppress knownConditionTrueFalse`. */
		TensorFlowCns tensorFlowCns2 = tensorFlowCns;
		Cns &cns2 = tensorFlowCns2;
		const ClassIoPath dumpToPath = "classTensorFlowCnsTests.tmp"; /* TODO: if file exists, ask user what to do? TODO: use system temp path? */
		cns.dumpTo(dumpToPath);
		cns2.loadFrom(dumpToPath);
//		classIoRemove(dumpToPath); /* TODO: does our IO library have a function for file removal? Perhaps there is some `fopen` flag to use instead, which causes removal as soon as the file is closed?  */
		assert(cns.equals(cns2));
		assert(cns.hashCode() == cns2.hashCode()); /* cppcheck-suppress knownConditionTrueFalse */
#endif /* else ndef SUSUWU_TENSORFLOWCNS_HAS_DUMPTO */

#if SUSUWU_TENSORFLOW_EXCEPTIONS
	} catch(const tensorflow::error::Status &w) {
		(void) /* TODO */
	}
#endif /* SUSUWU_TENSORFLOW_EXCEPTIONS */
	return success;
}
const bool classTensorFlowCnsTests() {
	bool success = true;
	std::function<const float(const Cns &, const float)> processToFloatLambda = [](const Cns &cns, const float x) { return cns.processToFloat(x); };
	std::function<const int(const Cns &, const int)> processToIntLambda = [](const Cns &cns, const int x) { return cns.processToInt(x); };
	classTensorFlowCnsTestLinear<TensorFlowCns::CoefficientDefaultType, float, float>(-1.0, 1.0, 0.001, /* 0.072094 ... */ 0.1180462, processToFloatLambda) || (success = false); /* "normalization" (average == 0, std == 1), most simple to learn */
	classTensorFlowCnsTestLinear<TensorFlowCns::CoefficientDefaultType, float, float>(-1000.0, 1000.0, 1.0, /* 70.774782 ... */ 107.8262, processToFloatLambda) || (success = false); /* (average == 0, std == 1000) */
	classTensorFlowCnsTestLinear<TensorFlowCns::CoefficientDefaultType, float, float>(0.0, 2.0, 0.001, /* 0.072054 ... */ 0.118012, processToFloatLambda) || (success = false); /* (average == 1, std == 1) */
	classTensorFlowCnsTestLinear<TensorFlowCns::CoefficientDefaultType, float, float>(0.0, 2000.0, 1.0, /* 70.698730 ... */ 117.921631, processToFloatLambda) || (success = false); /* (average == 1000, std == 1000) */
	classTensorFlowCnsTestLinear<TensorFlowCns::CoefficientDefaultType, int, int>(-1000, 1000, 1, /* TODO: reduce this */ 2242, processToIntLambda) || (success = false);
	classTensorFlowCnsTestLinear<TensorFlowCns::CoefficientDefaultType, int, int>(0, 2000, 1, /* TODO: reduce this */ 2552, processToIntLambda) || (success = false);
	return success;
}
```

`less `[`cxx/VirusAnalysis.hxx`](../cxx/VirusAnalysis.hxx)
```c++
/* (Work-in-progress) virus analysis: uses hashes, signatures, static analysis, sandboxes, plus artificial CNS (central nervous systems) */
typedef enum VirusAnalysisHook : unsigned char {
	virusAnalysisHookDefault = 0,      /* "real-time" virus scans not initialized */
	virusAnalysisHookQuery   = 0,      /* return present hooks (as enum) */
	virusAnalysisHookClear   = 1 << 0, /* unhook (remove present hooks), then parse rest of bits */
	virusAnalysisHookExec    = 1 << 1, /* hook {execl(), execlp(), execle(), execv(), execvp(), execvpe()} */
	virusAnalysisHookNewFile = 1 << 2, /* hook (for modeNew in {"w+", "a", "a+"}) fwrite((void *)ptr, (size_t)size, (size_t)nmemb, {fopen((const char *)pathname, modeNew), fdopen((int)fd, modeNew), freopen((const char *)pathname, modeNew, (FILE *)stream)}) */
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

extern bool virusAnalysisResultListIndex, virusAnalysisResultListWhitespace, virusAnalysisResultListPascal;
/* @throw what `std::istream` throws (std::bad_alloc, std::runtime_error?).
 * @pre @code std::istream(path) @endcode
 * @post @code passList.hashes.size() @endcode */
const bool virusAnalysisInit(const ClassIoPath &path, ResultList &passList, ResultList &abortList); /* virusAnalysisLoadFrom(path + ".{pass, abort}OrNull.config", {pass, abort}list); */
/* @pre @code std::ostream(path) @endcode
 * @throw what `std::ostream` throws. */
void virusAnalysisDumpTo(const ClassIoPath &path, const ResultList &list);
/* @pre @code std::istream(path) @endcode
 * @throw what `std::istream` throws. */
void virusAnalysisLoadFrom(const ClassIoPath &path, ResultList &list);

#if SUSUWU_UNIT_TESTS
/* `return (produceAbortListSignatures(EXAMPLES) && produceAnalysisCns(EXAMPLES) && produceVirusFixCns(EXAMPLES)) && virusAnalysisHookTests();`
 * @throw std::bad_alloc, std::runtime_error
 * @pre @code !analysisCns.isPureVirtual() && !virusFixCns.isPureVirtual() @endcode */
const bool virusAnalysisTests();
static const bool virusAnalysisTestsNoexcept() SUSUWU_NOEXCEPT { return templateCatchAll(virusAnalysisTests, "virusAnalysisTests()"); } /* cppcheck-suppress throwInNoexceptFunction */
const bool virusAnalysisInitTests(const ClassIoPath path, ResultList &passList, ResultList &abortList); /* virusAnalysisDumpTo(path + ".{pass, abort}OrNull.config", {pass, abort}list); return virusAnalysisInit(path, passList, abortList); */
const bool virusAnalysisHookTests(); /* return for(x: VirusAnalysisHook) {x == virusAnalysisHook(x)};` */
static const bool virusAnalysisHookTestsNoexcept() SUSUWU_NOEXCEPT { return templateCatchAll(virusAnalysisHookTests, "virusAnalysisHookTests()"); } /* cppcheck-suppress throwInNoexceptFunction */
#endif /* SUSUWU_UNIT_TESTS */

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
const VirusAnalysisResult straceOutputsAnalysis(const ClassIoPath &straceOutput); /* TODO: regex */

/* Analysis CNS */
/* Setup analysis CNS; is slow to produce (requires access to huge file databases);
but once produced, uses few resources (allow clients to do fast analysis.)
 * @pre @code !cns.isPureVirtual() && pass.bytecodes.size() && abort.bytecodes.size() @endcode
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
const VirusAnalysisResult virusAnalysis(const PortableExecutable &file); /* auto hash = classSha2(file.bytecode); for(VirusAnalysisFun analysis : virusAnalyses) { auto result = analysis(file, hash); if(virusAnalysisContinue != result) { return result; } } */
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
static const VirusAnalysisResult virusAnalysisInteractive(const PortableExecutable &file) { auto result = virusAnalysis(file); if(virusAnalysisRequiresReview == result) { return virusAnalysisManualReview(file); } return result; }

/* Setup virus fix CMS, uses more resources than `produceAnalysisCns()` */
/* `abortOrNull` should map to `passOrNull` (`ResultList` is composed of `std::tuple`s, because just `produceVirusFixCns()` requires this),
 * with `abortOrNull->bytecodes[x] = NULL` (or "\0") for new SW synthesis,
 * and `passOrNull->bytecodes[x] = NULL` (or "\0") if infected and CNS can not cleanse this.
 * @pre @code !cns.isPureVirtual() @endcode
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

`less `[`cxx/VirusAnalysis.cxx`](../cxx/VirusAnalysis.cxx)
```c++
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

bool virusAnalysisResultListIndex = false, virusAnalysisResultListWhitespace = false, virusAnalysisResultListPascal = true;
const bool virusAnalysisInit(const ClassIoPath &path, ResultList &passList, ResultList &abortList) {
	virusAnalysisLoadFrom(path + ".passOrNull.config", passList);
	virusAnalysisLoadFrom(path + ".abortOrNull.config", abortList);
	return true;
} /* TODO: handle exceptions in this? */
void virusAnalysisDumpTo(const ClassIoPath &path, const ResultList &list) {
	std::ofstream config(path);
	resultListDumpTo(list, config, virusAnalysisResultListIndex, virusAnalysisResultListWhitespace, virusAnalysisResultListPascal);
}
void virusAnalysisLoadFrom(const ClassIoPath &path, ResultList &list) {
#if IFSTREAM_HAS_IOS_BASE
	std::ifstream config(path);
#else
	const PortableExecutableBytecode configPE(path);
	std::stringstream config(configPE.bytecode);
#endif /* ! IFSTREAM_HAS_IOS_BASE */
	list.hashes.clear(); list.signatures.clear(); list.bytecodes.clear(); /* TODO: have `listLoadFrom()` do? */
	resultListLoadFrom(list, config, virusAnalysisResultListIndex, virusAnalysisResultListWhitespace, virusAnalysisResultListPascal);
}
#if SUSUWU_UNIT_TESTS
const bool virusAnalysisTests() {
	ResultList abortOrNull; {
		abortOrNull.hashes = {}, abortOrNull.signatures = {}, abortOrNull.bytecodes = { /* Produce from an antivirus vendor's (such as VirusTotal.com's) infection databases */
			"infection",
			"infectedSW",
			"corruptedSW",
			""
		};
	}
	ResultList passOrNull; {
		passOrNull.hashes = {}, passOrNull.signatures = {}, passOrNull.bytecodes = { /* Produce from an antivirus vendor's (such as VirusTotal.com's) fresh-files databases */
			"",
			"SW",
			"SW",
			"newSW"
		};
	}
	resultListProduceHashes(passOrNull);
	resultListProduceHashes(abortOrNull);
	produceAbortListSignatures(passOrNull, abortOrNull);
	const ClassIoPath gotOwnPath = classIoGetOwnPath(); /* replaced `argv[0]`, ["Uncontrolled data used in path expression"](https://github.com/SwuduSusuwu/SusuLib/security/code-scanning/1277) fix. */
	virusAnalysisInitTests(gotOwnPath, passOrNull, abortOrNull);
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
	if(ClassIoPath() != gotOwnPath) {
		const PortableExecutableBytecode executable(gotOwnPath);
		if(virusAnalysisAbort == virusAnalysisInteractive(executable)) {
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "{virusAnalysisAbort == virusAnalysisInteractive((args[0]);} /* With such false positives, shouldn't hook kernel modules (next test is to hook+unhook `exec*` to scan programs on launch). */"));
		}
		const ResultList origPassList = passList, origAbortList = abortList;
		passList.bytecodes.push_back(executable.bytecode);
		abortList.bytecodes.push_back("test");
		produceAbortListSignatures(passList, abortList);
		if(virusAnalysisAbort == virusAnalysisInteractive(executable)) {
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "{virusAnalysisAbort == virusAnalysisInteractive(args[0]);} /* Ignored `signaturesAnalysisCaches`. */"));
		}
		virusAnalysisResetCaches();
		if(virusAnalysisAbort != virusAnalysisInteractive(executable)) {
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "{virusAnalysisAbort != virusAnalysisInteractive(args[0]);} /* This test was supposed to match positive but did not. */"));
		}
		passList = origPassList, abortList = origAbortList;
	}
	const bool originalRootStatus = classSysHasRoot();
	classSysSetRoot(true);
	virusAnalysisHookTests();
	classSysSetRoot(originalRootStatus);
	return true;
}

const bool virusAnalysisInitTests(const ClassIoPath path, ResultList &passList, ResultList &abortList) {
	const std::string passPath = path + ".passList.config";
	const std::string abortPath = path + ".abortList.config";
	try {
		const std::ifstream config(passPath);
	} catch (std::exception &w) {
		SUSUWU_WARNING("`std::ifstream config(\"" + passPath + "\");` threw \" " + w.what() + "\"; will skip `virusAnalysisDumpTo()` and `virusAnalysisLoadFrom()` tests.");
		return false;
	}
	virusAnalysisDumpTo(passPath, passList);
	virusAnalysisDumpTo(abortPath, abortList);
	SUSUWU_NOTICE("resultListDumpTo(.list = passList, .os = std::cout, .index = true, .whitespace = true, .pascalValues = false);");
	SUSUWU_EXECUTEVERBOSE(resultListDumpTo(passList, std::cout, true, true, false));
	SUSUWU_NOTICE_EXECUTEVERBOSE((resultListDumpTo(/*.list = */abortList, /*.os = */std::cout, /*.index = */false, /*.whitespace = */false, /*.pascalValues = */false), std::cout << std::endl));
	virusAnalysisLoadFrom(passPath, passList);
	virusAnalysisLoadFrom(abortPath, abortList);
	return true;
}
const bool virusAnalysisHookTests() {
	const VirusAnalysisHook originalHookStatus = virusAnalysisGetHook();
	VirusAnalysisHook hookStatus = virusAnalysisHook(virusAnalysisHookClear | virusAnalysisHookExec);
	if(virusAnalysisHookExec != hookStatus) {
		throw std::runtime_error("`virusAnalysisHook(virusAnalysisHookClear | virusAnalysisHookExec)` == " + std::to_string(hookStatus));
		return false; /* cppcheck-suppress duplicateBreak */
	}
	hookStatus = virusAnalysisHook(virusAnalysisHookClear | virusAnalysisHookNewFile);
	if(virusAnalysisHookNewFile != hookStatus) {
		throw std::runtime_error("`virusAnalysisHook(virusAnalysisHookClear | virusAnalysisHookNewFile)` == " + std::to_string(hookStatus));
		return false; /* cppcheck-suppress duplicateBreak */
	}
	hookStatus = virusAnalysisHook(virusAnalysisHookClear);
	if(virusAnalysisHookDefault != hookStatus) {
		throw std::runtime_error("`virusAnalysisHook(virusAnalysisHookClear)` == " + std::to_string(hookStatus));
		return false; /* cppcheck-suppress duplicateBreak */
	}
	hookStatus = virusAnalysisHook(virusAnalysisHookExec | virusAnalysisHookNewFile);
	if((virusAnalysisHookExec | virusAnalysisHookNewFile) != hookStatus) {
		throw std::runtime_error("`virusAnalysisHook(virusAnalysisExec | virusAnalysisHookNewFile)` == " + std::to_string(hookStatus));
		return false; /* cppcheck-suppress duplicateBreak */
	}
	hookStatus = virusAnalysisHook(virusAnalysisHookClear | originalHookStatus);
	if(originalHookStatus != hookStatus) {
		throw std::runtime_error("`virusAnalysisHook(virusAnalysisHookClear | originalHookStatus)` == " + std::to_string(hookStatus));
		return false; /* cppcheck-suppress duplicateBreak */
	}
	return true;
}
#endif /* SUSUWU_UNIT_TESTS */

static const bool virusAnalysisImpl(const PortableExecutable &file) {
	switch(virusAnalysis(file)) {
	case virusAnalysisPass:
		return true; /* launch this */
	case virusAnalysisRequiresReview:
		return (virusAnalysisPass == virusAnalysisManualReview(file));
	default:
		return false; /* abort */
	}
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
#ifdef SUSUWU_POSIX
		auto lambdaScanExecv = [](const char *pathname, char *const argv[]) { /* NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays); this is the standard function signature. */ /* cppcheck-suppress constParameter */
			return static_cast<int>(virusAnalysisImpl(PortableExecutable(pathname)));
		};
		classSysKernelSetHook(execv, lambdaScanExecv);
#elif defined(SUSUWU_WIN32) /* def SUSUWU_POSIX else */
		auto lambdaScanCreateProcessA = [](LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR                lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation) {
			return virusAnalysisImpl(PortableExecutable(lpApplicationName));
		};
		classSysKernelSetHook(CreateProcessA, lambdaScanCreateProcessA);
#else /* defined(SUSUWU_WIN32) else */
		SUSUWU_ERROR("virusAnalysisHook(virusAnalysisHookExec) { if(!SUSUWU_POSIX && !SUSUWU_WIN32) { /* TODO: you can contribute or post to https://github.com/SwuduSusuwu/SusuLib/issues/new */ } }");
#endif /* defined(SUSUWU_WIN32) else */
		globalVirusAnalysisHook = (globalVirusAnalysisHook | virusAnalysisHookExec);
	}
	if(virusAnalysisHookNewFile & hookStatus) {
//		classSysKernelSetHook(fwrite, lambdaScanFwrite); /* TODO: OS-specific "hook"/"callback" for new files/downloads */
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
			SUSUWU_NOTICE("hashAnalysis(/*.file =*/ \"" + file.path + "\", /*.fileHash =*/ 0x" + classIoHexStr(fileHash) + ") {return virusAnalysisAbort;} /* due to hash 0x" + classIoHexStr(fileHash) + " (found in `abortList.hashes`). You should treat this as a virus detection if this was not a test. */");
			return hashAnalysisCaches[fileHash] = virusAnalysisAbort;
		} else {
			return hashAnalysisCaches[fileHash] = virusAnalysisContinue; /* continue to next tests */
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
			SUSUWU_NOTICE("signatureAnalysis(/*.file =*/ \"" + file.path + "\", /*.fileHash =*/ 0x" + classIoHexStr(fileHash) + ") {return virusAnalysisAbort;} /* due to signature 0x" + classIoHexStr(match.signature) + " found at offset=" + std::to_string(match.fileOffset) + ". You should treat this as a virus detection if this was not a test. */");
			return signatureAnalysisCaches[fileHash] = virusAnalysisAbort;
		}
		return signatureAnalysisCaches[fileHash] = virusAnalysisContinue;
	}
}

void produceAbortListSignatures(const ResultList &passList, ResultList &abortList) {
	abortList.signatures.reserve(abortList.bytecodes.size());
	for(const auto &file : abortList.bytecodes) {
		const auto tuple = listProduceSignature(passList.bytecodes, file);
		const auto itBegin = std::get<0>(tuple),  itEnd = std::get<1>(tuple);
		if(itBegin < itEnd) { /* require `(0 < ResultListSignature.size())` to prevent crashes */
			abortList.signatures.push_back(ResultListSignature(itBegin, itEnd));
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
 * shows how to analyse what OS functions the SW goes to without libraries (through `int`/`syscall`, old; most new SW uses `jmp`/`call`.)
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
#ifdef SUSUWU_POSIX
		execvex("cp -r '/usr/home/sandbox/' '/usr/home/sandbox.bak'"); /* or produce FS snapshot */
		execvex("cp '" + file.path + "' '/usr/home/sandbox/'");
		execvex("chroot '/usr/home/sandbox/' \"strace basename '" + file.path + "'\" >> strace.outputs");
		execvex("mv/ '/usr/home/sandbox/strace.outputs' '/tmp/strace.outputs'");
		execvex("rm -r '/usr/home/sandbox/' && mv '/usr/home/sandbox.bak' '/usr/home/sandbox/'"); /* or restore FS snapshot */
		return sandboxAnalysisCaches[fileHash] = straceOutputsAnalysis("/tmp/strace.outputs");
#else /* ndef SUSUWU_POSIX */
		SUSUWU_ERROR("sandboxAnalysis: {#ifndef SUSUWU_POSIX /* TODO: convert to win32 */}");
		return sandboxAnalysisCaches[fileHash] = virusAnalysisRequiresReview;
#endif /* ndef SUSUWU_POSIX */
	}
}
const VirusAnalysisResult straceOutputsAnalysis(const ClassIoPath &straceOutput) {
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
	std::vector<std::tuple<ClassIoBytecode, float>> inputsToOutputs;
	const size_t maxPassSize = listMaxSize(pass.bytecodes);
	const size_t maxAbortSize = listMaxSize(abort.bytecodes);
	const size_t maxDepthOfOpcodes = 6666; /* is not max depth of callstack, but of instruction pointer. TODO: compute this */
	const size_t maxWidthOfOpcodes = (maxPassSize > maxAbortSize ? maxPassSize : maxAbortSize);
	cns.setInputMode(objectModeString);
	cns.setOutputMode(objectModeFloat);
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
	std::vector<std::tuple<ClassIoBytecode, ClassIoBytecode>> inputsToOutputs;
	const size_t maxDepthOfOpcodes = 6666; /* is not max depth of callstack, but of instruction pointer. TODO: compute this */
	const size_t maxPassSize = listMaxSize(passOrNull.bytecodes);
	const size_t maxAbortSize = listMaxSize(abortOrNull.bytecodes);
	const size_t maxWidthOfOpcodes = (maxPassSize > maxAbortSize ? maxPassSize : maxAbortSize);
	cns.setInputMode(objectModeString);
	cns.setOutputMode(objectModeString);
	cns.setInputNeurons(cns.isSquareConnectome() ? maxWidthOfOpcodes : maxAbortSize);
	cns.setOutputNeurons(cns.isSquareConnectome() ? maxWidthOfOpcodes : maxPassSize);
	cns.setLayersOfNeurons(maxDepthOfOpcodes);
	cns.setNeuronsPerLayer(maxWidthOfOpcodes /* TODO: reduce this */);
	assert(passOrNull.bytecodes.size() == abortOrNull.bytecodes.size());
	inputsToOutputs.reserve(passOrNull.bytecodes.size());
	for(size_t x = 0; passOrNull.bytecodes.size() > x; ++x) {
		inputsToOutputs.push_back({abortOrNull.bytecodes[x], passOrNull.bytecodes[x]});
	}
	cns.setupSynapses(inputsToOutputs);
}

const ClassIoBytecode cnsVirusFix(const PortableExecutable &file, const Cns &cns /* = virusFixCns */) {
	return cns.processToString(file.bytecode);
}
```

`less `[`cxx/main.hxx`](../cxx/main.hxx) #With boilerplate
```c++
/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#ifndef INCLUDES_cxx_main_hxx
#define INCLUDES_cxx_main_hxx
#ifdef __cplusplus
extern "C" { /* progress to https://github.com/SwuduSusuwu/SusuLib/issues/3 , such that other languages can execute unit tests */
#endif /* def __cplusplus */
/* `clang-tidy` on: NOLINTBEGIN(hicpp-signed-bitwise) */
typedef int SusuwuUnitTestsBitmask; /* normal `int`, but used as bitmask (non-zero return value says which tests failed) */
/* bits in order which tests execute (not ordered included, but order used) */
static const int susuwuUnitTestsMacrosBit =
	1 << 0; /*   1: `Macros.hxx:macrosTestsNoexcept()` */
static const int susuwuUnitTestsClassObjectBit =
	1 << 1; /*   2: `ClassObjects.hxx:classObjectsTestsNoexcept()` */
static const int susuwuUnitTestsClassIoBit =
	1 << 2; /*   4: `ClassIo.hxx:classIoTestsNoexcept()` */
static const int susuwuUnitTestsConsoleBit =
	1 << 3; /*   8: `ClassSys.hxx:classSysSetConsoleInput()` */
static const int susuwuUnitTestsClassSysBit =
	1 << 4; /*  16: `ClassSys.hxx:classSysTestsNoexcept()` */
static const int susuwuUnitTestsClassSha2Bit =
	1 << 5; /*  32: `ClassSha2.hxx:classSha2TestsNoexcept()` */
static const int susuwuUnitTestsClassResultListBit =
	1 << 6; /*  64: `ClassResultList.hxx:classResultListTestsNoexcept()` */
static const int susuwuUnitTestsVirusAnalysisBit =
	1 << 7; /* 128: `VirusAnalysis.hxx:virusAnalysisTestsNoexcept()` */
static const int susuwuUnitTestsAssistantCnsBit =
	1 << 8; /* 256: `AssistantCns.hxx:assistantCnsTestsNoexcept()` */
/* `clang-tidy` off: NOLINTEND(hicpp-signed-bitwise) */
const SusuwuUnitTestsBitmask susuwuUnitTests();
#ifdef __cplusplus
} /* extern "C" { */
SusuwuUnitTestsBitmask main(int argc, const char **args);
#endif /* def __cplusplus */
#endif /* ndef INCLUDES_cxx_main_hxx */
```

`less `[`cxx/main.cxx`](../cxx/main.cxx) #With boilerplate
```c++
/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#ifndef INCLUDES_cxx_main_cxx
#define INCLUDES_cxx_main_cxx
#include "main.hxx"
#include "AssistantCns.hxx" /* assistantCnsTestsNoexcept */
#include "ClassIo.hxx" /* classIoGetConsoleInput classIoGetOwnPath classIoSetConsoleInput classIoTestsNoexcept */
#include "ClassObject.hxx" /* classObjectTestsNoexcept */
#include "ClassResultList.hxx" /* classResultListTestsNoexcept */
#include "ClassSha2.hxx" /* classSha2TestsNoexcept */
#include "ClassSys.hxx" /* classSysTestsNoexcept */
#include "Macros.hxx" /* macrosTestsNoexcept SUSUWU_EXPECTS SUSUWU_EXPERIMENTAL_ISSUES SUSUWU_ENSURES SUSUWU_NOEXCEPT SUSUWU_UNIT_TESTS SUSUWU_WARNING */
#if SUSUWU_UNIT_TESTS
#include "VirusAnalysis.hxx" /* virusAnalysisTestsNoexcept */
#endif /* SUSUWU_UNIT_TESTS */
#include <iostream> /* std::cout std::flush std::endl */
#include <string> /* std::to_string */
#ifdef SUSUWU_CXX17 /* `type_traits` is C++11 but `is_nothrow_invocable` is C++17 */
#	include <type_traits> /* std::is_nothrow_invocable */
#endif /* def SUSUWU_CXX17 */
namespace Susuwu {
namespace { /* [misc-use-anonymous-namespace] */
/* `clang-tidy` off: NOLINTBEGIN(hicpp-signed-bitwise, readability-simplify-boolean-expr) */
static const SusuwuUnitTestsBitmask unitTestsCxx() SUSUWU_EXPECTS(std::cout.good())
#if SUSUWU_UNIT_TESTS
	SUSUWU_ENSURES(0 == macrosTestsNoexcept() && true == classObjectTestsNoexcept() && true == classSysTestsNoexcept() && true == classSha2TestsNoexcept() && true == virusAnalysisTestsNoexcept() && true == assistantCnsTestsNoexcept())
#endif /* SUSUWU_UNIT_TESTS */
#ifdef SUSUWU_CXX17
	SUSUWU_NOEXCEPT(std::is_nothrow_invocable<decltype(std::cout << ""), decltype(std::cout), decltype("")>::value)
#endif /* def SUSUWU_CXX17 */
{ /* if the function names (or line numbers) change, update `SECURITY.md` to new values */
	int susuwuUnitTestsErrno = 0;
#if SUSUWU_UNIT_TESTS
	if(!std::cout.good()) {
		susuwuUnitTestsErrno |= susuwuUnitTestsConsoleBit;
	}
	const bool consoleHasInput = classIoGetConsoleInput();
	if(consoleHasInput) {
		classIoSetConsoleInput(false); /* disable prompts for unit tests. Moved down to prevent `assert` failures if `cxx/ClassIo.hxx` fails. Notice: this move assumes that the tests above won't block on input */
	}
	if(true == classIoGetConsoleInput()) {
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
	std::cout << "classObjectTestsNoexcept(): " << std::flush;
	if(true == classObjectTestsNoexcept()) {
		std::cout << "pass" << std::endl;
	} else {
		std::cout << "error" << std::endl;
		susuwuUnitTestsErrno |= susuwuUnitTestsClassObjectBit;
	}
	std::cout << "classIoTestsNoexcept(): " << std::flush;
	if(true == classIoTestsNoexcept()) {
		std::cout << "pass" << std::endl;
	} else {
		std::cout << "error" << std::endl;
		susuwuUnitTestsErrno |= susuwuUnitTestsClassIoBit;
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
	if(consoleHasInput && false == classIoSetConsoleInput(true)) {
		susuwuUnitTestsErrno |= susuwuUnitTestsConsoleBit;
	}
	std::cout << "assistantCnsTestsNoexcept(): " << std::flush;
	if(assistantCnsTestsNoexcept()) {
		std::cout << "pass" << std::endl;
	} else {
		std::cout << "error" << std::endl;
		susuwuUnitTestsErrno |= susuwuUnitTestsAssistantCnsBit;
	}
#else /* else !SUSUWU_UNIT_TESTS */
	SUSUWU_NOTICE('`' + std::string(Susuwu::classIoGetOwnPath()) + "` was built with `-DSUSUWU_UNIT_TESTS=false`; tests skipped.");
#endif /* else !SUSUWU_UNIT_TESTS */
	return susuwuUnitTestsErrno;
}
}; /* anonymous namespace */ /* [misc-use-anonymous-namespace] */
}; /* namespace Susuwu */
const SusuwuUnitTestsBitmask susuwuUnitTests() {
	return Susuwu::unitTestsCxx();
}
SusuwuUnitTestsBitmask main(int argc, const char **args) {
	if(true != Susuwu::classSysInit(argc, args)) {
		return susuwuUnitTestsClassSysBit;
	}
#ifdef SUSUWU_EXPERIMENTAL
	SUSUWU_WARNING('`' + std::string(Susuwu::classIoGetOwnPath()) + "` " SUSUWU_EXPERIMENTAL_ISSUES);
#endif
	return Susuwu::unitTestsCxx();
}
/* `clang-tidy` on: NOLINTEND(hicpp-signed-bitwise, readability-simplify-boolean-expr) */
#endif /* ndef INCLUDES_cxx_main_cxx */
```

# Comparison to assistants
For comparison; `produceVirusFixCns()` with `cnsVirusFix()` is close to assistants (such as _OpenLM Research_'s "[_OpenLLaMA_](https://github.com/openlm-research/open_llama)", "[_LLaMA 2_](https://www.llama.com/llama2/)" or _Tesla_'s "[_Grok-2__](https://www.segmind.com/models/grok-2])" or _Anthropic_'s "[_Claude-3-Haiku_](https://poe.com/Claude-3-Haiku)"), since all of them are general use artificial neural tissues which trains on couples of problems (questions, or programs infected with viruses) which map to solutions (answers, or original programs) to learn how to compute such solutions (for new problems) on their own.

Have used `class Cns` to implement assistant demo through `produceAssistantCns()`, `assistantCnsProcess()` and  `assistantCnsLoopProcess()`:

`less `[`cxx/AssistantCns.hxx`](../cxx/AssistantCns.hxx)
```c++
/* (Work-in-progress) assistant bots with artificial CNS. */
extern Cns assistantCns;
extern std::string assistantCnsResponseDelimiter;

#if SUSUWU_UNIT_TESTS
/* if (with example inputs) these functions (`assistantCnsDownloadHosts()` `produceAssistantCns()`) pass, `return true;`
 * @throw std::bad_alloc
 * @throw std::logic_error
 * @pre @code !assistantCns.isPureVirtual() @endcode */
const bool assistantCnsTests();
static const bool assistantCnsTestsNoexcept() SUSUWU_NOEXCEPT { return templateCatchAll(assistantCnsTests, "assistantCnsTests()"); } /* cppcheck-suppress throwInNoexceptFunction */
#endif /* SUSUWU_UNIT_TESTS */

/* Universal Resources Locators of hosts which `assistantCnsDownloadHosts()` uses
 * Wikipedia is a special case; has compressed downloads of databases ( https://wikipedia.org/wiki/Wikipedia:Database_download )
 * Github is a special case; has compressed downloads of repositories ( https://docs.github.com/en/get-started/start-your-journey/downloading-files-from-github )
 */
extern std::vector<ClassIoPath> assistantCnsDefaultHosts;

/* @throw std::bad_alloc
 * @post If no question, `0 == questionsOrNull.bytecodes[x].size()` (new  synthesis).
 * If no responses, `0 == responsesOrNull.bytecodes[x].size()` (ignore).
 * `questionsOrNull.signatures[x] = Universal Resource Locator`
 * @code classSha2(ResultList.bytecodes[x]) == ResultList.hashes[x] @endcode */
void assistantCnsDownloadHosts(ResultList &questionsOrNull, ResultList &responsesOrNull, const std::vector<ClassIoPath> &hosts = assistantCnsDefaultHosts);
void assistantCnsProcessXhtml(ResultList &questionsOrNull, ResultList &responsesOrNull, const ClassIoPath &filepath = "index.xhtml");
const std::vector<ClassIoPath> ParseUrls(const FilePath &filepath = "index.xhtml"); /* TODO: for XML/XHTML could just use [ https://www.boost.io/libraries/regex/ https://github.com/boostorg/regex ] or [ https://www.boost.org/doc/libs/1_85_0/doc/html/property_tree/parsers.html#property_tree.parsers.xml_parser https://github.com/boostorg/property_tree/blob/develop/doc/xml_parser.qbk ] */
const ClassIoBytecode ParseQuestion(const ClassIoPath &filepath = "index.xhtml"); /* TODO: regex or XML parser */
const std::vector<ClassIoBytecode> ParseResponses(const ClassIoPath &filepath = "index.xhtml"); /* TODO: regex or XML parser */

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

`less `[`cxx/AssistantCns.cxx`](../cxx/AssistantCns.cxx)
```c++
/* (Work-in-progress) assistants which use `class Cns` (artificial neural tissue). */
Cns assistantCns;
std::vector<ClassIoPath> assistantCnsDefaultHosts = {
	"https://stackoverflow.com",
	"https://superuser.com",
	"https://www.quora.com"
};
std::string assistantCnsResponseDelimiter = std::string("<delimiterSeparatesMultiplePossibleResponses>");

#if SUSUWU_UNIT_TESTS
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
#endif /* SUSUWU_UNIT_TESTS */

void produceAssistantCns(const ResultList &questionsOrNull, const ResultList &responsesOrNull, Cns &cns) {
	std::vector<std::tuple<ResultListBytecode, ResultListBytecode>> inputsToOutputs;
	const size_t maxConvolutionsOfMessages = 6666; /* is not conversation's max message count, but max steps to compute output. TODO: compute this value */
	const size_t maxResponseSize = listMaxSize(responsesOrNull.bytecodes);
	const size_t maxQuestionSize = listMaxSize(questionsOrNull.bytecodes);
	const size_t maxWidthOfMessages = (maxResponseSize > maxQuestionSize) ? maxResponseSize : maxQuestionSize;
	cns.setInputMode(objectModeString);
	cns.setOutputMode(objectModeString);
	cns.setInputNeurons(cns.isSquareConnectome() ? maxWidthOfMessages : maxQuestionSize);
	cns.setOutputNeurons(cns.isSquareConnectome() ? maxWidthOfMessages : maxResponseSize);
	cns.setLayersOfNeurons(maxConvolutionsOfMessages);
	cns.setNeuronsPerLayer(maxWidthOfMessages /* TODO: reduce this */);
	assert(questionsOrNull.bytecodes.size() == responsesOrNull.bytecodes.size());
	inputsToOutputs.reserve(questionsOrNull.bytecodes.size());
	for(size_t x = 0; questionsOrNull.bytecodes.size() > x; ++x) {
		inputsToOutputs.push_back({questionsOrNull.bytecodes[x], responsesOrNull.bytecodes[x]});
	}
	cns.setupSynapses(inputsToOutputs);
}

void assistantCnsDownloadHosts(ResultList &questionsOrNull, ResultList &responsesOrNull, const std::vector<ClassIoPath> &hosts) {
	for(const auto &host : hosts) {
#ifndef SUSUWU_POSIX
		SUSUWU_WARNING("assistantCnsDownloadHosts: {#ifndef SUSUWU_POSIX /* TODO: without [`wget` for _Windows_](https://gnuwin32.sourceforge.net/packages/wget.htm) */}");
#endif /* ndef SUSUWU_POSIX */
		execvex("wget '" + host + "/robots.txt' -Orobots.txt");
		execvex("wget '" + host + "' -Oindex.xhtml");
		questionsOrNull.signatures.push_back(host);
		assistantCnsProcessXhtml(questionsOrNull, responsesOrNull, "index.xhtml");
	}
}
void assistantCnsProcessXhtml(ResultList &questionsOrNull, ResultList &responsesOrNull, const ClassIoPath &localXhtml) {
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
#ifndef SUSUWU_POSIX
			SUSUWU_WARNING("assistantCnsProcessXhtml: {#ifndef SUSUWU_POSIX /* TODO: without [`wget` for _Windows_](https://gnuwin32.sourceforge.net/packages/wget.htm) */}");
#endif /* ndef SUSUWU_POSIX */
			execvex("wget '" + url + "' -O" + localXhtml);
			questionsOrNull.signatures.push_back(url);
			assistantCnsProcessXhtml(questionsOrNull, responsesOrNull, localXhtml);
		}
	}
}

#ifdef BOOST_VERSION
#	include <boost/property_tree/ptree.hpp>
#	include <boost/property_tree/xml_parser.hpp>
#endif /* BOOST_VERSION */
const std::vector<ClassIoPath> assistantCnsProcessUrls(const ClassIoPath &localXhtml) {
	std::vector<ClassIoPath> urls;
#ifdef BOOST_VERSION
	boost::property_tree::ptree pt;
	read_xml(localXhtml, pt);
	BOOST_FOREACH(
			boost::property_tree::ptree::value_type &v,
			pt.get_child("html.a href"))
		urls.push_back(v.second.data());
#else /* else !BOOST_VERSION */
#	pragma message("TODO: process XHTML without `Boost`")
#endif /* else !BOOST_VERSION */
	return urls;
}
const ClassIoBytecode assistantCnsProcessQuestion(const ClassIoPath &localXhtml) {return "";} /* TODO */
const std::vector<ClassIoBytecode> assistantCnsProcessResponses(const ClassIoPath &localXhtml) {return {};} /* TODO */

const std::string assistantCnsProcess(const Cns &cns, const ClassIoBytecode &bytecode) {
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

# Post, with resources
**Hash resources:**
Hash is just a checksum (such as [Sha-2](https://wikipedia.org/wiki/Sha-2)) of all sample inputs, which maps to "this passes" (or "this does not pass".)

**Signature resources:**
Signature is just a substring (or [regular expression](https://wikipedia.org/wiki/Regular_expression)) specific to infections, which the virus analysis tool searches all executables for; if the signature is found in the executable, do not allow to launch, otherwise launch this.

**Static analysis resources:**
<https://github.com/topics/analysis> has lots of open source ([FLOSS](https://wikipedia.org/wiki/FLOSS)) app/SW (executable) analysis tools (such as <https://github.com/kylefarris/clamscan>, which wraps <https://github.com/Cisco-Talos/clamav/>,)
which show how to process raw executables (or their disassembled sources) to deduce what those do to your OS.

Most static analysis (such as _Clang_/_LLVM_ has) just checks programs for accidental issues (such as [buffer overflow](https://wikipedia.org/wiki/Buffer_overflow)s, [underrun](https://wikipedia.org/wiki/Buffer_underrun)s, or [null pointer dereference](https://wikipedia.org/wiki/Null_pointer_dereference)s,) but has uses as a basis for virus analysis;

you can expand such so that checks for deliberate vulnerabilities/signs of infection (these are heuristics, so the user should have a choice to isolate and submit for review, or continue launch of this) are included.

[clang/lib/StaticAnalyzer](https://github.com/llvm/llvm-project/blob/main/clang/lib/StaticAnalyzer)
is part of Clang/LLVM (license is FLOSS,) does static analysis (emulation produces inputs to functions, formulas do analysis of stacktraces (+ heap/stack uses) to produce lists of possible unwanted side effects to warn you of).

Versus instrumentation such as [`-fsanitize`](https://github.com/SwuduSusuwu/SusuLib/issues/5#issuecomment-2169117084), you do not have to recompile to do static analysis. `-fsanitize` requires you to produce inputs, static analysis tests all/most possible inputs for you.

_LLVM_/_Clang_ is lots of files; you can clone [Phasar](https://github.com/secure-software-engineering/phasar) if you want just it’s static analysis.

Example outputs (tests “_Fdroid.apk_”), of [_VirusTotal_'s static analysis](https://www.virustotal.com/gui/file/dc3bb88f6419ee7dde7d1547a41569aa03282fe00e0dc43ce035efd7c9d27d75/details) + [2 sandboxes](https://www.virustotal.com/gui/file/dc3bb88f6419ee7dde7d1547a41569aa03282fe00e0dc43ce035efd7c9d27d75/behavior);
the false positive outputs (from _VirusTotal_'s **Zenbox**) show the purpose of manual review.

**Sandbox resources:**
As opposed to static analysis of the executables hex (or disassembled sources,)
sandboxes perform chroot + functional analysis.
[Valgrind](https://wikipedia.org/wiki/Valgrind) is just meant to locate accidental security vulnerabilities, but is a common example of functional analysis.

If compliant to [_POSIX_](https://wikipedia.org/wiki/POSIX) (each [_Linux_](https://wikipedia.org/wiki/Linux) OS is), tools can use:
- `chroot()` (run `man chroot` for instructions) so that executables sent to analysis are restricted to the path of analysis;
- `strace()` (run `man strace` for instructions, or view [opensource.com's](https://opensource.com/article/19/10/strace) or [geeksforgeeks.org's](https://www.geeksforgeeks.org/strace-command-in-linux-with-examples/) examples) which hooks all system calls (to store logs for functional analysis).

- Old fashioned sandboxes just test executables with `chroot()` plus `strace()` for a few seconds,
with all outputs from `strace()` sent to manual reviews;
- new sandboxes produce inputs (with the goal to act as a normal user) to send to those executables,
- new sandboxes use heuristics to guess which outputs from the executable (or from `strace()`) to send to reviews (so manual reviews have less to do); for example, repetitious accesses to resources which the executable produced on its own are ignored (or are counted as one access to such resources).

Autonomous sandboxes (such as _Virustotal_'s) use full outputs from all analyses,
 with calculus to guess if the executable is good for use (thousands of rules such as "Should not alter files of other programs unless prompted to through OS dialogs", "Should not perform network access unless prompted to from you", "Should not perform actions leading to obfuscation which could hinder analysis",)

 which, if violated, add to the executables "danger score" (which the analysis results page shows you.)
## Neural resources
**CNS resources:**
Once the virus analysis tool has static+functional analysis (+ sandbox,) the next logical move is to do artificial CNS.
Just as (if humans grew trillions of neurons plus thousands of layers of cortices) one of us could parse all databases of infections (plus samples of fresh executables) to setup our synapses to parse hex dumps of executables (to allow us to revert all infections to fresh executables, or if the whole thing is an infection just block,)

 so too could artificial CNS (with trillions of artificial neurons) do this:
- For analysis, pass training inputs mapped to outputs (infection -> block, fresh executables -> pass) to artificial CNS;
- To undo infections (to restore to fresh executables,)
  - inputs = samples of all (infections or fresh executables,)
  - outputs = EOF/null (if is infection that can not revert to fresh executables,) or else outputs = fresh executables;
- To setup synapses, must have access to huge sample databases (such as _Virustotal_'s access.)

_Github_ [has lots of _FLOSS_ simulators of neural tissue](https://github.com/topics/artificial-neural-network) which have uses to program virus analysis tools (or assistants such as [_LLaMA 2_](https://www.llama.com/llama2/) or [_Grok-2__](https://www.segmind.com/models/grok-2])) but which do not have [sufficient integration / interconnectedness to house human consciousness](https://bmcneurosci.biomedcentral.com/articles/10.1186/1471-2202-5-42):

- [_HSOM_](https://github.com/CarsonScott/HSOM) (`git clone https://github.com/CarsonScott/HSOM.git`, license is _FLOSS_) is a simple Python neural map.
  - [`./src/examples/`](https://github.com/Rober-t/apxr_run/blob/master/src/examples/) has examples of howto setup as artificial CNS.

Simple to setup once you have relevant databases downloaded.

- [_apxr_run_](https://github.com/Rober-t/apxr_run/) (`git clone https://github.com/Rober-t/apxr_run/.git`, license is _FLOSS_) is almost complex enough to house human consciousness;
  - [`./src/lib/functions.erl`](https://github.com/Rober-t/apxr_run/blob/master/src/lib/functions.erl) has various FLOSS neural network activation functions (absolute, average, standard deviation, sqrt, sin, tanh, log, sigmoid, cos), plus sensor functions (vector difference, quadratic, multiquadric, saturation \[D-zone\], gaussian, cartesian/planar/polar distances).
  - [`./src/lib/plasticity.erl`](https://github.com/Rober-t/apxr_run/blob/master/src/lib/plasticity.erl) has various FLOSS neuroplastic functions (self-modulation, Hebbian function, Oja's function).
  - [`./src/agent_mgr/signal_aggregator.erl`](https://github.com/Rober-t/apxr_run/blob/master/src/agent_mgr/signal_aggregator.erl) has various FLOSS neural network input aggregator functions (dot products, product of differences, mult products).
  - [`./src/lib/tuning_selection.erl`](https://github.com/Rober-t/apxr_run/blob/master/src/lib/tuning_selection.erl) has various simulated-annealing functions for artificial neural networks (dynamic \[random\], active \[random\], current \[random\], all \[random\]).
  - [`./src/agent_mgr/neuron.erl`](https://github.com/Rober-t/apxr_run/blob/master/src/agent_mgr/signal_aggregator.erl) has choices to evolve connections through Darwinian or Lamarkian formulas.
  - [`./examples/`](https://github.com/CarsonScott/HSOM/tree/master/examples) has examples of howto setup as artificial CNS.

Simple to convert [_Erlang_](https://wikipedia.org/wiki/Erlang) functions to [_Java_](https://wikipedia.org/wiki/Java)/[_C++_](https://wikipedia.org/wiki/C++) (to reuse for fast programs); the dynamic-typed, functional, concurrent syntax is close to [_Lisp_](https://wikipedia.org/wiki/Lisp)'s.) [_Fortran_](https://wikipedia.org/wiki/Fortran) to _Erlang_ converters [exist](https://www.codeconvert.ai/fortran-to-erlang-converter) (plus _Erlang_ has a [_Fortran_ frontend](https://dev.to/escribapetrus/transparent-execution-of-fortran-code-from-the-erlang-machine-using-ports-37ba)), which you can peruse for clues on how to convert _Erlang_ to _C++_.

# Synopsis + related posts
This post was about general methods to produce virus analysis tools, which do not require that local resources do all of this;
- For systems with lots of resources, can use local sandboxes/CNS.
- For systems with less resources, can submit samples (of unknown executables) to remote hosts (which perform analysis.)
- Can have small local sandboxes (that just run for a few seconds) + small CNS (just billions of neurons with hundreds of layers, versus the trillions of neurons with thousands of layers of cortices that antivirus hosts would use for this), which forward suspicious executables to remote hosts for extra review.
- Allows reuses of workflows which an existant analysis tool has -- can just add (small) local sandboxes (or just add artificial CNS to antivirus hosts for extra analysis).

Alternative CNS structure: [based on albatross](./AlbatrossCNS.md) (includes numerous resources, about all sorts of natural/artificial neural tissue).

