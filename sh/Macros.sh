#!/bin/sh
#
#/* (C) 2024 Swudu Susuwu, dual licenses: choose [_GPLv2_](./LICENSE_GPLv2) or [_Apache 2_](./LICENSE) (allows all uses).
# * Based on `../cxx/Macros.hxx`.
# * TODO: [map options/flags (which `SUSUWU_PROCESS_*` functions use) to descriptions (for `--help` output.)](https://github.com/SwuduSusuwu/SusuLib/issues/24) */

export SUSUWU_SH_CONSOLE_PARAMS="$*" #/* For functions which are not passed `$@` */
SUSUWU_SH_HAS_PARAM() ( #/* Usage: `if SUSUWU_SH_HAS_PARAM "--param [...]" "$@";`. [This processes params passed to `${0}`.] */
	if [ "$#" -eq 1 ]; then                                    #/* If function was not passed `$@`,
		SUSUWU_SH_HAS_PARAM "${1}" "${SUSUWU_SH_CONSOLE_PARAMS}" # * use stored values */
		return $?
	fi
	PARAM_Q="${1}"; shift;
	for PARAM_W in "$@"; do
		for PARAM in ${PARAM_Q}; do #/* The param can have aliases, such as short forms. */
			if [ "${PARAM}" = "${PARAM_W}" ]; then
				return 0
			fi
		done
	done
	return 1
)
SUSUWU_SH_REMOVE_PARAM() ( #/* Usage: `echo "$(SUSUWU_SH_REMOVE_PARAM "--unwanted-param" "$@")"`. [This processes params passed to `${0}`.] */
	PARAM=${1}; shift;
	NEW_PARAMS=""
	SUSUWU_SH_REMOVE_PARAM_FOUND=1 #/* `false` */
	for PARAM_W in "$@"; do
		if [ "${PARAM}" = "${PARAM_W}" ]; then
			SUSUWU_SH_REMOVE_PARAM_FOUND=0 #/* `true` */
		else
			if [ -z "${NEW_PARAMS}" ]; then
				NEW_PARAMS="${PARAM_W}"
			elif [ -n "${NEW_PARAMS}" ]; then
				NEW_PARAMS="${NEW_PARAMS} ${PARAM_W}" #/* TODO: spaces? */
			fi
		fi
	done
	echo "${NEW_PARAMS}"
	return ${SUSUWU_SH_REMOVE_PARAM_FOUND} #/* allows use as `if $(SUSUWU_SH_REMOVE_PARAM "--unwanted-param" "$@"); then` */
)
SUSUWU_STATIC_IS_PREVIEW() ( #/* Usage; `if SUSUWU_IS_PREVIEW_CONSTANT; then EXPERIMENTAL_CODE(); fi`. Is fast (versus `SUSUWU_IS_PREVIEW()`, but must hardcode for production (or release) versus "experimental" (or "preview") branches. */
	return 1 #/* TODO: for "experimental" (or "preview") branches, `return 0` */
)
SUSUWU_IS_PREVIEW() ( #/* Usage; `if SUSUWU_IS_PREVIEW ["<default branch>"]; then EXPERIMENTAL_CODE(); fi` */
	if command -v git >/dev/null && git rev-parse --is-inside-work-tree >/dev/null 2>&1; then #test -d ".git/"; then
		THIS_BRANCH="$(git rev-parse --abbrev-ref HEAD)" #/* Compute current branch */
		if [ ! "HEAD" = "${THIS_BRANCH}" ]; then #/* If `git rebase` (or "detached HEAD"); unknown branch, skip */
			if [ "$(SUSUWU_DEFAULT_BRANCH "${1}")" = "${THIS_BRANCH}" ]; then #/* If production (or release) branch */
				return 1
			else #/* else is "experimental" (or "preview") branch. */
				return 0
			fi
		fi
	fi
	SUSUWU_STATIC_IS_PREVIEW #/* Use hardcoded value. */
)
SUSUWU_PATH_SUFFIX_SLASH() ( #/* Usage: `OBJDIR="$(SUSUWU_PATH_SUFFIX_SLASH "${OBJDIR}")"` */
	DIR=${1}
	if [ "${DIR}" = "${DIR%/}" ]; then #/* "%/" removes slash; if equal after this,
		DIR="${DIR}/"                    # * ... original doesn't have '/', append '/'. */
	fi
	echo "${DIR}" #/* return with slash */
)
SUSUWU_PATH_AFFIX_DOTSLASH() ( #/* Usage: `BINDIR="$(SUSUWU_PATH_AFFIX_DOTSLASH "${BINDIR}")"` */
	DIR=${1}
	case "${DIR}" in
		./*) #/* If original has "./", */
			;; #/* ... continue. */
		*)               #/* If default (if original doesn't match "./"),
			DIR="./${DIR}" # * ... affix "./". */
			;;
	esac
	echo "${DIR}" #/* return with dot slash. */
)
SUSUWU_PATH_UNAMBIGUOUS() ( #/* Usage: `echo "USRBIN=\"$(SUSUWU_UNAMBIGUOUS_PATH "${USRBIN}")\"` */
	if [ "$(realpath -q "${1}")" != "${1}" ]; then #/* If relative path,
		SUSUWU_PATH_AFFIX_DOTSLASH "${1}"            # * ... ensure path starts with "./". */
	else
		echo "${1}"
	fi
)
SUSUWU_PATH_SHOULD_NOT_EXIST() { #/* Usage: `SUSUWU_PATH_SHOULD_NOT_EXIST "<function>" "<path>" && cp "${0}" "<path>"` */
	if [ -e "${2}" ]; then
		SUSUWU_PRINT "${1}: SUSUWU_PATH_SHOULD_NOT_EXIST()" "$(SUSUWU_SH_ERROR)" "$(SUSUWU_SH_QUOTE "PATH" "${2}") exists. Use $(SUSUWU_SH_QUOTE "CODE" "mv \"${2}\" \"${2}.bak\"") (or $(SUSUWU_SH_QUOTE "CODE" "rm \"${2}\"")) and re-execute $(SUSUWU_SH_QUOTE "CODE FUNCTION" "${1}") (perhaps with $(SUSUWU_SH_QUOTE "CODE" "${0}")) to continue."
		exit 1 #/* Exit with status 1, unless a subshell invoked this. */
	fi
	return 0
}
SUSUWU_ESCAPE_SPACES() ( #/* Usage: `SUSUWU_OBJECTLIST="${SUSUWU_OBJECTLIST} $(SUSUWU_ESCAPE_SPACES "${OBJECT}"). */
#	echo $(echo "$@" | sed 's/ /\\\ /') #/* Error: `sed not found`, although is installed. */
#	echo "\"${@}\""; #/* Error: if `LOCAL_OBJECT="obj/main.o"`, `SUSUWU_BUILD_EXECUTABLE()` gives `clang++: error: no such file or directory: '"obj/main.o"'` (uses path with literal quotes). */
	NEW_PATH=""
	for OLD_PATH_TOKEN in "$@"; do #/* Split old path into tokens */
		if [ -z "${NEW_PATH}" ]; then
			NEW_PATH="${OLD_PATH_TOKEN}"
		elif [ -n "${NEW_PATH}" ]; then
			NEW_PATH="${NEW_PATH}\\ ${OLD_PATH}" #/* Error: if `OBJECT="obj/long path.o"`, `SUSUWU_BUILD_EXECUTABLE()` gives `clang++: error: no such file or directory: 'obj/long\'\nclang++: error: no such file or directory: 'path.o'`. TODO: fix this. Is not a regression (`Macros.sh` never supported spaces; `build.sh`'s paths don't have spaces.) */
		fi
	done
	echo "${NEW_PATH}"
)
SUSUWU_ESCAPE_QUOTED() ( #/* Usage: `echo "\"param\": \"$(SUSUWU_ESCAPE_QUOTED "${VALUE}")\"" >> out.json`. */
	echo "$@" | sed 's/"/\\"/g' | sed 's/\\/\\\\/g'
)

