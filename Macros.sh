#!/bin/sh
SUSUWU_DIR_SUFFIX_SLASH() { #/* Usage: `OBJDIR=$(SUSUWU_ENSURE_DIR_SLASH "$OBJDIR") */
	DIR=$1
	if [ "${DIR}" = "${DIR%/}" ]; then #/* "%/" removes slash; if equal after this, original doesn't have '/'. */
		DIR="${DIR}/" #/* if original doesn't have, append '/' */
	fi
	echo "${DIR}" #/* return with slash */
}
SUSUWU_DIR_AFFIX_DOTSLASH() { #/* Usage: `OBJDIR=$(SUSUWU_ENSURE_DIR_SLASH "$OBJDIR") */
	DIR=$1
	case "${DIR}" in
		./*) #/* original has "./" */
			;;
		*) #/* default (if original doesn't match "./") */
			DIR="./${DIR}" #/* if original doesn't have, affix "./" */
			;;
	esac
	echo "${DIR}" #/* return with "./" */
}

#/* Based on cxx/Macros.hxx */
SUSUWU_SH_DEFAULT="\033[0m"
SUSUWU_SH_BLACK="\033[0;30m"
SUSUWU_SH_DARK_GRAY="\033[1;30m"
SUSUWU_SH_RED="\033[0;31m"
SUSUWU_SH_LIGHT_RED="\033[1;31m"
SUSUWU_SH_GREEN="\033[0;32m"
SUSUWU_SH_LIGHT_GREEN="\033[1;32m"
SUSUWU_SH_BROWN="\033[0;33m"
SUSUWU_SH_YELLOW="\033[1;33m"
SUSUWU_SH_BLUE="\033[0;34m"
SUSUWU_SH_LIGHT_BLUE="\033[1;34m"
SUSUWU_SH_PURPLE="\033[0;35m"
SUSUWU_SH_LIGHT_PURPLE="\033[1;35m"
SUSUWU_SH_CYAN="\033[0;36m"
SUSUWU_SH_LIGHT_CYAN="\033[1;36m"
SUSUWU_SH_LIGHT_GRAY="\033[0;37m"
SUSUWU_SH_WHITE="\033[1;37m"
SUSUWU_SH_ERROR="[${SUSUWU_SH_RED}Error: ${SUSUWU_SH_WHITE}"
SUSUWU_SH_WARNING="[${SUSUWU_SH_PURPLE}Warning: ${SUSUWU_SH_WHITE}"
SUSUWU_SH_INFO="[${SUSUWU_SH_CYAN}Info: ${SUSUWU_SH_WHITE}"
SUSUWU_SH_SUCCESS="[${SUSUWU_SH_GREEN}Success: ${SUSUWU_SH_WHITE}"
SUSUWU_SH_NOTICE="[${SUSUWU_SH_BLUE}Notice: ${SUSUWU_SH_WHITE}"
SUSUWU_SH_DEBUG="[${SUSUWU_SH_BLUE}Debug: ${SUSUWU_SH_WHITE}"
SUSUWU_SH_CLOSE_="${SUSUWU_SH_DEFAULT}]"
SUSUWU_PRINT() { #/* Usage: `SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "$0 launch" */
	local LEVEL="${1}"; local x="${2}"
	echo "${LEVEL}${x}${SUSUWU_SH_CLOSE_}" >&2 #/* fd=2 is `std::cerr`/`stderr` */
}

SUSUWU_REBUILD_CTAGS() {
	if command -v ctags > /dev/null; then
		ctags -R
	fi
}

SUSUWU_SETUP_CXX_PROCESS_MINGW() { #/* TODO: input args */
	CROSS_COMP=""
	if [ "--mingw" = "${1}" -o  "--mingw" = "${2}" ]; then
		CROSS_COMP=" --mingw"
		LDFLAGS="${LDFLAGS} -static-libgcc -static-libstdc++"
		if command -v x86_64-w64-mingw32-clang++ > /dev/null; then
			CXX="x86_64-w64-mingw32-clang++"
			CXXFLAGS_DEBUG="${CXXFLAGS_DEBUG} ${CXXFLAGS_FSAN}" #/* `-fsan*` [supports `x86_64-w64-mingw32-clang++`](https://github.com/SwuduSusuwu/SubStack/issues/16) */
		elif command -v x86_64-w64-mingw32-g++ > /dev/null; then
			CXX="x86_64-w64-mingw32-g++"
#			CXXFLAGS_DEBUG="${CXXFLAGS_DEBUG} ${CXXFLAGS_FSAN}" #/* `TODO: `-fsan*` for `x86_64-w64-mingw32-g++`](https://www.mingw-w64.org/contribute/#thorough-status-report-for-sanitizers-asan-tsan-usan) */
		else
			SUSUWU_PRINT "${SUSUWU_SH_ERROR}" "\`x86_64-w64-mingw32-clang++ not found\`, \`x86_64-w64-mingw32-g++ not found\`. Do \`apt install llvm-mingw-w64\` or \`apt install mingw-w64\`."
			exit 1
		fi
	elif command -v clang++ > /dev/null; then
		CXX="clang++" #/* TODO: +` -Xclang -analyze -Xclang -analyzer-output=text` (got no extra outputs from this) */
		CXXFLAGS_DEBUG="${CXXFLAGS_DEBUG} ${CXXFLAGS_FSAN}" #/* [`-fsan*` supports `g++`/`clang++`](https://developers.redhat.com/blog/2021/05/05/memory-error-checking-in-c-and-c-comparing-sanitizers-and-valgrind#tldr) */
	elif command -v g++ > /dev/null; then
		CXX="g++"
		CXXFLAGS_DEBUG="${CXXFLAGS_DEBUG} ${CXXFLAGS_FSAN}"
	elif command -v "${CXX}" > /dev/null; then #/* TODO: if our flags are compatible with all `${CXX}`, move this to top */
		SUSUWU_PRINT "${SUSUWU_SH_INFO}" "\`clang++ not found\`, \`g++ not found\`. \`\${CXX}\` (\"${CXX}\") found, will use this."
	else
		SUSUWU_PRINT "${SUSUWU_SH_ERROR}" "\`clang++ not found\`, \`g++ not found\`. \`\${CXX}\` (\"${CXX}\") not found. Do \`apt install clang\` or \`apt install gcc\`."
		exit 1
	fi
}

