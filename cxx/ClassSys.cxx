/* Dual licenses: choose "Creative Commons" or "Apache 2" (allows all uses) */
#ifndef INCLUDES_cxx_ClassSys_cxx
#define INCLUDES_cxx_ClassSys_cxx
#include "Macros.hxx" /* ERROR SUSUWU_ERRSTR SUSUWU_NOEXCEPT SUSUWU_NULLPTR SUSUWU_POSIX SUSUWU_WARNING SUSUWU_WIN32*/
#include "ClassSys.hxx" /* classSysHexStr classSysHexOs */
#include <cassert> /* assert */
#include <cerrno> /* errno */
#include <cstdlib> /* exit EXIT_FAILURE EXIT_SUCCESS getenv strtol */
#include <iostream> /* std::cerr std::cout std::endl std::flush std::ios::eofbit std::ios::goodbit */
#ifdef SUSUWU_POSIX
#include <stdexcept> /* std::runtime_error */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h> /* waitpid WIFEXITED WEXITSTATUS WIFSIGNALED WSIGTERM */
#include <unistd.h> /* execve execv fork geteuid getuid setuid */
#else
#	ifdef SUSUWU_WIN32
#	include <shlobj.h> /* IsUserAnAdmin */
#	endif /* def SUSUWU_WIN32 */
typedef int pid_t;
#endif /* def SUSUWU_POSIX */
#include <sstream> /* std::stringstream */
#include <string> /* std::string std::to_string */
#include <vector> /* std::vector */
namespace Susuwu {
int classSysArgc = 0;
const char **classSysArgs = {SUSUWU_NULLPTR};
const bool classSysInit(int argc, const char **args) {
	classSysArgc = argc;
	if(0 < argc) {
		classSysArgs = args;
		assert(SUSUWU_NULLPTR != args);
		assert(SUSUWU_NULLPTR != args[0]); /* `clangtidy` off: NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic) */
		return true;
	}
	return false;
}

const pid_t execvesFork(const std::vector<std::string> &argvS, const std::vector<std::string> &envpS) SUSUWU_NOEXCEPT {
#ifdef SUSUWU_POSIX
	const pid_t pid = fork();
	if(0 != pid) {
		if(-1 == pid) {
			SUSUWU_ERROR("execvesFork(): {(-1 == pid)}, errno=" + std::to_string(errno));
		}
		return pid;
	} /* if 0, is fork */
	const std::vector<std::string> argvSmutable = {argvS.cbegin(), argvS.cend()};
	std::vector<char *> argv;
	argv.reserve(argvSmutable.size());
	//for(auto x : argvSmutable) { /* with `fsanitize=address` this triggers "stack-use-after-scope" */
	for(const auto &x: argvSmutable /* auto x = argvSmutable.cbegin(); argvSmutable.cend() != x; ++x */) {
		argv.push_back(const_cast<char *>(x.c_str()));
	}
	argv.push_back(SUSUWU_NULLPTR);
	if(envpS.empty()) { /* Reuse LD_PRELOAD to fix https://github.com/termux-play-store/termux-issues/issues/24 */
		execv(argv[0], &argv[0]); /* NORETURN */
	} else {
		std::vector<std::string> envpSmutable = {envpS.cbegin(), envpS.cend()};
		std::vector<char *> envp;
		envp.reserve(envpSmutable.size());
		for(const auto &x: envpSmutable) {
			envp.push_back(const_cast<char *>(x.c_str()));
		}
		envp.push_back(SUSUWU_NULLPTR);
		execve(argv[0], &argv[0], &envp[0]); /* NORETURN */
	}
	exit(EXIT_FAILURE); /* execv*() is `NORETURN`. NOLINT(concurrency-mt-unsafe) */
#else /* ndef SUSUWU_POSIX */
#	undef ERROR /* undo `shlobj.h`'s `#define ERROR 0` */
	SUSUWU_ERROR("execvesFork: {#ifndef SUSUWU_POSIX /* TODO: convert to win32 */}");
	return -1;
#endif /* ndef SUSUWU_POSIX */
}
const int execves(const std::vector<std::string> &argvS, const std::vector<std::string> &envpS) {
#ifdef SUSUWU_POSIX
	const pid_t pid = execvesFork(argvS, envpS);
	int wstatus = 0;
	if(-1 == pid) {
		throw std::runtime_error(SUSUWU_ERRSTR(ERROR, "execves: -1 == execvesFork()"));
	}
	waitpid(pid, &wstatus, 0);
/* NOLINTBEGIN(misc-include-cleaner): `clang-tidy` can't detect `sys/wait.h` definitions of macros */
	if(WIFEXITED(wstatus) && 0 != WEXITSTATUS(wstatus)) {
		SUSUWU_NOTICE("execves(" + classSysColoredParamStr(argvS) + ", " + classSysColoredParamStr(envpS) + ") {if(WIFEXITED(wstatus) && 0 != WEXITSTATUS(wstatus)) {SUSUWU_NOTICE(... \"WEXITSTATUS(wstatus) is " SUSUWU_SH_PURPLE + std::to_string(WEXITSTATUS(wstatus)) + SUSUWU_SH_DEFAULT "\" ...);}}");
	} else if(WIFSIGNALED(wstatus)) {
		SUSUWU_NOTICE("execves(" + classSysColoredParamStr(argvS) + ", " + classSysColoredParamStr(envpS) + ") {if(WIFSIGNALED(wstatus)) {SUSUWU_NOTICE(... \"WTERMSIG(wstatus) is " SUSUWU_SH_PURPLE + std::to_string(WTERMSIG(wstatus)) + SUSUWU_SH_DEFAULT "\" ...);}}");
	} /* NOLINTEND(misc-include-cleaner): `clang-tidy` on */
	return wstatus;
#else /* ndef SUSUWU_POSIX */
	throw std::runtime_error(SUSUWU_ERRSTR(ERROR, "execves: {#ifndef SUSUWU_POSIX /* TODO: convert to win32 */}"));
#endif /* ndef SUSUWU_POSIX */
}

const bool classSysHasRoot() {
#ifdef SUSUWU_POSIX
	return (0 == geteuid());
#elif defined SUSUWU_WIN32
	return IsUserAnAdmin();
#else
	SUSUWU_WARNING("classSysHasRoot(bool) {#if !(defined SUSUWU_POSIX || defined SUSUWU_WIN32) /* TODO */}");
	return false;
#endif /* def SUSUWU_POSIX or def SUSUWU_WIN32 */
}
const bool classSysSetRoot(bool root) {
	if(classSysHasRoot() == root) {
		return root;
	}
#ifdef SUSUWU_POSIX
	if(root) {
		if(-1 == seteuid(0)) {
			SUSUWU_WARNING("classSysSetRoot(true) {(-1 == seteuid(0)) /* stuck as user, perhaps is not setuid executable */}");
		}
#if 0
#	ifdef __APPLE__ //TODO: https://stackoverflow.com/questions/2483755/how-to-programmatically-gain-root-privileges/35316538#35316538 says you must execute new processes to do this
#	else //TODO: https://stackoverflow.com/questions/34723861/calling-a-c-function-with-root-privileges-without-executing-the-whole-program/70149223#70149223 https://stackoverflow.com/questions/70615937/how-to-run-a-command-as-root-with-c-or-c-with-no-pam-in-linux-with-password-au https://stackoverflow.com/questions/2483755/how-to-programmatically-gain-root-privileges/2483789#2483789 says you must spawn new processes to do this
		/* TODO: polkit? Until this is finished, you must use chmod (to give setuid to executable), or execute new processes (with `sudo`/`su`) if you wish to use firewall/antivirus (which require root) */
#	endif /* __APPLE__ else */
#endif /* 0 */
	} else {
#	if 0 && defined LINUX // TODO: pam_loginuid.so(8) // https://stackoverflow.com/questions/10272784/how-do-i-get-the-users-real-uid-if-the-program-is-run-with-sudo/10272881#10272881
		uid_t sudoUid = audit_getloginuid();
#	else /* !def linux */
		uid_t sudoUid = getuid();
		if(0 == sudoUid) {
			char *sudoUidStr = getenv("SUDO_UID") /* NOLINT(concurrency-mt-unsafe) */, *sudoUidStrIt = SUSUWU_NULLPTR;
			if(SUSUWU_NULLPTR == sudoUidStr) {
				SUSUWU_WARNING("classSysSetRoot(false) {(SUSUWU_NULLPTR == getenv(\"SUDO_UID\")) /* stuck as root */}");
				return true;
			} else {
				sudoUid = static_cast<uid_t>(strtol(sudoUidStr, &sudoUidStrIt, 10));
				if(sudoUidStr == sudoUidStrIt || -1 == setuid(sudoUid)) { /* prevent reescalation to root */
					SUSUWU_WARNING("classSysSetRoot(false) {(-1 == setuid(sudoUid)) /* can't prevent reescalation to root */}");
				}
			}
		}
#	endif /* !def LINUX */
		if(0 == sudoUid) {
			SUSUWU_WARNING("classSysSetRoot(false) {(0 == sudoUid) /* stuck as root */}");
		} else if(-1 == seteuid(sudoUid)) {
			SUSUWU_WARNING("classSysSetRoot(false) {(-1 == seteuid(sudoUid)) /* stuck as root */}");
		}
	}
/* #elif defined SUSUWU_WIN32 */ //TODO: https://stackoverflow.com/questions/6418791/requesting-administrator-privileges-at-run-time says you must spawn new processes to do this
#else
	SUSUWU_WARNING("classSysSetRoot(bool) {#ifndef SUSUWU_POSIX /* TODO */}");
#endif /* SUSUWU_POSIX */
	return classSysHasRoot();
}

const bool classSysSetConsoleInput(bool input) {
	input ? std::cin.clear(std::ios::goodbit) : std::cin.setstate(std::ios::eofbit);
	return classSysGetConsoleInput();
}

static void classSysHexTests(const std::string &value) {
	const size_t ss = classSysHexStr(value).size();
	std::stringstream os;
	if(2 != ss) {
		throw std::runtime_error(SUSUWU_ERRSTR(ERROR, std::to_string(value.size()) + " == value.size(); " + std::to_string(ss) + " == classSysHexStr(value).size();"));
	}
	classSysHexOs(os, value);
	if(2 != os.str().size()) {
		throw std::runtime_error(SUSUWU_ERRSTR(ERROR, "classSysHexOs(os, value); " + std::to_string(value.size()) + " == value.size(); " + std::to_string(os.str().size()) + " == os.str().size();"));
#ifndef SUSUWU_POSIX
#	define ERROR 0 /* redo `shlobj.h`'s `#define ERROR 0` */
#endif /* ndef SUSUWU_POSIX */
	}
}
const bool classSysTests() {
	bool retval = true; /* TODO: choose all errors throw exceptions, or choose all errors return error values. Most of the other unit tests use exceptions, but `echo` is the best test for `execves`/`execvex`. */
	classSysHexTests(std::string({0}) /* test that char == 0x00 produces 2 hexits */);
	classSysHexTests("\010" /* test that char <= 0x10 produces 2 hexits */);
	classSysHexTests("\022" /* test that char >= 0x10 produces 2 hexits */);
	std::cout << "	execves(): " << std::flush;
	(EXIT_SUCCESS == execves({"/bin/echo", "pass"})) || (retval = false) || (std::cout << "error" << std::endl);
	std::cout << "	execvex(): " << std::flush;
	(EXIT_SUCCESS == execvex("/bin/echo pass")) || (retval = false) || (std::cout << "error" << std::endl);
	return retval;
}

}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassSys_cxx */

