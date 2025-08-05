/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#ifndef INCLUDES_cxx_ClassSys_cxx
#define INCLUDES_cxx_ClassSys_cxx
#include "Macros.hxx" /* SUSUWU_CXX20 SUSUWU_ERRSTR SUSUWU_IF_CPLUSPLUS SUSUWU_NOEXCEPT SUSUWU_NOTICE SUSUWU_NULLPTR SUSUWU_POSIX SUSUWU_SH_DEFAULT SUSUWU_SH_ERROR SUSUWU_SH_PURPLE SUSUWU_SH_WARNING SUSUWU_UNIT_TESTS SUSUWU_WARNING SUSUWU_WIN32 */
#include "ClassIo.hxx" /* classIoColoredParamStr classIoConsoleHasAnsiColors classIoHexStr SUSUWU_HEX_DOES_PREFIX */
#include "ClassSys.hxx" /* execves execvex */
#include SUSUWU_IF_CPLUSPLUS(<cassert>, <assert.h>) /* assert */
#include SUSUWU_IF_CPLUSPLUS(<cerrno>, <errno.h>) /* errno */
#include SUSUWU_IF_CPLUSPLUS(<cstddef>, <stddef.h>) /* size_t */
#include SUSUWU_IF_CPLUSPLUS(<cstdlib>, <stdlib.h>) /* exit EXIT_FAILURE EXIT_SUCCESS getenv strtol system */
#ifdef SUSUWU_POSIX
#endif /* def SUSUWU_POSIX */
#include <iostream> /* std::cout std::endl std::flush */
#include <stdexcept> /* std::invalid_argument */
#ifdef SUSUWU_POSIX
#include <sys/types.h> /* pid_t */
#include <sys/wait.h> /* waitpid WIFEXITED WEXITSTATUS WIFSIGNALED WSIGTERM */
#include <unistd.h> /* execve execv fork geteuid getuid setuid */
#else
#	ifdef SUSUWU_WIN32
#		include <shlobj.h> /* IsUserAnAdmin */
#		include <windows.h> /* CloseHandle CreateProcess GetLastError PROCESS_INFORMATION STARTUPINFO ZeroMemory */
#	endif /* def SUSUWU_WIN32 */
typedef int pid_t;
#endif /* def SUSUWU_POSIX */
#ifdef SUSUWU_CXX20
#	include <span> /* std::span */
#endif
#include <string> /* std::string std::to_string */
#include <vector> /* std::vector */
namespace Susuwu {
#ifdef SUSUWU_CXX20
std::span<const char *> classSysArgs({}); /* [cppcoreguidelines-pro-bounds-pointer-arithmetic] fix */
#else
int classSysArgc = 0;
const char **classSysArgs = {SUSUWU_NULLPTR};
#endif
const bool classSysInit(int argc, const char **args) {
#ifdef SUSUWU_CXX20
	classSysArgs = {args, static_cast<size_t>(argc)}; /* [cppcoreguidelines-pro-bounds-pointer-arithmetic] fix */
#else
	classSysArgc = argc;
	classSysArgs = args;
#endif
	if(0 < argc) {
		assert(SUSUWU_NULLPTR != args);
		assert(SUSUWU_NULLPTR != args[0]); /* `clangtidy` off: NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic) */
		return true;
	}
#ifndef SUSUWU_SH_SKIP_COLORS
	if(!classIoConsoleHasAnsiColors()) {
		SUSUWU_WARNING("classSysInit() {(!classIoConsoleHasAnsiColors()) /* Command Sequence Introducers disabled */}");
	}
#endif /* ndef SUSUWU_SH_SKIP_COLORS */
	return false;
}

const pid_t execvesForkThrow(const std::vector<std::string> &argvS, const std::vector<std::string> &envpS) {
	std::vector<char *> argv;
	const std::vector<std::string> argvSmutable = {argvS.cbegin(), argvS.cend()};
	argv.reserve(argvSmutable.size() + 1);
	/* Do not remove `&`. `for(auto x : argvSmutable)` with `fsanitize=address` triggers `stack-use-after-scope` */
	for(const auto &x: argvSmutable /* auto x = argvSmutable.cbegin(); argvSmutable.cend() != x; ++x */) {
		argv.push_back(const_cast<char *>(x.c_str()));
	}
	argv.push_back(SUSUWU_NULLPTR);
#ifdef SUSUWU_POSIX
	const pid_t pid = fork();
	if(0 != pid) {
		if(-1 == pid) {
			SUSUWU_ERROR("execvesFork(): {(-1 == pid)}, errno=" + std::to_string(errno));
		}
		return pid;
	} /* if 0, is fork */
	if(envpS.empty()) { /* Reuse LD_PRELOAD to fix https://github.com/termux-play-store/termux-issues/issues/24 */
		execv(argv[0], &argv[0]); /* has `noreturn` */
	} else {
		std::vector<char *> envp;
		std::vector<std::string> envpSmutable = {envpS.cbegin(), envpS.cend()}; /* NOLINT(misc-const-correctness); `const` causes `addresssanitizer` errors upon `execve()` */
		envp.reserve(envpSmutable.size() + 1);
		for(const auto &x: envpSmutable) {
			envp.push_back(const_cast<char *>(x.c_str()));
		}
		envp.push_back(SUSUWU_NULLPTR);
		execve(argv[0], &argv[0], &envp[0]); /* has `noreturn` */
	}
	exit(EXIT_FAILURE); /* execv*() has `noreturn`. NOLINT(concurrency-mt-unsafe) */
#else /* ndef SUSUWU_POSIX */
	if(1 != argvS.size()) {
		SUSUWU_ERROR("if(1 != argvS.size()) { /* TODO: non-POSIX systems (such as Win32) with multiple commands */ }");
		return -1;
	}
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	if(CreateProcess(
		NULL,    /* No module name (use command line) */
		argv[0], /* Command line */
		NULL,    /* Process handle not inheritable */
		NULL,    /* Thread handle not inheritable */
		FALSE,   /* Set handle inheritance to FALSE */
		0,       /* No creation flags */
		NULL,    /* Use parent's environment block */
		NULL,    /* Use parent's working directory */
		&si,     /* Pointer to STARTUPINFO structure */
		&pi)     /* Pointer to PROCESS_INFORMATION structure */
	) {
		SUSUWU_NOTICE("execvesFork(" + classIoColoredParamStr(argvS) + ", " + classIoColoredParamStr(envpS) + ") { if(WIFEXITED(wstatus) && 0 != WEXITSTATUS(wstatus)) { /* (preview) Win32 code */ if(CreateProcess(...)) { /* started, non-blocking */ }}}");
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	} else {
		SUSUWU_NOTICE("execvesFork(" + classIoColoredParamStr(argvS) + ", " + classIoColoredParamStr(envpS) + ") { if(WIFEXITED(wstatus) && 0 != WEXITSTATUS(wstatus)) { /* (preview) Win32 code */ if(!CreateProcess(...)) { /* failed to launch */ \"GetLastError()\" == \"" SUSUWU_SH_PURPLE + std::to_string(GetLastError()) + SUSUWU_SH_DEFAULT "\" ...); }}");
	}
	return 0;
#endif /* ndef SUSUWU_POSIX */
}
const int execves(const std::vector<std::string> &argvS, const std::vector<std::string> &envpS) {
#ifdef SUSUWU_POSIX
	const pid_t pid = execvesFork(argvS, envpS);
	int wstatus = 0;
	if(-1 == pid) {
		throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "execves: -1 == execvesFork()"));
	}
	waitpid(pid, &wstatus, 0);
