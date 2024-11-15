#!/bin/sh
. ./Macros.sh #/* SUSUWU_DIR_SUFFIX_SLASH, SUSUWU_DIR_AFFIX_DOTSLASH, SUSUWU_PRINT, SUSUWU_SH_* */
SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "Dual licenses: choose \"Creative Commons\" or \"Apache 2\" (allows all uses)."

if command -v ctags > /dev/null; then
	ctags -R
fi

CXXFLAGS_ANALYSIS="-Wall -Wno-unused -Wno-unused-function -Wextra -Wno-unused-parameter -Wno-ignored-qualifiers" #/*TODO: -`-Wno-*`, +`-Wpedantic`, +`-Werror` */
CXXFLAGS_RELEASE="-fomit-frame-pointer -DNDEBUG -O2" #/* without frame pointer (pointer used for stacktraces), without `assert(...)`/`SUSUWU_DEBUG(...)`/`SUSUWU_NOTICE(...)`, with optimization level 2 */
CXXFLAGS_DEBUG="-g -Og" #/* in MSVC is `/Zi /Od`: symbols for `gdb`/`lldb` use, optimizations compatible with `-g`/`-fsan*` */
CXXFLAGS_DEBUG="${CXXFLAGS_DEBUG} -fno-omit-frame-pointer" #/* thus optimization won't remove stacktraces: https://stackoverflow.com/questions/48234575/g-will-fno-omit-frame-pointer-be-effective-if-specified-before-o2-or-o3 https://clang.llvm.org/docs/MemorySanitizer.html */
#CXXFLAGS_DEBUG="${CXXFLAGS_DEBUG} -fno-optimize-sibling-calls" #/* Don't inline functions. Does extra stacktraces. */
CXXFLAGS_FSAN="-fsanitize=address -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment"
#CXXFLAGS_FSAN="${CXXFLAGS_FSAN} -fsanitize=undefined" #/* causes 'cannot locate symbol "__ubsan_handle_function_type_mismatch_abort"' */

CROSS_COMP=""
if [ "--mingw" = "${1}" -o  "--mingw" = "${2}" ]; then
	CROSS_COMP=" --mingw"
	LDFLAGS="${LDFLAGS} -static-libgcc -static-libstdc++"
	if command -v x86_64-w64-mingw32-clang++ > /dev/null; then
		CXX="x86_64-w64-mingw32-clang++"
		CXXFLAGS_DEBUG="${CXXFLAGS_DEBUG} ${CXXFLAGS_FSAN}" #/* `-fsan*` [supports `x86_64-w64-mingw32-clang++`](https://github.com/SwuduSusuwu/SubStack/issues/16) */
	elif command -v x86_64-w64-mingw32-g++ > /dev/null; then
		CXX="x86_64-w64-mingw32-g++"
#		CXXFLAGS_DEBUG="${CXXFLAGS_DEBUG} ${CXXFLAGS_FSAN}" #/* `TODO: `-fsan*` for `x86_64-w64-mingw32-g++`](https://www.mingw-w64.org/contribute/#thorough-status-report-for-sanitizers-asan-tsan-usan) */
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

if [ "--release" = "${1}" -o  "--release" = "${2}" ]; then
	SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "\`${0}${CROSS_COMP} --release\` does not support profilers/debuggers (use \`${0}${CROSS_COMP} --debug\` for this)."
	CXXFLAGS="${CXXFLAGS} ${CXXFLAGS_RELEASE}"
else
	if [ "--debug" != "${1}" ] && [ "--debug" != "${2}" ]; then
		SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "\`${0}${CROSS_COMP}\` defaults to \`${0}${CROSS_COMP} --debug\`."
	fi
	SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "Use \`${0}${CROSS_COMP} --release\` to improve how fast this executes."
	CXXFLAGS="${CXXFLAGS} ${CXXFLAGS_DEBUG}"
	export ASAN_OPTIONS=abort_on_error=1:fast_unwind_on_malloc=0:detect_leaks=0 UBSAN_OPTIONS=print_stacktrace=1 #/* "For LLDB/GDB and to prevent very short stack traces and usually false leaks detection" */
fi

LD="${CXX}"
CC="${CXX} -x c"
LDFLAGS="${LDFLAGS} ${CXXFLAGS}"
CXXFLAGS="${CXXFLAGS} ${CXXFLAGS_ANALYSIS}"
CCFLAGS="${CCFLAGS} ${CXXFLAGS}"
C_SOURCE_PATH="./c/"
CXX_SOURCE_PATH="./cxx/"

