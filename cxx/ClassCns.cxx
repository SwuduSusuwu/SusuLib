/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#ifndef INCLUDES_cxx_ClassCns_cxx
#define INCLUDES_cxx_ClassCns_cxx
#include "ClassCns.hxx" /* std::string std::tuple */
#include "Macros.hxx" /* SUSUWU_IF_CPLUSPLUS SUSUWU_OVERRIDE */
#include SUSUWU_IF_CPLUSPLUS(<cassert>, <assert.h>) /* assert */
#include SUSUWU_IF_CPLUSPLUS(<cctype>, <ctype.h>) /* size_t */
#include SUSUWU_IF_CPLUSPLUS(<cstdlib>, <stdlib.h>) /* exit EXIT_FAILURE */
namespace Susuwu {
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