/* NOLINTBEGIN(misc-include-cleaner): `clang-tidy` can't detect `sys/wait.h` definitions of macros */
	if(WIFEXITED(wstatus) && 0 != WEXITSTATUS(wstatus)) {
		SUSUWU_NOTICE("execves(" + classIoColoredParamStr(argvS) + ", " + classIoColoredParamStr(envpS) + ") { if(WIFEXITED(wstatus) && 0 != WEXITSTATUS(wstatus)) { SUSUWU_NOTICE(... \"WEXITSTATUS(wstatus) is " SUSUWU_SH_PURPLE + std::to_string(WEXITSTATUS(wstatus)) + SUSUWU_SH_DEFAULT "\" ...); }}");
	} else if(WIFSIGNALED(wstatus)) {
		SUSUWU_NOTICE("execves(" + classIoColoredParamStr(argvS) + ", " + classIoColoredParamStr(envpS) + ") { if(WIFSIGNALED(wstatus)) { SUSUWU_NOTICE(... \"WTERMSIG(wstatus) is " SUSUWU_SH_PURPLE + std::to_string(WTERMSIG(wstatus)) + SUSUWU_SH_DEFAULT "\" ...); }}");
	} /* NOLINTEND(misc-include-cleaner): `clang-tidy` on */
	return wstatus;
