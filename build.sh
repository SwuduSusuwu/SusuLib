#!/bin/sh
#
#/* (C) 2024 Swudu Susuwu, dual licenses: choose [_GPLv2_](./LICENSE_GPLv2) or [_Apache 2_](./LICENSE) (allows all uses).
# * Builds `./c/` and `./cxx/` into `./obj/` and `./bin/`. Usage: "Console flags" from `./README.md#optionssetup`. */
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
SUSUWU_INCLUDE "./sh/Macros.sh" #/* SUSUWU_DEFAULT_BRANCH() SUSUWU_PRINT() SUSUWU_PROCESS_ABORT_ON_FIRST_ERROR() SUSUWU_PROCESS_S() SUSUWU_PROCESS_VERBOSE() SUSUWU_PRODUCTION_USE() SUSUWU_SH_* */
SUSUWU_INCLUDE "./sh/make.sh" #/* SUSUWU_BUILD_CTAGS SUSUWU_BUILD_OBJECTS() SUSUWU_BUILD_EXECUTABLE() SUSUWU_DEPENDENCY_INCLUDE() SUSUWU_INSTALL() SUSUWU_PROCESS_CLEAN_REBUILD() SUSUWU_PROCESS_MINGW() SUSUWU_PROCESS_RELEASE_DEBUG() SUSUWU_SETUP_BUILD_FLAGS() SUSUWU_SETUP_CXX() SUSUWU_SETUP_BINDIR() SUSUWU_SETUP_OBJDIR() SUSUWU_SETUP_OUTPUT() SUSUWU_TEST_BASH() SUSUWU_TEST_OUTPUT() SUSUWU_UNINSTALL() */
SUSUWU_PRINT "$(SUSUWU_SH_NOTICE)" "(C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses."

THIS_DEFAULT_BRANCH="$(SUSUWU_DEFAULT_BRANCH "trunk")"
SUSUWU_PRINT "$(SUSUWU_SH_WARNING)" "$(SUSUWU_SH_QUOTE "CODE" "git branch") is \"$(SUSUWU_SH_QUOTE "CURRENT" "experimental")\" (which is unstable & sets $(SUSUWU_SH_QUOTE "CODE" "-DSUSUWU_EXPERIMENTAL")); for production use, execute $(SUSUWU_SH_QUOTE "CODE" "git switch $(SUSUWU_SH_QUOTE "PROPOSED" "${THIS_DEFAULT_BRANCH}")")."
export FLAGS_USER="-DSUSUWU_EXPERIMENTAL -DSUSUWU_DEFAULT_BRANCH=\"${THIS_DEFAULT_BRANCH}\"" #/* Usage: "Macro flags" from `./README.md#optionssetup`. */
export FLAGS_ANALYSIS="-Wall -Wno-unused-function -Wno-unused-function -Wextra -Wno-unused-parameter -Wno-ignored-qualifiers -Wpedantic" #/*TODO: -`-Wno-*`, +`-Werror` */
export FLAGS_RELEASE="-fomit-frame-pointer -DNDEBUG -O2" #/* without frame pointer (pointer used for stacktraces), without `assert(...)`/`SUSUWU_DEBUG(...)`/`SUSUWU_NOTICE(...)`, with optimization level 2 */
export CXXFLAGS_DEBUG="-std=c++11" #/* ensure unit tests pass with C++11 support as max */
export FLAGS_DEBUG="-g -Og" #/* in MSVC is `/Zi /Od`: symbols for `gdb`/`lldb` use, optimizations compatible with `-g`/`-fsan*` */
export FLAGS_DEBUG="${FLAGS_DEBUG} -fno-omit-frame-pointer" #/* thus optimization won't remove stacktraces: https://stackoverflow.com/questions/48234575/g-will-fno-omit-frame-pointer-be-effective-if-specified-before-o2-or-o3 https://clang.llvm.org/docs/MemorySanitizer.html */
#export FLAGS_DEBUG="${FLAGS_DEBUG} -fno-optimize-sibling-calls" #/* Don't inline functions. Does extra stacktraces. */
export FLAGS_FSAN="-fsanitize=address -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment"
#export FLAGS_FSAN="${FLAGS_FSAN} -fsanitize=undefined" #/* causes 'cannot locate symbol "__ubsan_handle_function_type_mismatch_abort"' */
export FLAGS_TENSORFLOW="-std=c++17 -DSUSUWU_USE_TENSORFLOW" #/* `./cxx/*` uses `#ifdef SUSUWU_USE_TENSORFLOW`, TensorFlow requires C++17 (for `std::optional`) */
export FLAGS_TENSORFLOW_RELEASE="export TF_MLIR_ENABLE_V1_OPTIMIZATION_PASS=true" #/* TODO */
C_SOURCE_PATH="./c/" #/* Usage: replace with directory root for _C_ source code */
CXX_SOURCE_PATH="./cxx/" #/* Usage: replace with directory root for _C++_ source code */

