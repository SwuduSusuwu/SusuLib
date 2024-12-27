/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#pragma once
#ifndef INCLUDES_cxx_ClassFS_hxx
#define INCLUDES_cxx_ClassFS_hxx
#include <string> /* std::string */
namespace Susuwu {
typedef std::string ClassFsPath; /* TODO: `std::char_traits<unsigned char>`, `std::basic_string<unsigned char>("string literal")` */
typedef ClassFsPath ClassFsBytecode; /* Uses `std::string` for bytecode (versus `std::vector`) because:
 * "If you are going to use the data in a string like fashon then you should opt for std::string as using a std::vector may confuse subsequent maintainers. If on the other hand most of the data manipulation looks like plain maths or vector like then a std::vector is more appropriate." -- https://stackoverflow.com/a/1556294/24473928
*/
typedef ClassFsPath ClassFsHash; /* TODO: `std::unordered_set<std::basic_string<unsigned char>>` */
}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassFS_hxx */

