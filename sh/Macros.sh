#!/bin/sh
#
#/* (C) 2024 Swudu Susuwu, dual licenses: choose [_GPLv2_](./LICENSE_GPLv2) or [_Apache 2_](./LICENSE) (allows all uses). */
#/* Based on ../cxx/Macros.hxx */
#/* TODO: [produce `for OPTION in @$; do` loops for all `SUSUWU_PROCESS_*` functions.](https://github.com/SwuduSusuwu/SubStack/issues/22) */
#/* TODO: [produce alias (such as {`--silent`, `--quiet`} -> `-s`) groups of options/flags, for `SUSUWU_PROCESS_*` functions.](https://github.com/SwuduSusuwu/SubStack/issues/23) */
#/* TODO: [map options/flags (which `SUSUWU_PROCESS_*` functions use) to descriptions (for `--help` output.)](https://github.com/SwuduSusuwu/SubStack/issues/24) */
SUSUWU_DIR_SUFFIX_SLASH() ( #/* Usage: `OBJDIR=$(SUSUWU_ENSURE_DIR_SLASH "${OBJDIR}") */
	DIR=${1}
	if [ "${DIR}" = "${DIR%/}" ]; then #/* "%/" removes slash; if equal after this, original doesn't have '/'. */
		DIR="${DIR}/" #/* if original doesn't have, append '/' */
	fi
	echo "${DIR}" #/* return with slash */
)
SUSUWU_DIR_AFFIX_DOTSLASH() ( #/* Usage: `BINDIR=$(SUSUWU_ENSURE_DIR_SLASH "${BINDIR}") */
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

#/* Based on cxx/Macros.hxx */
export SUSUWU_SH_DEFAULT="\033[0m"
export SUSUWU_SH_BLACK="\033[0;30m"
export SUSUWU_SH_DARK_GRAY="\033[1;30m"
export SUSUWU_SH_RED="\033[0;31m"
export SUSUWU_SH_LIGHT_RED="\033[1;31m"
export SUSUWU_SH_GREEN="\033[0;32m"
export SUSUWU_SH_LIGHT_GREEN="\033[1;32m"
export SUSUWU_SH_BROWN="\033[0;33m"
export SUSUWU_SH_YELLOW="\033[1;33m"
export SUSUWU_SH_BLUE="\033[0;34m"
export SUSUWU_SH_LIGHT_BLUE="\033[1;34m"
export SUSUWU_SH_PURPLE="\033[0;35m"
export SUSUWU_SH_LIGHT_PURPLE="\033[1;35m"
export SUSUWU_SH_CYAN="\033[0;36m"
export SUSUWU_SH_LIGHT_CYAN="\033[1;36m"
export SUSUWU_SH_LIGHT_GRAY="\033[0;37m"
export SUSUWU_SH_WHITE="\033[1;37m"
export SUSUWU_SH_ERROR="${SUSUWU_SH_RED}Error: ${SUSUWU_SH_WHITE}"
export SUSUWU_SH_WARNING="${SUSUWU_SH_PURPLE}Warning: ${SUSUWU_SH_WHITE}"
export SUSUWU_SH_INFO="${SUSUWU_SH_CYAN}Info: ${SUSUWU_SH_WHITE}"
export SUSUWU_SH_SUCCESS="${SUSUWU_SH_GREEN}Success: ${SUSUWU_SH_WHITE}"
export SUSUWU_SH_NOTICE="${SUSUWU_SH_BLUE}Notice: ${SUSUWU_SH_WHITE}"
export SUSUWU_SH_DEBUG="${SUSUWU_SH_BLUE}Debug: ${SUSUWU_SH_WHITE}"
SUSUWU_SH_CLOSE_="${SUSUWU_SH_DEFAULT}"
SUSUWU_S=false
SUSUWU_VERBOSE=false
SUSUWU_PROCESS_S() { #/* Usage: `SUSUWU_PROCESS_S $@`. [This processes params passed to `${0}`.] */
	if [ "-s" = "${1}" ] || [ "-s" = "${2}" ]; then
		SUSUWU_S=true
	fi
}
SUSUWU_PROCESS_VERBOSE() { #/* Usage: `SUSUWU_PROCESS_VERBOSE $@`. [This processes params passed to `${0}`.] */
	if [ "--verbose" = "${1}" ] || [ "--verbose" = "${2}" ]; then
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
SUSUWU_PRINT() ( #/* Usage: `SUSUWU_PRINT "${SUSUWU_SH_{ERROR,WARNING,INFO,SUCCESS,NOTICE,DEBUG}}" "<message>" */
	LEVEL="${1}"
	MESSAGE="${2}"
	case "${LEVEL}" in
		"${SUSUWU_SH_NOTICE}")
			${SUSUWU_S} && return 1
			;;
		"${SUSUWU_SH_DEBUG}")
			(! ${SUSUWU_VERBOSE}) && return 1
			;;
	esac
	echo "[${LEVEL}${MESSAGE}${SUSUWU_SH_CLOSE_}]" >&2 #/* fd=2 is `std::cerr`/`stderr` */
	return $?
)

SUSUWU_DEFAULT_BRANCH() ( #/* Usage: `echo "$(SUSUWU_DEFAULT_BRANCH)"` */
	DEFAULT_BRANCH="$(git symbolic-ref --short "refs/remotes/$(git remote)/HEAD" | sed -n "s/$(git remote)\/\(.*\)/\1/p")" #remote branch
	if [ -z "${DEFAULT_BRANCH}" ]; then #if `git remote` not found
		DEFAULT_BRANCH="$(git branch --sort=-refname | grep -o -m1 '\b\(main\|master\|trunk\)\b')" #local branch; if you update this, update `README.md#git`.
	fi
	echo "${DEFAULT_BRANCH}"
)
SUSUWU_PRODUCTION_USE() ( #/* Usage: `SUSUWU_PRODUCTION_USE` */
	if command -v git >/dev/null && git rev-parse --is-inside-work-tree >/dev/null 2>&1; then #test -d ".git/"; then
		THIS_BRANCH="$(git rev-parse --abbrev-ref HEAD)" #detect current branch
		if [ -n "${1}" ]; then
			DEFAULT_BRANCH="${1}" #use default branch from build script
		else
			DEFAULT_BRANCH="$(SUSUWU_DEFAULT_BRANCH)" #detect default branch
		fi
		if [ "${DEFAULT_BRANCH}" = "${THIS_BRANCH}" ]; then
			SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "\`git branch\` is \"${THIS_BRANCH}\"."
		else
			SUSUWU_PRINT "${SUSUWU_SH_WARNING}" "\`git branch\` is \"${THIS_BRANCH}\"; for production use, execute \`git switch ${DEFAULT_BRANCH}\`."
		fi
	fi
)

