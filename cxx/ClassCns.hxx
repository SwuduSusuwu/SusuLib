/* Attribution (henceforth "*this attribution*", whose syntax is *Markdown*): 2024 [Swudu Susuwu](https://swudususuwu.substack.com)
 * <https://github.com/SwuduSusuwu/SusuLib/> has the newest version of `./cxx/ClassCns.hxx` (henceforth "*this source code*").
 * If *this attribution* is shown, *this source code* allows all uses. *This attribution* constitutes the most permissive which is compatible with [*GPLv2*](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) + [*Apache 2*](https://www.apache.org/licenses/LICENSE-2.0.html), which is suitable for personal use (also suitable for school use).
 * If *this attribution* is not professional enough for business use: businesses can use *this source code* through included versions of [*GPLv2*](./LICENSE_GPLv2), [*Apache 2*](./LICENSE), or through both of those. */
#pragma once
#ifndef INCLUDES_cxx_ClassCns_hxx
#define INCLUDES_cxx_ClassCns_hxx
#include "ClassIo.hxx" /* ClassIoPath */
#include "ClassNumeral.hxx" /* NumeralNormalizers */
#include "ClassObject.hxx" /* Object ObjectMode SUSUWU_PURE_VIRTUAL_DEFAULTS() ToObjectMode */
#include "Macros.hxx" /* SUSUWU_IF_CPLUSPLUS SUSUWU_INTPTR SUSUWU_DEFAULT SUSUWU_NOEXCEPT SUSUWU_OVERRIDE */
#include SUSUWU_IF_CPLUSPLUS(<cassert>, <assert.h>) /* assert */
#include SUSUWU_IF_CPLUSPLUS(<cstddef>, <stddef.h>) /* size_t */
#include <functional> /* std::function */
#include <limits> /* std::numeric_limits */
#include <stdexcept> /* std::invalid_argument std::runtime_error */
#include <string> /* std::string std::to_string */
#include <tuple> /* std::tuple */
#include <vector> /* std::vector */
#ifndef SUSUWU_CNS_SEPARATE_NORMS
#	define SUSUWU_CNS_SEPARATE_NORMS false
#endif /* ndef SUSUWU_CNS_SEPARATE_NORMS */

