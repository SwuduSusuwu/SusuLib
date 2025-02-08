#!/bin/sh
#
#/* (C) 2024 Swudu Susuwu, dual licenses: choose [_GPLv2_](./LICENSE_GPLv2) or [_Apache 2_](./LICENSE) (allows all uses). */
#/* Based on ../cxx/Macros.hxx */
#/* TODO: [produce alias (such as {`--silent`, `--quiet`} -> `-s`) groups of options/flags, for `SUSUWU_PROCESS_*` functions.](https://github.com/SwuduSusuwu/SubStack/issues/23) */
#/* TODO: [map options/flags (which `SUSUWU_PROCESS_*` functions use) to descriptions (for `--help` output.)](https://github.com/SwuduSusuwu/SubStack/issues/24) */

export SUSUWU_SH_CONSOLE_PARAMS="$*" #/* For functions which are not passed `$@` */
SUSUWU_SH_HAS_PARAM() ( #/* Usage: `if SUSUWU_SH_HAS_PARAM "--param" "$@";`. [This processes params passed to `${0}`.] */
	if [ "$#" -eq 1 ]; then
		SUSUWU_SH_HAS_PARAM "${1}" "${SUSUWU_SH_CONSOLE_PARAMS}"
		return $?
	fi
	PARAM=${1}; shift;
	for PARAM_W in "$@"; do
		if [ "${PARAM}" = "${PARAM_W}" ]; then
			return 0
		fi
	done
	return 1
)
SUSUWU_SH_REMOVE_PARAM() ( #/* Usage: `echo "$(SUSUWU_SH_REMOVE_PARAM "--unwanted-param" "$@")"`. [This processes params passed to `${0}`.] */
	PARAM=${1}; shift;
	NEW_PARAMS=""
	SUSUWU_SH_REMOVE_PARAM_FOUND=1
	for PARAM_W in "$@"; do
		if [ "${PARAM}" != "${PARAM_W}" ]; then
			SUSUWU_SH_REMOVE_PARAM_FOUND=0
			if [ -z "${NEW_PARAMS}" ]; then
				NEW_PARAMS="${PARAM_W}"
			elif [ -n "${NEW_PARAMS}" ]; then
				NEW_PARAMS="${NEW_PARAMS} ${PARAM_W}" #/* TODO: spaces? */
			fi
		fi
	done
	echo "${NEW_PARAMS}"
	return ${SUSUWU_SH_REMOVE_PARAM_FOUND}
)
SUSUWU_DIR_SUFFIX_SLASH() ( #/* Usage: `OBJDIR=$(SUSUWU_ENSURE_DIR_SLASH "${OBJDIR}")` */
	DIR=${1}
	if [ "${DIR}" = "${DIR%/}" ]; then #/* "%/" removes slash; if equal after this, original doesn't have '/'. */
		DIR="${DIR}/" #/* if original doesn't have, append '/' */
	fi
	echo "${DIR}" #/* return with slash */
)
SUSUWU_DIR_AFFIX_DOTSLASH() ( #/* Usage: `BINDIR=$(SUSUWU_DIR_AFFIX_DOTSLASH "${BINDIR}")` */
	DIR=${1}
	case "${DIR}" in
		./*) #/* original has "./" */
			;;
		*) #/* default (if original doesn't match "./") */
			DIR="./${DIR}" #/* if original doesn't have, affix "./" */
			;;
	esac
	echo "${DIR}" #/* return with "./" */
)
SUSUWU_ESCAPE_SPACES() ( #/* Usage: `SUSUWU_OBJECTLIST="${SUSUWU_OBJECTLIST} $(SUSUWU_ESCAPE_SPACES "${OBJECT}"). */
#	echo $(echo "$@" | sed 's/ /\\\ /') #/* Error: `sed not found`, although is installed. */
#	echo "\"${@}\""; #/* Error: if `OBJECT="obj/main.o"`, `SUSUWU_BUILD_EXECUTABLE()` gives `clang++: error: no such file or directory: '"obj/main.o"'`. */
	NEW_PATH=""
	for OLD_PATH in "$@"; do
		if [ -z "${NEW_PATH}" ]; then
			NEW_PATH="${OLD_PATH}"
		elif [ -n "${NEW_PATH}" ]; then
			NEW_PATH="${NEW_PATH}\\ ${OLD_PATH}" #/* Error: if `OBJECT="obj/long path.o"`, `SUSUWU_BUILD_EXECUTABLE()` gives `clang++: error: no such file or directory: 'obj/long\'\nclang++: error: no such file or directory: 'path.o'`. TODO: fix this. Is not a regression (`Macros.sh` never supported spaces; `build.sh`'s paths don't have spaces.) */
		fi
	done
	echo "${NEW_PATH}"
)