SUSUWU_STR_TOKEN_FIRST() ( #/* Usage: `SUSUWU_STR_TOKEN_FIRST "<input>" "<delimiter>". Purpose: splits <input> on <delimiter>, returns all before last <delimiter>. */
	echo "${1%"${2}"*}" #/* TODO: allow <delimiter>="\033". */
) #/* Analogous to echo "${1}" | sed "s/\(${2}[^${2}]*\)\$//" #shellcheck disable=SC2001 #https://www.shellcheck.net/wiki/SC2001 */
[ "uwu" = "$(SUSUWU_STR_TOKEN_FIRST "uwu" ":")" ] || echo "[$0: SUSUWU_STR_TOKEN_FIRST(): Error: logic_error; test failed.]"
[ "uwu:q" = "$(SUSUWU_STR_TOKEN_FIRST "uwu:q:mukyu" ":")" ] || echo "[$0: SUSUWU_STR_TOKEN_FIRST(): Error: logic_error; test failed.]"
[ "uwu^q" = "$(SUSUWU_STR_TOKEN_FIRST "uwu^q^mukyu" "^")" ] || echo "[$0: SUSUWU_STR_TOKEN_FIRST(): Error: logic_error; test failed.]"
#[ "uwu\033q" = "$(SUSUWU_STR_TOKEN_FIRST "uwu\033q\033mukyu" "\\033")" ] || echo "[$0: SUSUWU_STR_TOKEN_FIRST(): Error: logic_error; test failed.]"
#[ "uwu$'\033'q" = "$(SUSUWU_STR_TOKEN_FIRST "uwu$'\033'q$'\033'mukyu" $'\033')" ] || echo "[$0: SUSUWU_STR_TOKEN_FIRST(): Error: logic_error; test failed.]"
SUSUWU_STR_TOKEN_LAST() ( #/* Usage: `SUSUWU_STR_TOKEN_LAST "<input>" "<delimiter>". Purpose: splits <input> on <delimiter>, returns all after last <delimiter>. */
	RESULT="${1#*"${2}"}" #/* `RESULT` is all after first <delimiter>` */
	if [ "${RESULT}" != "${1}" ]; then         #/* if `RESULT` is not the input, 1 or more <delimiter> was found,
		SUSUWU_STR_TOKEN_LAST "${RESULT}" "${2}" # * so search for next <delimiter>. */
	else
		echo "${RESULT}" #/* `RESULT` is now after last <delimiter> */
	fi
) #/* Analogous to: echo "${1}" | sed "s/^.*${2}//" #shellcheck disable=SC2001 #https://www.shellcheck.net/wiki/SC2001 */
[ "mukyu" = "$(SUSUWU_STR_TOKEN_LAST "mukyu" ":")" ] || echo "[$0: SUSUWU_STR_TOKEN_LAST(): Error: logic_error; test failed.]"
[ "mukyu" = "$(SUSUWU_STR_TOKEN_LAST "uwu:q:mukyu" ":")" ] || echo "[$0: SUSUWU_STR_TOKEN_LAST(): Error: logic_error; test failed.]"
[ "mukyu" = "$(SUSUWU_STR_TOKEN_LAST "uwu^q^mukyu" "^")" ] || echo "[$0: SUSUWU_STR_TOKEN_LAST(): Error: logic_error; test failed.]"
[ "mukyu" = "$(SUSUWU_STR_TOKEN_LAST "uwu\033q\033mukyu" "\033")" ] || echo "[$0: SUSUWU_STR_TOKEN_LAST(): Error: logic_error; test failed.]"

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
export SUSUWU_SH_BG_DEFAULT="\033[0;40m" #xterm256 "\033[48;5;0m"
export SUSUWU_SH_BG_RESET_BLACK="\033[0;40m" #xterm256 "\033[48;5;0m"
export SUSUWU_SH_BG_DARK_GRAY="\033[1;40m" #xterm256 "\033[48;5;8m"
export SUSUWU_SH_BG_RED="\033[0;41m" #xterm256 "\033[48;5;1m"
export SUSUWU_SH_BG_LIGHT_RED="\033[1;41m" #xterm256 "\033[48;5;9m"
export SUSUWU_SH_BG_GREEN="\033[0;42m" #xterm256 "\033[48;5;2m"
export SUSUWU_SH_BG_LIGHT_GREEN="\033[1;42m" #xterm256 "\033[48;5;10m"
export SUSUWU_SH_BG_BROWN="\033[0;43m" #xterm256 "\033[48;5;3m"
export SUSUWU_SH_BG_YELLOW="\033[1;43m" #xterm256 "\033[48;5;11m"
export SUSUWU_SH_BG_BLUE="\033[0;44m" #xterm256 "\033[48;5;4m"
export SUSUWU_SH_BG_LIGHT_BLUE="\033[1;44m" #xterm256 "\033[48;5;12m"
export SUSUWU_SH_BG_PURPLE="\033[0;45m" #xterm256 "\033[48;5;5m"
export SUSUWU_SH_BG_LIGHT_PURPLE="\033[1;45m" #xterm256 "\033[48;5;13m"
export SUSUWU_SH_BG_CYAN="\033[0;46m" #xterm256 "\033[48;5;6m"
export SUSUWU_SH_BG_LIGHT_CYAN="\033[1;46m" #xterm256 "\033[48;5;14m"
export SUSUWU_SH_BG_LIGHT_GRAY="\033[0;47m" #xterm256 "\033[48;5;7m"
export SUSUWU_SH_BG_WHITE="\033[1;47m" #xterm256 "\033[48;5;15m"
export SUSUWU_SH_CLOSE_="${SUSUWU_SH_DEFAULT}"

