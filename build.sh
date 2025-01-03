#!/bin/sh
#
#/* Builds `./c/` and `./cxx/` into `./obj/` and `./bin/`. Usage: "Console flags" from `./README.md#optionssetup`. */
[ -e "./sh/Macros.sh" ] || echo "[Error: \`./$(basename "$0")\` was not executed from this repo's root.]"
. ./sh/Macros.sh #/* SUSUWU_DEFAULT_BRANCH() SUSUWU_PRINT() SUSUWU_PROCESS_S() SUSUWU_PROCESS_VERBOSE() SUSUWU_PRODUCTION_USE() SUSUWU_SH_* */
. ./sh/make.sh #/* SUSUWU_BUILD_CTAGS SUSUWU_BUILD_OBJECTS() SUSUWU_BUILD_EXECUTABLE() SUSUWU_INSTALL() SUSUWU_PROCESS_CLEAN_REBUILD() SUSUWU_PROCESS_MINGW() SUSUWU_PROCESS_RELEASE_DEBUG() SUSUWU_SETUP_BUILD_FLAGS() SUSUWU_SETUP_CXX() SUSUWU_SETUP_BINDIR() SUSUWU_SETUP_OBJDIR() SUSUWU_SETUP_OUTPUT() SUSUWU_TEST_OUTPUT() SUSUWU_UNINSTALL() */
SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "(C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses."

THIS_DEFAULT_BRANCH="$(SUSUWU_DEFAULT_BRANCH "trunk")"
FLAGS_USER="" #/* Usage: "Macro flags" from `./README.md#optionssetup`. */
FLAGS_ANALYSIS="-Wall -Wno-unused-function -Wno-unused-function -Wextra -Wno-unused-parameter -Wno-ignored-qualifiers -Wpedantic" #/*TODO: -`-Wno-*`, +`-Werror` */
FLAGS_RELEASE="-fomit-frame-pointer -DNDEBUG -O2" #/* without frame pointer (pointer used for stacktraces), without `assert(...)`/`SUSUWU_DEBUG(...)`/`SUSUWU_NOTICE(...)`, with optimization level 2 */
CXXFLAGS_DEBUG="-std=c++11" #/* ensure unit tests pass with C++11 support as max */
FLAGS_DEBUG="-g -Og" #/* in MSVC is `/Zi /Od`: symbols for `gdb`/`lldb` use, optimizations compatible with `-g`/`-fsan*` */
FLAGS_DEBUG="${FLAGS_DEBUG} -fno-omit-frame-pointer" #/* thus optimization won't remove stacktraces: https://stackoverflow.com/questions/48234575/g-will-fno-omit-frame-pointer-be-effective-if-specified-before-o2-or-o3 https://clang.llvm.org/docs/MemorySanitizer.html */
#FLAGS_DEBUG="${FLAGS_DEBUG} -fno-optimize-sibling-calls" #/* Don't inline functions. Does extra stacktraces. */
FLAGS_FSAN="-fsanitize=address -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment"
#FLAGS_FSAN="${FLAGS_FSAN} -fsanitize=undefined" #/* causes 'cannot locate symbol "__ubsan_handle_function_type_mismatch_abort"' */
C_SOURCE_PATH="./c/" #/* Usage: replace with directory root for _C_ source code */
CXX_SOURCE_PATH="./cxx/" #/* Usage: replace with directory root for _C++_ source code */


SUSUWU_PROCESS_S $@ #/* Usage: `./build.sh -q`. Silences `SUSUWU_SH_NOTICE` ("Notice:") messages, prevents `set -x`. */
SUSUWU_PROCESS_VERBOSE $@ #/* Usage: `./build.sh --verbose`. Enables `SUSUWU_SH_DEBUG` ("Debug:") messages, forces `set -x`. */
SUSUWU_PRODUCTION_USE "${THIS_DEFAULT_BRANCH}"
SUSUWU_PROCESS_MINGW "$@" #/* Usage: `apt install mingw wine && ./build.sh --mingw`. [MinGW cross-builds to Windows.] */
SUSUWU_SETUP_CXX #/* Analogous to `make config` */
SUSUWU_PROCESS_RELEASE_DEBUG "$@" #/* Usage: `./build.sh --debug` or `./build.sh --release` */

SUSUWU_SETUP_OBJDIR "./obj/" #/* Usage: replace with directory root for new objects */
SUSUWU_SETUP_BINDIR "./bin/" #/* Usage: replace with directory root for new executables */
SUSUWU_SETUP_OUTPUT "Susuwu" #/* Usage: replace with name of your program */
SUSUWU_PROCESS_CLEAN_REBUILD "$@" #/* Usage: `./build.sh --clean` or `./build.sh --rebuild` */

SUSUWU_SETUP_BUILD_FLAGS #/* Analogous to `make config` */
SUSUWU_PROCESS_INCLUDES "${CXX_SOURCE_PATH}Class*.hxx" "${CXX_SOURCE_PATH}Macros.hxx"
SUSUWU_BUILD_CTAGS "-R --exclude=.git/ --exclude=*.html" "./sh/" "${C_SOURCE_PATH}" "${CXX_SOURCE_PATH}" #/* Usage: `apt install ctags vim && vim -t tagToSearchFor` */
SUSUWU_BUILD_OBJECTS "${CC}" "${CFLAGS}" ".c" "${C_SOURCE_PATH}rfc6234/sha1.c" "${C_SOURCE_PATH}rfc6234/sha224-256.c" "${C_SOURCE_PATH}rfc6234/sha384-512.c"
SUSUWU_BUILD_OBJECTS "${CXX}" "${CXXFLAGS}" ".cxx" "${CXX_SOURCE_PATH}*.cxx"
SUSUWU_BUILD_EXECUTABLE
SUSUWU_STATUS=$?
SUSUWU_BUILD_STATUS=${SUSUWU_STATUS}

SUSUWU_TEST_OUTPUT #/* Analogous to `make test` or `make execute` */
SUSUWU_STATUS=$?
[ 0 -eq ${SUSUWU_BUILD_STATUS} ] && SUSUWU_INSTALL "${USRBIN}" && SUSUWU_UNINSTALL "${USRBIN}" #/* Analogous to `make install && make uninstall`. Won't clobber files which exist. */
return ${SUSUWU_STATUS}

