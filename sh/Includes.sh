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

SUSUWU_INCLUDES_LIBXML2() { #/* If can include `libxml2`, set `-DSUSUWU_USE_LIBXML2` */
	SUSUWU_INSTALL_PACKAGES "libxml2-dev" || { #/* For GitHub / Ubuntu */
		SUSUWU_INSTALL_PACKAGES "libxml2" #/* For Termux */
	}
	FLAGS_USER_BACKUP="${FLAGS_USER}" #/* Allows to undo `-I` insertion */
	if SUSUWU_DEPENDENCY_INCLUDE "-I" "libxml2-dev" "libxml2/" "libxml/parser.h" "C" "sudo apt install libxml2-dev libxml2" && ! [ true = "${SUSUWU_INCLUDES_LIBXML2_ERROR}" ]; then #/* If `libxml2` was found */
#	&& ${LD} -lxml2 #/* /usr/lib/libxml2.so *?
		SUSUWU_INCLUDES_LIBXML2_TEST_PATH="libxml2Test.cxx.tmp"
		if (SUSUWU_PATH_SHOULD_NOT_EXIST "$0" "${SUSUWU_INCLUDES_LIBXML2_TEST_PATH}"); then
			echo "#include <libxml/parser.h> /* xmlDocPtr */
	int main() { xmlDocPtr doc; return 0; }" > "${SUSUWU_INCLUDES_LIBXML2_TEST_PATH}"
#shellcheck disable=SC2086 #`"${CXXFLAGS}"` gives "clang++: error: language not recognized"
			if [ true = "${SUSUWU_INCLUDES_LIBXML2_PASS}" ] || SUSUWU_SETUP_BUILD_FLAGS_CONDITIONAL "-DSUSUWU_USE_LIBXML2=true" "-DSUSUWU_USE_LIBXML2=true" "-lxml2" "${SUSUWU_INCLUDES_LIBXML2_TEST_PATH}"; then
				export SUSUWU_INCLUDES_LIBXML2_PASS=true
				return 0 #/* "success", true */
			else
				export SUSUWU_INCLUDES_LIBXML2_ERROR=true
				FLAGS_USER="${FLAGS_USER_BACKUP}" #/* Undo `-I` insertion */
			fi
			rm "${SUSUWU_INCLUDES_LIBXML2_TEST_PATH}"
		fi
	fi
	return 1 #/* "error", false */
}