namespace Susuwu {
#ifndef SUSUWU_CNS_VALUE_SEMANTICS
#	define SUSUWU_CNS_VALUE_SEMANTICS true
#endif /* ndef SUSUWU_CNS_VALUE_SEMANTICS */
#define SUSUWU_CNS_MODE_ENUM /* use `enum`s for multi-class indices */
typedef class Cns : public Object {
public:
	Cns() SUSUWU_DEFAULT /* Default constructor */
	Cns(const Cns &) SUSUWU_DEFAULT /* Copy constructor */
	Cns& operator=(const Cns &) SUSUWU_DEFAULT /* Copy assignment */
	Cns(Cns&&) SUSUWU_NOEXCEPT SUSUWU_DEFAULT /* Move constructor */
	Cns& operator=(Cns &&) SUSUWU_NOEXCEPT SUSUWU_DEFAULT /* Move assignment */
	~Cns() SUSUWU_OVERRIDE SUSUWU_DEFAULT
#if SUSUWU_CNS_VALUE_SEMANTICS
#	undef SUSUWU_CLASS_OPERATOREQUALTO
#	define SUSUWU_CLASS_OPERATOREQUALTO(noop) ; /* NOLINT(cppcoreguidelines-macro-usage) */
	bool operator==(const Class &obj) const SUSUWU_OVERRIDE { /* NOLINT(fuchsia-overloaded-operator) */
//		SUSUWU_VIRTUAL_OPERATOREQUALTO_WITH_VPTR /* shallow comparison, with virtual pointer included */
//		return this->hashCode() == obj.hashCode();
		const Cns *thisFlow = dynamic_cast<const Cns *>(this);
		const Cns *objFlow = dynamic_cast<const Cns *>(&obj);
		return typeid(*this) == typeid(obj) &&
			thisFlow->inputMode == objFlow->inputMode &&
			thisFlow->outputMode == objFlow->outputMode &&
			thisFlow->inputNeurons == objFlow->inputNeurons &&
			thisFlow->outputNeurons == objFlow->outputNeurons &&
			thisFlow->neuronsPerLayer == objFlow->neuronsPerLayer &&
			thisFlow->layersOfNeurons == objFlow->layersOfNeurons;
	}
	const bool equals(const Object &obj) const SUSUWU_OVERRIDE { return this->getClass().operator==(obj); }
	const SUSUWU_INTPTR hashCode() const SUSUWU_OVERRIDE SUSUWU_VIRTUAL_HASHCODE /* Shallow hash code. TODO: ensure that derivatives of `class Cns` override this to hash container values */
#endif /* SUSUWU_CNS_VALUE_SEMANTICS */
	SUSUWU_PURE_VIRTUAL_DEFAULTS(Susuwu::Cns) /* `getName()`, `isPureVirtual()`, `operator==()`, ... */
	const bool isInitialized() const SUSUWU_OVERRIDE { return initialized; } /* if can do "inference" (ergo "forwardpropagation"; `process*`) */
	virtual void setInitialized(const bool is) { initialized = is; } /* after "training" (ergo "backpropagation") finishes, set to `true` */

	/* Topological values; methods which set the "shape" of `Cns.synapses` (or of whatever the derived class uses to store the connectome) */
	virtual const bool isSquareConnectome() const { return false; } /* returns `true` if the implementation requires `inputNeurons == outputNeurons == neuronsPerLayer` (square layout does not support all uses, but is most simple to code and allows the most [SIMD](../posts/SimdGpgpuTpu.md) */
//	virtual bool setSquareConnectome(const bool is) { return false; } /* returns `true` (and sets the squareness to `is`) if the implementation has customizable squareness (dynamic code paths for square versus non-square connectomes) */
	virtual void setSquareConnectome(const bool is) { throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::setSquareConnectome(" + std::to_string(is) + ") { /* implementation does not support setting this */ }")); } /* if the implementation has customizable squareness (dynamic code paths for square versus non-square connectomes), sets the squareness to `is`. */
	virtual void restructureConnectome() { setInitialized(false); /* stub, since root class has no `synapses` tensor */ } /* updates (or rebuilds) connectome (tensor of coefficients and/or biases) to match new topological layout */
	virtual void setInputMode(ObjectMode x) { inputMode = x; } /* sets type of input */
	virtual void setOutputMode(ObjectMode x) { outputMode = x; } /* sets type of output (notice: some implementations require `inputMode == outputMode`) */
	virtual void setInputNeurons(size_t x) { /* sets connectome input count */
		if(x != inputNeurons) {
			if(isSquareConnectome() && 0 != neuronsPerLayer && x != neuronsPerLayer) {
				throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::setInputNeurons(.x = " + std::to_string(x) + ") { isSquareConnectome() && 0 != neuronsPerLayer && x != neuronsPerLayer }"));
			}
			inputNeurons = x;
			restructureConnectome();
		}
	}
	virtual void setOutputNeurons(size_t x) { /* sets connectome output count */
		if(x != outputNeurons) {
			if(isSquareConnectome() && 0 != neuronsPerLayer && x != neuronsPerLayer) {
				throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::setOutputNeurons(.x = " + std::to_string(x) + ") { isSquareConnectome() && 0 != neuronsPerLayer && x != neuronsPerLayer }"));
			}
			outputNeurons = x;
			restructureConnectome();
		}
	}
	virtual void setLayersOfNeurons(size_t x) { /* sets connectome "hidden layer" count */
		if(x != layersOfNeurons) {
			layersOfNeurons = x;
			restructureConnectome();
		}
	}
	virtual void setNeuronsPerLayer(size_t x) { /* sets connectome coefficients-per-"hidden layer" */
		if(x != neuronsPerLayer) {
			if(isSquareConnectome() && 0 != inputNeurons && x != inputNeurons) {
				throw std::invalid_argument(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::setNeuronsPerLayer(.x = " + std::to_string(x) + ") { isSquareConnectome() && 0 != inputNeurons && x != inputNeurons }"));
			}
			neuronsPerLayer = x;
			restructureConnectome();
		}
	}
	virtual const size_t getParameterCount() { /* Notice: must `override` if architecture is sparse. Must `override` to count constant biases. */
		if(!isSquareConnectome()) {
			/*throw std::runtime_error*/SUSUWU_WARNING(getName() + "::getParameterCount() { isSquareConnectome() == false; /*Is not simple dense, square connectome. Must `override` for sparse connectomes. */ }")/*) TODO: `throw`? */;
		}
		return neuronsPerLayer * neuronsPerLayer * layersOfNeurons; /* default is; counts 1 coefficient per input to next layer of neurons */
	}

/* NOLINTBEGIN(google-default-arguments): derivative classes use our default values */
	/* Compute values; setters of training momentum (and similar arguments) which the derivative classes use */
	virtual void setLearningFactor(ObjectMode x) {
		if(0 > learningFactor || 1 < learningFactor) {
			throw std::invalid_argument("ClassCns::setLearningFactor(\"" + std::to_string(x) + "\") out of bounds");
		}
		learningFactor = x; /* how much coefficients move (converge) per step of `setupSynapses` training loop. `0.0` is static, `1.0` is straight to the first reciprocal (or whatever form of gradient the optimizer uses) */
	}

	/* dump the connectome (the `tensorflow::Tensor` of synapse coefficients)
	 * @throw std::runtime_error */
	virtual void dumpTo(const ClassIoPath &modelPath) const { throw std::runtime_error("ClassCns::dumpTo(\"" + modelPath + "\") pure virtual call"); }
	/* load the connectome (the `tensorflow::Tensor` of synapse coefficients)
	 * @throw std::runtime_error */
	virtual void loadFrom(const ClassIoPath &modelPath) { throw std::runtime_error("ClassCns::loadFrom(\"" + modelPath + "\") pure virtual call"); }

	/* Internal function, which outros of `setupSynapses()` use.
	 * @throw std::bad_alloc std::runtime_error
	 * @pre @code !isPureVirtual() @endcode
	 * @post @code isInitialized() @endcode */
	virtual void setupSynapsesPostProcess() { setInitialized(true); } /* `override` with code common to all `setupSynapses`. */
#if SUSUWU_VIRTUAL_MEMBER_FUNCTION_TEMPLATES /* C++ does not support templates of virtual functions ( https://stackoverflow.com/a/78440416/24473928 ) */
	/* Initialize `Cns.synapses` with pseudorandom (uses `rand()` or recipricals of `inputsToOutputs`) values
	 * @throw std::runtime_error */
	template<class InputsToOutputs>
	virtual void pseudoRandomSynapses(const InputsToOutputs &inputsToOutputs) { throw std::runtime_error("ClassCns::pseudoRandomSynapses() pure virtual call"); }
	/* @throw bad_alloc
	 * @pre @code !isPureVirtual() @endcode */
	template<typename Input, typename Output>
	virtual void setupSynapses(std::vector<std::tuple<Input, Output>> inputsToOutputs, size_t trainingIterations = 0 /* if 0, guesses suitable loop count */); /* { restructureConnectome(); inputMode = ToObjectMode<Input>::value; outMode = ToObjectMode<Output>::value; setupSynapsesPostProcess(); throw std::runtime_error("ClassCns::setupSynapses() pure virtual call"); } */
	/* @pre @code isInitialized() @endcode */
	template<typename Input, typename Output>
	virtual const Output process(const Input input) const {
		assert(ToObjectMode<Input>::value == inputMode && ToObjectMode<Output>::value == outputMode);
		throw std::runtime_error("ClassCns::process() pure virtual call");
	}
#else /* !SUSUWU_VIRTUAL_MEMBER_FUNCTION_TEMPLATES */
/* NOLINTBEGIN(cppcoreguidelines-macro-usage,misc-unused-parameters): no virtual templates (must use macros), unused parameters are due to the fact this is a stub */
#	define SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, OUTPUT_TYPEDEF) \
	/* Initialize `Cns.synapses` with pseudorandom (uses `rand()` or recipricals of `inputsToOutputs`) values \
	 * @throw std::runtime_error */ \
	virtual void pseudoRandomSynapses(const std::vector<std::tuple<INPUT_TYPEDEF, OUTPUT_TYPEDEF>> &inputsToOutputs) { throw std::runtime_error("ClassCns::pseudoRandomSynapses() pure virtual call"); } /* NOLINT(bugprone-macro-parentheses): parentheses cause "error: expected expression [clang-diagnostic-error]" */ \
	/* @throw bad_alloc \
	 * @pre @code !isPureVirtual() @endcode \
	 * @post @code isInitialized() @endcode */\
	virtual void setupSynapses(const std::vector<std::tuple<INPUT_TYPEDEF, OUTPUT_TYPEDEF>> &inputsToOutputs, size_t trainingIterations = 0 /* if 0, guesses suitable loop count */) { restructureConnectome(); inputMode = ToObjectMode<INPUT_TYPEDEF>::value; outputMode = ToObjectMode<OUTPUT_TYPEDEF>::value; setupSynapsesPostProcess(); /* stub so unit tests pass */ } /* NOLINT(bugprone-macro-parentheses) */
#	define SUSUWU_TEMPLATE_WORKAROUND(INPUT_TYPEDEF) \
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, bool)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, char)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, int)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, unsigned int)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, float)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, double)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, std::vector<bool>)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, std::vector<char>)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, std::vector<int>)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, std::vector<unsigned int>)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, std::vector<float>)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, std::vector<double>)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, std::string)\
	\
	/* @pre @code isInitialized() @endcode */\
	virtual const bool processToBool(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeBool == outputMode); return 0; }\
	virtual const char processToChar(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeChar == outputMode); return 0; }\
	virtual const int processToInt(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeInt == outputMode); return 0; }\
	virtual const unsigned int processToUint(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeUint == outputMode); return 0; }\
	virtual const float processToFloat(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeFloat == outputMode); return 0; }\
	virtual const double processToDouble(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeDouble == outputMode); return 0; }\
	virtual const std::vector<bool> processToVectorBool(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeVectorBool == outputMode); return {}; }\
	virtual const std::vector<char> processToVectorChar(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeVectorChar == outputMode); return {}; }\
	virtual const std::vector<int> processToVectorInt(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeVectorInt == outputMode); return {}; }\
	virtual const std::vector<unsigned int> processToVectorUint(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeVectorUint == outputMode); return {}; }\
	virtual std::vector<float> processToVectorFloat(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeVectorFloat == outputMode); return {}; }\
	virtual const std::vector<double> processToVectorDouble(const INPUT_TYPEDEF &input) const { assert((ToObjectMode<INPUT_TYPEDEF>::value) == inputMode && objectModeVectorDouble == outputMode); return {}; }\
	virtual const std::string processToString(const INPUT_TYPEDEF &input) const { auto val = processToVectorChar(input); return std::string(&val[0], val.size()); }
	SUSUWU_TEMPLATE_WORKAROUND(bool)
	SUSUWU_TEMPLATE_WORKAROUND(char)
	SUSUWU_TEMPLATE_WORKAROUND(int)
	SUSUWU_TEMPLATE_WORKAROUND(unsigned int)
	SUSUWU_TEMPLATE_WORKAROUND(float)
	SUSUWU_TEMPLATE_WORKAROUND(double)
	SUSUWU_TEMPLATE_WORKAROUND(std::vector<bool>)
	SUSUWU_TEMPLATE_WORKAROUND(std::vector<char>)
	SUSUWU_TEMPLATE_WORKAROUND(std::vector<int>)
	SUSUWU_TEMPLATE_WORKAROUND(std::vector<unsigned int>)
	SUSUWU_TEMPLATE_WORKAROUND(std::vector<float>)
	SUSUWU_TEMPLATE_WORKAROUND(std::vector<double>)
	SUSUWU_TEMPLATE_WORKAROUND(std::string)
