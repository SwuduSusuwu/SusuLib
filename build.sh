#!/bin/sh
. ./Macros.sh #/* SUSUWU_BUILD_CTAGS SUSUWU_BUILD_OBJECTS() SUSUWU_BUILD_EXECUTABLE() SUSUWU_INSTALL() SUSUWU_PRINT() SUSUWU_PROCESS_CLEAN_REBUILD() SUSUWU_PROCESS_MINGW() SUSUWU_PROCESS_RELEASE_DEBUG() SUSUWU_PROCESS_S() SUSUWU_PROCESS_VERBOSE() SUSUWU_SETUP_BUILD_FLAGS() SUSUWU_SETUP_CXX() SUSUWU_SETUP_BINDIR() SUSUWU_SETUP_OBJDIR() SUSUWU_SETUP_OUTPUT() SUSUWU_SH_* SUSUWU_TEST_OUTPUT() SUSUWU_UNINSTALL() */
SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "Dual licenses: choose \"Creative Commons\" or \"Apache 2\" (allows all uses)."

FLAGS_SPECIAL="" #/* You can put special flags from `build.sh` into this to use. */
FLAGS_ANALYSIS="-Wall -Wno-unused-function -Wno-unused-function -Wextra -Wno-unused-parameter -Wno-ignored-qualifiers -Wpedantic" #/*TODO: -`-Wno-*`, +`-Werror` */
FLAGS_RELEASE="-fomit-frame-pointer -DNDEBUG -O2" #/* without frame pointer (pointer used for stacktraces), without `assert(...)`/`SUSUWU_DEBUG(...)`/`SUSUWU_NOTICE(...)`, with optimization level 2 */
CXXFLAGS_DEBUG="-std=c++11" #/* ensure unit tests pass with C++11 support as max */
FLAGS_DEBUG="-g -Og" #/* in MSVC is `/Zi /Od`: symbols for `gdb`/`lldb` use, optimizations compatible with `-g`/`-fsan*` */
FLAGS_DEBUG="${FLAGS_DEBUG} -fno-omit-frame-pointer" #/* thus optimization won't remove stacktraces: https://stackoverflow.com/questions/48234575/g-will-fno-omit-frame-pointer-be-effective-if-specified-before-o2-or-o3 https://clang.llvm.org/docs/MemorySanitizer.html */
#CXXFLAGS_DEBUG="${CXXFLAGS_DEBUG} -fno-optimize-sibling-calls" #/* Don't inline functions. Does extra stacktraces. */
FLAGS_FSAN="-fsanitize=address -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment"
#FLAGS_FSAN="${FLAGS_FSAN} -fsanitize=undefined" #/* causes 'cannot locate symbol "__ubsan_handle_function_type_mismatch_abort"' */
C_SOURCE_PATH="./c/"
CXX_SOURCE_PATH="./cxx/"


SUSUWU_PROCESS_S $@ #/* Usage: `./build.sh -q`. Silences `SUSUWU_SH_NOTICE` ("Notice:") messages, prevents `set -x`. */
SUSUWU_PROCESS_VERBOSE $@ #/* Usage: `./build.sh --verbose`. Enables `SUSUWU_SH_DEBUG` ("Debug:") messages, forces `set -x`. */
SUSUWU_PROCESS_MINGW "$@" #/* Usage: `apt install mingw wine && ./build.sh --mingw`. [MinGW cross-builds to Windows.] */
SUSUWU_SETUP_CXX #/* Analogous to `make config` */
SUSUWU_PROCESS_RELEASE_DEBUG "$@" #/* Usage: `./build.sh --debug` or `./build.sh --release` */

SUSUWU_SETUP_OBJDIR "./obj/"
SUSUWU_SETUP_BINDIR "./bin/"
SUSUWU_SETUP_OUTPUT "a"
SUSUWU_PROCESS_CLEAN_REBUILD "$@" #/* Usage: `./build.sh --clean` or `./build.sh --rebuild` */

SUSUWU_SETUP_BUILD_FLAGS #/* Analogous to `make config` */
SUSUWU_PROCESS_INCLUDES "${CXX_SOURCE_PATH}Class*.hxx" "${CXX_SOURCE_PATH}Macros.hxx"
SUSUWU_BUILD_CTAGS #/* Usage: `apt-get install ctags vim && vim -t tagToSearchFor` */
SUSUWU_BUILD_OBJECTS "${CC} ${CFLAGS}" ".c" "${C_SOURCE_PATH}rfc6234/sha1.c" "${C_SOURCE_PATH}rfc6234/sha224-256.c" "${C_SOURCE_PATH}rfc6234/sha384-512.c"
SUSUWU_BUILD_OBJECTS "${CXX} ${CXXFLAGS}" ".cxx" "${CXX_SOURCE_PATH}*.cxx"
SUSUWU_BUILD_EXECUTABLE
STATUS=$?

SUSUWU_TEST_OUTPUT #/* Analogous to `make test` or `make execute` */
STATUS=$?
SUSUWU_INSTALL && SUSUWU_UNINSTALL #/* Analogous to `make install && make uninstall`. Won't clobber files which exist. */
return ${STATUS}

