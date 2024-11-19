#!/bin/sh
SUSUWU_DIR_SUFFIX_SLASH() { #/* Usage: `OBJDIR=$(SUSUWU_ENSURE_DIR_SLASH "$OBJDIR") */
	DIR=$1
	if [ "${DIR}" = "${DIR%/}" ]; then #/* "%/" removes slash; if equal after this, original doesn't have '/'. */
		DIR="${DIR}/" #/* if original doesn't have, append '/' */
	fi
	echo "${DIR}" #/* return with slash */
}
SUSUWU_DIR_AFFIX_DOTSLASH() { #/* Usage: `BINDIR=$(SUSUWU_ENSURE_DIR_SLASH "$BINDIR") */
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

SUSUWU_BUILD_CTAGS() {
	if command -v ctags > /dev/null; then
		ctags -R
	fi
}

SUSUWU_PROCESS_MINGW() { #/* Usage: `SUSUWU_PROCESS_MINGW $?` */
	CROSS_COMP=""
	if [ "--mingw" = "${1}" -o  "--mingw" = "${2}" ]; then
		CROSS_COMP=" --mingw"
		LDFLAGS="${LDFLAGS} -static-libgcc -static-libstdc++"
		if command -v x86_64-w64-mingw32-clang++ > /dev/null; then
			CXX="x86_64-w64-mingw32-clang++"
			USE_FSAN=true #/* `-fsan*` [supports `x86_64-w64-mingw32-clang++`](https://github.com/SwuduSusuwu/SubStack/issues/16) */
		elif command -v x86_64-w64-mingw32-g++ > /dev/null; then
			CXX="x86_64-w64-mingw32-g++"
			USE_FSAN=false #/* `TODO: `-fsan*` for `x86_64-w64-mingw32-g++`](https://www.mingw-w64.org/contribute/#thorough-status-report-for-sanitizers-asan-tsan-usan) */
		else
			SUSUWU_PRINT "${SUSUWU_SH_ERROR}" "\`x86_64-w64-mingw32-clang++ not found\`, \`x86_64-w64-mingw32-g++ not found\`. Do \`apt install llvm-mingw-w64\` or \`apt install mingw-w64\`."
			exit 1
		fi
	fi
}
SUSUWU_SETUP_CXX() { #/* Usage: ... [SUSUWU_PROCESS_MINGE $?] SUSUWU_SETUP_CXX [SUSUWU_PROCESS_RELEASE_DEBUG $?] SUSUWU_SETUP_BUILD_FLAGS SUSUWU_SETUP_BINDIR "" SUSUWU_SETUP_OBJDIR "" SUSUWU_SETUP_OUTPUT "" [SUSUWU_PROCESS_CLEAN_REBUILD $?] [SUSUWU_PROCESS_INCLUDES ""] SUSUWU_BUILD_SOURCES ... */
	if [ ! -z ${CROSS_COMP} ]; then #/* if `--mingw`,         */
		return;                       #/* don't override `CXX`. */
	elif command -v clang++ > /dev/null; then
		CXX="clang++" #/* TODO: +` -Xclang -analyze -Xclang -analyzer-output=text` (got no extra outputs from this) */
		USE_FSAN=true #/* [`-fsan*` supports `g++`/`clang++`](https://developers.redhat.com/blog/2021/05/05/memory-error-checking-in-c-and-c-comparing-sanitizers-and-valgrind#tldr) */
	elif command -v g++ > /dev/null; then
		CXX="g++"
		USE_FSAN=true
	elif command -v "${CXX}" > /dev/null; then #/* TODO: if our flags are compatible with all `${CXX}`, move this to top */
		SUSUWU_PRINT "${SUSUWU_SH_INFO}" "\`clang++ not found\`, \`g++ not found\`. \`\${CXX}\` (\"${CXX}\") found, will use this."
		USE_FSAN=false #/* `TODO: test unknown compilers for `-fsan*` support */
	else
		SUSUWU_PRINT "${SUSUWU_SH_ERROR}" "\`clang++ not found\`, \`g++ not found\`. \`\${CXX}\` (\"${CXX}\") not found. Do \`apt install clang\` or \`apt install gcc\`."
		exit 1
	fi
}

SUSUWU_PROCESS_RELEASE_DEBUG() { #/* Usage: `SUSUWU_PROCESS_RELEASE_DEBUG $?` */
	if [ "--release" = "${1}" -o  "--release" = "${2}" ]; then
		SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "\`${0}${CROSS_COMP} --release\` does not support profilers/debuggers (use \`${0}${CROSS_COMP} --debug\` for this)."
		CXXFLAGS="${CXXFLAGS} ${CXXFLAGS_RELEASE}"
	else
		if [ "--debug" != "${1}" ] && [ "--debug" != "${2}" ]; then
			SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "\`${0}${CROSS_COMP}\` defaults to \`${0}${CROSS_COMP} --debug\`."
		fi
		SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "Use \`${0}${CROSS_COMP} --release\` to improve how fast this executes."
		CXXFLAGS="${CXXFLAGS} ${CXXFLAGS_DEBUG}"
		if [ true = ${USE_FSAN} ]; then
			CXXFLAGS="${CXXFLAGS} ${FLAGS_FSAN}"
			LDFLAGS="${LDFLAGS} ${FLAGS_FSAN}"
			export ASAN_OPTIONS=abort_on_error=1:fast_unwind_on_malloc=0:detect_leaks=0 UBSAN_OPTIONS=print_stacktrace=1 #/* "For LLDB/GDB and to prevent very short stack traces and usually false leaks detection" */
		fi
	fi
}
SUSUWU_SETUP_BUILD_FLAGS() { #/* Usage: ... [SUSUWU_PROCESS_MINGE $?] SUSUWU_SETUP_CXX [SUSUWU_PROCESS_RELEASE_DEBUG $?] SUSUWU_SETUP_BUILD_FLAGS SUSUWU_SETUP_BINDIR "" SUSUWU_SETUP_OBJDIR "" SUSUWU_SETUP_OUTPUT "" [SUSUWU_PROCESS_CLEAN_REBUILD $?] [SUSUWU_PROCESS_INCLUDES ""] SUSUWU_BUILD_SOURCES ... */
	LD="${CXX}"
	CC="${CXX} -x c"
	LDFLAGS="${LDFLAGS}"
	CXXFLAGS="${CXXFLAGS} ${CXXFLAGS_ANALYSIS}"
	CCFLAGS="${CCFLAGS} ${CXXFLAGS}"
	C_SOURCE_PATH=$(SUSUWU_DIR_SUFFIX_SLASH "${C_SOURCE_PATH}") #/* if inherit C_SOURCE_PATH, perhaps it lacks '/' */
	CXX_SOURCE_PATH=$(SUSUWU_DIR_SUFFIX_SLASH "${CXX_SOURCE_PATH}") #/* if inherit CXX_SOURCE_PATH, perhaps it lacks '/' */
	OBJECTLIST=""
}

SUSUWU_SETUP_OBJDIR() { #/* Usage: `SUSUWU_SETUP_OBJDIR "./obj/"` */
	if [ -z ${OBJDIR} ]; then
		OBJDIR="${1}"
		SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "To redirect \`${CXX} -c ... -o \${OBJDIR}\${OBJ}.o\` (which has \`OBJDIR=\"${OBJDIR}\"\`), execute \`OBJDIR=\"./obj/\"\` (where \"./obj/\" is a directory which you choose)."
	else
		SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "\`${CXX} -c ... -o \${OBJDIR}\` inherits local \`OBJDIR=\"${OBJDIR}\"\` until you execute \`unset OBJDIR\`."
	fi
	OBJDIR=$(SUSUWU_DIR_SUFFIX_SLASH "${OBJDIR}") #/* if inherit OBJDIR, perhaps it is without last '/' */
	mkdir -p "${OBJDIR}"
}
SUSUWU_SETUP_BINDIR() { #/* Usage: `SUSUWU_SETUP_BINDIR "./bin/"` */
	if [ -z ${BINDIR} ]; then
		BINDIR="${1}"
		SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "To redirect \`${LD} ... -o \${BINDIR}${OUTPUT}\` (which has \`BINDIR=\"${BINDIR}\"\`), execute \`BINDIR=\"./bin/\"\` (where \"./bin/\" is a directory which you choose)."
	else
		SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "\`${LD} ... -o \${BINDIR}${OUTPUT}\` inherits local \`BINDIR=\"${BINDIR}\"\` until you execute \`unset BINDIR\`."
	fi
	BINDIR=$(SUSUWU_DIR_SUFFIX_SLASH "${BINDIR}") #/* if inherit BINDIR, perhaps it is without last '/' */
	mkdir -p "${BINDIR}"
}
SUSUWU_SETUP_OUTPUT() { #/* Usage: `SUSUWU_SETUP_OUTPUT "YourProgram"` */
	if [ -z "${BINDIR%/}" ] || [ -z "${OBJDIR%/}" ]; then
		SUSUWU_PRINT "${SUSUWU_SH_ERROR}" "\`[ -z \"\${BINDIR%/}\" ] || [ \"-z \${OBJDIR%/}\" ]\`: \`${0}\` must call \`SUSUWU_SETUP_OBJDIR()\` + \`SUSUWU_SETUP_BINDIR()\` before \`SUSUWU_SETUP_OUTPUT()\`."
		exit 1 #/* `if("/" == BINDIR || "" == BINDIR)`, can't use `${BINDIR}${OUTPUT}` */
	fi
	if [ -z ${CROSS_COMP} ]; then
		OUTPUT="${1}.out"
	else
		OUTPUT="${1}.exe"
	fi
	if [ -e ${BINDIR}${OUTPUT} ]; then
		BUILDNEW=false
	else
		BUILDNEW=true
	fi
}
SUSUWU_CLEAN_OUTPUT_IMPL() { #/* Usage: `SUSUWU_CLEAN_OUTPUT_IMPL "Reason to clean" "postscript" */
	SUSUWU_PRINT "${SUSUWU_SH_INFO}" "${1}; will remove intermediate objects (\`rm ${OBJDIR}*.o\`), plus remove generated executables (\`rm ${BINDIR}*.{exe,out}\`)${2}"
	rm ${OBJDIR}*.o 2>/dev/null #/* The `-f` flag is omitted in case `OBJDIR` or `BINDIR` is set to "../*/". */
	rm ${BINDIR}*.exe 2>/dev/null
	rm ${BINDIR}*.out 2>/dev/null
}
SUSUWU_CLEAN_OUTPUT() { #/* Usage: `SUSUWU_REBUILD_OUTPUT "Reason to clean" */
	SUSUWU_CLEAN_OUTPUT_IMPL "${1}" ", plus exit. [Use \`${0}${CROSS_COMP} --rebuild\` to remove plus continue.]"
	exit 0
}
SUSUWU_REBUILD_OUTPUT() { #/* Usage: `SUSUWU_REBUILD_OUTPUT "Reason to rebuild" */
	SUSUWU_CLEAN_OUTPUT_IMPL "${1}" ", plus continue. [Use \`${0}${CROSS_COMP} --clean\` to remove plus exit.]"
}
SUSUWU_PROCESS_CLEAN_REBUILD() { #/* Usage: `SUSUWU_PROCESS_CLEAN_REBUILD $?` */
	if [ "--clean" = "${1}" -o  "--clean" = "${2}" ]; then
		SUSUWU_CLEAN_OUTPUT "Was called with \`${0}${CROSS_COMP} --clean\`"
	fi
	if [ "--rebuild" = "${1}" -o  "--rebuild" = "${2}" ]; then
		SUSUWU_REBUILD_OUTPUT "Was called with \`${0}${CROSS_COMP} --rebuild\`"
	fi
}

SUSUWU_PROCESS_INCLUDES() { #/* Usage: `SUSUWU_BUILD_SOURCES ${C_SOURCE_PATH}*.h ${CXX_SOURCE_PATH}*.hxx` */
	for SOURCE in $@; do
		OBJECT="${OBJDIR}$(basename ${SOURCE} .hxx).o" #/* `basename`'s second param removes suffix */
		SRCCXX="$(dirname ${SOURCE})/$(basename ${SOURCE} .hxx).cxx" #/* `basename`'s second param removes suffix */
		if [ ${OBJECT} -ot ${SOURCE} -a -e ${SOURCE} ]; then #/* `&& [ -e ${SOURCE} ]` is: skip unless `${SOURCE}` exists. */
			SUSUWU_REBUILD_OUTPUT "\`${SOURCE}\` (which is a common \`#include\`) is newer than \`${OBJECT}\`"
			break
		elif [ ! -e ${SRCCXX} ]; then #/* If `*.hxx` doesn't have `*.cxx` match,     */
			touch ${OBJECT};            #/* ... then produce `*.o` (for future tests.) */
		fi
	done
}
SUSUWU_BUILD_OBJECTS() { #/* Usage: `SUSUWU_BUILD_SOURCES [${CC} || ${CXX}] [${CCFLAGS} || ${CXXFLAGS}] ".cxx" ${CXX_SOURCE_PATH}*.cxx [ optionalExtraPath/*.cxx ] [ ... ]*/
	OLD_ARG_1=${1}
	OLD_ARG_2=${2}
	shift 2 #/* `${@:3}` requires `/bin/bash`. `shift X` sets `$@` to `${X+1} ... ${N-1}`. */
	set -x
	for SOURCE in $@; do
		OBJECT="${OBJDIR}$(basename "${SOURCE}" "${OLD_ARG_2}").o" #/* `basename`'s second param removes suffix */
		if [ ${OBJECT} -ot ${SOURCE} -o ! -s ${OBJECT} ] > /dev/null 2>&1; then
			${OLD_ARG_1} -c "${SOURCE}" -o "${OBJECT}"
			BUILDNEW=true
		fi
		OBJECTLIST="${OBJECTLIST} ${OBJECT}"
	done
}
SUSUWU_BUILD_EXECUTABLE() { #/* Usage: ... [SUSUWU_PROCESS_MINGE $?] SUSUWU_SETUP_CXX [SUSUWU_PROCESS_RELEASE_DEBUG $?] SUSUWU_SETUP_BUILD_FLAGS SUSUWU_SETUP_BINDIR "" SUSUWU_SETUP_OBJDIR "" SUSUWU_SETUP_OUTPUT "" [SUSUWU_PROCESS_CLEAN_REBUILD $?] [SUSUWU_PROCESS_INCLUDES ""] SUSUWU_BUILD_OBJECTS() SUSUWU_BUILD_EXECUTABLE() ... */
	${BUILDNEW} && ${LD} ${LDFLAGS}${OBJECTLIST} -o "${BINDIR}${OUTPUT}"
	STATUS=$?
	set +x
	if [ false = ${BUILDNEW} ]; then
		SUSUWU_PRINT "${SUSUWU_SH_SUCCESS}" "reused \`${BINDIR}${OUTPUT}\` (`stat -c%s ${BINDIR}${OUTPUT}` bytes)."
		return 0
	elif [ 0 -eq ${STATUS} ]; then
		SUSUWU_PRINT "${SUSUWU_SH_SUCCESS}" "produced \`${BINDIR}${OUTPUT}\` (`stat -c%s ${BINDIR}${OUTPUT}` bytes)."
	else
		SUSUWU_PRINT "${SUSUWU_SH_ERROR}" "\`${LD}\` returned status code ${STATUS}. [If errors include \"ld... unknown file type\" or \"ld... undefined symbol __asan_*\", use \`${0}${CROSS_COMP} --rebuild\` to remove plus continue.]"
	fi
	return ${STATUS}
}

SUSUWU_TEST_OUTPUT() {
	if [ 0 -eq ${STATUS} ] || [ false = ${BUILDNEW} ]; then
		if [ -z ${CROSS_COMP} ]; then #/* `if("" == CROSS_COMP)` */
			${BINDIR}${OUTPUT}
		else #/* if `--mingw` */
			if command -v wine > /dev/null; then
				wine ${BINDIR}${OUTPUT}
			else
				SUSUWU_PRINT "${SUSUWU_SH_INFO}" "\`wine not found\`. do \`apt install wine\`."
				return 0
			fi
		fi
		STATUS=$?
		if [ 0 -eq ${STATUS} ]; then
			SUSUWU_PRINT "${SUSUWU_SH_SUCCESS}" "\`${BINDIR}${OUTPUT}\` returned status SusuwuUnitTestsBitmask(${STATUS})."
		else
			SUSUWU_PRINT "${SUSUWU_SH_ERROR}" "\`${BINDIR}${OUTPUT}\` returned status SusuwuUnitTestsBitmask(${STATUS})."
		fi
	fi
	return ${STATUS}
}

SUSUWU_TODO_LIST() { #/* Usage: `SUSUWU_TODO_LIST "NameOfTool" "what to expect from future versions"` */
	SUSUWU_PRINT "${SUSUWU_SH_ERROR}" "\`${1}\` ${2} is on a long \"TODO\" list. If you want this: contribute (at https://github.com/SwuduSusuwu/SubStack/#Contributor-conventionsrules), or ask for this at https://github.com/SwuduSusuwu/SubStack/issues/new"
	exit 1
}
SUSUWU_FORMAT() { #/* Is analogous to `make format`. */
	SUSUWU_TODO_LIST "SUSUWU_FORMAT()" "(which will use tools such as \`prettier\` or \`clang-format\`)"
}
SUSUWU_DOCS() { #/* Is analogous to `make docs`. */
	SUSUWU_TODO_LIST "SUSUWU_DOCS()" "(which will use tools such as \`Sphinx\` or \`Doxygen\`)"
}
SUSUWU_INSTALL() { #/* Is analogous to `make install`. */
	SUSUWU_TODO_LIST "SUSUWU_INSTALL()" "(which will use scripts such as \`cp ${BINDIR}${OUTPUT} /usr/bin/\`)"
}
SUSUWU_UNINSTALL() { #/* Is analogous to `make uninstall`. */
	SUSUWU_TODO_LIST "SUSUWU_UNINSTALL()" "(which will use scripts such as \`rm /usr/bin/${OUTPUT}\`)"
}
