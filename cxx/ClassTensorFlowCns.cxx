/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#ifndef INCLUDES_cxx_ClassTensorFlowCns_cxx
#define INCLUDES_cxx_ClassTensorFlowCns_cxx
#define SUSUWU_TENSORFLOWCNS_PROTOBUF_FS /* The is the most close to fit-for-use */
#include "ClassTensorFlowCns.hxx" /* TensorFlowCns SUSUWU_CNS_USE_BIAS SUSUWU_CNS_USE_BIAS */
#include "ClassIo.hxx" /* ClassIoPath */
#include "ClassObject.hxx" /* ObjectMode */
#include "Macros.hxx" /* SUSUWU_ERRSTR SUSUWU_SH_ERROR SUSUWU_USE_TENSORFLOW */
#ifdef SUSUWU_USE_TENSORFLOW
#include <stdexcept> /* std::runtime_error */
#include <string> /* std::string std::to_string */
#include <tensorflow/core/framework/graph.pb.h> /* tensorflow::GraphDef */
#include <tensorflow/core/framework/tensor.h> /* tensorflow::Tensor */
#include <tensorflow/core/framework/types.h> /* tensorflow::uint64 */
#include <tensorflow/core/lib/core/status.h> /* tensorflow::Status */
#if defined(SUSUWU_TENSORFLOWCNS_MANUAL_FS) || defined(SUSUWU_TENSORFLOWCNS_PROTOBUF_FS)
#	include <tensorflow/core/lib/io/path.h> /* tensorflow::RandomAccessFile */
#endif /* defined(SUSUWU_TENSORFLOWCNS_MANUAL_FS) || defined(SUSUWU_TENSORFLOWCNS_PROTOBUF_FS) */
#include <tensorflow/core/platform/env.h> /* tensorflow::Env */
#include <tensorflow/core/platform/logging.h> /* TF_CHECK_OK */
#include <tensorflow/core/platform/status.h> /* TF_CHECK_OK */
#include <tensorflow/core/public/session_options.h> /* tensorflow::SessionOptions */
#include <vector> /* std::vector */

namespace Susuwu {

/* store the connectome (the synapse coefficients and biases); serialize from `TensorFlowCns::coefficients` (or `TensorFlowCns::graphDef`), into `modelPath`.
 * @throw std::runtime_error */
void TensorFlowCns::dumpTo(const ClassIoPath &modelPath) const { /* TODO: the implementation is in the header to allow future `template<>` use; If `dumpTo` will not use C++ `template<>`s, implement in `ClassTensorFlowCns.cxx` to reduce `./build.sh` resource use */
#if !SUSUWU_IN_MEMORY_COEFFICIENTS
	std::vector<tensorflow::Tensor> outputs;
	TF_CHECK_OK(session->Run({}, {"coefficients", "biases"}, {}, &outputs));
	const auto &coefficients = outputs[0];
	const auto &biases = outputs[1];
#endif /* !SUSUWU_IN_MEMORY_COEFFICIENTS */
#ifdef SUSUWU_TENSORFLOWCNS_PROTOBUF_FS
	tensorflow::TensorProto tensorProto;
#ifdef SUSUWU_TENSORFLOWCNS_BACKUP_TENSORS
	coefficients.AsProtoField(&tensorProto);
	biases.AsProtoField(&tensorProto);
	const tensorflow::Status status = tensorflow::WriteBinaryProto(tensorflow::Env::Default(), modelPath, tensorProto);
#else /* !def SUSUWU_TENSORFLOWCNS_BACKUP_TENSORS */
	const tensorflow::Status status = tensorflow::WriteBinaryProto(tensorflow::Env::Default(), modelPath, graphDef);
#endif /* !def SUSUWU_TENSORFLOWCNS_BACKUP_TENSORS */
	if (!status.ok()) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::dumpTo(.modelPath = \""+ modelPath + "\") { tensorflow::WriteBinaryProto(tensorflow::Env::Default(), modelPath, graphDef).ToString() == \"" + status.ToString() + "\" }"));
	}