export SUSUWU_SH_TPUT_COMMAND="${SUSUWU_SH_TPUT_COMMAND:-tput}" #/* Usage: override with `export SUSUWU_SH_TPUT_COMMAND=<executable>` */
SUSUWU_SH_COLOR_COUNT() ( #/* Usage: `LOCAL_COLOR_COUNT=SUSUWU_SH_COLOR_COUNT` */
	SUSUWU_SH_COLOR_COUNT_MINIMUM_COLORS=8
	if [ -n "${SUSUWU_SH_COLOR_COUNT_CACHE}" ]; then
		echo "${SUSUWU_SH_COLOR_COUNT_CACHE}"
		test "${SUSUWU_SH_COLOR_COUNT_CACHE}" -ge "${SUSUWU_SH_COLOR_COUNT_MINIMUM_COLORS}" #/* test that color count is Greater or Equal to minimum count */
		return $? #/* return `test`'s return value */
	fi
	if [ -n "${GITHUB_ACTIONS}" ]; then
#shellcheck disable=SC2028 #/* Do not want to expand those sequences */
		echo "[$0: Warning: SUSUWU_SH_COLOR_COUNT(): detected environment is _GitHub_ (due to \`[ -n \"\${GITHUB_ACTIONS}\" ]\`), will force color use. If this console (\`[ \"\${TERM}\" = \"${TERM}\" ]\`, which _GitHub_ uses) lacks colors such as ${SUSUWU_SH_BLUE}blue${SUSUWU_SH_DEFAULT} (shows glitches, or literal codes such as \"\\\033[0;34m\"), [post an issue](https://github.com/SwuduSusuwu/SusuLib/issues/new) about this.]" >&2
		echo 8 #/* [_GitHub_ Autobuild](https://github.com/SwuduSusuwu/SusuLib/actions/runs/13209802112/job/36880995224) workaround. */
		return 0 #/* TODO: include other tests (`return 1` if the console does not allow color codes) */
	fi
	if command -v "${SUSUWU_SH_TPUT_COMMAND}" >/dev/null; then            #/* if installed `ncurses-utils`,
		COLOR_COUNT="$(${SUSUWU_SH_TPUT_COMMAND} colors 2>/dev/null)" || COLOR_COUNT="-1"
		echo "${COLOR_COUNT}"
		test "${COLOR_COUNT}" -ge "${SUSUWU_SH_COLOR_COUNT_MINIMUM_COLORS}" # * test that color count is Greater or Equal to minimum count */
		return $? #/* return `test`'s return value */
	else
		echo "[$0: Notice: SUSUWU_SH_COLOR_COUNT(): The program \`${SUSUWU_SH_TPUT_COMMAND}\` was not found; use \`apt install ncurses-utils\` to have \`SUSUWU_SH_*()\` compatible with all consoles.]" >&2
	fi
	for CONSOLE in "xterm-256color" "screen-256color" "tmux-256color" "rxvt-256color"; do
		if [ "${TERM}" = "${CONSOLE}" ]; then
			echo 256 #/* 256-color console */
			return 0
		fi
	done
	for CONSOLE in "xterm" "xterm-color" "screen" "screen-color" "tmux" "tmux-color" "linux" "rxvt" "rxvt-unicode"; do
		if [ "${TERM}" = "${CONSOLE}" ]; then
			echo 8 #/* standard 8-color console */
			return 0 #/* supported console */
		fi
	done
	echo -1 #/* no known attributes */
	return 1 #/* unsupported console */
)
SUSUWU_SH_COLOR_COUNT_CACHE="$(SUSUWU_SH_COLOR_COUNT)"
SUSUWU_SH_HAS_UNIX_CONSOLE() ( #/* Usage: `if SUSUWU_SH_HAS_UNIX_CONSOLE; then echo "\033[0;34mThis is blue."` */
		test "$(SUSUWU_SH_COLOR_COUNT)" -ge "8" #/* test that color count is Greater or Equal to 8 */
		return $? #/* return `test`'s return value */
)
SUSUWU_SH_HAS_256COLOR_CONSOLE() ( #/* Usage: `if SUSUWU_SH_HAS_256COLOR_CONSOLE; then echo "\033[38;5;4mThis is blue."` */
		test "$(SUSUWU_SH_COLOR_COUNT)" -ge "256" #/* test that color count is Greater or Equal to 256 */
		return $? #/* return `test`'s return value */
)
export SUSUWU_SH_USE_PUSH_DEFAULT="${SUSUWU_SH_DEFAULT}"
export SUSUWU_SH_USE_PUSH_RESET_WHITE="${SUSUWU_SH_RESET_WHITE}"
SUSUWU_SH_USE_PUSH_DEBUG() ( #/* Usage: `SUSUWU_SH_USE_PUSH_DEBUG "${FUNCNAME}" "<arg>" "<step>"` */
	${SUSUWU_VERBOSE} && printf "[Debug: ${SUSUWU_SH_LIGHT_CYAN}${1}${SUSUWU_SH_WHITE}(\"${SUSUWU_SH_GREEN}%s${SUSUWU_SH_WHITE}\"): ${3} ${SUSUWU_SH_LIGHT_CYAN}SUSUWU_SH_DEFAULT${SUSUWU_SH_WHITE}=\"${SUSUWU_SH_GREEN}%s${SUSUWU_SH_WHITE}\", ${SUSUWU_SH_LIGHT_CYAN}SUSUWU_SH_RESET_WHITE${SUSUWU_SH_WHITE}=\"${SUSUWU_SH_GREEN}%s${SUSUWU_SH_WHITE}\", ${SUSUWU_SH_LIGHT_CYAN}SUSUWU_SH_USE_PUSH_DEFAULT${SUSUWU_SH_WHITE}=\"${SUSUWU_SH_GREEN}%s${SUSUWU_SH_WHITE}\", ${SUSUWU_SH_LIGHT_CYAN}SUSUWU_SH_USE_PUSH_RESET_WHITE${SUSUWU_SH_WHITE}=\"${SUSUWU_SH_GREEN}%s${SUSUWU_SH_WHITE}\"]\n" "${2}" "${SUSUWU_SH_DEFAULT}" "${SUSUWU_SH_RESET_WHITE}" "${SUSUWU_SH_USE_PUSH_DEFAULT}" "${SUSUWU_SH_USE_PUSH_RESET_WHITE}" >&2
)
SUSUWU_SH_USE_PUSH() { #/* Usage: `SUSUWU_SH_USE_PUSH "${SUSUWU_SH_<attribute>}" && SUSUWU_SH_USE "${SUSUWU_SH_<attribute>}" "<message>" ["${SUSUWU_SH_RESET_WHITE}"] && SUSUWU_SH_USE_POP`. Purpose: nested `SUSUWU_SH_USE`. */
	if SUSUWU_STATIC_IS_PREVIEW; then
		SUSUWU_SH_USE_PUSH_DEBUG "SUSUWU_SH_USE_PUSH" "${1}" "Pushing:"
		SUSUWU_SH_DEFAULT="${1}"
		SUSUWU_SH_RESET_WHITE="${1}"
		SUSUWU_SH_USE_PUSH_DEFAULT="${SUSUWU_SH_USE_PUSH_DEFAULT}^${1}"
		SUSUWU_SH_USE_PUSH_RESET_WHITE="${SUSUWU_SH_USE_PUSH_RESET_WHITE}^${1}"
		SUSUWU_SH_USE_PUSH_DEBUG "SUSUWU_SH_USE_PUSH" "${1}" "Pushed:"
	fi
}
SUSUWU_SH_USE_POP() { #/* Usage: `SUSUWU_SH_USE_PUSH "${SUSUWU_SH_<attribute>}" && SUSUWU_SH_USE "${SUSUWU_SH_<attribute>}" "<message>" ["${SUSUWU_SH_RESET_WHITE}"] && SUSUWU_SH_USE_POP`. Purpose: nested `SUSUWU_SH_USE`. */
	if SUSUWU_STATIC_IS_PREVIEW; then
		SUSUWU_SH_USE_PUSH_DEBUG "SUSUWU_SH_USE_POP" "" "Popping:"
#		SUSUWU_SH_USE_PUSH_DEFAULT="$(SUSUWU_STR_TOKEN_FIRST "${SUSUWU_SH_USE_PUSH_DEFAULT}" '^')"
#		SUSUWU_SH_USE_PUSH_RESET_WHITE="$(SUSUWU_STR_TOKEN_FIRST "${SUSUWU_SH_USE_PUSH_RESET_WHITE}" '^')"
#		SUSUWU_SH_DEFAULT="$(SUSUWU_STR_TOKEN_LAST "${SUSUWU_SH_USE_PUSH_DEFAULT}" '^')" #'\033')"
#		SUSUWU_SH_RESET_WHITE="$(SUSUWU_STR_TOKEN_LAST "${SUSUWU_SH_USE_PUSH_RESET_WHITE}" '^')" #TODO: figure out why this doesn't restore last color code
		SUSUWU_SH_USE_PUSH_DEFAULT="${SUSUWU_SH_USE_PUSH_DEFAULT%^*}"
		SUSUWU_SH_USE_PUSH_RESET_WHITE="${SUSUWU_SH_USE_PUSH_RESET_WHITE%^*}"
		SUSUWU_SH_DEFAULT="${SUSUWU_SH_USE_PUSH_DEFAULT##*^}"
		SUSUWU_SH_RESET_WHITE="${SUSUWU_SH_USE_PUSH_RESET_WHITE##*^}"
		SUSUWU_SH_USE_PUSH_DEBUG "SUSUWU_SH_USE_POP" "" "Popped:"
	fi
} #/* TODO: figure out the reason (it appears that) this is always executed sequential (does not nest blocks, which push & pop must do) */
SUSUWU_SH_USE() { #/* Usage: `SUSUWU_SH_USE "${SUSUWU_SH_<attribute>}" "<message>" ["${SUSUWU_SH_DEFAULT}"] ["&1"]`. Uses <attribute> on <message>. */
#	if [ ! $(SUSUWU_SH_HAS_UNIX_CONSOLE) ]; then
#		case "${1}" in
#			"${SUSUWU_SH_}") #/* TODO; non-standard code routes (such as Win32's `ConsoleApi2.h:SetConsoleTextAttribute`). */
#			;;
#		esac
#	fi
	if SUSUWU_SH_HAS_UNIX_CONSOLE; then
		printf '%b' "${1}"
		SUSUWU_SH_USE_PUSH "${1}"
		printf '%b' "${2}"
		SUSUWU_SH_USE_POP
		echo "${3:-${SUSUWU_SH_RESET_WHITE}}" #/* echo "${3:-${SUSUWU_SH_DEFAULT}}" */
	else
		echo "${2}" #TODO: `>${4:-&1}` #/* `&1` is `std::cout`/`stdout` */
	fi
}
SUSUWU_SH_USE2() { #/* Usage: `SUSUWU_SH_USE2 "${SUSUWU_SH_<attribute>}" "<message>" ["${SUSUWU_SH_RESET_WHITE}"] ["&2"]`. Is `SUSUWU_SH_USE` for `&2`. */
	SUSUWU_SH_USE "${1}" "${2}" "${3:-${SUSUWU_SH_RESET_WHITE}}" "${4:-&2}" #/* `&2` is `std::cerr`/`stderr` */
}