#/* `SUSUWU_SH_<color>`. Notice: update [cxx/Macros.hxx](cxx/Macros.hxx) if you update those. */
#/* Usage: `SUSUWU_PRINT "${SUSUWU_SH_<warn-level>}" "${SUSUWU_SH_<color>}<message>${SUSUWU_SH_DEFAULT}"`. */
export SUSUWU_SH_DEFAULT="\033[0m"
export SUSUWU_SH_BLACK="\033[0;30m" #xterm256 "\033[38;5;0m"
export SUSUWU_SH_DARK_GRAY="\033[1;30m" #xterm256 "\033[38;5;8m"
export SUSUWU_SH_RED="\033[0;31m" #xterm256 "\033[38;5;1m"
export SUSUWU_SH_LIGHT_RED="\033[1;31m" #xterm256 "\033[38;5;9m"
export SUSUWU_SH_GREEN="\033[0;32m" #xterm256 "\033[38;5;2m"
export SUSUWU_SH_LIGHT_GREEN="\033[1;32m" #xterm256 "\033[38;5;10m"
export SUSUWU_SH_BROWN="\033[0;33m" #xterm256 "\033[38;5;3m"
export SUSUWU_SH_YELLOW="\033[1;33m" #xterm256 "\033[38;5;11m"
export SUSUWU_SH_BLUE="\033[0;34m" #xterm256 "\033[38;5;4m"
export SUSUWU_SH_LIGHT_BLUE="\033[1;34m" #xterm256 "\033[38;5;12m"
export SUSUWU_SH_PURPLE="\033[0;35m" #xterm256 "\033[38;5;5m"
export SUSUWU_SH_LIGHT_PURPLE="\033[1;35m" #xterm256 "\033[38;5;13m"
export SUSUWU_SH_CYAN="\033[0;36m" #xterm256 "\033[38;5;6m"
export SUSUWU_SH_LIGHT_CYAN="\033[1;36m" #xterm256 "\033[38;5;14m"
export SUSUWU_SH_LIGHT_GRAY="\033[0;37m" #xterm256 "\033[38;5;7m"
export SUSUWU_SH_WHITE="\033[1;37m" #xterm256 "\033[38;5;15m"
export SUSUWU_SH_RESET_WHITE="\033[0;1;37m" #xterm256 "\033[38;5;15m"
export SUSUWU_SH_CLOSE_="${SUSUWU_SH_DEFAULT}"
SUSUWU_SH_COLOR_COUNT() ( #/* Usage: `LOCAL_COLOR_COUNT=SUSUWU_SH_COLOR_COUNT` */
	SUSUWU_SH_COLOR_COUNT_MINIMUM_COLORS=8
	if [ -n "${SUSUWU_SH_COLOR_COUNT_CACHE}" ]; then
		echo "${SUSUWU_SH_COLOR_COUNT_CACHE}"
		test "${SUSUWU_SH_COLOR_COUNT_CACHE}" -ge "${SUSUWU_SH_COLOR_COUNT_MINIMUM_COLORS}" #test that color count is Greater or Equal to minimum count
		return $? #return `test`'s return value
	fi
	if [ "${TERM}" = "" ]; then
		echo "[$0: Warning: SUSUWU_SH_COLOR_COUNT(): If your console (\`[ \"\${TERM}\" = \"${TERM}\" ]\`, which _GitHub_ uses) lacks colors such as ${SUSUWU_SH_BLUE}blue${SUSUWU_SH_DEFAULT} (shows glitches, or literal codes such as \"\\\033[0;34m\"), execute \`export TERM=\"dumb\"\` to disable those.]" >&2
		echo 8 #/* [_GitHub_ Autobuild](https://github.com/SwuduSusuwu/SubStack/actions/runs/13209802112/job/36880995224) workaround. */
		return 0 #/* TODO: include other tests (`return 1` if the console does not allow color codes) */
	fi
	if command -v "tput" >/dev/null; then #if installed `ncurses-utils`
		COLOR_COUNT="$(tput colors 2>/dev/null)" || COLOR_COUNT="-1"
		echo "${COLOR_COUNT}"
		test "${COLOR_COUNT}" -ge "${SUSUWU_SH_COLOR_COUNT_MINIMUM_COLORS}" #test that color count is Greater or Equal to minimum count
		return $? #return `test`'s return value
	else
		echo "[$0: ${SUSUWU_SH_DEBUG}SUSUWU_SH_COLOR_COUNT(): The program \`tput\` was not found; use \`apt install ncurses-utils\` to have \`SUSUWU_SH_*()\` compatible with all consoles."
	fi
	for CONSOLE in "xterm-256color" "screen-256color" "tmux-256color" "rxvt-256color"; do
		if [ "${TERM}" = "${CONSOLE}" ]; then
			echo 256 #256-color console
			return 0
		fi
	done
	for CONSOLE in "xterm" "xterm-color" "screen" "screen-color" "tmux" "tmux-color" "linux" "rxvt" "rxvt-unicode"; do
		if [ "${TERM}" = "${CONSOLE}" ]; then
			echo 8 #standard console
			return 0
		fi
	done
	echo -1
	return 1 #unsupported console
)
SUSUWU_SH_COLOR_COUNT_CACHE="$(SUSUWU_SH_COLOR_COUNT)"
SUSUWU_SH_HAS_UNIX_CONSOLE() ( #/* Usage: `if SUSUWU_SH_HAS_UNIX_CONSOLE; then echo "\033[0;34mThis is blue."` */
		test "$(SUSUWU_SH_COLOR_COUNT)" -ge "8" #test that color count is Greater or Equal to 8
		return $? #return `test`'s output value
)
SUSUWU_SH_HAS_256COLOR_CONSOLE() ( #/* Usage: `if SUSUWU_SH_HAS_256COLOR_CONSOLE; then echo "\033[38;5;4mThis is blue."` */
		test "$(SUSUWU_SH_COLOR_COUNT)" -ge "256" #test that color count is Greater or Equal to 256
		return $? #return `test`'s output value
)
SUSUWU_SH_USE() ( #/* Usage: `SUSUWU_SH_USE "${SUSUWU_SH_<attribute>}" "<message>" ["${SUSUWU_SH_DEFAULT}"] ["&1"]`. Uses <attribute> on <message>. */
#	if [ ! $(SUSUWU_SH_HAS_UNIX_CONSOLE) ]; then
#		case "${1}" in
#			"${SUSUWU_SH_}") #TODO; non-standard code routes (such as `ConsoleApi2.h:SetConsoleTextAttribute`).
#			;;
#		esac
#	fi
	if SUSUWU_SH_HAS_UNIX_CONSOLE; then
		echo "${1}${2}${3:-${SUSUWU_SH_DEFAULT}}" #TODO: `>${4:-&1}` #/* `&1` is `std::cout`/`stdout` */
	else
		echo "${2}" #TODO: `>${4:-&1}` #/* `&1` is `std::cout`/`stdout` */
	fi
)
SUSUWU_SH_USE2() ( #/* Usage: `SUSUWU_SH_USE2 "${SUSUWU_SH_<attribute>}" "<message>" ["${SUSUWU_SH_RESET_WHITE}"] ["&2"]`. Is `SUSUWU_SH_USE` for `&2`. */
	SUSUWU_SH_USE "${1}" "${2}" "${3:-${SUSUWU_SH_RESET_WHITE}}" "${4:-&2}" #/* `&2` is `std::cerr`/`stderr` */
)

