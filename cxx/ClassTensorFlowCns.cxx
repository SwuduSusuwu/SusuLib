/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#ifndef INCLUDES_cxx_ClassTensorFlowCns_cxx
#define INCLUDES_cxx_ClassTensorFlowCns_cxx
//#include "ClassCns.hxx" /* classCnsTests */
#include "ClassIo.hxx" /* ClassIoPath */
#include "ClassObject.hxx" /* ObjectMode */
#include "ClassTensorFlowCns.hxx" /* classCnsTests CoefficientDefaultType TensorFlowCns SUSUWU_CNS_USE_BIAS SUSUWU_CNS_USE_BIAS */
#include "Macros.hxx" /* SUSUWU_ERRSTR SUSUWU_SH_ERROR SUSUWU_USE_TENSORFLOW */
#ifdef SUSUWU_USE_TENSORFLOW
#include <stdexcept> /* std::runtime_error */
#include <string> /* std::string std::to_string */
//#include "tensorflow/core/lib/core/errors.h" /* TODO: true source of `tensorflow::error::Status` */
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
	/* Save base-class fields (inputMode, outputMode, inputNeurons, normalizers, etc.) */
	Cns::dumpTo(modelPath + ".cns");
#ifdef SUSUWU_TENSORFLOWCNS_PROTOBUF_FS
	/* Save the computation graph (GraphDef) */
	{
		const tensorflow::Status status = tensorflow::WriteBinaryProto(tensorflow::Env::Default(), modelPath + ".graphdef", graphDef);
		if(!status.ok()) {
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::dumpTo(.modelPath = \""+ modelPath + "\") { tensorflow::WriteBinaryProto(tensorflow::Env::Default(), modelPath + \".graphdef\", graphDef).ToString() == \"" + status.ToString() + "\" }"));
		}
	}
	/* Save model weights (coefficients and biases) */
#if SUSUWU_CNS_LOCAL_COEFFICIENTS
	{
		tensorflow::TensorProto coeffProto;
		coefficients.AsProtoField(&coeffProto);
		const tensorflow::Status status = tensorflow::WriteBinaryProto(tensorflow::Env::Default(), modelPath + ".tensors", coeffProto);
		if(!status.ok()) {
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::dumpTo(.modelPath = \""+ modelPath + "\") { tensorflow::WriteBinaryProto(..., modelPath + \".tensors\", coeffProto).ToString() == \"" + status.ToString() + "\" }"));
		}
	}
#	if SUSUWU_CNS_USE_BIAS
	{
		tensorflow::TensorProto biasProto;
		biases.AsProtoField(&biasProto);
		const tensorflow::Status status = tensorflow::WriteBinaryProto(tensorflow::Env::Default(), modelPath + ".biases", biasProto);
		if(!status.ok()) {
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::dumpTo(.modelPath = \""+ modelPath + "\") { tensorflow::WriteBinaryProto(..., modelPath + \".biases\", biasProto).ToString() == \"" + status.ToString() + "\" }"));
		}
	}
#	endif /* SUSUWU_CNS_USE_BIAS */
#else /* !SUSUWU_CNS_LOCAL_COEFFICIENTS */
	{
		std::vector<tensorflow::Tensor> outputs;
		const tensorflow::Status status = session->Run({}, {"coefficients" SUSUWU_CNS_IF_BIAS(SUSUWU_COMMA "biases")}, {}, &outputs);
		if(!status.ok()) {
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::dumpTo(.modelPath = \""+ modelPath + "\") { session->Run({}, {\"coefficients\"...}, {}, &outputs).ToString() == \"" + status.ToString() + "\" }"));
		}
		tensorflow::TensorProto coeffProto;
		outputs[0].AsProtoField(&coeffProto);
		{
			const tensorflow::Status writeStatus = tensorflow::WriteBinaryProto(tensorflow::Env::Default(), modelPath + ".tensors", coeffProto);
			if(!writeStatus.ok()) {
				throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::dumpTo(.modelPath = \""+ modelPath + "\") { tensorflow::WriteBinaryProto(..., modelPath + \".tensors\", coeffProto).ToString() == \"" + writeStatus.ToString() + "\" }"));
			}
		}
#	if SUSUWU_CNS_USE_BIAS
		tensorflow::TensorProto biasProto;
		outputs[1].AsProtoField(&biasProto);
		{
			const tensorflow::Status writeStatus = tensorflow::WriteBinaryProto(tensorflow::Env::Default(), modelPath + ".biases", biasProto);
			if(!writeStatus.ok()) {
				throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::dumpTo(.modelPath = \""+ modelPath + "\") { tensorflow::WriteBinaryProto(..., modelPath + \".biases\", biasProto).ToString() == \"" + writeStatus.ToString() + "\" }"));
			}
		}
#	endif /* SUSUWU_CNS_USE_BIAS */
	}
#endif /* !SUSUWU_CNS_LOCAL_COEFFICIENTS */
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
	/* Restore base-class fields (inputMode, outputMode, inputNeurons, normalizers, etc.) */
	Cns::loadFrom(modelPath + ".cns");
