/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#pragma once
#ifndef INCLUDES_cxx_ClassSys_hxx
#define INCLUDES_cxx_ClassSys_hxx
#include "Macros.hxx" /* SUSUWU_CXX20 SUSUWU_ERROR SUSUWU_NOEXCEPT SUSUWU_IF_CPLUSPLUS SUSUWU_POSIX SUSUWU_SH_DEFAULT SUSUWU_SH_ERROR SUSUWU_SH_GREEN SUSUWU_SH_RED SUSUWU_STATIC_ASSERT SUSUWU_UNIT_TESTS SUSUWU_WARNING */
#ifndef SUSUWU_HEX_TABLE
#	define SUSUWU_HEX_TABLE false
#endif /* ndef SUSUWU_HEX_TABLE */
#if SUSUWU_HEX_TABLE
#	include <array> /* std::array */
#endif /* SUSUWU_HEX_TABLE */
#include SUSUWU_IF_CPLUSPLUS(<cassert>, <assert.h>) /* assert */
#if SUSUWU_HEX_TABLE
#	include SUSUWU_IF_CPLUSPLUS(<climits>, <limits.h>) /* CHAR_BIT */
#else /* SUSUWU_HEX_TABLE else */
#	include SUSUWU_IF_CPLUSPLUS(<cctype>, <ctype.h>) /* isupper isxdigit */
#endif /* SUSUWU_HEX_TABLE else */
#include <chrono> /* std::chrono */
#include SUSUWU_IF_CPLUSPLUS(<cstddef>, <stddef.h>) /* size_t */
#include <exception> /* std::exception */
#include <iomanip> /* std::setw */
#include <ios> /* std::hex */
#include <iosfwd> /* std::streampos */
#include <iostream> /* std::cin */
#include <istream> /* std::basic_istream */
#include <stdexcept> /* std::runtime_error */
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
#ifndef SUSUWU_HEX_DOES_PREFIX
#	define SUSUWU_HEX_DOES_PREFIX false
#endif /* ndef SUSUWU_HEX_DOES_PREFIX */
#define SUSUWU_HEX_PREFIX_SZ (SUSUWU_HEX_DOES_PREFIX ? 2 : 0)
#ifndef SUSUWU_DASHOS
#	define SUSUWU_DASHOS false /* `-Os` param */
#endif /* ndef SUSUWU_DASHOS */
#ifndef SUSUWU_IO_THROW
#	define SUSUWU_IO_THROW false /* `if(SUSUWU_IO_THROW) { classIoCheckStr(__func__, expectedValue, value); } else { if(expectedValue != value) { is.setstate(std::ios::failbit); } }` */
#endif /* ndef SUSUWU_IO_THROW */
#ifndef SUSUWU_USE_STD_HEX
#	define SUSUWU_USE_STD_HEX false /* prefer `std::hex` for conversions */
#endif /* ndef SUSUWU_STD_HEX */
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
const unsigned char classSysGetConsoleAttributes(); /* if(_WIN32 || ) { return (background * 16) + foreground color; } else if(_POSIX_SOURCE) { return "\033[%1;%2m" -> (%1 * 16) + %2 ; } else { return 0; } */
const bool classSysConsoleHasAnsiColors();

template<class Os, class Int,
	typename std::enable_if<std::is_integral<Int>::value, int>::type = 0>
inline Os &classSysHexOs(Os &os, const Int &value) {
#if SUSUWU_HEX_DOES_PREFIX
	os << "0x";
#endif /* SUSUWU_HEX_DOES_PREFIX */
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
#if SUSUWU_HEX_DOES_PREFIX
	os << "0x";
#endif /* SUSUWU_HEX_DOES_PREFIX */
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
} /* TODO: move into `ClassStr.hxx`? */

static const size_t classSysAscii7Bit = (1 << 7);
SUSUWU_STATIC_ASSERT(0x80 == classSysAscii7Bit); /* ASCII, 7bit*/
#if SUSUWU_HEX_TABLE
static const std::array<signed char,
#	if SUSUWU_DASHOS
						 classSysAscii7Bit