#/* `SUSUWU_SH_<warn-level>`. Notice: update [cxx/Macros.hxx](cxx/Macros.hxx) if you update those. */
#/* Usage: `SUSUWU_PRINT "${SUSUWU_SH_<warn-level>}" "<message>"`. */
SUSUWU_SH_ERROR() (SUSUWU_SH_USE2 "${SUSUWU_SH_RED}" "Error: ")
SUSUWU_SH_WARNING() (SUSUWU_SH_USE2 "${SUSUWU_SH_PURPLE}" "Warning: ")
SUSUWU_SH_INFO() (SUSUWU_SH_USE2 "${SUSUWU_SH_CYAN}" "Info: ")
SUSUWU_SH_SUCCESS() (SUSUWU_SH_USE2 "${SUSUWU_SH_GREEN}" "Success: ")
SUSUWU_SH_NOTICE() (SUSUWU_SH_USE2 "${SUSUWU_SH_BLUE}" "Notice: ")
SUSUWU_SH_DEBUG() (SUSUWU_SH_USE2 "${SUSUWU_SH_BLUE}" "Debug: ")

SUSUWU_S=false
SUSUWU_VERBOSE=false
SUSUWU_PROCESS_S() { #/* Usage: `SUSUWU_PROCESS_S $@`. [This processes params passed to `${0}`.] */
	if SUSUWU_SH_HAS_PARAM "-s" "$@"; then
		SUSUWU_S=true
	fi
}
SUSUWU_PROCESS_VERBOSE() { #/* Usage: `SUSUWU_PROCESS_VERBOSE $@`. [This processes params passed to `${0}`.] */
	if SUSUWU_SH_HAS_PARAM "--verbose" "$@"; then
		SUSUWU_VERBOSE=true
		set -x
	fi
}
SUSUWU_SH_HAS_FUNCNAME() ( #/* Usage: `if SUSUWU_SH_HAS_FUNCNAME 2>/dev/null; then echo "${FUNCNAME[0]}(): used FUNCNAME."` */
#	[ "$(uname)" = "Darwin" ] && return 0 #redundant (due to `${FUNCNAME[0]}` test).
#	test "$(type -t FUNCNAME)" = "array" #always returns "1".
	#shellcheck disable=SC2039 #this is a feature test, so disable "In POSIX sh, array references are undefined."
	test "${FUNCNAME[0]}" = "SUSUWU_SH_HAS_FUNCNAME" 2>/dev/null #if no arrays, prints "Bad substitution".
	return $?
)
if [ -z "${SUSUWU_SH_HAS_FUNCNAME_RESULT}" ]; then
	SUSUWU_SH_HAS_FUNCNAME 2>/dev/null #`/bin/sh` ignores the `2>/dev/null` in `SUSUWU_SH_HAS_FUNCNAME()`.
	export SUSUWU_SH_HAS_FUNCNAME_RESULT=$? #/* Usage: `if [ ${SUSUWU_SH_HAS_FUNCNAME_RESULT} -eq 0 ]; then echo "${FUNCNAME[0]}: used FUNCNAME."` */
