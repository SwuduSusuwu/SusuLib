#!/bin/sh
#
#/* (C) 2024 Swudu Susuwu, dual licenses: choose [_GPLv2_](./LICENSE_GPLv2) or [_Apache 2_](./LICENSE) (allows all uses). */
#/* TODO: [produce alias (such as {`--silent`, `--quiet`} -> `-s`) groups of options/flags, for `SUSUWU_PROCESS_*` functions.](https://github.com/SwuduSusuwu/SubStack/issues/23) */
#/* TODO: [map options/flags (which `SUSUWU_PROCESS_*` functions use) to descriptions (for `--help` output.)](https://github.com/SwuduSusuwu/SubStack/issues/24) */
[ -e "./sh/Macros.sh" ] || echo "[Error: \`./sh/$(basename "$0")\` was not executed from this repo's root.]"
. ./sh/Macros.sh #/* SUSUWU_PATH_SUFFIX_SLASH() SUSUWU_PRINT() SUSUWU_S SUSUWU_SH_CONSOLE_PARAMS SUSUWU_SH_HAS_PARAM() SUSUWU_SH_REMOVE_PARAM() SUSUWU_SH_<color> SUSUWU_SH_<warn-level> SUSUWU_SH_USE2() SUSUWU_VERBOSE */

SUSUWU_PROCESS_MINGW() { #/* Usage: `SUSUWU_PROCESS_MINGW $@` [This processes params passed to `${0}`.] */
	CROSS_COMP=""
	if SUSUWU_SH_HAS_PARAM "--mingw" "$@"; then
		CROSS_COMP=" --mingw"
	fi
}
SUSUWU_SETUP_CXX() { #/* Usage: ... [SUSUWU_PROCESS_MINGW $@] SUSUWU_SETUP_CXX [SUSUWU_PROCESS_RELEASE_DEBUG $@] SUSUWU_SETUP_BUILD_FLAGS SUSUWU_SETUP_BINDIR "" SUSUWU_SETUP_OBJDIR "" SUSUWU_SETUP_OUTPUT "" [SUSUWU_PROCESS_CLEAN_REBUILD $@] [SUSUWU_PROCESS_INCLUDES ""] SUSUWU_BUILD_OBJECTS ... */
	if [ " --mingw" = "${CROSS_COMP}" ]; then
		if command -v x86_64-w64-mingw32-clang++ >/dev/null; then
			CXX="x86_64-w64-mingw32-clang++"
#			CXXFLAGS="${CXXFLAGS} -rtlib=compiler-rt"
			LDFLAGS="${LDFLAGS} -rtlib=compiler-rt -lunwind -static" #-no-lgcc -nolibgcc -lmingw32 -static-libstdc++" #workaround for [`lld: error: unable to find library -lgcc`](https://github.com/termux/termux-packages/issues/24194#issuecomment-2799574245)
			USE_FSAN=true #/* `-fsan*` [supports `x86_64-w64-mingw32-clang++`](https://github.com/SwuduSusuwu/SubStack/issues/16) */
		elif command -v x86_64-w64-mingw32-g++ >/dev/null; then
			CXX="x86_64-w64-mingw32-g++"
			LDFLAGS="${LDFLAGS} -static-libgcc -static-libstdc++"
			USE_FSAN=false #/* `TODO: `-fsan*` for `x86_64-w64-mingw32-g++`](https://www.mingw-w64.org/contribute/#thorough-status-report-for-sanitizers-asan-tsan-usan) */
		else
			SUSUWU_PRINT "SUSUWU_SETUP_CXX()" "$(SUSUWU_SH_ERROR)" "\`x86_64-w64-mingw32-clang++ not found\`, \`x86_64-w64-mingw32-g++ not found\`. Do \`apt install llvm-mingw-w64\` or \`apt install mingw-w64\`."
			exit 1
		fi
	elif command -v clang++ >/dev/null; then
		CXX="clang++" #/* TODO: +` -Xclang -analyze -Xclang -analyzer-output=text` (got no extra outputs from this) */
		USE_FSAN=true #/* [`-fsan*` supports `g++`/`clang++`](https://developers.redhat.com/blog/2021/05/05/memory-error-checking-in-c-and-c-comparing-sanitizers-and-valgrind#tldr) */
	elif command -v g++ >/dev/null; then
		CXX="g++"
		USE_FSAN=true
	elif command -v "${CXX}" >/dev/null; then #/* TODO: if our flags are compatible with all `${CXX}`, move this to top */
		SUSUWU_PRINT "SUSUWU_SETUP_CXX()" "$(SUSUWU_SH_INFO)" "\`clang++ not found\`, \`g++ not found\`. \`\${CXX}\` (\"${CXX}\") found, will use this."
		if command -v "${CC}" >/dev/null; then
			SUSUWU_PRINT "SUSUWU_SETUP_CXX()" "$(SUSUWU_SH_INFO)" "\`\${CC}\` (\"${CC}\") found, will use this."
		else
			CC="${CXX}"
			CFLAGS="${CFLAGS} -x c"
			SUSUWU_PRINT "SUSUWU_SETUP_CXX()" "$(SUSUWU_SH_INFO)" "\`\${CC}\` not found, will use \"\${CXX} -x -c\" (\"${CC}\")."
		fi
		if command -v "${LD}" >/dev/null; then
			SUSUWU_PRINT "SUSUWU_SETUP_CXX()" "$(SUSUWU_SH_INFO)" "\`\${LD}\` (\"${LD}\") found, will use this."
		else
			LD="${CXX}"
			SUSUWU_PRINT "SUSUWU_SETUP_CXX()" "$(SUSUWU_SH_INFO)" "\`\${LD}\` not found, will use \"\${CXX}\" (\"${LD}\")."
		fi
		USE_FSAN=false #/* `TODO: test unknown compilers for `-fsan*` support */
		return 0
	else
		SUSUWU_PRINT "SUSUWU_SETUP_CXX()" "$(SUSUWU_SH_ERROR)" "\`clang++ not found\`, \`g++ not found\`. \`\${CXX}\` (\"${CXX}\") not found. Do \`apt install clang\` or \`apt install gcc\`."
		exit 1
	fi
	LD="${CXX}"
	CC="${CXX}"
	CFLAGS="${CFLAGS} -x c"
	return 0
}

