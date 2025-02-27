/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#pragma once
#ifndef INCLUDES_cxx_ClassResultList_hxx
#define INCLUDES_cxx_ClassResultList_hxx
#include "ClassIo.hxx" /* ClassIoPath ClassIoBytecode ClassIoHash classIoCheckChar classIoCheckStr classIoCheckSz classIoGetline classIoHexOs SUSUWU_HEX_DOES_PREFIX SUSUWU_IO_WHITESPACE */
#include "ClassObject.hxx" /* Object SUSUWU_VIRTUAL_DEFAULTS() */
#include "ClassSha2.hxx" /* classSha2 */
#include "ClassSys.hxx" /* templateCatchAll */
#include "Macros.hxx" /* SUSUWU_IF_CPLUSPLUS SUSUWU_NOEXCEPT SUSUWU_OVERRIDE SUSUWU_PREFER_CSTR SUSUWU_UNIT_TESTS SUSUWU_UNREACHABLE */
#include <algorithm> /* std::search std::find std::set_intersection */
#include SUSUWU_IF_CPLUSPLUS(<cstddef>, <stddef.h>) /* size_t */
#include <iostream> /* std::endl std::getline std::streamsize */
//#include <stdexcept> /* std::runtime_error */
#if SUSUWU_PREFER_CSTR
#	include <cstring> /* strlen memmem */
#endif /* SUSUWU_PREFER_CSTR */
#include <string> /* std::string */
#include <tuple> /* std::tuple std::get */
#include <unordered_set> /* std::unordered_set */
#include <vector> /* std::vector */
namespace Susuwu {
typedef ClassIoHash ResultListHash;
typedef ClassIoBytecode ResultListBytecode; /* Should have structure of ClassIoBytecode, but is not just for files, can use for UTF8/webpages, so have a new type for this */
typedef ClassIoPath ResultListSignature; /* TODO: `typedef ResultListBytecode ResultListSignature; ResultListSignature("string literal");` */
typedef ptrdiff_t BytecodeOffset; /* all tests of `ResultListBytecode` should return `{BytecodeOffset, X}` (with the most common `X` as `ResultListHash` or `ResultListSignature`). `offset = -1` if no match */
typedef struct ResultList : public Object { /* Lists of {metadata, executables (or pages)} */
	SUSUWU_VIRTUAL_DEFAULTS(Susuwu::ResultList) /* `getName()`, `isPureVirtual()`, `operator==`()`, ... */
/* `clang-tidy` off: NOLINTBEGIN(misc-non-private-member-variables-in-classes) */
	typedef std::unordered_set<ResultListHash> Hashes;
	Hashes hashes; /* Checksums of executables (or pages); to avoid duplicates, plus to do constant ("O(1)") test for which executables (or pages) exists */
	typedef std::vector<ResultListSignature> Signatures;
	Signatures signatures; /* Smallest substrings (or regexes, or Universal Resource Locators) which can identify `bytecodes`; has uses close to `hashes`, but can match if executables (or pages) have small differences */
	typedef std::vector<ResultListBytecode> Bytecodes;
	Bytecodes bytecodes; /* Whole executables (for `VirusAnalysis`) or webpages (for `AssistantCns`); huge disk usage, just load this for signature synthesis (or CNS backpropagation). */
/* `clang-tidy` on: NOLINTEND(misc-non-private-member-variables-in-classes) */
} ResultList;

#if SUSUWU_UNIT_TESTS
const bool classResultListTests(); /* TODO: test most of `ClassResultList*` */
static const bool classResultListTestsNoexcept() SUSUWU_NOEXCEPT { return templateCatchAll(classResultListTests, "classResultListTests()"); }
#endif /* SUSUWU_UNIT_TESTS */

template<class List>
const size_t listMaxSize(const List &list) {
#if SUSUWU_PREFER_CSTR
	size_t max = 0;
	for(auto it = &list[0]; list.cend() != it; ++it) { const size_t temp = strlen(*it); if(temp > max) { max = temp; } }
	return max; /* WARNING! `strlen()` just does UTF8-strings/hex-strings; if binary, must use `it->size()` */
#else /* else !SUSUWU_PREFER_CSTR */
	auto it = std::max_element(list.cbegin(), list.cend(), [](const typename List::const_iterator::value_type &s, const typename List::const_iterator::value_type &x) { return s.size() < x.size(); });
	return it->size();
#endif /* SUSUWU_PREFER_CSTR else */
}

typedef enum ListFormat {
	listFormatInitializer, /* style: C or C++ */
	listFormatJson /* style: Java or JavaScript */
} ListFormat;
template<class List, class Os>
void listDumpTo(const List &list, Os &os, const bool index, const bool whitespace, const bool pascalValues, const bool finalList = true, const ListFormat listFormat = listFormatInitializer) {
	bool quoteValues = false;
	size_t index_ = 0;
	switch(listFormat) {
		case listFormatInitializer:
			os << '{';
		break;
		case listFormatJson:
			os << '[';
			quoteValues = true;
		break;
	}
#ifdef SUSUWU_LIST_COUNT
	os << list.size() << ':';
#endif /* def SUSUWU_LIST_COUNT */
	const std::string assignment = whitespace ? " = " : "=";
	for(const auto &value : list) {
		if(0 != index_) {
			os << ',';
		}
		if(whitespace) {
			os << std::endl << '\t';
		}
		if(index) {
			os << index_ << assignment;
		}
		if(quoteValues) {
			os << '"';
		}
		if(pascalValues) {
			os << value.size() << ':' /* TODO: replace "%Dec:" with "%Bin" */ << value;
		} else {
#if !SUSUWU_HEX_DOES_PREFIX
			os << "0x";
#endif /* !SUSUWU_HEX_DOES_PREFIX */
			classIoHexOs(os, value);
		}
		if(quoteValues) {
			os << '"';
		}
		++index_;
	}
	if(whitespace) {
		os << '\n';
	}
	switch(listFormat) {
		case listFormatInitializer:
			os << "};";
		break;
		case listFormatJson:
			os << (finalList ? "]" : "],");
		break;
	}
	if(whitespace) {
		os << std::endl;
	}
} /* view `ClassResultList.cxx:classResultListTests()` for examples of output from `listDumpTo()`+`resultListDumpTo()`. */
template<class Os>
void resultListDumpToAssignment(const std::string &listStr, Os &os, const bool whitespace, const ListFormat listFormat = listFormatInitializer) {
	switch(listFormat) {
		case listFormatInitializer:
			os << listStr << (whitespace ? " = " : "=");
		break;
		case listFormatJson:
			os << '"' << listStr << '"' << (whitespace ? ": " : ":");
		break;
	}
}
template<class List, class Os>
void resultListDumpTo(const List &list, Os &os, const bool index, const bool whitespace, const bool pascalValues, const ListFormat listFormat = listFormatInitializer) {
	if(listFormatJson == listFormat) {
		os << '{';
		if(whitespace) {
			os << '\n';
		}
	}
	resultListDumpToAssignment("list.hashes", os, whitespace, listFormat);
	listDumpTo(list.hashes, os, index, whitespace, pascalValues, false, listFormat);
	resultListDumpToAssignment("list.signatures", os, whitespace, listFormat);
	listDumpTo(list.signatures, os, index, whitespace, pascalValues, false, listFormat);
	resultListDumpToAssignment("list.bytecodes", os, whitespace, listFormat);
	listDumpTo(list.bytecodes, os, index, whitespace, pascalValues, true, listFormat);
	if(listFormatJson == listFormat) {
		os << '}';
	}
}
template<class List, class Is>
void listLoadFrom(List &list, Is &is, const bool index, const bool whitespace, const bool pascalValues, const bool finalList = true, const ListFormat listFormat = listFormatInitializer) {
	const bool whitespaceFrom = SUSUWU_IO_WHITESPACE && whitespace;
	bool quoteValues = false;
	char delim = '\0';
	char terminator = '\0';
	is >> delim;
	switch(listFormat) {
		case listFormatInitializer:
			classIoCheckChar(__func__, '{', delim);
			terminator = '}';
		break;
		case listFormatJson:
			classIoCheckChar(__func__, '[', delim);
			terminator = ']';
			quoteValues = true;
		break;
		default:
			SUSUWU_UNREACHABLE;
	}
//	decltype(list.front()) value;
#ifdef SUSUWU_LIST_COUNT
	size_t count = 0;
	is >> count;
	is >> delim;
	classIoCheckChar(__func__, ':', delim);
	list.reserve(count);
#endif /* def SUSUWU_LIST_COUNT */
	const std::string assignment = whitespaceFrom ? " = " : "=";
	const std::string hexPrefix = (whitespace && !quoteValues) ? " 0" : "0";
	for(size_t index_ = 0; is.good(); ++index_) {
		std::string token;
		const std::streampos pos = is.tellg();
		if(whitespaceFrom) {
			is >> delim;
			classIoCheckChar(__func__, '\n' /* TODO: support '\r'? */, delim);
		}
		is >> delim;
		if(terminator == delim) {
#ifdef SUSUWU_LIST_COUNT
			classIoCheckSz(__func__, count, index_);
#endif /* def SUSUWU_LIST_COUNT */
			break;
		}
		if(0 == index_) {
			is.seekg(pos);
		} else {
			classIoCheckChar(__func__, ',', delim);
		}
		if(whitespaceFrom) {
			is >> delim;
			classIoCheckChar(__func__, '\t', delim); /* is >> token; classIoCheckStr(__func__, "\n\t", token); */
		}
		if(index) {
			is >> token;
			classIoCheckStr(__func__, std::to_string(index_), token);
			is >> token;
			classIoCheckStr(__func__, assignment, token);
		}
		typename List::value_type value;
		if(quoteValues) {
			is >> delim;
			classIoCheckChar(__func__, '"', delim);
		}
		if(pascalValues) {
			std::streamsize tokenSz = 0;
			is >> tokenSz;
			is >> delim;
			classIoCheckChar(__func__, ':' /* TODO: replace "%Dec:" with "%Bin" */, delim);
			value.resize(tokenSz);
			if(0 < tokenSz) {
				is.read(&value[0], tokenSz); /* `is >> value;` won't do binary code */
				if(is.gcount() < tokenSz) {
					value.resize(is.gcount());
				}
				classIoCheckSz(__func__, is.gcount(), tokenSz);
			}
		} else {
#if !SUSUWU_HEX_DOES_PREFIX
//			is >> token; classIoCheckStr(__func__, "0x", token);
			std::getline(is, token, 'x'); /* used `std::getline` so 'x' is swallowed */
			classIoCheckStr(__func__, hexPrefix, token);
#endif /* !SUSUWU_HEX_DOES_PREFIX */
			classIoHexIs(is, value); /* can do `is >> std::hex >> tokenSz;` but not `>> token` (or `>> value`) */
		}
		if(quoteValues) {
			is >> delim;
			classIoCheckChar(__func__, '"', delim);
		}
		list.insert(list.end(), value); /* list.push_back(value); */
	}
	classIoCheckChar(__func__, terminator, delim);
	switch(listFormat) {
		case listFormatInitializer:
			is >> delim;
			classIoCheckChar(__func__, ';', delim);
		break;
		case listFormatJson:
			if(!finalList) {
				is >> delim;
				classIoCheckChar(__func__, ',', delim);
			}
		break;
	}
	if(whitespaceFrom) {
		is >> delim;
		classIoCheckChar(__func__, '\n', delim);
	}
} /* view `ClassResultList.cxx:classResultListTests()` for examples of input from `listLoadFrom()`+`resultListLoadFrom`. */
template<class Is>
void resultListLoadFromAssignment(const std::string &listStr, Is &is, const bool whitespace, const ListFormat listFormat = listFormatInitializer) {
	const bool whitespaceFrom = SUSUWU_IO_WHITESPACE && whitespace;
	std::string token;
	switch(listFormat) {
		case listFormatInitializer:
			classIoGetline(is, token, '{');
			classIoCheckStr(__func__, listStr + (whitespaceFrom ? " = " : "="), token);
		break;
		case listFormatJson:
			classIoGetline(is, token, '[');
			classIoCheckStr(__func__, '"' + listStr + '"' + (whitespaceFrom ? ": " : ":"), token);
		break;
	}
}
template<class List, class Is>
void resultListLoadFrom(List &list, Is &is, const bool index, const bool whitespace, const bool pascalValues, const ListFormat listFormat = listFormatInitializer) {
	const bool whitespaceFrom = SUSUWU_IO_WHITESPACE && whitespace;
	char delim = '\0';
	if(listFormatJson == listFormat) {
		is >> delim;
		classIoCheckChar(__func__, '{', delim);
		if(whitespaceFrom) {
			is >> delim;
			classIoCheckChar(__func__, '\n', delim);
		}
	}
	resultListLoadFromAssignment("list.hashes", is, whitespace, listFormat);
	listLoadFrom(list.hashes, is, index, whitespace, pascalValues, false, listFormat);
	resultListLoadFromAssignment("list.signatures", is, whitespace, listFormat);
	listLoadFrom(list.signatures, is, index, whitespace, pascalValues, false, listFormat);
	resultListLoadFromAssignment("list.bytecodes", is, whitespace, listFormat);
	listLoadFrom(list.bytecodes, is, index, whitespace, pascalValues, true, listFormat);
	if(listFormatJson == listFormat) {
		is >> delim;
		classIoCheckChar(__func__, '}', delim);
	}
}

template<class List, class List2>
/*	@pre @code !(list.empty() || hashes.full()) @endcode
 *	@post @code !hashes.empty() @endcode */
void listToHashes(const List &list /* ResultList::bytecodes or ResultList::hex*/, List2 &hashes /* ResultList::hashess */) {
	for(const auto &value : list) {
		hashes.insert(classSha2(value));
	}
}
/* Usage: if `ResultList` was not produced with `.hashes` */
static void resultListProduceHashes(ResultList &resultList) {
	listToHashes(resultList.bytecodes, resultList.hashes);
}

/* @pre @code std::is_sorted(list.cbegin(), list.cend()) && std::is_sorted(list2.cbegin(), list2.cend()) @endcode */
template<class List>
const List listIntersections(const List &list, const List &list2) {
	List intersections;
	std::set_intersection(list.cbegin(), list.cend(), list2.cbegin(), list2.cend(), std::back_inserter(intersections));
	return intersections;
}
template<class List>
const bool listsIntersect(const List &list, const List &list2) {
	return listIntersections(list, list2).size();
}

template<class List>
/* return `list`'s `const_iterator` to first instance of `value`, or `list.cend()` (if not found) */
auto listFindValue(const List &list, const typename List::value_type &value) -> decltype(std::find(list.cbegin(), list.cend(), value)) {
//const class List::const_iterator listFindValue(const List &list, const typename List::value_type &value) {
	return std::find(list.cbegin(), list.cend(), value);
}
template<class List>
const bool listHasValue(const List &list, const typename List::value_type &value) {
	return list.cend() != listFindValue(list, value);
}

template<class List>
/* return `list`'s `const_iterator` to first instance of `std::string(itBegin, itEndSubstr)`, or default iterator (if not found)
 * @pre @code itBegin < itEnd @endcode */
const typename List::value_type::const_iterator listFindSubstr(const List &list, typename List::value_type::const_iterator itBegin, typename List::value_type::const_iterator itEnd) {
#pragma unroll
	for(const auto &value : list) {
		auto result = std::search(value.cbegin(), value.cend(), itBegin, itEnd, [](char chValue, char chIt) { return chValue == chIt; });
		if(value.cend() != result) {
			return result;
		}
	}
	return typename List::value_type::const_iterator(); /* Equates to "Not found" */
}
template<class List>
/* @pre @code itBegin < itEnd @endcode */
const bool listHasSubstr(const List &list, typename List::value_type::const_iterator itBegin, typename List::value_type::const_iterator itEnd) {
	return typename List::value_type::const_iterator() != listFindSubstr(list, itBegin, itEnd);
}
template<class List>
/* Returns shortest substr from `value`, which is not found in `list`
 * Usage: `auto tuple = listProduceSignature(resultList.bytecodes, bytecode); resultList.signatures.push_back({std::get<0>(tuple), std::get<1>(tuple)});` */
const std::tuple<typename List::value_type::const_iterator, typename List::value_type::const_iterator> listProduceSignature(const List &list, const typename List::value_type &value) {
	ptrdiff_t smallest = value.size();
	auto itBegin = value.cbegin(), itEnd = value.cend();
	for(auto first = itBegin; value.cend() != first; ++first) {
		for(auto last = value.cend(); first != last; --last) {
			if((last - first) < smallest) {
				if(listHasSubstr(list, first, last)) {
					break;
				}
				smallest = last - first;
				itBegin = first, itEnd = last;
			}
		}
	} /* Incremental `for()` loops, is O(n^2 * m) complex formula to produce signatures; should use binary searches, or look for the Standard Template Lib (or Boost) function which optimizes this. */
	return {itBegin, itEnd};
}
typedef struct ResultListSignatureMatch {
	BytecodeOffset fileOffset;
	ResultListSignature signature;
} ResultListSignatureMatch;
template<class List>
/* Usage: `auto it = listFindSignatureOfValue(resultList.signatures, value)); if(it) { std::cout << "value has resultList.signatures[" << tohex(match.signature) << "]"; }` */
ResultListSignatureMatch listFindSignatureOfValue(const List &list, const typename List::value_type &value) {
	for(const auto &signature : list) {
#if SUSUWU_PREFER_CSTR
		auto it = memmem(&value[0], strlen(&value[0]), &signature[0], strlen(&signature[0]));
		if(NULL != it) {
#else /* !SUSUWU_PREFER_CSTR */
		auto it = std::search(value.cbegin(), value.cend(), signature.cbegin(), signature.cend(), [](char ch1, char ch2) { return ch1 == ch2; });
		if(signature.cend() != it) {
#endif /* !SUSUWU_PREFER_CSTR */
			return {it - value.cbegin(), signature};
		}
	}
	return {-1, ""};
}
template<class List>
/* Usage: `if(listHasSignatureOfValue(resultList.signatures, value)) { std::cout << "value has signature from ResultList.signatures"; }` */
const bool listHasSignatureOfValue(const List &list, const typename List::value_type &value) {
	return -1 != listFindSignatureOfValue(list, value).fileOffset;
}

template<class S>
const std::vector<S> explodeToList(const S &s, const S &token) {
	std::vector<S> list;
	for(auto x = s.cbegin(); s.cend() != x; ) {
		auto it = std::search(x, s.cend(), token.cbegin(), token.cend(), [](char ch1, char ch2) { return ch1 == ch2; });
		list.push_back(S(x, it));
		if(s.cend() == x) {
			return list;
		}
		x = it;
	}
	return list;
}
}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassResultList_hxx */

