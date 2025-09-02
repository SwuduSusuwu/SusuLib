#!/bin/sh
#
# /* Attribution (henceforth "*this attribution*", whose syntax is *Markdown*): 2024 [Swudu Susuwu](https://swudususuwu.substack.com)
#  * <https://github.com/SwuduSusuwu/SusuLib/> has the newest version of `./sh/Transcode.sh` (henceforth "*this source code*").
#  * If *this attribution* is shown, *this source code* allows all uses. *This attribution* constitutes the most permissive which is compatible with [*GPLv2*](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) + [*Apache 2*](https://www.apache.org/licenses/LICENSE-2.0.html), which is suitable for personal use (also suitable for school use). Just source code (or executables) must reproduce **this attribution**.
#  * If *this attribution* is not professional enough for business use: businesses can use *this source code* through included versions of [*GPLv2*](./LICENSE_GPLv2), [*Apache 2*](./LICENSE), or through both of those.
#  */
# /* {Searches for, installs, does unit tests for} system includes (popular includes such as {`libxml2`, `libtensorflow`}). */

export CFLAGS CXXFLAGS LDFLAGS FLAGS_USER

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
#SUSUWU_INCLUDE "./sh/Macros.sh" #/* SUSUWU_PATH_SHOULD_NOT_EXIST */ #uncomment if not included
#SUSUWU_INCLUDE "./sh/make.sh" #/* SUSUWU_DEPENDENCY_INCLUDE SUSUWU_INSTALL_PACKAGES SUSUWU_SETUP_BUILD_FLAGS_CONDITIONAL */ #uncomment if not included

SUSUWU_INCLUDES_LIBPUGIXML() { #/* If can include `libpugixml`, set `-DSUSUWU_USE_PUGIXML` */
	SUSUWU_INSTALL_PACKAGES "libpugixml-dev" "libpugixml1v5" || { #/* For GitHub / Ubuntu */
		SUSUWU_INSTALL_PACKAGES "libpugixml" #/* For Termux */
	}
	FLAGS_USER_BACKUP="${FLAGS_USER}" #/* Allows to undo `-I` insertion */
	if SUSUWU_DEPENDENCY_INCLUDE "-I" "libpugixml-dev" "pugixml/src/" "pugixml.hpp" "C++" "sudo apt install libpugixml-dev libpugixml1v5" && ! [ true = "${SUSUWU_INCLUDES_LIBPUGIXML_ERROR}" ]; then #/* If `libpugixml` was found */
	#	&& ${LD} -llibpugixml #/* /usr/lib/libpugixml.so *?
		SUSUWU_INCLUDES_LIBPUGIXML_TEST_PATH="libpugixmlTest.cxx.tmp"
		if (SUSUWU_PATH_SHOULD_NOT_EXIST "$0" "${SUSUWU_INCLUDES_LIBPUGIXML_TEST_PATH}"); then
			echo "#include <pugixml.hpp>
	int main() { pugi::xml_document doc; }" > "${SUSUWU_INCLUDES_LIBPUGIXML_TEST_PATH}"
	#shellcheck disable=SC2086 #`"${CXXFLAGS}"` gives "clang++: error: language not recognized"
			if [ true = "${SUSUWU_INCLUDES_LIBPUGIXML_PASS}" ] || SUSUWU_SETUP_BUILD_FLAGS_CONDITIONAL "" "-DSUSUWU_USE_PUGIXML=true" "-lpugixml" "${SUSUWU_INCLUDES_LIBPUGIXML_TEST_PATH}"; then
				export SUSUWU_INCLUDES_LIBPUGIXML_PASS=true
				return 0 #/* "success", true */
			else
				export SUSUWU_INCLUDES_LIBPUGIXML_ERROR=true
				FLAGS_USER="${FLAGS_USER_BACKUP}" #/* Undo `-I` insertion */
			fi
			rm "${SUSUWU_INCLUDES_LIBPUGIXML_TEST_PATH}"
		fi
	fi
	return 1 #/* "error", false */
}