#/* `SUSUWU_SH_<warn-level>`. Notice: update [cxx/Macros.hxx](cxx/Macros.hxx) if you update those.
# * Usage: `SUSUWU_PRINT "${SUSUWU_SH_<warn-level>}" "<message>"`. */
SUSUWU_SH_ERROR() ( SUSUWU_SH_USE2 "${SUSUWU_SH_RED}" "Error: "; )
SUSUWU_SH_WARNING() ( SUSUWU_SH_USE2 "${SUSUWU_SH_PURPLE}" "Warning: "; )
SUSUWU_SH_INFO() ( SUSUWU_SH_USE2 "${SUSUWU_SH_CYAN}" "Info: "; )
SUSUWU_SH_SUCCESS() ( SUSUWU_SH_USE2 "${SUSUWU_SH_GREEN}" "Success: "; )
SUSUWU_SH_NOTICE() ( SUSUWU_SH_USE2 "${SUSUWU_SH_BLUE}" "Notice: "; )
SUSUWU_SH_DEBUG() ( SUSUWU_SH_USE2 "${SUSUWU_SH_BLUE}" "Debug: "; )

SUSUWU_SH_QUOTE() { #/* Usage: `SUSUWU_SH_QUOTE "<type-of-quote [...]>" "<code | quote>" ["<optional original color>"])"`. */
	for SUSUWU_SH_QUOTE_W in ${1}; do continue; done
	if [ "${SUSUWU_SH_QUOTE_W}" != "${1}" ]; then #	if [ "${1% }" != "${1}" ]; then #if [ ${1} != "${1}" ]; then #/* analogous to `if [ "${1#}" -gt 1 ]; then` */
		SUSUWU_PRINT "SUSUWU_SH_QUOTE()" "$(SUSUWU_SH_DEBUG)" "Multiple modes: \${1}='$(SUSUWU_SH_QUOTE "CURRENT" "${1}")'"
	fi
	SUSUWU_SH_QUOTE_Q="${2}"
	for SUSUWU_SH_QUOTE_W in ${1}; do
		SUSUWU_SH_QUOTE_Q="$(SUSUWU_SH_QUOTE_SUB "${SUSUWU_SH_QUOTE_W}" "${SUSUWU_SH_QUOTE_Q}" "${3}")" #/* TODO: pass "" for <optional original color> unless this is the last cycle of the `do` */
	done
	echo "${SUSUWU_SH_QUOTE_Q}"
}
SUSUWU_SH_QUOTE_SUB() { #/* Usage: `SUSUWU_SH_QUOTE_SUB "<type-of-quote>" "<code | quote>" ["<optional original color>"])"` */
	case "${1}" in #/* Notice: update [`README.md#cc-source`](README.md#cc-source) if you update those. */
		"CODE") #/* command / code quote */
			echo "\`$(SUSUWU_SH_USE2 "${SUSUWU_SH_BROWN}" "${2}" "${3}")\`" ;;
		"PATH") #/* path to file or directory */
			echo "\"$(SUSUWU_SH_USE2 "${SUSUWU_SH_PURPLE}" "${2}" "${3}")\"" ;;
		"FUNCTION") #/* name of function */
			SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_CYAN}" "${2}" "${3}" ;;
		"STDERR") #/* error message quote */
			SUSUWU_SH_USE2 "${SUSUWU_SH_RED}" "${2}" "${3}" ;;
		"STATUS") #/* status code / return value */
			SUSUWU_SH_USE2 "${SUSUWU_SH_PURPLE}" "${2}" "${3}" ;;
		"VAR") #/* name of variable / name of constant */
			SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_CYAN}" "${2}" "${3}" ;;
		"CURRENT") #/* current value */
			SUSUWU_SH_USE2 "${SUSUWU_SH_GREEN}" "${2}" "${3}" ;;
		"PROPOSED") #/* speculative / proposed value */
			SUSUWU_SH_USE2 "${SUSUWU_SH_LIGHT_PURPLE}" "${2}" "${3}" ;;
		*) #/* default */
			SUSUWU_SH_USE2 "" "${2}" "${3}"
			SUSUWU_PRINT "SUSUWU_SH_QUOTE_SUB()" "$(SUSUWU_SH_NOTICE)" "Unknown $(SUSUWU_SH_QUOTE "VAR" "<type-of-quote>") $(SUSUWU_SH_QUOTE "CODE" "${1}")." ;;
	esac
	return $?
}