fi
#for var in SUSUWU_SH_FILE SUSUWU_SH_LINE SUSUWU_SH_FUNC; do
#	[ -z "${!var}" ] && export "$var=true" #prints "Bad substitution".
#done
export SUSUWU_SH_FILE="${SUSUWU_SH_FILE:-""}"
export SUSUWU_SH_LINE="${SUSUWU_SH_LINE:-""}"
export SUSUWU_SH_FUNC="${SUSUWU_SH_FUNC:-"true"}"
export SUSUWU_SH_FILE_OR_LINE="${SUSUWU_SH_FILE:-${SUSUWU_SH_LINE}}"
SUSUWU_PRINT() ( #/* Usage: `SUSUWU_PRINT ["<optional caller-name>"] "$(SUSUWU_SH_<warn-level>)" "<message>" */
	if [ "$#" -eq 3 ]; then
		CALLER_FUNC="${1}"; shift
	fi
	LEVEL="${1}"
	MESSAGE="${2}"
	case "${LEVEL}" in
		"$(SUSUWU_SH_NOTICE)")
			${SUSUWU_S} && return 1
			;;
		"$(SUSUWU_SH_DEBUG)")
			(! ${SUSUWU_VERBOSE}) && return 1
			;;
	esac
	NEW_MESSAGE="[${SUSUWU_SH_FILE:+"$0:"}${SUSUWU_SH_LINE:+"${LINENO}:"}${SUSUWU_SH_FILE_OR_LINE:+" "}${LEVEL}"
	if [ "true" = "${SUSUWU_SH_FUNC}" ]; then
		if [ -n "${CALLER_FUNC}" ]; then
			NEW_MESSAGE="${NEW_MESSAGE}${CALLER_FUNC}: "
		elif [ "${SUSUWU_SH_HAS_FUNCNAME_RESULT}" -eq 0 ]; then
