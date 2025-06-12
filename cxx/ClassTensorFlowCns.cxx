/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#ifndef INCLUDES_cxx_ClassTensorFlowCns_cxx
#define INCLUDES_cxx_ClassTensorFlowCns_cxx
//#include "ClassCns.hxx" /* Cns */
#include "ClassIo.hxx" /* ClassIoPath */
#include "ClassObject.hxx" /* ObjectMode */
#include "ClassTensorFlowCns.hxx" /* Cns CoefficientDefaultType TensorFlowCns SUSUWU_CNS_USE_BIAS SUSUWU_CNS_USE_BIAS */
#include "Macros.hxx" /* SUSUWU_ERRSTR SUSUWU_SH_ERROR SUSUWU_USE_TENSORFLOW */
#ifdef SUSUWU_USE_TENSORFLOW
#include <limits> /* std::numeric_limits */
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
#include <tuple> /* std::tuple */
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
	throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(\""+ modelPath + "\") { /* TODO; deserialize `sp` into `tensorflow::Tensor`. Use `tensorflow::SavedModelBundle` or `TF_LoadSessionFromSavedModel`? */"));
#endif /* SUSUWU_TENSORFLOWCNS_MANUAL_FS */
}

template <typename Numeral, class Input, class Output, typename Process>
static const bool classTensorFlowCnsTestLinear(const Input min, const Input max, const Input step, const Numeral epsilon /* For now, `epsilon` is best result known (so that regressions trigger debug messages) */, Process process) {
	std::vector<std::tuple<Input, Output>> inputsToOutputs;
//	 epsilon = std::numeric_limits<Numeral>::epsilon /* TODO: [compute most accurate possible values](https://github.com/copilot/share/c056538e-08c0-8822-9001-720924696114) */
	for(Input input = min; max >= input; input += step) {
		inputsToOutputs.push_back({input, input * 2});
	}
	TensorFlowCns tensorFlowCns;
	Cns &cns = tensorFlowCns;
	cns.setInputNeurons(1);
	cns.setInputMode(ToObjectMode<Input>::value);
	cns.setNeuronsPerLayer(1);
	cns.setLayersOfNeurons(1);
	cns.setOutputNeurons(1);
	cns.setOutputMode(ToObjectMode<Output>::value);
#if SUSUWU_TENSORFLOW_EXCEPTIONS
	try {
#endif /* SUSUWU_TENSORFLOW_EXCEPTIONS */
		const size_t inputsCount = inputsToOutputs.size();
		cns.setupSynapses(inputsToOutputs);
		{
#if SUSUWU_CNS_IS_VALUE_OBJECT /* if comparison is not limited to object addresses */
			TensorFlowCns tensorFlowCns2 = tensorFlowCns;
			Cns &cns2 = tensorFlowCns2;
#endif /* SUSUWU_CNS_IS_VALUE_OBJECT */
			assert(inputsCount == inputsToOutputs.size());
			Output worstLoss = 0;
			for(unsigned index = 0; inputsToOutputs.size() > index; ++index) {
				const Output label = std::get<1>(inputsToOutputs.at(index)),
					output = process(cns, std::get<0>(inputsToOutputs.at(index))),
					loss = label - output,
					absLoss = abs(loss);
				if(absLoss > worstLoss) {
					worstLoss = absLoss;
				}
				if(!(epsilon >= absLoss) /* inverted test (with `!`) catches `nan` */) {
					throw std::runtime_error("classTensorFlowCnsTests(.min = " + std::to_string(min) + ", .max = " + std::to_string(max) + ", .step = " + std::to_string(step) + ", .epsilon == " + std::to_string(epsilon) + ") { label[" + std::to_string(index) + "] == " + std::to_string(label) + "; output[" + std::to_string(index) + "] == " + std::to_string(output) + "; loss /* label - output */ == " + std::to_string(loss) + "; if(!(abs(loss) <= epsilon)) { throw std::runtime_error; } }");
				}
			}
			if(epsilon > (1 + worstLoss)) {
				SUSUWU_DEBUG("classTensorFlowCnsTests(.min = " + std::to_string(min) + ", .max = " + std::to_string(max) + ", .step = " + std::to_string(step) + ", .epsilon == " + std::to_string(epsilon) + ") { worstLoss == " + std::to_string(worstLoss) + "; /* `(epsilon > (1 + worstLoss))`, set `epsilon = worstLoss` to detect future regressions */; }");
			}

#if SUSUWU_CNS_IS_VALUE_OBJECT /* test that `process()` follows `const` method rules, and that `equals()` and `hashCode` do what those are supposed to do */
			assert(cns.equals(cns2));
			assert(cns.hashCode() == cns2.hashCode()); /* cppcheck-suppress knownConditionTrueFalse */
#endif /* SUSUWU_CNS_IS_VALUE_OBJECT */
		}

#ifdef SUSUWU_TENSORFLOWCNS_HAS_DUMPTO /* TODO: implement `dumpTo()` and `loadFrom()`, remove `cppcheck-suppress knownConditionTrueFalse`. */
		TensorFlowCns tensorFlowCns2 = tensorFlowCns;
		Cns &cns2 = tensorFlowCns2;
		const ClassIoPath dumpToPath = "classTensorFlowCnsTests.tmp"; /* TODO: if file exists, ask user what to do? TODO: use system temp path? */
		cns.dumpTo(dumpToPath);
		cns2.loadFrom(dumpToPath);
//		classIoRemove(dumpToPath); /* TODO: does our IO library have a function for file removal? Perhaps there is some `fopen` flag to use instead, which causes removal as soon as the file is closed?  */
		assert(cns.equals(cns2));
		assert(cns.hashCode() == cns2.hashCode()); /* cppcheck-suppress knownConditionTrueFalse */
#endif /* else ndef SUSUWU_TENSORFLOWCNS_HAS_DUMPTO */

#if SUSUWU_TENSORFLOW_EXCEPTIONS
	} catch(const tensorflow::error::InvalidArgument &w) {
		(void) /* TODO */
	} catch(const tensorflow::error::OutOfRange &w) {
		(void) /* TODO */
	} catch(const tensorflow::error::NotFound &w) {
		(void) /* TODO */
	} catch(const tensorflow::error::AlreadyExists &w) {
		(void) /* TODO */
	} catch(const tensorflow::error::Aborted &w) {
		(void) /* TODO */
	} catch(const tensorflow::error::Status &w) {
		(void) /* TODO */
	}
#endif /* SUSUWU_TENSORFLOW_EXCEPTIONS */
	return true;
}
const bool classTensorFlowCnsTests() {
	std::function<const float(const Cns &, const float)> processToFloatLambda = [](const Cns &cns, const float x) { return cns.processToFloat(x); };
	std::function<const int(const Cns &, const int)> processToIntLambda = [](const Cns &cns, const int x) { return cns.processToInt(x); };
	classTensorFlowCnsTestLinear<TensorFlowCns::CoefficientDefaultType, float, float>(-1.0, 1.0, 0.001, /* 0.072094 ... */ 0.1180462, processToFloatLambda); /* "normalization" (average == 0, std == 1), most simple to learn */
	classTensorFlowCnsTestLinear<TensorFlowCns::CoefficientDefaultType, float, float>(-1000.0, 1000.0, 1.0, /* 70.774782 ... */ 107.8262, processToFloatLambda); /* (average == 0, std == 1000) */
	classTensorFlowCnsTestLinear<TensorFlowCns::CoefficientDefaultType, float, float>(0.0, 2.0, 0.001, /* 0.072054 ... */ 0.118012, processToFloatLambda); /* (average == 1, std == 1) */
	classTensorFlowCnsTestLinear<TensorFlowCns::CoefficientDefaultType, float, float>(0.0, 2000.0, 1.0, /* 70.698730 ... */ 117.921631, processToFloatLambda); /* (average == 1000, std == 1000) */
	classTensorFlowCnsTestLinear<TensorFlowCns::CoefficientDefaultType, int, int>(-1000, 1000, 1, /* TODO: reduce this */ 2242, processToIntLambda);
	classTensorFlowCnsTestLinear<TensorFlowCns::CoefficientDefaultType, int, int>(0, 2000, 1, /* TODO: reduce this */ 2552, processToIntLambda);
	return true;
}

}; /* namespace Susuwu */
#endif /* def SUSUWU_USE_TENSORFLOW */
#endif /* ndef INCLUDES_cxx_ClassTensorFlowCns_cxx */

