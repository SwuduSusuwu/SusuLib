/* Dual licenses: choose "Creative Commons" or "Apache 2" (allows all uses) */
#pragma once
#ifndef INCLUDES_cxx_AssistantCns_hxx
#define INCLUDES_cxx_AssistantCns_hxx
#include "ClassCns.hxx" /* Cns CnsMode */
#include "ClassSys.hxx" /* templateCatchAll */
#include "ClassPortableExecutable.hxx" /* FilePath FileBytecode */
#include "ClassResultList.hxx" /* ResultList */
#include "Macros.hxx" /* SUSUWU_NOEXCEPT */
#include <iostream> /* std::cout */
#include <ostream> /* std::ostream */
#include <string> /* std::string */
#include <vector> /* std::vector */
/* (Work-in-progress) assistant bots with artificial CNS ("HSOM" (the simple Python artificial CNS) is enough to do this), which should have results almost as complex as "ChatGPT 4.0" (or as "Claude-3 Opus"); */
namespace Susuwu {
extern Cns assistantCns;
extern std::string assistantCnsResponseDelimiter;

/* if (with example inputs) these functions (`questionsResponsesFromHosts()` `produceAssistantCns()`) pass, `return true;`
 * @throw std::bad_alloc
 * @throw std::logic_error
 * @pre @code assistantCns.hasImplementation() @endcode */
const bool assistantCnsTests();
static const bool assistantCnsTestsNoexcept() SUSUWU_NOEXCEPT {return templateCatchAll(assistantCnsTests, "assistantCnsTests()");}

/* Universal Resources Locators of hosts which `questionsResponsesFromHosts()` uses
 * Wikipedia is a special case; has compressed downloads of databases ( https://wikipedia.org/wiki/Wikipedia:Database_download )
 * Github is a special case; has compressed downloads of repositories ( https://docs.github.com/en/get-started/start-your-journey/downloading-files-from-github )
 */
extern std::vector<FilePath> assistantCnsDefaultHosts;

/* @throw std::bad_alloc
 * @post If no question, `0 == questionsOrNull.bytecodes[x].size()` (new message synthesis).
 * If no responses, `0 == responsesOrNull.bytecodes[x].size()` (ignore).
 * `questionsOrNull.signatures[x] = Universal Resource Locator`
 * @code sha2(ResultList.bytecodes[x]) == ResultList.hashes[x] @endcode */
void questionsResponsesFromHosts(ResultList &questionsOrNull, ResultList &responsesOrNull, const std::vector<FilePath> &hosts = assistantCnsDefaultHosts);
void questionsResponsesFromXhtml(ResultList &questionsOrNull, ResultList &responsesOrNull, const FilePath &localXhtml = "index.xhtml");
const std::vector<FilePath> assistantParseUrls(const FilePath &localXhtml = "index.xhtml"); /* TODO: for XML/XHTML could just use [ https://www.boost.io/libraries/regex/ https://github.com/boostorg/regex ] or [ https://www.boost.org/doc/libs/1_85_0/doc/html/property_tree/parsers.html#property_tree.parsers.xml_parser https://github.com/boostorg/property_tree/blob/develop/doc/xml_parser.qbk ] */
const FileBytecode assistantParseQuestion(const FilePath &localXhtml = "index.xhtml"); /* TODO: regex or XML parser */
const std::vector<FileBytecode> assistantParseResponses(const FilePath &localXhtml = "index.xhtml"); /* TODO: regex or XML parser */

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
 * To process fast (lag less,) use flags which auto-vectorizes/auto-parallelizes; To do `produceAssistantCns` fast, use TensorFlow's `MapReduce`;
 * https://swudususuwu.substack.com/p/howto-run-devices-phones-laptops
 *
 * Alternative CNS's;
 * https://swudususuwu.substack.com/p/albatross-performs-lots-of-neural
 *
 * Autonomous robots (includes responses to replies from lots of forums);
 * https://swudususuwu.substack.com/p/program-general-purpose-robots-autonomous
 *
 * Simple examples of CNS as virus analysis;
 * https://swudususuwu.substack.com/p/howto-produce-better-virus-scanners
 *
 * Due to understanding of human's consciousness, could undo problems of overpopulation and food shortages, if lots of us become uploads of consciousness (as opposed to below article of how to move whole CNS to robots);
 * https://swudususuwu.substack.com/p/want-this-physical-form-gone-so-wont
 * https://swudususuwu.substack.com/p/destructive-unreversible-upload-of
 */
}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_AssistantCns_hxx */

