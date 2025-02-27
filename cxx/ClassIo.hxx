/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#pragma once
#ifndef INCLUDES_cxx_ClassIo_hxx
#define INCLUDES_cxx_ClassIo_hxx
#include "Macros.hxx" /* SUSUWU_IF_CPLUSPLUS SUSUWU_NOEXCEPT SUSUWU_SH_DEFAULT SUSUWU_SH_ERROR SUSUWU_SH_GREEN SUSUWU_SH_RED SUSUWU_UNIT_TESTS */
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
#include SUSUWU_IF_CPLUSPLUS(<cstdio>, <stdio.h>) /* FILE */
#include <iomanip> /* std::setw */
#include <ios> /* std::hex */
#include <iosfwd> /* std::streampos */
#include <istream> /* std::basic_istream */
#include <sstream> /* std::stringstream */
#include <stdexcept> /* std::runtime_error */
#include <string> /* std::string std::to_string */
#include <type_traits> /* std::enable_if */
/* Abstractions to do with filesystems (such as `procfs`) */
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
#ifndef SUSUWU_IO_WHITESPACE
#	define SUSUWU_IO_WHITESPACE false /* true if `getline` includes characters such as '\n' */
#endif /* ndef SUSUWU_IO_WHITESPACE */
typedef std::string ClassIoPath; /* TODO: `std::char_traits<unsigned char>`, `std::basic_string<unsigned char>("string literal")` */
typedef ClassIoPath ClassIoBytecode; /* Uses `std::string` for bytecode (versus `std::vector`) because:
 * "If you are going to use the data in a string like fashon then you should opt for std::string as using a std::vector may confuse subsequent maintainers. If on the other hand most of the data manipulation looks like plain maths or vector like then a std::vector is more appropriate." -- https://stackoverflow.com/a/1556294/24473928
*/
typedef ClassIoPath ClassIoHash; /* TODO: `std::unordered_set<std::basic_string<unsigned char>>` */

/* Usage: for Linux (or Windows,) if you don't trust `argv[0]`, replace it with `classIoGetOwnPath()`.
 * Error values: `return ClassIoPath();` */
const ClassIoPath classIoGetOwnPath() /* TODO: SUSUWU_NOEXCEPT(std::is_nothrow_constructible<ClassIoPath>::value) */;
const FILE *classIoFopenOwnPath() /* TODO: SUSUWU_NOEXCEPT(std::is_nothrow_invocable<classIoGetClassIoPath()>::value) */;

template<class Os, class Int,
	typename std::enable_if<std::is_integral<Int>::value, int>::type = 0>