if [ -n "${GITHUB_ACTIONS}" ]; then
	SUSUWU_IS_VIRTUAL=true #/* `build.sh` is executed through [GitHub Workflows](https://docs.github.com/en/actions/writing-workflows/about-workflows). TODO: `|| <test for other amnesiac environments, such as Docker>` */
else
	SUSUWU_IS_VIRTUAL=false
fi
SUSUWU_INSTALL_TENSORFLOW="${SUSUWU_INSTALL_TENSORFLOW:-"${SUSUWU_IS_VIRTUAL}"}" #/* If virtual, install prerequisites for `cxx/ClassTensorFlowCns.hxx`; use `export SUSUWU_INSTALL_TENSORFLOW=false` to reduce resource use, or `export SUSUWU_INSTALL_TENSORFLOW=true` to install prerequisites on all computers (default is to avoid changes to system unless virtual) */
if [ -f "${0}.bash" ] && [ -n "${BASH_VERSION}" ] || [ "${0##*.}" = "bash" ]; then #/* Notice: assumes left-associative */
	SUSUWU_INSTALL_TENSORFLOW=false #/* `SUSUWU_TEST_BASH` should not reinstall. TODO: ensure compatible (does not prevent install) with ports to `/bin/bash` */
fi
FLAGS_USER_BACKUP="${FLAGS_USER}" #/* Allows to undo insertion of TensorFlow includes */
if [ true = "${SUSUWU_INSTALL_TENSORFLOW}" ]; then
	SUSUWU_PRINT "$0" "$(SUSUWU_SH_NOTICE)" "Was executed through one of GitHub's Workflows (or user set $(SUSUWU_SH_QUOTE "VAR" "SUSUWU_INSTALL_TENSORFLOW")), will auto-install $(SUSUWU_SH_QUOTE "CODE" "libeigen3-dev") and $(SUSUWU_SH_QUOTE "CODE" "libtensorflow")."
	if ! (sudo apt -y install libtensorflow || sudo apt -y install libtensorflow-dev #|| git clone https://github.com/tensorflow/tensorflow.git --depth 1
		); then #/* If normal `apt` is not sufficient to install `libtensorflow` */
		if [ true = "${USE_GOOGLEAPIS_TENSORFLOW}" ]; then
			LIBTENSORFLOW_TAR="libtensorflow-cpu-linux-x86_64-2.11.0.tar.gz"
			wget --no-verbose "https://storage.googleapis.com/tensorflow/libtensorflow/${LIBTENSORFLOW_TAR}" && \
			tar xzf "${LIBTENSORFLOW_TAR}" && \
			ls -a include && ls -a include/tensorflow && ls -a include/tensorflow/core && ls -a include/tensorflow/third-party && \
			sudo mv lib/* /usr/lib/ #&& \
#			sudo mv include/* /usr/include/ && \
#			ls /usr/include/tensorflow/
			git clone https://github.com/openxla/xla.git --depth 1 #/* `libtensorflow` does not include `xla` */
		elif [ true = "${SUSUWU_BUILD_TENSORFLOW}" ]; then #/* prepackaged `libtensorflow` does not have C++ headers; use shallow clone of TensorFlow source */
			git clone https://github.com/tensorflow/tensorflow.git --depth 1
			command -v bazel || sudo apt install bazel || {
				curl -LO "https://github.com/bazelbuild/bazelisk/releases/latest/download/bazelisk-linux-amd64"
				mkdir -p "${GITHUB_WORKSPACE}/bin/"
				mv bazelisk-linux-amd64 "${GITHUB_WORKSPACE}/bin/bazel"
				chmod +x "${GITHUB_WORKSPACE}/bin/bazel"
				PATH="${PATH} ${GITHUB_WORKSPACE}/bin"
			}
			SUSUWU_PWD_BACKUP="$(pwd)"
			command -v bazel && cd ./tensorflow/third_party/xla/third_party/eigen3/ && {
				bazel build
				cd "${SUSUWU_PWD_BACKUP}" || exit 1
			}
		else #/* `libtensorflow` C++ package */
			wget --no-verbose "https://github.com/ika-rwth-aachen/libtensorflow_cc/releases/download/v2.13.0/libtensorflow-cc_2.13.0_$(dpkg --print-architecture).deb"
			sudo dpkg -i "libtensorflow-cc_2.13.0_$(dpkg --print-architecture).deb"
			sudo ldconfig
		fi
	fi