SUSUWU_S=false
SUSUWU_VERBOSE=false
export SUSUWU_ECHO_COMMANDS_TO="/dev/null" #/* `sh/make.sh:SUSUWU_SH_COMPILER_COMMAND`'s path for `echo`. */
SUSUWU_ECHO_COMMANDS() { #/* Usage: `SUSUWU_ECHO_COMMANDS [true | false] [<descriptor>]`. ] */
	(  ${1}) && (${SUSUWU_VERBOSE}) && set -x
	(! ${1}) && (${SUSUWU_VERBOSE}) && set +x
	(  ${1}) && (! ${SUSUWU_S}) && SUSUWU_ECHO_COMMANDS_TO="&2" #/* TODO: `${2:-&2}` as descriptor */
	(! ${1}) && (! ${SUSUWU_VERBOSE}) && SUSUWU_ECHO_COMMANDS_TO="/dev/null"
}
SUSUWU_PROCESS_S() { #/* Usage: `SUSUWU_PROCESS_S $@`. [This processes params passed to `${0}`.] */
	if SUSUWU_SH_HAS_PARAM "-s --silent --quiet --debug=n" "$@"; then
		SUSUWU_S=true
	fi
#	SUSUWU_PRINT "SUSUWU_PROCESS_VERBOSE()" "$(SUSUWU_SH_INFO)" "Was passed \`$(SUSUWU_SH_QUOTE "CURRENT" "--silent")\` (or an alias), so $(SUSUWU_SH_QUOTE "CODE" "\$(SUSUWU_SH_NOTICE)") is disabled." #/* TODO? Is it against the principles of `--silent` to print about it? */
}
SUSUWU_PROCESS_VERBOSE() { #/* Usage: `SUSUWU_PROCESS_VERBOSE $@`. [This processes params passed to `${0}`.] */
	if SUSUWU_SH_HAS_PARAM "-v --verbose -d --debug=a" "$@"; then
		SUSUWU_VERBOSE=true
		SUSUWU_ECHO_COMMANDS true
	fi
	SUSUWU_PRINT "SUSUWU_PROCESS_VERBOSE()" "$(SUSUWU_SH_DEBUG)" "Was passed \`$(SUSUWU_SH_QUOTE "CURRENT" "--verbose")\` (or an alias), so $(SUSUWU_SH_QUOTE "CODE" "\$(SUSUWU_SH_DEBUG)") is enabled."
}

