#!/bin/sh
. ./Macros.sh
SUSUWU_PRINT "${SUSUWU_SH_NOTICE}" "Dual licenses: choose \"Creative Commons\" or \"Apache 2\" (allows all uses)."

if command -v ctags > /dev/null; then
	ctags -R
fi

CXXFLAGS_ANALYSIS="-Wall -Wno-unused -Wno-unused-function -Wextra -Wno-unused-parameter -Wno-ignored-qualifiers" #TODO: -`-Wno-*, +`-Wpedantic`, +`-Werror``
CXXFLAGS_RELEASE="-fomit-frame-pointer -DNDEBUG -O2" #/* without frame pointer (pointer used for stacktraces), without `assert(...)`/`SUSUWU_DEBUG(...)`/`SUSUWU_NOTICE(...)`, with optimization level 2 */
CXXFLAGS_DEBUG="-g -Og" #/* in MSVC is `/Zi /Od`: symbols for `gdb`/`lldb` use, optimizations compatible with `-g`/`-fsan*` */
CXXFLAGS_DEBUG="${CXXFLAGS_DEBUG} -fno-omit-frame-pointer" #/* thus optimization won't remove stacktraces: https://stackoverflow.com/questions/48234575/g-will-fno-omit-frame-pointer-be-effective-if-specified-before-o2-or-o3 https://clang.llvm.org/docs/MemorySanitizer.html */
#CXXFLAGS_DEBUG="${CXXFLAGS_DEBUG} -fno-optimize-sibling-calls" #/* Don't inline functions. Does extra stacktraces. */
CXXFLAGS_FSAN="-fsanitize=address -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment"
#CXXFLAGS_FSAN="${CXXFLAGS_FSAN} -fsanitize=undefined" #/* causes 'cannot locate symbol "__ubsan_handle_function_type_mismatch_abort"' */

CROSS_COMP=""
if [ "--mingw" = "${1}" ] || [ "--mingw" = "${2}" ]; then
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
elif command -v "${CXX}" > /dev/null; then
	SUSUWU_PRINT "${SUSUWU_SH_INFO}" "\`clang++ not found\`, \`g++ not found\`. \`\${CXX}\` (\"${CXX}\") found, will use this."
else
	SUSUWU_PRINT "${SUSUWU_SH_ERROR}" "\`clang++ not found\`, \`g++ not found\`. \`\${CXX}\` (\"${CXX}\") not found. Do \`apt install clang\` or \`apt install gcc\`."
	exit 1
fi

if [ "--release" = "${1}" ] || [ "--release" = "${2}" ]; then
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
set -x
${CC} ${CCFLAGS} -c "${C_SOURCE_PATH}rfc6234/sha1.c"
${CC} ${CCFLAGS} -c "${C_SOURCE_PATH}rfc6234/sha224-256.c"
${CC} ${CCFLAGS} -c "${C_SOURCE_PATH}rfc6234/sha384-512.c"
${CXX} ${CXXFLAGS} -c "${CXX_SOURCE_PATH}Macros.cxx"
${CXX} ${CXXFLAGS} -c "${CXX_SOURCE_PATH}ClassSha2.cxx"
${CXX} ${CXXFLAGS} -c "${CXX_SOURCE_PATH}ClassResultList.cxx"
${CXX} ${CXXFLAGS} -c "${CXX_SOURCE_PATH}ClassSys.cxx"
${CXX} ${CXXFLAGS} -c "${CXX_SOURCE_PATH}ClassCns.cxx"
${CXX} ${CXXFLAGS} -c "${CXX_SOURCE_PATH}VirusAnalysis.cxx"
${CXX} ${CXXFLAGS} -c "${CXX_SOURCE_PATH}AssistantCns.cxx"
${CXX} ${CXXFLAGS} -c "${CXX_SOURCE_PATH}main.cxx"
#/* Order is 2 fold: language of code, plus which source the most `#include` (most include `Macros.hxx`). */
${LD} ${LDFLAGS} "sha1.o" "sha224-256.o" "sha384-512.o" "Macros.o" "ClassSha2.o" "ClassResultList.o" "ClassSys.o" "ClassCns.o" "VirusAnalysis.o" "AssistantCns.o" "main.o"
STATUS=$?
set +x

if [ 0 -eq ${STATUS} ]; then
	if [ -z ${CROSS_COMP} ]; then
		FILE_OUT="a.out"
	else
		FILE_OUT="a.exe"
	fi
	SUSUWU_PRINT "${SUSUWU_SH_SUCCESS}" "produced \`${FILE_OUT}\` (`stat -c%s ${FILE_OUT}` bytes)."
	./${FILE_OUT}
	STATUS=$?
	if [ 0 -eq ${STATUS} ]; then
		SUSUWU_PRINT "${SUSUWU_SH_SUCCESS}" "\`./${FILE_OUT}\` returned status SusuwuUnitTestsBitmask(${STATUS})."
	else
		SUSUWU_PRINT "${SUSUWU_SH_ERROR}" "\`./${FILE_OUT}\` returned status SusuwuUnitTestsBitmask(${STATUS})."
	fi
fi
return ${STATUS}

