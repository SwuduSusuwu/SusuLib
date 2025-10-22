/* Attribution (henceforth "*this attribution*", whose syntax is *Markdown*): 2024 [Swudu Susuwu](https://swudususuwu.substack.com)
 * <https://github.com/SwuduSusuwu/SusuLib/> has the newest version of `./cxx/main.hxx` (henceforth "*this source code*").
 * If *this attribution* is shown, *this source code* allows all uses. *This attribution* constitutes the most permissive which is compatible with [*GPLv2*](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) + [*Apache 2*](https://www.apache.org/licenses/LICENSE-2.0.html), which is suitable for personal use (also suitable for school use).
 * If *this attribution* is not professional enough for business use: businesses can use *this source code* through included versions of [*GPLv2*](./LICENSE_GPLv2), [*Apache 2*](./LICENSE), or through both of those. */
#pragma once
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
#endif /* def __cplusplus */
SusuwuUnitTestsBitmask main(int argc, const char **args);
#endif /* ndef INCLUDES_cxx_main_hxx */