#else /* ndef SUSUWU_POSIX */
	if(1 != argvS.size()) {
		throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "execves: if(1 != argvS.size()) { /* TODO: non-POSIX systems (such as Win32) with multiple commands */ }"));
	}
	const int status = system(argvS[0].c_str());
	if(status) { /* cppcheck-suppress duplicateBranch */
		SUSUWU_NOTICE("execves(" + classIoColoredParamStr(argvS) + ", " + classIoColoredParamStr(envpS) + ") { if(WIFEXITED(wstatus) && 0 != WEXITSTATUS(wstatus)) { /* (preview) Win32 code */ if(!CreateProcess(...)) { /* failed to launch */ \"GetLastError()\" == \"" SUSUWU_SH_PURPLE + std::to_string(GetLastError()) + SUSUWU_SH_DEFAULT "\" ...); }}}");
	} else {
		SUSUWU_NOTICE("execves(" + classIoColoredParamStr(argvS) + ", " + classIoColoredParamStr(envpS) + ") { if(WIFEXITED(wstatus) && 0 != WEXITSTATUS(wstatus)) { /* (preview) Win32 code */ if(CreateProcess(...)) { /* started, blocking */ }}}");
	}
	return status;
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
#	ifdef __APPLE__ //TODO: [execute new process to do this?](https://stackoverflow.com/questions/2483755/how-to-programmatically-gain-root-privileges/35316538#35316538)
#	else //TODO: spawn new process to do this? https://stackoverflow.com/questions/34723861/calling-a-c-function-with-root-privileges-without-executing-the-whole-program/70149223#70149223 https://stackoverflow.com/questions/70615937/how-to-run-a-command-as-root-with-c-or-c-with-no-pam-in-linux-with-password-au https://stackoverflow.com/questions/2483755/how-to-programmatically-gain-root-privileges/2483789#2483789
		/* TODO: polkit? Until this is finished, you must use chmod (to give setuid to executable), or execute new processes (with `sudo`/`su`) if you wish to use firewall/antivirus (which require root) */
#	endif /* __APPLE__ else */
#endif /* 0 */
	} else {
#	if 0 && defined LINUX /* TODO: [use `pam_loginuid.so(8)` to do this?](https://stackoverflow.com/questions/10272784/how-do-i-get-the-users-real-uid-if-the-program-is-run-with-sudo/10272881#10272881) */
		uid_t sudoUid = audit_getloginuid();
#	else /* !def linux */
		uid_t sudoUid = getuid();
		if(0 == sudoUid) {
			const char *sudoUidStr = getenv("SUDO_UID") /* NOLINT(concurrency-mt-unsafe) */;
			char *sudoUidStrIt = SUSUWU_NULLPTR;
			if(SUSUWU_NULLPTR == sudoUidStr) {
				SUSUWU_WARNING("classSysSetRoot(false) {(SUSUWU_NULLPTR == getenv(\"SUDO_UID\")) /* stuck as root */}");
				return true;
			} else {
				static const int baseDecimal = 10;
				sudoUid = static_cast<uid_t>(strtol(sudoUidStr, &sudoUidStrIt, baseDecimal));
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
// #elif defined SUSUWU_WIN32 /* TODO: [spawn new process to do this?](https://stackoverflow.com/questions/6418791/requesting-administrator-privileges-at-run-time) */
#else
	SUSUWU_WARNING("classSysSetRoot(bool) {#ifndef SUSUWU_POSIX /* TODO */}");
#endif /* SUSUWU_POSIX */
	return classSysHasRoot();
}

#if SUSUWU_UNIT_TESTS
const bool classSysTests() {
	bool retval = true; /* TODO: choose all errors throw exceptions, or choose all errors return error values. Most of the other unit tests use exceptions, but `echo` is the best test for `execves`/`execvex`. */
	std::cout << "	execves({\"/bin/echo\", \"passes\"}) " << std::flush;
	int exitCode = execves({
#ifndef SUSUWU_WIN32
			"/bin/"
#endif /* ndef SUSUWU_WIN32 */
			"echo", "passes"});
	if(EXIT_SUCCESS != exitCode) {
		retval = false;
		std::cout << "== " << std::to_string(exitCode) << std::endl;
	}
	std::cout << "	execvex(\"/bin/echo passes\") " << std::flush;
	exitCode = execvex({
#ifndef SUSUWU_WIN32
			"/bin/"
#endif /* ndef SUSUWU_WIN32 */
			"echo passes"});
	if(EXIT_SUCCESS != exitCode) {
		retval = false;
		std::cout << "== " << std::to_string(exitCode) << std::endl;
	}
	return retval;
}
#endif /* SUSUWU_UNIT_TESTS */

}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassSys_cxx */