#elif defined(SUSUWU_TENSORFLOWCNS_ROCM_FS)
	const tensorflow::Status status = tensorflow::SaveTensorToFile(modelPath, coefficients); /* TODO: Assistant suggested to use this function, but don't know what to include. [All Google found was ROCm](https://rocm.docs.amd.com/projects/rocPyDecode/en/latest/reference/decoderClass.html#savetensortofile-output-file-path-frame-adrs-width-height-rgb-format-surface-info) */ /* TODO: biases */
	if(!status.ok()) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::dumpTo(.modelPath = \""+ modelPath + "\") { !tensorflow::SaveTensorToFile(modelPath, coefficients).ok() }"));
	}
#elif defined(SUSUWU_TENSORFLOWCNS_SAVEDMODEL_FS)
	throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::dumpTo(.modelPath = \""+ modelPath + "\") { /* TODO; `SUSUWU_TENSORFLOWCNS_SAVEDMODEL_FS`. Numerous assistants suggest functions (such as `LoadSessionFromSavedModel`) which are not found, and results of related searches (such as for \"SavedModelBundle\") amount to \"TensorFlow's C / C++ API's do not have this for now\" */"));
#else /* else SUSUWU_TENSORFLOWCNS_MANUAL_FS */
	throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::dumpTo(.modelPath = \""+ modelPath + "\") { /* TODO; `SUSUWU_TENSORFLOWCNS_MANUAL_FS`. Perhaps just dump binary `coefficients` into `modelPath`? */"));
#endif /* SUSUWU_TENSORFLOWCNS_MANUAL_FS */
}

/* load the connectome (the synapse coefficients and biases); deserialize into `TensorFlowCns::const` (or `TensorFlowCns::graphDef`), from `modelPath`.
 * @throw std::runtime_error, tensorflow::errors::Internal, tensorflow::errors::Unavailable */
void TensorFlowCns::loadFrom(const ClassIoPath &modelPath) { /* TODO: the implementation is in the header to allow future `template<>` use; If `loadFrom` will not use C++ `template<>`s, implement in `ClassTensorFlowCns.cxx` to reduce `./build.sh` resource use */
	switch(inputMode) { /* TODO: support more types, or replace `template<class Input>` on `initScopeRootForward` with an argument (such as `ObjectMode input`) */
		case objectModeFloat:	initScopeRootForward<float, float>(DataTypeToEnum<float>::value); break;
		case objectModeInt:	initScopeRootForward<int, int>(DataTypeToEnum<int>::value); break;
		default:	throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(.modelPath = \""+ modelPath + "\") { inputMode == " + std::to_string(inputMode) + "; /* unsupported `ObjectMode` */ }"));
	}
#ifdef SUSUWU_TENSORFLOWCNS_PROTOBUF_FS
//	tensorflow::GraphDef graphDef; /* TODO: uncomment (use temp `tensorflow::GraphDef`) or remove (settle on `TensorFlowCns::graphDef`) */
#	ifdef SUSUWU_TENSORFLOWCNS_BACKUP_TENSORS
	throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(.modelPath = \""+ modelPath + "\") { /* TODO; `SUSUWU_TENSORFLOWCNS_BACKUP_TENSORS`. Solution: unset that macro for now. */ }"));
#	else /* !def SUSUWU_TENSORFLOWCNS_BACKUP_TENSORS */
	const tensorflow::Status status = ReadBinaryProto(tensorflow::Env::Default(), modelPath, &graphDef);
	if(!status.ok()) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(.modelPath = \""+ modelPath + "\") { !tensorflow::ReadBinaryProto(tensorflow::Env::Default(), modelPath, &graphDef).ok() }"));
	}