#	undef SUSUWU_TEMPLATE_WORKAROUND
	/* NOLINTEND(cppcoreguidelines-macro-usage,misc-unused-parameters) */
#endif /* !SUSUWU_VIRTUAL_MEMBER_FUNCTION_TEMPLATES */
/* NOLINTEND(google-default-arguments) */
	const NumeralNormalizers &inputNorms() const {
		return inputNormsStorage;
	}
	const NumeralNormalizers &outputNorms() const {
#if SUSUWU_CNS_SEPARATE_NORMS
		return outputNormsStorage;
#else /* else !SUSUWU_CNS_SEPARATE_NORMS */
		return inputNormsStorage;
#endif /* !SUSUWU_CNS_SEPARATE_NORMS */
	}
protected: /* NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers) */
	NumeralNormalizers inputNormsStorage; /* store inputNorms which `setupSynapses` ("training" / backpropagation) used, so that `processTo*` ("inference" / forwardpropagation) can reuse those */
#if SUSUWU_CNS_SEPARATE_NORMS
	NumeralNormalizers outputNormsStorage; /* separate normalization factors for "labels" (for expected output values) */
#endif /* !SUSUWU_CNS_SEPARATE_NORMS */
	ObjectMode inputMode = objectModeBool, outputMode = objectModeBool;
	size_t inputNeurons = 0, outputNeurons = 0, layersOfNeurons = 0, neuronsPerLayer = 0;
	size_t patience = 10; float minLossDelta = 0.001; /* `setupSynapses()`'s minimum per-iterations-improvement; abort (undefined if this is "success" or "failure") if the loss function is less than `minLossDelta` for `patience` iterations in a row */ /* TODO: include `setMinLossDelta()`, or turn `patience` and `minLossDelta` into arguments to `setupSynapses()` */
	float desiredLossThreshold = 0.01; /* return (success) from `setupSynapses()` before `trainingIterations` if the loss function goes below this value */ /* TODO: include `setDesiredLossThreshold()`, or turn `desiredLossThreshold` into an argument to `setupSynapses()` */
	float validationFactor = 0.2; /* so overfitting (convergence on non-generalizable patterns) is prevented, reserve this much of `inputsToOutputs` for validation (to compute true loss) */
	float learningFactor = 0.02;
	bool initialized = false;
/* NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers) */
} Cns;

#ifdef USE_HSOM_CNS
typedef class HsomCns : public Cns {
/* Work-in-progress (`ClassCns.cxx` for more information): `HSOM` is simple Python-based CNS from https://github.com/CarsonScott/HSOM
 * Examples of howto setup `HSOM` as artificial CNS; https://github.com/CarsonScott/HSOM/tree/master/examples
 * [ https://stackoverflow.com/questions/3286448/calling-a-python-method-from-c-c-and-extracting-its-return-value ] suggests various syntaxes to use for this, with unanswered comments such as "Does this support classes?"
 */
} HsomCns;
#endif /* USE_HSOM_CNS */

#ifdef USE_APXR_CNS
typedef class ApxrCns : public Cns {
/* Work-in-progress (`ClassCns.cxx for more information): `apxr` is complex Erlang-based CNS from https://github.com/Rober-t/apxr_run/
 * Examples of howto setup `apxr` as artificial CNS; https://github.com/Rober-t/apxr_run/blob/master/src/examples/
 * "apxr_run" has various FLOSS neural network activation functions (absolute, average, standard deviation, sqrt, sin, tanh, log, sigmoid, cos), plus sensor functions (vector difference, quadratic, multiquadric, saturation [+D-zone], gaussian, cartesian/planar/polar distances): https://github.com/Rober-t/apxr_run/blob/master/src/lib/functions.erl
 * Various FLOSS neuroplastic functions (self-modulation, Hebbian function, Oja's function): https://github.com/Rober-t/apxr_run/blob/master/src/lib/plasticity.erl
 * Various FLOSS neural network input aggregator functions (dot products, product of differences, mult products): https://github.com/Rober-t/apxr_run/blob/master/src/agent_mgr/signal_aggregator.erl
 * Various simulated-annealing functions for artificial neural networks (dynamic [+ random], active [+ random], current [+ random], all [+ random]): https://github.com/Rober-t/apxr_run/blob/master/src/lib/tuning_selection.erl
 * Choices to evolve connections through Darwinian or Lamarkian formulas: https://github.com/Rober-t/apxr_run/blob/master/src/agent_mgr/neuron.erl
 */
} ApxrCns;
#endif /* USE_APXR_CNS */

