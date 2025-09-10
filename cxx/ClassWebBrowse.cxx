/* Attribution (henceforth "*this attribution*", whose syntax is *Markdown*): 2024 [Swudu Susuwu](https://swudususuwu.substack.com)
 * <https://github.com/SwuduSusuwu/SusuLib/> has the newest version of `./cxx/ClassWebBrowse.cxx` (henceforth "*this source code*").
 * If *this attribution* is shown, *this source code* allows all uses. *This attribution* constitutes the most permissive which is compatible with [*GPLv2*](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) + [*Apache 2*](https://www.apache.org/licenses/LICENSE-2.0.html), which is suitable for personal use (also suitable for school use).
 * If *this attribution* is not professional enough for business use: businesses can use *this source code* through included versions of [*GPLv2*](./LICENSE_GPLv2), [*Apache 2*](./LICENSE), or through both of those. */
#ifndef INCLUDES_cxx_ClassWebBrowse_cxx
#define INCLUDES_cxx_ClassWebBrowse_cxx
#include "ClassIo.hxx" /* ClassIoPath */
#include "ClassSys.hxx" /* classSysMuSecondClock ClassSysMuSeconds execvex templateCatchAll */
#include "ClassWebBrowse.hxx" /* classWebBrowseProcessUrls ClassWebBrowseStatus */
#include "Macros.hxx" /* SUSUWU_ATOMIC SUSUWU_DEBUG SUSUWU_NOEXCEPT SUSUWU_UNIT_TESTS SUSUWU_WARNING SUSUWU_WIN32 */
#include <chrono> /* std::chrono */
#include SUSUWU_IF_CPLUSPLUS(<climits>, <limits.h>) /* CHAR_BIT */
#include SUSUWU_IF_CPLUSPLUS(<cstdlib>, <stdlib.h>) /* EXIT_FAILURE EXIT_SUCCESS */
#include <string> /* std::string std::to_string */
#include <thread> /* std::this_thread */
#include <vector> /* std::vector */
#ifdef BOOST_VERSION
#	include <boost/property_tree/ptree.hpp> /* boost::property_tree::ptree */
#	include <boost/property_tree/xml_parser.hpp> /* BOOST_FOREACH read_xml */
#elif defined(SUSUWU_USE_PUGIXML) /* !def BOOST_VERSION */
#	include <pugixml.hpp> /* pugi::xml_document pugi::xml_parse_result pugi::xml_node pugi::xpath_node */
#endif /* !def SUSUWU_USE_PUGIXML */
namespace Susuwu {
bool classWebBrowseUseIfModifiedSince = true; /* Does what `wget -N` does. Notice: depends on accurate system unix clock */
double classWebBrowseMaxRequestsPerSecondPerHost = 2; /* Does what `wget -w 1/classWebBrowseMaxRequestsPerSecondPerHost` does. TODO: measure per-host use across threads. */
double classWebBrowseMaxRequestsPerSecondGlobal = 2000; /* Notice: to prevent congestion (from bursts of requests), `classWebBrowseWget()` uses `1 / classWebBrowseMaxRequestsPerSecondGlobal` (as request interval value) */
double classWebBrowseMaxBitsPerSecondPerHost = 2000000; /* Does what `wget --limit-rate=1/classWebBrowseMaxBitsPerSecondPerHost` does. TODO: measure per-host use across threads. */
double classWebBrowseMaxBitsPerSecondGlobal = 42000000; /* Assumes that all instances of `classWebBrowseWget()` use `classWebBrowseMaxBitsPerSecondPerHost`. TODO: measure true connection use. */
ClassIoPath classWebBrowseDownloadDir = "downloads/"; /* Does what `wget -P classWebBrowseDownloadDir` does. Notice: does not wrap with "" for you. */
SUSUWU_ATOMIC(double) classWebBrowseBitsPerSecondGlobalUsed(0);
SUSUWU_ATOMIC(ClassSysMuSeconds) classWebBrowseLastRequestUnixStamp(0);

const ClassWebBrowseStatus classWebBrowseWget(const ClassIoPath &uniformResourceLocator, const ClassIoPath &localOutput, const bool asynchronousMax) {
	std::string execution = "wget \"" + uniformResourceLocator + "\" --limit-rate=" + std::to_string(classWebBrowseMaxBitsPerSecondPerHost / CHAR_BIT) + " -w " + std::to_string(1 / classWebBrowseMaxRequestsPerSecondPerHost);
	if(!localOutput.empty()) {
		execution += (" -O " + localOutput);
	} else if(!classWebBrowseDownloadDir.empty()) { /* [exclusive, since `-O` uses console redirection](https://stackoverflow.com/questions/55473784/is-it-possible-to-run-wget-using-both-o-and-p-options-together) */
		execution += (" -P " + classWebBrowseDownloadDir);
	}
	if(classWebBrowseUseIfModifiedSince) {
		execution += " -N";
	}
	const ClassSysMuSeconds thisUnixStamp = classSysMuSecondClock();
	const ClassSysMuSeconds thisUnixStampDiff = thisUnixStamp - classWebBrowseLastRequestUnixStamp;
	const ClassSysMuSeconds minUnixStampDiff = 1000000 / classWebBrowseMaxRequestsPerSecondGlobal;
	if(minUnixStampDiff > thisUnixStampDiff) {
		if(asynchronousMax) {
			SUSUWU_DEBUG("classWebBrowseWget(.uniformResourceLocator = \"" + uniformResourceLocator + "\", .localOutput = \"" + localOutput + "\") { (minUnixStampDiff > thisUnixStampDiff) { return EXIT_FAILURE; } }");
			return EXIT_FAILURE;
		}
		std::this_thread::sleep_for(std::chrono::microseconds(minUnixStampDiff - thisUnixStampDiff));
	}
	double thisBitsPerSecond = classWebBrowseMaxBitsPerSecondPerHost;
	while(thisBitsPerSecond + classWebBrowseBitsPerSecondGlobalUsed > classWebBrowseMaxBitsPerSecondGlobal) {
		if(asynchronousMax) {
			thisBitsPerSecond = classWebBrowseMaxBitsPerSecondGlobal - classWebBrowseBitsPerSecondGlobalUsed;
			if(0 == thisBitsPerSecond) {
				SUSUWU_DEBUG("classWebBrowseWget(.uniformResourceLocator = \"" + uniformResourceLocator + "\", .localOutput = \"" + localOutput + "\") { (classWebBrowseMaxBitsPerSecondGlobal == classWebBrowseBitsPerSecondGlobalUsed) { return EXIT_FAILURE; } }");
				return EXIT_FAILURE;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
	}
	classWebBrowseLastRequestUnixStamp = classSysMuSecondClock();
	classWebBrowseBitsPerSecondGlobalUsed = classWebBrowseBitsPerSecondGlobalUsed + thisBitsPerSecond; /* fixes "error: no viable overloaded '+='" */
	const int statusCode = execvex(execution);
	classWebBrowseBitsPerSecondGlobalUsed = classWebBrowseBitsPerSecondGlobalUsed - thisBitsPerSecond; /* fixes "error: no viable overloaded '-='" */
#ifdef SUSUWU_WIN32
#	pragma message("TODO: downloads without [`wget` for _Windows_](https://gnuwin32.sourceforge.net/packages/wget.htm)")
	if(EXIT_SUCCESS != statusCode) {
		SUSUWU_WARNING("classWebBrowseWget(.uniformResourceLocator = \"" + uniformResourceLocator + "\", .localOutput = \"" + localOutput + "\") { const int statusCode = execvex(execution); statusCode == std::to_string(statusCode); /* Suggestion: install [`wget` for _Windows_](https://gnuwin32.sourceforge.net/packages/wget.htm) */ }");
	}
#endif /* def SUSUWU_WIN32 */
	return statusCode;
}

const std::vector<ClassIoPath> classWebBrowseProcessUrls(const ClassIoPath &localXhtml) {
	std::vector<ClassIoPath> urls;
#ifdef BOOST_VERSION
	boost::property_tree::ptree pt; /* <https://www.boost.org/doc/libs/1_85_0/doc/html/property_tree/parsers.html#property_tree.parsers.xml_parser> <https://github.com/boostorg/property_tree/blob/develop/doc/xml_parser.qbk> */
	read_xml(localXhtml, pt);
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("html.a href")) {
		urls.push_back(v.second.data());
	}
#elif defined(SUSUWU_USE_PUGIXML) /* !def BOOST_VERSION */
	pugi::xml_document doc;
	const pugi::xml_parse_result result = doc.load_file(localXhtml.c_str());
	if(result) {
#	ifdef ASSISTANTCNS_LIMIT_TO_TOP_LEVEL
		for(pugi::xml_node node = doc.child("html").child("body").child("a"); node; node = node.next_sibling("a")) {
			if(node.attribute("href")) {
				urls.push_back(node.attribute("href").value());
			}
		} /* limited to direct descendants of `<body>` */
#	else /* !def ASSISTANTCNS_LIMIT_TO_TOP_LEVEL */
		const pugi::xpath_node_set links = doc.select_nodes("//a[@href]");
		for(const auto &link : links) {
			urls.push_back(link.node().attribute("href").value());
		}
#	endif /* else !def ASSISTANTCNS_LIMIT_TO_TOP_LEVEL */
	} else {
		SUSUWU_WARNING("classWebBrowseProcessUrls(.localXhtml = \"" + localXhtml + "\"): { (!doc.load_file(localXhtml.c_str())) }");
	}
#else /* else !def SUSUWU_USE_PUGIXML */
#	pragma message("TODO: process XHTML without `Boost` or `pugixml`") /* TODO: fall back to regular expression (such as <https://www.boost.io/libraries/regex/> <https://github.com/boostorg/regex>) */
#endif /* !def SUSUWU_USE_PUGIXML */
	return urls;
}

#if SUSUWU_UNIT_TESTS
/* @throw std::runtime_error */
const bool classWebBrowseTests() { return true; } /* TODO */
const bool classWebBrowseTestsNoexcept() SUSUWU_NOEXCEPT { return templateCatchAll(classWebBrowseTests, "classWebBrowseTests()"); }
#endif /* SUSUWU_UNIT_TESTS */

}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassWebBrowse_cxx */

