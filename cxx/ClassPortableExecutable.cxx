/* Attribution (henceforth "*this attribution*", whose syntax is *Markdown*): 2024 [Swudu Susuwu](https://swudususuwu.substack.com)
 * <https://github.com/SwuduSusuwu/SusuLib/> has the newest version of `./cxx/ClassPortableExecutable.cxx` (henceforth "*this source code*").
 * If *this attribution* is shown, *this source code* allows all uses. *This attribution* constitutes the most permissive which is compatible with [*GPLv2*](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) + [*Apache 2*](https://www.apache.org/licenses/LICENSE-2.0.html), which is suitable for personal use (also suitable for school use).
 * If *this attribution* is not professional enough for business use: businesses can use *this source code* through included versions of [*GPLv2*](./LICENSE_GPLv2), [*Apache 2*](./LICENSE), or through both of those. */
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

