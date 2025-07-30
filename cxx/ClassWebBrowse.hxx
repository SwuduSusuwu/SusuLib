/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#pragma once
#ifndef INCLUDES_cxx_ClassWebBrowse_hxx
#define INCLUDES_cxx_ClassWebBrowse_hxx
#include "ClassIo.hxx" /* ClassIoPath */
#include "Macros.hxx" /* SUSUWU_NOEXCEPT SUSUWU_UNIT_TESTS SUSUWU_WARNING */
#include <vector> /* std::vector */
/* Abstractions used to web browse */
namespace Susuwu {
const std::vector<ClassIoPath> classWebBrowseProcessUrls(const ClassIoPath &localXhtml); /* returns list of Uniform Resource Identifiers from `localXhtml` */

#if SUSUWU_UNIT_TESTS
/* @throw std::runtime_error */
const bool classWebBrowseTests();
const bool classWebBrowseTestsNoexcept() SUSUWU_NOEXCEPT;
#endif /* SUSUWU_UNIT_TESTS */
}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassWebBrowse_hxx */