fi

if SUSUWU_DEPENDENCY_INCLUDE "-I" "libtensorflow" "./" "tensorflow/core/" "sudo apt install libtensorflow"; then
	TENSORFLOW_PATH_PREFIX=""
elif SUSUWU_DEPENDENCY_INCLUDE "-I" "tensorflow" "tensorflow/" "tensorflow/core/" "git clone https://github.com/tensorflow/tensorflow.git --depth 1"; then
	TENSORFLOW_PATH_PREFIX="tensorflow/third_party/"
	TENSORFLOW_FULL_PATH_PREFIX="${SUSUWU_DEPENDENCY_INCLUDE_PATH}third_party/"
fi
TENSORFLOW_INCLUDE_PATH="${SUSUWU_DEPENDENCY_INCLUDE_PATH}" #/* `TENSORFLOW_INCLUDE_PATH=$(SUSUWU_DEPENDENCY_INCLUDE ...)` discards `SUSUWU_DEPENDENCY_INCLUDE`'s changes to env vars */
TENSORFLOW_FULL_PATH_PREFIX="${TENSORFLOW_INCLUDE_PATH}third_party/"
if [ -n "${TENSORFLOW_INCLUDE_PATH}" ]; then
	SUSUWU_DEPENDENCY_INCLUDE "-I" "tensorflow:xla" "${TENSORFLOW_PATH_PREFIX}xla/" "xla/" "https://github.com/openxla/xla.git --depth 1" && {
		XLA_SOURCE_PATH="${SUSUWU_DEPENDENCY_INCLUDE_PATH}"
		XLA_PATH_PREFIX="${TENSORFLOW_PATH_PREFIX}xla/third_party/"
		XLA_FULL_PATH_PREFIX="${TENSORFLOW_FULL_PATH_PREFIX}xla/third_party/"
		SUSUWU_DEPENDENCY_INCLUDE "-I" "tensorflow:xla:eigen" "${XLA_PATH_PREFIX}eigen3/" "Eigen/" "cd ${XLA_FULL_PATH_PREFIX}eigen3/ && bazel build"
		EIGEN_INCLUDE_PATH="${SUSUWU_DEPENDENCY_INCLUDE_PATH}"
	}
	SUSUWU_DEPENDENCY_INCLUDE "-I" "tensorflow:tsl" "${TENSORFLOW_PATH_PREFIX}xla/third_party/tsl/" "tsl/" ""
	if [ -z "${EIGEN_INCLUDE_PATH}" ]; then
		SUSUWU_DEPENDENCY_INCLUDE "-I" "eigen" "eigen3/" "unsupported/Eigen/" "sudo apt -y install libeigen3-dev eigen" ||
			SUSUWU_DEPENDENCY_INCLUDE "-I" "eigen" "eigen3/" "Eigen/" "git clone https://github.com/PX4/eigen.git --depth 1"
		if [ -z "${SUSUWU_DEPENDENCY_INCLUDE_PATH}" ] &&
			[ true = "${SUSUWU_INSTALL_TENSORFLOW}" ]; then
			if sudo apt -y install libeigen3-dev || sudo apt -y install eigen || git clone https://github.com/PX4/eigen.git --depth 1; then
				SUSUWU_DEPENDENCY_INCLUDE "-I" "eigen" "eigen3/" "unsupported/Eigen/" "" ||
					SUSUWU_DEPENDENCY_INCLUDE "-I" "eigen" "eigen3/" "Eigen/" ""
			fi
		fi
	fi
	ML_DTYPES_ROOT="xla/third_party/py/ml_dtypes/"
	ML_DTYPES_PATH="${TENSORFLOW_PATH_PREFIX}${ML_DTYPES_ROOT}"
	ML_DTYPES_PREFIX="ml_dtypes/include/"
	ML_DTYPES_FULL_PATH="${TENSORFLOW_FULL_PATH_PREFIX}${ML_DTYPES_ROOT}"
