/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#pragma once
#ifndef INCLUDES_cxx_ClassWebBrowse_hxx
#define INCLUDES_cxx_ClassWebBrowse_hxx
#include "ClassIo.hxx" /* ClassIoPath */
#include "Macros.hxx" /* SUSUWU_NOEXCEPT SUSUWU_UNIT_TESTS SUSUWU_WARNING */
#include <vector> /* std::vector */
/* Abstractions used to web browse */
namespace Susuwu {
extern bool classWebBrowseUseIfModifiedSince; /* Does what `wget -N` does. Notice: incompatible with `.localOutput`. Notice: depends on accurate system unix clock. */
extern double classWebBrowseMaxRequestsPerSecondPerHost; /* Does what `wget -w 1/classWebBrowseMaxRequestsPerSecondPerHost` does. TODO: measure per-host use across threads. */
extern double classWebBrowseMaxRequestsPerSecondGlobal;  /* TODO: limit `wget` through this */
extern double classWebBrowseMaxBitsPerSecondPerHost; /* Does what `wget --limit-rate=1/classWebBrowseMaxBitsPerSecondPerHost` does. TODO: measure per-host use across threads. */
extern double classWebBrowseMaxBitsPerSecondGlobal;      /* TODO: limit `wget` through this */
extern ClassIoPath classWebBrowseDownloadDir; /* Does what `wget -P classWebBrowseDownloadDir` does. Notice: does not wrap with "" for you. */
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

