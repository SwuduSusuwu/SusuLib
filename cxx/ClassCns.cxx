/* Attribution (henceforth "*this attribution*", whose syntax is *Markdown*): 2024 [Swudu Susuwu](https://swudususuwu.substack.com)
 * <https://github.com/SwuduSusuwu/SusuLib/> has the newest version of `./cxx/ClassCns.cxx` (henceforth "*this source code*").
 * If *this attribution* is shown, *this source code* allows all uses. *This attribution* constitutes the most permissive which is compatible with [*GPLv2*](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) + [*Apache 2*](https://www.apache.org/licenses/LICENSE-2.0.html), which is suitable for personal use (also suitable for school use).
 * If *this attribution* is not professional enough for business use: businesses can use *this source code* through included versions of [*GPLv2*](./LICENSE_GPLv2), [*Apache 2*](./LICENSE), or through both of those. */
#ifndef INCLUDES_cxx_ClassCns_cxx
#define INCLUDES_cxx_ClassCns_cxx
#include "ClassCns.hxx" /* std::string std::tuple */
#include "Macros.hxx" /* SUSUWU_ERRSTR SUSUWU_IF_CPLUSPLUS SUSUWU_OVERRIDE SUSUWU_SH_ERROR */
#include SUSUWU_IF_CPLUSPLUS(<cassert>, <assert.h>) /* assert */
#include SUSUWU_IF_CPLUSPLUS(<cctype>, <ctype.h>) /* size_t */
#include SUSUWU_IF_CPLUSPLUS(<cstdint>, <stdint.h>) /* uint8_t uint32_t uint64_t */
#include SUSUWU_IF_CPLUSPLUS(<cstdlib>, <stdlib.h>) /* exit EXIT_FAILURE */
#include SUSUWU_IF_CPLUSPLUS(<cstring>, <string.h>) /* std::memcmp */
#include <fstream> /* std::ifstream std::ofstream */
#include <stdexcept> /* std::runtime_error */

namespace Susuwu {

/* Binary format for `Cns::dumpTo` / `Cns::loadFrom`:
 * [4 bytes] magic "CNS\0"
 * [1 byte]  format version (currently 1)
 * [1 byte]  inputMode  (ObjectMode, which is `char`)
 * [1 byte]  outputMode (ObjectMode, which is `char`)
 * [8 bytes] inputNeurons   (uint64_t)
 * [8 bytes] outputNeurons  (uint64_t)
 * [8 bytes] layersOfNeurons  (uint64_t)
 * [8 bytes] neuronsPerLayer  (uint64_t)
 * [8 bytes] inputNormsStorage.average   (double)
 * [8 bytes] inputNormsStorage.magnitude (double)
 * [8 bytes] outputNormsStorage.average   (double, only if SUSUWU_CNS_SEPARATE_NORMS)
 * [8 bytes] outputNormsStorage.magnitude (double, only if SUSUWU_CNS_SEPARATE_NORMS)
 * [8 bytes] patience (uint64_t)
 * [4 bytes] minLossDelta         (float)
 * [4 bytes] desiredLossThreshold (float)
 * [4 bytes] validationFactor     (float)
 * [4 bytes] learningFactor       (float)
 * [1 byte]  initialized (0 or 1)
 */
static const char cnsDumpMagic[4] = {'C', 'N', 'S', '\0'};
static const uint8_t cnsDumpVersion = 1;

void Cns::dumpTo(const ClassIoPath &modelPath) const {
	std::ofstream file(modelPath, std::ios::binary | std::ios::trunc);
	if(!file) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::dumpTo(.modelPath = \"" + modelPath + "\") { !std::ofstream(modelPath, std::ios::binary | std::ios::trunc) }"));
	}
	file.write(cnsDumpMagic, sizeof(cnsDumpMagic));
	file.write(reinterpret_cast<const char *>(&cnsDumpVersion), sizeof(cnsDumpVersion));
	const char inMode = static_cast<char>(inputMode);
	const char outMode = static_cast<char>(outputMode);
	file.write(&inMode, sizeof(inMode));
	file.write(&outMode, sizeof(outMode));
	const uint64_t inN = static_cast<uint64_t>(inputNeurons);
	const uint64_t outN = static_cast<uint64_t>(outputNeurons);
	const uint64_t layersN = static_cast<uint64_t>(layersOfNeurons);
	const uint64_t neuronsN = static_cast<uint64_t>(neuronsPerLayer);
	file.write(reinterpret_cast<const char *>(&inN), sizeof(inN));
	file.write(reinterpret_cast<const char *>(&outN), sizeof(outN));
	file.write(reinterpret_cast<const char *>(&layersN), sizeof(layersN));
	file.write(reinterpret_cast<const char *>(&neuronsN), sizeof(neuronsN));
	const double inAvg = inputNormsStorage.average;
	const double inMag = inputNormsStorage.magnitude;
	file.write(reinterpret_cast<const char *>(&inAvg), sizeof(inAvg)); /* cppcheck-suppress invalidPointerCast */
	file.write(reinterpret_cast<const char *>(&inMag), sizeof(inMag)); /* cppcheck-suppress invalidPointerCast */
