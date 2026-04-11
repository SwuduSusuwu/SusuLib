/* Attribution (henceforth "*this attribution*", whose syntax is *Markdown*): 2024 [Swudu Susuwu](https://swudususuwu.substack.com)
 * <https://github.com/SwuduSusuwu/SusuLib/> has the newest version of `./cxx/ClassSha2.hxx` (henceforth "*this source code*").
 * If *this attribution* is shown, *this source code* allows all uses. *This attribution* constitutes the most permissive which is compatible with [*GPLv2*](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) + [*Apache 2*](https://www.apache.org/licenses/LICENSE-2.0.html), which is suitable for personal use (also suitable for school use).
 * If *this attribution* is not professional enough for business use: businesses can use *this source code* through included versions of [*GPLv2*](./LICENSE_GPLv2), [*Apache 2*](./LICENSE), or through both of those. */
#pragma once
#ifndef INCLUDES_cxx_ClassSha2_hxx
#define INCLUDES_cxx_ClassSha2_hxx
#include "ClassIo.hxx" /* ClassIoBytecode ClassIoHash */
#include "Macros.hxx" /* SUSUWU_NOEXCEPT SUSUWU_UNIT_TESTS */
namespace Susuwu {
/* Uses https://www.rfc-editor.org/rfc/rfc6234#section-8.2.2 */
/* const */ ClassIoHash /* 128 bits, not null-terminated */ classSha1(const ClassIoBytecode &bytecode);
/* const */ ClassIoHash /* 256 bits, not null-terminated */ classSha256(const ClassIoBytecode &bytecode);
/* const */ ClassIoHash /* 512 bits, not null-terminated */ classSha512(const ClassIoBytecode &bytecode);
typedef ClassIoHash (*ClassSha2)(const ClassIoBytecode &bytecode);
extern ClassSha2 classSha2/* = classSha256 */; /* To compress, apps can execute `classSha2 = classSha1;`. To double hash sizes, execute `classSha2 = classSha512;`. (Notice: this does not recompute hashes which exist) */
#if SUSUWU_UNIT_TESTS
const bool classSha2Tests();
const bool classSha2TestsNoexcept() SUSUWU_NOEXCEPT;
#endif /* SUSUWU_UNIT_TESTS */
}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassSha2_hxx */

