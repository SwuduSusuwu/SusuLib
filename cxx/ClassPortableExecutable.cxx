/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#ifndef INCLUDES_cxx_ClassPortableExecutable_cxx
#define INCLUDES_cxx_ClassPortableExecutable_cxx
#include "ClassIo.hxx" /* ClassIoPath */
#include "ClassPortableExecutable.hxx" /* PortableExecutable PortableExecutableBytecode PortableExecutableFunctionSig */
#include <fstream> /* std::ifstream */
#include <iterator> /* std::istreambuf_iterator */
#include <string> /* std::string */
#include <utility> /* std::move */
#include <vector> /* std::vector */
namespace Susuwu {
const std::vector<PortableExecutableFunctionSig> PortableExecutable::importedFunctionsList() const {
	static_cast<void>(bytecode); /* silences `[functionStatic]`, plus hints how to implement this */
	return {}; /* fixes crash, until `importedFunctionsList` is implemented/finished */
	/* TODO: process [“Portable Executable” for Win32](https://learn.microsoft.com/en-us/windows/win32/debug/pe-format https://wikipedia.org/wiki/Portable_Executable),
	 * [“Extended Linker Format” for Linux / Unix](https://wikipedia.org/wiki/Executable_and_Linkable_Format),
	 * for lists of libs (`.dll`'s / .`so`s) plus functions (the new version of `syscall`s) which the executable uses.
	 */
}
PortableExecutableBytecode::PortableExecutableBytecode(ClassIoPath path_) : PortableExecutable(std::move(path_)) {
	std::ifstream input(path);
	if(input.good()) {
		bytecode = std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
	}
}
}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassPortableExecutable_cxx */

