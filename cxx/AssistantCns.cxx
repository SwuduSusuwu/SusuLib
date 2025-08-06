/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#ifndef INCLUDES_cxx_AssistantCns_cxx
#define INCLUDES_cxx_AssistantCns_cxx
#include "AssistantCns.hxx" /* assistantCnsProcessQuestion assistantCnsProcessResponses assistantCnsProcessUrls */
#include "ClassCns.hxx" /* Cns CnsMode */
#include "ClassIo.hxx" /* ClassIoBytecode ClassIoPath */
#include "ClassResultList.hxx" /* explodeToList listMaxSize listHasValue ResultList ResultListBytecode resultListDumpTo resultListProduceHashes */
#include "ClassSha2.hxx" /* classSha2 */
#include "ClassSys.hxx" /* execvex */
#include "Macros.hxx" /* SUSUWU_IF_CPLUSPLUS SUSUWU_NOTICE_EXECUTEVERBOSE SUSUWU_POSIX SUSUWU_UNIT_TESTS */
#include SUSUWU_IF_CPLUSPLUS(<cassert>, <assert.h>) /* assert */
#include SUSUWU_IF_CPLUSPLUS(<cstddef>, <stddef.h>) /* size_t */
#include <iostream> /* std::cin std::cout */
#include <ostream> /* std::ostream */
#include <string> /* std::string */
#include <tuple> /* std::tuple */
#include <vector> /* std::vector */
/* (Work-in-progress) system which uses tuples of inputs (questions, or document titles) plus solutions (answers, or documents), to setup artificial neural tissue (such as [**HSOM**](https://github.com/SwuduSusuwu/SusuLib/blob/5eff71e6486562b1d5c7b349618fba9634d479cd/cxx/ClassCns.cxx#L11-L81) or [*TensorFlow*](https://github.com/SwuduSusuwu/SusuLib/blob/preview/cxx/ClassTensorFlowCns.hxx)), which then uses new inputs to produce new solutions (similar to _OpenLM Research_'s "[_OpenLLaMA_](https://github.com/openlm-research/open_llama)", "[_LLaMA 2_](https://www.llama.com/llama2/)" or _Tesla_'s "[_Grok-2__](https://www.segmind.com/models/grok-2])") */
namespace Susuwu { /* NOLINTBEGIN(performance-inefficient-string-concatenation): suggestion triggers `cppcheck`'s `error: syntax error: "' -O" += [syntaxError]` */
Cns assistantCns;
std::vector<ClassIoPath> assistantCnsDefaultHosts = {
	"https://stackoverflow.com",
	"https://superuser.com",
	"https://www.quora.com"
};
double assistantCnsMaxRequestsPerSecondPerHost = 2;   /* TODO: limit `wget` through this */
double assistantCnsMaxRequestsPerSecondGlobal = 2000; /* TODO: limit `wget` through this */
double assistantCnsMaxBitsPerSecondPerHost = 2000000; /* TODO: limit `wget` through this */
double assistantCnsMaxBitsPerSecondGlobal = 42000000; /* TODO: limit `wget` through this */
std::string assistantCnsResponseDelimiter = std::string("<delimiterSeparatesMultiplePossibleResponses>");

#if SUSUWU_UNIT_TESTS
const bool assistantCnsTests() {
	ResultList questionsOrNull; {
		questionsOrNull.hashes = {}, questionsOrNull.signatures = {}, questionsOrNull.bytecodes = { /* UTF-8 */
			ResultListBytecode("2^16"),
			ResultListBytecode("How to cause harm?"),
			ResultListBytecode("Do not respond."),
			ResultListBytecode("")
		};
	}
	ResultList responsesOrNull; {
		responsesOrNull.hashes = {}, responsesOrNull.signatures = {}, responsesOrNull.bytecodes = { /* UTF-8 */
			ResultListBytecode("65536") + assistantCnsResponseDelimiter + "65,536", /* `+` is `concat()` for C++ */
			ResultListBytecode(""),
			ResultListBytecode(""),
			ResultListBytecode("How do you do?") + assistantCnsResponseDelimiter + "Fanuc produces autonomous robots"
		};
	}
	resultListProduceHashes(questionsOrNull);
	resultListProduceHashes(responsesOrNull);
	assert(4 == questionsOrNull.bytecodes.size());
	assert(responsesOrNull.bytecodes.size() == questionsOrNull.bytecodes.size());
	assert(4 == questionsOrNull.hashes.size());
	assert(3 == responsesOrNull.hashes.size());
	SUSUWU_NOTICE_EXECUTEVERBOSE(resultListDumpTo(questionsOrNull, std::cout, true, true, false));
	SUSUWU_NOTICE_EXECUTEVERBOSE((resultListDumpTo(responsesOrNull, std::cout, false, false, false), std::cout << std::endl));
	assistantCnsDownloadHosts(questionsOrNull, responsesOrNull);
	produceAssistantCns(questionsOrNull, responsesOrNull, assistantCns);
	return true;
}
#endif /* SUSUWU_UNIT_TESTS */

void produceAssistantCns(const ResultList &questionsOrNull, const ResultList &responsesOrNull, Cns &cns) {
	std::vector<std::tuple<ResultListBytecode, ResultListBytecode>> inputsToOutputs;
	const size_t maxConvolutionsOfMessages = 6666; /* is not conversation's max message count, but max steps to compute output. TODO: compute this value */
	const size_t maxResponseSize = listMaxSize(responsesOrNull.bytecodes);
	const size_t maxQuestionSize = listMaxSize(questionsOrNull.bytecodes);
	const size_t maxWidthOfMessages = (maxResponseSize > maxQuestionSize) ? maxResponseSize : maxQuestionSize;
	cns.setInputMode(cnsModeString);
	cns.setOutputMode(cnsModeString);
	cns.setInputNeurons(maxQuestionSize);
	cns.setOutputNeurons(maxResponseSize);
	cns.setLayersOfNeurons(maxConvolutionsOfMessages);
	cns.setNeuronsPerLayer(maxWidthOfMessages /* TODO: reduce this */);
	assert(questionsOrNull.bytecodes.size() == responsesOrNull.bytecodes.size());
	inputsToOutputs.reserve(questionsOrNull.bytecodes.size());
	for(size_t x = 0; questionsOrNull.bytecodes.size() > x; ++x) {
		inputsToOutputs.push_back({questionsOrNull.bytecodes[x], responsesOrNull.bytecodes[x]});
	}
	cns.setupSynapses(inputsToOutputs);
}

void assistantCnsDownloadHosts(ResultList &questionsOrNull, ResultList &responsesOrNull, const std::vector<ClassIoPath> &hosts) {
	for(const auto &host : hosts) {
#ifndef SUSUWU_POSIX
    SUSUWU_WARNING("assistantCnsDownloadHosts: {#ifndef SUSUWU_POSIX /* TODO: without [`wget` for _Windows_](https://gnuwin32.sourceforge.net/packages/wget.htm) */}");
#endif /* ndef SUSUWU_POSIX */
		execvex("wget '" + host + "/robots.txt' -Orobots.txt");
		execvex("wget '" + host + "' -Oindex.xhtml");
		questionsOrNull.signatures.push_back(host);
		assistantCnsProcessXhtml(questionsOrNull, responsesOrNull, "index.xhtml");
	}
}
void assistantCnsProcessXhtml(ResultList &questionsOrNull, ResultList &responsesOrNull, const ClassIoPath &localXhtml) {
	auto noRobots = assistantCnsProcessUrls("robots.txt");
	auto question = assistantCnsProcessQuestion(localXhtml);
	if(!question.empty()) {
		auto questionSha2 = classSha2(question);
		if(listHasValue(questionsOrNull.hashes, questionSha2)) { /* TODO */ } else {
			decltype(question) response = "";
			auto responses = assistantCnsProcessResponses(localXhtml);
			if(!responses.empty()) { /* cppcheck-suppress knownConditionTrueFalse */
				questionsOrNull.hashes.insert(questionSha2);
				questionsOrNull.bytecodes.push_back(question);
				size_t responseCount = 0;
				for(const auto &responseIt : responses) {
					if(1 != ++responseCount) {
						response += assistantCnsResponseDelimiter;
					}
					response += responseIt;
				}
				auto responseSha2 = classSha2(response);
				if(listHasValue(responsesOrNull.hashes, responseSha2)) { /* TODO */ } else {
					responsesOrNull.hashes.insert(responseSha2);
					responsesOrNull.bytecodes.push_back(response);
				}
			}
		}
	}
	auto urls = assistantCnsProcessUrls(localXhtml);
	for(const auto &url : urls) { /* cppcheck-suppress knownEmptyContainer */
		if(!listHasValue(questionsOrNull.signatures, url) && !listHasValue(noRobots, url)) {
#ifndef SUSUWU_POSIX
			SUSUWU_WARNING("assistantCnsProcessXhtml: {#ifndef SUSUWU_POSIX /* TODO: without [`wget` for _Windows_](https://gnuwin32.sourceforge.net/packages/wget.htm) */}");
#endif /* ndef SUSUWU_POSIX */
			execvex("wget '" + url + "' -O" + localXhtml);
			questionsOrNull.signatures.push_back(url);
			assistantCnsProcessXhtml(questionsOrNull, responsesOrNull, localXhtml);
		}
	}
}

#ifdef BOOST_VERSION
#	include <boost/property_tree/ptree.hpp>
#	include <boost/property_tree/xml_parser.hpp>
#endif /* BOOST_VERSION */
const std::vector<ClassIoPath> assistantCnsProcessUrls(const ClassIoPath &localXhtml) {
	std::vector<ClassIoPath> urls;
#ifdef BOOST_VERSION
	boost::property_tree::ptree pt;
	read_xml(localXhtml, pt);
	BOOST_FOREACH(
			boost::property_tree::ptree::value_type &v,
			pt.get_child("html.a href"))
		urls.push_back(v.second.data());
#else /* else !BOOST_VERSION */
#	pragma message("TODO: process XHTML without `Boost`")
#endif /* else !BOOST_VERSION */
	return urls;
}
const ClassIoBytecode assistantCnsProcessQuestion(const ClassIoPath &localXhtml) {return "";} /* TODO */
const std::vector<ClassIoBytecode> assistantCnsProcessResponses(const ClassIoPath &localXhtml) {return {};} /* TODO */

const std::string assistantCnsProcess(const Cns &cns, const ClassIoBytecode &bytecode) {
	return cns.processToString(bytecode);
}
void assistantCnsLoopProcess(const Cns &cns, std::ostream &os /* = std::cout */) {
	std::string input;
	while(std::cin >> input) {
		std::vector<std::string> responses = explodeToList(cns.processToString(input), assistantCnsResponseDelimiter);
		std::string response; /* cppcheck-suppress unusedVariable */
		if(responses.size() > 1) {
			int responseNumber = 1;
			for(const auto &it : responses) {
#ifdef IGNORE_PAST_MESSAGES
				os << "Response #" << std::to_string(responseNumber++) << ": " << it << std::endl;
			}
		} else {
			os << responses.at(0) << std::endl;
		}
		input = ""; /* reset past messages */
#else /* !def IGNORE_PAST_MESSAGES */
				response += "Response #" + std::to_string(responseNumber++) + ": " + it + '\n';
			}
		} else {
			response = responses.at(0);
		}
		input += "\n<response>" + response + "</response>\n";
		os << response;
#endif /* !def IGNORE_PAST_MESSAGES */
	}
}
/* How to have performance improve;
 * [compiler options / flags to use](https://github.com/SwuduSusuwu/SusuLib/blob/preview/posts/SimdGpgpuTpu.md#intro)
 * Use [**SIMD**](https://github.com/SwuduSusuwu/SusuLib/blob/preview/posts/SimdGpgpuTpu.md#simd-single-instruction-multiple-data) for functions such as `resultListProduceHashes`.
 * Use [**TPU**s](https://github.com/SwuduSusuwu/SusuLib/blob/preview/posts/SimdGpgpuTpu.md#tpus-tensor-processor-units) for functions such as `produceAssistantCns`. The [**TensorFlow** backend for `class Cns`](https://github.com/SwuduSusuwu/SusuLib/blob/preview/cxx/ClassTensorFlowCns.hxx) can use: {**SIMD**, **GPGPU**s, **TPU**s}.
 */
}; /* namespace Susuwu */ /* NOLINTEND(performance-inefficient-string-concatenation) */
#endif /* ndef INCLUDES_cxx_AssistantCns_cxx */

