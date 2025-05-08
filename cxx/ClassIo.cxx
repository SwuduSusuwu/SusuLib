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
#include SUSUWU_IF_CPLUSPLUS(<cstring>, <string.h>) /* strcmp strlen */
#ifdef __linux__
#	include <linux/limits.h> /* PATH_MAX */
#endif /* def __linux__ */
#include <iostream> /* std::cerr std::cout std::endl std::flush std::ios::eofbit std::ios::goodbit */
#include <sstream> /* std::stringstream */
#include <stdexcept> /* std::logic_error */
#include <string> /* std::string std::to_string */
#ifdef __linux__ /* `clang-tidy` wants ordered `#include`s */
#	include <sys/types.h> /* ssize_t */
#	include <unistd.h> /* readlink */
#endif /* def __linux__ */
#ifdef SUSUWU_WIN32
#	include <windows.h> /* CONSOLE_SCREEN_BUFFER_INFO DWORD GetConsoleMode GetConsoleScreenBufferInfo GetLastError GetModuleFileName GetModuleHandle GetStdHandle HMODULE PROCESS_INFORMATION SetConsoleMode STARTUPINFO STD_OUTPUT_HANDLE ZeroMemory */
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

const bool classIoSetConsoleInput(bool input) {
	input ? std::cin.clear(std::ios::goodbit) : std::cin.setstate(std::ios::eofbit);
	return classIoGetConsoleInput();
}
const unsigned char classIoGetConsoleAttributes() {
#ifdef SUSUWU_WIN32
	CONSOLE_SCREEN_BUFFER_INFO info;
	if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) {
		SUSUWU_WARNING("classIoGetConsoleAttributes() {/* TODO: [decode response from `GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)`](https://github.com/SwuduSusuwu/SubStack/issues/17)");
		return info.wAttributes;
	} else {
		SUSUWU_ERROR("classIoGetConsoleAttributes() {!GetConsoleScreenBufferInfo() && GetLastError() == " SUSUWU_SH_PURPLE + std::to_string(GetLastError()) + SUSUWU_SH_DEFAULT "}");
	}
#elif defined SUSUWU_POSIX
	std::cout << "\033[?6;1;1t" /* Request console attributes */ << std::flush;
	char buffer[32];
	std::cin.read/*non-blocking*/(buffer, sizeof(buffer)); /* read request response from console */ /* TODO: have it portable, support all consoles */
	const size_t bytesRead = strlen(buffer);
	if(0 < bytesRead) {
		SUSUWU_WARNING("classIoGetConsoleAttributes() {/* TODO: [decode response from `\\033[?6;1;1t`](https://github.com/SwuduSusuwu/SubStack/issues/17)");
# ifdef SUSUWU_DEBUG2
		std::cerr << "Current color settings: ";
		for(size_t i = 0; i < bytesRead; ++i) {
			std::cerr << buffer[i]; /* TODO: decode this response (Termux doesn't have this) */
		}
		std::cout << std::endl;
# endif /* def SUSUWU_DEBUG2 */
	} else {
# ifndef NDEBUG
		SUSUWU_WARNING("classIoGetConsoleAttributes() {std::cout << \"\\033[?6;1;1t\" << std::flush; char buffer[32]; (" + std::to_string(bytesRead) + " == std::cin.readsome(buffer, sizeof(buffer));)");
# endif /* ndef NDEBUG */
	}
#else /* elif defined SUSUWU_POSIX else */
	SUSUWU_NOTICE("classIoGetConsoleAttributes() { /* [TODO](https://github.com/SwuduSusuwu/SubStack/issues/17): `#if !defined(SUSUWU_WIN32) && !defined(SUSUWU_POSIX)`. Hardcoded to `errno = ENOTTY; return 0;`. */ }");
#endif /* elif defined SUSUWU_POSIX else */
	errno = ENOTTY;
	return 0;
}
const bool classIoConsoleHasAnsiColors() {
#if defined(SUSUWU_SH_SKIP_COLORS) && SUSUWU_SH_SKIP_COLORS
	return false;
#elif defined(__WIN32__)
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if(INVALID_HANDLE_VALUE == hConsole) {
		SUSUWU_PRINT(SUSUWU_SH_WARNING, "classIoConsoleHasAnsiColors() {(!GetConsoleScreenBufferInfo()} && GetLastError() == " SUSUWU_SH_PURPLE + std::to_string(GetLastError()) + SUSUWU_SH_DEFAULT "}");
		return false;
	}
	DWORD mode;
	if(!GetConsoleMode(hConsole, &mode)) {
		return false;
	}
	if(!SetConsoleMode(hConsole, mode | ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING /* virtual mode allows CSI colors */)) {
		SUSUWU_PRINT(SUSUWU_SH_WARNING, "classIoConsoleHasAnsiColors() {(!SetConsoleMode(hConsole, mode | ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) && GetLastError() == " SUSUWU_SH_PURPLE + std::to_string(GetLastError()) + SUSUWU_SH_DEFAULT "}");
		return false;
	}
	return true;
#elif defined _POSIX_VERSION
	return true;
#else /* ndef _POSIX_VERSION */
	const char *term = getenv("TERM");
#	if defined(SUSUWU_SH_SKIP_COLORS) && !SUSUWU_SH_SKIP_COLORS
	static const char *uncolored[] = {"dumb", SUSUWU_NULLPTR}; /* blacklist */
	for(const char **it = uncolored; SUSUWU_NULLPTR != *it; ++*it) {
		if(0 == strcmp(term, *it)) {
			return false;
		}
	}
	return true;
#	else /* ndef SUSUWU_SH_SKIP_COLORS */
	static const char *colored[] = {"screen", "screen-256color", "vt100", "xterm", "xterm-256color", SUSUWU_NULLPTR}; /* whitelist */
	for(const char **it = colored; SUSUWU_NULLPTR != *it; ++*it) {
		if(0 == strcmp(term, *it)) {
			return true;
		}
	}
	return false;
#	endif /* ndef SUSUWU_SH_SKIP_COLORS_BLACKLIST */
#endif /* ndef _POSIX_VERSION */
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
	classIoHexOsSzTest("", (sizeof("") - sizeof('\0')) * 2, true /* test that `value.empty()` produces 0 hexits */);
	classIoHexOsSzTest("22", (sizeof("22") - sizeof('\0')) * 2, true /* test that 2 chars produces 4 hexits */);
	classIoHexOsSzTest("uwu", (sizeof("uwu") - sizeof('\0')) * 2, true /* test that 3 chars produces 6 hexits */);
	classIoHexOsSzTest(std::string({'\0'}), sizeof('\0') * 2, false /* test that char == 0x00 produces 2 hexits */);
	classIoHexOsSzTest("\010", sizeof('\010') * 2, false /* test that char <= 0x10 produces 2 hexits */);
	classIoHexOsSzTest("\022", sizeof('\022') * 2, false /* test that char >= 0x10 produces 2 hexits */);

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