#	if [ -e "../${ML_DTYPES_PATH}${ML_DTYPES_PREFIX}float8.h" ]; then
#		echo "Found: ../${ML_DTYPES_PATH}${ML_DTYPES_PREFIX}float8.h"
#	fi
	if ! SUSUWU_DEPENDENCY_INCLUDE "-I" "tensorflow:ml_dtypes" "${ML_DTYPES_PATH}" "${ML_DTYPES_PREFIX}float8.h" "cd ${ML_DTYPES_FULL_PATH} && bazel build"; then #/* If can't use `ml_dtypes` from `tensorflow` */
		ML_DTYPES_GIT="https://github.com/jax-ml/ml_dtypes.git"
		if [ ! -d "ml_dtypes" ] && [ true = "${SUSUWU_INSTALL_TENSORFLOW}" ]; then
			ML_DTYPES_COMMIT_HASH="00d98cd92ade342fef589c0470379abb27baebe9" #/* TODO: extract compatible commit hash from `workspace.bzl` */
			if [ -z "${ML_DTYPES_COMMIT_HASH}" ]; then
				git clone "${ML_DTYPES_GIT}" --depth 1 #/* Possible mismatch if commit is too new */
			else
				if git init ml_dtypes && cd ml_dtypes; then
					git remote add origin ${ML_DTYPES_GIT}
					git fetch --depth 1 origin ${ML_DTYPES_COMMIT_HASH}
					git checkout FETCH_HEAD
					cd ../
				fi
			fi
		fi
		if ! SUSUWU_DEPENDENCY_INCLUDE "-I" "jax-ml:ml_dtypes" "ml_dtypes/" "${ML_DTYPES_PREFIX}float8.h" "git clone ${ML_DTYPES_GIT} --depth 1"; then #/* If can't use `ml_dtypes` from `jax-ml` */
			ML_DTYPES_FALLBACK_PREFIX="tensorflow/core/platform/"
			ML_DTYPES_FALLBACK="${TENSORFLOW_INCLUDE_PATH}${ML_DTYPES_FALLBACK_PREFIX}"
			if [ -e "${ML_DTYPES_FALLBACK}float8.h" ]; then # If fallback path has `float8.h`
				SUSUWU_PRINT "$0" "$(SUSUWU_SH_WARNING)" "As last resort, will use $(SUSUWU_SH_QUOTE "PATH" "${ML_DTYPES_FALLBACK}") (which has $(SUSUWU_SH_QUOTE "PATH" "float8.h")) as path for $(SUSUWU_SH_QUOTE "CODE" "ml_dtypes"). If this causes more $(SUSUWU_SH_QUOTE "CODE" "#include") errors, execute $(SUSUWU_SH_QUOTE "CODE" "cd ${XLA_SOURCE_PATH} && git reset --hard HEAD") or $(SUSUWU_SH_QUOTE "CODE" "find \"${XLA_SOURCE_PATH}\" -type f -exec sed \"s|\\\"${ML_DTYPES_PREFIX}|\\\"${ML_DTYPES_FALLBACK_PREFIX}|\" -i'' {} +") to undo."
				find "${XLA_SOURCE_PATH}" -type f -exec sed "s|\"${ML_DTYPES_PREFIX}|\"${ML_DTYPES_FALLBACK_PREFIX}|" -i'' {} + # [error: 'ml_dtypes/include/float8.h' file not found](https://github.com/tensorflow/tensorflow/issues/93130) fix. #/* TODO: filter with `grep "\.\(h\|cc\)$"` */
				export SUSUWU_USED_ML_DTYPES_SED=true
			fi
		fi
	fi
