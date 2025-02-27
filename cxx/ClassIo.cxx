/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#ifndef INCLUDES_cxx_ClassIo_cxx
#define INCLUDES_cxx_ClassIo_cxx
#include "Macros.hxx" /* SUSUWU_IF_CPLUSPLUS SUSUWU_NOEXCEPT SUSUWU_NULLPTR SUSUWU_POSIX SUSUWU_UNIT_TESTS SUSUWU_WIN32 */
#include "ClassIo.hxx" /* ClassIoPath */
#include "ClassSys.hxx" /* templateCatchAll */
//#include SUSUWU_IF_CPLUSPLUS(<cassert>, <assert.h>) /* assert */
#include SUSUWU_IF_CPLUSPLUS(<cerrno>, <errno.h>) /* errno */
#include SUSUWU_IF_CPLUSPLUS(<cstdio>, <stdio.h>) /* FILE fopen */
#ifdef __linux__
#	include <linux/limits.h> /* PATH_MAX */
#endif /* def __linux__ */
//#include <stdexcept> /* std::runtime_error */
#include <string> /* std::to_string */
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
	HMODULE hModule = GetModuleHandle(SUSUWU_NULLPTR);
	if(hModule) {
		char ownPathStr[MAX_PATH];
		GetModuleFileName(hModule, ownPathStr, sizeof(ownPathStr));
		return ClassIoPath(ownPathStr);
	} else {
		SUSUWU_ERROR("classIoGetOwnPath(): { if(!GetModuleHandle(NULL)) {/* this shouldn't happen */} }");
		return ClassIoPath(); /* return EXIT_FAILURE; */
	}
#else /* def SUSUWU_WIN32 else */
	if(SUSUWU_NULLPTR == classIoArgs) {
		SUSUWU_ERROR("classIoGetOwnPath(): { if(SUSUWU_NULLPTR == classIoArgs) {/* `classIoInit()` was not used? */} }");
		return ClassIoPath(); /* return EXIT_FAILURE; */
	} else if(SUSUWU_NULLPTR == classIoArgs[0]) { /* NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic) */
		SUSUWU_ERROR("classIoGetOwnPath(): { if(SUSUWU_NULLPTR == classIoArgs[0]) {/* `classIoInit()` was not used? */} }");
		return ClassIoPath(); /* return EXIT_FAILURE; */
	}
	return ClassIoPath(classIoArgs[0]); /* NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic) */
#endif /* def SUSUWU_WIN32 else */
}

#if SUSUWU_UNIT_TESTS
const bool classIoTests() {
	return true;
}
const bool classIoTestsNoexcept() SUSUWU_NOEXCEPT { return templateCatchAll(classIoTests, "classIoTests()"); }
#endif /* SUSUWU_UNIT_TESTS */

}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassIo_cxx */

