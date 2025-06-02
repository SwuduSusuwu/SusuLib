/* Attribution (henceforth "*this attribution*", whose syntax is *Markdown*): 2024 [Swudu Susuwu](https://swudususuwu.substack.com)
 * <https://github.com/SwuduSusuwu/SusuLib/> has the newest version of `./cxx/ClassTensorFlowCns.hxx` (henceforth "*this source code*").
 * If *this attribution* is shown, *this source code* allows all uses. *This attribution* constitutes the most permissive which is compatible with [*GPLv2*](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) + [*Apache 2*](https://www.apache.org/licenses/LICENSE-2.0.html), which is suitable for personal use (also suitable for school use).
 * If *this attribution* is not professional enough for business use: businesses can use *this source code* through included versions of [*GPLv2*](./LICENSE_GPLv2), [*Apache 2*](./LICENSE), or through both of those. */
#pragma once
#ifndef INCLUDES_cxx_ClassTensorFlowCns_hxx
#define INCLUDES_cxx_ClassTensorFlowCns_hxx
#include "ClassCns.hxx" /* Cns */
#include "ClassNumeral.hxx" /* numeralDenormalization() numeralNormalization() NumeralNormalizers::fromTuple */
#include "ClassObject.hxx" /* ObjectMode ToObjectMode SUSUWU_PURE_VIRTUAL_DEFAULTS() */
#include "Macros.hxx" /* SUSUWU_ERRSTR SUSUWU_IF_CPLUSPLUS SUSUWU_INFO SUSUWU_INTPTR SUSUWU_NULLPTR SUSUWU_OVERRIDE SUSUWU_SH_ERROR SUSUWU_USE_TENSORFLOW SUSUWU_WARNING */
#ifdef SUSUWU_USE_TENSORFLOW
#include SUSUWU_IF_CPLUSPLUS(<cassert>, <assert.h>) /* assert */
#include SUSUWU_IF_CPLUSPLUS(<cstddef>, <stddef.h>) /* size_t */
#if (defined(SUSUWU_CNS_TRUE_RANDOM_INIT) && SUSUWU_CNS_TRUE_RANDOM_INIT) || ( defined(SUSUWU_CNS_HE_INIT) && SUSUWU_CNS_HE_INIT) || ( defined(SUSUWU_CNS_XAVIER_INIT) && SUSUWU_CNS_XAVIER_INIT)
#	include SUSUWU_IF_CPLUSPLUS(<cstdlib>, <stdlibf.h>) /* rand() RAND_MAX */
#endif /* defined(SUSUWU_CNS_TRUE_RANDOM_INIT) && SUSUWU_CNS_TRUE_RANDOM_INIT */
#include <stdexcept> /* std::invalid_argument std::runtime_error */
#include <string> /* std::string */
//#include <tensorflow/c/experimental/ops/math_ops.h> /* tensorflow::ops::* */ /* TODO: fix "../tensorflow/tensorflow/c/eager/abstract_function.h:19:10: error: 'tensorflow/core/framework/function.pb.h' file not found" */
//#include <tensorflow/c/experimental/ops/nn_ops.h> /* tensorflow::ops::Relu */ /* TODO: fix "../tensorflow/tensorflow/c/eager/abstract_function.h:19:10: error: 'tensorflow/core/framework/function.pb.h' file not found" */
#include <tensorflow/cc/framework/scope.h> /* tensorflow::Scope tensorflow::NewRootScope */
#include <tensorflow/cc/ops/standard_ops.h> /* tensorflow::ops::* */
#include <tensorflow/core/framework/graph.pb.h> /* tensorflow::GraphDef */
#include <tensorflow/core/framework/tensor.h> /* tensorflow::Tensor tensorflow::TensorShape */
#include <tensorflow/core/framework/types.h> /* tensorflow::DT_FLOAT, tensorflow::DT_INT32, tensorflow::DT_STRING tensorflow::DataType tensorflow::int64 */
#include <tensorflow/core/lib/core/status.h> /* tensorflow::Status */
#include <tensorflow/core/platform/logging.h> /* TF_CHECK_OK */
#include <tensorflow/core/platform/status.h> /* TF_CHECK_OK */
#include <tensorflow/core/public/session.h> /* tensorflow::NewSession tensorflow::Session tensorflow::SessionOptions */
#include <tensorflow/core/public/session_options.h> /* tensorflow::SessionOptions */
#include <tuple> /* std::tuple */
#include <vector> /* std::vector */
#ifndef SUSUWU_CNS_LOCAL_COEFFICIENTS
#	define SUSUWU_CNS_LOCAL_COEFFICIENTS false
#endif /* ndef SUSUWU_CNS_LOCAL_COEFFICIENTS */
#ifndef SUSUWU_TENSORFLOW_HAS_DATATYPETOENUM
#	define SUSUWU_TENSORFLOW_HAS_DATATYPETOENUM false
#endif /* ndef SUSUWU_TENSORFLOW_HAS_DATATYPETOENUM */
/* NOLINTBEGIN(cppcoreguidelines-macro-usage) */
#ifndef SUSUWU_CNS_USE_MLP
#	define SUSUWU_CNS_USE_MLP false /* Multiple-Layer-Perceptron mode. No reason to disable this (if `1 == layersOfNeurons`, `setupSynapses()` and `processTo*()` act as Single-Layer-Perceptrons), but for now is TODO */
#endif /* ndef SUSUWU_CNS_USE_MLP */
#if SUSUWU_CNS_IF_MLP
# define SUSUWU_CNS_IF_MLP(THEN, ELSE) THEN
#else /* else !SUSUWU_CNS_USE_MLP */
# define SUSUWU_CNS_IF_MLP(THEN, ELSE) ELSE
#endif /* else !SUSUWU_CNS_USE_MLP */
/* NOLINTEND(cppcoreguidelines-macro-usage) */