SUSUWU_SH_HAS_FUNCNAME() ( #/* Usage: `if SUSUWU_SH_HAS_FUNCNAME 2>/dev/null; then echo "${FUNCNAME[0]}(): used FUNCNAME."` */
#	[ "$(uname)" = "Darwin" ] && return 0 #/* redundant (due to `${FUNCNAME[0]}` test). */
#	test "$(type -t FUNCNAME)" = "array" #/* always returns "1". */
	#shellcheck disable=SC2039,SC3028,SC3054 #/* this is a feature test, so disable "In POSIX sh, array references are undefined." */
	test "${FUNCNAME[0]}" = "SUSUWU_SH_HAS_FUNCNAME" 2>/dev/null #/* without array support, prints "Bad substitution". */
) #/* ends with implicit `return $?` */
if [ -z "${SUSUWU_SH_HAS_FUNCNAME_RESULT}" ]; then
	SUSUWU_SH_HAS_FUNCNAME 2>/dev/null #/* `/bin/sh` ignores the `2>/dev/null` in `SUSUWU_SH_HAS_FUNCNAME()`. */
	export SUSUWU_SH_HAS_FUNCNAME_RESULT=$? #/* Usage: `if [ ${SUSUWU_SH_HAS_FUNCNAME_RESULT} -eq 0 ]; then echo "${FUNCNAME[0]}: used FUNCNAME."` */
