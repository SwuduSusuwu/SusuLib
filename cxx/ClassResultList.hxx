/* Dual licenses: choose "Creative Commons" or "Apache 2" (allows all uses) */
#pragma once
#ifndef INCLUDES_cxx_ClassResultList_hxx
#define INCLUDES_cxx_ClassResultList_hxx
//#include "ClassObject.hxx" /* Object */ /* TODO: fix "Initialization of non-aggregate type" */
#include "ClassPortableExecutable.hxx" /* FilePath FileBytecode FileHash */
#include "ClassSha2.hxx" /* sha2 */
#include "ClassSys.hxx" /* classSysHexOs */
#include "Macros.hxx" /* SUSUWU_NOEXCEPT SUSUWU_PREFER_CSTR */
#include <algorithm> /* std::search std::find std::set_intersection */
#include <cstddef> /* size_t */
#if SUSUWU_PREFER_CSTR
#	include <cstring> /* strlen memmem */
#endif /* SUSUWU_PREFER_CSTR */
#include <tuple> /* std::tuple std::get */
#include <unordered_set> /* std::unordered_set */
#include <vector> /* std::vector */
namespace Susuwu {
typedef FileHash ResultListHash;
typedef FileBytecode ResultListBytecode; /* Should have structure of FileBytecode, but is not just for files, can use for UTF8/webpages, so have a new type for this */
typedef FilePath ResultListSignature; /* TODO: `typedef ResultListBytecode ResultListSignature; ResultListSignature("string literal");` */
typedef ptrdiff_t BytecodeOffset; /* all tests of `ResultListBytecode` should return `{BytecodeOffset, X}` (with the most common `X` as `ResultListHash` or `ResultListSignature`). `offset = -1` if no match */
typedef struct ResultList : Object { /* Lists of {metadata, executables (or pages)} */
	const std::string getName() const override {return "Susuwu::struct ResultList";}
/* `clang-tidy` off: NOLINTBEGIN(misc-non-private-member-variables-in-classes) */
	typedef std::unordered_set<ResultListHash> Hashes;
	Hashes hashes; /* Checksums of executables (or pages); to avoid duplicates, plus to do constant ("O(1)") test for which executables (or pages) exists */
	typedef std::vector<ResultListSignature> Signatures;
	Signatures signatures; /* Smallest substrings (or regexes, or Universal Resource Locators) which can identify `bytecodes`; has uses close to `hashes`, but can match if executables (or pages) have small differences */
	typedef std::vector<ResultListBytecode> Bytecodes;
	Bytecodes bytecodes; /* Whole executables (for `VirusAnalysis`) or webpages (for `AssistantCns`); huge disk usage, just load this for signature synthesis (or CNS backpropagation). */
/* `clang-tidy` on: NOLINTEND(misc-non-private-member-variables-in-classes) */
} ResultList;

const bool classResultListTests(); /* TODO: test most of `ClassResultList*` */
static const bool classResultListTestsNoexcept() SUSUWU_NOEXCEPT {return templateCatchAll(classResultListTests, "classResultListTests()");}

template<class List>
const size_t listMaxSize(const List &list) {
#if SUSUWU_PREFER_CSTR
	size_t max = 0;
	for(auto it = &list[0]; list.cend() != it; ++it) { const size_t temp = strlen(*it); if(temp > max) {max = temp;}}
	return max; /* WARNING! `strlen()` just does UTF8-strings/hex-strings; if binary, must use `it->size()` */
#else /* else !SUSUWU_PREFER_CSTR */
	auto it = std::max_element(list.cbegin(), list.cend(), [](const auto &s, const auto &x) { return s.size() < x.size(); });
	return it->size();
#endif /* SUSUWU_PREFER_CSTR else */
}

template<class List, class Os>
void listDumpTo(const List &list, Os &os, const bool index, const bool whitespace, const bool pascalValues) {
	size_t index_ = 0;
	os << '{';
	for(const auto &value : list) {
		if(0 != index_) {
			os << ',';
		}
		if(whitespace) {
			os << std::endl << '\t';
		}
		if(index) {
			os << index_;
			whitespace ? (os << " = ") : (os << '=');
		}
		if(pascalValues) {
			os << value.size() << ':' /* TODO: replace "%Dec:" with "%Bin" */ << value;
		} else {
			os << "0x";
			classSysHexOs(os, value);
		}
		++index_;
	}
	if(whitespace) {
		os << "\n};" << std::endl;
	} else {
		os << "};";
	}
} /* view `ClassResultList.cxx`:`classResultListTests()` for examples of output from `listDumpTo()`+`resultListDumpTo()`. TODO: +`listLoadFrom()`/+`resultListLoadFrom()` */
template<class List, class Os>
void resultListDumpTo(const List &list, Os &os, const bool index, const bool whitespace, const bool pascalValues) {
	os << "list.hashes" << (whitespace ? " = " : "=");
	listDumpTo(list.hashes, os, index, whitespace, pascalValues);
	os << "list.signatures" << (whitespace ? " = " : "=");
	listDumpTo(list.signatures, os, index, whitespace, pascalValues);
	os << "list.bytecodes" << (whitespace ? " = " : "=");
	listDumpTo(list.bytecodes, os, index, whitespace, pascalValues);
}

template<class List, class List2>
/*	@pre @code !(list.empty() || hashes.full()) @endcode
 *	@post @code !hashes.empty() @endcode */
void listToHashes(const List &list /* ResultList::bytecodes or ResultList::hex*/, List2 &hashes /* ResultList::hashess */) {
	for(const auto &value : list) {
		hashes.insert(sha2(value));
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
auto listFindValue(const List &list, const typename List::value_type &value) {
	return std::find(list.cbegin(), list.cend(), value);
}
template<class List>
const bool listHasValue(const List &list, const typename List::value_type &value) {
	return list.cend() != listFindValue(list, value);
}

template<class List>
const typename List::value_type::const_iterator listDefaultIterator = typename List::value_type::const_iterator(); /* Equates to "Not found" */
template<class List>
/* return `list`'s `const_iterator` to first instance of `std::string(itBegin, itEndSubstr)`, or default iterator (if not found)
 * @pre @code itBegin < itEnd @endcode */
decltype(listDefaultIterator<List>) listFindSubstr(const List &list, typename List::value_type::const_iterator itBegin, typename List::value_type::const_iterator itEnd) {
#pragma unroll
	for(const auto &value : list) {
		auto result = std::search(value.cbegin(), value.cend(), itBegin, itEnd, [](char chValue, char chIt) { return chValue == chIt; });
		if(value.cend() != result) {
			return result;
		}
	}
	return listDefaultIterator<List>;
}
template<class List>
/* @pre @code itBegin < itEnd @endcode */
const bool listHasSubstr(const List &list, typename List::value_type::const_iterator itBegin, typename List::value_type::const_iterator itEnd) {
	return listDefaultIterator<List> != listFindSubstr(list, itBegin, itEnd);
}
template<class List>
/* Returns shortest substr from `value`, which is not found in `list`
 * Usage: `resultList.signatures.push_back({listProduceSignature(resultList.bytecodes, bytecode));` */
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
/* Usage: `auto it = listFindSignatureOfValue(resultList.signatures, value)); if(it) {std::cout << "value has resultList.signatures[" << tohex(match.signature) << "]";}` */
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
/* Usage: `if(listHasSignatureOfValue(resultList.signatures, value)) {std::cout << "value has signature from ResultList.signatures";}` */
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

