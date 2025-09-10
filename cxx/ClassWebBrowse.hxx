/* Attribution (henceforth "*this attribution*", whose syntax is *Markdown*): 2024 [Swudu Susuwu](https://swudususuwu.substack.com)
 * <https://github.com/SwuduSusuwu/SusuLib/> has the newest version of `./cxx/ClassWebBrowse.hxx` (henceforth "*this source code*").
 * If *this attribution* is shown, *this source code* allows all uses. *This attribution* constitutes the most permissive which is compatible with [*GPLv2*](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) + [*Apache 2*](https://www.apache.org/licenses/LICENSE-2.0.html), which is suitable for personal use (also suitable for school use).
 * If *this attribution* is not professional enough for business use: businesses can use *this source code* through included versions of [*GPLv2*](./LICENSE_GPLv2), [*Apache 2*](./LICENSE), or through both of those. */
#pragma once
#ifndef INCLUDES_cxx_ClassWebBrowse_hxx
#define INCLUDES_cxx_ClassWebBrowse_hxx
#include "ClassIo.hxx" /* ClassIoPath */
#include "Macros.hxx" /* SUSUWU_NOEXCEPT SUSUWU_UNIT_TESTS SUSUWU_WARNING */
#include <vector> /* std::vector */
/* Abstractions used to web browse */
namespace Susuwu {
extern bool classWebBrowseUseIfModifiedSince; /* TODO: Does what `wget -N` does. */
extern double classWebBrowseMaxRequestsPerSecondPerHost; /* TODO: limit `wget` through this */
extern double classWebBrowseMaxRequestsPerSecondGlobal;  /* TODO: limit `wget` through this */
extern double classWebBrowseMaxBitsPerSecondPerHost;     /* TODO: limit `wget` through this */
extern double classWebBrowseMaxBitsPerSecondGlobal;      /* TODO: limit `wget` through this */

const std::vector<ClassIoPath> classWebBrowseProcessUrls(const ClassIoPath &localXhtml); /* returns list of Uniform Resource Identifiers from `localXhtml` */

#if SUSUWU_UNIT_TESTS
/* @throw std::runtime_error */
const bool classWebBrowseTests();
const bool classWebBrowseTestsNoexcept() SUSUWU_NOEXCEPT;
#endif /* SUSUWU_UNIT_TESTS */
}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassWebBrowse_hxx */

