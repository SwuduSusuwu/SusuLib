#!/bin/sh
#
# /* Attribution (henceforth "*this attribution*", whose syntax is *Markdown*): 2024 [Swudu Susuwu](https://swudususuwu.substack.com)
#  * <https://github.com/SwuduSusuwu/SusuLib/> has the newest version of `./build.sh` (henceforth "*this source code*").
#  * If *this attribution* is shown, *this source code* allows all uses. *This attribution* constitutes the most permissive which is compatible with [*GPLv2*](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) + [*Apache 2*](https://www.apache.org/licenses/LICENSE-2.0.html), which is suitable for personal use (also suitable for school use).
#  * If *this attribution* is not professional enough for business use: businesses can use *this source code* through included versions of [*GPLv2*](./LICENSE_GPLv2), [*Apache 2*](./LICENSE), or through both of those. */
# /* This is alternative to `make config; make build; make test`.
#  * Builds `./c/` and `./cxx/` into `./obj/` and `./bin/`. Usage: "Console flags" from `./README.md#optionssetup`.
#  */

GIT_ROOT="$(dirname "$(git rev-parse --git-dir)")/" #`git` does not set `${GIT_DIR}`, nor `${GIT_WORK_TREE}`
SUSUWU_INCLUDE_ERROR() { #/* Usage; `SUSUWU_INCLUDE_ERROR "<relative path>" "error message"` */
	echo "[$0: Error: \`GIT_WORK_TREE=${GIT_ROOT}\` \`\${GIT_WORK_TREE}${1}\` ${2}.]"; exit 1
}
#shellcheck source=./sh/make.sh
SUSUWU_INCLUDE() { #/* Usage; `SUSUWU_INCLUDE "<relative path>"` */
	SUSUWU_INCLUDE_PATH=${1}; shift #/* So path is not included in source'd scripts `$@` and `$*`. */
	if [ ! -e "${GIT_ROOT}${SUSUWU_INCLUDE_PATH}" ]; then
		SUSUWU_INCLUDE_ERROR "${SUSUWU_INCLUDE_PATH}" "was not found"
	elif [ ! -x "${GIT_ROOT}${SUSUWU_INCLUDE_PATH}" ] || ! . "${GIT_ROOT}${SUSUWU_INCLUDE_PATH}"; then
		SUSUWU_INCLUDE_ERROR "${SUSUWU_INCLUDE_PATH}" "is not executable"
	fi
}
SUSUWU_INCLUDE "./sh/Macros.sh" #/* SUSUWU_DEFAULT_BRANCH() SUSUWU_PRINT() SUSUWU_PROCESS_ABORT_ON_FIRST_ERROR() SUSUWU_PROCESS_S() SUSUWU_PROCESS_VERBOSE() SUSUWU_PRODUCTION_USE() SUSUWU_SH_* */
SUSUWU_INCLUDE "./sh/make.sh" #/* SUSUWU_BUILD_CTAGS SUSUWU_BUILD_OBJECTS() SUSUWU_BUILD_EXECUTABLE() SUSUWU_INSTALL() SUSUWU_PROCESS_CLEAN_REBUILD() SUSUWU_PROCESS_MINGW() SUSUWU_PROCESS_RELEASE_DEBUG() SUSUWU_SETUP_BUILD_FLAGS() SUSUWU_SETUP_CXX() SUSUWU_SETUP_BINDIR() SUSUWU_SETUP_OBJDIR() SUSUWU_SETUP_OUTPUT() SUSUWU_TEST_BASH() SUSUWU_TEST_OUTPUT() SUSUWU_UNINSTALL() */
SUSUWU_PRINT "$(SUSUWU_SH_NOTICE)" "(C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses."