#if SUSUWU_CNS_SEPARATE_NORMS
	const double outAvg = outputNormsStorage.average;
	const double outMag = outputNormsStorage.magnitude;
	file.write(reinterpret_cast<const char *>(&outAvg), sizeof(outAvg)); /* cppcheck-suppress invalidPointerCast */
	file.write(reinterpret_cast<const char *>(&outMag), sizeof(outMag)); /* cppcheck-suppress invalidPointerCast */
#endif /* SUSUWU_CNS_SEPARATE_NORMS */
	const uint64_t pat = static_cast<uint64_t>(patience);
	file.write(reinterpret_cast<const char *>(&pat), sizeof(pat));
	file.write(reinterpret_cast<const char *>(&minLossDelta), sizeof(minLossDelta)); /* cppcheck-suppress invalidPointerCast */
	file.write(reinterpret_cast<const char *>(&desiredLossThreshold), sizeof(desiredLossThreshold)); /* cppcheck-suppress invalidPointerCast */
	file.write(reinterpret_cast<const char *>(&validationFactor), sizeof(validationFactor)); /* cppcheck-suppress invalidPointerCast */
	file.write(reinterpret_cast<const char *>(&learningFactor), sizeof(learningFactor)); /* cppcheck-suppress invalidPointerCast */
	const uint8_t init = initialized ? 1 : 0;
	file.write(reinterpret_cast<const char *>(&init), sizeof(init));
	if(!file) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::dumpTo(.modelPath = \"" + modelPath + "\") { !file /* write failed */ }"));
	}
}

void Cns::loadFrom(const ClassIoPath &modelPath) {
	std::ifstream file(modelPath, std::ios::binary);
	if(!file) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(.modelPath = \"" + modelPath + "\") { !std::ifstream(modelPath, std::ios::binary) }"));
	}
	char magic[4];
	file.read(magic, sizeof(magic));
	if(std::memcmp(magic, cnsDumpMagic, sizeof(cnsDumpMagic)) != 0) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(.modelPath = \"" + modelPath + "\") { magic != \"CNS\\0\" /* unrecognized file format */ }"));
	}
	uint8_t version = 0;
	file.read(reinterpret_cast<char *>(&version), sizeof(version));
	if(version != cnsDumpVersion) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(.modelPath = \"" + modelPath + "\") { version == " + std::to_string(version) + "; /* unsupported format version */ }"));
	}
	char inMode = 0, outMode = 0;
	file.read(&inMode, sizeof(inMode));
	file.read(&outMode, sizeof(outMode));
	uint64_t inN = 0, outN = 0, layersN = 0, neuronsN = 0;
	file.read(reinterpret_cast<char *>(&inN), sizeof(inN));
	file.read(reinterpret_cast<char *>(&outN), sizeof(outN));
	file.read(reinterpret_cast<char *>(&layersN), sizeof(layersN));
	file.read(reinterpret_cast<char *>(&neuronsN), sizeof(neuronsN));
	double inAvg = 0.0, inMag = 1.0;
	file.read(reinterpret_cast<char *>(&inAvg), sizeof(inAvg)); /* cppcheck-suppress invalidPointerCast */
	file.read(reinterpret_cast<char *>(&inMag), sizeof(inMag)); /* cppcheck-suppress invalidPointerCast */