template <class CnsSub, typename CnsSubCoefficient, class Input, class Output, typename Process>
static const bool classCnsTestsLinear(const bool abortOnFirstError, const Input min, const Input max, const Input step, const CnsSubCoefficient epsilon /* For now, `epsilon` is best result known (so that regressions trigger debug messages) */, Process process, const bool cnsIsValueObject, const bool cnsSubHasDumpTo) {
	bool success = true;
	const std::string debugMessage = "classCnsTests(.min = " + std::to_string(min) + ", .max = " + std::to_string(max) + ", .step = " + std::to_string(step) + ", .epsilon == " + std::to_string(epsilon) + ") { ";
	std::vector<std::tuple<Input, Output>> inputsToOutputs;
//	 epsilon = std::numeric_limits<CnsSubCoefficient>::epsilon /* TODO: [compute most accurate possible values](https://github.com/copilot/share/c056538e-08c0-8822-9001-720924696114) */
	for(Input input = min; max >= input; input += step) {
		inputsToOutputs.push_back({input, input * 2});
	}
	CnsSub cnsSub;
	Cns &cns = cnsSub;
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
			CnsSub cnsSub2;
			if(cnsIsValueObject) { /* if comparison is not limited to object addresses */
				cnsSub2 = cnsSub;
			}
			assert(inputsCount == inputsToOutputs.size());
			Output bestLoss = std::numeric_limits<Output>::max(),
				worstLoss = 0;
			for(unsigned index = 0; inputsToOutputs.size() > index; ++index) {
				const Output label = std::get<1>(inputsToOutputs.at(index)),
					output = process(cns, std::get<0>(inputsToOutputs.at(index))),
					loss = label - output,
					absLoss = abs(loss);
				if(absLoss < bestLoss) {
					bestLoss = absLoss;
				}
				if(absLoss > worstLoss) {
					worstLoss = absLoss;
				}
				if(!(epsilon >= absLoss) /* inverted test (with `!`) catches `nan` */) {
					const std::string errorMessage = debugMessage + "bestLoss == " + std::to_string(bestLoss) + "; label[" + std::to_string(index) + "] == " + std::to_string(label) + "; output[" + std::to_string(index) + "] == " + std::to_string(output) + "; loss /* label - output */ == " + std::to_string(loss) + "; if(!(abs(loss) <= epsilon)) { throw std::runtime_error; } }";
					if(abortOnFirstError) {
						throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, errorMessage));
					} else { /* so all warnings show */
						SUSUWU_WARNING(errorMessage);
					}
					success = false;
				}
			}
			if(epsilon > (1 + worstLoss)) { /* cppcheck-suppress duplicateBranch */
				SUSUWU_DEBUG(debugMessage + "bestLoss == " + std::to_string(bestLoss) + "; worstLoss == " + std::to_string(worstLoss) + "; /* `(epsilon > (1 + worstLoss))`, set `epsilon = worstLoss` to detect future regressions */; }");
			} else {
				SUSUWU_DEBUG(debugMessage + "bestLoss == " + std::to_string(bestLoss) + "; worstLoss == " + std::to_string(worstLoss) + "; }");
			}

			if(cnsIsValueObject) { /* test that `process()` follows `const` method rules, and that `equals()` and `hashCode` do what those are supposed to do */
				Cns const &cns2 = cnsSub2;
				assert(cns.equals(cns2));
				assert(cns.hashCode() == cns2.hashCode()); /* cppcheck-suppress knownConditionTrueFalse */
			}
		}

		if(cnsSubHasDumpTo) {
			CnsSub cnsSub2 = cnsSub;
			Cns &cns2 = cnsSub2;
			const ClassIoPath dumpToPath = "classCnsTests.tmp"; /* TODO: if file exists, ask user what to do? TODO: use system temp path? */
			cns.dumpTo(dumpToPath);
			cns2.loadFrom(dumpToPath);
//			classIoRemove(dumpToPath); /* TODO: does our IO library have a function for file removal? Perhaps there is some `fopen` flag to use instead, which causes removal as soon as the file is closed? */
			assert(cns.equals(cns2));
			assert(cns.hashCode() == cns2.hashCode()); /* cppcheck-suppress knownConditionTrueFalse */
		}

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
	return success;
}
template<class Input, class CnsSubCoefficient>
struct ClassCnsTestsLinearArgus { /* for internal use */
	Input min; Input max; Input step;
	CnsSubCoefficient epsilon; /* For now, `epsilon` is best result known (so that regressions trigger debug messages) */
};
template<class CnsSub, class CnsSubCoefficient>
const bool classCnsTests(const bool cnsIsValueObject, const bool cnsSubHasDumpTo) {
	const bool abortOnFirstError = false;
	bool success = true;
	std::function<const float(const Cns &, const float)> processToFloatLambda = [](const Cns &cns, const float x) { return cns.processToFloat(x); };
	std::function<const int(const Cns &, const int)> processToIntLambda = [](const Cns &cns, const int x) { return cns.processToInt(x); };
	std::vector<struct ClassCnsTestsLinearArgus<float, CnsSubCoefficient>> testsLinearFloatArgus = {
		{-1.0,    1.0,    0.001, /* 0.072094 ...  */  0.1180462}, /* "normalization" (average == 0, std == 1), most simple to learn */
		{-1000.0, 1000.0, 1.0,   /* 70.774782 ... */ 107.8262  }, /* (average == 0, std == 1000) */
		{ 0.0,    2.0,    0.001, /* 0.072054 ...  */  0.118012 }, /* (average == 1, std == 1) */
		{ 0.0,    2000.0, 1.0,   /* 70.698730 ... */ 117.921631}  /* (average == 1000, std == 1000) */
	};                         /* absLoss min...*/ /* ..max */
	for(const auto &argus: testsLinearFloatArgus) {
		if(!classCnsTestsLinear<CnsSub, CnsSubCoefficient, float, float>(abortOnFirstError, argus.min, argus.max, argus.step, argus.epsilon, processToFloatLambda, cnsIsValueObject, cnsSubHasDumpTo)) {
			if(abortOnFirstError) { return false; }
			success = false;
		}
	}
	std::vector<struct ClassCnsTestsLinearArgus<int, CnsSubCoefficient>> testsLinearIntArgus = {
		{-1000, 1000, 1, /* 1.563975 ... */ 2242},
		{ 0   , 2000, 1, /* 2.443702 ... */ 2552}
	};                 /* `lossVal`    */ /* TODO: reduce integer-version `epsilons` */
	for(const auto &argus: testsLinearIntArgus) {
		if(!classCnsTestsLinear<CnsSub, CnsSubCoefficient, int, int>(abortOnFirstError, argus.min, argus.max, argus.step, argus.epsilon, processToIntLambda, cnsIsValueObject, cnsSubHasDumpTo)) {
			if(abortOnFirstError) { return false; }
			success = false;
		}
	}
	return success;
}

