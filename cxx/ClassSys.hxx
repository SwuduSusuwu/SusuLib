/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#pragma once
#ifndef INCLUDES_cxx_ClassSys_hxx
#define INCLUDES_cxx_ClassSys_hxx
#include "Macros.hxx" /* SUSUWU_CXX20 SUSUWU_ERROR SUSUWU_NOEXCEPT SUSUWU_POSIX SUSUWU_SH_DEFAULT SUSUWU_SH_ERROR SUSUWU_SH_GREEN SUSUWU_UNIT_TESTS SUSUWU_WARNING */
#include <chrono> /* std::chrono */
#include <exception> /* std::exception */
#include <iomanip> /* std::setw */
#include <ios> /* std::hex */
#include <iostream> /* std::cin */
#ifdef SUSUWU_CXX20
#	include <span> /* std::span */
#endif
#include <sstream> /* std::stringstream */
#include <string> /* std::string std::to_string */
#ifdef SUSUWU_POSIX
#include <sys/types.h> /* pid_t */
#else
typedef int pid_t;
#endif
#include <type_traits> /* std::enable_if */
#include <vector> /* std::vector */
/* Abstractions to do with: `sh` scripts (such as: `exec*`, `sudo`), sockets (such as `socket`, `WinSock2`) */
namespace Susuwu {
#ifdef SUSUWU_CXX20
extern std::span<const char *> classSysArgs; /* [cppcoreguidelines-pro-bounds-pointer-arithmetic] fix */
#else
extern int classSysArgc;
extern const char **classSysArgs;
#endif
/* Called from main(), stores {argc, args} into {classSysArgc, classSysArgs}
 * Much simpler to use path from args[0] (versus https://stackoverflow.com/questions/1528298/get-path-of-executable/34109000#34109000)
 * @pre @code (0 < argc && SUSUWU_NULLPTR != args && SUSUWU_NULLPTR != args[0]
 * @post @code (0 < classSysArgc && SUSUWU_NULLPTR != classSysArgs && SUSUWU_NULLPTR != classSysArgs[0] */
const bool classSysInit(int argc, const char **args);

typedef decltype(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) ClassSysUSeconds;
inline const ClassSysUSeconds classSysUSecondClock() {
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

/* `std::array<char *>argv = argvS; argv += NULL; envp = envpS + NULL: pid_t pid = fork(); if(-1 != pid) {pid || (envpS.empty() ? execv(argv[0], &argv[0]) : execve(argv[0], &argv[0], &envp[0]));} return pid;`
 * @pre @code (-1 != access(argvS[0], X_OK) @endcode */
const pid_t execvesFork(/* const std::string &pathname, -- `execve` requires `&pathname == &argv[0]` */ const std::vector<std::string> &argvS = {}, const std::vector<std::string> &envpS = {}) SUSUWU_NOEXCEPT;
static const pid_t execvexFork(const std::string &toSh) SUSUWU_NOEXCEPT {return execvesFork({"/bin/sh", "-c", toSh});}
/* `pid_t pid = execvesFork(argvS, envpS); int status; waitpid(pid, &wstatus, 0); return wstatus;}`
 * @throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "execves: -1 == execvesFork()"))
 * @throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "execves: if(1 != argvS.size()) // TODO: non-POSIX systems with multiple commands
 * @pre @code (-1 != access(argvS[0], X_OK) @endcode */
const int execves(const std::vector<std::string> &argvS = {}, const std::vector<std::string> &envpS = {});
static const int execvex(const std::string &toSh) {return execves({"/bin/sh", "-c", toSh});}

/* #if SUSUWU_POSIX, `return (0 == geteuid());` #elif SUSUWU_WIN32 `return IsUserAnAdmin();` #endif `return false;` */
const bool classSysHasRoot();
/* #if SUSUWU_POSIX, `root ? (seteuid(0) : (seteuid(getuid() || getenv("SUDO_UID")), setuid(geteuid)); return classSysHasRoot();` #endif
 * Usage: classSysSetRoot(true); classSysKernelSetHook(...); classSysSetRoot(false); */
const bool classSysSetRoot(bool root); /* root ? (seteuid(0) : (seteuid(getuid() || atoi(getenv("SUDO_UID"))), setuid(geteuid)); return classSysHasRoot(); */

/* Effect: `(callback(args...) ? func(args...) : decltype(func(args...))())` */
template<typename func, typename callback, typename... Args>
auto classSysKernelCallback(Args... args) -> decltype(func(args...)) {
	const auto ret = callback(args...);
	return static_cast<bool>(ret) ? func(args...) : ret;
}
/* Usage: `classSysKernelSetHook(download, classSysKernelCallback<download, virusAnalysisDownloadCallback>);`
 * Effect: `:%s/func(/classSysKernelCallback<func, callback>(/`
 * @pre @code classSysHasRoot() @endof */
template<typename Func, typename Lambda>
const bool classSysKernelSetHook(Func func, Lambda callback) {
	if(classSysHasRoot()) {
		SUSUWU_WARNING("classSysKernelSetHook: TODO");
//		return true; /* TODO: hook `func` */
	} else {
		SUSUWU_ERROR("classSysKernelSetHook: if(!classSysHasRoot()) {/* kernel hook impossible to use */}");
	}
	return false;
}

static const bool classSysGetConsoleInput() { return std::cin.good() && !std::cin.eof(); }
const bool classSysSetConsoleInput(bool input); /* Set to `false` for unit tests/background tasks (acts as if user pressed `<ctrl>+d`, thus input prompts will use default choices.) Returns `classSysGetConsoleInput();` */

template<class Os, class Int,
	typename std::enable_if<std::is_integral<Int>::value, int>::type = 0>
inline Os &classSysHexOs(Os &os, const Int &value) {
	const std::ios::fmtflags oldFlags = os.flags();
	os << std::hex << value;
	os.flags(oldFlags);
	return os;
}
template<class Int,
	typename std::enable_if<std::is_integral<Int>::value, int>::type = 0>
inline const std::string classSysHexStr(const Int &value) {
	std::stringstream os;
	classSysHexOs(os, value);
	return os.str();
}
template<class Os, class Str,
	typename std::enable_if<!std::is_integral<Str>::value, int>::type = 0>
inline Os &classSysHexOs(Os &os, const Str &value) {
	const std::ios::fmtflags oldFlags = os.flags();
	const char oldFill = os.fill();
	os << std::hex;
	os.fill('0');
	for(const unsigned char ch : value) {
		os << std::setw(2)/* `setw` is unset after each use */ << static_cast<int>(ch);
	}
	os.fill(oldFill);
	os.flags(oldFlags);
	return os;
}
template<class Str,
	typename std::enable_if<!std::is_integral<Str>::value, int>::type = 0>
inline const Str classSysHexStr(const Str &value) {
	std::stringstream os;
	classSysHexOs(os, value);
	return os.str();
}
template<class Os, class List>
inline Os &classSysColoredParamOs(Os &os, const List &argvS, const bool parenthesis/* {...} */ = true) {
	if(parenthesis) {
		os << '{';
	}
	for(const auto &it: argvS) {
		if(&it != &*argvS.cbegin()) {
			os << ", ";
		}
		os << SUSUWU_SH_GREEN "\"";
		os << it;
		os << "\"" SUSUWU_SH_DEFAULT;
	}
	if(parenthesis) {
		os << '}';
	}
	return os;
}
template<class List>
inline const typename List::value_type classSysColoredParamStr(const List &argvS, const bool parenthesis/* {...} */ = true) {
	typename List::value_type str = (parenthesis ? "{" : "");
	for(const auto &it: argvS) {
		if(&it != &*argvS.cbegin()) {
			str += ", ";
		}
		str += SUSUWU_SH_GREEN "\"";
		str += it;
		str += "\"" SUSUWU_SH_DEFAULT;
	}
	if(parenthesis) {
		str += '}';
	}
	return str;
}

template<typename Func, typename... Args>
auto templateCatchAll(Func func, const std::string &funcName, Args... args) -> const decltype(func(args...)) {
	try {
		return func(args...);
	} catch (const std::exception &ex) {
		SUSUWU_ERROR(funcName + " {throw std::exception(\"" + ex.what() + "\");}");
		return decltype(func(args...))(); /* `func(args...)`'s default return value; if `int func(args...)`, `return 0;`. If `bool func(args...)`, `return false;` */
	}
}

#if SUSUWU_UNIT_TESTS
/* @throw std::runtime_error */
const bool classSysTests();
static const bool classSysTestsNoexcept() SUSUWU_NOEXCEPT {return templateCatchAll(classSysTests, "classSysTests()");}
#endif /* SUSUWU_UNIT_TESTS */

}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassSys_hxx */
