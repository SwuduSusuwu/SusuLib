/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#pragma once
#ifndef INCLUDES_cxx_ClassIo_hxx
#define INCLUDES_cxx_ClassIo_hxx
#include "Macros.hxx" /* SUSUWU_IF_CPLUSPLUS SUSUWU_NOEXCEPT SUSUWU_UNIT_TESTS */
#include SUSUWU_IF_CPLUSPLUS(<cstdio>, <stdio.h>) /* FILE */
#include <string> /* std::string */
/* Abstractions to do with filesystems (such as `procfs`) */
namespace Susuwu {
typedef std::string ClassIoPath; /* TODO: `std::char_traits<unsigned char>`, `std::basic_string<unsigned char>("string literal")` */
typedef ClassIoPath ClassIoBytecode; /* Uses `std::string` for bytecode (versus `std::vector`) because:
 * "If you are going to use the data in a string like fashon then you should opt for std::string as using a std::vector may confuse subsequent maintainers. If on the other hand most of the data manipulation looks like plain maths or vector like then a std::vector is more appropriate." -- https://stackoverflow.com/a/1556294/24473928
*/
typedef ClassIoPath ClassIoHash; /* TODO: `std::unordered_set<std::basic_string<unsigned char>>` */

/* Usage: for Linux (or Windows,) if you don't trust `argv[0]`, replace it with `classIoGetOwnPath()`.
 * Error values: `return ClassIoPath();` */
const ClassIoPath classIoGetOwnPath() /* TODO: SUSUWU_NOEXCEPT(std::is_nothrow_constructible<ClassIoPath>::value) */;
const FILE *classIoFopenOwnPath() /* TODO: SUSUWU_NOEXCEPT(std::is_nothrow_invocable<classIoGetClassIoPath()>::value) */;

#if SUSUWU_UNIT_TESTS
/* @throw std::runtime_error */
const bool classIoTests();
const bool classIoTestsNoexcept() SUSUWU_NOEXCEPT;
#endif /* SUSUWU_UNIT_TESTS */

}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassIo_hxx */