if [ -z ${CROSS_COMP} ]; then
	OUTPUT="a.out"
else
	OUTPUT="a.exe"
fi
if [ -z ${OBJDIR} ]; then
	OBJDIR="./obj/"
	SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "To redirect \`${CXX} -c ... -o \${OBJDIR}\${OBJ}.o\` (which has \`OBJDIR=\"${OBJDIR}\"\`), execute \`export OBJDIR=\"./obj/\"\` (where \"./obj/\" is a directory which you choose)."
else
	SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "\`${CXX} -c ... -o \${OBJDIR}\` inherits local \`OBJDIR=\"${OBJDIR}\"\` until you execute \`unset OBJDIR\`."
fi
if [ -z ${BINDIR} ]; then
	BINDIR="./bin/"
	SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "To redirect \`${LD} ... -o \${BINDIR}${OUTPUT}\` (which has \`BINDIR=\"${BINDIR}\"\`), execute \`export BINDIR=\"./bin/\"\` (where \"./bin/\" is a directory which you choose)."
else
	SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "\`${LD} ... -o \${BINDIR}${OUTPUT}\` inherits local \`BINDIR=\"${BINDIR}\"\` until you execute \`unset BINDIR\`."
fi
mkdir -p "${OBJDIR}" "${BINDIR}"
if [ "--rebuild" = "${1}" -o  "--rebuild" = "${2}" ]; then
	SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "Was called with \`${0}${CROSS_COMP} --rebuild\`, will remove intermediate objects+ continue."
	rm ${OBJDIR}*.o
fi
if [ "--clean" = "${1}" -o  "--clean" = "${2}" ]; then
	SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "Was called with \`${0}${CROSS_COMP} --clean\`, will remove intermediate objects + exit. Use \`${0}${CROSS_COMP} --rebuild\` to clean + continue."
	rm ${OBJDIR}*.o
	exit 0
fi

C_SOURCE_PATH=$(SUSUWU_DIR_SUFFIX_SLASH "${C_SOURCE_PATH}") #/* if inherit C_SOURCE_PATH, perhaps it lacks '/' */
CXX_SOURCE_PATH=$(SUSUWU_DIR_SUFFIX_SLASH "${CXX_SOURCE_PATH}") #/* if inherit CXX_SOURCE_PATH, perhaps it lacks '/' */
OBJDIR=$(SUSUWU_DIR_SUFFIX_SLASH "${OBJDIR}") #/* if inherit OBJDIR, perhaps it is without last '/' */
BINDIR=$(SUSUWU_DIR_SUFFIX_SLASH "${BINDIR}") #/* if inherit BINDIR, perhaps it is without last '/' */
if [ -e ${BINDIR}${OUTPUT} ]; then
	BUILDNEW=false
else
	BUILDNEW=true
fi
for SOURCE in ${CXX_SOURCE_PATH}Class*.hxx ${CXX_SOURCE_PATH}Macros.hxx; do
	OBJECT="${OBJDIR}$(basename ${SOURCE} .hxx).o" #/* `basename`'s second param removes suffix */
	SRCCXX="${CXX_SOURCE_PATH}$(basename ${SOURCE} .hxx).cxx" #/* `basename`'s second param removes suffix */
	if [ ${OBJECT} -ot ${SOURCE} -a -e ${SOURCE} ]; then #/* `&& [ -e ${SOURCE} ]` is: skip unless `${SOURCE}` exists. */
		SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "\`${SOURCE}\` is newer than \`${OBJECT}\`, will rebuild all objects."
		rm ${OBJDIR}*.o
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
	BINDIR=$(SUSUWU_DIR_AFFIX_DOTSLASH "${BINDIR}")
	${BINDIR}${OUTPUT}
	STATUS=$?
	if [ 0 -eq ${STATUS} ]; then
		SUSUWU_PRINT "${SUSUWU_SH_SUCCESS}" "\`${BINDIR}${OUTPUT}\` returned status SusuwuUnitTestsBitmask(${STATUS})."
	else
		SUSUWU_PRINT "${SUSUWU_SH_ERROR}" "\`${BINDIR}${OUTPUT}\` returned status SusuwuUnitTestsBitmask(${STATUS})."
	fi
fi
return ${STATUS}

