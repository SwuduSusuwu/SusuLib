/* Attribution (henceforth "*this attribution*", whose syntax is *Markdown*): 2024 [Swudu Susuwu](https://swudususuwu.substack.com)
 * <https://github.com/SwuduSusuwu/SusuLib/> has the newest version of `./cxx/ClassWebBrowse.hxx` (henceforth "*this source code*").
 * If *this attribution* is shown, *this source code* allows all uses. *This attribution* constitutes the most permissive which is compatible with [*GPLv2*](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) + [*Apache 2*](https://www.apache.org/licenses/LICENSE-2.0.html), which is suitable for personal use (also suitable for school use).
 * If *this attribution* is not professional enough for business use: businesses can use *this source code* through included versions of [*GPLv2*](./LICENSE_GPLv2), [*Apache 2*](./LICENSE), or through both of those. */
#pragma once
#ifndef INCLUDES_cxx_ClassWebBrowse_hxx
#define INCLUDES_cxx_ClassWebBrowse_hxx
#include "ClassIo.hxx" /* ClassIoPath */
#include "ClassSys.hxx" /* ClassSysMuSeconds */
#include "Macros.hxx" /* SUSUWU_ATOMIC SUSUWU_NOEXCEPT SUSUWU_UNIT_TESTS SUSUWU_WARNING */
#include <vector> /* std::vector */
/* Abstractions used to web browse */
namespace Susuwu {
extern bool classWebBrowseUseIfModifiedSince; /* Does what `wget -N` does. Notice: incompatible with `.localOutput`. Notice: depends on accurate system unix clock. */
extern double classWebBrowseMaxRequestsPerSecondPerHost; /* Does what `wget -w 1/classWebBrowseMaxRequestsPerSecondPerHost` does. TODO: measure per-host use across threads. */
extern double classWebBrowseMaxRequestsPerSecondGlobal; /* Notice: to prevent congestion (from bursts of requests), `classWebBrowseWget()` uses `1 / classWebBrowseMaxRequestsPerSecondGlobal` (as request interval value) */
extern double classWebBrowseMaxBitsPerSecondPerHost; /* Does what `wget --limit-rate=1/classWebBrowseMaxBitsPerSecondPerHost` does. TODO: measure per-host use across threads. */
extern double classWebBrowseMaxBitsPerSecondGlobal; /* Assumes that all instances of `classWebBrowseWget()` use `classWebBrowseMaxBitsPerSecondPerHost`. TODO: measure true connection use. */
extern ClassIoPath classWebBrowseDownloadDir; /* Does what `wget -P classWebBrowseDownloadDir` does. Notice: does not wrap with "" for you. */
extern SUSUWU_ATOMIC(double) classWebBrowseBitsPerSecondGlobalUsed;
extern SUSUWU_ATOMIC(ClassSysMuSeconds) classWebBrowseLastRequestUnixStamp;
typedef int ClassWebBrowseStatus; /* [Uses `wget` status codes](https://www.gnu.org/software/wget/manual/html_node/Exit-Status.html), except if limit is exceeded with `asynchronousMax`, which gives `return EXIT_FAILURE;` */

const ClassWebBrowseStatus classWebBrowseWget(const ClassIoPath &uniformResourceLocator, const ClassIoPath &localOutput, bool asynchronousMax = false); /* return execvex("wget \"" + uniformResourceLocator + '"' + (localOutput ? " -O \"" + localOutput + '"' : (classWebBrowseDownloadDir ? (" -P " + classWebBrowseDownloadDir) : "")) + " --limit-rate=" + std::to_string(classWebBrowseMaxBitsPerSecondPerHost / CHAR_BIT) + " -w " + std::to_string(1 / classWebBrowseMaxRequestsPerSecondPerHost) + (classWebBrowseUseIfModifiedSince ? " -N" : "")); */
const std::vector<ClassIoPath> classWebBrowseProcessUrls(const ClassIoPath &localXhtml); /* returns list of Uniform Resource Identifiers from `localXhtml` */

#if SUSUWU_UNIT_TESTS
/* @throw std::runtime_error */
const bool classWebBrowseTests();
const bool classWebBrowseTestsNoexcept() SUSUWU_NOEXCEPT;
#endif /* SUSUWU_UNIT_TESTS */
}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassWebBrowse_hxx */