SUSUWU_PROCESS_RELEASE_DEBUG() { #/* Usage: `SUSUWU_PROCESS_RELEASE_DEBUG $@` [This processes params passed to `${0}`.] */
	if SUSUWU_SH_HAS_PARAM "--release" "$@"; then
		SUSUWU_PRINT "SUSUWU_PROCESS_RELEASE_DEBUG()" "$(SUSUWU_SH_NOTICE)" "\`${0} $(SUSUWU_SH_REMOVE_PARAM "--release" "$@") $(SUSUWU_SH_USE2 "${SUSUWU_SH_GREEN}" "--release")\` does not support profilers/debuggers (use \`${0} $(SUSUWU_SH_REMOVE_PARAM "--release" "$@") $(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_PURPLE}" "--debug")\` for this)."
		CFLAGS="${CFLAGS} ${FLAGS_RELEASE} ${CFLAGS_RELEASE}"
		CXXFLAGS="${CXXFLAGS} ${FLAGS_RELEASE} ${CXXFLAGS_RELEASE}"
	else
		if ! SUSUWU_SH_HAS_PARAM "--debug" "$@"; then
			SUSUWU_PRINT "SUSUWU_PROCESS_RELEASE_DEBUG()" "$(SUSUWU_SH_NOTICE)" "\`${0} $*\` defaults to \`${0} $* $(SUSUWU_SH_USE2 "${SUSUWU_SH_GREEN}" "--debug")\`."
		fi
		SUSUWU_PRINT "SUSUWU_PROCESS_RELEASE_DEBUG()" "$(SUSUWU_SH_NOTICE)" "\`${0} $(SUSUWU_SH_REMOVE_PARAM "--debug" "$@") $(SUSUWU_SH_USE2 "${SUSUWU_SH_GREEN}" "--debug")\` is slow (use \`${0} $(SUSUWU_SH_REMOVE_PARAM "--debug" "$@") $(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_PURPLE}" "--release")\` to improve how fast \`./\${BINDIR}/\${OUTPUT}\` executes)."
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
SUSUWU_SETUP_BUILD_FLAGS() { #/* Usage: ... [SUSUWU_PROCESS_MINGW $@] SUSUWU_SETUP_CXX [SUSUWU_PROCESS_RELEASE_DEBUG $@] SUSUWU_SETUP_BUILD_FLAGS SUSUWU_SETUP_BINDIR "" SUSUWU_SETUP_OBJDIR "" SUSUWU_SETUP_OUTPUT "" [SUSUWU_PROCESS_CLEAN_REBUILD $@] [SUSUWU_PROCESS_INCLUDES ""] SUSUWU_BUILD_OBJECTS ... */
	LDFLAGS="${LDFLAGS}"
	CFLAGS="${CFLAGS} ${FLAGS_USER} ${FLAGS_ANALYSIS}"
	CXXFLAGS="${CXXFLAGS} ${FLAGS_USER} ${FLAGS_ANALYSIS}"
	C_SOURCE_PATH=$(SUSUWU_PATH_SUFFIX_SLASH "${C_SOURCE_PATH}") #/* if inherit C_SOURCE_PATH, perhaps it lacks '/' */
	CXX_SOURCE_PATH=$(SUSUWU_PATH_SUFFIX_SLASH "${CXX_SOURCE_PATH}") #/* if inherit CXX_SOURCE_PATH, perhaps it lacks '/' */
	SUSUWU_OBJECTLIST=""
}

SUSUWU_SETUP_OBJDIR() { #/* Usage: `SUSUWU_SETUP_OBJDIR "./obj/"` */
	if [ -z "${OBJDIR}" ]; then
		OBJDIR="${1}"
		SUSUWU_SETUP_OBJDIR_OLD() (echo "\"$(SUSUWU_SH_USE2 "${SUSUWU_SH_GREEN}" "${OBJDIR}")\"")
		SUSUWU_SETUP_OBJDIR_NEW() (echo "\"$(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_PURPLE}" "./objnew/")\"")
		SUSUWU_PRINT "SUSUWU_SETUP_OBJDIR()" "$(SUSUWU_SH_NOTICE)" "To redirect \`${CXX} -c ... -o \"\${$(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_CYAN}" "OBJDIR")}\${OBJ}.o\"\` (which has \`$(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_CYAN}" "OBJDIR")=$(SUSUWU_SETUP_OBJDIR_OLD)\`), execute \`$(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_CYAN}" "OBJDIR")=$(SUSUWU_SETUP_OBJDIR_NEW)\` (where $(SUSUWU_SETUP_OBJDIR_NEW) is a directory which you choose)."
	else
		SUSUWU_PRINT "SUSUWU_SETUP_OBJDIR()" "$(SUSUWU_SH_NOTICE)" "\`${CXX} -c ... -o \"\${$(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_CYAN}" "OBJDIR")}\"\` inherits local \`$(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_CYAN}" "OBJDIR")=\"$(SUSUWU_SH_USE2 "${SUSUWU_SH_GREEN}" "${OBJDIR}")\"\` until you execute \`unset OBJDIR\`."
	fi
	OBJDIR="$(SUSUWU_PATH_SUFFIX_SLASH "${OBJDIR}")" #/* if inherit OBJDIR, perhaps it is without last '/' */
	mkdir -p "${OBJDIR}"
}
SUSUWU_SETUP_BINDIR() { #/* Usage: `SUSUWU_SETUP_BINDIR "./bin/"` */
	if [ -z "${BINDIR}" ]; then
		BINDIR="${1}"
		SUSUWU_SETUP_BINDIR_OLD() (echo "\"$(SUSUWU_SH_USE2 "${SUSUWU_SH_GREEN}" "${BINDIR}")\"")
		SUSUWU_SETUP_BINDIR_NEW() (echo "\"$(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_PURPLE}" "./binnew/")\"")
		SUSUWU_PRINT "SUSUWU_SETUP_BINDIR()" "$(SUSUWU_SH_NOTICE)" "To redirect \`${LD} ... -o \"\${$(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_CYAN}" "BINDIR")}\${OUTPUT}\"\` (which has \`$(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_CYAN}" "BINDIR")=$(SUSUWU_SETUP_BINDIR_OLD)\`), execute \`$(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_CYAN}" "BINDIR")=$(SUSUWU_SETUP_BINDIR_NEW)\` (where $(SUSUWU_SETUP_BINDIR_NEW) is a directory which you choose)."
	else
		SUSUWU_PRINT "SUSUWU_SETUP_BINDIR()" "$(SUSUWU_SH_NOTICE)" "\`${LD} ... -o \"\${$(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_CYAN}" "BINDIR")}\${OUTPUT}\"\` inherits local \`$(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_CYAN}" "BINDIR")=\"$(SUSUWU_SH_USE2 "${SUSUWU_SH_GREEN}" "${BINDIR}")\"\` until you execute \`unset BINDIR\`."
	fi
	BINDIR="$(SUSUWU_PATH_SUFFIX_SLASH "${BINDIR}")" #/* if inherit BINDIR, perhaps it is without last '/' */
	mkdir -p "${BINDIR}"
}
SUSUWU_SETUP_OUTPUT() { #/* Usage: `SUSUWU_SETUP_OUTPUT "YourProgram"` */
	if [ -z "${BINDIR%/}" ] || [ -z "${OBJDIR%/}" ]; then
		SUSUWU_PRINT "SUSUWU_SETUP_OUTPUT()" "$(SUSUWU_SH_ERROR)" "\`[ -z \"\${BINDIR%/}\" ] || [ \"-z \${OBJDIR%/}\" ]\`: \`${0}\` must call \`SUSUWU_SETUP_OBJDIR()\` + \`SUSUWU_SETUP_BINDIR()\` before \`SUSUWU_SETUP_OUTPUT()\`."
		exit 1 #/* `if("/" == BINDIR || "" == BINDIR)`, can't use `${BINDIR}${OUTPUT}` */
	fi
	if [ -z "${CROSS_COMP}" ]; then
		OUTPUT="${1}.out"
	else
		OUTPUT="${1}.exe"
	fi
	if [ -e "${BINDIR}${OUTPUT}" ]; then
		BUILDNEW=false
	else
		BUILDNEW=true
	fi
}
SUSUWU_CLEAN_OUTPUT_IMPL() ( #/* Usage: `SUSUWU_CLEAN_OUTPUT_IMPL "Reason to clean" "postscript" */
	SUSUWU_PRINT "SUSUWU_CLEAN_OUTPUT_IMPL()" "$(SUSUWU_SH_INFO)" "${1}; will remove intermediate objects (\`rm \"${OBJDIR}\"*.o\`), plus remove generated executables (\`rm \"${BINDIR}\"*.{exe,out}\`)${2}"
	rm "${OBJDIR}"*.o 2>/dev/null #/* The `-f` flag is omitted in case `OBJDIR` or `BINDIR` is set to "../*/". */
	rm "${BINDIR}"*.exe 2>/dev/null
	rm "${BINDIR}"*.out 2>/dev/null
)
SUSUWU_CLEAN_OUTPUT() { #/* Usage: `SUSUWU_REBUILD_OUTPUT "Reason to clean" */
	SUSUWU_CLEAN_OUTPUT_IMPL "${1}" ", plus exit. [Use \`${0} $(SUSUWU_SH_REMOVE_PARAM "--clean") $(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_PURPLE}" "--rebuild")\` to remove plus continue.]"
	exit 0
}
SUSUWU_REBUILD_OUTPUT() ( #/* Usage: `SUSUWU_REBUILD_OUTPUT "Reason to rebuild" */
	SUSUWU_CLEAN_OUTPUT_IMPL "${1}" ", plus continue. [Use \`${0} $(SUSUWU_SH_REMOVE_PARAM "--rebuild") $(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_PURPLE}" "--clean")\` to remove plus exit.]"
)
SUSUWU_PROCESS_CLEAN_REBUILD() { #/* Usage: `SUSUWU_PROCESS_CLEAN_REBUILD $@` [This processes params passed to `${0}`.] */
	if SUSUWU_SH_HAS_PARAM "--clean" "$@"; then
		SUSUWU_CLEAN_OUTPUT "Was called as \`${0} $(SUSUWU_SH_REMOVE_PARAM "--clean" "$@") $(SUSUWU_SH_USE2 "${SUSUWU_SH_GREEN}" "--clean")\`"
	fi
	if SUSUWU_SH_HAS_PARAM "--rebuild" "$@"; then
		SUSUWU_REBUILD_OUTPUT "Was called as \`${0} $(SUSUWU_SH_REMOVE_PARAM "--rebuild" "$@") $(SUSUWU_SH_USE2 "${SUSUWU_SH_GREEN}" "--rebuild")\`"
	fi
}

SUSUWU_PROCESS_INCLUDES() ( #/* Usage: `SUSUWU_PROCESS_INCLUDES ${C_SOURCE_PATH}*.h ${CXX_SOURCE_PATH}*.hxx` */
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
			SUSUWU_PRINT "SUSUWU_BUILD_CTAGS()" "$(SUSUWU_SH_INFO)" "Was called with less than 2 params; will default to \`SUSUWU_BUILD_CTAGS ${CTAGS_DEFAULTS}\`."
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
		SUSUWU_PRINT "SUSUWU_BUILD_CTAGS()" "$(SUSUWU_SH_ERROR)" "\"ctags not found\"; do \`apt install ctags\`."
	fi
	return ${SUSUWU_STATUS};
)
SUSUWU_BUILD_OBJECTS() { #/* Usage: `SUSUWU_BUILD_OBJECTS "[${CC} || ${CXX}]" "[${CFLAGS} || ${CXXFLAGS}]" ".cxx" ${CXX_SOURCE_PATH}*.cxx [ optionalExtraPath/*.cxx ] [ ... ]*/
	LOCAL_BUILD=${1}
	LOCAL_BUILDFLAGS=${2} #`=$(echo ${2} | xargs)` strips quotes, which breaks `-DSUSUWU_DEFAULT_BRANCH="trunk"`.
	LOCAL_SOURCE_SUFFIX=${3}
	shift 3 #/* `${@:4}` requires `/bin/bash`. `shift X` sets `$@` to `${X+1} ... ${N-1}`. */
	if [ ! true = ${SUSUWU_S} ]; then
		set -x
	fi
#shellcheck disable=SC2068 #`"$@"` gives "clang++: error: no such file or directory: './cxx/*.cxx'"
	for LOCAL_SOURCE in $@; do
		LOCAL_OBJECT="${OBJDIR}$(basename "${LOCAL_SOURCE}" "${LOCAL_SOURCE_SUFFIX}").o" #/* `basename`'s second param removes suffix */
#shellcheck disable=SC2166 #With `set -x`, the `[] || []` form prints 2 commands
		if [ -n "$(find "${LOCAL_SOURCE}" -newer "${LOCAL_OBJECT}" 2>/dev/null)" -o ! -s "${LOCAL_OBJECT}" ]; then
#shellcheck disable=SC2086 #`"${LOCAL_BUILDFLAGS}"` gives "clang++: error: language not recognized"
			"${LOCAL_BUILD}" ${LOCAL_BUILDFLAGS} -c "${LOCAL_SOURCE}" -o "${LOCAL_OBJECT}"
			BUILDNEW=true
		fi
		SUSUWU_OBJECTLIST="${SUSUWU_OBJECTLIST} $(SUSUWU_ESCAPE_SPACES "${LOCAL_OBJECT}")"
	done
}
SUSUWU_BUILD_EXECUTABLE() { #/* Usage: ... [SUSUWU_PROCESS_MINGW $@] SUSUWU_SETUP_CXX [SUSUWU_PROCESS_RELEASE_DEBUG $@] SUSUWU_SETUP_BUILD_FLAGS SUSUWU_SETUP_BINDIR "" SUSUWU_SETUP_OBJDIR "" SUSUWU_SETUP_OUTPUT "" [SUSUWU_PROCESS_CLEAN_REBUILD $@] [SUSUWU_PROCESS_INCLUDES ""] SUSUWU_BUILD_OBJECTS() SUSUWU_BUILD_EXECUTABLE() ... */
#shellcheck disable=SC2046 #Is not possible to use more quotes.
	${BUILDNEW} && "${LD}" $(echo "${LDFLAGS}${SUSUWU_OBJECTLIST}" | xargs) -o "${BINDIR}${OUTPUT}"
	SUSUWU_STATUS=$?
	if [ ! true = ${SUSUWU_S} ] && [ ! true = ${SUSUWU_VERBOSE} ]; then
		set +x
	fi
	if [ false = ${BUILDNEW} ]; then
		SUSUWU_STATUS=0
		SUSUWU_PRINT "SUSUWU_BUILD_EXECUTABLE()" "$(SUSUWU_SH_SUCCESS)" "Reused \`${BINDIR}${OUTPUT}\` ($(stat -c%s "${BINDIR}${OUTPUT}") bytes)."
	elif [ 0 -eq ${SUSUWU_STATUS} ]; then
		SUSUWU_PRINT "SUSUWU_BUILD_EXECUTABLE()" "$(SUSUWU_SH_SUCCESS)" "Produced \`${BINDIR}${OUTPUT}\` ($(stat -c%s "${BINDIR}${OUTPUT}") bytes)."
	else
		SUSUWU_PRINT "SUSUWU_BUILD_EXECUTABLE()" "$(SUSUWU_SH_ERROR)" "\`${LD}\` returned status code $(SUSUWU_SH_USE2 "${SUSUWU_SH_PURPLE}" "${SUSUWU_STATUS}"). [If errors include \"ld... $(SUSUWU_SH_USE2 "${SUSUWU_SH_RED}" "unknown file type")\" or \"ld... $(SUSUWU_SH_USE2 "${SUSUWU_SH_RED}" "undefined symbol __asan_")*\"; remove intermediate objects (\`rm \"${OBJDIR}\"*.o\`). Use \`${0} ${SUSUWU_SH_CONSOLE_PARAMS} $(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_PURPLE}" "--rebuild")\` to remove plus continue."
	fi
	return ${SUSUWU_STATUS}
}

SUSUWU_TEST_OUTPUT() { #/* Usage: ... `SUSUWU_BUILD_EXECUTABLE && SUSUWU_TEST_OUTPUT [<silent-execution>]; exit $?` */
	if [ 0 -eq ${SUSUWU_STATUS} ] || [ false = ${BUILDNEW} ]; then
		if [ -z "${CROSS_COMP}" ]; then #/* `if("" == CROSS_COMP)` */
			if [ true = ${SUSUWU_S} ]; then
				"${BINDIR}${OUTPUT}" 2>/dev/null 1>&2
			else
				"${BINDIR}${OUTPUT}"
			fi
		else #/* if `--mingw` */
			if ! command -v wine >/dev/null; then
				SUSUWU_PRINT "SUSUWU_TEST_OUTPUT()" "$(SUSUWU_SH_INFO)" "\`wine not found\`. do \`apt install wine\`."
				return 1
			fi
			if [ true = ${SUSUWU_S} ]; then
				wine "${BINDIR}${OUTPUT}" 2>/dev/null 1>&2
			else
				wine "${BINDIR}${OUTPUT}"
			fi
		fi
		SUSUWU_STATUS=$?
		if [ 0 -eq ${SUSUWU_STATUS} ]; then
			SUSUWU_PRINT "SUSUWU_TEST_OUTPUT()" "$(SUSUWU_SH_SUCCESS)" "\`${BINDIR}${OUTPUT}\` returned status SusuwuUnitTestsBitmask($(SUSUWU_SH_USE2 "${SUSUWU_SH_PURPLE}" "${SUSUWU_STATUS}"))."
		else
			SUSUWU_PRINT "SUSUWU_TEST_OUTPUT()" "$(SUSUWU_SH_ERROR)" "\`${BINDIR}${OUTPUT}\` returned status SusuwuUnitTestsBitmask($(SUSUWU_SH_USE2 "${SUSUWU_SH_PURPLE}" "${SUSUWU_STATUS}"))."
		fi
	fi
	return ${SUSUWU_STATUS}
}

SUSUWU_TODO_LIST() ( #/* Usage: `SUSUWU_TODO_LIST "NameOfTool" "what to expect from future versions"` */
	SUSUWU_PRINT "SUSUWU_TODO_LIST()" "$(SUSUWU_SH_ERROR)" "\`${1}\` ${2} is on a long \"TODO\" list. If you want this soon; you can [contribute](https://github.com/SwuduSusuwu/SubStack/#Contributor-conventionsrules), or [ask for this](https://github.com/SwuduSusuwu/SubStack/issues/new)."
	exit 1
)
SUSUWU_FORMAT() ( #/* Is analogous to `make format`. */
	SUSUWU_TODO_LIST "SUSUWU_FORMAT()" "(which will use tools such as \`prettier\` or \`clang-format\`)"
)
SUSUWU_DOCS() ( #/* Is analogous to `make docs`. */
	SUSUWU_TODO_LIST "SUSUWU_DOCS()" "(which will use tools such as \`sphinx\` or \`doxygen\`)"
)
SUSUWU_HAS_USABLE_USRBIN() ( #/* Usage: `[ 0 -eq $(SUSUWU_HAS_USABLE_USRBIN("${USRBIN}")) ]`. Is just for internal use. */
	if [ ! -d "${1}" ]; then
		return 1
	fi
	if [ -z "${PATH}" ]; then
		SUSUWU_PRINT "SUSUWU_HAS_USABLE_USRBIN()" "$(SUSUWU_SH_WARNING)" "\`\${PATH}\` not set; will use \`$(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_CYAN}" "USRBIN")=\"$(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_PURPLE}" "${1}")\"\` (first guessed path which exists)."
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
SUSUWU_FIRST_PATH() ( #/* Usage: `USRBIN="$(SUSUWU_FIRST_PATH)"`. Is just for internal use. */
	IFS=":" #`PATH` format is `"PATH_0:PATH_1:PATH_N-1"`.
	for PATH_W in ${PATH}; do #With `in "${PATH|"`, this will not loop
		if [ -d "${PATH_W}" ]; then
			echo "${PATH_W}"
			return 0
		fi
	done
	SUSUWU_PRINT "SUSUWU_FIRST_PATH()" "$(SUSUWU_SH_ERROR)" "\`\${PATH}\` not set."
	return 1
)
SUSUWU_PROCESS_USRBIN() { #/* Usage: `SUSUWU_USRBIN ["SUSUWU_[UN]INSTALL"] ["/usr/bin"...]`. Is just for internal use. */
	if [ -n "${2}" ]; then #/* function `${1}` was passed USRBIN as its first param (our second param) */
		USRBIN="${2}"
		if [ -d "${USRBIN}" ]; then
			return 0
		else
			SUSUWU_PRINT "SUSUWU_PROCESS_USRBIN()" "$(SUSUWU_SH_WARNING)" "\`${1}\` was passed \`$(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_CYAN}" "USRBIN")=\"$(SUSUWU_SH_USE2 "${SUSUWU_SH_GREEN}" "${USRBIN}")\"\` (which is not a directory); will autodetect \`$(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_CYAN}" "USRBIN")\`."
		fi
	fi
	for USRBIN in "$(realpath -q ~/.local/bin)" "$(realpath -q ~/../usr/bin)" "$(realpath -q ~/../usr/local/bin)" "$(realpath -q ~/../local/bin)" "$(dirname "$(which sh)")" "/usr/local/bin" "/usr/bin" "/bin"; do
		if SUSUWU_HAS_USABLE_USRBIN "${USRBIN}"; then
			break
		fi
		USRBIN=""
	done
	if [ -z "${USRBIN}" ]; then #if array above is "", or does not have usable values
		SUSUWU_PRINT "SUSUWU_PROCESS_USRBIN()" "$(SUSUWU_SH_NOTICE)" "None of the usual paths for \`$(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_CYAN}" "USRBIN")\` suitable to use; will use the first directory from \`\${PATH}\` which exists."
		USRBIN="$(SUSUWU_FIRST_PATH)"
	fi
	if [ -d "${USRBIN}" ]; then
		SUSUWU_PRINT "SUSUWU_PROCESS_USRBIN()" "$(SUSUWU_SH_NOTICE)" "Have set \`$(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_CYAN}" "USRBIN")=\"$(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_PURPLE}" "${USRBIN}")\"\`, since \`${1}\` was not passed \`$(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_CYAN}" "USRBIN")\`."
		return 0
	fi
	SUSUWU_PRINT "SUSUWU_PROCESS_USRBIN()" "$(SUSUWU_SH_ERROR)" "Failed to autodetect \`$(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_CYAN}" "USRBIN")\` (which \`${1}\` uses)."
	return 1 #/* Notice: `exit 1` would cause GitHub rubbers (with unknown install paths) to fail unit tests. */
}
SUSUWU_INSTALL() ( #/* Usage: `SUSUWU_INSTALL [USRBIN]`. Is analogous to `make install`. */
	SUSUWU_PROCESS_USRBIN "SUSUWU_INSTALL()" "${1}" &&
	cp --interactive "${BINDIR}${OUTPUT}" "${USRBIN}/${OUTPUT}"
) #/* returns result of `cp`; thus `SUSUWU_INSTALL && SUSUWU_UNINSTALL` won't remove files unless the interactive `SUSUWU_INSTALL` has total success */
SUSUWU_UNINSTALL() ( #/* Usage: `SUSUWU_UNINSTALL [USRBIN]`. Is analogous to `make uninstall`. */
	SUSUWU_PROCESS_USRBIN "SUSUWU_UNINSTALL()" "${1}" &&
	rm "${USRBIN}/${OUTPUT}"
)

