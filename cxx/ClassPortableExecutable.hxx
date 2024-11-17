/* Dual licenses: choose "Creative Commons" or "Apache 2" (allows all uses) */
#pragma once
#ifndef INCLUDES_cxx_ClassPortableExecutable_hxx
#define INCLUDES_cxx_ClassPortableExecutable_hxx
#include "ClassObject.hxx" /* Object */
#include "Macros.hxx" /* SUSUWU_OVERRIDE */
#include <fstream> /* std::ifstream */
#include <iterator> /* std::istreambuf_iterator */
#include <string> /* std::string */
#include <utility> /* std::move */
namespace Susuwu {
typedef std::string FilePath; /* TODO: `std::char_traits<unsigned char>`, `std::basic_string<unsigned char>("string literal")` */
typedef FilePath FileBytecode; /* Uses `std::string` for bytecode (versus `std::vector`) because:
 * "If you are going to use the data in a string like fashon then you should opt for std::string as using a std::vector may confuse subsequent maintainers. If on the other hand most of the data manipulation looks like plain maths or vector like then a std::vector is more appropriate." -- https://stackoverflow.com/a/1556294/24473928
*/
typedef FilePath FileHash; /* TODO: `std::unordered_set<std::basic_string<unsigned char>>` */
typedef class PortableExecutable : Object {
/* TODO: union of actual Portable Executable (Microsoft) + ELF (Linux) specifications */
public:
	const std::string getName() const SUSUWU_OVERRIDE {return "Susuwu::class PortableExecutable";}
	explicit PortableExecutable(FilePath path_ = "") : path(std::move(path_)) {}
	PortableExecutable(FilePath path_, FileBytecode bytecode_) : path(std::move(path_)), bytecode(std::move(bytecode_)) {} /* TODO: NOLINT(bugprone-easily-swappable-parameters) */
/*TODO: overload on typedefs which map to the same types:	PortableExecutable(const FilePath &path_, const std::string &hex_) : path(path_), hex(hex_) {} */
/* `clang-tidy` off: NOLINTBEGIN(misc-non-private-member-variables-in-classes) */
	const FilePath path; /* Suchas "C:\Program.exe" or "/usr/bin/library.so" */ /* NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members) */
	FileBytecode bytecode; /* compiled programs; bytecode */
	std::string hex; /* `hexdump(path)`, hexadecimal, for C string functions */
/* `clang-tidy` on: NOLINTEND(misc-non-private-member-variables-in-classes) */
} PortableExecutable;
typedef class PortableExecutableBytecode : public PortableExecutable {
public:
	const std::string getName() const SUSUWU_OVERRIDE {return "Susuwu::class PortableExecutableBytecode";}
	explicit PortableExecutableBytecode(FilePath path_) : PortableExecutable(std::move(path_))  {std::ifstream input(path); if(input.good()) {bytecode = std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());}}
} PortableExecutableBytecode;
}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassPortableExecutable_hxx */

