/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#ifndef INCLUDES_cxx_AssistantCns_cxx
#define INCLUDES_cxx_AssistantCns_cxx
#include "AssistantCns.hxx" /* assistantCnsProcessQuestion assistantCnsProcessResponses assistantCnsProcessUrls */
#include "ClassCns.hxx" /* Cns */
#include "ClassIo.hxx" /* ClassIoBytecode ClassIoPath */
#include "ClassObject.hxx" /* ObjectMode */
#include "ClassResultList.hxx" /* explodeToList listMaxSize listHasValue ResultList ResultListBytecode resultListDumpTo resultListProduceHashes */
#include "ClassSha2.hxx" /* classSha2 */
#ifdef SUSUWU_USE_TENSORFLOW
#	include "ClassTensorFlowCns.hxx" /* TensorFlowCns */
#endif /* def SUSUWU_USE_TENSORFLOW */
#include "ClassWebBrowse.hxx" /* classWebBrowseProcessUrls classWebBrowseWget */
#include "Macros.hxx" /* SUSUWU_IF_CPLUSPLUS SUSUWU_NOTICE_EXECUTEVERBOSE SUSUWU_POSIX SUSUWU_UNIT_TESTS SUSUWU_USE_TENSORFLOW */
#include SUSUWU_IF_CPLUSPLUS(<cassert>, <assert.h>) /* assert */
#include SUSUWU_IF_CPLUSPLUS(<cstddef>, <stddef.h>) /* size_t */
#include <iostream> /* std::cin std::cout */
#include <ostream> /* std::ostream */
#if defined(SUSUWU_USE_PUGIXML) /* !def BOOST_VERSION */
#	include <pugixml.hpp> /* pugi::xml_document pugi::xml_parse_result pugi::xml_node pugi::xpath_node */
#endif /* !def SUSUWU_USE_PUGIXML */
#include <string> /* std::string */
#include <tuple> /* std::tuple */
#include <vector> /* std::vector */
/* (Work-in-progress) system which uses tuples of inputs (questions, or document titles) plus solutions (answers, or documents), to setup artificial neural tissue (such as [**HSOM**](https://github.com/SwuduSusuwu/SusuLib/blob/5eff71e6486562b1d5c7b349618fba9634d479cd/cxx/ClassCns.cxx#L11-L81) or [*TensorFlow*](https://github.com/SwuduSusuwu/SusuLib/blob/preview/cxx/ClassTensorFlowCns.hxx)), which then uses new inputs to produce new solutions (similar to _OpenLM Research_'s "[_OpenLLaMA_](https://github.com/openlm-research/open_llama)", "[_LLaMA 2_](https://www.llama.com/llama2/)" or _Tesla_'s "[_Grok-2__](https://www.segmind.com/models/grok-2])") */
namespace Susuwu { /* NOLINTBEGIN(performance-inefficient-string-concatenation): suggestion triggers `cppcheck`'s `error: syntax error: "' -O" += [syntaxError]` */
#ifdef SUSUWU_USE_TENSORFLOW
TensorFlowCns assistantCns;
#else /* !defined(SUSUWU_USE_TENSORFLOW) */
Cns assistantCns;
#endif /* !defined(SUSUWU_USE_TENSORFLOW) */
std::vector<ClassIoPath> assistantCnsDefaultHosts = {
	"https://stackoverflow.com",
	"https://superuser.com",
	"https://www.quora.com"
};
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
	cns.setInputMode(objectModeString);
	cns.setOutputMode(objectModeString);
	cns.setInputNeurons(cns.isSquareConnectome() ? maxWidthOfMessages : maxQuestionSize);
	cns.setOutputNeurons(cns.isSquareConnectome() ? maxWidthOfMessages : maxResponseSize);
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
		classWebBrowseWget(host + "/robots.txt", "");
		classWebBrowseWget(host, "" /* if `localOutput` is used, can't use `classWebBrowseDownloadDir` */);
		const ClassIoPath localDocumentSource = classWebBrowseDownloadDir + "index.html"; /* TODO: some hosts give `.xhtml` or `.htm`, deduce this */
		questionsOrNull.signatures.push_back(host);
		SUSUWU_WARNING("assistantCnsDownloadHosts: { /* TODO: [deduce `ClassIoPath localDocumentSource;` from `classWebBrowseWget(\"" + host + "\", \"\");`](https://poe.com/s/QpjvvHmETSVP6K4wRiU5) */}");
		assistantCnsProcessXhtml(questionsOrNull, responsesOrNull, localDocumentSource);
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
	for(const auto &url : urls) {
		if(!listHasValue(questionsOrNull.signatures, url) && !listHasValue(noRobots, url)) {
#ifndef SUSUWU_POSIX
			SUSUWU_WARNING("assistantCnsProcessXhtml: {#ifndef SUSUWU_POSIX /* TODO: without [`wget` for _Windows_](https://gnuwin32.sourceforge.net/packages/wget.htm) */}");
#endif /* ndef SUSUWU_POSIX */
			const ClassIoPath localDocumentSource = classWebBrowseDownloadDir + "path_todo.tmp";
			classWebBrowseWget(url, localDocumentSource);
			questionsOrNull.signatures.push_back(url);
			SUSUWU_WARNING("assistantCnsProcessXhtml: { /* TODO: parse `auto urls = assistantCnsProcessUrls(\"" + localXhtml + "\";` into `ClassIoPath localDocumentSource;` */}");
			assistantCnsProcessXhtml(questionsOrNull, responsesOrNull, localDocumentSource);
		}
	}
}

const std::vector<ClassIoPath> assistantCnsProcessUrls(const ClassIoPath &localXhtml) {
	return classWebBrowseProcessUrls(localXhtml);
}
const ClassIoBytecode assistantCnsProcessQuestion(const ClassIoPath &localXhtml) {
#if defined(SUSUWU_USE_PUGIXML)
	pugi::xml_document doc;
	const pugi::xml_parse_result result = doc.load_file(localXhtml.c_str());
	if(result) {
		const pugi::xpath_node question = doc.select_node("//div[@class='question']"); /* TODO: if there is still no Web Consortium standard which marks questions, hardcode values for popular resources which graduates use (such as for StackOverflow), or implement heuristics to use */
		if(question) {
			return question.node().child_value();
		}
	}
#else /* else !def SUSUWU_USE_PUGIXML */
#	pragma message("TODO: process XHTML without pugixml")
#endif /* !def SUSUWU_USE_PUGIXML */
	return "";
}
const std::vector<ClassIoBytecode> assistantCnsProcessResponses(const ClassIoPath &localXhtml) {
	std::vector<ClassIoBytecode> responses;
#if defined(SUSUWU_USE_PUGIXML)
	pugi::xml_document doc;
	const pugi::xml_parse_result result = doc.load_file(localXhtml.c_str());
	if(result) {
		const pugi::xpath_node_set responseSet = doc.select_nodes("//div[@class='response']"); /* TODO: if there is still no Web Consortium standard which marks answers, hardcode values for popular resources which graduates use (such as for StackOverflow), or implement heuristics to use */
		for(const auto &response : responseSet) {
			responses.push_back(response.node().child_value());
		}
	}
#else /* else !def SUSUWU_USE_PUGIXML */
#	pragma message("TODO: process XHTML without pugixml")
#endif /* !def SUSUWU_USE_PUGIXML */
	return responses;
}

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

