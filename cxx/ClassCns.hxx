/* Attribution (henceforth "*this attribution*", whose syntax is *Markdown*): 2024 [Swudu Susuwu](https://swudususuwu.substack.com)
 * <https://github.com/SwuduSusuwu/SusuLib/> has the newest version of `./cxx/ClassCns.hxx` (henceforth "*this source code*").
 * If *this attribution* is shown, *this source code* allows all uses. *This attribution* constitutes the most permissive which is compatible with [*GPLv2*](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) + [*Apache 2*](https://www.apache.org/licenses/LICENSE-2.0.html), which is suitable for personal use (also suitable for school use).
 * If *this attribution* is not professional enough for business use: businesses can use *this source code* through included versions of [*GPLv2*](./LICENSE_GPLv2), [*Apache 2*](./LICENSE), or through both of those. */
#pragma once
#ifndef INCLUDES_cxx_ClassCns_hxx
#define INCLUDES_cxx_ClassCns_hxx
#include "ClassObject.hxx" /* Object SUSUWU_PURE_VIRTUAL_DEFAULTS() */
#include "Macros.hxx" /* SUSUWU_CXX17 SUSUWU_IF_CPLUSPLUS SUSUWU_INTPTR SUSUWU_DEFAULT SUSUWU_NOEXCEPT SUSUWU_OVERRIDE */
#include SUSUWU_IF_CPLUSPLUS(<cassert>, <assert.h>) /* assert */
#include SUSUWU_IF_CPLUSPLUS(<cstddef>, <stddef.h>) /* size_t */
#if SUSUWU_VIRTUAL_MEMBER_FUNCTION_TEMPLATES /* C++ does not support templates of virtual functions ( https://stackoverflow.com/a/78440416/24473928 ) */
#	include <stdexcept> /* std::runtime_error */
#else /* !SUSUWU_VIRTUAL_MEMBER_FUNCTION_TEMPLATES */
#	include <string> /* std::string */
#endif /* !SUSUWU_VIRTUAL_MEMBER_FUNCTION_TEMPLATES */
#include <tuple> /* std::tuple */
#include <vector> /* std::vector */

namespace Susuwu {
#ifndef SUSUWU_CNS_VALUE_SEMANTICS
#	define SUSUWU_CNS_VALUE_SEMANTICS true
#endif /* ndef SUSUWU_CNS_VALUE_SEMANTICS */
typedef enum CnsMode : char {
	cnsModeBool, cnsModeChar, cnsModeInt, cnsModeUint, cnsModeFloat, cnsModeDouble,
	cnsModeVectorBool, cnsModeVectorChar, cnsModeVectorInt, cnsModeVectorUint, cnsModeVectorFloat, cnsModeVectorDouble,
#if defined(SUSUWU_CXX17) && defined(SUSUWU_PREFER_STRING_VIEW /* TODO */)
	cnsModeString = cnsModeVectorChar /* std::string == std::vector<char> */
#else /* else !def SUSUWU_CXX17 */
/* https://stackoverflow.com/questions/5115166/how-to-construct-a-stdstring-from-a-stdvectorchar */
	cnsModeString
#endif /* def SUSUWU_CXX17 else */
} CnsMode;

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

	/* Topological values; sets the "shape" of `Cns.synapses` (or of whatever the derived class uses to store the connectome) */
	virtual void setInputMode(CnsMode x) { inputMode = x; } /* sets type of input */
	virtual void setOutputMode(CnsMode x) { outputMode = x; } /* sets type of output (notice: some implementations require `inputMode == outputMode`) */
	virtual void setInputNeurons(size_t x) { inputNeurons = x; } /* sets connectome input count */
	virtual void setOutputNeurons(size_t x) { outputNeurons = x; } /* sets connectome output count (notice: some implementations require `inputNeurons == outputNeurons`) */
	virtual void setLayersOfNeurons(size_t x) { layersOfNeurons = x; } /* sets connectome "hidden layer" count */
	virtual void setNeuronsPerLayer(size_t x) { neuronsPerLayer = x; } /* sets connectome coefficients-per-"hidden layer" (notice: some implementations require `inputNeurons == neuronsPerLayer`) */

