/* Attribution (henceforth "*this attribution*", whose syntax is *Markdown*): 2024 [Swudu Susuwu](https://swudususuwu.substack.com)
 * <https://github.com/SwuduSusuwu/SusuLib/> has the newest version of `./cxx/ClassWebBrowse.cxx` (henceforth "*this source code*").
 * If *this attribution* is shown, *this source code* allows all uses. *This attribution* constitutes the most permissive which is compatible with [*GPLv2*](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) + [*Apache 2*](https://www.apache.org/licenses/LICENSE-2.0.html), which is suitable for personal use (also suitable for school use).
 * If *this attribution* is not professional enough for business use: businesses can use *this source code* through included versions of [*GPLv2*](./LICENSE_GPLv2), [*Apache 2*](./LICENSE), or through both of those. */
#ifndef INCLUDES_cxx_ClassWebBrowse_cxx
#define INCLUDES_cxx_ClassWebBrowse_cxx
#include "ClassIo.hxx" /* ClassIoPath */
#include "ClassSys.hxx" /* templateCatchAll */
#include "ClassWebBrowse.hxx" /* classWebBrowseProcessUrls */
#include "Macros.hxx" /* SUSUWU_NOEXCEPT SUSUWU_UNIT_TESTS SUSUWU_WARNING */
#include <vector> /* std::vector */
#ifdef BOOST_VERSION
#	include <boost/property_tree/ptree.hpp> /* boost::property_tree::ptree */
#	include <boost/property_tree/xml_parser.hpp> /* BOOST_FOREACH read_xml */
#endif /* !def BOOST_VERSION */
namespace Susuwu {
bool classWebBrowseUseIfModifiedSince = true; /* TODO. Does what `wget -N` does. */
double classWebBrowseMaxRequestsPerSecondPerHost = 2;   /* TODO: limit `wget` through this */
double classWebBrowseMaxRequestsPerSecondGlobal = 2000; /* TODO: limit `wget` through this */
double classWebBrowseMaxBitsPerSecondPerHost = 2000000; /* TODO: limit `wget` through this */
double classWebBrowseMaxBitsPerSecondGlobal = 42000000; /* TODO: limit `wget` through this */

const std::vector<ClassIoPath> classWebBrowseProcessUrls(const ClassIoPath &localXhtml) {
	std::vector<ClassIoPath> urls;
#ifdef BOOST_VERSION
	boost::property_tree::ptree pt; /* <https://www.boost.org/doc/libs/1_85_0/doc/html/property_tree/parsers.html#property_tree.parsers.xml_parser> <https://github.com/boostorg/property_tree/blob/develop/doc/xml_parser.qbk> */
	read_xml(localXhtml, pt);
	BOOST_FOREACH(
			boost::property_tree::ptree::value_type &v,
			pt.get_child("html.a href"))
		urls.push_back(v.second.data());
#else /* else !def BOOST_VERSION */
#	pragma message("TODO: process XHTML without `Boost`") /* TODO: fall back to regular expression (such as <https://www.boost.io/libraries/regex/> <https://github.com/boostorg/regex>) */
#endif /* !def BOOST_VERSION */
	return urls;
}

#if SUSUWU_UNIT_TESTS
/* @throw std::runtime_error */
const bool classWebBrowseTests() { return true; } /* TODO */
const bool classWebBrowseTestsNoexcept() SUSUWU_NOEXCEPT { return templateCatchAll(classWebBrowseTests, "classWebBrowseTests()"); }
#endif /* SUSUWU_UNIT_TESTS */

}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassWebBrowse_cxx */

