#!/bin/sh
. ./Macros.sh #/* SUSUWU_BUILD_CTAGS SUSUWU_BUILD_SOURCES() SUSUWU_PRINT() SUSUWU_PROCESS_CLEAN_REBUILD() SUSUWU_PROCESS_MINGW() SUSUWU_PROCESS_RELEASE_DEBUG() SUSUWU_SETUP_BUILD_FLAGS() SUSUWU_SETUP_CXX() SUSUWU_SETUP_BINDIR() SUSUWU_SETUP_OBJDIR() SUSUWU_SETUP_OUTPUT() SUSUWU_SH_* SUSUWU_TEST_OUTPUT() */
SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "Dual licenses: choose \"Creative Commons\" or \"Apache 2\" (allows all uses)."

SUSUWU_BUILD_CTAGS #/* Usage: `apt-get install ctags` */

CXXFLAGS_SPECIAL="" #/* You can put special flags from `build.sh` into this to use. */
CXXFLAGS_ANALYSIS="-Wall -Wno-unused -Wno-unused-function -Wextra -Wno-unused-parameter -Wno-ignored-qualifiers" #/*TODO: -`-Wno-*`, +`-Wpedantic`, +`-Werror` */
CXXFLAGS_RELEASE="-fomit-frame-pointer -DNDEBUG -O2" #/* without frame pointer (pointer used for stacktraces), without `assert(...)`/`SUSUWU_DEBUG(...)`/`SUSUWU_NOTICE(...)`, with optimization level 2 */
CXXFLAGS_DEBUG="-std=c++11 -g -Og" #/* in MSVC is `/Zi /Od`: symbols for `gdb`/`lldb` use, optimizations compatible with `-g`/`-fsan*` */
CXXFLAGS_DEBUG="-g -Og" #/* in MSVC is `/Zi /Od`: symbols for `gdb`/`lldb` use, optimizations compatible with `-g`/`-fsan*` */
CXXFLAGS_DEBUG="${CXXFLAGS_DEBUG} -fno-omit-frame-pointer" #/* thus optimization won't remove stacktraces: https://stackoverflow.com/questions/48234575/g-will-fno-omit-frame-pointer-be-effective-if-specified-before-o2-or-o3 https://clang.llvm.org/docs/MemorySanitizer.html */
#CXXFLAGS_DEBUG="${CXXFLAGS_DEBUG} -fno-optimize-sibling-calls" #/* Don't inline functions. Does extra stacktraces. */
CXXFLAGS_FSAN="-fsanitize=address -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment"
#CXXFLAGS_FSAN="${CXXFLAGS_FSAN} -fsanitize=undefined" #/* causes 'cannot locate symbol "__ubsan_handle_function_type_mismatch_abort"' */
C_SOURCE_PATH="./c/"
CXX_SOURCE_PATH="./cxx/"


SUSUWU_PROCESS_MINGW $@ #/* Usage: `./build.sh --mingw` */
SUSUWU_SETUP_CXX

SUSUWU_PROCESS_RELEASE_DEBUG $@ #/* Usage: `./build.sh --debug` or `./build.sh --release` */
SUSUWU_SETUP_BUILD_FLAGS

BUILDNEW=false
SUSUWU_SETUP_OUTPUT "a"
SUSUWU_SETUP_OBJDIR "./obj/"
SUSUWU_SETUP_BINDIR "./bin/"
SUSUWU_PROCESS_CLEAN_REBUILD $@ #/* Usage: `./build.sh --clean` or `./build.sh --rebuild` */

SUSUWU_PROCESS_INCLUDES ${CXX_SOURCE_PATH}Class*.hxx ${CXX_SOURCE_PATH}Macros.hxx
SUSUWU_BUILD_SOURCES
STATUS=$?

SUSUWU_TEST_OUTPUT
STATUS=$?
return ${STATUS}