	/* @throw bad_alloc
	 * @pre @code !isPureVirtual() @endcode
	 * @post @code isInitialized() @endcode */
#if SUSUWU_VIRTUAL_MEMBER_FUNCTION_TEMPLATES /* C++ does not support templates of virtual functions ( https://stackoverflow.com/a/78440416/24473928 ) */
	template<typename Input, typename Output>
	virtual void setupSynapses(std::vector<std::tuple<Input, Output>> inputsToOutputs); /* { inputMode = typeToCnsMode<Input>; outMode = typeToCnsMode<Output>; throw std::runtime_error("ClassCns::setupSynapses() pure virtual call"); } */
	/* @pre @code isInitialized() @endcode */
	template<typename Input, typename Output>
	virtual const Output process(const Input input) const { /* assert(typeToCnsMode<Input> == inputMode && typeToCnsMode<Output> == outputMode);*/ throw std::runtime_error("ClassCns::process() pure virtual call"); }
#else /* !SUSUWU_VIRTUAL_MEMBER_FUNCTION_TEMPLATES */
/* NOLINTBEGIN(cppcoreguidelines-macro-usage,misc-unused-parameters): no virtual templates (must use macros), unused parameters are due to the fact this is a stub */
#	define SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, INPUT_MODE, OUTPUT_TYPEDEF, OUTPUT_MODE) \
	/* @throw bad_alloc \
	 * @pre @code !isPureVirtual() @endcode \
	 * @post @code isInitialized() @endcode */\
	virtual void setupSynapses(const std::vector<std::tuple<INPUT_TYPEDEF, OUTPUT_TYPEDEF>> &inputsToOutputs) { inputMode = (INPUT_MODE); outputMode = OUTPUT_MODE; } /* NOLINT(bugprone-macro-parentheses): parentheses cause "error: expected expression [clang-diagnostic-error]" */
#	define SUSUWU_TEMPLATE_WORKAROUND(INPUT_MODE, INPUT_TYPEDEF) \
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, INPUT_MODE, bool, cnsModeBool)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, INPUT_MODE, char, cnsModeChar)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, INPUT_MODE, int, cnsModeInt)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, INPUT_MODE, unsigned int, cnsModeUint)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, INPUT_MODE, float, cnsModeFloat)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, INPUT_MODE, double, cnsModeDouble)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, INPUT_MODE, std::vector<bool>, cnsModeVectorBool)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, INPUT_MODE, std::vector<char>, cnsModeVectorChar)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, INPUT_MODE, std::vector<int>, cnsModeVectorInt)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, INPUT_MODE, std::vector<unsigned int>, cnsModeVectorUint)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, INPUT_MODE, std::vector<float>, cnsModeVectorFloat)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, INPUT_MODE, std::vector<double>, cnsModeVectorDouble)\
	SUSUWU_CNS_SETUP_SYNAPSES(INPUT_TYPEDEF, INPUT_MODE, std::string, cnsModeString)\
	\
	/* @pre @code isInitialized() @endcode */\
	virtual const bool processToBool(const INPUT_TYPEDEF &input) const { assert((INPUT_MODE) == inputMode && cnsModeBool == outputMode); return 0; }\
	virtual const char processToChar(const INPUT_TYPEDEF &input) const { assert((INPUT_MODE) == inputMode && cnsModeChar == outputMode); return 0; }\
	virtual const int processToInt(const INPUT_TYPEDEF &input) const { assert((INPUT_MODE) == inputMode && cnsModeInt == outputMode); return 0; }\
	virtual const unsigned int processToUint(const INPUT_TYPEDEF &input) const { assert((INPUT_MODE) == inputMode && cnsModeUint == outputMode); return 0; }\
	virtual const float processToFloat(const INPUT_TYPEDEF &input) const { assert((INPUT_MODE) == inputMode && cnsModeFloat == outputMode); return 0; }\
	virtual const double processToDouble(const INPUT_TYPEDEF &input) const { assert((INPUT_MODE) == inputMode && cnsModeDouble == outputMode); return 0; }\
	virtual const std::vector<bool> processToVectorBool(const INPUT_TYPEDEF &input) const { assert((INPUT_MODE) == inputMode && cnsModeVectorBool == outputMode); return {}; }\
	virtual const std::vector<char> processToVectorChar(const INPUT_TYPEDEF &input) const { assert((INPUT_MODE) == inputMode && cnsModeVectorChar == outputMode); return {}; }\
	virtual const std::vector<int> processToVectorInt(const INPUT_TYPEDEF &input) const { assert((INPUT_MODE) == inputMode && cnsModeVectorInt == outputMode); return {}; }\
	virtual const std::vector<unsigned int> processToVectorUint(const INPUT_TYPEDEF &input) const { assert((INPUT_MODE) == inputMode && cnsModeVectorUint == outputMode); return {}; }\
	virtual std::vector<float> processToVectorFloat(const INPUT_TYPEDEF &input) const { assert((INPUT_MODE) == inputMode && cnsModeVectorFloat == outputMode); return {}; }\
	virtual const std::vector<double> processToVectorDouble(const INPUT_TYPEDEF &input) const { assert((INPUT_MODE) == inputMode && cnsModeVectorDouble == outputMode); return {}; }\
	virtual const std::string processToString(const INPUT_TYPEDEF &input) const { auto val = processToVectorChar(input); return std::string(&val[0], val.size());}
	SUSUWU_TEMPLATE_WORKAROUND(cnsModeBool, bool)
	SUSUWU_TEMPLATE_WORKAROUND(cnsModeChar, char)
	SUSUWU_TEMPLATE_WORKAROUND(cnsModeInt, int)
	SUSUWU_TEMPLATE_WORKAROUND(cnsModeUint, unsigned int)
	SUSUWU_TEMPLATE_WORKAROUND(cnsModeFloat, float)
	SUSUWU_TEMPLATE_WORKAROUND(cnsModeDouble, double)
	SUSUWU_TEMPLATE_WORKAROUND(cnsModeVectorBool, std::vector<bool>)
	SUSUWU_TEMPLATE_WORKAROUND(cnsModeVectorChar, std::vector<char>)
	SUSUWU_TEMPLATE_WORKAROUND(cnsModeVectorInt, std::vector<int>)
	SUSUWU_TEMPLATE_WORKAROUND(cnsModeVectorUint, std::vector<unsigned int>)
	SUSUWU_TEMPLATE_WORKAROUND(cnsModeVectorFloat, std::vector<float>)
	SUSUWU_TEMPLATE_WORKAROUND(cnsModeVectorDouble, std::vector<double>)
	SUSUWU_TEMPLATE_WORKAROUND(cnsModeString, std::string)