#if SUSUWU_CNS_SEPARATE_NORMS
	double outAvg = 0.0, outMag = 1.0;
	file.read(reinterpret_cast<char *>(&outAvg), sizeof(outAvg)); /* cppcheck-suppress invalidPointerCast */
	file.read(reinterpret_cast<char *>(&outMag), sizeof(outMag)); /* cppcheck-suppress invalidPointerCast */
	outputNormsStorage.average = outAvg;
	outputNormsStorage.magnitude = outMag;
#endif /* SUSUWU_CNS_SEPARATE_NORMS */
	uint64_t pat = 0;
	file.read(reinterpret_cast<char *>(&pat), sizeof(pat));
	file.read(reinterpret_cast<char *>(&minLossDelta), sizeof(minLossDelta)); /* cppcheck-suppress invalidPointerCast */
	file.read(reinterpret_cast<char *>(&desiredLossThreshold), sizeof(desiredLossThreshold)); /* cppcheck-suppress invalidPointerCast */
	file.read(reinterpret_cast<char *>(&validationFactor), sizeof(validationFactor)); /* cppcheck-suppress invalidPointerCast */
	file.read(reinterpret_cast<char *>(&learningFactor), sizeof(learningFactor)); /* cppcheck-suppress invalidPointerCast */
	uint8_t init = 0;
	file.read(reinterpret_cast<char *>(&init), sizeof(init));
	if(!file) {
		throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, getName() + "::loadFrom(.modelPath = \"" + modelPath + "\") { !file /* read failed or truncated */ }"));
	}
	inputMode = static_cast<ObjectMode>(inMode);
	outputMode = static_cast<ObjectMode>(outMode);
	inputNeurons = static_cast<size_t>(inN);
	outputNeurons = static_cast<size_t>(outN);
	layersOfNeurons = static_cast<size_t>(layersN);
	neuronsPerLayer = static_cast<size_t>(neuronsN);
	inputNormsStorage.average = inAvg;
	inputNormsStorage.magnitude = inMag;
	patience = static_cast<size_t>(pat);
	initialized = (init != 0);
}
#ifdef USE_HSOM_CNS
/* Sources: `git clone https://github.com/CarsonScott/HSOM.git`
 * Install: `pip install pynum && pip install json && pip install git+https://github.com/CarsonScott/HSOM.git`
 * Documentation: `less HSOM/README.md` `less HSOM/Documentation.md` */