namespace Susuwu {
typedef struct TensorFlowCnsLoss { tensorflow::Output loss, backprop; } /* `decltype(tensorflow::ops::SoftmaxCrossEntropyWithLogits(tensorflow::Scope::NewRootScope(), tensorflow::ops::Add, tensorflow::ops::Placeholder))`? */ TensorFlowCnsLoss;

template<> /* specialization continued from `cxx/ClassObject.hxx` */
struct ToObjectMode<tensorflow::tstring> { static SUSUWU_CONSTEXPR ObjectMode value = objectModeString; }; /* `error: implicit instantiation of undefined template 'Susuwu::ToObjectMode<tsl::tstring>'` fix */

template<class Q>
#if SUSUWU_TENSORFLOW_HAS_DATATYPETOENUM
struct DataTypeToEnum { static SUSUWU_CONSTEXPR tensorflow::DataType value = tensorflow::DataTypeToEnum<Q>::v(); };
#else /* else !SUSUWU_TENSORFLOW_HAS_DATATYPETOENUM */ /* `error: no template named 'DataTypeToEnum' in namespace 'tensorflow';` fix. */
struct DataTypeToEnum; /* The template specializations (which follow) require forward declaration of this. */
// typedef struct DataTypeToEnum { static tensorflow::DataType value; } DataTypeToEnum; /* TODO: fix `error: expected ';' after struct` */
template<>
struct DataTypeToEnum<float> { static SUSUWU_CONSTEXPR tensorflow::DataType value = tensorflow::DT_FLOAT; };
template<>
struct DataTypeToEnum<int> { static SUSUWU_CONSTEXPR tensorflow::DataType value = tensorflow::DT_INT32; };
template<>
struct DataTypeToEnum<tensorflow::int64> { static SUSUWU_CONSTEXPR tensorflow::DataType value = tensorflow::DT_INT64; };
template<>
struct DataTypeToEnum<std::string> { static SUSUWU_CONSTEXPR tensorflow::DataType value = tensorflow::DT_STRING; };
template<>
struct DataTypeToEnum<tensorflow::tstring> { static SUSUWU_CONSTEXPR tensorflow::DataType value = tensorflow::DT_STRING; };
#endif /* else !SUSUWU_TENSORFLOW_HAS_DATATYPETOENUM */


class TensorFlowCns : public Cns {
public:
	typedef tensorflow::int64 /* decltype(tensorflow::TensorShapeRep({}).num_elements()) */ DimSz; /* `[-Wnarrowing]` fix. `tensorflow/core/framework/tensor_shape.h` has "We use `int64` and not `size_t` to be compatible with `Eigen::Tensor` which uses `ptrdiff_t`" */
	typedef float CoefficientDefaultType; /* `float` is the most common (due to: is fast and accurate). This `typedef` allows to set a new default for the whole `TensorFlowCns` class. */

	/* @throw std::runtime_error, tensorflow::errors::Internal, tensorflow::errors::Unavailable */
	TensorFlowCns() : root(tensorflow::Scope::NewRootScope()) {
		const tensorflow::SessionOptions sessionOptions;
		const tensorflow::Status status = tensorflow::NewSession(sessionOptions, &session);
		if(!status.ok()) {
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, this->getName() + "() { (!tensorflow::NewSession(sessionOptions, &session).ok()) { status.ToString() == \"" + status.ToString() + "\"; } }"));
		}
	}
	TensorFlowCns(const TensorFlowCns &obj) : Cns(obj), root(tensorflow::Scope::NewRootScope()) {
		const tensorflow::SessionOptions sessionOptions;
		const tensorflow::Status status = tensorflow::NewSession(sessionOptions, &session);
		if(!status.ok()) {
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, this->getName() + "() { (!tensorflow::NewSession(sessionOptions, &session).ok()) { status.ToString() == \"" + status.ToString() + "\"; } }"));
		}
#if SUSUWU_CNS_LOCAL_COEFFICIENTS
		this->coefficients = obj.coefficients;
		this->biases = obj.biases;
#endif /* SUSUWU_CNS_LOCAL_COEFFICIENTS */
		this->graphDef = obj.graphDef;
	}

	~TensorFlowCns() SUSUWU_OVERRIDE {
		if(SUSUWU_NULLPTR != session) {
			TF_CHECK_OK(session->Close());
			delete session;
		}
	}