#	undef SUSUWU_TEMPLATE_WORKAROUND
	/* NOLINTEND(cppcoreguidelines-macro-usage,misc-unused-parameters) */
#endif /* !SUSUWU_VIRTUAL_MEMBER_FUNCTION_TEMPLATES */
private:
	bool initialized = false;
	CnsMode inputMode = cnsModeBool, outputMode = cnsModeBool;
	size_t inputNeurons = 0, outputNeurons = 0, layersOfNeurons = 0, neuronsPerLayer = 0;
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

/* Related to this:
 * [Howto use `class Cns` for virus analysis and digital assistants](https://github.com/SwuduSusuwu/SusuLib/blob/preview/posts/VirusAnalysis.md) [2](https://swudususuwu.substack.com/p/howto-improve-virus-analysis) (demos; `#include "VirusAnalysis.hxx"` and `#include "AssistantCns.hxx"`)
 * [How to: throughput improved](https://github.com/SwuduSusuwu/SusuLib/blob/experimental/posts/SimdGpgpuTpu.md) [2](https://swudususuwu.substack.com/p/howto-run-devices-phones-laptops)
 * [Alternative CNS's architectures](https://github.com/SwuduSusuwu/SusuLib/blob/experimental/posts/AlbatrossCNS.md) [2](https://swudususuwu.substack.com/p/albatross-performs-lots-of-neural)
 * [Overview of howto do autonomous tools](https://github.com/SwuduSusuwu/SusuLib/blob/experimental/posts/ArduinoElegooTools.md) [2](https://swudususuwu.substack.com/p/how-to-use-floss-systems-to-produce) [3](https://swudususuwu.substack.com/p/program-general-purpose-robots-autonomous)
 * [Overview of howto do uploads of consciousness](https://swudususuwu.substack.com/p/destructive-unreversible-upload-of)
 */
}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassCns_hxx */

