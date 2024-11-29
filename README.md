Dual licenses: choose [_Creative Commons_](https://creativecommons.org/share-your-work/use-remix/) or _[Apache 2](https://github.com/SwuduSusuwu/SubStack/blob/trunk/LICENSE)_ (allows all uses).

# Table of Contents
- [Purposes](#Purposes)
- [How to use this](#How-to-use-this)
  - [Download](#Download)
  - [Options/setup](#Optionssetup)
- [How to contribute](#How-to-contribute)
  - [Beta test/experimental builds](#Beta-testexperimental-builds)
  - [Good first issues to contribute to](https://github.com/SwuduSusuwu/SubStack/contribute)
  - [Sensitive issues](https://github.com/SwuduSusuwu/SubStack/blob/trunk/SECURITY.md#Sensitive-issues)
  - [Contributor conventions/rules](#Contributor-conventionsrules)
    - [`git`](#Git)
    - [`sh` source](#Sh-source)
    - [_C_/_C++_ source](#Cc-source)

# Purposes
[`./posts/`](https://github.com/SwuduSusuwu/SubStack/blob/trunk/posts/) stages posts (school classes) for [https://SwuduSusuwu.SubStack.com/](https://SwuduSusuwu.SubStack.com/) about artificial neural tissue, antivirus, assistants, plus autonomous tools.

[`./c/`](https://github.com/SwuduSusuwu/SubStack/blob/trunk/c/) _C_ implementations of posts (TODO, [issue #3](https://github.com/SwuduSusuwu/SubStack/issues/3) which you can contribute to, or can request that more resources go to this task), + vendored code (for now just _RFC6234_, for `sha2`).

[`./cxx/`](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/) _C++_ implementations of posts (for now is just neural system pure virtual template ([`./cxx/ClassCns.hxx`](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/ClassCns.hxx)) + antivirus([`./cxx/VirusAnalysis.cxx`](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/VirusAnalysis.cxx)) + assistant([`./cxx/AssistantCns.cxx`](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/AssistantCns.cxx)), with lots of [issues](https://github.com/SwuduSusuwu/SubStack/issues) which you can contribute to, or can request that more resources go to).

# How to use this
Minimum requirements (build targets which this supports):
- Operating systems: _Windows_, _Linux_ (such as _Android_ or _Ubuntu_), _Unix_ (such as _BSD_, _Solaris_ or _Mach_/_OSX_), or _iOS_.
- Languages: Minimum [_C++11_](https://gcc.gnu.org/projects/cxx-status.html#cxx11) (all `CXX` with `201102 <= __cplusplus`,) due to use of `auto`, `class { bool defaultMemberInit = true; };`, `decltype`, `for(value: list) {}`).
  - Other than those 4, most non-[_C++98_](https://gcc.gnu.org/projects/cxx-status.html#cxx98) features were replaced with `cxx/Macros.hxx` macros (which turn into no-ops if the compile doesn't support those), such as: [`constexpr`, `default`, `final`, `__func__`, `override`, `noexcept`, `nullptr`, `static_asset`](https://gcc.gnu.org/projects/cxx-status.html#cxx11), [`[[no_unique_address]]`](https://gcc.gnu.org/projects/cxx-status.html#cxx20).
  - If you must have _C99_ support; ask for this (in [issue #3](https://github.com/SwuduSusuwu/SubStack/issues/3)), or [contribute](#Contributor-conventionsrules).
  - If you must have _C++98_ support; ask for this (in [issue #20](https://github.com/SwuduSusuwu/SubStack/issues/20)), or [contribute](#Contributor-conventionsrules).
## Download
Download source with `git clone https://github.com/SwuduSusuwu/SubStack.git`. If this does not have all the tools you want, you can opt-in to the beta with `git switch experimental` (opt-out with `get switch trunk`).
## Options/setup
Usage: [`./build.sh [OPTIONS]`](https://github.com/SwuduSusuwu/SubStack/blob/trunk/build.sh) produces objects (`./obj/*.o`, for distribution into other tools,) plus [_Executable and Linkable Format_](https://wikipedia.org/wiki/Executable_and_Linkable_Format) (`./bin/a.out`, to do examples/[unit tests](https://wikipedia.org/wiki/Unit_test#Agile) which prove how effective functions execute,) both of which you can redirect with `export OBJDIR=___` (or `export BINDIR=___`.)
- [`./cxx/main.hxx`](https://github.com/SwuduSusuwu/SubStack/blob/trunk/cxx/main.hxx) has constants to use to interpret `a.out`'s return values.
- Console flags:
  - `./build.sh` : Defaults to `./build.sh --debug`. For all source code, if intermediate object doesn't exist or is older than source, builds source.
  - `./build.sh --clean` : removes intermediate object files + exits; to reduce disc use.
  - `./build.sh --rebuild` : removes intermediate object files + continues; to rebuild with new flags (or if `./build.sh` doesn't rebuild code which includes updated headers).
  - `./build.sh --debug` : includes frame-pointers/debug symbols (`-g`), includes `valgrind`-replacement tools (such as `-fsanitize=address`), optimizes with `-Og`.
  - `./build.sh --release` : excludes `--debug` (`-DNDEBUG`), strips frame-pointers/symbols, optimizes with `-O2`.
  - `./build.sh --mingw` : can mix with `--release` or `--debug`. Produces [_Portable Executable_](https://wikipedia.org/wiki/Portable_Executable) (`./bin/a.exe`), for _Windows_.
- Environment flags (use `vim build.sh` to put into `FLAGS_USER`); other than `_PREFER_`/`_SKIP_`, most use more resources if set to `true`:
  - Custom `sh` (console) output:
    - `-DSUSUWU_SH_PREFER_STDIO` to replace `std::cXXX << ...` with `fprintf(stdXXX, ...)`; default is `!defined(__cplusplus)`.
    - `-DSUSUWU_SH_VERBOSE` to print diagnostic messages (`SUSUWU_SH_USE_FILE`, `SUSUEU_SH_USE_LINE`, `SUSUWU_NOTICE`, `SUSUWU_DEBUG`, `SUSUWU_DEBUGEXECUTE`, `SUSUWU_NOTICE_EXECUTE`, `SUSUWU_DEBUG_EXECUTE` all use `#if SUSUWU_SH_VERBOSE`); default is `!defined(NDEBUG)`.
    - `-DSUSUWU_SH_SKIP_BRACKETS=true` sets output format to `WARN_LEVEL: message`; default is `false`.
    - `-DSUSUWU_SH_FILE=true` sets output format to `[__FILE__: WARN_LEVEL: message]`; default is `SUSUWU_SH_VERBOSE`.
    - `-DSUSUWU_SH_LINE=true` sets output format to `[__LINE__: WARN_LEVEL: message]`; default is `SUSUWU_SH_VERBOSE`.
    - `-DSUSUWU_SH_FUNC=true` sets output format to `[__func__: WARN_LEVEL: message]`; default is `false`.
    - `-DSUSUWU_SH_SKIP_COLORS=true` to omit _VT100_ (_ANSI_) colors; default is `defined(SUSUWU_SH_COLORS_UNSUPPORTED)`).
    - `-DSUSUWU_SH_SKIP_COLORS=false` to force (even if unsupported) _VT100_ (_ANSI_) color use.
    - TODO (for now, no effect; once [issue #17](https://github.com/SwuduSusuwu/SubStack/issues/17) is closed, you can use):
      - `-DSUSUWU_SH_RUNTIME_OSC` to replace `#ifdef _POSIX_VERSION\nAccessClipboard();\n#endif` with `termcmp`/`GetConsoleMode()` (for choices on whether or not to use Operating System Commands); default is undefined.
      - `-DSUSUWU_SH_RUNTIME_COLORS` to replace `#if _POSIX_VERSION\nColors();\n#endif` with `termcmp`/`GetConsoleMode()` (for choices on whether or not to use colors); default is undefined.
  - To match `g++`/`clang++` console format, use `-DSUSUWU_SKIP_BRACKETS=true, -DSUSUWU_SH_FILE=true, -DSUSUWU_SH_LINE=true, -DSUSUWU_SH_FUNC=false, -DSUSUWU_SKIP_COLORS=false` (sets output format to `__FILE__:__LINE__: WARN_LEVEL: message`).
  - Unstable/`experimental` flags:
    - `-DSUSUWU_EXPERIMENTAL` to enable experimental (more new, but unfinished/unstable) versions of code; default is unset, unless `git switch experimental` is executed.
  - TODO (for now won't build, or has no effect):
    - `-DSUSUWU_PREFER_CSTR` to replace `std::string` with `char *` (more compatible with non-C++ projects); default is `false`.
    - `-DSUSUWU_PREFER_C` sets `SUSUWU_PREFER_CSTR` + `SUSUWU_SH_PREFER_STDIO` ([plus other flags which will exist to allow non-C++ projects to include this](https://github.com/SwuduSusuwu/SubStack/issues/3); default is `false`.

# How to contribute
View [documented issues](https://github.com/SwuduSusuwu/SubStack/issues/) (for ideas on code to contribute, plus so you do not report documented issues.)
## Beta test/`experimental` builds
- `git switch experimental && ./build.sh`
  - View results for symptoms of new issues (hint: look for "Warning:"s or "Error:"s).
  - If you found new issue(s) (which aren't due to misconfigurations in your system), [post new issue(s)](https://github.com/SwuduSusuwu/SubStack/issues/new).
    - Note: [sensitive issue(s)](https://github.com/SwuduSusuwu/SubStack/blob/trunk/SECURITY.md#Sensitive-issues) have a separate protocol.
# Contributor conventions/rules
So that code is consistant, pull requests have language-specific syntax rules:
## Git
Do atomic commits: if you cannot `./build.sh` your commit if it is swapped (such as through `git rebase -i`) with a previous commit, or cannot `./build.sh` if a previous commit got `git revert`, your commit message must include such as "Is followup to \<commit hash\>" (which shows temporal order).

`git commit` message format/syntax:
- affix "()" onto functions (regardless of number of arguments), such as `function()`, or use the function name (such as `function`) alone.
- if commit does `git add NewFile`: message has `+\`NewFile\``.
- if `git rm Exists`: `-\`Exists\``.
- if `touch Exists && git add Exists`: `@\`Exists\`` or `?\`Exists\``.
  - Simple wildcards/regex for altered functions: `\`%s/oldFunction()/newFunction()/\``.
  - '*' is not used as update prefix, since '*' has much other use in _Regex_ (wildcards) & _C++_ (such as block comments, dereferences, or math).
    - From the root commit through 159940fb8b60b176a38a13cdfbd9393596daa9b5 (Date:   Thu Jul 4 07:56:01 2024 -0700), '@' was the prefix for updates. From then until this commit, '?' was the prefix for updates.
    - From this commit on (this is the successor to commit 0ae6233c02d9e04fca60027b1e32b885eb69bb8a (Date:   Sat Nov 30 17:50:40 2024 -0800)), '@' is (once more) the prefix for updates, due to: it is more common for projects to so use '@'.
- if `echo "int newFunction() {...}" >> Exists && git add Exists`: `@\`Exists\`:+\`NewFunction()\``.
- if `git mv OldPath/ NewPath/`: `\`OldPath/.* -> NewPath/.*\``.
- to indent, use tabs to form blocks, such as
```
?`README.md`:
	?`#How-to-use-this`:
	Split into:
		+`## Download`: new; howto clone, howto switch branches.
		+`## Optionssetup`: "Options/setup"; howto use `./build.sh` (with or without options.)
	?`#How-to-contribute`,
	?[Good first issues to contribute to]: (moved into `#How-to-contribute)
```
[Notice: Commit titles can omit backticks (``) if not enough room; the backticks just allow _GitHub_ to format code/paths.]
## `sh` source
Is as for _C++_, except that you act as if all functions/variables are macros (which use `CONSTANT_CASE`).

Specific to `sh` (but doesn't conflict with _C++_):
- Variable access: uses "${...}" (thus not `if [ true = $BOOL ]`, but `if [ true = ${BOOL} ]`), in case text is tacked onto the variable ("1(true==$BOOL2)path==$PATH" is an error, but "1)true==${BOOL}2)path==${PATH}" is cool).
- Str variable access: uses `"${...}"` (thus not `if [ "-q" = ${PARAM}`, but `if [ "-q" = "${PARAM}" ]`), in case `${PARAM}` has spaces, also to avoid diagnostics (such as ["Double quote array expansions to avoid re-splitting elements."](https://github.com/SwuduSusuwu/SubStack/security/code-scanning/1724).
- For temp variables, you affix `local` (thus not `for VALUE in ${LIST}; do`, but `local VALUE; for VALUE in ${LIST}; do`).
  - [Notice; to split (on spaces) is the purpose of the `for` loop.]
## _C_/_C++_ source
Linter: `apt install clang && clang-tidy cxx/*.cxx` (defaults to [`.clang-tidy`](https://github.com/SwuduSusuwu/SubStack/blob/trunk/.clang-tidy) options).

Most of what [_Mozilla Org_'s (_Firefox_'s) style](https://firefox-source-docs.mozilla.org/code-quality/coding-style/coding_style_cpp.html) suggests is sound (you should follow this unless you have specific reasons not to).

Code rules (lots overlap with _Mozilla Org_'s):

- Files: `${C_SOURCE_PATH}/PascalCase.*` or `${CXX_SOURCE_PATH}/PascalCase.*xx` (such as: `./cxx/ClassFoo.hxx`, used as `#include "ClassFoo.hxx"`), as this is most common.
  - `./build.sh` requires: that all local includes prefix as `Class*.hxx` (so it knows to execute `--rebuild` if you upgrade a common include.) TODO: incremental builds which don't require this.

- Structs, enums, classe: `typedef struct PascalCase {} PascalCase;`, `typedef enum PascalCase {} PascalCase;`, `typedef class PascalCase {} PascalCase;`, as this is most common.

- Macros: `#define NAMESPACE_CONSTANT_CASE(snake_case_param) assert(snake_case_param);`, as this is most common.

- Indent: tabs ('^I', reduced memory use, allows local configs to set width, allows arrow keys to move fast); as much tabs as braces ('{', '}'). [All which conflicts with _Mozilla Org_'s format is tab use.]

- Braces, functions:
  - Do not produce lots of functions with the same name but different arguments, as such "overloads" make this difficult to [port](https://github.com/SwuduSusuwu/SubStack/issues/10).
  - Single statement blocks can use the form: `virtual bool hasInstance() { return true; }`.
  - Most common form:
```
const /* const prevents `if(func() = x)` where you wished for `if(func() == x)` */ bool classPrefixCamelCase(bool s, bool x) {
	if(s && x) {
		return s;
	} else {
		return x;
	}
}
```
- Args/params, local variables, objects: `const bool camelCase = true`; Global variables/objects: `extern const bool classFooFunction;`, as this is most common.
  - Functions/globals can omit "classFoo" if wrapped as `namespace ClassFoo { extern bool global; };`, or (for `class ClassFoo { static bool global; };`).
    - The project as a whole should have `namespace Susuwu {};`, but you can nest `namespace`s.)

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
- 
  - It is arguable whether or not you should document such possible system errors; almost all Standard Template Library functions can throw derivatives of `std::logic_error`.
  - Regex `:%s/@pre (.*) @code (.*) @endcode/[[expects: \2]] \\* \1 \\*/` `:%s/@post (.*) @code (.*) @endcode/[[ensures: \2]] \\* \1 \\*/` once have _Contracts_/_C++26_.
  - cxx/Macros.cxx has `ASSUME(X)`, which is close to `[[expects: x]]`, but `ASSUME(X)` goes to `*.cxx`, whereas `[[expects]]` goes to `*.hxx`.
  - Documentation of interfaces belongs to `*.hxx`; `*.cxx` is to do implementations. Do not duplicate interface comments.
  - Advantages of `[[expects]]`; allows to move information of interfaces out of `*.cxx`, to `*.hxx`.