#if SUSUWU_CNS_VALUE_SEMANTICS
#	define SUSUWU_CLASS_OPERATOREQUALTO(noop) ; /* NOLINT(cppcoreguidelines-macro-usage) */
	bool operator==(const Class &obj) const SUSUWU_OVERRIDE { /* NOLINT(fuchsia-overloaded-operator) */
		const auto *thisFlow = dynamic_cast<const TensorFlowCns *>(this);
		const auto *objFlow = dynamic_cast<const TensorFlowCns *>(&obj);
		return Cns::operator==(obj) &&
#if SUSUWU_CNS_LOCAL_COEFFICIENTS
			thisFlow->coefficients == objFlow->coefficients &&
			thisFlow->biases == objFlow->biases;
#else /* else !SUSUWU_CNS_LOCAL_COEFFICIENTS */
			thisFlow->graphDef.SerializeAsString() == objFlow->graphDef.SerializeAsString();
/* TODO: `#include <google/protobuf/util/message_differencer.h>` `bool areEqual = google::protobuf::util::MessageDifferencer::Equals(thisFlow->graphDef, objFlow->graphDef);`. This development environment does not have `message_differencer.h` to test those. */
#endif /* else !SUSUWU_CNS_LOCAL_COEFFICIENTS */
//		return this->hashCode() == obj.hashCode();
	}
	const SUSUWU_INTPTR hashCode() const SUSUWU_OVERRIDE SUSUWU_VIRTUAL_HASHCODE /* Shallow hash code. TODO: hash container values */
