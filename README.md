(C) 2024 Swudu Susuwu, dual licenses: choose [_GPLv2_](./LICENSE_GPLv2) or [_Apache 2_](./LICENSE) (allows all uses).

# Table of Contents
- [Purposes](#Purposes)
- [How to use this](#How-to-use-this)
  - [Download](#Download)
  - [Signature/certificate](#Signaturecertificate)
  - [Options/setup](#Optionssetup)
- [How to contribute](#How-to-contribute)
  - [Beta test/experimental builds](#Beta-testexperimental-builds)
  - [Good first issues to contribute to](https://github.com/SwuduSusuwu/SubStack/contribute)
  - [Sensitive issues](./SECURITY.md#Sensitive-issues)
  - [Contributor conventions/rules](#Contributor-conventionsrules)
    - [`git`](#Git)
    - [`sh` source](#Sh-source)
    - [_C_/_C++_ source](#Cc-source)

# Purposes
[`./.ssh/`](./.ssh/) is to [compute signatures/certificates](#Signaturecertificate).

[`./posts/`](./posts/) stages posts (school classes) for [https://SwuduSusuwu.SubStack.com/](https://SwuduSusuwu.SubStack.com/) about artificial neural tissue, antivirus, assistants, plus autonomous tools.
- [`./posts/TranscodeMuxHowto.md`](./posts/TranscodeMuxHowto.md) is simple [`/bin/sh`](https://wikipedia.org/wiki/Bourne_shell) commands for advanced [`ffmpeg`](https://wikipedia.org/wiki/FFmpeg) use (formulas to encode visuals relate to [issue #2](https://github.com/SwuduSusuwu/SubStack/issues/2#issuecomment-2110726542)).
- [`./posts/AlbatrossCNS.md#Post-with-resources`](./posts/AlbatrossCNS.md#Post-with-resources) is resources which have to do with `./cxx/ClassCns.hxx` + [issue #6](https://github.com/SwuduSusuwu/SubStack/issues/6).
- [`./posts/VirusAnalysis.md#Post-with-resources`](./posts/VirusAnalysis.md#Post-with-resources) is resources which have to do with `./cxx/VirusAnalysis.hxx`+ [issue #8](https://github.com/SwuduSusuwu/SubStack/issues/8).

[`./c/`](./c/) _C_ implementations of posts (TODO, [issue #3](https://github.com/SwuduSusuwu/SubStack/issues/3) which you can contribute to, or can request that more resources go to this task)
- [`./c/rfc6234/`](./c/rfc6234) is vendored code (direct from the official [_RFC6234_](https://www.rfc-editor.org/rfc/rfc6234#section-8)), which is used for {`classSha128()`, `classSha256()`, `classSha512()`}.

[`./cxx/`](/cxx/) _C++_ implementations of posts
- [`./cxx/Macros.hxx`](/cxx/Macros.hxx) is
  - macros with wrap C++ features/attributes, such as {`SUSUWU_ASSUME`, `SUSUWU_CONSTEXPR`, `SUSUWU_DEFAULT`, `SUSUWU_DELETE`, `SUSUWU_EXPECTS`, `SUSUWU_ENSURES`, `SUSUWU_FINAL`, `SUSUWU_IF_CPLUSPLUS`, `SUSUWU_NOEXCEPT`, `SUSUWU_NORETURN`, `SUSUWU_NULLPTR`, `SUSUWU_OVERRIDE`, `SUSUWU_STATIC_ASSERT`, `SUSUWU_UNREACHABLE`} which (if used on old compilers, or with options such as `-std=c++11`) are replaced with no-ops or alternatives which have the same use,
  - macro options (which control the macro constants/macro functions). (View [Options/setup](/README.md#Optionssetup) for options),
  - macro constants, such as `SUSUWU_SH_<color>` (`color` = {`DEFAULT`, `BLACK`, `DARK_GRAY`, `RED`, `LIGHT_RED`, `GREEN`, `LIGHT_GREEN`, `BROWN`, `YELLOW`, `BLUE`, `LIGHT_BLUE`, `PURPLE`, `LIGHT_PURPLE`, `CYAN`, `LIGHT_CYAN`, `LIGHT_GRAY`, `WHITE`}, if supported, expands to the [_ANSI_ color](https://wikipedia.org/wiki/Ansi_color) codes, else expands to ""),
  - macro functions, such as {`SUSUWU_ERROR`, `SUSUWU_WARNING`, `SUSUWU_INFO`, `SUSUWU_SUCCESS`, which use `SUSUWU_PRINT`}, `SUSUWU_PRINT` (if `__cplusplus`, uses `SUSUWU_CERR`, else uses `SUSUWU_STDERRR`),
  - `macroTestsNoexcept()` (unit tests, with return value for errors).
- [`./cxx/ClassObject.hxx`](/cxx/ClassObject.hxx) is
  - `class Instrumentation` (port of [`java.lang.instrument.Instrumentation`](https://docs.oracle.com/javase/8/docs/api/java/lang/instrument/Instrumentation.html)), `class Class : public Instrumentation` (port of [`java.lang.Class`](https://docs.oracle.com/javase/8/docs/api/java/lang/Class.html)), `class Object : public Class` (port of [`java.lang.Object`](https://docs.oracle.com/javase/8/docs/api/java/lang/Object.html)),
  - `classObjectTests()`, or `classObjectTestsNoexcept()` (unit tests with exceptions for errors, or return value for errors).
- [`./cxx/ClassPortableExecutable.hxx`](/cxx/ClassPortableExecutable.hxx) is
  - `FilePath` (`PortableExecutable`'s constructor argument), `FileBytecode` (`classSha2`'s input argument), `FileHash` (`classSha2`'s return value)
  - `class PortableExecutable : public Object` (stores file `path` and/or `bytecode` and/or `hex`code. TODO; `hash`?)
  - `class PortableExecutableBytecode : public PortableExecutable` loads `bytecode` from `path`. TODO; `hash`?
- [`./cxx/ClassSys.hxx`](/cxx/ClassSys.hxx) is
  - typedefs {ClassSysUSeconds}
  - globals {classSysArgc, classSysArgs}
  - modular functions to interact with: console (_Posix_ `/bin/sh` or _Windows_ `cmd``) {`classSysGetConsoleInput()`, `classSysSetConsoleInput()`}, own process {`classSysInit()`, `classSysGetOwnPath()`, `classSysFopenOwnPath()`, `templateCatchAll()`}, strings (or streams) {`classSysHexOs()`, `classSysHexStr()`, `classSysColoredParamOs()`, `classSysColoredParamStr()`}, the OS {`classSysUSecondClock()`, `execvesFork()`, `execvexFork()`, `execves()`, `execvex()`, `classSysHasRoot()`, `classSysSetRoot()`, `classSysKernelCallback()`, `classSysKernelSetHook()`}. TODO: filesystem (perhaps just have `cxx/ClassPortableExecutable.hxx` do this?), internet.
  - `classSysTests()`, or `classSysTestsNoexcept()` (unit tests with exceptions for errors, or return value for errors).
- [`./cxx/ClassSha2.hxx`](/cxx/ClassSha2.hxx) is
  - the `classSha2` function pointer, which defaults to `classSha256()` (but you can set `classSha2 = sha128;` or `classSha2 = sha512;`), wrapped around official _RFC6234_ code. `./cxx/ClassResultList.hxx`, `./cxx/VirusAnalysis.cxx` and `./cxx/AssistantCns.cxx` all use `classSha2`.
  - `classSha2Tests()`, or `classSha2TestsNoexcept()` (unit tests with exceptions for errors, or return value for errors).
- [`./cxx/ClassCns.hxx`](/cxx/ClassCns.hxx) is `class Cns : public Object` (abstract neural system class with pure virtuals.) [Issue #6](https://github.com/SwuduSusuwu/SubStack/issues/6) is to implement this class.
- [`./cxx/ClassResultList.hxx`](/cxx/ClassResultList.hxx) is
  - `class ResultList : public Object` (holds `hashes`, `signatures`, `bytecodes`); `resultList*()` functions {`resultListDumpTo()`, `resultListProduceHashes()` (`virusAnalysisTests()` uses this)}.
  - modular template (can use on all containers such as `std::vector`, `std::map` or `std::list`) `list*()` functions (such as `listMaxSize()`, `listDumpTo()`, `listToHashes()`, `listIntersections()`, `listsIntersect()`, `listFindValue()`, `listHasValue()`, `listFindSubstr()`, `listHasSubstr()`, `listProduceSignature()` (`produceAbortListSignatures` uses this), `listFindSignatureOfValue()`, `listHasSignatureOfValue()` (`signatureAnalysis()` uses this), `explodeToList` (`./cxx/AssistantCns.cxx` uses this), 
  produce unique signature, compare file against list of signatures), most of which were produced for antivirus signature analysis.
  - `classResultListsTests()`, or `classResultListsTestsNoexcept()` (unit tests with exceptions for errors, or return value for errors).
- [`./cxx/VirusAnalysis.hxx`](/cxx/VirusAnalysis.hxx) is
  - modular helper functions {`produceAbortListSignatures()` (for `signatureAnalysis()` use), `importedFunctionsList()` (work-in-progress, `staticAnalysis()` uses this), `straceOutputsAnalysis()` (work-in-progress, `sandboxAnalysis()` uses this), `produceAnalysisCns()` (for `cnsAnalysis()` use), `produceVirusFixCns()` (for `cnsVirusFix()` use)},
  - kernel hook function (`virusAnalysisHook()`), which uses `virusAnalysis()` to scan new downloadss (or scan all programs which execute); work-in-progress.
  - modular scan functions {`hashAnalysis()`, `signatureAnalysis()`, `staticAnalysis()` (processes [_Executable and Linkable Format_](https://wikipedia.org/wiki/Executable_and_Linkable_Format) or [_Portable Executable_](https://wikipedia.org/wiki/Portable_Executable)s, work-in-progress), `sandboxAnalysis()` (executes with `strace` + `chroot`, work-in-progress), `cnsAnalysis()` (uses `ClassCns.hxx`)} plus disinfection function (`cnsVirusFix()`) which form an antivirus program.
  - `virusAnalysisTests()`, or `virusAnalysisTestsNoexcept()` (unit tests with exceptions for errors, or return value for errors).
- [`./cxx/AssistantCns.hxx`](/cxx/AssistantCns.hxx) is
  - modular functions {`assistantCnsDownloadHosts()` (uses `wget` on `assistantCnsDefaultHosts`), `assistantCnsProcessXhtml()` (uses the next 2 functions to process `wget`'s downloads: `assistantCnsProcessUrls` (uses `boost/property_tree/xml_parser.hpp` to extract new URLs), `assistantCnsProcessQuestion` (work-in-progress, extracts question), `assistantCnsProcessResponses()` (work-in-progress, extracts answers)), `produceAssistantCns()` (uses datasets for backpropagation), `assistantCnsProcess` (uses forwardpropagation to answer new questions)} which form an assistant.
  - `assistantCnsTests()`, or `assistantCnsTestsNoexcept()` (unit tests with exceptions for errors, or return value for errors).
- [`./cxx/main.hxx`](/cxx/main.hxx) is `SusuwuUnitTestsBitmask main()` (executes all of those `*TestsNoexcept()` unit tests into a bitmask return value.)
All have lots of [issues](https://github.com/SwuduSusuwu/SubStack/issues) which you can contribute to, or can request that more resources go to).


[`./Macros.sh`](./Macros.sh) is a modular `./bin/sh` script with functions which `./build.sh` uses.

[`./build.sh`](./build.sh) does what `./configure`, `make` often do. (View [Options/setup](#Optionssetup) for options).

# How to use this
Minimum requirements (build targets which this supports):
- Operating systems: _Windows_, _Linux_ (such as _Android_ or _Ubuntu_), _Unix_ (such as _BSD_, _Solaris_ or _Mach_/_OSX_), or _iOS_.
- Languages: Minimum [_C++11_](https://gcc.gnu.org/projects/cxx-status.html#cxx11) (all `CXX` with `201102 <= __cplusplus`,) due to use of `auto`, `class { bool defaultMemberInit = true; };`, `decltype`, `for(value: list) {}`).
  - Other than those 4, most non-[_C++98_](https://gcc.gnu.org/projects/cxx-status.html#cxx98) features were replaced with [`./cxx/Macros.hxx`](./cxx/Macros.hxx) macros (which turn into no-ops if the compile doesn't support those), such as: [`constexpr`, `default`, `final`, `__func__`, `override`, `noexcept`, `nullptr`, `static_assert`](https://gcc.gnu.org/projects/cxx-status.html#cxx11), [`[[no_unique_address]]`](https://gcc.gnu.org/projects/cxx-status.html#cxx20).
  - \[Notice: update [`./c/README.md#Progress`](./c/README.md#Progress) if you update this list.\]
  - If you must have _C99_ support; ask for this (in [issue #3](https://github.com/SwuduSusuwu/SubStack/issues/3)), or [contribute](#Contributor-conventionsrules).
  - If you must have _C++98_ support; ask for this (in [issue #20](https://github.com/SwuduSusuwu/SubStack/issues/20)), or [contribute](#Contributor-conventionsrules).
## Download
Download source with `git clone https://github.com/SwuduSusuwu/SubStack.git`. If this does not have all the tools you want, you can opt-in to the beta with `git switch experimental` (opt-out with `get switch trunk`).
## Signature/certificate
`.ssh/setup.sh` (you can compare those certificates to [our blog post](https://swudususuwu.substack.com/p/githubcomswudususuwusubstack-certificate-new).)

`git log —show-signatures` (which shall match [`./.ssh/sha256.sig`](./.ssh/sha256.sig) for [new commits](https://github.com/SwuduSusuwu/SubStack/commit/3efe601f15ae0fdfd05cfbc1a75a0e6a4a08124b)),

or `git verify-commit <ref>` (where <ref> is the hash of an individual commit).

\[Notice: This [public crypto](https://docs.gitlab.com/ee/user/project/repository/signed_commits/ssh.html#verify-commits-locally) "signature", is not related to "signature analysis" ([Substr scans](#Purposes)).\]
## Options/setup
Usage: [`./build.sh [OPTIONS]`](./build.sh) produces objects (`./obj/*.o`, for distribution into other tools,) plus [_Executable and Linkable Format_](https://wikipedia.org/wiki/Executable_and_Linkable_Format) (`./bin/Susuwu.out`, to do examples/[unit tests](https://wikipedia.org/wiki/Unit_test#Agile) which prove how effective functions execute,) both of which you can redirect with `export OBJDIR=___` (or `export BINDIR=___`.)
- [`./cxx/main.hxx`](./cxx/main.hxx) has constants to use to interpret `Susuwu.out`'s return values.
- Environment flags: as [_GNU_ `make`'s](https://www.gnu.org/software/make/manual/make.html#Implicit-Variables).
- Console flags:
  - `./build.sh` : Defaults to `./build.sh --debug`. For all source code, if intermediate object doesn't exist or is older than source, builds source.
  - `./build.sh --clean` : removes intermediate object files + exits; to reduce disc use.
  - `./build.sh --rebuild` : removes intermediate object files + continues; to rebuild with new flags (or if `./build.sh` doesn't rebuild code which includes updated headers).
  - `./build.sh --debug` : includes frame-pointers/debug symbols (`-g`), includes `valgrind`-replacement tools (such as `-fsanitize=address`), optimizes with `-Og`.
  - `./build.sh --release` : excludes `--debug` (`-DNDEBUG`), strips frame-pointers/symbols, optimizes with `-O2`.
  - `./build.sh --mingw` : can mix with `--release` or `--debug`. Produces [_Portable Executable_](https://wikipedia.org/wiki/Portable_Executable) (`./bin/Susuwu.exe`), for _Windows_.
- Macro flags (use `vim build.sh` to put into `FLAGS_USER`). If `=true`, most use more resources, except `SUSUWU*PREFER_*` or `SUSUWU*SKIP_*`. "default is `=!defined(NDEBUG)`" is short for; "if `--debug`, default `=true`, but if `--release`, default `=false`".
  - `-DSUSUWU_UNIT_TESTS[=true|=false]` with `=true` to build + execute unit tests. Default is `=true`, but more stable future version could have default `=!defined(NDEBUG)`. If set to `=false`; compilation time, object size, execuable size reduced (to around half).
  - Custom `sh` (console) output:
    - `-DSUSUWU_SH_PREFER_STDIO=true` to replace `std::cXXX << ...` with `fprintf(stdXXX, ...)`; default is `=!defined(__cplusplus)`.
    - `-DSUSUWU_SH_VERBOSE[=true|=false]` with `=true` to print diagnostic messages (`SUSUWU_SH_USE_FILE`, `SUSUEU_SH_USE_LINE`, `SUSUWU_NOTICE`, `SUSUWU_DEBUG`, `SUSUWU_DEBUGEXECUTE`, `SUSUWU_NOTICE_EXECUTE`, `SUSUWU_DEBUG_EXECUTE` all use `#if SUSUWU_SH_VERBOSE`); default is `=!defined(NDEBUG)`.
    - `-DSUSUWU_SH_SKIP_BRACKETS=true` sets output format to `WARN_LEVEL: message`; default is `=false`.
    - `-DSUSUWU_SH_FILE=true` sets output format to `[__FILE__: WARN_LEVEL: message]`; default is `=SUSUWU_SH_VERBOSE`.
    - `-DSUSUWU_SH_LINE=true` sets output format to `[__LINE__: WARN_LEVEL: message]`; default is `=SUSUWU_SH_VERBOSE`.
    - `-DSUSUWU_SH_FUNC=true` sets output format to `[__func__: WARN_LEVEL: message]`; default is `=false`.
    - `-DSUSUWU_SH_SKIP_COLORS=true` to omit _VT100_ (_ANSI_) colors; default is `=defined(SUSUWU_SH_COLORS_UNSUPPORTED)`).
    - `-DSUSUWU_SH_SKIP_COLORS=false` to force (even if unsupported) _VT100_ ([_ANSI_ color](https://wikipedia.org/wiki/Ansi_color)) use.
    - TODO (for now, no effect; once [issue #17](https://github.com/SwuduSusuwu/SubStack/issues/17) is closed, you can use):
      - `-DSUSUWU_SH_RUNTIME_OSC` to replace `#ifdef _POSIX_VERSION\nAccessClipboard();\n#endif` with `termcmp`./`GetConsoleMode()` (for choices on whether or not to use Operating System Commands); default is undefined.
      - `-DSUSUWU_SH_RUNTIME_COLORS` to replace `#if _POSIX_VERSION\nColors();\n#endif` with `termcmp`./`GetConsoleMode()` (for choices on whether or not to use colors); default is undefined.
  - To match `g++`./`clang++` console format, use `-DSUSUWU_SKIP_BRACKETS=true, -DSUSUWU_SH_FILE=true, -DSUSUWU_SH_LINE=true, -DSUSUWU_SH_FUNC=false` (sets output format to `__FILE__:__LINE__: WARN_LEVEL: message`).
  - Unstable/`experimental` flags:
    - `-DSUSUWU_EXPERIMENTAL` to enable experimental (more new, but unfinished/unstable) versions of code; default is unset, unless `git switch experimental` is executed.
    - `-DSUSUWU_VIRTUAL_OPERATORS_USE_VPTRS=false`: [`./cxx/ClassObject.hxx`](./cxx/ClassObject.hxx):`Class::operator==(const Class &obj) { return this->hasLayoutOf(obj) && 0 == memcmp(sizeof(NULL) + (char *)this, sizeof(NULL) + (char *)&obj, this->getObjectSize() - sizeof(NULL)); }`, thus `Susuwu::Object() == Susuwu::Class()` but `CXX` output with nonstandard `vptr` layout crashes. Default `=true`; (`return typeid(this) == typeid(obj) && 0 == memcmp(this, *obj, this->getObjectSize());`).
    - `-DSUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES=false`: to use [`./cxx/ClassObject.hxx`](./cxx/ClassObject.hxx):`Object::equals(const Object &obj) { return this->operator==(obj); }`. Default is `=true` (`return this == &obj`). For now, just controls `Object::equals` (in future, perhaps `SUSUWU_VIRTUAL_OPERATORS_USE_ADDRESSES` inherits this).
  - TODO (for now won't build, or has no effect):
    - `-DSUSUWU_VIRTUAL_OPERATORS_USE_ADDRESSES=true`: No effect. If implemented, `Class::operator==(const Class &obj) { return &obj == this; }`. Default is `=false`.
    - `-DSUSUWU_PREFER_CSTR=true` to replace `std::string` with `char *` (more compatible with non-C++ projects); default is `=SUSUWU_PREFER_C`.
    - `-DSUSUWU_PREFER_C=true` sets `SUSUWU_PREFER_CSTR` + `SUSUWU_SH_PREFER_STDIO` ([plus other flags which will exist to allow non-C++ projects to include this](https://github.com/SwuduSusuwu/SubStack/issues/3); default is `=!defined(__plusplus)`).

# How to contribute
View [documented issues](https://github.com/SwuduSusuwu/SubStack/issues/) (for ideas on code to contribute, plus so you do not report documented issues.)
## Beta test/`experimental` builds
- `git switch experimental && ./build.sh`
  - View results for symptoms of new issues (hint: look for "Warning:"s or "Error:"s).
  - If you found new issue(s) (which aren't due to misconfigurations in your system), [post new issue(s)](https://github.com/SwuduSusuwu/SubStack/issues/new).
    - Note: [sensitive issue(s)](./SECURITY.md#Sensitive-issues) have a separate protocol.
# Contributor conventions/rules
If your commit introduces/removes functions, have `./README.md#Purposes` include this.
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
- if `git mv OldPath/ NewPath/`: `\`OldPath/\` -> \`NewPath/\`` or `mv OldPath/ NewPath/`.
- to indent: use tabs to form blocks, such as:
```
?`README.md`:
	?`#How-to-use-this`:
	Split into:
		+`## Download`: new; howto clone, howto switch branches.
		+`## Optionssetup`: "Options/setup"; howto use `./build.sh` (with or without options.)
	?`#How-to-contribute`,
	?[Good first issues to contribute to]: (moved into `#How-to-contribute`)
```
/[Notice: Commit titles can omit backticks (``) if not enough room; the backticks just allow _GitHub_ to do _Markdown_-format code/paths.\]
## `sh` source
Is as for [_C_/_C++_ source](#Cc-source), plus specifics to `sh`:
- Act as if all functions/variables are macros (which use `CONSTANT_CASE`).
- Variable access: uses `${...}` (thus not `echo $BOOL`, but `echo ${BOOL}`).
  - Rationales:
    - In case future versions append to this (`echo $BOOL2` is a silent error, but `echo ${BOOL}2` is cool).
    - Avoids [SC2250](https://www.shellcheck.net/wiki/SC2250) ["Prefer putting braces around variable references even when not strictly required." notices](https://github.com/SwuduSusuwu/SubStack/security/code-scanning?query=rule%3Ashellcheck_SC2250).
  - Exceptions: [language limits](https://www.shellcheck.net/wiki/SC3030).
    - To support `/bin/sh`: do not use `${@}`, but `$@`.
- Str variable access: uses `"$..."` (thus not `ls ${STR}`, but `ls "${STR}"`).
  - Rationales:
    - So if `STR="/bin/"` is replaced with `STR="/path with/spaces/"` ([without `IFS=""`](https://tldp.org/LDP/abs/html/internalvariables.html#IFSREF) \[[2](https://www.commandlinux.com/man-page/man1/sh.1.html#lbBK)\]), that 1 parameter [won't expand into 2](https://tldp.org/LDP/abs/html/special-chars.html#FIELDREF).
    - So if `STR="/bin/"` is replaced with `STR="*"` ([without `set -f`](https://www.commandlinux.com/man-page/man1/sh.1.html#lbBL)), the glob is passed to `ls` (which [expands this into numerous paths](https://tldp.org/LDP/abs/html/globbingref.html), rather than expanded in your script.
    - Avoids [SC2086](https://www.shellcheck.net/wiki/SC2086) ["Double quote to prevent globbing and word splitting." notices](https://github.com/SwuduSusuwu/SubStack/security/code-scanning?query=rule%3Ashellcheck_SC2086).
  - Exceptions: specifics of command use.
    - Do not use `ctags "${FLAGS}" "${PATH}"`, but `ctags ${FLAGS} "${PATH}"`.
- Str variable access: uses `"${...}"` (thus not `if [ "-q" = ${PARAM} ]`, but `if [ "-q" = "${PARAM}" ]`).
  - Rationales: in case `${PARAM}` has spaces.
    - Avoids [SC2068](https://www.shellcheck.net/wiki/SC2068) ["Double quote array expansions to avoid re-splitting elements." notices](https://github.com/SwuduSusuwu/SubStack/security/code-scanning?query=rule%3Ashellcheck_SC2068).
  - Exceptions: to split (on spaces) is the purpose of the `for` loop.
    - To parse numerous params: do not use `for VALUE in "$@"`; do`, but `for VALUE in $@; do`.
- For temp variables, you affix `local` (thus not `for VALUE in ${LIST}; do`, but `local VALUE; for VALUE in ${LIST}; do`).
  - Rationales: avoids [*shadowed variables*](https://wikipedia.org/wiki/Variable_shadowing) (such as `f2() { VALUE=false; }; f1() { VALUE=true; f2(); return VALUE; };` causes)
  - Exceptions: [language limits](https://austingroupbugs.net/bug_view_page.php?bug_id=767).
    - To support `/bin/sh`: do not use `local` (such as `f2() { local VALUE=false; }`), but [subshells](https://stackoverflow.com/questions/18597697/posix-compliant-way-to-scope-variables-to-a-function-in-a-shell-script/64946874#64946874) (such as `f2() ( VALUE=false; )`.)
    - `local` is [not applicable to `IFS="<delimiter>"` or `cd <path>`](https://unix.stackexchange.com/questions/493729/list-of-shells-that-support-local-keyword-for-defining-local-variables/493838#493838), [not applicable to `readonly`](https://unix.stackexchange.com/questions/506009/unable-to-have-a-local-variable-with-the-same-name-as-a-global-read-only-variabl) (so use subshells for this).
    - If you require code which is consistant across platforms ([`local` has inconsistant dynamic versus static scope, plus inconsistant inheritance](https://unix.stackexchange.com/questions/493729/list-of-shells-that-support-local-keyword-for-defining-local-variables/493743#493743)).
- Command variables: uses `$(...)` (thus not `` stat `pwd` `` , but `stat $(pwd)`).
  - Rationales: most simple to nest (`echo $(stat $(pwd))`). Common (much known) subshell syntax is reused.
    - Avoids [SC2006](https://www.shellcheck.net/wiki/SC2006) ["Use $(...) notation instead of legacy backticked ...." notices](https://github.com/SwuduSusuwu/SubStack/security/code-scanning?query=rule%3Ashellcheck_SC2006).
## _C_/_C++_ source
Linter: `apt install clang && clang-tidy cxx/*.cxx` (defaults to [`.clang-tidy`](./.clang-tidy) options).

Most of what [_Mozilla Org_'s (_Firefox_'s) style](https://firefox-source-docs.mozilla.org/code-quality/coding-style/coding_style_cpp.html) suggests is sound (you should follow this unless you have specific reasons not to).

Code rules (lots overlap with _Mozilla Org_'s):

- Files: `${C_SOURCE_PATH}/PascalCase.*` or `${CXX_SOURCE_PATH}/PascalCase.*xx` (such as: `./cxx/ClassFoo.hxx`, used as `#include "ClassFoo.hxx" /* classFooFunction() */`), as this is most common.
  - `./build.sh` requires: that all local includes prefix as `Class*.hxx` (so it knows to execute `--rebuild` if you upgrade a common include.) TODO: incremental builds which don't require this.
  - To assist with insertion/removal of `#include` statements, comments shall list all functions/macros used.

- Structs, enums, classe: `typedef struct PascalCase {} PascalCase;`, `typedef enum PascalCase {} PascalCase;`, `typedef class PascalCase {} PascalCase;`, as this is most common.

- Macros: `#define NAMESPACE_CONSTANT_CASE(snake_case_param) assert(snake_case_param);`, as this is most common.

- Indent: tabs ('^I', reduced memory use, allows local configs to set width, allows arrow keys to move fast); as much tabs as braces ('{', '}'). \[All which conflicts with _Mozilla Org_'s format is tab use.\]

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
- Do not perform tasks within `assert()`, due to: the standard says "\[`#if NDEBUG\n#define assert(x) (0)\n#endif`\]".
- All userland errors should go to `throw std::runtime_error(message)` (or `throw Q()` where `class Q : public std::exception`), `std::cerr << message`, `errno = code;`, or `return code;`. Comments about possible errors should go above function declarations (Doxygen convention).
  - `message` should use the new common syntax for this: `"[WARN_LEVEL: OPTIONAL_FUNCTION_NAME {code which triggered the error/warning/diagnostic/notice} /* OPTIONAL COMMENTS */]"`,
  - [`./cxx/Macros.hxx`](./cxx/Macros.hxx): {SUSUWU_STR(x), SUSUWU_CERR(x), SUSUWU_STDOUT(x)} have the new syntax for this.
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
  - [`./cxx/Macros.hxx`](./cxx/Macros.hxx) has `ASSUME(X)`, which is close to `[[expects: x]]`, but `ASSUME(X)` goes to `*.cxx`, whereas `[[expects]]` goes to `*.hxx`.
  - Documentation of interfaces belongs to `*.hxx`; `*.cxx` is to do implementations. Do not duplicate interface comments.
  - Advantages of `[[expects]]`; allows to move information of interfaces out of `*.cxx`, to `*.hxx`.