/* Related to this:
 * [Howto use `class Cns` for virus analysis and digital assistants](https://github.com/SwuduSusuwu/SusuLib/blob/preview/posts/VirusAnalysis.md) [2](https://swudususuwu.substack.com/p/howto-improve-virus-analysis) (demos; `#include "VirusAnalysis.hxx"` and `#include "AssistantCns.hxx"`)
 * [How to: throughput improved](https://github.com/SwuduSusuwu/SusuLib/blob/experimental/posts/SimdGpgpuTpu.md) [2](https://swudususuwu.substack.com/p/howto-run-devices-phones-laptops)
 * [Alternative CNS's architectures](https://github.com/SwuduSusuwu/SusuLib/blob/experimental/posts/AlbatrossCNS.md) [2](https://swudususuwu.substack.com/p/albatross-performs-lots-of-neural)
 * [Overview of howto do autonomous tools](https://github.com/SwuduSusuwu/SusuLib/blob/experimental/posts/ArduinoElegooTools.md) [2](https://swudususuwu.substack.com/p/how-to-use-floss-systems-to-produce) [3](https://swudususuwu.substack.com/p/program-general-purpose-robots-autonomous)
 * [Overview of howto do uploads of consciousness](https://swudususuwu.substack.com/p/destructive-unreversible-upload-of)
 */
}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassCns_hxx */