/* "If you're using Python >3.5, PyString_FromString() is PyUnicode_FromString()" */
#include <Python.h> /* Sources: `pkg install python` */
#include <vector> /* std::vector */
typedef class HsomCns : Cns { /* TODO. ( https://stackoverflow.com/questions/3286448/calling-a-python-method-from-c-c-and-extracting-its-return-value ) suggests various syntaxes to use for this, with unanswered comments such as "Does this support classes?" */
	//template<Input, Output> void setupSynapses(const std::vector<std::tuple<Input, Output>>) { /* TODO: templates not allowed for virtual functions with C++ ( https://stackoverflow.com/a/78440416/24473928 ), so must produce codes for each combination of inputMode+outputMode */
	void setupSynapses(const std::vector<std::tuple<float, float>>) SUSUWU_OVERRIDE {
		setenv("PYTHONPATH", ".",1);
		Py_Initialize();
//		PyRun_SimpleString("import sys; sys.path.append('.')"); PyRun_SimpleString("import hsom; from hsom import SelfOrganizingNetwork;");
#if USE_PYRUN /* Was told not to use PyRun because "PyRun requires all results go to stdout" */
		PyRun_SimpleString("import sys; sys.path.append('./HSOM/')");

/* Based off of https://github.com/CarsonScott/HSOM/blob/master/examples/self_organizing_network.py
 * Not sure if `input_size` is "Inputs from each layer to next layer" and `node_count` is "Inputs to HSOM" (process(input.length())) or vice versa, assumed vice versa */

		PyRun_SimpleString("import hsom\n\
from hsom import SelfOrganizingNetwork\n\
from random import sample\n\
\
input_size = " + inputNeurons + "\n\
layer_sizes = []\n\
for x in range(" + layersOfNeurons + "):\n\
	layer_sizes.append(" + neuronsPerLayer + ");\n\
layer_sizes.append(" + outputNeurons + ");\n\
input_percents = [0.2, 0.2, 0.2, 0.2, 0.75, 1.0]\n\
learning_rate = 0.05\n\
boost_factor = 1\n\
node_count = 5\n\
winner_count = 1\n\
initial_range = (-0.5, 0.5)\n\
\
# Create layersOfNeurons+1 hierarchical layers of sizes = neuronsPerLayer, and outputNeurons for last\n\
self_organizing_network = SelfOrganizingNetwork(\
	input_size=input_size,\
	layer_sizes=layer_sizes,\
	input_percents=input_percents,\
	learning_rates=learning_rate,\
	boost_factors=boost_factor,\
	node_counts=node_count,\
	winner_counts=winner_count,\
	initial_ranges=initial_range)\n\
\
# Create a set of sparse samples\n\
samples = []");
		foreach(inputsToOutputs as sample) { /* TODO: templates not allowed for virtual functions with C++ ( https://stackoverflow.com/a/78440416/24473928 ), so must produce codes for each combination of inputMode+outputMode */
			PyRun_SimpleString("samples.append(" + sample.first() +" -> " + sample.last() + ")");
		}
		PyRun_SimpleString("for i in range(200):\n\
	self_organizing_network.train(samples)");
#else /* else !USE_PYRUN */
		PyObject *module = PyImport_ImportModule("hsom")
		if(NULL == module) { throw "'hsom' module not found"; }
		PyObject *selfOrganizingNetwork = PyObject_GetAttrString(module, const_cast<char *>("SelfOrganizingNetwork")); /* or `PyObject *pDict = PyModule_GetDict(module);  PyObject *selfOrganizingNetwork = PyDict_GetItemString(pDict, const_cast<char *>("SelfOrganizingNetwork"));` */
		if(NULL == selfOrganizingNetwork || !PyCallable_Check(selfOrganizingNetwork)) { throw "'SelfOrganizingNetwork' object not found"; }
		double result = PyObject_CallFunction(selfOrganizingNetwork, "d", 2.0); /* or `PyObject *pValue = Py_BuildValue("(z)",const_cast<char *>("args"));	PyObject *pResult = PyObject_CallObject(selfOrganizingNetwork, pValue); if(NULL == pResult) { throw "PyObject_CallObject failed"; } double result = PyInt_AsLong(pResult)); Py_DECREF(pValue);` */
		Py_DECREF(module);
#endif /* USE_PYRUN else */
	}
	~HsomCns() {
#if PYTHON3
		Py_FinalizeEx();
#else /* else !PYTHON */
		Py_Finalize();
#endif /* PYTHON3 else */
	}
} HsomCns;
#endif /* USE_HSOM_CNS */

#ifdef USE_APXR_CNS
/* Sources: `git clone https://github.com/Rober-t/apxr_run.git`
 * Howto install apxr_run: `less apxr_run/README.md` or `lynx https://github.com/Rober-t/apxr_run/blob/master/README.md` */
typedef class ApxrCns : Cns {
/* TODO: https://stackoverflow.com/questions/1811516/integrating-erlang-with-c (first result for "Howto use Erlang functions from C/C++"):
 * ""Port drivers: you can link a C code to the Erlang VM, and access it using port_command."" references https://www.erlang.org/doc/tutorial/c_portdriver.html , which appears to just show howto use C/C++ functions from Erlang (not vice versa)
 * ""C Nodes: With the ei library you can mimic a VM and talk to your Erlang VMs using the Erlang distribution format."" references https://www.erlang.org/doc/man/ei.html , which shows some promises
 * ""The closest thing I know for interfacing Erlang with C++ directly is EPAPI. Of course it relies on the tried and tested C erl_interface that comes standard with the Erlang distribution."" references https://epapi.googlecode.com/ , which returns "404 not found".
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
#endif /* ndef INCLUDES_cxx_ClassCns_cxx */