#ifdef SUSUWU_TENSORFLOWCNS_PROTOBUF_FS
	/* Load the computation graph from disk */
	{
		const tensorflow::Status status = tensorflow::ReadBinaryProto(tensorflow::Env::Default(), modelPath + ".graphdef", &graphDef);
		if(!status.ok()) {
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(.modelPath = \""+ modelPath + "\") { tensorflow::ReadBinaryProto(tensorflow::Env::Default(), modelPath + \".graphdef\", &graphDef).ToString() == \"" + status.ToString() + "\" }"));
		}
	}
	/* Close the old session and create a fresh one with the loaded graph */
	if(SUSUWU_NULLPTR != session) {
		TF_CHECK_OK(session->Close());
		delete session;
		session = SUSUWU_NULLPTR;
	}
	{
		const tensorflow::SessionOptions sessionOptions;
		const tensorflow::Status status = tensorflow::NewSession(sessionOptions, &session);
		if(!status.ok()) {
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(.modelPath = \""+ modelPath + "\") { tensorflow::NewSession(sessionOptions, &session).ToString() == \"" + status.ToString() + "\" }"));
		}
	}
	{
		const tensorflow::Status status = session->Create(graphDef);
		if(!status.ok()) {
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(.modelPath = \""+ modelPath + "\") { session->Create(graphDef).ToString() == \"" + status.ToString() + "\" }"));
		}
	}
	/* Load and restore model weights (coefficients and biases) */
	{
		tensorflow::TensorProto coeffProto;
		{
			const tensorflow::Status status = tensorflow::ReadBinaryProto(tensorflow::Env::Default(), modelPath + ".tensors", &coeffProto);
			if(!status.ok()) {
				throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(.modelPath = \""+ modelPath + "\") { tensorflow::ReadBinaryProto(..., modelPath + \".tensors\", &coeffProto).ToString() == \"" + status.ToString() + "\" }"));
			}
		}
		tensorflow::Tensor coeffTensor;
		if(!coeffTensor.FromProto(coeffProto)) {
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(.modelPath = \""+ modelPath + "\") { !coeffTensor.FromProto(coeffProto) /* malformed coefficients proto */ }"));
		}
#	if SUSUWU_CNS_LOCAL_COEFFICIENTS
		coefficients = coeffTensor;
#	endif /* SUSUWU_CNS_LOCAL_COEFFICIENTS */
#	if SUSUWU_CNS_USE_BIAS
		tensorflow::TensorProto biasProto;
		{
			const tensorflow::Status status = tensorflow::ReadBinaryProto(tensorflow::Env::Default(), modelPath + ".biases", &biasProto);
			if(!status.ok()) {
				throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(.modelPath = \""+ modelPath + "\") { tensorflow::ReadBinaryProto(..., modelPath + \".biases\", &biasProto).ToString() == \"" + status.ToString() + "\" }"));
			}
		}
		tensorflow::Tensor biasTensor;
		if(!biasTensor.FromProto(biasProto)) {
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(.modelPath = \""+ modelPath + "\") { !biasTensor.FromProto(biasProto) /* malformed biases proto */ }"));
		}
#		if SUSUWU_CNS_LOCAL_COEFFICIENTS
		biases = biasTensor;
#		endif /* SUSUWU_CNS_LOCAL_COEFFICIENTS */
		/* Restore weights into the session variables using the assign ops from the training graph */
		{
			const tensorflow::Status status = session->Run(
				{{"randomCoefficients", coeffTensor}, {"randomBiases", biasTensor}},
				{}, {"assignCoefficients", "assignBiases"}, SUSUWU_NULLPTR);
			if(!status.ok()) {
				throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(.modelPath = \""+ modelPath + "\") { session->Run({randomCoefficients, randomBiases}, {}, {assignCoefficients, assignBiases}, nullptr).ToString() == \"" + status.ToString() + "\" }"));
			}
		}
#	else /* !SUSUWU_CNS_USE_BIAS */
		/* Restore coefficients-only into the session variable (bias-free build) */
		{
			const tensorflow::Status status = session->Run(
				{{"randomCoefficients", coeffTensor}},
				{}, {"assignCoefficients"}, SUSUWU_NULLPTR);
			if(!status.ok()) {
				throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(.modelPath = \""+ modelPath + "\") { session->Run({randomCoefficients}, {}, {assignCoefficients}, nullptr).ToString() == \"" + status.ToString() + "\" }"));
			}
		}
#	endif /* !SUSUWU_CNS_USE_BIAS */
	}
	setInitialized(true);
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

const bool classTensorFlowCnsTests() {
	return classCnsTests<TensorFlowCns, TensorFlowCns::CoefficientDefaultType>(
#if SUSUWU_CNS_IS_VALUE_OBJECT
			true
#else
			false
#endif /* SUSUWU_CNS_IS_VALUE_OBJECT else */
			,
#ifdef SUSUWU_TENSORFLOWCNS_HAS_DUMPTO
			true
#else
			false
#endif /* def SUSUWU_TENSORFLOWCNS_HAS_DUMPTO else */
			);
}

}; /* namespace Susuwu */
#endif /* def SUSUWU_USE_TENSORFLOW */
#endif /* ndef INCLUDES_cxx_ClassTensorFlowCns_cxx */

