#!/bin/sh
#
#/* (C) 2024 Swudu Susuwu, dual licenses: choose [_GPLv2_](./LICENSE_GPLv2) or [_Apache 2_](./LICENSE) (allows all uses). */
#/* TODO: [produce `for OPTION in @$; do` loops for all `SUSUWU_PROCESS_*` functions.](https://github.com/SwuduSusuwu/SubStack/issues/22) */
#/* TODO: [produce alias (such as {`--silent`, `--quiet`} -> `-s`) groups of options/flags, for `SUSUWU_PROCESS_*` functions.](https://github.com/SwuduSusuwu/SubStack/issues/23) */
#/* TODO: [map options/flags (which `SUSUWU_PROCESS_*` functions use) to descriptions (for `--help` output.)](https://github.com/SwuduSusuwu/SubStack/issues/24) */
[ -e "./sh/Macros.sh" ] || echo "[Error: \`./sh/$(basename "$0")\` was not executed from this repo's root.]"
. ./sh/Macros.sh #/* SUSUWU_PRINT() SUSUWU_S SUSUWU_SH_* SUSUWU_VERBOSE */

SUSUWU_PROCESS_MINGW() { #/* Usage: `SUSUWU_PROCESS_MINGW $@` [This processes params passed to `${0}`.] */
	CROSS_COMP=""
	if [ "--mingw" = "${1}" ] || [ "--mingw" = "${2}" ]; then
		CROSS_COMP=" --mingw"
	fi
}
SUSUWU_SETUP_CXX() { #/* Usage: ... [SUSUWU_PROCESS_MINGW $@] SUSUWU_SETUP_CXX [SUSUWU_PROCESS_RELEASE_DEBUG $@] SUSUWU_SETUP_BUILD_FLAGS SUSUWU_SETUP_BINDIR "" SUSUWU_SETUP_OBJDIR "" SUSUWU_SETUP_OUTPUT "" [SUSUWU_PROCESS_CLEAN_REBUILD $@] [SUSUWU_PROCESS_INCLUDES ""] SUSUWU_BUILD_SOURCES ... */
	if [ " --mingw" = "${CROSS_COMP}" ]; then
		LDFLAGS="${LDFLAGS} -static-libgcc -static-libstdc++"
		if command -v x86_64-w64-mingw32-clang++ >/dev/null; then
			CXX="x86_64-w64-mingw32-clang++"
			USE_FSAN=true #/* `-fsan*` [supports `x86_64-w64-mingw32-clang++`](https://github.com/SwuduSusuwu/SubStack/issues/16) */
		elif command -v x86_64-w64-mingw32-g++ >/dev/null; then
			CXX="x86_64-w64-mingw32-g++"
			USE_FSAN=false #/* `TODO: `-fsan*` for `x86_64-w64-mingw32-g++`](https://www.mingw-w64.org/contribute/#thorough-status-report-for-sanitizers-asan-tsan-usan) */
		else
			SUSUWU_PRINT "${SUSUWU_SH_ERROR}" "\`x86_64-w64-mingw32-clang++ not found\`, \`x86_64-w64-mingw32-g++ not found\`. Do \`apt install llvm-mingw-w64\` or \`apt install mingw-w64\`."
			exit 1
		fi
	elif command -v clang++ >/dev/null; then
		CXX="clang++" #/* TODO: +` -Xclang -analyze -Xclang -analyzer-output=text` (got no extra outputs from this) */
		USE_FSAN=true #/* [`-fsan*` supports `g++`/`clang++`](https://developers.redhat.com/blog/2021/05/05/memory-error-checking-in-c-and-c-comparing-sanitizers-and-valgrind#tldr) */
	elif command -v g++ >/dev/null; then
		CXX="g++"
		USE_FSAN=true
	elif command -v "${CXX}" >/dev/null; then #/* TODO: if our flags are compatible with all `${CXX}`, move this to top */
		SUSUWU_PRINT "${SUSUWU_SH_INFO}" "\`clang++ not found\`, \`g++ not found\`. \`\${CXX}\` (\"${CXX}\") found, will use this."
		if command -v "${CC}" >/dev/null; then
			SUSUWU_PRINT "${SUSUWU_SH_INFO}" "\`\${CC}\` (\"${CC}\") found, will use this."
		else
			CC="${CXX} -x c"
			SUSUWU_PRINT "${SUSUWU_SH_INFO}" "\`\${CC}\` not found, will use \"\${CXX} -x -c\" (\"${CC}\")."
		fi
		if command -v "${LD}" >/dev/null; then
			SUSUWU_PRINT "${SUSUWU_SH_INFO}" "\`\${LD}\` (\"${LD}\") found, will use this."
		else
			LD="${CXX}"
			SUSUWU_PRINT "${SUSUWU_SH_INFO}" "\`\${LD}\` not found, will use \"\${CXX}\" (\"${LD}\")."
		fi
		USE_FSAN=false #/* `TODO: test unknown compilers for `-fsan*` support */
		return 0
	else
		SUSUWU_PRINT "${SUSUWU_SH_ERROR}" "\`clang++ not found\`, \`g++ not found\`. \`\${CXX}\` (\"${CXX}\") not found. Do \`apt install clang\` or \`apt install gcc\`."
		exit 1
	fi
	LD="${CXX}"
	CC="${CXX} -x c"
	return 0
}