fi

SUSUWU_PROCESS_S $@ #/* Usage: `./build.sh -q`. Silences `SUSUWU_SH_NOTICE` ("Notice:") messages, prevents `set -x`. */
SUSUWU_PROCESS_VERBOSE $@ #/* Usage: `./build.sh --verbose`. Enables `SUSUWU_SH_DEBUG` ("Debug:") messages, forces `set -x`. */
SUSUWU_PRODUCTION_USE "${THIS_DEFAULT_BRANCH}"
SUSUWU_PROCESS_MINGW "$@" #/* Usage: `apt install mingw wine && ./build.sh --mingw`. [MinGW cross-builds to Windows.] */
SUSUWU_SETUP_CXX #/* Analogous to `make config` */
SUSUWU_PROCESS_RELEASE_DEBUG "$@" #/* Usage: `./build.sh --debug` or `./build.sh --release` */
SUSUWU_PROCESS_ABORT_ON_FIRST_ERROR "$@" #/* Usage: `./build.sh --abort-on-first-error`. If an object fails to build, `exit 1`. */

SUSUWU_SETUP_OBJDIR "./obj/" #/* Usage: replace with directory root for new objects */
SUSUWU_SETUP_BINDIR "./bin/" #/* Usage: replace with directory root for new executables */
SUSUWU_SETUP_OUTPUT "Susuwu" #/* Usage: replace with name of your program */
SUSUWU_PROCESS_CLEAN_REBUILD "$@" #/* Usage: `./build.sh --clean` or `./build.sh --rebuild` */

CFLAGS_BACKUP="${CFLAGS}" #/* Allows to undo insertion of TensorFlow includes */
CXXFLAGS_BACKUP="${CXXFLAGS}" #/* Allows to undo insertion of TensorFlow includes */
SUSUWU_SETUP_BUILD_FLAGS #/* Analogous to `make config` */
if [ -n "${TENSORFLOW_INCLUDE_PATH}" ] && ! [ true = "${SUSUWU_TENSORFLOW_ERROR}" ]; then #/* If `libtensorflow` was found */
	SUSUWU_TENSORFLOW_TEST_PATH="${CXX_SOURCE_PATH}ClassTensorFlowCns.hxx"