#shellcheck disable=SC2039 #if `SUSUWU_SH_HAS_FUNCNAME`, console supports this
			NEW_MESSAGE="${NEW_MESSAGE}${FUNCNAME[1]}(): "
		elif [ -n "$KSH_VERSION" ]; then
			NEW_MESSAGE="${NEW_MESSAGE}${.sh.fun}: "
		fi
	fi
	NEW_MESSAGE="${NEW_MESSAGE}${MESSAGE}${SUSUWU_SH_CLOSE_}]"
	printf '%b\n' "${NEW_MESSAGE}" >&2 #/* fd=2 is `std::cerr`/`stderr` */
	return $?
)
if ! SUSUWU_SH_HAS_UNIX_CONSOLE && [ ! "${SUSUWU_SH_CONSOLE_ERROR_SHOWN}" ]; then
	export SUSUWU_SH_CONSOLE_ERROR_SHOWN=true
	SUSUWU_PRINT "SUSUWU_SH_HAS_UNIX_CONSOLE()" "$(SUSUWU_SH_WARNING)" "failed. TODO: support systems without UNIX console codes. If your console (\`[ \"\${TERM}\" = \"${TERM}\" ]\`) shows colors such as ${SUSUWU_SH_BLUE}blue${SUSUWU_SH_DEFAULT} (not glitches or literal codes such as \"\\\033[0;34m\"), you can [post an issue](https://github.com/SwuduSusuwu/SubStack/issues/new) about this, or execute \`export TERM=\"linux\"\` to enable console code use."
fi

SUSUWU_DEFAULT_BRANCH() ( #/* Usage: `echo "$(SUSUWU_DEFAULT_BRANCH ["<fallback>"])"` */
	DEFAULT_BRANCH="$(git symbolic-ref -q --short "refs/remotes/$(git remote)/HEAD" | sed -n "s/$(git remote)\/\(.*\)/\1/p")" #remote branch
	if [ -z "${DEFAULT_BRANCH}" ]; then #if `git remote` not found
		DEFAULT_BRANCH="$(git branch --sort=-refname | grep -o -m1 '\b\(main\|master\|trunk\)\b')" #local branch; if you update this, update `README.md#git`.
	fi
	echo "${DEFAULT_BRANCH:-${1}}" #https://github.com/SwuduSusuwu/SubStack/actions/runs/<number>/job/<number> has bare repos, which use <fallback>.
)
SUSUWU_PRODUCTION_USE() ( #/* Usage: `SUSUWU_PRODUCTION_USE ["<default branch>"]` */
	if command -v git >/dev/null && git rev-parse --is-inside-work-tree >/dev/null 2>&1; then #test -d ".git/"; then
		THIS_BRANCH="$(git rev-parse --abbrev-ref HEAD)" #detect current branch
		if [ -n "${1}" ]; then
			DEFAULT_BRANCH="${1}" #use default branch from build script
		else
			DEFAULT_BRANCH="$(SUSUWU_DEFAULT_BRANCH "${1}")" #detect default branch
		fi
		if [ "${DEFAULT_BRANCH}" = "${THIS_BRANCH}" ]; then
			SUSUWU_PRINT "SUSUWU_PRODUCTION_USE()" "$(SUSUWU_SH_NOTICE)" "\`git branch\` is \"$(SUSUWU_SH_USE2 "${SUSUWU_SH_GREEN}" "${THIS_BRANCH}")\"."
		else
			SUSUWU_PRINT "SUSUWU_PRODUCTION_USE()" "$(SUSUWU_SH_WARNING)" "\`git branch\` is \"$(SUSUWU_SH_USE2 "${SUSUWU_SH_GREEN}" "${THIS_BRANCH}")\"; for production use, execute \`git switch $(SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_PURPLE}" "${DEFAULT_BRANCH}")\`."
		fi
	fi
)