#	else /* SUSUWU_DASHOS else */
						 1 << CHAR_BIT
#	endif /* SUSUWU_DASHOS else */
						 > classSysHex2Nib = {
	/* memset(&classSysHex2Nib[0x00], -1, 48) */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* [0x30] = '0' */ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, /* memset(&classSysHex2Nib[0x3A], -1, 7) */ -1, -1, -1, -1, -1, -1, -1, /* [0x41] = 'A' */ 10, 11, 12, 13, 14, 15, /* memset(&classSysHex2Nib[0x48], -1, 26) */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* [0x61] = 'a' */ 10, 11, 12, 13, 14, 15, /* memset(&classSysHex2Nib[0x67], -1, 25) */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
#	if 8 < CHAR_BIT
#		error "TODO: 8 < CHAR_BIT"
#	elif 7 < CHAR_BIT && !SUSUWU_DASHOS
	, /* memset(&classSysHex2Nib[classSysAscii7Bit], -1, (1 << CHAR_BIT) - classSysAscii7Bit) */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
#	endif /* 7 < CHAR_BIT */
}; /* TODO: move into `ClassStr.hxx`? */
#endif /* SUSUWU_HEX_TABLE */
inline const bool classSysIsXdigit(const unsigned char char_) {
#if SUSUWU_HEX_TABLE
#	if SUSUWU_DASHOS
	return (-1 != classSysHex2Nib.at(char_));
#	else /* SUSUWU_DASHOS else */
	return (-1 != classSysHex2Nib[char_]); /* NOLINT(cppcoreguidelines-pro-bounds-constant-array-index) */
#	endif /* SUSUWU_DASHOS else */
#else /* SUSUWU_HEX_TABLE else */
	return (0 != isxdigit(char_));
#endif /* SUSUWU_HEX_TABLE */
} /* TODO: move into `ClassStr.hxx`? */
inline unsigned char classSysHexitToNibble(const unsigned char hexit) {
#if SUSUWU_HEX_TABLE
	assert(-1 != classSysHex2Nib.at(hexit)); /* Min valid = 0x30, max valid = 0x66 */
#	if SUSUWU_DASHOS
	return classSysHex2Nib.at(hexit);
#	else /* SUSUWU_DASHOS else */
	return classSysHex2Nib[hexit]; /* NOLINT(cppcoreguidelines-pro-bounds-constant-array-index) */
#	endif /* SUSUWU_DASHOS else */
#else /* SUSUWU_HEX_TABLE else */
	static const size_t aToHex = 10;
	assert(0 != isxdigit(hexit));
	if(0 == isupper(hexit)) {
		return hexit - '0';
	} else {
		return hexit - 'A' + aToHex;
	}
#endif /* SUSUWU_HEX_TABLE else */
} /* TODO: move into `ClassStr.hxx`? */
inline unsigned char classSysHex2Char(unsigned char hexit1, unsigned char hexit2) {
		hexit1 = classSysHexitToNibble(hexit1);
		hexit2 = classSysHexitToNibble(hexit2);
		return static_cast<unsigned char>((hexit1 << 4) | hexit2);
} /* TODO: move into `ClassStr.hxx`? */
template<class Is>
/* Usage: @code classSysDebugIs(__func__, is); @endcode */
static void classSysDebugIs(const std::string &func, Is &is) {
#ifdef NDEBUG
	(void)func; (void)is; /* suppress `clang-tidy`/`lint` notices */
#else /* ndef NDEBUG */
	const std::streampos pos = is.tellg();
	std::string token;
	if(!is.good()) {
		SUSUWU_DEBUG("(!is.good())");
		return;
	} /* auto isState = is.getstate(); */
	if(is >> token) {
		SUSUWU_DEBUG(func + ": is == \"" + token + "\"");
	} else if(is.fail() && !(is.bad() || is.eof())) {
		is.clear(std::ios::goodbit); /* is.setstate(isState); */
	}
	is.seekg(pos);
#endif /* ndef NDEBUG */
}
template<class Is, class Int,
	typename std::enable_if<std::is_integral<Int>::value, int>::type = 0>
