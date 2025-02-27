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
SusuwuUnitTestsBitmask main(int argc, const char **args);
#ifdef __cplusplus
} /* extern "C" { */
#endif /* def __cplusplus */
#endif /* ndef INCLUDES_cxx_main_hxx */

