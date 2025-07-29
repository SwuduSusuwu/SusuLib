/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#ifndef INCLUDES_cxx_ClassPortableExecutable_cxx
#define INCLUDES_cxx_ClassPortableExecutable_cxx
#include "ClassIo.hxx" /* ClassIoPath */
#include "ClassPortableExecutable.hxx" /* PortableExecutableBytecode */
#include <fstream> /* std::ifstream */
#include <iterator> /* std::istreambuf_iterator */
#include <string> /* std::string */
#include <utility> /* std::move */
namespace Susuwu {
PortableExecutableBytecode::PortableExecutableBytecode(ClassIoPath path_) : PortableExecutable(std::move(path_)) {
	std::ifstream input(path);
	if(input.good()) {
		bytecode = std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
	}
}
}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassPortableExecutable_cxx */

