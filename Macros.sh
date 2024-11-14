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