#endif /* SUSUWU_CNS_VALUE_SEMANTICS */
	SUSUWU_PURE_VIRTUAL_DEFAULTS(Susuwu::TensorFlowCns) /* `getName()`, `isPureVirtual()`, `operator==()`, ... */

	/* Topological values; sets the "shape" of `Cns.coefficients` (or of whatever the derived class uses to store the connectome) */
	const bool isSquareConnectome() const SUSUWU_OVERRIDE { return true; /* TODO: `return usesLayerIndexToNeuronCount;` */}
	void setSquareConnectome(const bool is) SUSUWU_OVERRIDE {
		if(!is) { throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::setSquareConnectome(.is = " + std::to_string(is) + ") { /* TODO: {`setupSynapses`, `processTo*`} with `std::vector<DimSz> layerIndexToNeuronCount` */ }")); }
	}
//	void setInputMode(ObjectMode x) SUSUWU_OVERRIDE { inputMode = x; } /* sets type of input */
//	void setOutputMode(ObjectMode x) SUSUWU_OVERRIDE { outputMode = x; } /* sets type of output (notice: some implementations require `inputMode == outputMode`) */
	template<class CoefficientType>
	void restructureConnectomeImpl() { /* CRTP backend, for internal use. */
		setInitialized(false);
#if SUSUWU_CNS_LOCAL_COEFFICIENTS
		coefficients = tensorflow::Tensor(DataTypeToEnum<CoefficientType>::value, tensorflow::TensorShape({static_cast<DimSz>(inputNeurons), static_cast<DimSz>(neuronsPerLayer)}));
		biases = tensorflow::Tensor(DataTypeToEnum<CoefficientType>::value, tensorflow::TensorShape({static_cast<DimSz>(neuronsPerLayer)}));
#endif /* SUSUWU_CNS_LOCAL_COEFFICIENTS */
	}
	void restructureConnectome() SUSUWU_OVERRIDE {
		restructureConnectomeImpl<CoefficientDefaultType>();
	}
	void setInputNeurons(size_t x) SUSUWU_OVERRIDE { /* sets connectome input count */
		if(0 == x) {
			throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::" + __func__ + "(.x = 0) { /* `" + getName() + "::" + "setupSynapses()` assumes 1 or more input neurons */ }"));
		}
		if(x != inputNeurons) {
			if(isSquareConnectome() && 0 != neuronsPerLayer && x != neuronsPerLayer) {
				throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::setInputNeurons(.x = " + std::to_string(x) + ") { isSquareConnectome() && 0 != neuronsPerLayer && x != neuronsPerLayer }"));
			}
			inputNeurons = x;
			restructureConnectome();
		}
	}
	void setOutputNeurons(size_t x) SUSUWU_OVERRIDE { /* sets connectome output count */
		if(0 == x) {
			throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::" + __func__ + "(.x = 0) { /* `" + getName() + "::" + "processTo*()` assumes 1 or more output neurons */ }"));
		}
		if(x != outputNeurons) {
			if(isSquareConnectome() && 0 != neuronsPerLayer && x != neuronsPerLayer) {
				throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::setOutputNeurons(.x = " + std::to_string(x) + ") { isSquareConnectome() && 0 != neuronsPerLayer && x != neuronsPerLayer }"));
			}
			outputNeurons = x;
			restructureConnectome();
		}
	}
	void setLayersOfNeurons(size_t x) SUSUWU_OVERRIDE { /* sets connectome "hidden layer" count */
		if(x != layersOfNeurons) {
			if(1 < layersOfNeurons) {
#if !SUSUWU_CNS_USE_MLP
				/*throw std::runtime_error*/SUSUWU_WARNING(getName() + "::setLayersOfNeurons(.x = " + std::to_string(x) + ") { (1 < layersOfNeurons) { /* TODO: `SUSUWU_CNS_USE_MLP` */ } }")/*) TODO: `throw`? */;
#endif /* ndef SUSUWU_CNS_USE_MLP */
			}
			layersOfNeurons = x;
			restructureConnectome();
		}
	}
	void setNeuronsPerLayer(size_t x) SUSUWU_OVERRIDE { /* sets connectome coefficients-per-"hidden layer" */
		if(0 == x) {
			throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::" + __func__ + "(.x = 0) { /* `" + getName() + "::" + "setupSynapses()` assumes 1 or more neurons per hidden */ }"));
		}
		if(x != neuronsPerLayer) {
			if(isSquareConnectome() && 0 != inputNeurons && x != inputNeurons) {
				throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::setNeuronsPerLayer(.x = " + std::to_string(x) + ") { isSquareConnectome() && 0 != inputNeurons && x != inputNeurons }"));
			}
			neuronsPerLayer = x;
			restructureConnectome();
		}
	}
	const size_t getParameterCount() SUSUWU_OVERRIDE {
		if(!isSquareConnectome()) {
			/*throw std::runtime_error*/SUSUWU_WARNING(getName() + "::getParameterCount() { isSquareConnectome() == false; /*Is not simple dense, square connectome. Must `override` for sparse connectomes. */ }")/*) TODO: `throw`? */;
		}
		return (neuronsPerLayer * neuronsPerLayer * layersOfNeurons) + (neuronsPerLayer * layersOfNeurons);
	}

	template<class CoefficientType,	class InputIt>
	void initCoefficient(CoefficientType &coefficientRef, InputIt inputIt, const float scale) {
#if defined(SUSUWU_CNS_TRUE_RANDOM_INIT) && SUSUWU_CNS_TRUE_RANDOM_INIT
		coefficientRef = static_cast<CoefficientType>(rand()) / static_cast<CoefficientType>(RAND_MAX) /* start with true random values. */
#elif defined(SUSUWU_CNS_HE_INIT) && SUSUWU_CNS_HE_INIT /* He initialization prevents saturation of `ReLU` functions. */
		coefficientRef = (static_cast<CoefficientType>(rand()) / static_cast<CoefficientType>(RAND_MAX) * 2 - 1);
#elif defined(SUSUWU_CNS_XAVIER_INIT) && SUSUWU_CNS_XAVIER_INIT /* Also known as "Glorot initialization", prevents saturation of `sigmoid` and `tanh` functions. */
		coefficientRef = (static_cast<CoefficientType>(rand()) / static_cast<CoefficientType>(RAND_MAX) * 2 - 1);
#elif defined(SUSUWU_CNS_BIAS_INIT) && SUSUWU_CNS_BIAS_INIT /* Biases are good approximations of constant functions */
		coefficientRef = std::get<1>(*inputIt) - std::get<0>(*inputIt) /* start with simple biases. */;
#else /* Fallback mode. Reciprocals are good approximations of linear functions. */
		coefficientRef = std::get<1>(*inputIt) / std::get<0>(*inputIt) /* start with reciprocals. */;
#endif /* !(defined(SUSUWU_CNS_TRUE_RANDOM_INIT) && SUSUWU_CNS_TRUE_RANDOM_INIT) */
		coefficientRef *= scale;
	}
	template<class CoefficientType, class InputsToOutputs>
	void pseudoRandomSynapses(const InputsToOutputs &inputsToOutputs)
#if SUSUWU_VIRTUAL_MEMBER_FUNCTION_TEMPLATES
	SUSUWU_OVERRIDE
#endif /* SUSUWU_VIRTUAL_MEMBER_FUNCTION_TEMPLATES */
	{ /* Initialize `coefficients` with pseudorandom (uses `rand()` or recipricals of `inputsToOutputs`) values */
#if !SUSUWU_CNS_LOCAL_COEFFICIENTS
		tensorflow::Tensor coefficients(tensorflow::Tensor(DataTypeToEnum<CoefficientType>::value, tensorflow::TensorShape({static_cast<DimSz>(inputNeurons), static_cast<DimSz>(neuronsPerLayer)}))); /* connectome coefficients ("synaptic strengths") */
		tensorflow::Tensor biases(tensorflow::Tensor(DataTypeToEnum<CoefficientType>::value, tensorflow::TensorShape({static_cast<DimSz>(neuronsPerLayer)}))); /* connectome biases (membrane potentials) */
#endif /* !SUSUWU_CNS_LOCAL_COEFFICIENTS */
		const auto inputBegin = inputsToOutputs.cbegin(), inputEnd = inputsToOutputs.cend();
		auto inputIt = inputBegin;
#if defined(SUSUWU_CNS_HE_INIT) && SUSUWU_CNS_HE_INIT /* He initialization prevents saturation of `ReLU` functions. */
		const float scale = sqrt(2.0f / (float)neuronsPerLayer);
#elif defined(SUSUWU_CNS_XAVIER_INIT) && SUSUWU_CNS_XAVIER_INIT /* Also known as "Glorot initialization", prevents saturation of `sigmoid` and `tanh` functions. */
		const float scale = sqrt(6.0f / (float)(neuronsPerLayer + neuronsPerLayer));
#else
		const float scale = 1.0; /* placeholder */
#endif /* !(defined(SUSUWU_CNS_HE_INIT) && SUSUWU_CNS_HE_INIT) */
		for(int i = 0; i < coefficients.dim_size(0); ++i) {
			for(int j = 0; j < coefficients.dim_size(1); ++j) {
				if(inputEnd != inputIt) {
					++inputIt;
				} else {
					inputIt = inputBegin;
				}
				initCoefficient(coefficients.tensor<CoefficientType, 2>()(i, j), inputIt, scale);
			}
		}
		TF_CHECK_OK(session->Run({
					{"randomCoefficients", coefficients}, {"randomBiases", biases}
					}, {}, {"assignCoefficients", "assignBiases"},
					SUSUWU_NULLPTR)); /* "LoadError: Tensorflow error: Status: Attempting to use uninitialized value" fix */
	}

#if SUSUWU_CNS_IF_MLP
#	pragma message("TODO: `SUSUWU_TENSORFLOWCNS_LOGITS` loop for `SUSUWU_CNS_USE_MLP`")
//	for(long w = 0; this->layersOfNeurons > w; ++w) { /* TODO */ }
#endif /* SUSUWU_CNS_IF_MLP */
#define SUSUWU_TENSORFLOWCNS_LOGITS tensorflow::ops::Add(root.WithOpName("logits"), tensorflow::ops::MatMul(root, input, coefficientsVar), biasesVar)
#define SUSUWU_TENSORFLOWCNS_INIT_SCOPE /* Simple (1 dense layer) inference model. TODO: [choose how to implement `layersOfNeurons`](https://github.com/copilot/share/427f408e-08e0-8c80-9151-f24920212857) */\
		const auto inputDim = static_cast<DimSz>(inputNeurons), outputDim = static_cast<DimSz>(outputNeurons); /* TODO: assert `1` for base types? */ \
		root = tensorflow::Scope::NewRootScope(); \
		auto input = tensorflow::ops::Placeholder(root.WithOpName("input"), /* `DataTypeToEnum<Input>::value` gives "INVALID_ARGUMENT: Inconsistent values" */coefficientFormat/*, Placeholder::Shape({-1, inputDim})*/); \
		auto coefficientsVar = tensorflow::ops::Variable(root.WithOpName("coefficients"), tensorflow::TensorShape({inputDim, outputDim}), coefficientFormat); \
		auto randomCoefficientsVar = tensorflow::ops::Variable(root.WithOpName("randomCoefficients"), tensorflow::TensorShape({inputDim, outputDim}), coefficientFormat); \
		auto biasesVar = tensorflow::ops::Variable(root.WithOpName("biases"), tensorflow::TensorShape({outputDim}), coefficientFormat); \
		auto randomBiasesVar = tensorflow::ops::Variable(root.WithOpName("randomBiases"), tensorflow::TensorShape({outputDim}), coefficientFormat); \
		auto logits = SUSUWU_TENSORFLOWCNS_LOGITS; /* TODO: ensure that `"logits"` is usable as-is for `process*()`. For classification use `"relu"`? */ \
		auto relu = tensorflow::ops::Relu(root.WithOpName("relu"), logits); /* TODO: benchmark test how accurate this is? [Rectified Linear Unit activation function](https://www.tensorflow.org/api_docs/cc/class/tensorflow/ops/relu) does; scaling, cutoff and `abs`. Gives non-linear version of `logits` (can use as input to hidden-layer `loss`). [Python version has better documents](https://www.tensorflow.org/api_docs/python/tf/keras/layers/ReLU). */
	template<class Input, class Output>
	void initScopeRootForward(tensorflow::DataType coefficientFormat) { /* Reusable graph logic (for `process*()`, `initScopeRootBack()`) */
		SUSUWU_TENSORFLOWCNS_INIT_SCOPE /* TODO: merge macro into `initScopeRootBack` */
//		auto output = tensorflow::ops::Variable(root.WithOpName("output"), {outputDim}, coefficientFormat);
//		auto process = tensorflow::ops::Assign(root.WithOpName("process"), output, logits); /* `{"output"}, {"process"}, &oTensors);` gives `status.ToString() == "INVALID_ARGUMENT: Tensor output:0, specified in either feed_devices or fetch_devices was not found in the Graph";` */

		/* Create the graph (`this->graphDef`) */
		TF_CHECK_OK(root.ToGraphDef(&graphDef)); /* produce `tensorflow::GraphDef` */
		TF_CHECK_OK(session->Create(graphDef));
	}
	template<class Input, class Output>
	void initScopeRootBack(tensorflow::DataType coefficientFormat) { /* Reusable graph logic (for `setupSynapses()`) */
		SUSUWU_TENSORFLOWCNS_INIT_SCOPE /* TODO: replace with `initScopeRootForward(coefficientFormat);`. But how to capture all local variables from functions? */

		/* Define the loss function (ergo, softmax cross-entropy) */
		auto labels = tensorflow::ops::Placeholder(root.WithOpName("labels"), /* `DataTypeToEnum<Output>::value` gives "INVALID_ARGUMENT: Inconsistent values" */coefficientFormat/*, Placeholder::Shape({-1, outputDim})*/);
		auto loss = tensorflow::ops::Mean(root, tensorflow::ops::SoftmaxCrossEntropyWithLogits(root, relu /* or `logits` */, labels), 0);

		/* Gradients (last argument, `delta`, to `tensorflow::ops::ApplyGradientDescent` */
//		auto gradients = tensorflow::ops::Gradients(root, {loss}, {coefficientsVar}); /* gives `error: no member named 'Gradients' in namespace 'tensorflow::ops'` */

		auto error = tensorflow::ops::Subtract(root, logits /* TODO: should this use `relu`? */, labels); /* shape: [batch_size, output_dim] */
		auto inputTranspose = tensorflow::ops::Transpose(root, input, {1, 0}); /* Transpose input: shape [input_dim, batch_size] */
		auto gradCoefficients = tensorflow::ops::MatMul(root, inputTranspose, error); /* MatMul: shape [input_dim, output_dim] */

		/* Scalar batch size (to divide by) */
		auto batchSize = tensorflow::ops::Shape(root, input /* With `inputDim`, does not converge. With `input`, diverges. TODO: improve */); /* shape: [batch_size, input_dim] */
		auto batchSizeScalar = tensorflow::ops::Slice(root, batchSize, {0}, {1}); /* [batch_size] */
		auto batchSizeFloat = tensorflow::ops::Cast(root, batchSizeScalar, coefficientFormat);
		auto batchSizeReduced = tensorflow::ops::Squeeze(root, batchSizeFloat); /* Reduce batchSize to scalar */

		auto two = tensorflow::ops::Const(root, 2.0f /* since this is for "derivative of squared error" */);
		auto scale = tensorflow::ops::Div(root, two, batchSizeReduced); /* `scale = 2.0 / batchSize` */

		/* Scaled grads (`ApplyGradientDescent`'s `delta` = `grad / scale`) */
		auto gradCoefficientsScaled = tensorflow::ops::Multiply(root, gradCoefficients, scale);

#if SUSUWU_CNS_IF_MLP
#	pragma message("TODO: `ApplyGradientDescent` loop for `SUSUWU_CNS_USE_MLP`")
//	for(long w = 0; this->layersOfNeurons > w; ++w) { /* TODO */ }
#endif /* SUSUWU_CNS_IF_MLP */
	//		auto optimizer = tensorflow::ops::ApplyAdam(root, coefficientsVar, biasesVar, learningFactor); /* TODO? Heard this is just part of the Python TensorFlow */
//		auto optimizer = tensorflow::ops::AdamOptimizer(learningFactor); /* TODO? */
		auto optimizerCoefficients = tensorflow::ops::ApplyGradientDescent(root.WithOpName("optimizerCoefficients"), coefficientsVar, learningFactor, gradCoefficientsScaled /* gradients[0] */); /* TODO: allow to configure this? Default is `SGD` (Stochastic Gradient Descent). */
		auto optimizerBiases = tensorflow::ops::ApplyGradientDescent(root.WithOpName("optimizerBiases"), biasesVar, learningFactor, gradBiasesScaled /* gradients[1] */); /* TODO: the derivative of `coefficients` is `n`, but the derivative of `biases` is `1`; use low-order optimizer for `biases`? */

		auto initCoefficients = tensorflow::ops::Assign(root.WithOpName("initCoefficients"), coefficientsVar, tensorflow::ops::Const(root, 0.2f /* gradients are all `0` if initial values are `0` */, tensorflow::TensorShape({inputDim, outputDim})) /* std::vector<std::vector<CoefficientType>>(inputDim, std::vector<CoefficientType>(outputDim, 0.0f)))*/ );
		auto assignCoefficients = tensorflow::ops::Assign(root.WithOpName("assignCoefficients"), coefficientsVar, randomCoefficientsVar);
		auto initBiases = tensorflow::ops::Assign(root.WithOpName("initBiases"), biasesVar, tensorflow::ops::Const(root, 0.2f, tensorflow::TensorShape({outputDim})));
		auto assignBiases = tensorflow::ops::Assign(root.WithOpName("assignBiases"), biasesVar, randomBiasesVar);
//		auto init = tensorflow::ops::InitializeVariables(root.WithOpName("init")); /* gives `error: no member named 'InitializeVariables' in namespace 'tensorflow::ops'` */

		/* Create and init the graph (`this->graphDef`) */
		TF_CHECK_OK(root.ToGraphDef(&graphDef)); /* produce `tensorflow::GraphDef` */
		TF_CHECK_OK(session->Create(graphDef));
		TF_CHECK_OK(session->Run({}, {}, {"initCoefficients"}, SUSUWU_NULLPTR)); /* Prevents "LoadError: Tensorflow error: Status: Attempting to use uninitialized value" in case `pseudoRandomSynapses` is not used. */
	}
	void printGraphNodes() const {
		SUSUWU_INFO(getName() + "::" + __func__ + "() { for(const auto &node : graphDef.node()) { ");
		for(const auto &node : graphDef.node()) {
			SUSUWU_INFO("    node.name() == \"" + node.name() + "\"; node.op == \"" + node.op() + "\";");
		}
		SUSUWU_INFO("} }");
	}

	template<class CoefficientType, class Input, class Output>
	void setupSynapsesImpl(const std::vector<std::tuple<Input /* "input" */, Output /* "label" */>> &inputsToOutputs, size_t trainingIterations) {
		const size_t inputCount = inputsToOutputs.size();
		const auto inputDim = static_cast<DimSz>(inputNeurons); /* TODO: assert `1` for base types? */
		const auto outputDim = static_cast<DimSz>(outputNeurons); /* TODO: assert `1` for base types? */
		assert(ToObjectMode<Input>::value == inputMode);
		setInitialized(false);
		tensorflow::Tensor inputTensor(DataTypeToEnum<CoefficientType /* implicit conversion from `Input` to prevent "INVALID_ARGUMENT: Inconsistent values" */>::value, tensorflow::TensorShape({static_cast<DimSz>(inputCount), static_cast<DimSz>(inputDim)})) /* backpropagation's `x` axis, which TensorFlow calls "input" */,
			expectedOutputTensor(DataTypeToEnum<CoefficientType /* implicit conversion from `Output` */>::value, tensorflow::TensorShape({static_cast<DimSz>(inputCount), static_cast<DimSz>(inputDim)})) /* backpropagation's `y` axis, which TensorFlow calls "label" */;

		auto inputTensorMapped = inputTensor.matrix<CoefficientType /* implicit conversion from `Input` */>();
		auto expectedOutputTensorMapped = expectedOutputTensor.matrix<CoefficientType /* implicit conversion from `Output` */Output>();
		inputNormsStorage = NumeralNormalizers::fromTuple<0>(inputsToOutputs);
#if SUSUWU_CLASSCNS_SEPARATE_NORMS
		outputNormsStorage = NumeralNormalizers::fromTuple<1>(inputsToOutputs);
#endif /* !SUSUWU_CLASSCNS_SEPARATE_NORMS */
		NumeralNormalizersReciprocal inputNormsRecip(inputNorms()),
			outputNormsRecip(outputNorms());
		for(size_t i = 0; i < inputCount; ++i) {
			inputTensorMapped(i, 0 /* TODO: for versions of `setupSynapses` where `std::tuple` holds `std::vector`s, replace 0 with `std::vector` index, and loop */) = numeralNormalization(std::get<0>(inputsToOutputs[i]), inputNormsRecip); /* Training input */
			expectedOutputTensorMapped(i, 0) = numeralNormalization(std::get<1>(inputsToOutputs[i]), outputNormsRecip); /* Expected output */
		}

		/* Define the neural network architecture */
		restructureConnectomeImpl<CoefficientType>(); /* TODO: move `Scope` construction into this? Move `GraphDef` construction into this? */
//		auto coefficientsMapped = coefficients.matrix<CoefficientType>(); /* TODO? https://poe.com/s/kMLk4ogWhaNWvpIsHruCoefficients */
		initScopeRootBack<Input, Output>(DataTypeToEnum<CoefficientType>::value);
		pseudoRandomSynapses<CoefficientType>(inputsToOutputs); /* Initialize `"coefficients"` with pseudorandom (uses `rand()` or recipricals of `inputsToOutputs`) values */

		if(0 == trainingIterations) {
			trainingIterations = 1000; /* TODO: use input specifics (and available host resources?) to compute best value */
		}
		for(size_t epoch = 0; epoch < trainingIterations; ++epoch) {
			std::vector<tensorflow::Tensor> outputs;
			const tensorflow::Status status = session->Run(
				{
					{"input", inputTensor},
					{"labels", expectedOutputTensor}
				},
				{/* outputTensors */},
				{"optimizerCoefficients"},
				&outputs /* TODO: unused, remove? */
			);
			if(!status.ok()) {
				throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::" + __func__ + "() { const tensorflow::Status status = session->Run({{\"input\", inputTensor}, {\"labels\", expectedOutputTensor}, {}, {\"optimizerW\"}, &outputs); (!status.ok()) { epoch == " + std::to_string(epoch) + "; status.ToString() == \"" + status.ToString() + "\"; } } }"));
			}
		}
		setupSynapsesPostProcess();
	}
	void setupSynapsesPostProcess() SUSUWU_OVERRIDE {
		if(0 == graphDef.node_size()) {
			SUSUWU_WARNING(getName() + "::" + __func__ + "() { if(0 == graphDef.node_size()) { /* This path is unreachable unless `setupSynapsesPostProcess()` is called by user code, or is overridden but `setupSynapses()` is not overridden (such as if an unimplemented type of `setupSynapses()` was used) */ } } ");
		} else {
#if SUSUWU_CNS_LOCAL_COEFFICIENTS /* Store model values into `class TensorFlowCns`. */
			std::vector<tensorflow::Tensor> outputs;
			const tensorflow::Status status = session->Run({}, {"coefficients", "biases"}, {}, &outputs);
			if(!status.ok()) {
				printGraphNodes();
				throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::" + __func__ + "() { const tensorflow::Status status = session->Run({}, {\"coefficients\", \"biases\"}, {}, &outputs); (!status.ok()) { outputs.size() == " + std::to_string(outputs.size()) + "; status.ToString() == \"" + status.ToString() + "\"; } }"));
			}
			this->coefficients = outputs[0];
			this->biases = outputs[1];
#endif /* SUSUWU_CNS_LOCAL_COEFFICIENTS */
			setInitialized(true);
		}
	}

	/* Also known as "backpropagation" or "gradient descent". `float` to `float` version.
	 * @throw std::runtime_error, tensorflow::errors::Internal, tensorflow::errors::Unavailable */
	void setupSynapses(const std::vector<std::tuple<float /* "input" */, float /* "label" */>> &inputsToOutputs, size_t trainingIterations = 0 /* if 0, guesses suitable loop count */) SUSUWU_OVERRIDE { /* NOLINT(google-default-arguments): is 0 for base and derivatives */
		setupSynapsesImpl<CoefficientDefaultType, float, float>(inputsToOutputs, trainingIterations);
	}

	/* Also known as "backpropagation" or "gradient descent". `int` to `int` version.
	 * @throw std::runtime_error, tensorflow::errors::Internal, tensorflow::errors::Unavailable */
	void setupSynapses(const std::vector<std::tuple<int /* "input" */, int /* "label" */>> &inputsToOutputs, size_t trainingIterations = 0 /* if 0, guesses suitable loop count */) SUSUWU_OVERRIDE { /* NOLINT(google-default-arguments): is 0 for base and derivatives */
		setupSynapsesImpl<CoefficientDefaultType, int, int>(inputsToOutputs, trainingIterations);
	}

	/* Helper function for `processTo*()`. For internal use.
	 * @throw std::runtime_error, tensorflow::errors::Internal, tensorflow::errors::Unavailable */
	template<class CoefficientType, class Output>
	const std::vector<tensorflow::Tensor> processToImpl(const tensorflow::Tensor &inputTensor, const std::string &func) const {
		if(ToObjectMode<Output>::value != outputMode) {
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::" + func + "() { outputMode == " + std::to_string(outputMode) + "; outputMode != " + std::to_string(ToObjectMode<Output>::value) + " /* ToObjectMode<Output>::value */; }"));
		}
//		initScopeRootForward<Input, Output>(DataTypeToEnum<CoefficientType>::value); /* TODO: uncomment if `root` is set to `mutable` */
		std::vector<tensorflow::Tensor> oTensors;
		const tensorflow::Status status = session->Run(
			{{"input", inputTensor}/*, {"coefficients", coefficients}, {"biases", biases} */},
			{"logits"}, {"logits"}, &oTensors);
		if(!status.ok()) {
			const std::string coefficientStr = "/*, {\"coefficients\", coefficients}, {\"biases\", biases}*/"; /* Notice: can set to "" */
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::" + func + "() { const tensorflow::Status status = session->Run({{\"input\", inputTensor}" + coefficientStr + "}, {\"output\"}, {}, &oTensors); (!status.ok()) { status.ToString() == \"" + status.ToString() + "\"; } }"));
		}
		return oTensors; //return static_cast<Output /* cannot cast to `std::string` */>(oTensors[0].matrix<CoefficientType>()(0, 0));
	}

	/* Also known as "forwardpropagation" or "inference". `float` version.
	 * @throw std::runtime_error, tensorflow::errors::Internal, tensorflow::errors::Unavailable */
	const float processToFloat(const float &input) const SUSUWU_OVERRIDE {
		tensorflow::Tensor inputTensor(DataTypeToEnum<CoefficientDefaultType /* If `CoefficientDefaultType` switches to `double`, `float` could give "INVALID_ARGUMENT: Inconsistent values" */>::value, tensorflow::TensorShape({1, 1}));
		inputTensor.matrix<CoefficientDefaultType>()(0, 0) = numeralNormalization(input, inputNorms());
		auto oTensors = processToImpl<CoefficientDefaultType, float>(inputTensor, __func__);
		return static_cast<float>(numeralDenormalization(oTensors[0].matrix<CoefficientDefaultType /* `float` */>()(0, 0), outputNorms()));
	}

	/* Also known as "forwardpropagation" or "inference". `int` version.
	 * @throw std::runtime_error, tensorflow::errors::Internal, tensorflow::errors::Unavailable */
	const int processToInt(const int &input) const SUSUWU_OVERRIDE {
		tensorflow::Tensor inputTensor(DataTypeToEnum<CoefficientDefaultType /* `int` gives "INVALID_ARGUMENT: Inconsistent values" */>::value, tensorflow::TensorShape({1, 1}));
		inputTensor.matrix<CoefficientDefaultType>()(0, 0) = numeralNormalization(input, inputNorms());
		auto oTensors = processToImpl<CoefficientDefaultType, int>(inputTensor, __func__);
		return static_cast<int>(numeralDenormalization(oTensors[0].matrix<CoefficientDefaultType /* `int` */>()(0, 0), outputNorms()));
	}

	/* Also known as "forwardpropagation" or "inference". `std::string` version. */
	const std::string processToString(const std::string &input) const SUSUWU_OVERRIDE {
//		tensorflow::Tensor inputTensor(tensorflow::DT_STRING, tensorflow::TensorShape({1, 1})); /* Alternative? [`tensorflow::ops::DecodeRaw`](https://www.tensorflow.org/api_docs/cc/class/tensorflow/ops/decode-raw) into tensor of `char`s */
//		inputTensor.matrix<tensorflow::tstring>()(0, 0) = input; /* `<std::string>` gives `tensorflow/core/framework/types.h:336:3: error: static_assert failed due to requirement 'IsValidDataType<std::basic_string<char, std::char_traits<char>, std::allocator<char>>>::value' "Specified Data Type not supported"` */
		tensorflow::Tensor inputTensor(DataTypeToEnum<CoefficientDefaultType>::value, tensorflow::TensorShape({1, 1} /* TODO: map sentences into tokens */));
		inputTensor.matrix<CoefficientDefaultType>()(0, 0) = std::stof(input /* map text number into `float` */);
		auto oTensors = processToImpl<CoefficientDefaultType, tensorflow::tstring>(inputTensor, __func__);
		return std::to_string(oTensors[0].matrix<CoefficientDefaultType /* `std::string` gives "INVALID_ARGUMENT: Inconsistent values" */>()(0, 0)); /* TODO: tokens, which map into sentences */
	}

protected: /* NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes) */
	tensorflow::Session* session = SUSUWU_NULLPTR;
	tensorflow::Scope root; /* TODO: If not used after init, move this into `setupRootScope()` */
	tensorflow::GraphDef graphDef; /* TODO: If not used after init, move this into `setupRootScope()`. */
#if SUSUWU_CNS_LOCAL_COEFFICIENTS
	tensorflow::Tensor coefficients = tensorflow::Tensor(tensorflow::DT_FLOAT, tensorflow::TensorShape({static_cast<DimSz>(inputNeurons), static_cast<DimSz>(neuronsPerLayer)})); /* connectome coefficients ("synaptic strengths") */
	tensorflow::Tensor biases = tensorflow::Tensor(tensorflow::DT_FLOAT, tensorflow::TensorShape({static_cast<DimSz>(neuronsPerLayer)})); /* connectome biases ("membrane potentials") */
#endif /* SUSUWU_CNS_LOCAL_COEFFICIENTS */
/* NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes) */
};

}; /* namespace Susuwu */
#endif /* def SUSUWU_USE_TENSORFLOW */
#endif /* ndef INCLUDES_cxx_ClassTensorFlowCns_hxx */