fi

#for var in SUSUWU_SH_FILE SUSUWU_SH_LINE SUSUWU_SH_FUNC; do
#	[ -z "${!var}" ] && export "$var=true" #/* prints "Bad substitution". */
#done
export SUSUWU_SH_FILE="${SUSUWU_SH_FILE:-""}"
export SUSUWU_SH_LINE="${SUSUWU_SH_LINE:-""}"
export SUSUWU_SH_FUNC="${SUSUWU_SH_FUNC:-"true"}"
export SUSUWU_SH_FILE_OR_LINE="${SUSUWU_SH_FILE:-${SUSUWU_SH_LINE}}"
SUSUWU_PRINT() ( #/* Usage: `SUSUWU_PRINT ["<optional caller-name>"] "$(SUSUWU_SH_<warn-level>)" "<message>" */
	if [ "$#" -eq 3 ]; then #/* If passed `<caller-name>` */
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
			NEW_MESSAGE="${NEW_MESSAGE}$(SUSUWU_SH_QUOTE "FUNCTION" "${CALLER_FUNC}: ")"
		elif [ "${SUSUWU_SH_HAS_FUNCNAME_RESULT}" -eq 0 ] && [ "${#FUNCNAME}" -ge 2 ]; then
#shellcheck disable=SC2039 #if `SUSUWU_SH_HAS_FUNCNAME`, console supports this
			NEW_MESSAGE="${NEW_MESSAGE}$(SUSUWU_SH_QUOTE "FUNCTION" "${FUNCNAME[1]}(): ")"
		elif [ -n "${KSH_VERSION}" ]; then
			NEW_MESSAGE="${NEW_MESSAGE}$(SUSUWU_SH_QUOTE "FUNCTION" "${.sh.fun}: ")"
		fi
	fi
	NEW_MESSAGE="${NEW_MESSAGE}${MESSAGE}${SUSUWU_SH_CLOSE_}]"
	printf '%b\n' "${NEW_MESSAGE}" >&2 #/* fd=2 is `std::cerr`/`stderr` */
) #/* ends with implicit `return $?` */
SUSUWU_PRINT "SUSUWU_PRINT()" "$(SUSUWU_SH_DEBUG)" "Test: $(SUSUWU_SH_QUOTE "CODE" "$(SUSUWU_SH_QUOTE "FUNCTION" "SUSUWU_SH_USE_PUSH") ... $(SUSUWU_SH_QUOTE "FUNCTION" "SUSUWU_SH_USE_POP")"). TODO: test should have ellipses ($(SUSUWU_SH_QUOTE "CODE" "...")) brown."
if ! SUSUWU_SH_HAS_UNIX_CONSOLE && [ ! "${SUSUWU_SH_CONSOLE_ERROR_SHOWN}" ]; then
	export SUSUWU_SH_CONSOLE_ERROR_SHOWN=true
	SUSUWU_PRINT "SUSUWU_SH_HAS_UNIX_CONSOLE()" "$(SUSUWU_SH_WARNING)" "failed. TODO: support systems without UNIX console codes. If your console ($(SUSUWU_SH_QUOTE "CODE" "[ \"\${TERM}\" = \"${TERM}\" ]")) shows colors such as ${SUSUWU_SH_BLUE}blue${SUSUWU_SH_DEFAULT} (not glitches or literal codes such as \"\\\033[0;34m\"), you can [post an issue](https://github.com/SwuduSusuwu/SusuLib/issues/new) about this, or execute $(SUSUWU_SH_QUOTE "CODE" "export TERM=\"linux\"") to enable console code use."
fi

export SUSUWU_ABORT_ON_FIRST_ERROR=false
SUSUWU_PROCESS_ABORT_ON_FIRST_ERROR() { #/* Usage: `SUSUWU_PROCESS_ABORT_ON_FIRST_ERROR $@`. */
	if SUSUWU_SH_HAS_PARAM "--abort-on-first-error -S --no-keep-going --stop" "$@"; then
		SUSUWU_ABORT_ON_FIRST_ERROR=true
	fi
}