/* @pre @code is.good(); @endcode */
inline Is &classSysHexIs(Is &is, Int &value) {
	const std::ios::fmtflags oldFlags = is.flags();
	const char oldFill = is.fill();
	is << std::hex;
	is.fill('0');
	/* classSysDebugIs(std::string(__func__) + "(pre)", is); */
	if(!is.good()) {
		SUSUWU_NOTICE("(!is.good())");
		return is;
	} /* auto isState = is.getstate(); */
	std::streampos pos = is.tellg();
	if(is >> std::hex >> std::setw(2)/* `setw` is unset after each use */ >> value) {
		const std::streampos newPos = is.tellg();
		/* SUSUWU_DEBUG("value == 0x" + classSysHexStr(value) + ", pos += " + std::to_string(newPos - pos)); */
		pos = newPos;
	} else if(is.fail() && !(is.bad() || is.eof())) {
		is.seekg(pos); /* TODO: prove if this can be removed for all flags*/
		is.clear(std::ios::goodbit); /* is.setstate(isState); */
	}
	/* classSysDebugIs(std::string(__func__) + "(post)", is); */
	is.fill(oldFill);
	is.flags(oldFlags);
	return is;
} /* TODO: refactor this and the `Str` version, to reuse common code */
template<class Is, class Str,
	typename std::enable_if<!std::is_integral<Str>::value, int>::type = 0>
