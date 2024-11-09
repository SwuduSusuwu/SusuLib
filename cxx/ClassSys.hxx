/* Dual licenses: choose "Creative Commons" or "Apache 2" (allows all uses) */
#pragma once
#ifndef INCLUDES_cxx_ClassSys_hxx
#define INCLUDES_cxx_ClassSys_hxx
#include "Macros.hxx" /* ERROR SUSUWU_NOEXCEPT SUSUWU_PRINT */
#include <cassert> /* assert */
#include <chrono> /* std::chrono */
#include <exception> /* std::exception */
#include <iomanip> /* std::dec std::hex */
#include <ios> /* std::streamsize */
#include <iostream> /* std::cerr std::cin std::endl */
#include <sstream> /* std::stringstream */
#include <string> /* std::string std::to_string */
#ifdef _POSIX_VERSION
#include <sys/types.h> /* pid_t */
#else
typedef int pid_t;
#endif
#include <type_traits> /* std::remove_const_t */
#include <vector> /* std::vector */
/* Abstractions to do with: `sh` scripts (such as: exec*, sudo), sockets (TODO), filesystems (TODO) */
namespace Susuwu {
extern int classSysArgc;
extern const char **classSysArgs;
/* Called from main(), stores {argc, args} into {classSysArgc, classSysArgs}
 * Much simpler to use path from args[0] (versus https://stackoverflow.com/questions/1528298/get-path-of-executable/34109000#34109000)
 * @pre @code (0 < argc && nullptr != args && nullptr != args[0]
 * @post @code (0 < classSysArgc && nullptr != classSysArgs && nullptr != classSysArgs[0] */
const bool classSysInit(int argc, const char **args);

inline const auto classSysUSecondClock() {
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
typedef decltype(classSysUSecondClock()) ClassSysUSeconds;

/* `std::array<char *>argv = argvS; argv += NULL; envp = envpS + NULL: pid_t pid = fork(); if(-1 != pid) {pid || (envpS.empty() ? execv(argv[0], &argv[0]) : execve(argv[0], &argv[0], &envp[0]));} return pid;`
 * @pre @code (-1 != access(argvS[0], X_OK) @endcode */
const pid_t execvesFork(/* const std::string &pathname, -- `execve` requires `&pathname == &argv[0]` */ const std::vector<std::string> &argvS = {}, const std::vector<std::string> &envpS = {}) SUSUWU_NOEXCEPT;
static const pid_t execvexFork(const std::string &toSh) SUSUWU_NOEXCEPT {return execvesFork({"/bin/sh", "-c", toSh});}
/* `pid_t pid = execvesFork(argvS, envpS); int status; waitpid(pid, &wstatus, 0); return wstatus;}`
 * @throw std::runtime_error(SUSUWU_ERRSTR(ERROR, "execves: -1 == execvesFork()"))
 * @pre @code (-1 != access(argvS[0], X_OK) @endcode */
const int execves(const std::vector<std::string> &argvS = {}, const std::vector<std::string> &envpS = {});
static const int execvex(const std::string &toSh) {return execves({"/bin/sh", "-c", toSh});}

/* #if _POSIX_VERSION, `return (0 == geteuid());` #elif __WIN32__ `return IsUserAnAdmin();` #endif `return false;` */
const bool classSysHasRoot();
/* #if _POSIX_VERSION, `root ? (seteuid(0) : (seteuid(getuid() || getenv("SUDO_UID")), setuid(geteuid)); return classSysHasRoot();` #endif
 * Usage: classSysSetRoot(true); functionsWhichRequireRoot; classSysSetRoot(false); */
const bool classSysSetRoot(bool root); /* root ? (seteuid(0) : (seteuid(getuid() || atoi(getenv("SUDO_UID"))), setuid(geteuid)); return classSysHasRoot(); */

static const bool classSysGetConsoleInput() { return std::cin.good() && !std::cin.eof(); }
const bool classSysSetConsoleInput(bool input); /* Set to `false` for unit tests/background tasks (acts as if user pressed `<ctrl>+d`, thus input prompts will use default choices.) Returns `classSysGetConsoleInput();` */

template<class Os, class Str>
inline Os &classSysHexOs(Os &os, const Str &value) {
	const std::ios::fmtflags oldFlags = std::cout.flags();
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
template<class Str>
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
inline const auto classSysColoredParamStr(const List &argvS, const bool parenthesis/* {...} */ = true) {
	std::remove_const_t<typename List::value_type> str = (parenthesis ? "{" : "");
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
auto templateCatchAll(Func func, const std::string &funcName, Args... args) {
	try {
		return func(args...);
	} catch (const std::exception &ex) {
		SUSUWU_PRINT(ERROR, funcName + " {throw std::exception(\"" + ex.what() + "\");}");
		return decltype(func(args...))(); /* `func(args...)`'s default return value; if `int func(args...)`, `return 0;`. If `bool func(args...)`, `return false;` */
	}
}

/* @throw std::runtime_error */
const bool classSysTests();
static const bool classSysTestsNoexcept() SUSUWU_NOEXCEPT {return templateCatchAll(classSysTests, "classSysTests()");}

}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassSys_hxx */