//		TF_CHECK_OK(root.ToGraphDef(&graphDef)); /* uncomment if `ReadBinaryProto` loads `tensorflow::Scope root` */
//		TF_CHECK_OK(session->Create(graphDef)); /* uncomment to reload `tensorflow::GraphDef` */
#	endif /* !def SUSUWU_TENSORFLOWCNS_BACKUP_TENSORS */
#elif defined(SUSUWU_TENSORFLOWCNS_ROCM_FS)
	const tensorflow::Status status = tensorflow::ReadTensorFromFile(modelPath, &coefficients); /* Assistant suggested to use this function, but don't know what to include. All Google found was [ShuffleNetV2Plus from MindX SDK](https://zhuanlan.zhihu.com/p/558676663) */ /* TODO: biases */
#elif defined(SUSUWU_TENSORFLOWCNS_SAVEDMODEL_FS)
	tensorflow::SavedModelBundle bundle;
	tensorflow::SessionOptions sessionOptions;
	tensorflow::RunOptions runOptions;
	const tensorflow::Status status = tensorflow::LoadSessionFromSavedModel(sessionOptions, runOptions, modelPath, {tensorflow::kSavedModelTagServe}, &bundle); /* TODO: GitHub's assistant suggests this function, but don't know what to `#include` */
	if(!status.ok()) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(.modelPath = \""+ modelPath + "\") { tensorflow::LoadSessionFromSavedModel(sessionOptions, runOptions, modelPath, {tensorflow::kSavedModelTagServe}, &bundle).ToString() == \"" + status.ToString() + "\" }"));
	} /* Use `tensorflow::SavedModelBundle`? */
	session = bundle.session.get();
#else /* SUSUWU_TENSORFLOWCNS_MANUAL_FS */
	auto env = tensorflow::Env::Default(); /* OS / platform environment handle */

	auto status = env->FileExists(modelPath);
	if(status.ok() == false) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(\""+ modelPath + "\") { (!env->FileExists(modelPath).ok()) }"));
	}

	tensorflow::uint64 fileSize;
	status = env->GetFileSize(modelPath, &fileSize);

	if(status.ok() == false) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(\""+ modelPath + "\") { tensorflow::uint64 fileSize); (!env->GetFileSize(modelPath, &fileSize).ok()) }"));
	}

	SUSUWU_DEBUG(getName() + "::loadFrom(modelPath = \""+ modelPath + "\") { tensorflow::uint64 fileSize; env->GetFileSize(modelPath, &fileSize); (fileSize == " + std::to_string(fileSize) + ") }");

	std::unique_ptr<tensorflow::RandomAccessFile> fileHandle;
	status = env->NewRandomAccessFile(modelPath, &fileHandle);

	if(status.ok() == false) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(modelPath = \""+ modelPath + "\") { std::unique_ptr<tensorflow::RandomAccessFile> fileHandle; (!env->NewRandomAccessFile(modelPath, &fileHandle).ok()) }"));
	}

	std::string content;
	content.resize(fileSize);

	tensorflow::StringPiece sp;

	status = fileHandle->Read(0, fileSize, &sp, &(content)[0]);

	if(status.ok() == false) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(\""+ modelPath + "\") { std::unique_ptr<tensorflow::RandomAccessFile> fileHandle; (!fileHandle->Read(0, fileSize, &sp, &(content)[0]).ok()) }"));
	}

//	coefficients = /* TODO */
	throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(\""+ modelPath + "\") { /* TODO; deserialize `sp` into `tensorflow::Tensor`. Use `tensorflow::SavedModelBundle` or `TF_LoadSessionFromSavedModel`? */ }"));
#endif /* SUSUWU_TENSORFLOWCNS_MANUAL_FS */
}

}; /* namespace Susuwu */
#endif /* def SUSUWU_USE_TENSORFLOW */
#endif /* ndef INCLUDES_cxx_ClassTensorFlowCns_cxx */

