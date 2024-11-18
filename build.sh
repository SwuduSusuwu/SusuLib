#!/bin/sh
. ./Macros.sh #/* SUSUWU_BUILD_SOURCES() SUSUWU_PRINT() SUSUWU_PROCESS_CLEAN_REBUILD() SUSUWU_PROCESS_RELEASE_DEBUG() SUSUWU_SETUP_CXX_PROCESS_MINGW() SUSUWU_SETUP_OUTPUT_DIRS() SUSUWU_SH_* SUSUWU_TEST_OUTPUT() */
SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "Dual licenses: choose \"Creative Commons\" or \"Apache 2\" (allows all uses)."

if command -v ctags > /dev/null; then
	ctags -R
fi

CXXFLAGS_ANALYSIS="-Wall -Wno-unused -Wno-unused-function -Wextra -Wno-unused-parameter -Wno-ignored-qualifiers" #/*TODO: -`-Wno-*`, +`-Wpedantic`, +`-Werror` */
CXXFLAGS_RELEASE="-fomit-frame-pointer -DNDEBUG -O2" #/* without frame pointer (pointer used for stacktraces), without `assert(...)`/`SUSUWU_DEBUG(...)`/`SUSUWU_NOTICE(...)`, with optimization level 2 */
CXXFLAGS_DEBUG="-std=c++11 -g -Og" #/* in MSVC is `/Zi /Od`: symbols for `gdb`/`lldb` use, optimizations compatible with `-g`/`-fsan*` */
CXXFLAGS_DEBUG="-g -Og" #/* in MSVC is `/Zi /Od`: symbols for `gdb`/`lldb` use, optimizations compatible with `-g`/`-fsan*` */
CXXFLAGS_DEBUG="${CXXFLAGS_DEBUG} -fno-omit-frame-pointer" #/* thus optimization won't remove stacktraces: https://stackoverflow.com/questions/48234575/g-will-fno-omit-frame-pointer-be-effective-if-specified-before-o2-or-o3 https://clang.llvm.org/docs/MemorySanitizer.html */
#CXXFLAGS_DEBUG="${CXXFLAGS_DEBUG} -fno-optimize-sibling-calls" #/* Don't inline functions. Does extra stacktraces. */
CXXFLAGS_FSAN="-fsanitize=address -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment"
#CXXFLAGS_FSAN="${CXXFLAGS_FSAN} -fsanitize=undefined" #/* causes 'cannot locate symbol "__ubsan_handle_function_type_mismatch_abort"' */

SUSUWU_SETUP_CXX_PROCESS_MINGW $@

SUSUWU_PROCESS_RELEASE_DEBUG $@

LD="${CXX}"
CC="${CXX} -x c"
LDFLAGS="${LDFLAGS} ${CXXFLAGS}"
CXXFLAGS="${CXXFLAGS} ${CXXFLAGS_ANALYSIS}"
CCFLAGS="${CCFLAGS} ${CXXFLAGS}"
C_SOURCE_PATH="./c/"
CXX_SOURCE_PATH="./cxx/"

BUILDNEW=false
SUSUWU_SETUP_OUTPUT_DIRS
SUSUWU_PROCESS_CLEAN_REBUILD $@

SUSUWU_BUILD_SOURCES
STATUS=$?

SUSUWU_TEST_OUTPUT
STATUS=$?
return ${STATUS}

