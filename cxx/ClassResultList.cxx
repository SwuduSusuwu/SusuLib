/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#ifndef INCLUDES_cxx_ClassResultList_cxx
#define INCLUDES_cxx_ClassResultList_cxx
#include "ClassResultList.hxx" /* resultList resultListDumpTo ResultListHash */
#include "Macros.hxx" /* SUSUWU_ERRSTR SUSUWU_SH_GREEN SUSUWU_NOEXCEPT SUSUWU_SH_ERROR SUSUWU_SH_RED SUSUWU_SH_WHITE SUSUWU_UNIT_TESTS */
#include <sstream> /* std::stringstream */
#include <stdexcept> /* std::runtime_error */
#include <string> /* std::string */
#if SUSUWU_UNIT_TESTS
namespace Susuwu {
namespace { /* [misc-use-anonymouse-namespace] */
static void classResultListDumpToTest(const ResultList &resultList, bool index, bool whitespace, bool pascalValues, const std::string &expectedValue) {
	std::stringstream os;
	resultListDumpTo(resultList, os, index, whitespace, pascalValues);
	if(expectedValue != os.str()) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, std::string("classResultListDumpToTest(resultList, ") + (index ? "true" : "false") + ", " + (whitespace ? "true" : "false") + ", " + (pascalValues ? "true" : "false") + "); \"" SUSUWU_SH_RED + os.str() + SUSUWU_SH_WHITE "\" == os.str(); \"" SUSUWU_SH_GREEN + expectedValue + SUSUWU_SH_WHITE "\" != os.str();")); /* TODO: standard macros for error/success colors, plus `SUSUWU_ERR` default color */
	}
}
}; /* namespace */
const bool classResultListTests() {
	ResultList resultList;
	resultList.hashes.insert(ResultListHash({'\x32'})); /* `.hashes` is `std::unordered_set`, thus test just 1 value. */
	resultList.signatures = {"1", "2"};
	resultList.bytecodes = {"01", "02"};
#ifdef SUSUWU_LIST_COUNT
	const std::string listHashesSz = "1:", listSignaturesSz = "2:", listBytecodesSz = "2:";
#else /* def SUSUWU_LIST_COUNT else */
	const std::string listHashesSz = "", listSignaturesSz = "", listBytecodesSz = ""; /* NOLINT(readability-redundant-string-init): define that those are "". */
#endif /* ndef SUSUWU_LIST_COUNT */
	classResultListDumpToTest(resultList, false, false, false, "list.hashes={" + listHashesSz + "0x32};list.signatures={" + listSignaturesSz + "0x31,0x32};list.bytecodes={" + listBytecodesSz + "0x3031,0x3032};");
	classResultListDumpToTest(resultList, true, true, false, "list.hashes = {" + listHashesSz + "\n\t0 = 0x32\n};\nlist.signatures = {" + listSignaturesSz + "\n\t0 = 0x31,\n\t1 = 0x32\n};\nlist.bytecodes = {" + listBytecodesSz + "\n\t0 = 0x3031,\n\t1 = 0x3032\n};\n");
	classResultListDumpToTest(resultList, false, false, true, "list.hashes={" + listHashesSz + "1:2};list.signatures={" + listSignaturesSz + "1:1,1:2};list.bytecodes={" + listBytecodesSz + "2:01,2:02};");
	return true;
}
}; /* namespace Susuwu */
#endif /* SUSUWU_UNIT_TESTS */
#endif /* ndef INCLUDES_cxx_ClassResultList_cxx */