SUSUWU_PROCESS_RELEASE_DEBUG() { #/* Usage: `SUSUWU_PROCESS_RELEASE_DEBUG $@` [This processes params passed to `${0}`.] */
	if [ "--release" = "${1}" ] || [ "--release" = "${2}" ]; then
		SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "\`${0}${CROSS_COMP} --release\` does not support profilers/debuggers (use \`${0}${CROSS_COMP} --debug\` for this)."
		CFLAGS="${CFLAGS} ${FLAGS_RELEASE} ${CFLAGS_RELEASE}"
		CXXFLAGS="${CXXFLAGS} ${FLAGS_RELEASE} ${CXXFLAGS_RELEASE}"
	else
		if [ "--debug" != "${1}" ] && [ "--debug" != "${2}" ]; then
			SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "\`${0}${CROSS_COMP}\` defaults to \`${0}${CROSS_COMP} --debug\`."
		fi
		SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "\`${0}${CROSS_COMP} --debug\` is slow (use \`${0}${CROSS_COMP} --release\` to improve how fast \"\${BINDIR}/\${OUTPUT}\" executes)."
		CFLAGS="${CFLAGS} ${FLAGS_DEBUG} ${CFLAGS_DEBUG}"
		CXXFLAGS="${CXXFLAGS} ${FLAGS_DEBUG} ${CXXFLAGS_DEBUG}"
		if [ true = ${USE_FSAN} ]; then
			CFLAGS="${CFLAGS} ${FLAGS_FSAN}"
			CXXFLAGS="${CXXFLAGS} ${FLAGS_FSAN}"
			LDFLAGS="${LDFLAGS} ${FLAGS_FSAN}"
			export ASAN_OPTIONS=abort_on_error=1:fast_unwind_on_malloc=0:detect_leaks=0 UBSAN_OPTIONS=print_stacktrace=1 #/* "For LLDB/GDB and to prevent very short stack traces and usually false leaks detection" */
		fi
	fi
}
SUSUWU_SETUP_BUILD_FLAGS() { #/* Usage: ... [SUSUWU_PROCESS_MINGW $@] SUSUWU_SETUP_CXX [SUSUWU_PROCESS_RELEASE_DEBUG $@] SUSUWU_SETUP_BUILD_FLAGS SUSUWU_SETUP_BINDIR "" SUSUWU_SETUP_OBJDIR "" SUSUWU_SETUP_OUTPUT "" [SUSUWU_PROCESS_CLEAN_REBUILD $@] [SUSUWU_PROCESS_INCLUDES ""] SUSUWU_BUILD_SOURCES ... */
	LDFLAGS="${LDFLAGS}"
	CFLAGS="${CFLAGS} ${FLAGS_USER} ${FLAGS_ANALYSIS}"
	CXXFLAGS="${CXXFLAGS} ${FLAGS_USER} ${FLAGS_ANALYSIS}"
	C_SOURCE_PATH=$(SUSUWU_DIR_SUFFIX_SLASH "${C_SOURCE_PATH}") #/* if inherit C_SOURCE_PATH, perhaps it lacks '/' */
	CXX_SOURCE_PATH=$(SUSUWU_DIR_SUFFIX_SLASH "${CXX_SOURCE_PATH}") #/* if inherit CXX_SOURCE_PATH, perhaps it lacks '/' */
	SUSUWU_OBJECTLIST=""
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
	if [ -z "${CROSS_COMP}" ]; then
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
SUSUWU_CLEAN_OUTPUT_IMPL() ( #/* Usage: `SUSUWU_CLEAN_OUTPUT_IMPL "Reason to clean" "postscript" */
	SUSUWU_PRINT "${SUSUWU_SH_INFO}" "${1}; will remove intermediate objects (\`rm \"${OBJDIR}\"*.o\`), plus remove generated executables (\`rm \"${BINDIR}\"*.{exe,out}\`)${2}"
	rm "${OBJDIR}"*.o 2>/dev/null #/* The `-f` flag is omitted in case `OBJDIR` or `BINDIR` is set to "../*/". */
	rm "${BINDIR}"*.exe 2>/dev/null
	rm "${BINDIR}"*.out 2>/dev/null
)
SUSUWU_CLEAN_OUTPUT() { #/* Usage: `SUSUWU_REBUILD_OUTPUT "Reason to clean" */
	SUSUWU_CLEAN_OUTPUT_IMPL "${1}" ", plus exit. [Use \`${0}${CROSS_COMP} --rebuild\` to remove plus continue.]"
	exit 0
}
SUSUWU_REBUILD_OUTPUT() ( #/* Usage: `SUSUWU_REBUILD_OUTPUT "Reason to rebuild" */
	SUSUWU_CLEAN_OUTPUT_IMPL "${1}" ", plus continue. [Use \`${0}${CROSS_COMP} --clean\` to remove plus exit.]"
)
SUSUWU_PROCESS_CLEAN_REBUILD() { #/* Usage: `SUSUWU_PROCESS_CLEAN_REBUILD $@` [This processes params passed to `${0}`.] */
	if [ "--clean" = "${1}" ] || [ "--clean" = "${2}" ]; then
		SUSUWU_CLEAN_OUTPUT "Was called with \`${0}${CROSS_COMP} --clean\`"
	fi
	if [ "--rebuild" = "${1}" ] || [ "--rebuild" = "${2}" ]; then
		SUSUWU_REBUILD_OUTPUT "Was called with \`${0}${CROSS_COMP} --rebuild\`"
	fi
}

SUSUWU_PROCESS_INCLUDES() ( #/* Usage: `SUSUWU_BUILD_SOURCES ${C_SOURCE_PATH}*.h ${CXX_SOURCE_PATH}*.hxx` */
#shellcheck disable=SC2068
	for LOCAL_SOURCE in $@; do
		LOCAL_OBJECT="${OBJDIR}$(basename "${LOCAL_SOURCE}" .hxx).o" #/* `basename`'s second param removes suffix */
		LOCAL_SRCCXX="$(dirname "${LOCAL_SOURCE}")/$(basename "${LOCAL_SOURCE}" .hxx).cxx" #/* `basename`'s second param removes suffix */
		if [ -n "$(find "${LOCAL_SOURCE}" -newer "${LOCAL_OBJECT}" 2>/dev/null)" ] && [ -e "${LOCAL_SOURCE}" ]; then #/* `-n`: not nil, `-e`: exists. */
			SUSUWU_REBUILD_OUTPUT "\`${LOCAL_SOURCE}\` (which is a common \`#include\`) is newer than \`${LOCAL_OBJECT}\`"
			break
		elif [ ! -e "${LOCAL_SRCCXX}" ]; then #/* If `*.hxx` doesn't have `*.cxx` match,     */
			touch "${LOCAL_OBJECT}";            #/* ... then produce `*.o` (for future tests.) */
		fi
	done
)
SUSUWU_BUILD_CTAGS() ( #/* Usage: `SUSUWU_BUILD_CTAGS [-flags... --flags...] [SOURCE_DIR]...`. Return value: if `ctags` is called; `0`, if not; `1`. */
	SUSUWU_STATUS=1
	if command -v ctags >/dev/null; then
		if [ -z "${1}" ] || [ -z "${2}" ]; then
			CTAGS_DEFAULTS="-R --exclude=.git/ --exclude=*.html ."
			SUSUWU_PRINT "${SUSUWU_SH_INFO}" "\`SUSUWU_BUILD_CTAGS()\` was called with less than 2 params; will default to \`SUSUWU_BUILD_CTAGS ${CTAGS_DEFAULTS}\`."
#shellcheck disable=SC2086
			ctags ${CTAGS_DEFAULTS} && SUSUWU_STATUS=0
		else
			FLAGS=${1}
#shellcheck disable=SC2086
			ctags ${FLAGS} "${2}" && SUSUWU_STATUS=0
			shift 2 #/* `${@:3}` requires `/bin/bash`. `shift X` sets `$@` to `${X+1} ... ${N-1}`. */
			for SOURCE_PATH in "$@"; do
#shellcheck disable=SC2086
				ctags ${FLAGS} -a "${SOURCE_PATH}" || SUSUWU_STATUS=1
			done
		fi
	else
		SUSUWU_PRINT "${SUSUWU_SH_ERROR}" "\`SUSUWU_BUILD_CTAGS()\`: \"ctags not found\"; do \`apt install ctags\`."
	fi
	return ${SUSUWU_STATUS};
)
SUSUWU_BUILD_OBJECTS() { #/* Usage: `SUSUWU_BUILD_SOURCES "[${CC} || ${CXX}] [${CFLAGS} || ${CXXFLAGS}]" ".cxx" ${CXX_SOURCE_PATH}*.cxx [ optionalExtraPath/*.cxx ] [ ... ]*/
	LOCAL_BUILD=${1}
	LOCAL_SOURCE_SUFFIX=${2}
	shift 2 #/* `${@:3}` requires `/bin/bash`. `shift X` sets `$@` to `${X+1} ... ${N-1}`. */
	if [ ! true = ${SUSUWU_S} ]; then
		set -x
	fi
#shellcheck disable=SC2068 #With `$@`, this breaks.
	for LOCAL_SOURCE in $@; do
		LOCAL_OBJECT="${OBJDIR}$(basename "${LOCAL_SOURCE}" "${LOCAL_SOURCE_SUFFIX}").o" #/* `basename`'s second param removes suffix */
#shellcheck disable=SC2166 #With `set -x`, the `[] || {}` form prints 2 commands
		if [ -n "$(find "${LOCAL_SOURCE}" -newer "${LOCAL_OBJECT}" 2>/dev/null)" -o ! -s "${LOCAL_OBJECT}" ]; then
			${LOCAL_BUILD} -c "${LOCAL_SOURCE}" -o "${LOCAL_OBJECT}"
			BUILDNEW=true
		fi
		SUSUWU_OBJECTLIST="${SUSUWU_OBJECTLIST} $(SUSUWU_ESCAPE_SPACES "${LOCAL_OBJECT}")"
	done
}
SUSUWU_BUILD_EXECUTABLE() { #/* Usage: ... [SUSUWU_PROCESS_MINGW $@] SUSUWU_SETUP_CXX [SUSUWU_PROCESS_RELEASE_DEBUG $@] SUSUWU_SETUP_BUILD_FLAGS SUSUWU_SETUP_BINDIR "" SUSUWU_SETUP_OBJDIR "" SUSUWU_SETUP_OUTPUT "" [SUSUWU_PROCESS_CLEAN_REBUILD $@] [SUSUWU_PROCESS_INCLUDES ""] SUSUWU_BUILD_OBJECTS() SUSUWU_BUILD_EXECUTABLE() ... */
#shellcheck disable=SC2086
	${BUILDNEW} && ${LD} ${LDFLAGS}${SUSUWU_OBJECTLIST} -o "${BINDIR}${OUTPUT}"
	SUSUWU_STATUS=$?
	if [ ! true = ${SUSUWU_S} ] && [ ! true = ${SUSUWU_VERBOSE} ]; then
		set +x
	fi
	if [ false = ${BUILDNEW} ]; then
		SUSUWU_STATUS=0
		SUSUWU_PRINT "${SUSUWU_SH_SUCCESS}" "reused \`${BINDIR}${OUTPUT}\` ($(stat -c%s "${BINDIR}${OUTPUT}") bytes)."
	elif [ 0 -eq ${SUSUWU_STATUS} ]; then
		SUSUWU_PRINT "${SUSUWU_SH_SUCCESS}" "produced \`${BINDIR}${OUTPUT}\` ($(stat -c%s "${BINDIR}${OUTPUT}") bytes)."
	else
		SUSUWU_PRINT "${SUSUWU_SH_ERROR}" "\`${LD}\` returned status code ${SUSUWU_STATUS}. [If errors include \"ld... unknown file type\" or \"ld... undefined symbol __asan_*\"; remove intermediate objects (\`rm \"${OBJDIR}\"*.o\`). Use \`${0}${CROSS_COMP} --rebuild\` to remove plus continue.]"
	fi
	return ${SUSUWU_STATUS}
}

SUSUWU_TEST_OUTPUT() {
	if [ 0 -eq ${SUSUWU_STATUS} ] || [ false = ${BUILDNEW} ]; then
		if [ -z "${CROSS_COMP}" ]; then #/* `if("" == CROSS_COMP)` */
			${BINDIR}${OUTPUT}
		else #/* if `--mingw` */
			if command -v wine >/dev/null; then
				wine ${BINDIR}${OUTPUT}
			else
				SUSUWU_PRINT "${SUSUWU_SH_INFO}" "\`wine not found\`. do \`apt install wine\`."
				return 1
			fi
		fi
		SUSUWU_STATUS=$?
		if [ 0 -eq ${SUSUWU_STATUS} ]; then
			SUSUWU_PRINT "${SUSUWU_SH_SUCCESS}" "\`${BINDIR}${OUTPUT}\` returned status SusuwuUnitTestsBitmask(${SUSUWU_STATUS})."
		else
			SUSUWU_PRINT "${SUSUWU_SH_ERROR}" "\`${BINDIR}${OUTPUT}\` returned status SusuwuUnitTestsBitmask(${SUSUWU_STATUS})."
		fi
	fi
	return ${SUSUWU_STATUS}
}

SUSUWU_TODO_LIST() ( #/* Usage: `SUSUWU_TODO_LIST "NameOfTool" "what to expect from future versions"` */
	SUSUWU_PRINT "${SUSUWU_SH_ERROR}" "\`${1}\` ${2} is on a long \"TODO\" list. If you want this: contribute (at https://github.com/SwuduSusuwu/SubStack/#Contributor-conventionsrules), or ask for this at https://github.com/SwuduSusuwu/SubStack/issues/new"
	exit 1
)
SUSUWU_FORMAT() ( #/* Is analogous to `make format`. */
	SUSUWU_TODO_LIST "SUSUWU_FORMAT()" "(which will use tools such as \`prettier\` or \`clang-format\`)"
)
SUSUWU_DOCS() ( #/* Is analogous to `make docs`. */
	SUSUWU_TODO_LIST "SUSUWU_DOCS()" "(which will use tools such as \`Sphinx\` or \`Doxygen\`)"
)
SUSUWU_HAS_USABLE_USRBIN() ( #/* Usage: `[ 0 -eq $(SUSUWU_HAS_USABLE_USRBIN("${USRBIN}")) ]`. Is just for internal use. */
	if [ ! -d "${1}" ]; then
		return 1
	fi
	if [ -z "${PATH}" ]; then
		SUSUWU_PRINT "${SUSUWU_SH_WARNING}" "SUSUWU_HAS_USABLE_USRBIN(): \`\${PATH}\` not set; will use \`USRBIN=\"${1}\"\` (first guessed path which exists)."
		return 0
	fi
	IFS=":" #`PATH` format is `"PATH_0:PATH_1:PATH_N-1"`.
	for PATH_W in ${PATH}; do
		if [ "${PATH_W}" = "${1}" ]; then
			return 0
		fi
	done
	return 1
)
SUSUWU_FIRST_PATH() ( #/* Usage: `USRBIN="$(SUSUWU_FIRST_PATH)`. Is just for internal use. */
	IFS=":" #`PATH` format is `"PATH_0:PATH_1:PATH_N-1"`.
	for PATH_W in ${PATH}; do
		if [ -d "${PATH_W}" ]; then
			echo "${PATH_W}"
			return 0
		fi
	done
	SUSUWU_PRINT "${SUSUWU_SH_ERROR}" "SUSUWU_FIRST_PATH(): \`\${PATH}\` not set."
	return 1
)
SUSUWU_PROCESS_USRBIN() { #/* Usage: `SUSUWU_USRBIN ["SUSUWU_[UN]INSTALL"] ["/usr/bin"...]`. Is just for internal use. */
	if [ -n "${2}" ]; then #/* function `${1}` was passed USRBIN as its first param (our second param) */
		USRBIN="${2}"
		if [ -d "${USRBIN}" ]; then
			return 0
		else
			SUSUWU_PRINT "${SUSUWU_SH_WARNING}" "SUSUWU_PROCESS_USRBIN(): \`${1}\` was passed \`USRBIN=\"${USRBIN}\"\` (which is not a directory); will autodetect \`USRBIN\`."
		fi
	fi
	for USRBIN in "$(realpath -q ~/.local/bin)" "$(realpath -q ~/../usr/bin)" "$(realpath -q ~/../usr/local/bin)" "$(realpath -q ~/../local/bin)" "$(dirname "$(which sh)")" "/usr/local/bin" "/usr/bin" "/bin"; do
		if SUSUWU_HAS_USABLE_USRBIN "${USRBIN}"; then
			break
		fi
		USRBIN=""
	done
	if [ -z "${USRBIN}" ]; then #if array above is "", or does not have usable values
		SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "SUSUWU_PROCESS_USRBIN(): none of the usual paths for \`USRBIN\` suitable to use; will use the first directory from \`\${PATH}\` which exists."
		USRBIN="$(SUSUWU_FIRST_PATH)"
	fi
	if [ -d "${USRBIN}" ]; then
		SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "SUSUWU_PROCESS_USRBIN(): have set \`USRBIN=\"${USRBIN}\"\`, since \`${1}\` was not passed \`USRBIN\`."
		return 0
	fi
	SUSUWU_PRINT "${SUSUWU_SH_ERROR}" "SUSUWU_PROCESS_USRBIN(): failed to autodetect \`USRBIN\` (which \`${1}\` uses)."
	return 1 #/* Notice: `exit 1` would cause GitHub rubbers (with unknown install paths) to fail unit tests. */
}
SUSUWU_INSTALL() ( #/* Usage: `SUSUWU_INSTALL [USRBIN]`. Is analogous to `make install`. */
	SUSUWU_PROCESS_USRBIN "SUSUWU_INSTALL()" "${1}" &&
	cp --interactive ${BINDIR}${OUTPUT} "${USRBIN}/${OUTPUT}"
) #/* returns result of `cp`; thus `SUSUWU_INSTALL && SUSUWU_UNINSTALL` won't remove files unless the interactive `SUSUWU_INSTALL` has total success */
SUSUWU_UNINSTALL() ( #/* Usage: `SUSUWU_UNINSTALL [USRBIN]`. Is analogous to `make uninstall`. */
	SUSUWU_PROCESS_USRBIN "SUSUWU_UNINSTALL()" "${1}" &&
	rm "${USRBIN}/${OUTPUT}"
)