THIS_DEFAULT_BRANCH="$(SUSUWU_DEFAULT_BRANCH "trunk")"
SUSUWU_REV_PARSE_HEAD="$(git rev-parse HEAD)"
export FLAGS_USER="-DSUSUWU_REV_PARSE_HEAD=\"${SUSUWU_REV_PARSE_HEAD}\"" #/* Usage: "Macro flags" from `./README.md#optionssetup`. */
export FLAGS_ANALYSIS="-Wall -Wno-unused-function -Wno-unused-function -Wextra -Wno-unused-parameter -Wno-ignored-qualifiers -Wpedantic" #/*TODO: -`-Wno-*`, +`-Werror` */
export FLAGS_RELEASE="-fomit-frame-pointer -DNDEBUG -O2" #/* without frame pointer (pointer used for stacktraces), without `assert(...)`/`SUSUWU_DEBUG(...)`/`SUSUWU_NOTICE(...)`, with optimization level 2 */
export CXXFLAGS_DEBUG="-std=c++11" #/* ensure unit tests pass with C++11 support as max */
export FLAGS_DEBUG="-g -Og" #/* in MSVC is `/Zi /Od`: symbols for `gdb`/`lldb` use, optimizations compatible with `-g`/`-fsan*` */
export FLAGS_DEBUG="${FLAGS_DEBUG} -fno-omit-frame-pointer" #/* thus optimization won't remove stacktraces: https://stackoverflow.com/questions/48234575/g-will-fno-omit-frame-pointer-be-effective-if-specified-before-o2-or-o3 https://clang.llvm.org/docs/MemorySanitizer.html */
#export FLAGS_DEBUG="${FLAGS_DEBUG} -fno-optimize-sibling-calls" #/* Don't inline functions. Does extra stacktraces. */
export FLAGS_FSAN="-fsanitize=address -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment"
#export FLAGS_FSAN="${FLAGS_FSAN} -fsanitize=undefined" #/* causes 'cannot locate symbol "__ubsan_handle_function_type_mismatch_abort"' */
C_SOURCE_PATH="./c/" #/* Usage: replace with directory root for _C_ source code */
CXX_SOURCE_PATH="./cxx/" #/* Usage: replace with directory root for _C++_ source code */


SUSUWU_PROCESS_S "$@" #/* Usage: `./build.sh -q`. Silences `SUSUWU_SH_NOTICE` ("Notice:") messages, prevents `set -x`. */
SUSUWU_PROCESS_VERBOSE "$@" #/* Usage: `./build.sh --verbose`. Enables `SUSUWU_SH_DEBUG` ("Debug:") messages, forces `set -x`. */
SUSUWU_PRODUCTION_USE "${THIS_DEFAULT_BRANCH}"
SUSUWU_PROCESS_MINGW "$@" #/* Usage: `apt install mingw wine && ./build.sh --mingw`. [MinGW cross-builds to Windows.] */
SUSUWU_SETUP_CXX #/* Analogous to `make config` */
SUSUWU_PROCESS_RELEASE_DEBUG "$@" #/* Usage: `./build.sh --debug` or `./build.sh --release` */
SUSUWU_PROCESS_ABORT_ON_FIRST_ERROR "$@" #/* Usage: `./build.sh --abort-on-first-error`. If an object fails to build, `exit 1`. */

SUSUWU_SETUP_OBJDIR "./obj/" #/* Usage: replace with directory root for new objects */
SUSUWU_SETUP_BINDIR "./bin/" #/* Usage: replace with directory root for new executables */
SUSUWU_SETUP_OUTPUT "Susuwu" #/* Usage: replace with name of your program */
SUSUWU_PROCESS_CLEAN_REBUILD "$@" #/* Usage: `./build.sh --clean` or `./build.sh --rebuild` */

SUSUWU_SETUP_BUILD_FLAGS #/* Analogous to `make config` */

SUSUWU_INCLUDE "./sh/Includes.sh" #/* CFLAGS, CXXFLAGS, LDFLAGS, SUSUWU_INCLUDES_LIBPUGIXML(), SUSUWU_INCLUDES_LIBXML2(), USER_FLAGS */
SUSUWU_INCLUDES_LIBPUGIXML
SUSUWU_INCLUDES_LIBXML2

SUSUWU_PROCESS_INCLUDES "${CXX_SOURCE_PATH}Class*.hxx" "${CXX_SOURCE_PATH}Macros.hxx"
#shellcheck disable=SC2119 #Specifics were removed from `SUSUWU_BUILD_CTAGS` call to match `./hooks/pre-commit`.
SUSUWU_BUILD_CTAGS #/* Usage: `apt install ctags vim && vim -t tagToSearchFor` */
SUSUWU_BUILD_OBJECTS "${CC}" "${CFLAGS}" ".c" "${C_SOURCE_PATH}rfc6234/sha1.c" "${C_SOURCE_PATH}rfc6234/sha224-256.c" "${C_SOURCE_PATH}rfc6234/sha384-512.c"
SUSUWU_BUILD_OBJECTS "${CXX}" "${CXXFLAGS}" ".cxx" "${CXX_SOURCE_PATH}*.cxx"
SUSUWU_BUILD_EXECUTABLE
SUSUWU_STATUS=$?
SUSUWU_BUILD_STATUS=${SUSUWU_STATUS}

SUSUWU_TEST_OUTPUT #/* Analogous to `make test` or `make execute` */
SUSUWU_STATUS=$?
[ 0 -eq ${SUSUWU_BUILD_STATUS} ] && SUSUWU_INSTALL "${USRBIN}" && SUSUWU_UNINSTALL "${USRBIN}" #/* Analogous to `make install && make uninstall`. Won't clobber files which exist. */
[ 0 -eq ${SUSUWU_STATUS} ] && {
	SUSUWU_TEST_BASH || SUSUWU_STATUS=$?
}
exit ${SUSUWU_STATUS}

