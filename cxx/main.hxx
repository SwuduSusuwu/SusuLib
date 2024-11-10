/* Licenses: allows all uses ("Creative Commons"/"Apache 2") */
#ifndef INCLUDES_cxx_main_hxx
#define INCLUDES_cxx_main_hxx
#ifdef __cplusplus
extern "C" { /* progress to https://github.com/SwuduSusuwu/SubStack/issues/3 , such that other languages can execute unit tests */
#endif /* def __cplusplus */
/* `clang-tidy` on: NOLINTBEGIN(hicpp-signed-bitwise) */
typedef int SusuwuUnitTestsBitmask; /* normal `int`, but used as bitmask (non-zero return value says which tests failed) */
static const int susuwuUnitTestsMacrosBit        = 1 << 0; /*  1: `Macros.hxx`:`macrosTestsNoexcept()` */
static const int susuwuUnitTestsClassSysBit      = 1 << 1; /*  2: `ClassSys.hxx`:`classSysTestsNoexcept()` */
static const int susuwuUnitTestsClassSha2Bit     = 1 << 2; /*  4: `ClassSha2.hxx`:`classSha2TestsNoexcept()` */
static const int susuwuUnitTestsVirusAnalysisBit = 1 << 3; /*  8: `VirusAnalysis.hxx`:`virusAnalysisTestsNoexcept()` */
static const int susuwuUnitTestsAssistantCnsBit  = 1 << 4; /* 16: `AssistantCns.hxx`:`assistantCnsTestsNoexcept()` */
static const int susuwuUnitTestsConsoleBit       = 1 << 5; /* 32: `classSys.hxx`:`classSysSetConsoleInput()` */
/* `clang-tidy` on: NOLINTEND(hicpp-signed-bitwise) */
const SusuwuUnitTestsBitmask susuwuUnitTests();
SusuwuUnitTestsBitmask main(int argc, const char **args);
#ifdef __cplusplus
} /* extern "C" { */
#endif /* def __cplusplus */
#endif /* ndef INCLUDES_cxx_main_hxx */

