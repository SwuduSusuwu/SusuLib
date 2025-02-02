(C) 2024 Swudu Susuwu, dual licenses: choose [_GPLv2_](./LICENSE_GPLv2) or [_Apache 2_](./LICENSE) (allows all uses).
# [Table of Contents](../README.md#table-of-contents)
- [Purposes](#purposes)
# Purposes
[`./cxx/`](./) _C++_ implementations of posts
- [`./cxx/Macros.hxx`](./Macros.hxx) is
  - macros with wrap C++ features/attributes, such as {`SUSUWU_ASSUME`, `SUSUWU_CONSTEXPR`, `SUSUWU_DEFAULT`, `SUSUWU_DELETE`, `SUSUWU_EXPECTS`, `SUSUWU_ENSURES`, `SUSUWU_FINAL`, `SUSUWU_IF_CPLUSPLUS`, `SUSUWU_NOEXCEPT`, `SUSUWU_NORETURN`, `SUSUWU_NULLPTR`, `SUSUWU_OVERRIDE`, `SUSUWU_STATIC_ASSERT`, `SUSUWU_UNREACHABLE`} which (if used on old compilers, or with options such as `-std=c++11`) are replaced with no-ops or alternatives which have the same use,
  - macro options (which control the macro constants/macro functions). (View [Options/setup](../README.md#optionssetup) for options),
  - macro constants, such as `SUSUWU_SH_<color>` (`color` = {`DEFAULT`, `BLACK`, `DARK_GRAY`, `RED`, `LIGHT_RED`, `GREEN`, `LIGHT_GREEN`, `BROWN`, `YELLOW`, `BLUE`, `LIGHT_BLUE`, `PURPLE`, `LIGHT_PURPLE`, `CYAN`, `LIGHT_CYAN`, `LIGHT_GRAY`, `WHITE`}, if supported, expands to the [_ANSI_ color](https://wikipedia.org/wiki/Ansi_color) codes, else expands to ""),
  - macro functions, such as {`SUSUWU_ERROR`, `SUSUWU_WARNING`, `SUSUWU_INFO`, `SUSUWU_SUCCESS`, which use `SUSUWU_PRINT`}, `SUSUWU_PRINT` (if `__cplusplus`, uses `SUSUWU_CERR`, else uses `SUSUWU_STDERRR`),
  - `macroTestsNoexcept()` (unit tests, with return value for errors).
- [`./cxx/ClassFs.hxx`](./ClassFs.hxx) is
  - `ClassFsPath` (`PortableExecutable`'s constructor argument), `ClassFsBytecode` (`classSha2`'s input argument), `ClassFsHash` (`classSha2`'s return value)
  - modular functions to interact with filesystems {`classFsGetOwnPath()`, `classFsFopenOwnPath()`}
- [`./cxx/ClassObject.hxx`](./ClassObject.hxx) is
  - `class Instrumentation` (port of [`java.lang.instrument.Instrumentation`](https://docs.oracle.com/javase/8/docs/api/java/lang/instrument/Instrumentation.html)), `class Class : public Instrumentation` (port of [`java.lang.Class`](https://docs.oracle.com/javase/8/docs/api/java/lang/Class.html)), `class Object : public Class` (port of [`java.lang.Object`](https://docs.oracle.com/javase/8/docs/api/java/lang/Object.html)),
  - `classObjectTests()`, or `classObjectTestsNoexcept()` (unit tests with exceptions for errors, or return value for errors).
- [`./cxx/ClassPortableExecutable.hxx`](./ClassPortableExecutable.hxx) is
  - `class PortableExecutable : public Object` (stores file `path` and/or `bytecode` and/or `hex`code. TODO; `hash`?)
  - `class PortableExecutableBytecode : public PortableExecutable` loads `bytecode` from `path`. TODO; `hash`?
- [`./cxx/ClassSys.hxx`](./ClassSys.hxx) is
  - typedefs {ClassSysUSeconds}
  - globals {classSysArgc, classSysArgs}
  - modular functions to interact with:
    - console (_Posix_ `/bin/sh` or _Windows_ `cmd``) {`classSysGetConsoleInput()`, `classSysSetConsoleInput()`, `classSysGetConsoleAttributes()`, `classSysConsoleHasAnsiColors()`}
    - own process (`$0`) {`classSysInit()`, `templateCatchAll()`}
    - strings (or streams) {`classSysHexOs()`, `classSysHexStr()`, `classSysColoredParamOs()`, `classSysColoredParamStr()`, `classSysDebugIs()`, `classSysHexIs()`, `classSysGetline()`, `classSysCheckChar()`, `classSysCheckSz()`, `classSysCheckStr()`}
    - the OS {`classSysUSecondClock()`, `execvesFork()`, `execvexFork()`, `execves()`, `execvex()`, `classSysHasRoot()`, `classSysSetRoot()`, `classSysKernelCallback()`, `classSysKernelSetHook()`}
    - TODO: internet (`socket`, `Winsock2`).
  - `classSysTests()`, or `classSysTestsNoexcept()` (unit tests with exceptions for errors, or return value for errors).
- [`./cxx/ClassSha2.hxx`](./ClassSha2.hxx) is
  - the `classSha2` function pointer, which defaults to `classSha256()` (but you can set `classSha2 = sha128;` or `classSha2 = sha512;`), wrapped around official _RFC6234_ code. `./cxx/ClassResultList.hxx`, `./cxx/VirusAnalysis.cxx` and `./cxx/AssistantCns.cxx` all use `classSha2`.
  - `classSha2Tests()`, or `classSha2TestsNoexcept()` (unit tests with exceptions for errors, or return value for errors).
- [`./cxx/ClassCns.hxx`](./ClassCns.hxx) is `class Cns : public Object` (abstract neural system class with pure virtuals.) [Issue #6](https://github.com/SwuduSusuwu/SubStack/issues/6) is to implement this class.
- [`./cxx/ClassResultList.hxx`](./ClassResultList.hxx) is
  - `class ResultList : public Object` (holds `hashes`, `signatures`, `bytecodes`); `resultList*()` functions {`resultListDumpTo()`, `resultListLoadFrom()`, `resultListProduceHashes()` (`virusAnalysisTests()` uses this)}.
  - modular template (can use on all containers such as `std::vector`, `std::map` or `std::list`) `list*()` functions (such as `listMaxSize()`, `listDumpTo()`, `listLoadFrom()`, `listToHashes()`, `listIntersections()`, `listsIntersect()`, `listFindValue()`, `listHasValue()`, `listFindSubstr()`, `listHasSubstr()`, `listProduceSignature()` (`produceAbortListSignatures` uses this), `listFindSignatureOfValue()`, `listHasSignatureOfValue()` (`signatureAnalysis()` uses this), `explodeToList` (`./cxx/AssistantCns.cxx` uses this),
  produce unique signature, compare file against list of signatures), most of which were produced for antivirus signature analysis.
  - `classResultListsTests()`, or `classResultListsTestsNoexcept()` (unit tests with exceptions for errors, or return value for errors).
- [`./cxx/VirusAnalysis.hxx`](./VirusAnalysis.hxx) is
  - modular helper functions {`produceAbortListSignatures()` (for `signatureAnalysis()` use), `importedFunctionsList()` (work-in-progress, `staticAnalysis()` uses this), `straceOutputsAnalysis()` (work-in-progress, `sandboxAnalysis()` uses this), `produceAnalysisCns()` (for `cnsAnalysis()` use), `produceVirusFixCns()` (for `cnsVirusFix()` use)},
  - kernel hook function (`virusAnalysisHook()`), which uses `virusAnalysis()` to scan new downloadss (or scan all programs which execute); work-in-progress.
  - modular scan functions {`hashAnalysis()`, `signatureAnalysis()`, `staticAnalysis()` (processes [_Executable and Linkable Format_](https://wikipedia.org/wiki/Executable_and_Linkable_Format) or [_Portable Executable_](https://wikipedia.org/wiki/Portable_Executable)s, work-in-progress), `sandboxAnalysis()` (executes with `strace` + `chroot`, work-in-progress), `cnsAnalysis()` (uses `ClassCns.hxx`)} plus disinfection function (`cnsVirusFix()`) which form an antivirus program.
  - `virusAnalysisTests()`, or `virusAnalysisTestsNoexcept()` (unit tests with exceptions for errors, or return value for errors).
- [`./cxx/AssistantCns.hxx`](./AssistantCns.hxx) is
  - modular functions {`assistantCnsDownloadHosts()` (uses `wget` on `assistantCnsDefaultHosts`), `assistantCnsProcessXhtml()` (uses the next 2 functions to process `wget`'s downloads: `assistantCnsProcessUrls` (uses `boost/property_tree/xml_parser.hpp` to extract new URLs), `assistantCnsProcessQuestion` (work-in-progress, extracts question), `assistantCnsProcessResponses()` (work-in-progress, extracts answers)), `produceAssistantCns()` (uses datasets for backpropagation), `assistantCnsProcess` (uses forwardpropagation to answer new questions)} which form an assistant.
  - `assistantCnsTests()`, or `assistantCnsTestsNoexcept()` (unit tests with exceptions for errors, or return value for errors).
- [`./cxx/main.hxx`](./main.hxx) is `SusuwuUnitTestsBitmask main()` (executes all of those `*TestsNoexcept()` unit tests into a bitmask return value.)
All have lots of [issues](https://github.com/SwuduSusuwu/SubStack/issues) which you can [contribute to](../README.md#how-to-contribute), or can request that more resources go to).

