Dual licenses: choose `Creative Commons` or `Apache 2` (allows all uses).

Purposes: `/posts/` stages posts (school classes) for https://SwuduSusuwu.substack.com/

Purposes: `/cxx/` C++ implementations ( based off of ideas/pseudocodes from https://SwuduSusuwu.substack.com/ , most of which is about autonomous tools, or artificial central nervous systems)

Targets: Windows/Linux/Android/OSX/iOS; all C++ compilers, requires some extensions (such as `for(auto x : s) {}`)

Usage: `./build.sh` produces `*.o` static libraries (for distribution to others,) plus `a.out` to do unit tests (test harnesses). Allowed flags; `--debug` (default; includes frame-pointers/debug symbols (`-g`), includes `valgrind`-replacement tools (such as `-fsanitize=address`), `--release` (excludes `--debug`, strips frame-pointers/symbols, optimizes with `-O2`), `--mingw` (if on _Linux_, can use with `--release` or `--debug`, produces _Portable Executable_ for _Windows_. If on _Windows_, the default is to produce _Portable Executable_'s for _Windows_)
    Optional flags (`vim build.sh` to use):
        `-DSUSUWU_SH_PREFER_STDIO` to replace `std::cXXX << ...` with `fprintf(stdXXX, ...)`, default is `!defined(__cplusplus)`.
        `-DSUSUWU_SH_VERBOSE` to print diagnostic messages (`SUSUWU_SH_USE_FILE`, `SUSUEU_SH_USE_LINE`, `SUSUWU_NOTICE`, `SUSUWU_DEBUG`, `SUSUWU_DEBUGEXECUTE`, ?`SUSUWU_NOTICE_EXECUTE`, ?`SUSUWU_DEBUG_EXECUTE`: was all `#if !defined(NDEBUG)`, now is `#if SUSUWU_SH_VERBOSE`), default is `!defined(NDEBUG)`.
        `-DSUSUWU_SH_SKIP_BRACKETS = true` sets output format to `WARN_LEVEL: message`, default is `false`.
        `-DSUSUWU_SH_FILE = true` sets output format to `[__FILE__: WARN_LEVEL: message]`, default is `SUSUWU_SH_VERBOSE`.
        `-DSUSUWU_SH_LINE = true` sets output format to `[__LINE__: WARN_LEVEL: message]`, default is `SUSUWU_SH_VERBOSE`.
        `-DSUSUWU_SH_FUNC = true` sets output format to `[__func__: WARN_LEVEL: message]`, default is `false`.
        `-DSUSUWU_SH_SKIP_COLORS = true` to omit _VT100_ (_ANSI_) colors, default is `defined(SUSUWU_SH_COLORS_UNSUPPORTED)`).
        `-DSUSUWU_SH_SKIP_COLORS = false` to force (even if unsupported) _VT100_ (_ANSI_) color use.
    Flags which https://github.com/SwuduSusuwu/SubStack/issues/17 will introduce (TODO):
        `-DSUSUWU_SH_RUNTIME_OSC` to replace `#ifdef _POSIX_VERSION\nAccessClipboard();\n#endif` with `termcmp`/`GetConsoleMode()` (for choices on whether or not to use Operating System Commands,) default is undefined.
        `-DSUSUWU_SH_RUNTIME_COLORS` to replace `#if _POSIX_VERSION\nColors();\n#endif` `termcmp`/`GetConsoleMode()` (for choices on whether or not to use colors,) default is undefined.
        To match `g++`/`clang++` output rules, use `-DSUSUWU_SKIP_BRACKETS = true, -DSUSUWU_SH_FILE = true, -DSUSUWU_SH_LINE = true, -DSUSUWU_SH_FUNC = false, -DSUSUWU_SKIP_COLORS = false` (sets output format to `__FILE__:__LINE__: WARN_LEVEL: message`).

Linter: `clang-tidy cxx/*.cxx` /* uses `.clang_tidy` options */

Conventions = Mozilla Org (ergo Firefox) style:

    Files: `#import "PascalCase.hxx"`

    Structs, enums, classe: `typedef struct PascalCase {} PascalCase;`, `typedef enum PascalCase {} PascalCase;`, `typedef class PascalCase {} PascalCase;`

    Macros: `#define NAMESPACE_CONSTANT_CASE(snake_case_param) assert(snake_case_param);`

    Indentation = tabs ('^I'); as much tabs as braces ('{', '}').

    Braces, functions:
```
        const /* const prevents `if(func() = x)` */ bool classPrefixCamelCase(bool s, bool x) {

            if(s && x) {

        return s;

            } else {

            return x;

            }

        }
```
    Local variables, objects: `const bool camelCase = true`; Global variables/objects: `extern const bool classPrefixCamelCase;`
        Functions/globals can omit "classPrefix" if the file has `namespace` used to avoid collisions, or has `class` used to mask member `typedef`s/functions.
        [The project as a whole should have `namespace`, but you can nest `namespace`s.]

    `git commit` message format/syntax: `git add NewFile` = "+`NewFile`", `git rm Exists` = "-`Exists`", `touch Exists && git add Exists` = "@`Exists`" or "?`Exists`" = `echo "int newFunction() >> Exists && git add Exists` = "@`Exists`:+`NewFunction`. Do not include the quote marks (""). Commit titles can omit backticks if not enough room.

    Simple wildcards/regex for multiple files: "OldPath/.* -> NewPath/.*" or `%s/oldFunction/newFunction/`

    Include guards:

        #ifndef INCLUDES_Path_To_File

        #define INCLUDES_Path_To_File

        #endif /* ndef INCLUDES_Path_To_File */

Conventions +=
    [Indent multi-level macros as `#if X # if S ,,, # endif #endif`](https://stackoverflow.com/questions/1854550/c-macro-define-indentation)
    [\_DEBUG is specific to MSVC, thus use NDEBUG](https://stackoverflow.com/questions/2290509/debug-vs-ndebug), [Pass `-D NDEBUG` to disable asssets + enable optimizations](https://stackoverflow.com/questions/2249282/c-c-portable-way-to-detect-debug-release)
    Do not perform tasks within `assert()`, due to: the standard says "[`#if NDEBUG\n#define assert(x) (0)\n#endif`]".
    All userland errors should go to `throw std::exception()` _or derivatives of std::exception_, `std::cerr`, `extern int errno;`, or `return errno;`. Comments about possible errors should go above function declarations (Doxygen convention).
        `throw` / `std:cerr` should use the new common syntax for this: `"[WARN_LEVEL: OPTIONAL_FUNCTION_NAME {code which triggered the error/warning/diagnostic/notice} /* OPTIONAL COMMENTS */]"`, 
        cxx/Macros.hxx:{SUSUWU_STR(x), SUSUWU_CERR(x), SUSUWU_STDOUT(x)} have the new syntax for this
    [Doxygen-ish "@pre"/"@post" prepares for C++26 Contracts](https://github.com/doxygen/doxygen/issues/6702): 
```
        /* @throw std::bad_alloc If function uses {malloc, realloc, new[], std::*::{push_back, push_front, insert}}
         * @throw std::logic_error Optional. Would include most functions which use std::*
         * @pre @code !output.full() @endcode
         * @post @code !output.empty() @endcode
         */
        bool functionDeclaration(std::string input, std::deque<vector> output);
```
        It is arguable whether or not you should document such possible system errors; almost all Standard Template Library functions can throw derivatives of `std::logic_error`.
        Regex `:%s/@pre (.*) @code (.*) @endcode/[[expects: \2]] \\* \1 \\*/` `:%s/@post (.*) @code (.*) @endcode/[[ensures: \2]] \\* \1 \\*/` once have Contracts/C++26
        cxx/Macros.cxx has `ASSUME(X)`, which is close to `[[expects: x]]`, but `ASSUME(X)` goes to `*.cxx`, whereas `[[expects]]` goes to `*.hxx`.
        Documentation of interfaces belongs to `*.hxx`; `*.cxx` is to do implementations. Do not duplicate interface comments.
        Advantages of `[[expects]]`; allows to move information of interfaces out of `*.cxx`, to `*.hxx`.

