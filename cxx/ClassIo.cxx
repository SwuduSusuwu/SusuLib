/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#ifndef INCLUDES_cxx_ClassIo_cxx
#define INCLUDES_cxx_ClassIo_cxx
#include "Macros.hxx" /* SUSUWU_IF_CPLUSPLUS SUSUWU_NOEXCEPT SUSUWU_NULLPTR SUSUWU_POSIX SUSUWU_SH_GREEN SUSUWU_SH_DEFAULT SUSUWU_SH_RED SUSUWU_UNIT_TESTS SUSUWU_WIN32 */
#include "ClassIo.hxx" /* classIoHexStr classIoHexIs classIoHexOs ClassIoPath SUSUWU_HEX_PREFIX_SZ */
#include "ClassSys.hxx" /* classSysArgs templateCatchAll */
//#include SUSUWU_IF_CPLUSPLUS(<cassert>, <assert.h>) /* assert */
#include SUSUWU_IF_CPLUSPLUS(<cerrno>, <errno.h>) /* errno */
#include SUSUWU_IF_CPLUSPLUS(<climits>, <limits.h>) /* CHAR_BIT */
#include SUSUWU_IF_CPLUSPLUS(<cstdio>, <stdio.h>) /* FILE fopen */
#ifdef __linux__
#	include <linux/limits.h> /* PATH_MAX */
#endif /* def __linux__ */
#include <sstream> /* std::stringstream */
#include <stdexcept> /* std::logic_error */
#include <string> /* std::string std::to_string */
#ifdef __linux__ /* `clang-tidy` wants ordered `#include`s */
#	include <sys/types.h> /* ssize_t */
#	include <unistd.h> /* readlink */
#endif /* def __linux__ */
#ifdef SUSUWU_WIN32
# include <windows.h> /* GetModuleFileName GetModuleHandle HMODULE */
# undef ERROR /* undo `windows.h`'s `#define ERROR 0` */
#endif /* def SUSUWU_WIN32 */
namespace Susuwu {
const FILE *classIoFopenOwnPath() {
	return fopen(classIoGetOwnPath().c_str(), "r");
}
const ClassIoPath classIoGetOwnPath() {
#ifdef __linux__
	char path[PATH_MAX]; /* NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays) */
	const ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
	if (len == -1) {
		SUSUWU_ERROR("classIoGetOwnPath(): { if(-1 == readlink(\"/proc/self/exe\", path, sizeof(path) - 1)) { errno == " + std::to_string(errno) + "; } }");
		return ClassIoPath(); /* return EXIT_FAILURE; */
	}
	path[len] = '\0'; /* NOLINT(cppcoreguidelines-pro-bounds-constant-array-index) */
//	`return "/proc/self/exe"; /* if _Termux_, causes `PortableExecutableBytecode(classIoGetOwnPath())` to act as `PortableExecutableBytecode("/apex/com.android.runtime/bin/linker64")` */
	return ClassIoPath(path); /* causes `PortableExecutableBytecode(classIoGetOwnPath())` to act as `PortableExecutableBytecode(argv[0])` */
#elif defined SUSUWU_WIN32
	const HMODULE hModule = GetModuleHandle(SUSUWU_NULLPTR);
	const size_t nSize = GetModuleFileName(hModule, SUSUWU_NULLPTR, 0);
	static const std::string getModuleFileNameReturn = "classIoGetOwnPath(): { HMODULE hModule = GetModuleHandle(nullptr); size_t nSize = GetModuleFileName(hModule, nullptr, 0); (nSize == " SUSUWU_SH_PURPLE;
	if(0 < nSize) {
		char *const lpFilename = new char[nSize];
		const size_t result = GetModuleFileName(hModule, lpFilename, nSize);
		if(nSize == result) {
			return ClassIoPath(lpFilename);
		} else {
			SUSUWU_ERROR(getModuleFileNameReturn + std::to_string(nSize) + SUSUWU_SH_DEFAULT "); char *const lpFilename = new char[nSize]; (GetModuleFileName(hModule, lpFileName, nSize) == " SUSUWU_SH_PURPLE + std::to_string(result) + SUSUWU_SH_DEFAULT " /* expected `== nSize` */); (GetLastError() == " SUSUWU_SH_PURPLE + std::to_string(GetLastError()) + SUSUWU_SH_DEFAULT "); }");
			return ClassIoPath(); /* return EXIT_FAILURE; */
		}
	} else {
		SUSUWU_ERROR(getModuleFileNameReturn + "0" SUSUWU_SH_DEFAULT " /* expected `> 0` */); (GetLastError() == " SUSUWU_SH_PURPLE + std::to_string(GetLastError()) + SUSUWU_SH_DEFAULT "); }");
		return ClassIoPath(); /* return EXIT_FAILURE; */
	}
#else /* def SUSUWU_WIN32 else */
	if(SUSUWU_NULLPTR == classSysArgs) {
		SUSUWU_ERROR("classIoGetOwnPath(): { if(SUSUWU_NULLPTR == classSysArgs) {/* `classSysInit()` was not used? */} }");
		return ClassIoPath(); /* return EXIT_FAILURE; */
	} else if(SUSUWU_NULLPTR == classSysArgs[0]) { /* NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic) */
		SUSUWU_ERROR("classIoGetOwnPath(): { if(SUSUWU_NULLPTR == classSysArgs[0]) {/* `classSysInit()` was not used? */} }");
		return ClassIoPath(); /* return EXIT_FAILURE; */
	}
	return ClassIoPath(classSysArgs[0]); /* NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic) */
#endif /* def SUSUWU_WIN32 else */
}

#if SUSUWU_UNIT_TESTS
namespace { /* [misc-use-anonymous-namespace] */
static void classIoHexOsSzTest(const std::string &value, const size_t hexSz, const bool printable) {
	const size_t ss = classIoHexStr(value).size();
	const std::string escapedValue = (printable ? ('"' + value + '"') : "value");
	if(hexSz + SUSUWU_HEX_PREFIX_SZ != ss) {
		throw std::logic_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "classIoHexStr(" + escapedValue + ").size() == " SUSUWU_SH_RED + std::to_string(value.size()) + SUSUWU_SH_DEFAULT "; classIoHexStr(" + escapedValue + ").size() != " SUSUWU_SH_GREEN + std::to_string(hexSz) + SUSUWU_SH_DEFAULT ";"));
	}
	std::stringstream os;
	classIoHexOs(os, value);
	if(ss != os.str().size()) {
		throw std::logic_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "classIoHexOs(os, " + escapedValue + "); " SUSUWU_SH_RED + std::to_string(value.size()) + SUSUWU_SH_DEFAULT " /* value.size() */ != " SUSUWU_SH_GREEN + std::to_string(os.str().size()) + SUSUWU_SH_DEFAULT " /* os.str().size() */;"));
	}
}
static void classIoHexSsNumTest(const long &num) {
	std::stringstream os;
	classIoHexOs(os, num);
	long newNum = 0;
	classIoHexIs(os, newNum);
	if(num != newNum) {
		throw std::logic_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, std::string("classIoHexOs(os, " SUSUWU_SH_GREEN) + std::to_string(num) + SUSUWU_SH_DEFAULT "\"); long newNum; classIoHexIs(os, newNum); newNum == " SUSUWU_SH_RED + std::to_string(newNum) + SUSUWU_SH_DEFAULT ";"));
	}
}
static void classIoHexSsStrTest(const std::string &value, const bool printable) {
	std::stringstream os;
	classIoHexOs(os, value);
	std::string newValue;
	classIoHexIs(os, newValue);
	if(value != newValue) {
		const std::string escapedValue = (printable ? ("\"" SUSUWU_SH_GREEN + value + SUSUWU_SH_DEFAULT "\"") : (SUSUWU_SH_GREEN + classIoHexStr(value) + SUSUWU_SH_DEFAULT));
		const std::string escapedNewValue = (newValue.empty() ? "\"\"" : (printable ? ("\"" SUSUWU_SH_RED + newValue + SUSUWU_SH_DEFAULT "\"") : SUSUWU_SH_RED + classIoHexStr(newValue) + SUSUWU_SH_DEFAULT));
		throw std::logic_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "std::string value = " + escapedValue + ", newValue; classIoHexOs(os, value); classIoHexIs(os, newValue); newValue != value; newValue == " + escapedNewValue + ';'));
	}
}
}; /* namespace */
const bool classIoTests() {
	/* test `classIoHexStr()` and `classIoHexOs()`'s output lengths */
	classIoHexOsSzTest("", 0, true /* test that `value.empty()` produces 0 hexits */);
	classIoHexOsSzTest("22", 4, true /* test that 2 chars produces 4 hexits */);
	classIoHexOsSzTest("uwu", 6, true /* test that 3 chars produces 6 hexits */);
	classIoHexOsSzTest(std::string({'\0'}), 2, false /* test that char == 0x00 produces 2 hexits */);
	classIoHexOsSzTest("\010", 2, false /* test that char <= 0x10 produces 2 hexits */);
	classIoHexOsSzTest("\022", 2, false /* test that char >= 0x10 produces 2 hexits */);

	/* test that `classIoHexIs()` undoes `classIoHexOs()` */
	for(long q = 0; (1 << CHAR_BIT) >= q /* test all `char` + first `short` */; ++q) {
		classIoHexSsNumTest(q);
	}
	classIoHexSsStrTest("", true);
	classIoHexSsStrTest("2", true);
	classIoHexSsStrTest("22", true);
	classIoHexSsStrTest("\010", false);
	classIoHexSsStrTest("\022", false);
	classIoHexSsStrTest(std::string({'\0'}), false);

	return true;
}
const bool classIoTestsNoexcept() SUSUWU_NOEXCEPT { return templateCatchAll(classIoTests, "classIoTests()"); }
#endif /* SUSUWU_UNIT_TESTS */

}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassIo_cxx */

