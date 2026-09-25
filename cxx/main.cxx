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
#ifdef SUSUWU_USE_TENSORFLOW
#	include "ClassTensorFlowCns.hxx" /* classTensorFlowCnsTestsNoexcept */
#endif /* def SUSUWU_USE_TENSORFLOW */
#include "ClassWebBrowse.hxx" /* classWebBrowseTestsNoexcept */
#include "Macros.hxx" /* macrosTestsNoexcept SUSUWU_EXPECTS SUSUWU_EXPERIMENTAL_ISSUES SUSUWU_ENSURES SUSUWU_NOEXCEPT SUSUWU_UNIT_TESTS SUSUWU_WARNING SUSUWU_USE_TENSORFLOW */
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
		classIoSetConsoleInput(false); /* disable prompts for unit tests. Moved down to prevent `assert` failures if `cxx/Classio.hxx` fails. Notice: this move assumes that the tests above won't block on input */
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
	std::cout << "classWebBrowseTestsNoexcept" << std::flush;
	if(classWebBrowseTestsNoexcept()) {
		std::cout << "pass" << std::endl;
	} else {
		std::cout << "error" << std::endl;
		susuwuUnitTestsErrno |= susuwuUnitTestsClassWebBrowseBit;
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
#	ifdef SUSUWU_USE_TENSORFLOW
	std::cout << "classTensorFlowCnsTestsNoexcept" << std::flush;
	if(classTensorFlowCnsTestsNoexcept()) {
		std::cout << "pass" << std::endl;
	} else {
		std::cout << "error" << std::endl;
		susuwuUnitTestsErrno |= susuwuUnitTestsClassTensorFlowCnsBit;
	}
#	endif /* def SUSUWU_USE_TENSORFLOW */
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