SUSUWU_PROCESS_RELEASE_DEBUG() { #/* TODO: input args */
	if [ "--release" = "${1}" -o  "--release" = "${2}" ]; then
		SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "\`${0}${CROSS_COMP} --release\` does not support profilers/debuggers (use \`${0}${CROSS_COMP} --debug\` for this)."
		CXXFLAGS="${CXXFLAGS} ${CXXFLAGS_RELEASE}"
	else
		if [ "--debug" != "${1}" ] && [ "--debug" != "${2}" ]; then
			SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "\`${0}${CROSS_COMP}\` defaults to \`${0}${CROSS_COMP} --debug\`."
		fi
		SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "Use \`${0}${CROSS_COMP} --release\` to improve how fast this executes."
		CXXFLAGS="${CXXFLAGS} ${CXXFLAGS_DEBUG}"
		ASAN_OPTIONS=abort_on_error=1:fast_unwind_on_malloc=0:detect_leaks=0 UBSAN_OPTIONS=print_stacktrace=1 #/* "For LLDB/GDB and to prevent very short stack traces and usually false leaks detection" */
	fi
}
SUSUWU_SETUP_BUILD_FLAGS() {
	LD="${CXX}"
	CC="${CXX} -x c"
	LDFLAGS="${LDFLAGS} ${CXXFLAGS}"
	CXXFLAGS="${CXXFLAGS} ${CXXFLAGS_ANALYSIS}"
	CCFLAGS="${CCFLAGS} ${CXXFLAGS}"
}

SUSUWU_SETUP_OUTPUT_DIRS() { #/* Usage: `SUSUWU_SETUP_OUTPUT_DIRS()` */
	if [ -z ${CROSS_COMP} ]; then
		OUTPUT="a.out"
	else
		OUTPUT="a.exe"
	fi
	if [ -z ${OBJDIR} ]; then
		OBJDIR="./obj/"
		SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "To redirect \`${CXX} -c ... -o \${OBJDIR}\${OBJ}.o\` (which has \`OBJDIR=\"${OBJDIR}\"\`), execute \`OBJDIR=\"./obj/\"\` (where \"./obj/\" is a directory which you choose)."
	else
		SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "\`${CXX} -c ... -o \${OBJDIR}\` inherits local \`OBJDIR=\"${OBJDIR}\"\` until you execute \`unset OBJDIR\`."
	fi
	if [ -z ${BINDIR} ]; then
		BINDIR="./bin/"
		SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "To redirect \`${LD} ... -o \${BINDIR}${OUTPUT}\` (which has \`BINDIR=\"${BINDIR}\"\`), execute \`BINDIR=\"./bin/\"\` (where \"./bin/\" is a directory which you choose)."
	else
		SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "\`${LD} ... -o \${BINDIR}${OUTPUT}\` inherits local \`BINDIR=\"${BINDIR}\"\` until you execute \`unset BINDIR\`."
	fi
	C_SOURCE_PATH=$(SUSUWU_DIR_SUFFIX_SLASH "${C_SOURCE_PATH}") #/* if inherit C_SOURCE_PATH, perhaps it lacks '/' */
	CXX_SOURCE_PATH=$(SUSUWU_DIR_SUFFIX_SLASH "${CXX_SOURCE_PATH}") #/* if inherit CXX_SOURCE_PATH, perhaps it lacks '/' */
	OBJDIR=$(SUSUWU_DIR_SUFFIX_SLASH "${OBJDIR}") #/* if inherit OBJDIR, perhaps it is without last '/' */
	BINDIR=$(SUSUWU_DIR_SUFFIX_SLASH "${BINDIR}") #/* if inherit BINDIR, perhaps it is without last '/' */
	mkdir -p "${OBJDIR}" "${BINDIR}"
}
SUSUWU_CLEAN_OUTPUT() {
	rm ${OBJDIR}*.o
}
SUSUWU_PROCESS_CLEAN_REBUILD() {
	if [ "--clean" = "${1}" -o  "--clean" = "${2}" ]; then
		SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "Was called with \`${0}${CROSS_COMP} --clean\`, will remove intermediate objects + exit. Use \`${0}${CROSS_COMP} --rebuild\` to clean + continue."
		SUSUWU_CLEAN_OUTPUT
		exit 0
	fi
	if [ "--rebuild" = "${1}" -o  "--rebuild" = "${2}" ]; then
		SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "Was called with \`${0}${CROSS_COMP} --rebuild\`, will remove intermediate objects + continue."
		SUSUWU_CLEAN_OUTPUT
	fi
}

