Dual licenses: choose [_Creative Commons_](https://creativecommons.org/share-your-work/use-remix/) or _[Apache 2](https://github.com/SwuduSusuwu/SubStack/blob/trunk/LICENSE)_ (allows all uses).

# Table of Contents
- [Purposes](#Purposes)
- [How to use this](#How-to-use-this)
- [Contributor conventions/rules](#Contributor-conventionsrules)
  - [Git](#Git)
  - [Source](#Source)
  - [Good first issues](https://github.com/SwuduSusuwu/SubStack/contribute)
  - [Secure](https://github.com/SwuduSusuwu/SubStack/blob/trunk/SECURITY.md)

# Purposes
[`./posts/`](https://github.com/SwuduSusuwu/SubStack/blob/trunk/posts/) stages posts (school classes) for https://SwuduSusuwu.SubStack.com/ about artificial neural tissue, antivirus, assistants, plus autonomous tools.

[`./c/`](https://github.com/SwuduSusuwu/SubStack/blob/trunk/c/) _C_ implementations of posts (TODO, has [issue](https://github.com/SwuduSusuwu/SubStack/issues/3) which you can contribute to, or can request that more resources go to this task), + vendored code (for now just _RFC6234_, for `sha2`).

[`./cxx/`](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/) _C++_ implementations of posts (for now is just neural system pure virtual template ([`./cxx/ClassCns.hxx`](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/ClassCns.hxx)) + antivirus([`./cxx/VirusAnalysis.cxx`](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/VirusAnalysis.cxx)) + assistant([`./cxx/AssistantCns.cxx`](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/AssistantCns.cxx)), with lots of [issues](https://github.com/SwuduSusuwu/SubStack/issues) which you can contribute to, or can request that more resources go to).

# How to use this
Targets: Windows/Linux/Android/OSX/iOS; all compilers with `201102 <= __cplusplus` (minimum [_C++11_](https://gcc.gnu.org/projects/cxx-status.html#cxx11), due to use of `auto`, `decltype`, `for(x: list) {}`nullptr`, `__func__`). [TODO: It is possible that `[modernize-use-default-member-init]` fix (commit 579a9089248a018005f36522968026ee464fc061) bumped this up to [_C++14_](https://gcc.gnu.org/projects/cxx-status.html#cxx14) (if so will undo, as was told that not all microcontrollers have compilers which support this).]

Usage: [`./build.sh`](https://github.com/SwuduSusuwu/SubStack/blob/trunk/build.sh) produces `*.o` static libraries (for distribution to others,) plus `a.out` to do unit tests (test harnesses).

- Allowed flags; `./build.sh --debug` (default; includes frame-pointers/debug symbols (`-g`), includes `valgrind`-replacement tools (such as `-fsanitize=address`), `./build.sh --release` (excludes `--debug`, strips frame-pointers/symbols, optimizes with `-O2`), `./build.sh --mingw` (if on _Linux_, can use with `--release` or `--debug`, produces _Portable Executable_ for _Windows_. If on _Windows_, the default is to produce _Portable Executable_'s for _Windows_)
- Extra flags (`vim build.sh` to use); other than `_PREFER_`/`_SKIP_`, most use more resources if set to `true`:
  - Custom `sh` (console) output:
    - `-DSUSUWU_SH_PREFER_STDIO` to replace `std::cXXX << ...` with `fprintf(stdXXX, ...)`; default is `!defined(__cplusplus)`.
    - `-DSUSUWU_SH_VERBOSE` to print diagnostic messages (`SUSUWU_SH_USE_FILE`, `SUSUEU_SH_USE_LINE`, `SUSUWU_NOTICE`, `SUSUWU_DEBUG`, `SUSUWU_DEBUGEXECUTE`, `SUSUWU_NOTICE_EXECUTE`, `SUSUWU_DEBUG_EXECUTE` all use `#if SUSUWU_SH_VERBOSE`); default is `!defined(NDEBUG)`.
    - `-DSUSUWU_SH_SKIP_BRACKETS = true` sets output format to `WARN_LEVEL: message`; default is `false`.
    - `-DSUSUWU_SH_FILE = true` sets output format to `[__FILE__: WARN_LEVEL: message]`; default is `SUSUWU_SH_VERBOSE`.
    - `-DSUSUWU_SH_LINE = true` sets output format to `[__LINE__: WARN_LEVEL: message]`; default is `SUSUWU_SH_VERBOSE`.
    - `-DSUSUWU_SH_FUNC = true` sets output format to `[__func__: WARN_LEVEL: message]`; default is `false`.
    - `-DSUSUWU_SH_SKIP_COLORS = true` to omit _VT100_ (_ANSI_) colors; default is `defined(SUSUWU_SH_COLORS_UNSUPPORTED)`).
    - `-DSUSUWU_SH_SKIP_COLORS = false` to force (even if unsupported) _VT100_ (_ANSI_) color use.
    - Flags which https://github.com/SwuduSusuwu/SubStack/issues/17 will introduce (TODO):
      - `-DSUSUWU_SH_RUNTIME_OSC` to replace `#ifdef _POSIX_VERSION\nAccessClipboard();\n#endif` with `termcmp`/`GetConsoleMode()` (for choices on whether or not to use Operating System Commands); default is undefined.
      - `-DSUSUWU_SH_RUNTIME_COLORS` to replace `#if _POSIX_VERSION\nColors();\n#endif` with `termcmp`/`GetConsoleMode()` (for choices on whether or not to use colors); default is undefined.
  - To match `g++`/`clang++` console format, use `-DSUSUWU_SKIP_BRACKETS = true, -DSUSUWU_SH_FILE = true, -DSUSUWU_SH_LINE = true, -DSUSUWU_SH_FUNC = false, -DSUSUWU_SKIP_COLORS = false` (sets output format to `__FILE__:__LINE__: WARN_LEVEL: message`).
  - TODO:
    - `-DSUSUWU_PREFER_CSTR` to replace `std::string` with `char *` (more compatible with non-C++ projects); default is `false`.
    - `-DSUSUWU_PREFER_C` sets SUSUWU_PREFER_CSTR ([plus other flags which will exist to allow non-C++ projects to include this](https://github.com/SwuduSusuwu/SubStack/issues/3); default is `false`.

# Contributor conventions/rules
Linter: `clang-tidy cxx/*.cxx` /* uses [`.clang-tidy`](https://github.com/SwuduSusuwu/SubStack/blob/trunk/.clang-tidy) options */ 
## Git
Do atomic commits: Commits should allow to use `git rebase -i` to reorder, unless commit messages include such as "Is followup to \<commit hash\>" (which shows temporal order).

`git commit` message format/syntax:
- if commit does `git add NewFile`: message has `+\`NewFile\``
- if `git rm Exists`: `-`Exists``
- if `touch Exists && git add Exists`: `@\`Exists\`` or `?\`Exists\``
  - Simple wildcards/regex for altered functions: `\`%s/oldFunction/newFunction/\``
- if `echo "int newFunction() {...}" >> Exists && git add Exists`: `@\`Exists\`:+\`NewFunction()\``.
- if `git mv OldPath/ NewPath/`: `\`OldPath/.* -> NewPath/.*\``

[Notice: Commit titles can omit backticks (``) if not enough room; the backticks just allow _GitHub_ to format code/paths.]
## Source
Most of what [_Mozilla Org_'s (_Firefox_'s) style](https://firefox-source-docs.mozilla.org/code-quality/coding-style/coding_style_cpp.html) suggests is sound (you should follow this unless you have specific reasons not to).
Code rules (lots overlap with _Mozilla Org_'s):

- Files: `#import "PascalCase.hxx"`, as this is most common.

- Structs, enums, classe: `typedef struct PascalCase {} PascalCase;`, `typedef enum PascalCase {} PascalCase;`, `typedef class PascalCase {} PascalCase;`, as this is most common.

- Macros: `#define NAMESPACE_CONSTANT_CASE(snake_case_param) assert(snake_case_param);`, as this is most common.

- Indent: tabs ('^I', reduced memory use, allows local configs to set width, allows arrow keys to move fast); as much tabs as braces ('{', '}'). [All which conflicts with _Mozilla Org_'s format is tab use]

- Braces, functions; most common form:
```
const /* const prevents `if(func() = x)` where you wished for `if(func() == x)` */ bool classPrefixCamelCase(bool s, bool x) {
    if(s && x) {
        return s;
    } else {
        return x;
    }
}
```
- Local variables, objects: `const bool camelCase = true`; Global variables/objects: `extern const bool classPrefixCamelCase;`, as this is most common.
  - Functions/globals can omit "classPrefix" if the file has `namespace` used to avoid collisions, or has `class` used to mask member `typedef`s/functions.
  - [The project as a whole should have `namespace`, but you can nest `namespace`s.]

- Include guards:
```
#ifndef INCLUDES_Path_To_File
#define INCLUDES_Path_To_File
#endif /* ndef INCLUDES_Path_To_File */
```
- [Indent multi-level macros as `#if X # if S ,,, # endif #endif`](https://stackoverflow.com/questions/1854550/c-macro-define-indentation)
- [\_DEBUG is specific to MSVC, thus use NDEBUG](https://stackoverflow.com/questions/2290509/debug-vs-ndebug), [Pass `-D NDEBUG` to disable asssets + enable optimizations](https://stackoverflow.com/questions/2249282/c-c-portable-way-to-detect-debug-release)
- Do not perform tasks within `assert()`, due to: the standard says "[`#if NDEBUG\n#define assert(x) (0)\n#endif`]".
- All userland errors should go to `throw std::exception()` _or derivatives of std::exception_, `std::cerr`, `extern int errno;`, or `return errno;`. Comments about possible errors should go above function declarations (Doxygen convention).
  - `throw` / `std:cerr` should use the new common syntax for this: `"[WARN_LEVEL: OPTIONAL_FUNCTION_NAME {code which triggered the error/warning/diagnostic/notice} /* OPTIONAL COMMENTS */]"`, 
  - [`./cxx/Macros.hxx`](https://github.com/SwuduSusuwu/SubStack/tree/trunk/cxx/Macros.hxx): {SUSUWU_STR(x), SUSUWU_CERR(x), SUSUWU_STDOUT(x)} have the new syntax for this.
- [_Doxygen_-ish "@pre"/"@post" prepares for _C++26_ _Contracts_](https://github.com/doxygen/doxygen/issues/6702): 
```
/* @throw std::bad_alloc If function uses {malloc, realloc, new[], std::*::{push_back, push_front, insert}}
 * @throw std::logic_error Optional. Would include most functions which use std::*
 * @pre @code !output.full() @endcode
 * @post @code !output.empty() @endcode
 */
bool functionDeclaration(std::string input, std::deque<vector> output);
```
  - It is arguable whether or not you should document such possible system errors; almost all Standard Template Library functions can throw derivatives of `std::logic_error`.
  - Regex `:%s/@pre (.*) @code (.*) @endcode/[[expects: \2]] \\* \1 \\*/` `:%s/@post (.*) @code (.*) @endcode/[[ensures: \2]] \\* \1 \\*/` once have _Contracts_/_C++26_
  - cxx/Macros.cxx has `ASSUME(X)`, which is close to `[[expects: x]]`, but `ASSUME(X)` goes to `*.cxx`, whereas `[[expects]]` goes to `*.hxx`.
  - Documentation of interfaces belongs to `*.hxx`; `*.cxx` is to do implementations. Do not duplicate interface comments.
  - Advantages of `[[expects]]`; allows to move information of interfaces out of `*.cxx`, to `*.hxx`.