#shellcheck disable=SC2086 #`"${CXXFLAGS}"` gives "clang++: error: language not recognized"
	if [ true = "${SUSUWU_TENSORFLOW_PASS}" ] || (${CXX} ${CXXFLAGS} ${FLAGS_TENSORFLOW} -c "${SUSUWU_TENSORFLOW_TEST_PATH}"); then #/* TODO: ` 2>/dev/null; then` as soon as difficult-to-parse errors such as `fatal error: "unsupported/Eigen/CXX11/Tensor' file not found" have solutions */
		export SUSUWU_TENSORFLOW_PASS=true
		FLAGS_USER="${FLAGS_USER} ${FLAGS_TENSORFLOW}" #/* TODO; if sure `FLAGS_USER` has no use after `SUSUWU_SETUP_BUILD_FLAGS()`, remove. */
		CXXFLAGS="${CXXFLAGS} ${FLAGS_TENSORFLOW}"
		SUSUWU_PRINT "$0" "$(SUSUWU_SH_NOTICE)" "$(SUSUWU_SH_QUOTE "CODE" "${CXX} ${SUSUWU_TENSORFLOW_TEST_PATH}") passed, will enable $(SUSUWU_SH_QUOTE "CODE" "CXXFLAGS=\"\${CXXFLAGS} ${FLAGS_TENSORFLOW}\"")."
	else
		export SUSUWU_TENSORFLOW_ERROR=true
		FLAGS_USER="${FLAGS_USER_BACKUP}" #/* Undo insertion of TensorFlow includes */
		CFLAGS="${CFLAGS_BACKUP}" #/* Undo insertion of TensorFlow includes */
		CXXFLAGS="${CXXFLAGS_BACKUP}" #/* Undo insertion of TensorFlow includes */
		SUSUWU_SETUP_BUILD_FLAGS #/* Analogous to `make config` */
		SUSUWU_PRINT "$0" "$(SUSUWU_SH_NOTICE)" "$(SUSUWU_SH_QUOTE "CODE" "${CXX} ${CXXFLAGS} ${SUSUWU_TENSORFLOW_TEST_PATH}") failed, will not enable $(SUSUWU_SH_QUOTE "CODE" "CXXFLAGS=\"\${CXXFLAGS} ${FLAGS_TENSORFLOW}\"") (skipped). If $(SUSUWU_SH_QUOTE "CODE" "libtensorflow") is installed, insert $(SUSUWU_SH_QUOTE "CODE" "${FLAGS_TENSORFLOW}") into $(SUSUWU_SH_QUOTE "CODE" "$0:FLAGS_USER"). To troubleshoot, use $(SUSUWU_SH_QUOTE "CODE" "cd ${TENSORFLOW_INCLUDE_PATH} && ./configure")"
#		${SUSUWU_USED_ML_DTYPES_SED} && find "${XLA_SOURCE_PATH}" -type f -exec sed "s|\"${ML_DTYPES_FALLBACK_PREFIX}|\"${ML_DTYPES_PREFIX}|" -i'' {} + # [error: 'ml_dtypes/include/float8.h' file not found](https://github.com/tensorflow/tensorflow/issues/93130) fix. #TODO: exclude 'third_party/xla/xla/tsl/platform/resource_loader.h'
	fi
fi
SUSUWU_PROCESS_INCLUDES "${CXX_SOURCE_PATH}Class*.hxx" "${CXX_SOURCE_PATH}Macros.hxx"
#shellcheck disable=SC2119 #Specifics were removed from `SUSUWU_BUILD_CTAGS` call to match `./hooks/pre-commit`.
SUSUWU_BUILD_CTAGS #/* Usage: `apt install ctags vim && vim -t tagToSearchFor` */
SUSUWU_BUILD_OBJECTS "${CC}" "${CFLAGS}" ".c" "${C_SOURCE_PATH}rfc6234/sha1.c" "${C_SOURCE_PATH}rfc6234/sha224-256.c" "${C_SOURCE_PATH}rfc6234/sha384-512.c"
SUSUWU_BUILD_OBJECTS "${CXX}" "${CXXFLAGS}" ".cxx" "${CXX_SOURCE_PATH}*.cxx"
SUSUWU_BUILD_EXECUTABLE
SUSUWU_STATUS=$?
SUSUWU_BUILD_STATUS=${SUSUWU_STATUS}
export TF_CPP_MIN_LOG_LEVEL=0 #/* Prints debug info to `stderr` */
SUSUWU_TEST_OUTPUT #/* Analogous to `make test` or `make execute` */
SUSUWU_STATUS=$?
[ 0 -eq ${SUSUWU_BUILD_STATUS} ] && SUSUWU_INSTALL "${USRBIN}" && SUSUWU_UNINSTALL "${USRBIN}" #/* Analogous to `make install && make uninstall`. Won't clobber files which exist. */
[ 0 -eq ${SUSUWU_STATUS} ] && {
	SUSUWU_TEST_BASH || SUSUWU_STATUS=$?
}
exit ${SUSUWU_STATUS}