SUSUWU_LOCAL_WORKSPACE_PATH() ( #/* Usage: `"$(SUSUWU_LOCAL_WORKSPACE_PATH)/compile_commands.json"` [Substitute for `${GIT_WORK_TREE}` or `${GIT_DIR}`]` */
	git rev-parse --absolute-git-dir >/dev/null 2>&1 || return 1
	dirname "$(git rev-parse --absolute-git-dir 2>/dev/null)"
)
SUSUWU_DEFAULT_BRANCH() ( #/* Usage: `echo "$(SUSUWU_DEFAULT_BRANCH ["<fallback>"])"` */
	DEFAULT_BRANCH="$(git symbolic-ref -q --short "refs/remotes/$(git remote)/HEAD" # | sed -n "s/$(git remote)\/\(.*\)/\1/p")" #/* remote branch */
	)"; DEFAULT_BRANCH="${DEFAULT_BRANCH#"$(git remote)/"}" #/* remote branch */ /* `sed -n` replaced with substitution */
	if [ -z "${DEFAULT_BRANCH}" ]; then #/* if `git remote` not found */
		DEFAULT_BRANCH="$(git branch --sort=-refname | grep -o -m1 '\b\(main\|master\|trunk\)\b')" #/* local branch; if you update this, update `README.md#git`. */
	fi
	echo "${DEFAULT_BRANCH:-${1}}" #/* [_GitHub Workflows_](https://docs.github.com/en/actions/concepts/workflows-and-actions/workflows) use "bare repos", which use <fallback>. */
)
SUSUWU_PRODUCTION_USE() ( #/* Usage: `SUSUWU_PRODUCTION_USE ["<default branch>"]` */
	if command -v git >/dev/null && git rev-parse --is-inside-work-tree >/dev/null 2>&1; then #/* this improves `test -d ".git/"; then` */
		THIS_BRANCH="$(git rev-parse --abbrev-ref HEAD)" #/* detect current branch */
		if [ -n "${1}" ]; then
			DEFAULT_BRANCH="${1}" #/* use default branch from build script */
		else
			DEFAULT_BRANCH="$(SUSUWU_DEFAULT_BRANCH "${1}")" #/* detect default branch */
		fi
		if [ "${DEFAULT_BRANCH}" = "${THIS_BRANCH}" ]; then
			SUSUWU_PRINT "SUSUWU_PRODUCTION_USE()" "$(SUSUWU_SH_NOTICE)" "$(SUSUWU_SH_QUOTE "CODE" "git branch") is \"$(SUSUWU_SH_QUOTE "CURRENT" "${THIS_BRANCH}")\"."
		else
			SUSUWU_PRINT "SUSUWU_PRODUCTION_USE()" "$(SUSUWU_SH_WARNING)" "$(SUSUWU_SH_QUOTE "CODE" "git branch") is \"$(SUSUWU_SH_QUOTE "CURRENT" "${THIS_BRANCH}")\"; for production use, execute $(SUSUWU_SH_QUOTE "CODE" "git switch $(SUSUWU_SH_QUOTE "PROPOSED" "${DEFAULT_BRANCH}")")."
		fi
	fi
)

SUSUWU_TEST_BASH() ( #/* Usage: `s/exit ${STATUS}/${STATUS} && SUSUWU_TEST_BASH && STATUS=$?; exit ${STATUS}/` */
	if command -v "bash" >/dev/null && [ -z "${BASH_VERSION}" ]; then #/* If system has `bash`, && this is not an infinite loop (`${0}` not executed though `bash`) ...
		BASH_PATH="${0}.bash" # * ... , path for `bash` version of this. */
		SUSUWU_PATH_SHOULD_NOT_EXIST "SUSUWU_TEST_BASH()" "${BASH_PATH}" #/* Stop if user disabled this (or if this crashed on last execution). */
		SUSUWU_PRINT "SUSUWU_TEST_BASH()" "$(SUSUWU_SH_NOTICE)" "Will produce $(SUSUWU_SH_QUOTE "CODE" "${BASH_PATH}") to test $(SUSUWU_SH_QUOTE "CODE" "/bin/bash"). Execute $(SUSUWU_SH_QUOTE "CODE" "touch '${BASH_PATH}'") to disable this."
		cp "${0}" "${BASH_PATH}" || exit 1
		if sed 's|#!/bin/sh$|#!/bin/bash|' -i'' "${BASH_PATH}"; then #/* If produced `/bin/bash` version... */
			#shellcheck disable=SC2016 #/* This is not supposed to "expand". */
			sed 's|^\s*SUSUWU_BUILD_OBJECTS[^#]*|$(SUSUWU_S=true; \0) |' -i'' "${BASH_PATH}" #/* ... , silence subsequent `SUSUWU_BUILD_OBJECTS()` ... */
			#shellcheck disable=SC2016 #/* This is not supposed to "expand". */
			sed 's|^\s*SUSUWU_TEST_OUTPUT[^#]*|$(SUSUWU_S=true; \0) |' -i'' "${BASH_PATH}" #/* ... , silence subsequent `SUSUWU_TEST_OUTPUT()` ... */
			("${BASH_PATH}") #/* ... , `/bin/bash` version. */
		fi
		BASH_STATUS=$?
		rm "${BASH_PATH}"
		if [ 0 -eq ${BASH_STATUS} ]; then
			SUSUWU_PRINT "SUSUWU_TEST_BASH()" "$(SUSUWU_SH_SUCCESS)" "$(SUSUWU_SH_QUOTE "CODE" "${BASH_PATH}") returned status code $(SUSUWU_SH_QUOTE "STATUS" "${BASH_STATUS}")."
		else
			SUSUWU_PRINT "SUSUWU_TEST_BASH()" "$(SUSUWU_SH_ERROR)" "$(SUSUWU_SH_QUOTE "CODE" "${BASH_PATH}") returned status code $(SUSUWU_SH_QUOTE "STATUS" "${BASH_STATUS}")."
		fi
		return ${BASH_STATUS}
	fi
)

