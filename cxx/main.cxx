/* Licenses: allows all uses ("Creative Commons"/"Apache 2") */
#ifndef INCLUDES_cxx_main_cxx
#define INCLUDES_cxx_main_cxx
#include "main.hxx"
#include "AssistantCns.hxx" /* assistantCnsTestsNoexcept */
#include "ClassResultList.hxx" /* classResultListTestsNoexcept */
#include "ClassSha2.hxx" /* classSha2TestsNoexcept */
#include "ClassSys.hxx" /* classSysGetOwnPath classSysSetConsoleInput classSysTestsNoexcept templateCatchAll */
#include "Macros.hxx" /* macrosTestsNoexcept SUSUWU_EXPECTS SUSUWU_EXPERIMENTAL_ISSUES SUSUWU_ENSURES SUSUWU_NOEXCEPT SUSUWU_WARNING */
#include "VirusAnalysis.hxx" /* virusAnalysisTestsNoexcept */
#include <iostream> /* std::cout std::flush std::endl */
#include <string> /* std::to_string */
#ifdef SUSUWU_CXX17 /* `type_traits` is C++11 but `is_nothrow_invocable` is C++17 */
#	include <type_traits> /* std::is_nothrow_invocable */
#endif /* def SUSUWU_CXX17 */
namespace Susuwu {
/* `clang-tidy` off: NOLINTBEGIN(hicpp-signed-bitwise, readability-simplify-boolean-expr) */
static const SusuwuUnitTestsBitmask unitTestsCxx() SUSUWU_EXPECTS(std::cout.good()) SUSUWU_ENSURES(0 == macrosTestsNoexcept() && true == classSysTestsNoexcept() && true == classSha2TestsNoexcept() && true == virusAnalysisTestsNoexcept() && true == assistantCnsTestsNoexcept())
#ifdef SUSUWU_CXX17 /* `type_traits` is C++11 but `is_nothrow_invocable` is C++17 */
	SUSUWU_NOEXCEPT(std::is_nothrow_invocable<decltype(std::cout << ""), decltype(std::cout), decltype("")>::value)
#endif /* def SUSUWU_CXX17 */
	{
	int susuwuUnitTestsErrno = 0;
	if(!std::cout.good()) {
		susuwuUnitTestsErrno |= susuwuUnitTestsConsoleBit;
	}
	const bool consoleHasInput = classSysGetConsoleInput();
	if(consoleHasInput) {
		classSysSetConsoleInput(false); /* disable prompts for unit tests. Moved down to prevent `assert` failures if `cxx/ClassSys.hxx` fails. Notice: this move assumes that the tests above won't block on input */
	}
	if(true == classSysGetConsoleInput()) {
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
	std::cout << "virusAnalysisTestsNoexcept(): " << std::flush;
	if(virusAnalysisTestsNoexcept()) {
		std::cout << "pass" << std::endl;
	} else {
		std::cout << "error" << std::endl;
		susuwuUnitTestsErrno |= susuwuUnitTestsVirusAnalysisBit;
	}
	if(consoleHasInput && false == classSysSetConsoleInput(true)) {
		susuwuUnitTestsErrno |= susuwuUnitTestsConsoleBit;
	}
	std::cout << "assistantCnsTestsNoexcept(): " << std::flush;
	if(assistantCnsTestsNoexcept()) {
		std::cout << "pass" << std::endl;
	} else {
		std::cout << "error" << std::endl;
		susuwuUnitTestsErrno |= susuwuUnitTestsAssistantCnsBit;
	}
	return susuwuUnitTestsErrno;
}
}; /* namespace Susuwu */
const SusuwuUnitTestsBitmask susuwuUnitTests() {
	return Susuwu::unitTestsCxx();
}
SusuwuUnitTestsBitmask main(int argc, const char **args) {
	if(true != Susuwu::classSysInit(argc, args)) {
		return susuwuUnitTestsClassSysBit;
	}
#ifdef SUSUWU_EXPERIMENTAL
	SUSUWU_WARNING('`' + std::string(Susuwu::classSysGetOwnPath()) + "` " SUSUWU_EXPERIMENTAL_ISSUES);
#endif
	return Susuwu::unitTestsCxx();
}
/* `clang-tidy` on: NOLINTEND(hicpp-signed-bitwise, readability-simplify-boolean-expr) */
#endif /* ndef INCLUDES_cxx_main_cxx */

