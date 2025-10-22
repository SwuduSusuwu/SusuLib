/* Attribution (henceforth "*this attribution*", whose syntax is *Markdown*): 2024 [Swudu Susuwu](https://swudususuwu.substack.com)
 * <https://github.com/SwuduSusuwu/SusuLib/> has the newest version of `./cxx/ClassPortableExecutable.hxx` (henceforth "*this source code*").
 * If *this attribution* is shown, *this source code* allows all uses. *This attribution* constitutes the most permissive which is compatible with [*GPLv2*](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) + [*Apache 2*](https://www.apache.org/licenses/LICENSE-2.0.html), which is suitable for personal use (also suitable for school use).
 * If *this attribution* is not professional enough for business use: businesses can use *this source code* through included versions of [*GPLv2*](./LICENSE_GPLv2), [*Apache 2*](./LICENSE), or through both of those. */
#pragma once
#ifndef INCLUDES_cxx_ClassPortableExecutable_hxx
#define INCLUDES_cxx_ClassPortableExecutable_hxx
#include "ClassIo.hxx" /* ClassIoPath ClassIoBytecode ClassIoHash */
#include "ClassObject.hxx" /* Object SUSUWU_VIRTUAL_DEFAULTS */
#include <string> /* std::string */
#include <utility> /* std::move */
#include <vector> /* std::vector */
namespace Susuwu {
typedef std::string PortableExecutableFunctionSig;
typedef class PortableExecutable : public Object {
/* TODO: union of actual Portable Executable (Microsoft) + ELF (Linux) specifications */
public:
	SUSUWU_VIRTUAL_DEFAULTS(Susuwu::PortableExecutable) /* `getName()`, `isPureVirtual()`, `operator==`()`, ... */
	explicit PortableExecutable(ClassIoPath path_ = "") : path(std::move(path_)) {}
	PortableExecutable(ClassIoPath path_, ClassIoBytecode bytecode_) : path(std::move(path_)), bytecode(std::move(bytecode_)) {} /* TODO: NOLINT(bugprone-easily-swappable-parameters) */
/*TODO: overload on typedefs which map to the same types:	PortableExecutable(const ClassIoPath &path_, const std::string &hex_) : path(path_), hex(hex_) {} */
	const std::vector<PortableExecutableFunctionSig> importedFunctionsList() const;
/* `clang-tidy` off: NOLINTBEGIN(misc-non-private-member-variables-in-classes) */
	const ClassIoPath path; /* Suchas "C:\Program.exe" or "/usr/bin/library.so" */ /* NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members) */
	ClassIoBytecode bytecode; /* compiled programs; bytecode */
	std::string hex; /* `hexdump(path)`, hexadecimal, for C string functions */
/* `clang-tidy` on: NOLINTEND(misc-non-private-member-variables-in-classes) */
} PortableExecutable;
typedef class PortableExecutableBytecode : public PortableExecutable {
public:
	SUSUWU_VIRTUAL_DEFAULTS(Susuwu::PortableExecutableBytecode) /* `getName()`, `isPureVirtual()`, `operator==`()`, ... */
	explicit PortableExecutableBytecode(ClassIoPath path_);
} PortableExecutableBytecode;
}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassPortableExecutable_hxx */

