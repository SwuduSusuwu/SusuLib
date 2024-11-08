/* Licenses: allows all uses ("Creative Commons"/"Apache 2") */
#ifndef INCLUDES_cxx_main_cxx
#define INCLUDES_cxx_main_cxx
#include "AssistantCns.hxx" /* assistantCnsTestsNoexcept */
#include "ClassSha2.hxx" /* classSha2TestsNoexcept */
#include "ClassSys.hxx" /* classSysSetConsoleInput classSysTestsNoexcept templateCatchAll */
#include "Macros.hxx" /* SUSUWU_ASSUME SUSUWU_EXPECTS SUSUWU_ENSURES SUSUWU_NOEXCEPT SUSUWU_NORETURN SUSUWU_UNREACHABLE */
#include "VirusAnalysis.hxx" /* virusAnalysisTestsNoexcept */
#include <cstdlib> /* exit */
#include <iostream> /* std::cout std::flush std::endl */
namespace Susuwu {
void noExcept() SUSUWU_NOEXCEPT(true);
SUSUWU_NORETURN void noReturn();
void noExcept() SUSUWU_NOEXCEPT {std::cout << std::flush;}
void noReturn() {exit(0);}
const int testHarnesses() SUSUWU_EXPECTS(true) SUSUWU_ENSURES(true) {
	const bool consoleHasInput = classSysGetConsoleInput();
	if(consoleHasInput) {
		classSysSetConsoleInput(false);
	}
	assert(!classSysGetConsoleInput());
	std::cout << "cxx/Macros.hxx: " << std::flush;
	SUSUWU_ASSUME(true);
	noExcept();
	std::cout << "pass" << std::endl;
	std::cout << "classSysTestsNoexcept(): " << std::flush;
	classSysTestsNoexcept();
	std::cout << "classSha2TestsNoexcept(): " << std::flush;
	std::cout << (classSha2TestsNoexcept() ? "pass" : "error") << std::endl;
	std::cout << "virusAnalysisTestsNoexcept(): " << std::flush;
	if(virusAnalysisTestsNoexcept()) {
		std::cout << "pass" << std::endl;
	} else {
		std::cout << "error" << std::endl;
	}
	if(consoleHasInput) {
		assert(classSysSetConsoleInput(true));
	}
	std::cout << "assistantCnsTestsNoexcept(): " << std::flush;
	if(assistantCnsTestsNoexcept()) {
		std::cout << "pass" << std::endl;
	} else {
		std::cout << "error" << std::endl;
	}
	noReturn();
	SUSUWU_UNREACHABLE;
}
}; /* namespace Susuwu */
int main(int argc, const char **args) {
	const bool classSysInitSuccess = Susuwu::classSysInit(argc, args);
	assert(classSysInitSuccess);
	return Susuwu::testHarnesses();
}
#endif /* ndef INCLUDES_cxx_main_cxx */

