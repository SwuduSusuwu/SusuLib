(C) 2024 Swudu Susuwu, dual licenses: choose [_GPLv2_](./LICENSE_GPLv2) or [_Apache 2_](./LICENSE) (allows all uses).

*Notice*: You switched to the [`preview`](https://github.com/SwuduSusuwu/SusuLib/blob/preview/) branch, which has the newest features, but is unstable, and has much use of `git rebase` force pushes (which require you to use `git pull --rebase`); use [_trunk_ branch](https://github.com/SwuduSusuwu/SusuLib/blob/trunk/README.md#table-of-contents) (`git switch trunk`) for code which is more stable plus has more support.
- This `preview` branch is for [beta tests](#beta-tests--preview-builds) / [continuous integration](https://google.com?q=continuous-integration-branch).
- Until <https://github.com/SwuduSusuwu/SusuLib/discussions/70> is closed, no `git commit`s can `merge` into the `trunk` branch (due to the prohibition against use of `git rebase` for the `trunk` branch). To use those `git commit`s (which are otherwise "set to go" into the `trunk` branch), use `git switch new`.
  - The [`new`](https://github.com/SwuduSusuwu/SusuLib/blob/new/) branch is the subset of the `preview` branch which stops just before [commit 7c4f7521e @`README.md`; preview versus trunk branch use.](https://github.com/SwuduSusuwu/SusuLib/commit/7c4f7521eb3c51fd46e14dc30698ef8ded2b4768). That `git commit` marks the start of the subset of the `preview` branch which is unstable, plus which is subject to much use of `git rebase`. That `git commit` will receive a new hash on each `git push` to the `new` branch, but the `commit` "name"/"message" will stick to "@`README.md`; preview versus trunk branch use.".
  - To switch to the most new (but finished) source code, use `git switch new`.
# Table of Contents
- [Purposes](#purposes)
- [How to use this](#how-to-use-this)
  - [Download](#download)
  - [Signatures / certificates](#signatures--certificates)
  - [Options / setup](#options--setup)
- [How to contribute](#how-to-contribute)
  - [Beta tests / preview builds](#beta-tests--preview-builds)
  - [Good first issues to contribute to](https://github.com/SwuduSusuwu/SusuLib/contribute)
  - [Sensitive issues](./SECURITY.md#sensitive-issues)
  - [Contributor conventions / rules](#contributor-conventions--rules)
    - [_Markdown_](#markdown)
    - [`git`](#git)
      - [`git tag`](#git-tag)
    - [`sh` source](#sh-source)
    - [_C_/_C++_ source](#cc-source)
  - [Sponsor](#sponsor)
    - [Escrow](#escrow)
    - [Affiliates](#affiliates)
  - [Tools used](#tools-used)
  - [Contributors / sponsors](#contributors--sponsors)

# Purposes
<https://github.com/SwuduSusuwu/SusuLib/> as a whole is: [computer-related blog posts](./posts/) and modular {[`Susuwu::*`, `susuwu*`](./cxx/), [`SUSUWU_*`](./sh/)} code about those posts.
- \[Notice\] Moved from <https://github.com/SwuduSusuwu/SubStack/> to <https://github.com/SwuduSusuwu/SusuLib/> ([since the focus shifted from blog posts to source code](https://github.com/SwuduSusuwu/SusuLib/discussions/58).)
  - <https://github.com/SwuduSusuwu/SusuPosts/tree/preview/posts/> stages posts for <https://SwuduSusuwu.SubStack.com/> (which includes [`./posts/`](./posts/) plus posts which are not about artificial neural tissue, antiviruses, assistants, or autonomous tools).
- \[Notice\] Plan to move {`Susuwu::*`, `susuwu*`, `SUSUWU_*`} to {`SusuLib::*`, `susulib*`, `SUSULIB_*`}.

[`./.ssh/`](./.ssh/) is to [use signatures / certificates](#signatures--certificates).

[`./posts/`](./posts/) stages posts (virtual schools) for <https://SwuduSusuwu.SubStack.com/> about: artificial neural tissue, antiviruses, assistants, plus autonomous tools.
- [`./posts/TranscodeMuxHowto.md`](./posts/TranscodeMuxHowto.md) is simple [`/bin/sh`](https://wikipedia.org/wiki/Bourne_shell) commands for advanced [`ffmpeg`](https://wikipedia.org/wiki/FFmpeg) use (formulas to encode visuals relate to [issue #2](https://github.com/SwuduSusuwu/SusuLib/issues/2#issuecomment-2110726542)).
- [`./posts/ArduinoElegooTools.md`](./posts/ArduinoElegooTools.md) is a preview (rough-draft work-in-progress), to process sensors (which has to do with [issue #2](https://github.com/SwuduSusuwu/SusuLib/issues/2#issuecomment-2110726542)) + plan routes.
- [`./posts/SimdGpgpuTpu.md`](./posts/SimdGpgpuTpu.md) is a preview post about **SIMD**, **GPU** and **TPU** implementations of formulas, which has to do with [issue #6](https://github.com/SwuduSusuwu/SusuLib/issues/6).
- [`./posts/AlbatrossCNS.md`](./posts/AlbatrossCNS.md) is resources which have to do with `./cxx/ClassCns.hxx` + [issue #6](https://github.com/SwuduSusuwu/SusuLib/issues/6).
- [`./posts/VirusAnalysis.md`](./posts/VirusAnalysis.md) is resources which have to do with `./cxx/VirusAnalysis.hxx`+ [issue #8](https://github.com/SwuduSusuwu/SusuLib/issues/8).
- [`./posts/CnsCompress.md`](./posts/CnsCompress.md) says how to use clones of human Central Nervous Systems to do [sensor fusion](https://wikipedia.org/wiki/Sensor_fusion) + compression + decompression (which is [issue #2](https://github.com/SwuduSusuwu/SusuLib/issues/2)).
- [`./posts/JavaVersion.md`](./posts/JavaVersion.md) is [*Grok-2*](https://poe.com/Grok-2)'s [conversion](https://poe.com/s/7FJo3HoiAeGs4OhnQId1) of [`./cxx/ClassObject.hxx`](./cxx/ClassObject.hxx) into *Java* source code, which serves as a simple illustration of how *C++* source code [maps to *Java* source code](https://github.com/SwuduSusuwu/SusuLib/issues/10).

[`./build.sh`](./build.sh) does what {`./configure`, `make`} often do, can use most of [_GNU_ `make`'s options](https://maketools.com/). \[View [*Options / setup*](#options--setup) for options.\]

[`./sh/`](./sh/) is [`/bin/sh`](https://wikipedia.org/wiki/Bourne_shell) "scripts" / source code (produced for `./build.sh` and for general use):.
- [`./sh/Macros.sh`](./sh/Macros.sh) is a standalone lib for common console tasks (can do most of what [ncurses](https://wikipedia.org/wiki/Ncurses) can do). `./build.sh` and `./sh/make.sh` use this.
  - Exports functions: {`SUSUWU_CURRENT_PROJECT()`, `SUSUWU_DEFAULT_BRANCH()`, `SUSUWU_ECHO_COMMANDS()`, `SUSUWU_ESCAPE_SPACES()`, `SUSUWU_ESCAPE_QUOTED()`, `SUSUWU_LOCAL_WORKSPACE_PATH()`, `SUSUWU_PATH_AFFIX_DOTSLASH()`, `SUSUWU_PATH_SHOULD_NOT_EXIST()`, `SUSUWU_PATH_SUFFIX_SLASH()`, `SUSUWU_PATH_UNAMBIGUOUS()`, `SUSUWU_PRINT()`, `SUSUWU_SH_HAS_PARAM()`, `SUSUWU_SH_REMOVE_PARAM()`, `SUSUWU_SH_<type-of-code>()`, `SUSUWU_SH_<warn-level>()`, `SUSUWU_TEST_BASH()`}
  - Exports variables: {`SUSUWU_ABORT_ON_FIRST_ERROR`, `SUSUWU_ECHO_COMMANDS_TO`, `SUSUWU_S`, `SUSUWU_SH_CONSOLE_PARAMS`, `SUSUWU_SH_<color>`, `SUSUWU_VERBOSE`}
- [`./sh/make.sh`](./sh/make.sh) is a standalone (just imports `./sh/Macros.sh`) port of [`make`](https://wikipedia.org/wiki/Make_(software)) to `/bin/sh`. `./build.sh` uses this.
  - Exports functions: {`SUSUWU_BUILD_CTAGS()`, `SUSUWU_BUILD_OBJECTS()`, `SUSUWU_BUILD_EXECUTABLE()`, `SUSUWU_DEPENDENCY_INCLUDE()`, `SUSUWU_FIND_INCLUDE()`, `SUSUWU_INSTALL()`, `SUSUWU_INSTALL_PACKAGES()`, `SUSUWU_PROCESS_CLEAN_REBUILD()`, `SUSUWU_PROCESS_MINGW()`, `SUSUWU_PROCESS_RELEASE_DEBUG()`, `SUSUWU_SETUP_BUILD_FLAGS()`, `SUSUWU_SETUP_BUILD_FLAGS_CONDITIONAL()`, `SUSUWU_SETUP_CXX()`, `SUSUWU_SETUP_BINDIR()`, `SUSUWU_SETUP_OBJDIR()`, `SUSUWU_SETUP_OUTPUT()`, `SUSUWU_TEST_OUTPUT()`, `SUSUWU_UNINSTALL()`}
- [`./sh/Includes.sh`](./Includes.sh) {Searches for, installs, does unit tests for} system includes (popular includes such as {`libxml2`, `libtensorflow`}). Most scripts (such as `./build.sh`) which use `./sh/make.sh`, will also use `./sh/Includes.sh` (unless those just use standard **C++** includes).
  - Exports functions: {`SUSUWU_INCLUDES_LIBPUGIXML()`, `SUSUWU_INCLUDES_LIBXML2()`}
- [`./sh/Transcode.sh`](./sh/Transcode.sh) is a standalone [`ffmpeg`](https://github.com/FFmpeg/FFmpeg)-based tool which goes with [`./posts/TranscodeMuxHowto.md`](./posts/TranscodeMuxHowto.md).

[`./c/`](./c/) is [_C_](https://wikipedia.org/wiki/C_(programming_language)) source code (produced for [`./posts/`](./posts/) and for general use):
- [`./c/rfc6234/`](./c/rfc6234) is [vendored code](./.gitattributes) (direct from the official [_RFC6234_](https://www.rfc-editor.org/rfc/rfc6234#section-8)), which is used for {`classSha128()`, `classSha256()`, `classSha512()`}.
- TODO: [issue #3 produce analogous _C_ versions of source code](https://github.com/SwuduSusuwu/SusuLib/issues/3) which you can [contribute to](#how-to-contribute), or can request that more resources go to this issue

[`./cxx/`](./cxx/) is [_C++_](https://wikipedia.org/wiki/C++_(programming_language)) source code (produced for [`./posts/`](./posts/) and for general use):
- [`./cxx/Macros.hxx`](./cxx/Macros.hxx) is
  - macros with wrap C++ features/attributes, such as {`SUSUWU_ASSUME`, `SUSUWU_ATOMIC`, `SUSUWU_CONSTEXPR`, `SUSUWU_DEFAULT`, `SUSUWU_DELETE`, `SUSUWU_EXPECTS`, `SUSUWU_ENSURES`, `SUSUWU_FINAL`, `SUSUWU_IF_CPLUSPLUS`, `SUSUWU_INTPTR`, `SUSUWU_NOEXCEPT`, `SUSUWU_NORETURN`, `SUSUWU_NULLPTR`, `SUSUWU_OVERRIDE`, `SUSUWU_STATIC_ASSERT`, `SUSUWU_UNREACHABLE`} which (if used on old compilers, or with options such as `-std=c++11`) are replaced with no-ops or alternatives which have the same use,
  - macro options (which control the macro constants/macro functions). (View [*Options / setup*](#options--setup) for options),
  - macro constants, such as `SUSUWU_SH_<color>` (`color` = {`DEFAULT`, `BLACK`, `DARK_GRAY`, `RED`, `LIGHT_RED`, `GREEN`, `LIGHT_GREEN`, `BROWN`, `YELLOW`, `BLUE`, `LIGHT_BLUE`, `PURPLE`, `LIGHT_PURPLE`, `CYAN`, `LIGHT_CYAN`, `LIGHT_GRAY`, `WHITE`}, if supported, expands to the [_ANSI_ color](https://wikipedia.org/wiki/Ansi_color) codes, else expands to ""),
  - macro functions, such as {`SUSUWU_ERROR`, `SUSUWU_WARNING`, `SUSUWU_INFO`, `SUSUWU_SUCCESS`, which use `SUSUWU_PRINT`}, `SUSUWU_PRINT` (if `__cplusplus`, uses `SUSUWU_CERR`, else uses `SUSUWU_STDERRR`),
  - `macroTestsNoexcept()` (unit tests, with return value for errors).
- [`./cxx/ClassIo.hxx`](./cxx/ClassIo.hxx) is
  - `ClassIoPath` (`PortableExecutable`'s constructor argument), `ClassIoBytecode` (`classSha2`'s input argument), `ClassIoHash` (`classSha2`'s return value)
  - modular functions to interact with filesystems {`classIoGetOwnPath()`, `classIoFopenOwnPath()`}
    - console (_Posix_ `/bin/sh` or _Windows_ `cmd``) {`classIoGetConsoleInput()`, `classIoSetConsoleInput()`, `classIoGetConsoleAttributes()`, `classIoConsoleHasAnsiColors()`}
    - strings (or streams) {`classIoEscapeStr()`, `classIoHexOs()`, `classIoHexStr()`, `classIoColoredParamOs()`, `classIoColoredParamStr()`, `classIoIsXdigit()`, `classIoHexitToNibble()`, `classIoHex2Char()`, `classIoDebugIs()`, `classIoHexIs()`, `classIoGetline()`, `classIoCheckChar()`, `classIoCheckSz()`, `classIoCheckStr()`}
- [`./cxx/ClassObject.hxx`](./cxx/ClassObject.hxx) is
  - `class Instrumentation` (port of [`java.lang.instrument.Instrumentation`](https://docs.oracle.com/javase/8/docs/api/java/lang/instrument/Instrumentation.html)), `class Class : public Instrumentation` (port of [`java.lang.Class`](https://docs.oracle.com/javase/8/docs/api/java/lang/Class.html)), `class Object : public Class` (port of [`java.lang.Object`](https://docs.oracle.com/javase/8/docs/api/java/lang/Object.html)),
  - `classObjectTests()`, or `classObjectTestsNoexcept()` (unit tests with exceptions for errors, or return value for errors).
- [`./cxx/ClassPortableExecutable.hxx`](./cxx/ClassPortableExecutable.hxx) is
  - `class PortableExecutable : public Object` (stores file `path` and/or `bytecode` and/or `hex`code. TODO; `hash`?)
  - `class PortableExecutableBytecode : public PortableExecutable` loads `bytecode` from `path`. TODO; `hash`?
- [`./cxx/ClassSys.hxx`](./cxx/ClassSys.hxx) is
  - typedefs {ClassSysMuSeconds}
  - globals {classSysArgc, classSysArgs}
  - modular functions to interact with:
    - own process (`$0`) {`classSysInit()`, `templateCatchAll()`}
    - the OS {`classSysMuSecondClock()`, `execvesFork()`, `execvexFork()`, `execves()`, `execvex()`, `classSysHasRoot()`, `classSysSetRoot()`, `classSysKernelCallback()`, `classSysKernelSetHook()`}
    - TODO: internet (`socket`, `Winsock2`).
  - `classSysTests()`, or `classSysTestsNoexcept()` (unit tests with exceptions for errors, or return value for errors).
- [`./cxx/ClassNumeral.hxx`](./cxx/ClassNumeral.hxx) is to numerals (such as `int`s and `float`s) as `./cxx/ClassResultList.hxx` is to substrs (such as `std::string` and `std::stringview`):
  - Functions of scalars to new scalar values; `numeralRamp()`.
  - Functions of vectors / tensors to scalar values; `numeralAverage()`, `numeralMagnitude()`, `numeralBalancedMagnitude()`.
  - Structures which store those values; `struct NumeralNormalizers`, `struct NumeralNormalizersReciprocal`.
  - Functions which use those to transform vectors / tensors; `numeralNormalization()`, `numeralDenormalization()`.
- [`./cxx/ClassSha2.hxx`](./cxx/ClassSha2.hxx) is
  - the `classSha2` function pointer, which defaults to `classSha256()` (but you can set `classSha2 = sha128;` or `classSha2 = sha512;`), wrapped around official _RFC6234_ code. `./cxx/ClassResultList.hxx`, `./cxx/VirusAnalysis.cxx` and `./cxx/AssistantCns.cxx` all use `classSha2`.
  - `classSha2Tests()`, or `classSha2TestsNoexcept()` (unit tests with exceptions for errors, or return value for errors).
- [`./cxx/ClassCns.hxx`](./cxx/ClassCns.hxx) is `class Cns : public Object` (abstract neural system class with pure virtuals.) [Issue #6](https://github.com/SwuduSusuwu/SusuLib/issues/6) is to implement this class.
  - `classCnsTests()` (template for unit tests, which subclasses shall use).
- [`./cxx/ClassTensorFlowCns.hxx`](./cxx/ClassTensorFlowCns.hxx) is `class TensorFlowCns : public Cns` (implements `./cxx/ClassCns.hxx`). This will close [issue #6](https://github.com/SwuduSusuwu/SusuLib/issues/6) if [_TensorFlow_](https://github.com/tensorflow/tensorflow) passes `./build.sh` for all [supported systems](#how-to-use-this).
- [`./cxx/ClassResultList.hxx`](./cxx/ClassResultList.hxx) is
  - `class ResultList : public Object` (holds `hashes`, `signatures`, `bytecodes`); `resultList*()` functions {`resultListDumpTo()`, `resultListLoadFrom()`, `resultListProduceHashes()` (`virusAnalysisTests()` uses this)}.
  - `enum ListFormat { listFormatInitializer /* style: C or C++ */, listFormatJson /* style: Java or JavaScript */ };` format to store to (or load from) disk.
  - modular template (can use on all containers such as `std::vector`, `std::map` or `std::list`) `list*()` functions (such as `listMaxSize()`, `listDumpTo()`, `listLoadFrom()`, `listToHashes()`, `listIntersections()`, `listsIntersect()`, `listFindValue()`, `listHasValue()`, `listFindSubstr()`, `listHasSubstr()`, `listProduceSignature()` (`produceAbortListSignatures` uses this), `listFindSignatureOfValue()`, `listHasSignatureOfValue()` (`signatureAnalysis()` uses this), `explodeToList` (`./cxx/AssistantCns.cxx` uses this),
  produce unique signature, compare file against list of signatures), most of which were produced for antivirus signature analysis.
  - `classResultListsTests()`, or `classResultListsTestsNoexcept()` (unit tests with exceptions for errors, or return value for errors).
- [`./cxx/ClassWebBrowse.hxx`](./cxx/ClassWebBrowse.hxx) is
  - `classWebBrowseWget()`, `classWebBrowseProcessUrls()`
  - `classWebBrowseTests()`, or `classWebBrowseTestsNoexcept()` (unit tests with exceptions for errors, or return value for errors).
- [`./cxx/VirusAnalysis.hxx`](./cxx/VirusAnalysis.hxx) is
  - store / load functions {`virusAnalysisInit()`, `virusAnalysisDumpTo()`, `virusAnalysisLoadFrom()`}. Allows hosts to produce databases which clients use.
  - modular helper functions {`produceAbortListSignatures()` (for `signatureAnalysis()` use), `importedFunctionsList()` (work-in-progress, `staticAnalysis()` uses this), `straceOutputsAnalysis()` (work-in-progress, `sandboxAnalysis()` uses this), `produceAnalysisCns()` (for `cnsAnalysis()` use), `produceVirusFixCns()` (for `cnsVirusFix()` use)},
  - kernel hook function (`virusAnalysisHook()`), which uses `virusAnalysis()` to scan new downloadss (or scan all programs which execute); work-in-progress.
  - modular scan functions {`hashAnalysis()`, `signatureAnalysis()`, `staticAnalysis()` (processes [_Executable and Linkable Format_](https://wikipedia.org/wiki/Executable_and_Linkable_Format) or [_Portable Executable_](https://wikipedia.org/wiki/Portable_Executable)s, work-in-progress), `sandboxAnalysis()` (executes with `strace` + `chroot`, work-in-progress), `cnsAnalysis()` (uses `ClassCns.hxx`)} plus disinfection function (`cnsVirusFix()`) which form an antivirus program.
  - `virusAnalysisTests()`, or `virusAnalysisTestsNoexcept()` (unit tests with exceptions for errors, or return value for errors).
- [`./cxx/AssistantCns.hxx`](./cxx/AssistantCns.hxx) is
  - modular functions {`assistantCnsDownloadHosts()` (uses `wget` on `assistantCnsDefaultHosts`), `assistantCnsProcessXhtml()` (uses the next 2 functions to process `wget`'s downloads: `assistantCnsProcessUrls` (uses `boost/property_tree/xml_parser.hpp` to extract new URLs), `assistantCnsProcessQuestion` (work-in-progress, extracts question), `assistantCnsProcessResponses()` (work-in-progress, extracts answers)), `produceAssistantCns()` (uses datasets for backpropagation), `assistantCnsProcess` (uses forwardpropagation to answer new questions)} which form an assistant.
  - `assistantCnsTests()`, or `assistantCnsTestsNoexcept()` (unit tests with exceptions for errors, or return value for errors).
- [`./cxx/main.hxx`](./cxx/main.hxx) is `SusuwuUnitTestsBitmask main()` (executes all of those `*TestsNoexcept()` unit tests into a bitmask return value.)
- All have [issues](https://github.com/SwuduSusuwu/SusuLib/issues) which you can [contribute to](#how-to-contribute), or can request that more resources go to).

[`./hooks/`](./hooks) is `git` scripts ([`man githooks`](https://git-scm.com/docs/githooks)) which assist you; install with `cp -ra ./hooks/* ./.git/hooks/`.
- [`./hooks/post-checkout`](./hooks/post-checkout) is custom `post-checkout` (produces `./tags`.)
- [`./hooks/post-merge`](./hooks/post-merge) is custom `post-merge` (produces `./tags`.)
- [`./hooks/pre-commit`](./hooks/pre-commit) is [custom `pre-commit`](https://git-scm.com/book/en/v2/Customizing-Git-Git-Hooks) (`git add ./tags` if tracked, + `SUSUWU_GITHUB_WORKSPACE_JSON && git add ./compile_commands.json`) + [`.git/hooks/pre-commit.sample` (scans for non-ASCII filenames, conflict markers or whitespace errors)](https://github.com/auth0/gitzero/blob/master/tests/example/_git/hooks/pre-commit.sample)
- [`./hooks/pre-rebase`](./hooks/pre-rebase) is [custom `pre-rebase`](https://git-scm.com/book/en/v2/Customizing-Git-Git-Hooks) (restores `./tags` to avoid `error: cannot rebase: You have unstaged changes.`)
- [`./hooks/prepare-commit-message`](./hooks/prepare-commit-message) is [custom `prepare-commit-msg`](https://git-scm.com/book/en/v2/Customizing-Git-Git-Hooks) (`git add ./tags` if tracked, + `SUSUWU_GITHUB_WORKSPACE_JSON && git add ./compile_commands.json`.)

[`./compile_commands.json`](./compile_commands.json) is [`clang-tidy`'s compilation database](https://clang.llvm.org/docs/JSONCompilationDatabase.html#build-system-integration). Included for tools which execute before `./build.sh` (which produces this).

[`./tags`](./tags) is [an _IntelliSense_ alternative for editors such as `vim`](https://vi.stackexchange.com/a/45044) (stores data for [autocompletion use](https://vim.fandom.com/wiki/Any_word_completion), or to [jump to declarations through tags](https://dev.to/iggredible/how-to-use-tags-in-vim-to-jump-to-definitions-quickly-2g28)). Included for systems without [`ctags`](https://github.com/universal-ctags/ctags?tab=readme-ov-file#universal-ctags) (which produces this).

# How to use this
## Minimum requirements to use `./sh/` scripts:
- Operating systems: All which fall under [_POSIX_](https://wikipedia.org/wiki/POSIX) and / or [_UNIX_](https://wikipedia.org/wiki/Unix-like). Also [_Microsoft Windows_](https://wikipedia.org/wiki/Microsoft_Windows) (with [_Cygwin_](https://wikipedia.org/wiki/Cygwin) or [_Windows Subsystem for Linux_](https://wikipedia.org/wiki/Windows_Subsystem_for_Linux)).
- Languages: Minimum [`/bin/sh`](https://wikipedia.org/wiki/Bourne_shell). Can also use derivatives (such as [`/bin/bash`](https://wikipedia.org/wiki/Bash_(Unix_shell))).
## Minimum requirements to use `./cxx/` source code (systems which allow to use this `./build.sh`):
- Operating systems: [_Microsoft Windows_](https://wikipedia.org/wiki/Microsoft_Windows) (with [_MinGW_](https://wikipedia.org/wiki/MinGW) or [_Mingw-w64_](https://wikipedia.org/wiki/Mingw-w64)), [_Linux_](https://wikipedia.org/wiki/Linux) (such as _Android_ or _Ubuntu_), [_Unix_](https://wikipedia.org/wiki/Unix-like) (such as _BSD_, _Solaris_ or _Mach_/_OSX_), or [_iOS_](https://wikipedia.org/wiki/iOS).
- Languages: Minimum [_C++11_](https://gcc.gnu.org/projects/cxx-status.html#cxx11) (all `CXX` with `201102 <= __cplusplus`,) due to use of `auto`, `class { bool defaultMemberInit = true; };`, `decltype`, `for(value: list) {}`).
  - Other than those 4, most non-[_C++98_](https://gcc.gnu.org/projects/cxx-status.html#cxx98) features were replaced with [`./cxx/Macros.hxx`](./cxx/Macros.hxx) macros (which turn into no-ops if the compile doesn't support those), such as: [`constexpr`, `default`, `final`, `__func__`, `override`, `noexcept`, `nullptr`, `static_assert`](https://gcc.gnu.org/projects/cxx-status.html#cxx11), [`[[no_unique_address]]`](https://gcc.gnu.org/projects/cxx-status.html#cxx20).
  - \[Notice: update [`./c/README.md#progress`](./c/README.md#progress) if you update this list.\]
  - If _C99_ support would have use; ask for this (in [issue #3](https://github.com/SwuduSusuwu/SusuLib/issues/3)), or [contribute](#contributor-conventions--rules).
  - If _C++98_ support would have use; ask for this (in [issue #20](https://github.com/SwuduSusuwu/SusuLib/issues/20)), or [contribute](#contributor-conventions--rules).
## Download
Download source with `git clone https://github.com/SwuduSusuwu/SusuLib.git`. If this does not have all the tools you want, you can opt-in to [beta tests](#beta-tests--preview-builds) with `git switch preview` (opt-out with `get switch trunk`).
## Signatures / certificates
[`./.ssh/setup.sh`](./.ssh/setup.sh) is to setup `gpg.ssh.allowedSignersFile` (allows to use `git verify <ref>` or `git log --show-signature`).
- `git verify <ref>` or `git log --show-signature` shall match [`./.ssh/sha256.sig`](./.ssh/sha256.sig) for [new commits](https://github.com/SwuduSusuwu/SusuLib/commit/1cf751c10ee99dc3ed84e8dc7761358545486550#diff-f6ed156e4bf5c791680662464b94ea5d753f219ee816b385f67870e2c0d7d4c7)
- You can compare those certificates to [our blog post](https://swudususuwu.substack.com/p/githubcomswudususuwususulibgit-certificate-new) ([new scheduled](https://swudususuwu.substack.com/p/s256_1ywl0ridf6zuex1qazgx1ihgdytfkyf9t97gonfroio)).

\[Notice: This [public crypto](https://docs.gitlab.com/ee/user/project/repository/signed_commits/ssh.html#verify-commits-locally) "signature", is not related to "signature analysis" ([Substr scans](#purposes)).\]
## Options / setup
Usage: [`./build.sh [OPTIONS]`](./build.sh) produces objects (`./obj/*.o`, for distribution into other tools,) plus [_Executable and Linkable Format_](https://wikipedia.org/wiki/Executable_and_Linkable_Format) (`./bin/Susuwu.out`, to do examples/[unit tests](https://wikipedia.org/wiki/Unit_test#Agile) which prove how effective functions execute,) both of which you can redirect with `export OBJDIR=___` (or `export BINDIR=___`.)
- [`./cxx/main.hxx`](./cxx/main.hxx) has constants to use to interpret `Susuwu.out`'s return values.
- Console **options** (`./build.sh OPTIONS`): close to [_GNU_ `make`'s](https://www.gnu.org/software/make/manual/html_node/Options-Summary.html);
  - none : Defaults to `./build.sh --debug`. For all source code, if intermediate object doesn't exist or is older than source, builds source.
  - `--clean` : removes intermediate object files + exits; to reduce disc use.
  - `--rebuild` : removes intermediate object files + continues; to rebuild with new flags (or if `./build.sh` doesn't rebuild code which includes updated headers).
  - `--debug` : around 6.2 megabyte executable. Includes [frame-pointers](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#index-fomit-frame-pointer)/debug symbols (`-g`, which also includes [`-funreachable-traps`](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#index-funreachable-traps)), includes `valgrind`-replacement tools (such as `-fsanitize=address`), optimizes with [`-Og`](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#index-Og).
  - `--release` : around 322 kilobyte executable. Excludes `--debug`; strips [frame-pointers](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#index-fomit-frame-pointer)/symbols, [`-DNDEBUG`](https://en.cppreference.com/w/c/error/assert), optimizes with [`-O2`](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#index-O2).
  - `--mingw` : can mix with `--release` or `--debug`. Produces [_Portable Executable_](https://wikipedia.org/wiki/Portable_Executable) (`./bin/Susuwu.exe`), for _Windows_.
  - `--abort-on-first-error`, [`-S`, `--no-keep-going`, `--stop`](https://www.gnu.org/software/make/manual/html_node/Options-Summary.html#index-_002d_002dstop) : sets [`SUSUWU_ABORT_ON_FIRST_ERROR=true`](./sh/Macros.sh), which causes [`SUSUWU_BUILD_OBJECTS`](./sh/make.sh) to `exit 1` if an object fails to build (if `${CC}`] or `${CXX}` do not return `true`). Default is to continue if possible.
  - `-s`, `--silent`, `--quiet` or [`--debug=n`](https://www.gnu.org/software/make/manual/html_node/Options-Summary.html#index-_002d_002ddebug) : disables [`SUSUWU_PRINT`](./sh/Macros.sh) for [`$(SUSUWU_SH_NOTICE)`](./sh/Macros.sh). Disables [`SUSUWU_ECHO_COMMANDS`](./sh/Macros.sh).
  - , `-v`, `--verbose`, [`-d` or `--debug=a`](https://www.gnu.org/software/make/manual/html_node/Options-Summary.html#index-_002dd) : enables [`SUSUWU_PRINT`](./sh/Macros.sh) for [`$(SUSUWU_SH_DEBUG)`](./sh/Macros.sh). Has [`SUSUWU_ECHO_COMMANDS`](./sh/Macros.sh) use `set -x`.
  - `--abort-on-first-error`: sets [`SUSUWU_ABORT_ON_FIRST_ERROR=true`](./sh/Macros.sh), which causes [`SUSUWU_BUILD_OBJECTS`](./sh/make.sh) to `exit 1` if a subbuild ([`${CC}`](./build.sh) or [`${CXX}`](./build.sh)) fails.
- Environment flags: as [_GNU_ `make`'s](https://www.gnu.org/software/make/manual/make.html#Implicit-Variables), plus;
  - `export SUSUWU_SH_TPUT_COMMAND=<path>`; replaces calls to `tput` with `<path>` (for instance, with `no-such-command` to test that `SUSUWU_SH_COLOR_COUNT()` does not require `ncurses-utils`).
- Macro flags (use `vim build.sh` to put into `FLAGS_USER`). If `=true`, most use more resources, except `SUSUWU*PREFER_*` or `SUSUWU*SKIP_*`. "default is `=!defined(NDEBUG)`" is short for; "if `--debug`, default `=true`, but if `--release`, default `=false`".
  - `-DSUSUWU_UNIT_TESTS[=true|=false]` with `=true` to build + execute unit tests. Default is `=true`, but more stable future version could have default `=!defined(NDEBUG)`. If set to `=false`; compilation time, object size, execuable size reduced (to around half).
  - `-DSUSUWU_HEX_DOES_PREFIX=true` to have `classIoHex*()` insert/remove "0x". Default is `=false` (caller must do).
  - `-DSUSUWU_LIST_COUNT` to have `listDumpTo()` prefix the list count (which allows verification through `listLoadFrom()`. Default undefined.
  - `-DSUSUWU_INHERIT_GSL_OWNER=true` to disable `cxx/ClassIo.hxx`'s definition of `gsl::owner<>` if it conflicts with [**GSL**](https://github.com/microsoft/GSL). Default is undefined.
  - `-DSUSUWU_CNS_VALUE_SEMANTICS=false` to have `class Cns` inherit `Object::equals()` and `Object::hashCode()` (comparison of addresses). Default is `=true`; `class Cns` overrides `Object::equals()` and `Object::hashCode()` (so that those do comparison of values).
  - Custom `sh` (console) output:
    - `-DSUSUWU_SH_PREFER_STDIO=true` to replace `std::cXXX << ...` with `fprintf(stdXXX, ...)`; default is `=!defined(__cplusplus)`.
    - `-DSUSUWU_SH_VERBOSE[=true|=false]` with `=true` to print diagnostic messages (`SUSUWU_SH_USE_FILE`, `SUSUEU_SH_USE_LINE`, `SUSUWU_NOTICE`, `SUSUWU_DEBUG`, `SUSUWU_DEBUGEXECUTE`, `SUSUWU_NOTICE_EXECUTE`, `SUSUWU_DEBUG_EXECUTE` all use `#if SUSUWU_SH_VERBOSE`); default is `=!defined(NDEBUG)`.
    - `-DSUSUWU_SH_SKIP_BRACKETS=true` sets output format to `WARN_LEVEL: message`; default is `=false`.
    - `-DSUSUWU_SH_FILE=true` sets output format to `[__FILE__: WARN_LEVEL: message]`; default is `=SUSUWU_SH_VERBOSE`.
    - `-DSUSUWU_SH_LINE=true` sets output format to `[__LINE__: WARN_LEVEL: message]`; default is `=SUSUWU_SH_VERBOSE`.
    - `-DSUSUWU_SH_FUNC=true` sets output format to `[__func__: WARN_LEVEL: message]`; default is `=false`.
    - `-DSUSUWU_SH_SKIP_COLORS=true` to omit _VT100_ (_ANSI_) colors; default is `=defined(SUSUWU_SH_COLORS_UNSUPPORTED)`).
    - `-DSUSUWU_SH_SKIP_COLORS=false` to force (unless `TERM` is blacklisted) _VT100_ ([_ANSI_ color](https://wikipedia.org/wiki/Ansi_color)) use.
    - TODO (for now, no effect; once [issue #17](https://github.com/SwuduSusuwu/SusuLib/issues/17) is closed, you can use):
      - `-DSUSUWU_SH_RUNTIME_OSC` to replace `#ifdef _POSIX_VERSION\nAccessClipboard();\n#endif` with `termcmp`./`GetConsoleMode()` (for choices on whether or not to use Operating System Commands); default is undefined.
      - `-DSUSUWU_SH_RUNTIME_COLORS` to replace `#if _POSIX_VERSION\nColors();\n#endif` with `termcmp`./`GetConsoleMode()` (for choices on whether or not to use colors); default is undefined.
  - To match `g++`./`clang++` console format, use `-DSUSUWU_SKIP_BRACKETS=true, -DSUSUWU_SH_FILE=true, -DSUSUWU_SH_LINE=true, -DSUSUWU_SH_FUNC=false` (sets output format to `__FILE__:__LINE__: WARN_LEVEL: message`).
  - Unstable/`preview` flags:
    - `-DSUSUWU_CNS_LOCAL_COEFFICIENTS=true` to have derivatives of `class Cns` store the connectome as part of the class, even if external libs (such as [_TensorFlow_](https://github.com/tensorflow/tensorflow)) also store the connectome.
    - `-DSUSUWU_TENSORFLOW_HAS_DATATYPETOENUM=true` to use [`tensorflow::DataTypeToEnum`](https://github.com/tensorflow/tensorflow/issues/30828#issuecomment-3039819975). Default is `=false` (implement `Susuwu::DataTypeToEnum`.)
    - `-DSUSUWU_EXPERIMENTAL` to enable `preview` (more new, but unfinished/unstable) versions of code; default is unset, unless `git switch preview` is executed.
      - `-DSUSUWU_DEFAULT_BRANCH` if errors, suggest `git switch SUSUWU_DEFAULT_BRANCH`; default is "trunk".
    - `-DSUSUWU_USE_PUGIXML` to use [`libpugixml`](https://github.com/zeux/pugixml) to parse [**XML**](https://wikipedia.org/wiki/XML) / [**DOM**](https://wikipedia.org/wiki/Document_Object_Model); default is unset.
      - Improves [`./cxx/ClassWebBrowse.cxx`](./cxx/ClassWebBrowse.cxx). Improves sections of ([`./cxx/AssistantCns.cxx`](./cxx/AssistantCns.cxx) which use [**XHTML**](https://wikipedia.org/wiki/XHTML)'s **DOM** for `class Cns::setupSynapses`).
    - `-DSUSUWU_USE_LIBXML2` to use [`libxml2`](https://github.com/GNOME/libxml2) to parse [**XML**](https://wikipedia.org/wiki/XML) / [**DOM**](https://wikipedia.org/wiki/Document_Object_Model); default is unset.
      - Improves [`./cxx/ClassWebBrowse.cxx`](./cxx/ClassWebBrowse.cxx). Improves sections of ([`./cxx/AssistantCns.cxx`](./cxx/AssistantCns.cxx) which use [**XHTML**](https://wikipedia.org/wiki/XHTML)'s **DOM** for `class Cns::setupSynapses`).
    - `-DSUSUWU_CNS_SEPARATE_NORMS=true` causes [`class Cns`](./cxx/ClassCns.hxx) (plus subclasses) to compute separate [normalization factors](../cxx/ClassNumeral.hxx) for input versus output values. Default `=false`; reuse input factors as output factors.
    - `-DSUSUWU_VIRTUAL_OPERATORS_USE_VPTRS=false`: [`./cxx/ClassObject.hxx`](./cxx/ClassObject.hxx):`Class::operator==(const Class &obj) { return this->hasLayoutOf(obj) && 0 == memcmp(sizeof(NULL) + (char *)this, sizeof(NULL) + (char *)&obj, this->getObjectSize() - sizeof(NULL)); }`, thus `Susuwu::Object() == Susuwu::Class()` but `CXX` output with nonstandard `vptr` layout crashes. Default `=true`; (`return typeid(this) == typeid(obj) && 0 == memcmp(this, *obj, this->getObjectSize());`).
    - `-DSUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES=false`: to use [`./cxx/ClassObject.hxx`](./cxx/ClassObject.hxx):`Object::equals(const Object &obj) { return this->operator==(obj); }`. Default is `=true` (`return this == &obj`). For now, just controls `Object::equals` (in future, perhaps `SUSUWU_VIRTUAL_OPERATORS_USE_ADDRESSES` inherits this).
    - `-DSUSUWU_HEX_TABLE=true` replaces [`classIoHexitToNibble()`](./cxx/ClassIo.hxx) (and future `classIoNibbleToHexit()`) computations with lookups through _ASCII_ maps. Replaces `isxdigit()` with `classIoHex2Nib[]`.
    - `-DSUSUWU_USE_STD_HEX=true` replaces [`classIoHex2Char()`](./cxx/ClassIo.hxx) (and future `classIoChar2Hex()`) with `std::hex`.
    - `-DSUSUWU_IO_THROW=true`; replaces `is.setstate(std::ios::failbit);` with `throw`. For now, just affects `std::istream` (and derivatives) params; don't know if `os` should have its own flag.
  - TODO (for now won't build, or has no effect):
    - `-DSUSUWU_VIRTUAL_OPERATORS_USE_ADDRESSES=true`: No effect. If implemented, `Class::operator==(const Class &obj) { return &obj == this; }`. Default is `=false`.
    - `-DSUSUWU_VIRTUAL_MEMBER_FUNCTION_TEMPLATES=true`: causes "error: 'virtual' cannot be specified on member function templates". Default is `=false`.
    - `-DSUSUWU_PREFER_CSTR=true` to replace `std::string` with `char *` (more compatible with non-C++ projects); default is `=SUSUWU_PREFER_C`.
    - `-DSUSUWU_PREFER_C=true` sets `SUSUWU_PREFER_CSTR` + `SUSUWU_SH_PREFER_STDIO` ([plus other flags which will exist to allow non-C++ projects to include this](https://github.com/SwuduSusuwu/SusuLib/issues/3); default is `=!defined(__plusplus)`).

# How to contribute
View [documented issues](https://github.com/SwuduSusuwu/SusuLib/issues/) (for ideas on code to contribute, plus so you do not report documented issues.)
## Beta tests / `preview` builds
`git branch`s:
- [`trunk`](https://github.com/SwuduSusuwu/SusuLib/tree/trunk) publishes what most projects call "releases", with changes limited to what [*SemVer* calls "Patch" revisions](https://semver.org/spec/v2.0.0.html#spec-item-6)
- [`new`](https://github.com/SwuduSusuwu/SusuLib/tree/new) publishes what most projects call "release candidates", with changes limited to what [*SemVer* calls "Minor" revisions](https://semver.org/spec/v2.0.0.html#spec-item-7)
- [`preview`](https://github.com/SwuduSusuwu/SusuLib/tree/preview) publishes what most projects call "beta tests", with changes up to what [*SemVer* calls "Major" revisions](https://semver.org/spec/v2.0.0.html#spec-item-8)

How to use the most new sources:
- `git switch preview && ./build.sh`
  - View results for symptoms of new issues (hint: look for "Warning:"s or "Error:"s).
  - If you found new issue(s) (which aren't due to misconfigurations in your system), [post new issue(s)](https://github.com/SwuduSusuwu/SusuLib/issues/new).
    - Notice: [sensitive issue(s) have this separate mechanism to use](./SECURITY.md#sensitive-issues).

# Contributor conventions / rules
General comment/message syntax rules:
- If on its own line (alone) the comment refers to the next line. If the next line starts a block of code (such as a function), the comment refers to that whole block of code.
- If at the end of a line (which has code) the comment refers to that code.
- Parenthesis which indicates function arguments (such as `func(arg)` shall not have whitespace prefix.
- Parenthesis which is used in the traditional English sense (such as this, which gives more information about the preceding natural-language text) shall have whitespace prefix.
- `<>` goes around type of option/argument (such as `<commit-hash>`, `[]` goes around optional comments/options/arguments (such as `[<optional fallback value>]`, `...` is affixed to allow multiple options/arguments (such as `[; optional extra arguments]...`). This rule is used to document function arguments (such as `sh`, `C` or `C++` use), plus to document `git` uses.
To ensure consistent code, submissions of code (such as through [pull requests](https://github.com/SwuduSusuwu/SusuLib/pulls)) have language-specific syntax rules:
## _Markdown_
`` *.md `` shall use:
- [_GitHub flavored Markdown_](https://github.github.com/gfm/), which is not just compatible with [_GitHub_](https://github.com) but also:
  - Has numerous [unit tests](https://wikipedia.org/wiki/Unit_test#Agile). Most of the differences from the original _Markdown_ are just so rules are less ambiguous.
  - Is close to the original _Markdown_ (thus compatible with most _Markdown_ tools, such as [`glow`](https://github.com/charmbracelet/glow?tab=readme-ov-file#glow)).
- [ISO 8601](https://wikipedia.org/wiki/ISO_8601), which
  - Is the most popular national standard format.
  - Versus formats which use locale-dependent names of months, is more portable and less ambiguous.
  - Versus formats which use backslashes, is more portable (filesystem paths can include).
- [_Unix_](https://wikipedia.org/wiki/Unix) paths start with `./` (if relative) or `/` (if absolute), so that [`sed`](https://manpages.org/sed) (and [`grep`](https://manpages.org/grep)) [performance is improved](https://poe.com/s/NX7kVKtCL9k04WIqieoh).
  - That is, paths shall match the [Regular Expression](https://wikipedia.org/wiki/Regular_expression) `^\.*\/[\w]*` (more than 1 `.` is allowed).
  - [_Microsoft Windows_](https://wikipedia.org/wiki/Microsoft_Windows) can use _Unix_ paths, except that absolute paths must start with the drive prefix (`[A-Z]:/` versus `/`).
  - [_HTTP_](https://wikipedia.org/wiki/HTTP) can use _Unix_ paths, except that absolute paths must start with the protocol (`http[s]*://` versus `/`).

## `git`
If `git commit` introduces/removes functions, have `./README.md#purposes` include this.
Do ["atomic" / granular commits](https://poe.com/s/lnG5yXNXUTl0m8muGjav): if swapping the new commit with a previous commit (such as through `git rebase -i`) --- or if `git revert` of a previous commit --- causes  `./build.sh` to return a non-0 exit status, `git commit`'s message shall include such as:
> Is followup to: \<ref | commit-hash\> \(\<commit-message\>\)\[, comment\] \[; \<ref | commit-hash\> \(\<commit-message\>\)\[, comment\]\]...

- This shows the temporal order of commits required for `./build.sh` to pass.
- `<commit-message>` is so that `git rebase` (which changes `<commit-hash>`) does not make it impossible to follow (plus, so comments are reduced), thus you should use the exact message. You can use ellipsis (`...`) to omit extra lines, but it is best if the first line is exact (left as-is).
- Notice: [commit 9eda0ed5ed2abcdcec92c5b265f6e950e1196558 (+`sh/Macros.sh:SUSUWU_SH_{FILE, LINE, FUNC}`)](https://github.com/SwuduSusuwu/SusuLib/commit/9eda0ed5ed2abcdcec92c5b265f6e950e1196558/), and older, used `,` (as opposed to `;`) to delimit the list of commits; those extra `<ref | commit-hash>`'s are not extra comments, but are extra commits. The new format allows comments to include `<commit-hash>`'s plus `,`'s (just not `;`'s).

`git commit` message format/syntax:
- prefix "./" onto relative paths (such as `./README.md`), so those are not confused with *DNS references* (nor with shortcuts for *Uniform Resource Locators*). Exception: since `git commit` limits `COMMIT_EDITMSG`'s first row to `size() <= 50`, the first row can skip "./".
- affix "()" onto functions (regardless of number of arguments), such as `function()`, or use the function name (such as `function`) alone.
- if commit does `git add NewFile`: message has `` +`NewFile` ``.
- if `git rm Exists`: `` -`Exists` ``.
- if `touch Exists && git add Exists`: `` @`Exists` `` or `` ?`Exists` ``.
  - Simple wildcards/regex for altered functions: `` `%s/oldFunction()/newFunction()/` ``.
  - `*` is not used as update prefix, since `*` has much other uses: for _Regular Expressions_ (wildcards), plus for _C++_ (such as block comments, dereferences, or math).
    - From the root commit through 159940fb8b60b176a38a13cdfbd9393596daa9b5 (Date:   Thu Jul 4 07:56:01 2024 -0700), '@' was the prefix for updates. From then until this commit, '?' was the prefix for updates.
    - From this commit on (this is the successor to commit 0ae6233c02d9e04fca60027b1e32b885eb69bb8a (Date:   Sat Nov 30 17:50:40 2024 -0800)), '@' is (once more) the prefix for updates, due to: it is more common for projects to so use '@'.
- if `echo "int newFunction() {...}" >> Exists && git add Exists`: `` @`Exists`: +`NewFunction()` ``.
- if `git mv OldPath/ NewPath/`: `` `OldPath/` -> `NewPath/` `` or `` `mv OldPath/ NewPath/` ``.
- as default branch, choose `master`, `main` or `trunk` (do not have more than 1 of those branches, or [`./Macros.sh:SUSUWU_DEFAULT_BRANCH()`](./Macros.sh) is ambiguous).
- to indent: use tabs to form blocks, such as:
```markdown
@`README.md`:
	@`#How-to-use-this`:
	Split into:
		@`## Download`: new; howto clone, howto switch branches.
		@`## Optionssetup`: "Options/setup"; howto use `./build.sh` (with or without options.)
	@`#How-to-contribute`,
	@[Good first issues to contribute to]: (moved into `#How-to-contribute`)
```

\[Notice: Commit titles can omit backticks (``) if not enough room; the backticks just allow _GitHub_ to do _Markdown_-format code / paths.\]

## `sh` source
Is as for [_C_/_C++_ source](#cc-source), plus specifics to `sh`:
- Act as if all functions/variables are macros (which use `CONSTANT_CASE`).
- Variable access: uses `${...}` (thus not `echo $BOOL`, but `echo ${BOOL}`).
  - Rationales:
    - In case future versions append to this (`echo $BOOL2` is a silent error, but `echo ${BOOL}2` is cool).
    - Avoids [_SC2250_ "Prefer putting braces around variable references even when not strictly required."](https://www.shellcheck.net/wiki/SC2250) notices.
  - Exceptions: [**language limits**](https://www.shellcheck.net/wiki/SC3030).
    - To support `/bin/sh`: do not use `${@}`, but `$@`.
- Str variable access: uses `"$..."` (thus not `ls ${STR}`, but `ls "${STR}"`).
  - Rationales:
    - So if `STR="/bin/"` is replaced with `STR="/path with/spaces/"` ([without `IFS=""`](https://tldp.org/LDP/abs/html/internalvariables.html#IFSREF) \[[2](https://www.commandlinux.com/man-page/man1/sh.1.html#lbBK)\]), that 1 parameter [won't expand into 2](https://tldp.org/LDP/abs/html/special-chars.html#FIELDREF).
    - So if `STR="/bin/"` is replaced with `STR="*"` ([without `set -f`](https://www.commandlinux.com/man-page/man1/sh.1.html#lbBL)), the glob is passed to `ls` (which [expands this into numerous paths](https://tldp.org/LDP/abs/html/globbingref.html), rather than expanded in your script.
    - Avoids [_SC2086_ "Double quote to prevent globbing and word splitting."](https://www.shellcheck.net/wiki/SC2086) notices,
  - Exceptions: specifics of command use.
    - Do not use `ctags "${FLAGS}" "${PATH}"`, but `ctags ${FLAGS} "${PATH}"`.
- Str variable access: uses `"${...}"` (thus not `if [ "-q" = ${PARAM} ]`, but `if [ "-q" = "${PARAM}" ]`).
  - Rationales: in case `${PARAM}` has spaces.
    - Avoids [_SC2068_ "Double quote array expansions to avoid re-splitting elements."](https://www.shellcheck.net/wiki/SC2068) notices.
  - Exceptions: to split (on spaces) is the purpose of the `for` loop.
    - To parse numerous params: do not use `for VALUE in "$@"`; do`, but `for VALUE in $@; do`.
- Restrict temp variables:
  - Rationales: avoids [*shadowed variables*](https://wikipedia.org/wiki/Variable_shadowing) (such as `f2() { VALUE=false; }; f1() { VALUE=true; f2(); return VALUE; };` causes).
  - If your project will always stick to `/bin/bash` (if does not have to support _POSIX_ systems), you affix `local` (thus not `for VALUE in ${LIST}; do`, but `local VALUE; for VALUE in ${LIST}; do`) to do this.
    - Exceptions: [**language limits**](https://austingroupbugs.net/bug_view_page.php?bug_id=767).
      - `local` is [not applicable to `IFS="<delimiter>"` or `cd <path>`](https://unix.stackexchange.com/questions/493729/list-of-shells-that-support-local-keyword-for-defining-local-variables/493838#493838), [not applicable to `readonly`](https://unix.stackexchange.com/questions/506009/unable-to-have-a-local-variable-with-the-same-name-as-a-global-read-only-variabl) (so use **subshells** for this).
      - If you require code which is consistant across platforms ([`local` has inconsistant dynamic versus static scope, plus inconsistant inheritance](https://unix.stackexchange.com/questions/493729/list-of-shells-that-support-local-keyword-for-defining-local-variables/493743#493743)), use **subshells** for this.
  - If your project supports _POSIX_ (`/bin/sh`): do not use `local` (such as `f2() { local VALUE=false; }`), but [use **subshells** for this](https://stackoverflow.com/questions/18597697/posix-compliant-way-to-scope-variables-to-a-function-in-a-shell-script/64946874#64946874) (such as `f2() ( VALUE=false; )`.)
    - Rationales (other than `local`'s **language limits**):
      - Avoids [_SC3034_ "In POSIX sh, local is undefined."](https://www.shellcheck.net/wiki/SC3043) notices.
- Command variables: uses `$(...)` (thus not `` stat `pwd` `` , but `stat $(pwd)`).
  - Rationales:
    - Most simple to nest (`echo $(stat $(pwd))`). Common (much known) **subshell** syntax is reused.
    - Avoids [_SC2006_ "Use $(...) notation instead of legacy backticked ...."](https://www.shellcheck.net/wiki/SC2006) notices.
- Document unintuitive quirks
  - Most languages group `&&` before `||`, but in `/bin/sh` (plus derivatives, such as `/bin/bash`) `&&` is grouped similar to `||`, [which requires to restructure the code to use the fact that `/bin/sh` is left-associative](https://unix.stackexchange.com/questions/88850/precedence-of-the-shell-logical-operators/88851#88851).

### `git tag`
`git tag`s use *SemVer 2* ([*Semantic Versioning 2*](https://semver.org/spec/v2.0.0.html)) to mark versions (`git tag <*SemVer*> <commit-hash>`). Annotated tag messages (`git tag -m "message"`) use [*Markdown*](https://github.github.com/gfm/).

Due to concerns of use of the wrong numbers to version the `git commit`s ([*SemVer 2* says those numbers are immutable](https://semver.org/spec/v2.0.0.html#spec-item-3), `git tag` was not used on this repo until now. For now, just the first few `git commit`s have `git tags`: [`git tag -n99 0.1.0` or `git show 0.1.0 --no-patch`](https://github.com/SwuduSusuwu/SusuLib/releases/tag/0.1.0), [`0.0.1`](https://github.com/SwuduSusuwu/SusuLib/releases/tag/0.0.1), [`0.0.0`](https://github.com/SwuduSusuwu/SusuLib/releases/tag/0.0.0), which serve as examples of which `git tag` conventions to use.

Notice: [*StackOverflow* says to use the "v" or "version" prefix](https://stackoverflow.com/questions/21639437/git-flow-release-branches-and-tags-with-or-without-v-prefix), but [`dev.to` says not to](https://dev.to/davidb31/for-version-as-git-tag-use-annotated-tag-not-v-prefix-2i4c), so if scripts are used to parse the `git tag`s of this repo, those scripts should allow [optional "v"](https://semver.org/#is-v123-a-semantic-version) (which stannds for "version-") prefixes (such as future versions `v0.2.2`, `version-0.2.2` or `0.2.2`).

## _C_/_C++_ source
Linter: `apt install clang && clang-tidy cxx/*.cxx` (defaults to [`.clang-tidy`](./.clang-tidy) options).

Most of what [_Mozilla Org_'s (_Firefox_'s) style](https://firefox-source-docs.mozilla.org/code-quality/coding-style/coding_style_cpp.html) suggests is sound (you should follow this unless you have specific reasons not to).

Code rules (lots overlap with _Mozilla Org_'s):

- Indent: tabs ('^I'); as much tabs as braces ('{' = +1 tab, '}' = -1 tab). \[All which conflicts with _Mozilla Org_'s format is tab use.\]
  - Rationales: reduced memory use, allows local configs (such as `~/.vimrc`) to set width, allows arrow keys to move fast.

- Files: `${C_SOURCE_PATH}/PascalCase.*` or `${CXX_SOURCE_PATH}/PascalCase.*xx` (such as: `./cxx/ClassFoo.hxx`, used as `#include "ClassFoo.hxx" /* classFooFunction() */`), as this is most common.
  - `./build.sh` requires: that all local includes prefix as `Class*.hxx` (so it knows to execute `--rebuild` if you upgrade a common include.) TODO: incremental builds which don't require this.
  - To assist with insertion/removal of `#include` statements, comments shall list all functions/macros used.

- Structs, enums, classe: `typedef struct PascalCase {} PascalCase;`, `typedef enum PascalCase {} PascalCase;`, `typedef class PascalCase {} PascalCase;`, as this is most common.

- Macros: `#define NAMESPACE_CONSTANT_CASE(snake_case_param) assert(snake_case_param);`, as this is most common.
  - [Indent multi-level macros as `#if X # if S ,,, # endif #endif`](https://stackoverflow.com/questions/1854550/c-macro-define-indentation).
  - [\_DEBUG is specific to MSVC, thus use NDEBUG](https://stackoverflow.com/questions/2290509/debug-vs-ndebug), [Pass `-D NDEBUG` to disable asssets + enable optimizations](https://stackoverflow.com/questions/2249282/c-c-portable-way-to-detect-debug-release).
  - Do not perform tasks within `assert()`, due to: the standard says "\[`#if NDEBUG\n#define assert(x) (0)\n#endif`\]".

- Braces, functions:
  - Do not produce numerous functions with the same name but different arguments, as such "overloads" make this difficult to [port](https://github.com/SwuduSusuwu/SusuLib/issues/10).
  - Single statement blocks can use the form: `virtual bool hasInstance() { return true; }`.
  - Most common form:
    ```c++
    /* `const` prevents `if(func() = true)` where you wished for `if(func() == true)` */
    const bool classPrefixCamelCase(bool s, bool q) {
    	if(s && q) {
    		return s;
    	} else {
    		return q;
    	}
    }
    ```
- Args/params, local variables, objects: `const bool camelCase = true`; Global variables/objects: `extern const bool classFooFunction;`, as this is most common.
  - Functions/globals can omit "classFoo" if wrapped as `namespace ClassFoo { extern bool global; };`, or (for `class ClassFoo { static bool global; };`).
    - The project as a whole should have `namespace Susuwu {};`, but you can nest `namespace`s.)
- For error, warning or notice syntax use: `"[WARN_LEVEL: OPTIONAL_FUNCTION_NAME {code which triggered the error/warning/diagnostic/notice} /* OPTIONAL COMMENTS */]"`,
  - [`./cxx/Macros.hxx`](./cxx/Macros.hxx): {SUSUWU_STR(x), SUSUWU_CERR(x), SUSUWU_STDOUT(x)} have the new syntax for this.
  - Modules should choose one of this list to send all userland errors, warnings or notices to:
    - `throw std::runtime_error(message)` (or `throw Q()` where `class Q : public std::exception`)
    - `std::cerr << message` (or `fprintf(stdout, message)`
    - `errno = code;`, or `return code;`.
- Follow tools such as {`clang++`, `g++`, `vim`, _Markdown_}:
  - use [`./cxx/Macros.hxx`](./cxx/Macros.hxx)`:SUSUWU_SH_BROWN` (and / or `` `<code>` ``) to quote code (or commands).
  - use `./cxx/Macros.hxx:SUSUWU_SH_PURPLE` (and / or `"<path>"`) to quote paths.
  - use `./cxx/Macros.hxx:SUSUWU_SH_LIGHT_CYAN` to quote names of functions.
  - use `./cxx/Macros.hxx:SUSUWU_SH_RED` to quote error messages.
  - use `./cxx/Macros.hxx:SUSUWU_SH_PURPLE` to quote status codes (or return values).
  - use `./cxx/Macros.hxx:SUSUWU_SH_LIGHT_CYAN` to quote names of variables / constants.
  - use `./cxx/Macros.hxx:SUSUWU_SH_GREEN` to quote (current) arguments / values.
  - use `./cxx/Macros.hxx:SUSUWU_SH_LIGHT_PURPLE` to quote (speculative / proposed) replacement arguments / values.
  - [`.sh`](#sh-source) code uses `./sh/Macros.sh:SUSUWU_SH_<color>`; new values above should also goto [`./sh/Macros.sh`](./sh/Macros.sh).

- Comments
  - Comments about possible `return code;`s (or `throw`s) go above function declarations (_Doxygen_ convention).
    - It is arguable whether or not you should document all possible system errors; most _Standard Template Library_ functions can `throw`.
  - `*.hxx` is to document interfaces (above function declarations); `*.cxx` is to do implementations (do not duplicate interface comments).
  - Use _Markdown_. Rationales: for non-English users (or for computers), such syntax assists use.
  - Instead of `//new single-line comments`, prefer `/* old fashioned */`.
    - Rationale: simpler to port. More obvious where the comment stops if the comment wraps around.
  - [_Doxygen_-ish "@pre"/"@post" prepares for _C++26_ _Contracts_](https://github.com/doxygen/doxygen/issues/6702):
    ```c++
    /* @throw std::bad_alloc If function uses {`malloc`, `realloc`, `new[]`, `std::*::{push_back, push_front, insert}`}
     * @throw std::logic_error Optional. Would include most functions which use `std::*`
     * @pre @code !output.full() @endcode
     * @post @code !output.empty() @endcode
     */
    bool functionDeclaration(std::string input, std::deque<vector> output);
    ```
    - As soon as `clang++` (or `g++`) has _Contracts_ (part of _C++26_), regular expressions (such as `:%s/@pre (.*) @code (.*) @endcode/[[expects: \2]] \\* \1 \\*/` `:%s/@post (.*) @code (.*) @endcode/[[ensures: \2]] \\* \1 \\*/`) can convert _Doxygen_-ish comments into contracts.
    - [`./cxx/Macros.hxx`](./cxx/Macros.hxx) has `SUSUWU_ASSUME(X)`, which is close to `[[expects: x]]`, but `SUSUWU_ASSUME(X)` goes to `*.cxx`, whereas `[[expects]]` goes to `*.hxx`.
    - Advantages of `[[expects]]`: allows to move information of interfaces out of `*.cxx`, to `*.hxx`.
- `#include` guards versus `#pragma once`: use both, since [(on all compilers) `#include` guards enforce the One Definition Rule](https://wikipedia.org/wiki/Include_guard), but [`#pragma once` (on supported compilers) reduces resource use](https://learn.microsoft.com/en-us/cpp/preprocessor/once?view=msvc-170).
  ```c++
  #pragma once
  #ifndef INCLUDES_Path_To_File
  #define INCLUDES_Path_To_File
  #endif /* ndef INCLUDES_Path_To_File */
  ```

## Sponsor
To sponsor this (which allows us to produce more source codes), you can use crypto (such as [**Bitcoin**](https://wikipedia.org/wiki/Bitcoin)) to [produce a one-time-use address](https://poe.com/s/IPhIMyuMY6SnYM0yqEJl) (which you deposit funds into), and send the address&private-key to [a contact which `./SECURITY.md` lists](./SECURITY.md#sensitive-issues).
- Rather than us publish a send-to address (for a particular protocol), this allows us to accept all forms of crypto.
- If asked, will include sponsorship info into the [contributors / sponsors list](#contributors--sponsors).
- If amount is more than $100 and you don't trust the contact platforms, use [`./.ssh/id_ed25519.pub`](./.ssh/id_ed25519.pub) to [secure those](https://superuser.com/questions/576506/how-to-use-ssh-rsa-public-key-to-encrypt-a-text/1850928#1850928).
- If crypto is not suitable, you can also send gift card codes or [virtual cards](https://www.forbes.com/advisor/credit-cards/virtual-credit-card-numbers-guide/) to use.

### Escrow
If you want proof that your crypto/cash will go to produce specific systems, use [**escrow** services](https://wikipedia.org/wiki/Escrow) (what you send the **escrow** is: crypto/cash, plus contract which references an [open issue which you choose](https://github.com/SwuduSusuwu/SusuLib/issues/)).
- If none of those issues match what you want, you can [post your own issue](https://github.com/SwuduSusuwu/SusuLib/issues/new) for this.
- Ensure that the **escrow** contract includes specifics as to what will count as "issue closed" [to the **escrow** service (so you do not have to trust the author),](https://wikipedia.org/wiki/Online_dispute_resolution) which will release the crypto/cash (once the **escrow** service considers your issue as closed).
  - For example; "The **source code** (through `./build.sh`), must produce a **system** (a **shared object** or **executable**) which uses just half of the training data to [setup its neural network, which must produce virtual synapses](https://wikipedia.org/wiki/Backpropagation) which the **system** [uses to produce **accurate** results](https://wikipedia.org/wiki/Residual_neural_network#Forward_propagation) on the other half, where **accurate** (for [classifiers](https://wikipedia.org/wiki/Learning_classifier_system)) is less than 2% false negatives and less than 2% false positives, and **accurate** (for [generators](https://wikipedia.org/wiki/Generative_artificial_intelligence)) is [divergence](https://wikipedia.org/wiki/Kullback%E2%80%93Leibler_divergence) of less than 2%." is a contract which an **escrow** can use for [issue #6](https://github.com/SwuduSusuwu/SusuLib/issues/6).

### Affiliates
You can use [_Capital 1_'s affiliate program](https://i.capitalone.com/JgR02Y4pE) to allow us to produce more source codes.

## Tools used
For now, <https://github.com/SwuduSusuwu> just accepts anonymous sponsors. If thus is not suitable, sponsor tools used (which accept more popular sponsorship protocols) such as:
* *Ubuntu*: <https://documentation.ubuntu.com/project/community/contribute/contribute/> <https://ubuntu.com/download/desktop/thank-you>
* *AOSP*: <https://source.android.com/docs/setup/contribute>
  * *Termux*: <https://github.com/sponsors/termux> (<https://termux.dev/en/donate> has numerous protocols for sponsorships).
* `/bin/sh`: On most systems this uses `dash`: <https://git.kernel.org/pub/scm/utils/dash/dash.git>.
  * `g++`: <https://github.com/gcc-mirror/gcc?tab=contributing-ov-file> says browse to <https://gcc.gnu.org/contribute.html>.
  * `clang++`: <https://github.com/llvm/llvm-project?tab=contributing-ov-file> says browse to <https://llvm.org/docs/Contributing.html>.
  * `java`: <https://github.com/openjdk/jdk?tab=contributing-ov-file> says browse to <https://dev.java/contribute/openjdk/> <https://openjdk.org/guide/>.
  * `git`: <https://github.com/git/git?tab=contributing-ov-file> says browse to <https://git-scm.com/community#contributing>.

## Contributors / sponsors
This section will show contributors (of code, or sponsors) who request inclusion into the contributors / sponsors list.
* Since published, `SusuLib` has no follows, no sponsors. But the producers of [tools used](#tools-used) are indirect contributors.