/* @pre @code is.good(); @endcode */
inline Is &classSysHexIs(Is &is, Str &value) {
	value.clear(); /* TODO; move after `if(!is.good()) { ... return is; }` (to preserve `value` if precondition fails)? */
	if(!is.good()) {
		SUSUWU_DEBUG("(!is.good())"); /* TODO; remove (since caller should check `is.good()`)? */
		return is;
	} /* auto isState = is.getstate(); */
	std::streampos pos = is.tellg();
#if SUSUWU_USE_STD_HEX /* TODO: fix or remove this code path */
	const std::ios::fmtflags oldFlags = is.flags();
	const char oldFill = is.fill();
	is << std::hex;
	is.fill('0');
	/* classSysDebugIs(std::string(__func__) + "(pre)", is); */
	for(unsigned int ch; is.good() && is >> std::hex >> std::setw(2)/* `setw` is unset after each use */ >> ch; ) {
		const std::streampos newPos = is.tellg();
		/* SUSUWU_DEBUG("ch == 0x" + classSysHexStr(ch) + ", pos += " + std::to_string(newPos - pos)); */
		for(auto i = ((newPos - pos) >> 1) - (SUSUWU_HEX_DOES_PREFIX ? 1 : 0); i--; ) {
			value += reinterpret_cast<unsigned char *>(&ch)[i];
		}
		pos = newPos;
	}
	is.fill(oldFill);
	is.flags(oldFlags);
	if(is.fail() && !(is.bad() || is.eof())) { /* If stopped due to non-hex input, */
		is.seekg(pos); /* restore non-hex input (which was consumed). */
		is.clear(std::ios::goodbit); /* `is.setstate(isState);` */
	}
#else /* SUSUWU_USE_STD_HEX else */
#	if SUSUWU_HEX_DOES_PREFIX
#		if SUSUWU_PREFER_GETLINE
	std::string prefix;
	std::getline(is, prefix, 'x'); /* used `std::getline` so 'x' is swallowed */
	if("0" != prefix) {
#		else /* SUSUWU_PREFER_GETLINE else */
	char prefix[2] = {'\0', '\0'};
	is >> prefix[0];
	if(!('0' == prefix[0] && is >> prefix[1] && 'x' == prefix[1])) {
#		endif /* SUSUWU_PREFER_GETLINE else */
		is.seekg(pos);
#		if SUSUWU_IO_THROW
		classSysCheckStr(__func__, "0x", prefix); /* TODO: choose `std::ios::failbit` or `throw` */
#		else /* SUSUWU_IO_THROW else */
		SUSUWU_DEBUG("(SUSUWU_HEX_DOES_PREFIX && (prefix != \"" SUSUWU_SH_GREEN "0x" SUSUWU_SH_DEFAULT "\") && (prefix == \"" SUSUWU_SH_RED + std::string(prefix) + SUSUWU_SH_DEFAULT "\"))");
		is.setstate(std::ios::failbit);
#		endif /* SUSUWU_IO_THROW else */
		return is;
	}
	pos = is.tellg();
#	endif /* SUSUWU_HEX_DOES_PREFIX */
	for(unsigned char hexit1 = '\0', hexit2 = '\0'; is.good() && is >> hexit1; ) {
		if(0 == classSysIsXdigit(hexit1) || (is >> hexit2 && 0 == classSysIsXdigit(hexit2))) {
			if(!is.bad()) {
				is.seekg(pos);
				is.clear(std::ios::goodbit); /* is.setstate(isState); */
			}
			break;
		}
		pos = is.tellg();
		/* SUSUWU_DEBUG("hexit1 == \"" + classSysHexStr(hexit1) + "\", hexit2 == \"" + classSysHexStr(hexit2) + '"'); */
		value += classSysHex2Char(hexit1, hexit2);
	}
#endif /* SUSUWU_USE_STD_HEX else */
	/* classSysDebugIs(std::string(__func__) + "(post)", is); */
	return is;
}
template<class Is, class Value>
/* usage: @code U value = classSysHexIs<U>(is); @endcode
 * @pre @code is.good(); @endcode */
inline const Value classSysHexIs(Is &is) {
	Value value;
	classSysHexIs(is, &value);
	return value;
}
template <class CharT, class Traits, class Allocator>
/* Usage: is as `std::getline` except that `delim` is not consumed
 * @pre @code is.good(); @endcode */
static std::basic_istream<CharT, Traits>& classSysGetline(std::basic_istream<CharT, Traits>& is, std::basic_string<CharT, Traits, Allocator>& str, CharT delim = '\n') {
	str.clear();
	for(char token = delim; is.good(); str += token) {
		const std::streampos pos = is.tellg();
		is >> token;
		if(delim == token) {
			is.seekg(pos);
			return is;
		}
	}
	return is;
}
/* Usage: @code is >> value; classSysCheckChar(__func__, '{', value); @endcode */
inline void classSysCheckChar(const std::string &func, const char expected, const char got) {
	if(expected != got) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, func + ": expected '" SUSUWU_SH_GREEN + expected + SUSUWU_SH_DEFAULT "', got '" SUSUWU_SH_RED + got + SUSUWU_SH_DEFAULT "'"));
	} /* classSysCheckStr(func, std::string(expected), got); */
}
/* Usage: @code is >> value; classSysCheckSz(__func__, 42, value); @endcode */
inline void classSysCheckSz(const std::string &func, const size_t expected, const size_t got) {
	if(expected != got) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, func + ": expected '" SUSUWU_SH_GREEN + std::to_string(expected) + SUSUWU_SH_DEFAULT "', got '" SUSUWU_SH_RED + std::to_string(got) + SUSUWU_SH_DEFAULT "'"));
	} /* classSysCheckStr(func, std::to_string(expected), std::to_string(got)); */
}
/* Usage: @code if(rand() % 2) {is >> value;} else {classSysGetline(is, value);} classSysCheckStr(__func__, "};", value); @endcode */
static void classSysCheckStr(const std::string &func, const std::string &expected, const std::string &got) {
	if(expected != got) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, func + ": expected '" SUSUWU_SH_GREEN + expected + SUSUWU_SH_DEFAULT "', got '" + SUSUWU_SH_RED + got + SUSUWU_SH_DEFAULT "'"));
	}
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
