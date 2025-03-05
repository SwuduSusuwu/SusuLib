/* Dual licenses: choose "Creative Commons" or "Apache 2" (allows all uses) */
#pragma once
#ifndef INCLUDES_cxx_VirusAnalysis_hxx
#define INCLUDES_cxx_VirusAnalysis_hxx
#include "ClassCns.hxx" /* Cns CnsMode */
#include "ClassFs.hxx" /* ClassFsPath */
#include "ClassPortableExecutable.hxx" /* PortableExecutable */
#include "ClassResultList.hxx" /* ResultList smallestUniqueSubstr */
#include "ClassSha2.hxx" /* classSha2 */
#include "ClassSys.hxx" /* templateCatchAll */
#include "Macros.hxx" /* SUSUWU_NOEXCEPT SUSUWU_UNIT_TESTS */
#include <map> /* std::map */
#include <string> /* std::string */
#include <vector> /* std::vector */
/* (Work-in-progress) virus analysis: uses hashes, signatures, static analysis, sandboxes, plus artificial CNS (central nervous systems) */
namespace Susuwu {
typedef enum VirusAnalysisHook : unsigned char {
	virusAnalysisHookDefault = 0,      /* "real-time" virus scans not initialized */
	virusAnalysisHookQuery   = 0,      /* return present hooks (as enum) */
	virusAnalysisHookClear   = 1 << 0, /* unhook (remove present hooks), then parse rest of bits */
	virusAnalysisHookExec    = 1 << 1, /* hook {execl(), execlp(), execle(), execv(), execvp(), execvpe()} */
	virusAnalysisHookNewFile = 1 << 2, /* hook (for modeNew in {"w+", "a", "a+"}) fwrite((void *)ptr, (size_t)size, (size_t)nmemb, {fopen((const char *)pathname, modeNew), fdopen((int)fd, modeNew), freopen((const char *)pathname, modeNew, (FILE *)stream)}) */
} VirusAnalysisHook;
/* `clang-tidy` suppress: NOLINTBEGIN(clang-analyzer-optin.core.EnumCastOutOfRange, fuchsia-overloaded-operator) */
static const VirusAnalysisHook operator|(VirusAnalysisHook x,  VirusAnalysisHook s) {return static_cast<VirusAnalysisHook>(static_cast<unsigned>(x) | static_cast<unsigned>(s));}
static const VirusAnalysisHook operator&(VirusAnalysisHook x,  VirusAnalysisHook s) {return static_cast<VirusAnalysisHook>(static_cast<unsigned>(x) & static_cast<unsigned>(s));}
/* `clang-tidy` on: NOLINTEND(clang-analyzer-optin.core.EnumCastOutOfRange, fuchsia-overloaded-operator) */
extern VirusAnalysisHook globalVirusAnalysisHook /*= virusAnalysisHookDefault*/; /* Just use virusAnalysisHook() to set+get this, virusAnalysisGetHook() to get this */

typedef enum VirusAnalysisResult : char { /* TODO? All other cases convert to `bool(true)` unless you `switch` all individual enums. The actual constant values do not matter for this. NOLINT(cert-int09-c, readability-enum-initial-value) */
	virusAnalysisAbort = static_cast<char>(false), /* do not launch */
	virusAnalysisPass = static_cast<char>(true), /* launch this (file passes) */
	virusAnalysisRequiresReview, /* submit to hosts to do analysis (infection is difficult to prove, other than known signatures) */
	virusAnalysisContinue /* continue to next tests (is normal; most analyses can not prove a file passes) */
} VirusAnalysisResult; /* if(virusAnalysisAbort != VirusAnalysisResult) {static_assert(true == static_cast<bool>(VirusAnalysisResult));} */

extern ResultList passList, abortList; /* hosts produce, clients initialize shared clones of this from disk */
extern Cns analysisCns, virusFixCns; /* hosts produce, clients initialize shared clones of this from disk */

extern bool virusAnalysisResultListIndex, virusAnalysisResultListWhitespace, virusAnalysisResultListPascal;
/* @throw what `std::istream` throws (std::bad_alloc, std::runtime_error?).
 * @pre @code std::istream(path) @endcode
 * @post @code passList.hashes.size() @endcode */
const bool virusAnalysisInit(const ClassFsPath &path, ResultList &passList, ResultList &abortList); /* virusAnalysisLoadFrom(path + ".{pass, abort}OrNull.config", {pass, abort}list); */
/* @pre @code std::ostream(path) @endcode
 * @throw what `std::ostream` throws. */
void virusAnalysisDumpTo(const ClassFsPath &path, const ResultList &list);
/* @pre @code std::istream(path) @endcode
 * @throw what `std::istream` throws. */
void virusAnalysisLoadFrom(const ClassFsPath &path, ResultList &list);

#if SUSUWU_UNIT_TESTS
/* `return (produceAbortListSignatures(EXAMPLES) && produceAnalysisCns(EXAMPLES) && produceVirusFixCns(EXAMPLES)) && virusAnalysisHookTests();`
 * @throw std::bad_alloc, std::runtime_error
 * @pre @code !analysisCns.isPureVirtual() && !virusFixCns.isPureVirtual() @endcode */
const bool virusAnalysisTests();
static const bool virusAnalysisTestsNoexcept() SUSUWU_NOEXCEPT {return templateCatchAll(virusAnalysisTests, "virusAnalysisTests()");}
const bool virusAnalysisHookTests(); /* return for(x: VirusAnalysisHook) {x == virusAnalysisHook(x)};` */
static const bool virusAnalysisHookTestsNoexcept() SUSUWU_NOEXCEPT {return templateCatchAll(virusAnalysisHookTests, "virusAnalysisHookTests()");}
#endif /* SUSUWU_UNIT_TESTS */

/* Use to turn off, query status of, or turn on what other virus scanners refer to as "real-time scans"
 * @pre @code (virusAnalysisHookDefault == virusAnalysisGetHook() || virusAnalysisHookExec == virusAnalysisGetHook() || virusAnalysisHookNewFile == virusAnalysisGetHook() || (virusAnalysisHookExec | virusAnalysisHookNewFile) == virusAnalysisGetHook()) @endcode
 * @post @code (virusAnalysisHookDefault == virusAnalysisGetHook() || virusAnalysisHookExec == virusAnalysisGetHook() || virusAnalysisHookNewFile == virusAnalysisGetHook() || (virusAnalysisHookExec | virusAnalysisHookNewFile) == virusAnalysisGetHook()) @endcode */
const VirusAnalysisHook virusAnalysisHook(VirusAnalysisHook hookStatus);
static const VirusAnalysisHook virusAnalysisGetHook() {return virusAnalysisHook(virusAnalysisHookQuery);} /* Ignore depth-of-1 recursion: NOLINT(misc-no-recursion) */

const VirusAnalysisResult hashAnalysis(const PortableExecutable &file, const ResultListHash &fileHash); /* `if(abortList[file]) {return Abort;} if(passList[file] {return Pass;} return Continue;` */

/* To produce virus signatures:
 * use passlists (of files reviewed which pass),
 * plus abortlists (of files which failed), such lists as Virustotal has.
 * `produceAbortListSignatures()` is to produce the `abortList.signatures` list, with the smallest substrings unique to infected files; is slow, requires huge database of executables; just hosts should produce this.
 * For clients: Comodo has lists of virus signatures to check against at https://www.comodo.com/home/internet-security/updates/vdp/database.php
 * @throw std::bad_alloc
 * @pre @code passList.bytecodes.size() && abortList.bytecodes.size() && !listsIntersect(passList.bytecodes, abortList.bytecodes) @endcode
 * @post @code abortList.signatures.size() @endcode */
void produceAbortListSignatures(const ResultList &passList, ResultList &abortList);
 /* `if(intersection(file.bytecode, abortList.signatures)) {return VirusAnalysisRequiresReview;} return VirusAnalysisContinue;`
	* @pre @code abortList.signatures.size() @endcode */
const VirusAnalysisResult signatureAnalysis(const PortableExecutable &file, const ResultListHash &fileHash);

/* Static analysis */
/* @throw bad_alloc */
const std::vector<std::string> importedFunctionsList(const PortableExecutable &file);
extern std::vector<std::string> syscallPotentialDangers;
const VirusAnalysisResult staticAnalysis(const PortableExecutable &file, const ResultListHash &fileHash); /* if(intersection(importedFunctionsList(file), dangerFunctionsList)) {return RequiresReview;} return Continue;` */

/* Analysis sandbox */
const VirusAnalysisResult sandboxAnalysis(const PortableExecutable &file, const ResultListHash &fileHash); /* `chroot(strace(file)) >> outputs; return straceOutputsAnalysis(outputs);` */
extern std::vector<std::string> stracePotentialDangers;
const VirusAnalysisResult straceOutputsAnalysis(const ClassFsPath &straceOutput); /* TODO: regex */

/* Analysis CNS */
/* Setup analysis CNS; is slow to produce (requires access to huge file databases);
but once produced, uses few resources (allow clients to do fast analysis.)
 * @pre @code !cns.isPureVirtual() && pass.bytecodes.size() && abort.bytecodes.size() @endcode
 * @post @code cns.isInitialized() @endcode */
void produceAnalysisCns(const ResultList &pass, const ResultList &abort,
	const ResultList &unreviewed = ResultList() /* WARNING! Possible danger to use unreviewed files */,
	Cns &cns = analysisCns
);
/* If bytecode resembles `abortList`, `return 0;`. If undecidable (resembles `unreviewedList`), `return 1 / 2`. If resembles passList, `return 1;`
 * @pre @code cns.isInitialized() @endcode */
const float cnsAnalysisScore(const PortableExecutable &file, const ResultListHash &fileHash, const Cns &cns = analysisCns);
/* `return (bool)round(cnsAnalysisScore(file, fileHash))`
 * @pre @code cns.isInitialized() @endcode */
const VirusAnalysisResult cnsAnalysisImpl(const PortableExecutable &file, const ResultListHash &fileHash, const Cns &cns = analysisCns);
const VirusAnalysisResult cnsAnalysis(const PortableExecutable &file, const ResultListHash &fileHash);

/* temporary caches; memoizes results */
extern std::map<ResultListHash, VirusAnalysisResult> hashAnalysisCaches, signatureAnalysisCaches, staticAnalysisCaches, cnsAnalysisCaches, sandboxAnalysisCaches, manualReviewCaches;
/* call to use new versions of `passList`/`abortList`
 * @post @code *AnalysisCaches.empty() @encode
 */
void virusAnalysisResetCaches() SUSUWU_NOEXCEPT;

typedef const VirusAnalysisResult (*VirusAnalysisFun)(const PortableExecutable &file, const ResultListHash &fileHash);
extern std::vector<VirusAnalysisFun> virusAnalyses;
const VirusAnalysisResult virusAnalysis(const PortableExecutable &file); /* auto hash = classSha2(file.bytecode); for(VirusAnalysisFun analysis : virusAnalyses) { auto result = analysis(file, hash); if(virusAnalysisContinue != result) { return result; } } */
const VirusAnalysisResult virusAnalysisRemoteAnalysis(const PortableExecutable &file, const ResultListHash &fileHash); /* TODO: compatible hosts to upload to */
const VirusAnalysisResult virusAnalysisManualReviewCacheless(const PortableExecutable &file, const ResultListHash &fileHash); /* Ask user to "Block", "Submit to remote hosts for analysis", or "Allow". */
static const VirusAnalysisResult virusAnalysisManualReview(const PortableExecutable &file, const ResultListHash &fileHash) {
	try {
		const auto result = manualReviewCaches.at(fileHash);
		return result;
	} catch (...) {
		return manualReviewCaches[fileHash] = virusAnalysisManualReviewCacheless(file, fileHash);
	}
}
static const VirusAnalysisResult virusAnalysisManualReview(const PortableExecutable &file) { return virusAnalysisManualReview(file, classSha2(file.bytecode)); }
static const VirusAnalysisResult virusAnalysisInteractive(const PortableExecutable &file) { auto result = virusAnalysis(file); if(virusAnalysisRequiresReview == result) { return virusAnalysisManualReview(file); } return result; }

/* Setup virus fix CMS, uses more resources than `produceAnalysisCns()` */
/* `abortOrNull` should map to `passOrNull` (`ResultList` is composed of `std::tuple`s, because just `produceVirusFixCns()` requires this),
 * with `abortOrNull->bytecodes[x] = NULL` (or "\0") for new SW synthesis,
 * and `passOrNull->bytecodes[x] = NULL` (or "\0") if infected and CNS can not cleanse this.
 * @pre @code !cns.isPureVirtual() @endcode
 * @post @code cns.isInitialized() @encode
 */
void produceVirusFixCns(
	const ResultList &passOrNull, /* Expects `resultList->bytecodes[x] = NULL` if does not pass */
	const ResultList &abortOrNull, /* Expects `resultList->bytecodes[x] = NULL` if does pass */
	Cns &cns = virusFixCns
);

/* Uses more resources than `cnsAnalysis()`, can undo infection from bytecodes (restore to fresh SW)
 * @pre @code cns.isInitialized() @endcode */
const std::string cnsVirusFix(const PortableExecutable &file, const Cns &cns = virusFixCns);

/* Related to this:
 * `cnsVirusFix` is close to assistants (such as "ChatGPT 4.0" or "Claude-3 Opus",) "HSOM" (the simple Python artificial CNS) is enough to do this;
 * #include "AssistantCns.cxx"
 *
 * To process fast (lag less,) use flags which auto-vectorizes/auto-parallelizes; To do `produceConversationCns` fast, use TensorFlow's `MapReduce`;
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
#endif /* ndef INCLUDES_cxx_VirusAnalysis_hxx */