SUSUWU_BUILD_SOURCES() {
	if [ -e ${BINDIR}${OUTPUT} ]; then
		BUILDNEW=false
	else
		BUILDNEW=true
	fi
	for SOURCE in ${CXX_SOURCE_PATH}Class*.hxx ${CXX_SOURCE_PATH}Macros.hxx; do
		OBJECT="${OBJDIR}$(basename ${SOURCE} .hxx).o" #/* `basename`'s second param removes suffix */
		SRCCXX="${CXX_SOURCE_PATH}$(basename ${SOURCE} .hxx).cxx" #/* `basename`'s second param removes suffix */
		if [ ${OBJECT} -ot ${SOURCE} -a -e ${SOURCE} ]; then #/* `&& [ -e ${SOURCE} ]` is: skip unless `${SOURCE}` exists. */
			SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "\`${SOURCE}\` (which is a common `#include`) is newer than \`${OBJECT}\`, will rebuild all objects."
			SUSUWU_CLEAN_OUTPUT
			break
		elif [ ! -e ${SRCCXX} ]; then #/* If `*.hxx` doesn't have `*.cxx` match,     */
			touch ${OBJECT};            #/* ... then produce `*.o` (for future tests.) */
		fi
	done
	OBJECTLIST=""
	set -x
	#for SOURCE in ${C_SOURCE_PATH}*/*.c; do #/* The extra "*/" is since this just has vendored code (`rfc6234/`) */
	for SOURCE in "${C_SOURCE_PATH}rfc6234/sha1.c" "${C_SOURCE_PATH}rfc6234/sha224-256.c" "${C_SOURCE_PATH}rfc6234/sha384-512.c"; do
		OBJECT="${OBJDIR}$(basename ${SOURCE} .c).o" #/* `basename`'s second param removes suffix */
		if [ ${OBJECT} -ot ${SOURCE} -o ! -e ${OBJECT} ] >/dev/null; then
			${CC} ${CCFLAGS} -c "${SOURCE}" -o "${OBJECT}"
			BUILDNEW=true
		fi
		OBJECTLIST="${OBJECTLIST} ${OBJECT}"
	done
	for SOURCE in ${CXX_SOURCE_PATH}*.cxx; do
		OBJECT="${OBJDIR}$(basename ${SOURCE} .cxx).o" #/* `basename`'s second param removes suffix */
		if [ ${OBJECT} -ot ${SOURCE} -o ! -e ${OBJECT} ] > /dev/null 2>&1; then
			${CXX} ${CXXFLAGS} -c "${SOURCE}" -o "${OBJECT}"
			BUILDNEW=true
		fi
		OBJECTLIST="${OBJECTLIST} ${OBJECT}"
	done
	${BUILDNEW} && ${LD} ${LDFLAGS}${OBJECTLIST} -o "${BINDIR}${OUTPUT}"
	STATUS=$?
	set +x
	if [ 0 -eq ${STATUS} -o false = ${BUILDNEW} ]; then
 		${BUILDNEW} && SUSUWU_PRINT "${SUSUWU_SH_SUCCESS}" "produced \`${BINDIR}${OUTPUT}\` (`stat -c%s ${BINDIR}${OUTPUT}` bytes)."
		${BUILDNEW} || SUSUWU_PRINT "${SUSUWU_SH_SUCCESS}" "reused \`${BINDIR}${OUTPUT}\` (`stat -c%s ${BINDIR}${OUTPUT}` bytes)."
	fi
	return ${STATUS}
}

SUSUWU_TEST_OUTPUT() {
	if [ 0 -eq ${STATUS} -o false = ${BUILDNEW} ]; then
		${BINDIR}${OUTPUT}
		STATUS=$?
		if [ 0 -eq ${STATUS} ]; then
			SUSUWU_PRINT "${SUSUWU_SH_SUCCESS}" "\`${BINDIR}${OUTPUT}\` returned status SusuwuUnitTestsBitmask(${STATUS})."
		else
			SUSUWU_PRINT "${SUSUWU_SH_ERROR}" "\`${BINDIR}${OUTPUT}\` returned status SusuwuUnitTestsBitmask(${STATUS})."
		fi
	fi
	return ${STATUS}
}

