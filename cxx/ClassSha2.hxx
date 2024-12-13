/* Dual licenses: choose "Creative Commons" or "Apache 2" (allows all uses) */
#pragma once
#ifndef INCLUDES_cxx_ClassSha2_hxx
#define INCLUDES_cxx_ClassSha2_hxx
#include "ClassPortableExecutable.hxx" /* FileBytecode FileHash */
#include "Macros.hxx" /* SUSUWU_NOEXCEPT SUSUWU_UNIT_TESTS */
namespace Susuwu {
/* Uses https://www.rfc-editor.org/rfc/rfc6234#section-8.2.2 */
/* const */ FileHash /* 128 bits, not null-terminated */ classSha1(const FileBytecode &bytecode);
/* const */ FileHash /* 256 bits, not null-terminated */ classSha256(const FileBytecode &bytecode);
/* const */ FileHash /* 512 bits, not null-terminated */ classSha512(const FileBytecode &bytecode);
typedef FileHash (*ClassSha2)(const FileBytecode &bytecode);
extern ClassSha2 classSha2/* = classSha256 */; /* To compress, apps can execute `sha2 = classSha1;`. To double hash sizes, execute `sha2 = classSha512;`. (Notice: this does not recompute hashes which exist) */
#if SUSUWU_UNIT_TESTS
const bool classSha2Tests();
const bool classSha2TestsNoexcept() SUSUWU_NOEXCEPT;
#endif /* SUSUWU_UNIT_TESTS */
}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassSha2_hxx */

