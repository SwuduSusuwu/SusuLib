/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
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
extern ClassSha2 classSha2/* = classSha256 */; /* To compress, apps can execute `sha2 = classSha1;`. To double hash sizes, execute `sha2 = classSha512;`. (Notice: this does not recompute hashes which exist) */
#if SUSUWU_UNIT_TESTS
const bool classSha2Tests();
const bool classSha2TestsNoexcept() SUSUWU_NOEXCEPT;
#endif /* SUSUWU_UNIT_TESTS */
}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassSha2_hxx */