SUSUWU_INCLUDES_LIBTENSORFLOW() { #/* If can include `libtensorflow`, set `-DSUSUWU_USE_TENSORFLOW` */
	FLAGS_USER_BACKUP="${FLAGS_USER}" #/* Allows to undo `-I` insertions. */
	CXXFLAGS_BACKUP="${CXXFLAGS}" #/* Allows to undo `-I` insertions. */
	CFLAGS_BACKUP="${CFLAGS}" #/* Allows to undo `-I` insertions. */
	LDFLAGS_BACKUP="${LDFLAGS}" #/* Allows to undo insertions. */
	SUSUWU_INCLUDES_LIBTENSORFLOW_FLAGS="-std=c++17 -DSUSUWU_USE_TENSORFLOW" #/* `./cxx/*` uses `#ifdef SUSUWU_USE_TENSORFLOW`, TensorFlow requires C++17 (for `std::optional`) */
#	SUSUWU_INCLUDES_LIBTENSORFLOW_FLAGS_RELEASE="export TF_MLIR_ENABLE_V1_OPTIMIZATION_PASS=true" #/* TODO */

	SUSUWU_INSTALL_TENSORFLOW="${SUSUWU_INSTALL_TENSORFLOW:-"${SUSUWU_IS_VIRTUAL}"}" #/* If virtual, install prerequisites for `cxx/ClassTensorFlowCns.hxx`; use `export SUSUWU_INSTALL_TENSORFLOW=false` to reduce resource use, or `export SUSUWU_INSTALL_TENSORFLOW=true` to install prerequisites on all computers (default is to avoid changes to system unless virtual) */
	if [ -f "${0}.bash" ] && [ -n "${BASH_VERSION}" ] || [ "${0##*.}" = "bash" ]; then #/* Notice: assumes left-associative */
		SUSUWU_INSTALL_TENSORFLOW=false #/* `SUSUWU_TEST_BASH` should not reinstall. TODO: ensure compatible (does not prevent install) with ports to `/bin/bash` */
	fi
	if [ true = "${SUSUWU_INSTALL_TENSORFLOW}" ]; then
		SUSUWU_PRINT "$0" "$(SUSUWU_SH_NOTICE)" "Was executed through one of GitHub's Workflows (or user set $(SUSUWU_SH_QUOTE "VAR" "SUSUWU_INSTALL_TENSORFLOW")), will auto-install $(SUSUWU_SH_QUOTE "CODE" "libeigen3-dev") and $(SUSUWU_SH_QUOTE "CODE" "libtensorflow")."
		if ! (SUSUWU_INSTALL_PACKAGES "libtensorflow" || SUSUWU_INSTALL_PACKAGES "libtensorflow-dev" #|| git clone https://github.com/tensorflow/tensorflow.git --depth 1
			); then #/* If system package manager is not sufficient to install `libtensorflow` */
			if [ true = "${USE_GOOGLEAPIS_TENSORFLOW}" ]; then
				LIBTENSORFLOW_TAR="libtensorflow-cpu-linux-x86_64-2.11.0.tar.gz"
				wget --no-verbose "https://storage.googleapis.com/tensorflow/libtensorflow/${LIBTENSORFLOW_TAR}" && \
				tar xzf "${LIBTENSORFLOW_TAR}" && \
				ls -a include && ls -a include/tensorflow && ls -a include/tensorflow/core && ls -a include/tensorflow/third-party && \
				sudo mv lib/* /usr/lib/ #&& \
#				sudo mv include/* /usr/include/ && \
#				ls /usr/include/tensorflow/
				git clone https://github.com/openxla/xla.git --depth 1 #/* `libtensorflow` does not include `xla` */
			elif [ true = "${SUSUWU_BUILD_TENSORFLOW}" ]; then #/* prepackaged `libtensorflow` does not have C++ headers; use shallow clone of TensorFlow source */
				git clone https://github.com/tensorflow/tensorflow.git --depth 1
				command -v bazel || SUSUWU_INSTALL_PACKAGES "bazel" || {
					curl -LO "https://github.com/bazelbuild/bazelisk/releases/latest/download/bazelisk-linux-amd64"
					mkdir -p "${GITHUB_WORKSPACE}/bin/"
					mv bazelisk-linux-amd64 "${GITHUB_WORKSPACE}/bin/bazel"
					chmod +x "${GITHUB_WORKSPACE}/bin/bazel"
					PATH="${PATH} ${GITHUB_WORKSPACE}/bin"
				}
				SUSUWU_INCLUDES_LIBTENSORFLOW_PWD_BACKUP="$(pwd)"
				command -v bazel && cd ./tensorflow/third_party/xla/third_party/eigen3/ && {
					bazel build
					cd "${SUSUWU_INCLUDES_LIBTENSORFLOW_PWD_BACKUP}" || exit 1
				}
			else #/* `libtensorflow` C++ package */
				wget --no-verbose "https://github.com/ika-rwth-aachen/libtensorflow_cc/releases/download/v2.13.0/libtensorflow-cc_2.13.0_$(dpkg --print-architecture).deb"
				sudo dpkg -i "libtensorflow-cc_2.13.0_$(dpkg --print-architecture).deb"
				sudo ldconfig
			fi
		fi
	fi

	if SUSUWU_DEPENDENCY_INCLUDE "-I" "libtensorflow" "./" "tensorflow/core/" "C++" "sudo apt install libtensorflow"; then
		TENSORFLOW_PATH_PREFIX=""
	elif SUSUWU_DEPENDENCY_INCLUDE "-I" "tensorflow" "tensorflow/" "tensorflow/core/" "C++" "git clone https://github.com/tensorflow/tensorflow.git --depth 1"; then
		TENSORFLOW_PATH_PREFIX="tensorflow/third_party/"
		TENSORFLOW_FULL_PATH_PREFIX="${SUSUWU_DEPENDENCY_INCLUDE_PATH}third_party/"
	fi
	TENSORFLOW_INCLUDE_PATH="${SUSUWU_DEPENDENCY_INCLUDE_PATH}" #/* `TENSORFLOW_INCLUDE_PATH=$(SUSUWU_DEPENDENCY_INCLUDE ...)` discards `SUSUWU_DEPENDENCY_INCLUDE`'s changes to env vars */
	TENSORFLOW_FULL_PATH_PREFIX="${TENSORFLOW_INCLUDE_PATH}third_party/"
	if [ -n "${TENSORFLOW_INCLUDE_PATH}" ]; then
		SUSUWU_DEPENDENCY_INCLUDE "-I" "tensorflow:xla" "${TENSORFLOW_PATH_PREFIX}xla/" "xla/" "C++" "https://github.com/openxla/xla.git --depth 1" && {
			XLA_SOURCE_PATH="${SUSUWU_DEPENDENCY_INCLUDE_PATH}"
			XLA_PATH_PREFIX="${TENSORFLOW_PATH_PREFIX}xla/third_party/"
			XLA_FULL_PATH_PREFIX="${TENSORFLOW_FULL_PATH_PREFIX}xla/third_party/"
			SUSUWU_DEPENDENCY_INCLUDE "-I" "tensorflow:xla:eigen" "${XLA_PATH_PREFIX}eigen3/" "Eigen/" "C++" "cd ${XLA_FULL_PATH_PREFIX}eigen3/ && bazel build"
			EIGEN_INCLUDE_PATH="${SUSUWU_DEPENDENCY_INCLUDE_PATH}"
		}
		SUSUWU_DEPENDENCY_INCLUDE "-I" "tensorflow:tsl" "${TENSORFLOW_PATH_PREFIX}xla/third_party/tsl/" "tsl/" "C++" ""
		if [ -z "${EIGEN_INCLUDE_PATH}" ]; then
			SUSUWU_DEPENDENCY_INCLUDE "-I" "eigen" "eigen3/" "unsupported/Eigen/" "C++" "sudo apt -y install libeigen3-dev eigen" ||
				SUSUWU_DEPENDENCY_INCLUDE "-I" "eigen" "eigen3/" "Eigen/" "C++" "git clone https://github.com/PX4/eigen.git --depth 1"
			if [ -z "${SUSUWU_DEPENDENCY_INCLUDE_PATH}" ] &&
				[ true = "${SUSUWU_INSTALL_TENSORFLOW}" ]; then
				if SUSUWU_INSTALL_PACKAGES "libeigen3-dev" || SUSUWU_INSTALL_PACKAGES "eigen" || git clone https://github.com/PX4/eigen.git --depth 1; then
					SUSUWU_DEPENDENCY_INCLUDE "-I" "eigen" "eigen3/" "unsupported/Eigen/" "C++" "" ||
						SUSUWU_DEPENDENCY_INCLUDE "-I" "eigen" "eigen3/" "Eigen/" "C++" ""
				fi
			fi
		fi
		if [ -z "${ABSEIL_INCLUDE_PATH}" ]; then #/* Allows custom path with `ABSEIL_INCLUDE_PATH=path; ./build.sh` */
			SUSUWU_DEPENDENCY_INCLUDE "-I" "abseil" "absl/" "status/status.h" "C++" "git clone https://github.com/abseil/abseil-cpp.git --depth 1 && sudo apt install libabsl-dev"
			if [ -z "${SUSUWU_DEPENDENCY_INCLUDE_PATH}" ] &&
				[ true = "${SUSUWU_INSTALL_TENSORFLOW}" ]; then
				git clone https://github.com/abseil/abseil-cpp.git --depth 1 && SUSUWU_INSTALL_PACKAGES "libabsl-dev"
				SUSUWU_DEPENDENCY_INCLUDE "-I" "abseil" "absl/" "status/status.h" ""
			fi
		fi
		ML_DTYPES_ROOT="xla/third_party/py/ml_dtypes/"
		ML_DTYPES_PATH="${TENSORFLOW_PATH_PREFIX}${ML_DTYPES_ROOT}"
		ML_DTYPES_FULL_PATH="${TENSORFLOW_FULL_PATH_PREFIX}${ML_DTYPES_ROOT}"
		ML_DTYPES_PREFIX="ml_dtypes/include/"
#		if [ -e "../${ML_DTYPES_PATH}${ML_DTYPES_PREFIX}float8.h" ]; then
#			echo "Found: ../${ML_DTYPES_PATH}${ML_DTYPES_PREFIX}float8.h"
#		fi
		if ! SUSUWU_DEPENDENCY_INCLUDE "-I" "tensorflow:ml_dtypes" "${ML_DTYPES_PATH}" "${ML_DTYPES_PREFIX}float8.h" "C++" "cd ${ML_DTYPES_FULL_PATH} && bazel build"; then #/* If can't use `ml_dtypes` from `tensorflow` */
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
			if ! SUSUWU_DEPENDENCY_INCLUDE "-I" "jax-ml:ml_dtypes" "ml_dtypes/" "${ML_DTYPES_PREFIX}float8.h" "C++" "git clone ${ML_DTYPES_GIT} --depth 1"; then #/* If can't use `ml_dtypes` from `jax-ml` */
				ML_DTYPES_FALLBACK_PREFIX="tensorflow/core/platform/"
				ML_DTYPES_FALLBACK="${TENSORFLOW_INCLUDE_PATH}${ML_DTYPES_FALLBACK_PREFIX}"
				if [ -e "${ML_DTYPES_FALLBACK}float8.h" ]; then # If fallback path has `float8.h`
					SUSUWU_PRINT "$0" "$(SUSUWU_SH_WARNING)" "As last resort, will use $(SUSUWU_SH_QUOTE "PATH" "${ML_DTYPES_FALLBACK}") (which has $(SUSUWU_SH_QUOTE "PATH" "float8.h")) as path for $(SUSUWU_SH_QUOTE "CODE" "ml_dtypes"). If this causes more $(SUSUWU_SH_QUOTE "CODE" "#include") errors, execute $(SUSUWU_SH_QUOTE "CODE" "cd ${XLA_SOURCE_PATH} && git reset --hard HEAD") or $(SUSUWU_SH_QUOTE "CODE" "find \"${XLA_SOURCE_PATH}\" -type f -exec sed \"s|\\\"${ML_DTYPES_PREFIX}|\\\"${ML_DTYPES_FALLBACK_PREFIX}|\" -i'' {} +") to undo."
					find "${XLA_SOURCE_PATH}" -type f -exec sed "s|\"${ML_DTYPES_PREFIX}|\"${ML_DTYPES_FALLBACK_PREFIX}|" -i'' {} + # [error: 'ml_dtypes/include/float8.h' file not found](https://github.com/tensorflow/tensorflow/issues/93130) fix. #/* TODO: filter with `grep "\.\(h\|cc\)$"` */
					export SUSUWU_USED_ML_DTYPES_SED=true
				fi
			fi
		fi

		SUSUWU_INCLUDES_LIBTENSORFLOW_HAS_PROTOS=$(test -f "${TENSORFLOW_INCLUDE_PATH}tensorflow/core/framework/types.pb.h")
		if [ true = "${SUSUWU_INSTALL_TENSORFLOW}" ] && ! ${SUSUWU_INCLUDES_LIBTENSORFLOW_HAS_PROTOS}; then
			if ! command -v protoc >/dev/null; then
				sudo apt install protobuf || sudo snap install protobuf
			fi
			TENSORFLOW_FW="tensorflow/core/framework/"
#			TENSORFLOW_INCLUDE_PATH_FW="${TENSORFLOW_INCLUDE_PATH}${TENSORFLOW_FW}"
#			for PROTO_SOURCE_CODE in "${TENSORFLOW_INCLUDE_PATH}${TENSORFLOW_FW}"*.proto; do
#				if [ ! -e "${PROTO_SOURCE_CODE%proto}pb.h" ]; then
#					protoc --cpp_out="${TENSORFLOW_INCLUDE_PATH_FW}" --proto_path="${TENSORFLOW_INCLUDE_PATH}" --experimental_allow_proto3_optional "${PROTO_SOURCE_CODE##*"${TENSORFLOW_INCLUDE_PATH}"}" #/* "../tensorflow/tensorflow/core/framework/tensor_shape.h:*:10: fatal error: 'tensorflow/core/framework/*.pb.h' file not found" workaround */
			SUSUWU_INCLUDES_LIBTENSORFLOW_PWD_BACKUP="$(pwd)"
			cd "${TENSORFLOW_INCLUDE_PATH}" || exit 1
#			ls "${TENSORFLOW_FW}" | grep ".proto"
#			protoc --cpp_out="${TENSORFLOW_INCLUDE_PATH_FW}" --proto_path="${TENSORFLOW_INCLUDE_PATH}" "${TENSORFLOW_FW}*.proto" #/* "../tensorflow/tensorflow/core/framework/tensor_shape.h:*:10: fatal error: 'tensorflow/core/framework/*.pb.h' file not found" workaround */
#			if [ ! -e "${TENSORFLOW_INCLUDE_PATH_FW}types.pb.h" ]; then
#				protoc --cpp_out="${TENSORFLOW_INCLUDE_PATH_FW}" --proto_path="${TENSORFLOW_INCLUDE_PATH}" "${TENSORFLOW_FW}types.proto" #/* "../tensorflow/tensorflow/core/framework/tensor_shape.h:22:10: fatal error: 'tensorflow/core/framework/types.pb.h' file not found" workaround */
#			fi
#			if [ ! -e "${TENSORFLOW_INCLUDE_PATH_FW}function.pb.h" ]; then
#				protoc --cpp_out="${TENSORFLOW_INCLUDE_PATH_FW}" --proto_path="${TENSORFLOW_INCLUDE_PATH}" "${TENSORFLOW_FW}function.proto" #/* " ../tensorflow/tensorflow/c/eager/abstract_function.h:19:10: fatal error: 'tensorflow/core/framework/function.pb.h' file not found" */
#			fi
			for PROTO_SOURCE_CODE in "${TENSORFLOW_FW}"*.proto; do
				if [ ! -e "${PROTO_SOURCE_CODE%proto}pb.h" ]; then
					protoc --cpp_out="${TENSORFLOW_FW}" --proto_path=./ --experimental_allow_proto3_optional "${PROTO_SOURCE_CODE}" #/* "../tensorflow/tensorflow/core/framework/tensor_shape.h:*:10: fatal error: 'tensorflow/core/framework/*.pb.h' file not found" workaround */
				fi
			done
			if [ -d "${XLA_SOURCE_PATH}" ]; then
				TENSORFLOW_PROTOBUF="xla/tsl/protobuf/"
				cd "${SUSUWU_INCLUDES_LIBTENSORFLOW_PWD_BACKUP}" && cd "${XLA_SOURCE_PATH}" || exit 1
#				ls "${TENSORFLOW_PROTOBUF}" | grep ".proto"
				for PROTO_SOURCE_CODE in "${TENSORFLOW_PROTOBUF}"*.proto; do
					if [ ! -e "${PROTO_SOURCE_CODE%proto}pb.h" ]; then
						protoc --cpp_out="${TENSORFLOW_PROTOBUF}" --proto_path=./ "${PROTO_SOURCE_CODE}" #/* "../tensorflow/third_party/xla/xla/tsl/platform/status.h:38:10: fatal error: 'xla/tsl/protobuf/error_codes.pb.h' file not found" workaround */
					fi
				done
			fi
			cd "${SUSUWU_INCLUDES_LIBTENSORFLOW_PWD_BACKUP}" || exit 1
		fi
	fi

	export TF_CPP_MIN_LOG_LEVEL=0 #/* Prints debug info to `stderr` */
	if [ -n "${TENSORFLOW_INCLUDE_PATH}" ] && ! [ true = "${SUSUWU_INCLUDES_LIBTENSORFLOW_ERROR}" ]; then #/* If `libtensorflow` was found */
		SUSUWU_INCLUDES_LIBTENSORFLOW_TEST_PATH="${CXX_SOURCE_PATH}ClassTensorFlowCns.hxx"
#shellcheck disable=SC2086 #`"${CXXFLAGS}"` gives "clang++: error: language not recognized"
		if [ true = "${SUSUWU_INCLUDES_LIBTENSORFLOW_PASS}" ] || SUSUWU_SETUP_BUILD_FLAGS_CONDITIONAL "" "${SUSUWU_INCLUDES_LIBTENSORFLOW_FLAGS}" "-labsl_status -labsl_strings -labsl_base -ltensorflow_cc -ltensorflow_framework" "${SUSUWU_INCLUDES_LIBTENSORFLOW_TEST_PATH}" "To troubleshoot, use $(SUSUWU_SH_QUOTE "CODE" "cd ${TENSORFLOW_INCLUDE_PATH} && ./configure")"; then
			export SUSUWU_INCLUDES_LIBTENSORFLOW_PASS=true
#			LDFLAGS="${LDFLAGS} -lprotobuf" #/* `-lprotobuf` gives "SUMMARY: AddressSanitizer: SEGV generated_message_reflection.cc in google::protobuf::(anonymous namespace)::AddDescriptorsImpl(google::protobuf::internal::DescriptorTable const*)" */
#			LDFLAGS="${LDFLAGS} -l:libabsl_status.so"
#	if [ -d "/usr/local/lib/" ]; then
#			#LDFLAGS="${LDFLAGS} -L/usr/local/lib"
#		if [ -f /usr/local/lib/libtensorflow_framework.so ] || [ -f /usr/local/lib/libtensorflow_framework.so.2 ]; then
#			ls /usr/local/lib/libtensorflow*
			return 0 #/* "success", true */
		else
			export SUSUWU_INCLUDES_LIBTENSORFLOW_ERROR=true
			FLAGS_USER="${FLAGS_USER_BACKUP}" #/* Undo `-I` insertions. */
			CXXFLAGS="${CXXFLAGS_BACKUP}" #/* Undo `-I` insertions. */
			CFLAGS="${CFLAGS_BACKUP}" #/* Undo `-I` insertions. */
			LDFLAGS="${LDFLAGS_BACKUP}" #/* Undo insertions. */
			SUSUWU_SETUP_BUILD_FLAGS #/* Analogous to `make config` */
#			${SUSUWU_USED_ML_DTYPES_SED} && find "${XLA_SOURCE_PATH}" -type f -exec sed "s|\"${ML_DTYPES_FALLBACK_PREFIX}|\"${ML_DTYPES_PREFIX}|" -i'' {} + # [error: 'ml_dtypes/include/float8.h' file not found](https://github.com/tensorflow/tensorflow/issues/93130) fix. #TODO: exclude 'third_party/xla/xla/tsl/platform/resource_loader.h'
		fi
	fi
	return 1 #/* "error", false */
}

