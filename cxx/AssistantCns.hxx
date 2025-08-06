/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#pragma once
#ifndef INCLUDES_cxx_AssistantCns_hxx
#define INCLUDES_cxx_AssistantCns_hxx
#include "ClassCns.hxx" /* Cns CnsMode */
#include "ClassIo.hxx" /* ClassIoPath ClassIoBytecode */
#include "ClassResultList.hxx" /* ResultList */
#include "ClassSys.hxx" /* templateCatchAll */
#include "Macros.hxx" /* SUSUWU_NOEXCEPT SUSUWU_UNIT_TESTS */
#include <iostream> /* std::cout */
#include <ostream> /* std::ostream */
#include <string> /* std::string */
#include <vector> /* std::vector */
/* (Work-in-progress) system which uses tuples of inputs (questions, or document titles) plus solutions (answers, or documents), to setup artificial neural tissue (such as [**HSOM**](https://github.com/SwuduSusuwu/SusuLib/blob/5eff71e6486562b1d5c7b349618fba9634d479cd/cxx/ClassCns.cxx#L11-L81) or [*TensorFlow*](https://github.com/SwuduSusuwu/SusuLib/blob/preview/cxx/ClassTensorFlowCns.hxx)), which then uses new inputs to produce new solutions (similar to _OpenLM Research_'s "[_OpenLLaMA_](https://github.com/openlm-research/open_llama)", "[_LLaMA 2_](https://www.llama.com/llama2/)" or _Tesla_'s "[_Grok-2__](https://www.segmind.com/models/grok-2])") */
namespace Susuwu {
extern Cns assistantCns;
extern std::string assistantCnsResponseDelimiter;

#if SUSUWU_UNIT_TESTS
/* if (with example inputs) these functions (`assistantCnsDownloadHosts()` `produceAssistantCns()`) pass, `return true;`
 * @throw std::bad_alloc
 * @throw std::logic_error
 * @pre @code !assistantCns.isPureVirtual() @endcode */
const bool assistantCnsTests();
static const bool assistantCnsTestsNoexcept() SUSUWU_NOEXCEPT { return templateCatchAll(assistantCnsTests, "assistantCnsTests()"); } /* cppcheck-suppress throwInNoexceptFunction */
#endif /* SUSUWU_UNIT_TESTS */

/* Universal Resources Locators of hosts which `assistantCnsDownloadHosts()` uses. `extern`s are set in `AssistantCns.cxx`.
 * To downloads those as datasets, ensure to register with services such as <https://dash.cloudflare.com/?to=/:account/configurations/verified-bots>
 * Wikipedia is special; [has compressed downloads of databases for such uses](https://wikipedia.org/wiki/Wikipedia:Database_download)
 * Github is special; [has compressed downloads of repositories for such uses](https://docs.github.com/en/get-started/start-your-journey/downloading-files-from-github)
 */
extern std::vector<ClassIoPath> assistantCnsDefaultHosts;
extern double assistantCnsMaxRequestsPerSecondPerHost; /* TODO: limit `wget` through this */
extern double assistantCnsMaxRequestsPerSecondGlobal;  /* TODO: limit `wget` through this */
extern double assistantCnsMaxBitsPerSecondPerHost;     /* TODO: limit `wget` through this */
extern double assistantCnsMaxBitsPerSecondGlobal;      /* TODO: limit `wget` through this */

/* @throw std::bad_alloc
 * @post If no question, `0 == questionsOrNull.bytecodes[x].size()` (new message synthesis).
 * If no responses, `0 == responsesOrNull.bytecodes[x].size()` (ignore).
 * `questionsOrNull.signatures[x] = Universal Resource Locator`
 * @code classSha2(ResultList.bytecodes[x]) == ResultList.hashes[x] @endcode */
void assistantCnsDownloadHosts(ResultList &questionsOrNull, ResultList &responsesOrNull, const std::vector<ClassIoPath> &hosts = assistantCnsDefaultHosts);
void assistantCnsProcessXhtml(ResultList &questionsOrNull, ResultList &responsesOrNull, const ClassIoPath &localXhtml = "index.xhtml");
const std::vector<ClassIoPath> assistantCnsProcessUrls(const ClassIoPath &localXhtml = "index.xhtml"); /* TODO: for XML/XHTML could just use [ https://www.boost.io/libraries/regex/ https://github.com/boostorg/regex ] or [ https://www.boost.org/doc/libs/1_85_0/doc/html/property_tree/parsers.html#property_tree.parsers.xml_parser https://github.com/boostorg/property_tree/blob/develop/doc/xml_parser.qbk ] */
const ClassIoBytecode assistantCnsProcessQuestion(const ClassIoPath &localXhtml = "index.xhtml"); /* TODO: regex or XML parser */
const std::vector<ClassIoBytecode> assistantCnsProcessResponses(const ClassIoPath &localXhtml = "index.xhtml"); /* TODO: regex or XML parser */

/* @pre `questionsOrNull` maps to `responsesOrNull`,
 * `0 == questionsOrNull.bytecodes[x].size()` for new assistant synthesis (empty question has responses),
 * `0 == responsesOrNull.bytecodes[x].size()` if should not respond (question does not have answers).
 * @post Can use `assistantCnsProcess(cns, text)` @code cns.isInitialized() @endcode */
void produceAssistantCns(const ResultList &questionsOrNull, const ResultList &responsesOrNull, Cns &cns);

/* All clients use is these 2 functions */
/* `return cns.processStringToString(bytecodes);`
 * @pre @code cns.isInitialized() @encode */
const std::string assistantCnsProcess(const Cns &cns, const std::string &bytecode);
/* `while(std::cin >> questions) { std::cout << assistantCnsProcess(questions); }` but more complex
 * @pre @code cns.isInitialized() @encode */
void assistantCnsLoopProcess(const Cns &cns, std::ostream &os = std::cout);


/* Related to this:
 * [How to: throughput improved](https://github.com/SwuduSusuwu/SusuLib/blob/experimental/posts/SimdGpgpuTpu.md) [2](https://swudususuwu.substack.com/p/howto-run-devices-phones-laptops)
 * [Alternative CNS's architectures](https://github.com/SwuduSusuwu/SusuLib/blob/experimental/posts/AlbatrossCNS.md) [2](https://swudususuwu.substack.com/p/albatross-performs-lots-of-neural)
 * [`cnsVirusFix` is close to assistants](https://github.com/SwuduSusuwu/SusuLib/blob/preview/posts/VirusAnalysis.md#comparison-to-assistants) [2](https://swudususuwu.substack.com/p/howto-improve-virus-analysis?utm_source=publication-search#%C2%A7comparison-to-assistants) (demo; `#include "AssistantCns.hxx"`)
 */
}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_AssistantCns_hxx */