inline Os &classIoHexOs(Os &os, const Int &value) {
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
inline const std::string classIoHexStr(const Int &value) {
	std::stringstream os;
	classIoHexOs(os, value);
	return os.str();
}
template<class Os, class Str,
	typename std::enable_if<!std::is_integral<Str>::value, int>::type = 0>
inline Os &classIoHexOs(Os &os, const Str &value) {
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
inline const Str classIoHexStr(const Str &value) {
	std::stringstream os;
	classIoHexOs(os, value);
	return os.str();
}
template<class Os, class List>
inline Os &classIoColoredParamOs(Os &os, const List &argvS, const bool parenthesis/* {...} */ = true) {
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
inline const typename List::value_type classIoColoredParamStr(const List &argvS, const bool parenthesis/* {...} */ = true) {
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
static const size_t classIoAscii7Bit = (1 << 7);
SUSUWU_STATIC_ASSERT(0x80 == classIoAscii7Bit); /* ASCII, 7bit*/
#if SUSUWU_HEX_TABLE
static const std::array<signed char,
#	if SUSUWU_DASHOS
						 classIoAscii7Bit
#	else /* SUSUWU_DASHOS else */
						 1 << CHAR_BIT
#	endif /* SUSUWU_DASHOS else */
						 > classIoHex2Nib = {
	/* memset(&classIoHex2Nib[0x00], -1, 48) */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* [0x30] = '0' */ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, /* memset(&classIoHex2Nib[0x3A], -1, 7) */ -1, -1, -1, -1, -1, -1, -1, /* [0x41] = 'A' */ 10, 11, 12, 13, 14, 15, /* memset(&classIoHex2Nib[0x48], -1, 26) */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* [0x61] = 'a' */ 10, 11, 12, 13, 14, 15, /* memset(&classIoHex2Nib[0x67], -1, 25) */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
#	if 8 < CHAR_BIT
#		error "TODO: 8 < CHAR_BIT"
#	elif 7 < CHAR_BIT && !SUSUWU_DASHOS
	, /* memset(&classIoHex2Nib[classIoAscii7Bit], -1, (1 << CHAR_BIT) - classIoAscii7Bit) */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
#	endif /* 7 < CHAR_BIT */
}; /* TODO: move into `ClassStr.hxx`? */
#endif /* SUSUWU_HEX_TABLE */
inline const bool classIoIsXdigit(const unsigned char char_) {
#if SUSUWU_HEX_TABLE
#	if SUSUWU_DASHOS
	return (-1 != classIoHex2Nib.at(char_));
#	else /* SUSUWU_DASHOS else */
	return (-1 != classIoHex2Nib[char_]); /* NOLINT(cppcoreguidelines-pro-bounds-constant-array-index) */
#	endif /* SUSUWU_DASHOS else */
#else /* SUSUWU_HEX_TABLE else */
	return (0 != isxdigit(char_));
#endif /* SUSUWU_HEX_TABLE */
} /* TODO: move into `ClassStr.hxx`? */
inline unsigned char classIoHexitToNibble(const unsigned char hexit) {
#if SUSUWU_HEX_TABLE
	assert(-1 != classIoHex2Nib.at(hexit)); /* Min valid = 0x30, max valid = 0x66 */
#	if SUSUWU_DASHOS
	return classIoHex2Nib.at(hexit);
#	else /* SUSUWU_DASHOS else */
	return classIoHex2Nib[hexit]; /* NOLINT(cppcoreguidelines-pro-bounds-constant-array-index) */
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
inline unsigned char classIoHex2Char(unsigned char hexit1, unsigned char hexit2) {
		hexit1 = classIoHexitToNibble(hexit1);
		hexit2 = classIoHexitToNibble(hexit2);
		return static_cast<unsigned char>((hexit1 << 4) | hexit2);
} /* TODO: move into `ClassStr.hxx`? */
template<class Is>
/* Usage: @code classIoDebugIs(__func__, is); @endcode */
static void classIoDebugIs(const std::string &func, Is &is) {
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
inline Is &classIoHexIs(Is &is, Int &value) {
	const std::ios::fmtflags oldFlags = is.flags();
	const char oldFill = is.fill();
	is << std::hex;
	is.fill('0');
	/* classIoDebugIs(std::string(__func__) + "(pre)", is); */
	if(!is.good()) {
		SUSUWU_NOTICE("(!is.good())");
		return is;
	} /* auto isState = is.getstate(); */
	std::streampos pos = is.tellg();
	if(is >> std::hex >> std::setw(2)/* `setw` is unset after each use */ >> value) {
		const std::streampos newPos = is.tellg();
		/* SUSUWU_DEBUG("value == 0x" + classIoHexStr(value) + ", pos += " + std::to_string(newPos - pos)); */
		pos = newPos;
	} else if(is.fail() && !(is.bad() || is.eof())) {
		is.seekg(pos); /* TODO: prove if this can be removed for all flags*/
		is.clear(std::ios::goodbit); /* is.setstate(isState); */
	}
	/* classIoDebugIs(std::string(__func__) + "(post)", is); */
	is.fill(oldFill);
	is.flags(oldFlags);
	return is;
} /* TODO: refactor this and the `Str` version, to reuse common code */
template<class Is, class Str,
	typename std::enable_if<!std::is_integral<Str>::value, int>::type = 0>
/* @pre @code is.good(); @endcode */
inline Is &classIoHexIs(Is &is, Str &value) {
	value.clear(); /* TODO; move after `if(!is.good()) { ... return is; }` (to preserve `value` if precondition fails)? */
	if(!is.good()) {
		SUSUWU_DEBUG("(!is.good())"); /* TODO; remove (since caller should check `is.good()`)? */
		return is;
	} /* auto isState = is.getstate(); */
	std::streampos pos = is.tellg();
#if SUSUWU_USE_CXX_HEX /* TODO: fix or remove this code path */
	const std::ios::fmtflags oldFlags = is.flags();
	const char oldFill = is.fill();
	is << std::hex;
	is.fill('0');
	/* classIoDebugIs(std::string(__func__) + "(pre)", is); */
	for(unsigned int ch; is.good() && is >> std::hex >> std::setw(2)/* `setw` is unset after each use */ >> ch; ) {
		const std::streampos newPos = is.tellg();
		/* SUSUWU_DEBUG("ch == 0x" + classIoHexStr(ch) + ", pos += " + std::to_string(newPos - pos)); */
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
#else /* SUSUWU_USE_CXX_HEX else */
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
		classIoCheckStr(__func__, "0x", prefix); /* TODO: choose `std::ios::failbit` or `throw` */
#		else /* SUSUWU_IO_THROW else */
		SUSUWU_DEBUG("(SUSUWU_HEX_DOES_PREFIX && (prefix != \"" SUSUWU_SH_GREEN "0x" SUSUWU_SH_DEFAULT "\") && (prefix == \"" SUSUWU_SH_RED + std::string(prefix) + SUSUWU_SH_DEFAULT "\"))");
		is.setstate(std::ios::failbit);
#		endif /* SUSUWU_IO_THROW else */
		return is;
	}
	pos = is.tellg();
#	endif /* SUSUWU_HEX_DOES_PREFIX */
	for(unsigned char hexit1 = '\0', hexit2 = '\0'; is.good() && is >> hexit1; ) {
		if(0 == classIoIsXdigit(hexit1) || (is >> hexit2 && 0 == classIoIsXdigit(hexit2))) {
			if(!is.bad()) {
				is.seekg(pos);
				is.clear(std::ios::goodbit); /* is.setstate(isState); */
			}
			break;
		}
		pos = is.tellg();
		/* SUSUWU_DEBUG("hexit1 == \"" + classIoHexStr(hexit1) + "\", hexit2 == \"" + classIoHexStr(hexit2) + '"'); */
		value += classIoHex2Char(hexit1, hexit2);
	}
#endif /* SUSUWU_USE_CXX_HEX else */
	/* classIoDebugIs(std::string(__func__) + "(post)", is); */
	return is;
}
template<class Is, class Value>
/* usage: @code U value = classIoHexIs<U>(is); @endcode
 * @pre @code is.good(); @endcode */
inline const Value classIoHexIs(Is &is) {
	Value value;
	classIoHexIs(is, &value);
	return value;
}
template <class CharT, class Traits, class Allocator>
/* Usage: is as `std::getline` except that `delim` is not consumed
 * @pre @code is.good(); @endcode */
static std::basic_istream<CharT, Traits>& classIoGetline(std::basic_istream<CharT, Traits>& is, std::basic_string<CharT, Traits, Allocator>& str, CharT delim = '\n') {
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
/* Usage: @code is >> value; classIoCheckChar(__func__, '{', value); @endcode */
inline void classIoCheckChar(const std::string &func, const char expected, const char got) {
	if(expected != got) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, func + ": expected '" SUSUWU_SH_GREEN + expected + SUSUWU_SH_DEFAULT "', got '" SUSUWU_SH_RED + got + SUSUWU_SH_DEFAULT "'"));
	} /* classIoCheckStr(func, std::string(expected), got); */
}
/* Usage: @code is >> value; classIoCheckSz(__func__, 42, value); @endcode */
inline void classIoCheckSz(const std::string &func, const size_t expected, const size_t got) {
	if(expected != got) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, func + ": expected '" SUSUWU_SH_GREEN + std::to_string(expected) + SUSUWU_SH_DEFAULT "', got '" SUSUWU_SH_RED + std::to_string(got) + SUSUWU_SH_DEFAULT "'"));
	} /* classIoCheckStr(func, std::to_string(expected), std::to_string(got)); */
}
/* Usage: @code if(rand() % 2) {is >> value;} else {classIoGetline(is, value);} classIoCheckStr(__func__, "};", value); @endcode */
static void classIoCheckStr(const std::string &func, const std::string &expected, const std::string &got) {
	if(expected != got) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, func + ": expected '" SUSUWU_SH_GREEN + expected + SUSUWU_SH_DEFAULT "', got '" + SUSUWU_SH_RED + got + SUSUWU_SH_DEFAULT "'"));
	}
}

#if SUSUWU_UNIT_TESTS
/* @throw std::runtime_error */
const bool classIoTests();
const bool classIoTestsNoexcept() SUSUWU_NOEXCEPT;
#endif /* SUSUWU_UNIT_TESTS */

}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassIo_hxx */

