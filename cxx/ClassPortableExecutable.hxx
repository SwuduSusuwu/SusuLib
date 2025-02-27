/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#pragma once
#ifndef INCLUDES_cxx_ClassPortableExecutable_hxx
#define INCLUDES_cxx_ClassPortableExecutable_hxx
#include "ClassIo.hxx" /* ClassIoPath ClassIoBytecode ClassIoHash */
#include "ClassObject.hxx" /* Object SUSUWU_VIRTUAL_DEFAULTS */
#include <fstream> /* std::ifstream */
#include <iterator> /* std::istreambuf_iterator */
#include <string> /* std::string */
#include <utility> /* std::move */
namespace Susuwu {
typedef class PortableExecutable : public Object {
/* TODO: union of actual Portable Executable (Microsoft) + ELF (Linux) specifications */
public:
	SUSUWU_VIRTUAL_DEFAULTS(Susuwu::PortableExecutable) /* `getName()`, `isPureVirtual()`, `operator==`()`, ... */
	explicit PortableExecutable(ClassIoPath path_ = "") : path(std::move(path_)) {}
	PortableExecutable(ClassIoPath path_, ClassIoBytecode bytecode_) : path(std::move(path_)), bytecode(std::move(bytecode_)) {} /* TODO: NOLINT(bugprone-easily-swappable-parameters) */
/*TODO: overload on typedefs which map to the same types:	PortableExecutable(const ClassIoPath &path_, const std::string &hex_) : path(path_), hex(hex_) {} */
/* `clang-tidy` off: NOLINTBEGIN(misc-non-private-member-variables-in-classes) */
	const ClassIoPath path; /* Suchas "C:\Program.exe" or "/usr/bin/library.so" */ /* NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members) */
	ClassIoBytecode bytecode; /* compiled programs; bytecode */
	std::string hex; /* `hexdump(path)`, hexadecimal, for C string functions */
/* `clang-tidy` on: NOLINTEND(misc-non-private-member-variables-in-classes) */
} PortableExecutable;
typedef class PortableExecutableBytecode : public PortableExecutable {
public:
	SUSUWU_VIRTUAL_DEFAULTS(Susuwu::PortableExecutableBytecode) /* `getName()`, `isPureVirtual()`, `operator==`()`, ... */
	explicit PortableExecutableBytecode(ClassIoPath path_) : PortableExecutable(std::move(path_)) { std::ifstream input(path); if(input.good()) { bytecode = std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()); } }
} PortableExecutableBytecode;
}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassPortableExecutable_hxx */

