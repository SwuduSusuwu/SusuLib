/* Dual licenses: choose "Creative Commons" or "Apache 2" (allows all uses) */
#ifndef INCLUDES_cxx_VirusAnalysis_cxx
#define INCLUDES_cxx_VirusAnalysis_cxx
#include "ClassCns.hxx" /* Cns CnsMode */
#include "ClassIo.hxx" /* ClassIoBytecode ClassIoPath classIoGetOwnPath classIoHexStr */
#include "ClassPortableExecutable.hxx" /* PortableExecutable PortableExecutableBytecode */
#include "ClassResultList.hxx" /* size_t listMaxSize listHasValue listProduceSignature listFindSignatureOfValue ResultList resultListDumpTo resultListLoadFrom resultListProduceHashes */
#include "ClassSha2.hxx" /* classSha2 */
#include "ClassSys.hxx" /* classSysHasRoot classSysSetRoot classSysKernelSetHook execvex */
#include "Macros.hxx" /* SUSUWU_ERROR SUSUWU_ERRSTR SUSUWU_IF_CPLUSPLUS SUSUWU_NOTICE SUSUWU_EXECUTEVERBOSE SUSUWU_NOTICE_EXECUTEVERBOSE SUSUWU_POSIX SUSUWU_SH_ERROR SUSUWU_UNIT_TESTS */
#include "VirusAnalysis.hxx" /* abortList passList *AnalyisCaches */
#include <algorithm> /* std::sort */
#include SUSUWU_IF_CPLUSPLUS(<cassert>, <assert.h>) /* assert */
#include SUSUWU_IF_CPLUSPLUS(<cmath>, <math.h>) /* round */
#include SUSUWU_IF_CPLUSPLUS(<cstddef>, <stddef.h>) /* size_t */
#include <exception> /* std::exception */
#include <fstream> /* std::ifstream std::ofstream */
#include <ios> /* std::streamsize */
#include <iostream> /* std::cin std::cout std::endl */
#include <limits> /* std::numeric_limits */
#include <map> /* std::map */
#include <sstream> /* std::stringstream */
#include <stdexcept> /* std::runtime_error */
#include <string> /* std::string std::to_string */
#include <tuple> /* std::tuple std::get */
#ifdef SUSUWU_POSIX
#include <unistd.h> /* execv */
#elif defined(SUSUWU_WIN32)
#include <processthreadsapi.h> /* CreateProcessA BOOL DWORD LPCSTR LPPROCESS_INFORMATION LPSECURITY_ATTRIBUTES LPSTARTUPINFOA LPSTR LPVOID */
#endif /* elif DEFINED(SUSUWU_WIN32) */
#include <vector> /* std::vector */
/* (Work-in-progress) virus analysis: uses hashes, signatures, static analysis, sandboxes, plus [artificial *central nervous systems*](./ClassCns.hxx) (such as [*TensorFlow*](https://github.com/SwuduSusuwu/SusuLib/blob/preview/cxx/ClassTensorFlowCns.hxx)) */
namespace Susuwu {
VirusAnalysisHook globalVirusAnalysisHook = virusAnalysisHookDefault; /* Just use virusAnalysisHook() to set+get this, virusAnalysisGetHook() to get this */
ResultList passList, abortList; /* hosts produce, clients initialize shared clones of this from disk */
Cns analysisCns, virusFixCns; /* hosts produce, clients initialize shared clones of this from disk */
std::vector<std::string> syscallPotentialDangers = {
	"memopen", "fwrite", "socket", "GetProcAddress", "IsVmPresent"
};
std::vector<std::string> stracePotentialDangers = {"write(*)"};
std::map<ResultListHash, VirusAnalysisResult> hashAnalysisCaches, signatureAnalysisCaches, staticAnalysisCaches, cnsAnalysisCaches, sandboxAnalysisCaches, manualReviewCaches; /* temporary caches; memoizes results */
void virusAnalysisResetCaches() SUSUWU_NOEXCEPT {
	hashAnalysisCaches.clear();
	signatureAnalysisCaches.clear();
	staticAnalysisCaches.clear();
	cnsAnalysisCaches.clear();
	sandboxAnalysisCaches.clear();
}
std::vector<VirusAnalysisFun> virusAnalyses = {hashAnalysis, signatureAnalysis, staticAnalysis, cnsAnalysis, sandboxAnalysis /* sandbox is slow, so put last*/};

bool virusAnalysisResultListIndex = false, virusAnalysisResultListWhitespace = false, virusAnalysisResultListPascal = true;
const bool virusAnalysisInit(const ClassIoPath &path, ResultList &passList, ResultList &abortList) {
	virusAnalysisLoadFrom(path + ".passOrNull.config", passList);
	virusAnalysisLoadFrom(path + ".abortOrNull.config", abortList);
	return true;
} /* TODO: handle exceptions in this? */
void virusAnalysisDumpTo(const ClassIoPath &path, const ResultList &list) {
	std::ofstream config(path);
	resultListDumpTo(list, config, virusAnalysisResultListIndex, virusAnalysisResultListWhitespace, virusAnalysisResultListPascal);
}
void virusAnalysisLoadFrom(const ClassIoPath &path, ResultList &list) {
#if IFSTREAM_HAS_IOS_BASE
	std::ifstream config(path);
#else
	const PortableExecutableBytecode configPE(path);
	std::stringstream config(configPE.bytecode);
#endif /* ! IFSTREAM_HAS_IOS_BASE */
	list.hashes.clear(); list.signatures.clear(); list.bytecodes.clear(); /* TODO: have `listLoadFrom()` do? */
	resultListLoadFrom(list, config, virusAnalysisResultListIndex, virusAnalysisResultListWhitespace, virusAnalysisResultListPascal);
}
#if SUSUWU_UNIT_TESTS
const bool virusAnalysisTests() {
	ResultList abortOrNull; {
		abortOrNull.hashes = {}, abortOrNull.signatures = {}, abortOrNull.bytecodes = { /* Produce from an antivirus vendor's (such as VirusTotal.com's) infection databases */
			"infection",
			"infectedSW",
			"corruptedSW",
			""
		};
	}
	ResultList passOrNull; {
		passOrNull.hashes = {}, passOrNull.signatures = {}, passOrNull.bytecodes = { /* Produce from an antivirus vendor's (such as VirusTotal.com's) fresh-files databases */
			"",
			"SW",
			"SW",
			"newSW"
		};
	}
	resultListProduceHashes(passOrNull);
	resultListProduceHashes(abortOrNull);
	produceAbortListSignatures(passOrNull, abortOrNull);
	const ClassIoPath gotOwnPath = classIoGetOwnPath(); /* replaced `argv[0]`, ["Uncontrolled data used in path expression"](https://github.com/SwuduSusuwu/SusuLib/security/code-scanning/1277) fix. */
	virusAnalysisInitTests(gotOwnPath, passOrNull, abortOrNull);
	SUSUWU_NOTICE("resultListDumpTo(.list = passOrNull, .os = std::cout, .index = true, .whitespace = true, .pascalValues = false);");
	SUSUWU_EXECUTEVERBOSE(resultListDumpTo(passOrNull, std::cout, true, true, false));
	SUSUWU_NOTICE_EXECUTEVERBOSE((resultListDumpTo(/*.list = */abortOrNull, /*.os = */std::cout, /*.index = */false, /*.whitespace = */false, /*.pascalValues = */false), std::cout << std::endl));
	assert(4 == passOrNull.bytecodes.size());
	assert(passOrNull.bytecodes.size() - 1 /* 2 instances of "SW", discount dup */ == passOrNull.hashes.size());
	assert(0 == passOrNull.signatures.size()); /* NOLINT(readability-container-size-empty); all `.size()`, intuitive */
	assert(4 == abortOrNull.bytecodes.size());
	assert(abortOrNull.bytecodes.size() == abortOrNull.hashes.size());
	assert(abortOrNull.bytecodes.size() - 1 /* discount empty substr */ == abortOrNull.signatures.size());
	produceAnalysisCns(passOrNull, abortOrNull, ResultList(), analysisCns);
	produceVirusFixCns(passOrNull, abortOrNull, virusFixCns);
	if(ClassIoPath() != gotOwnPath) {
		const PortableExecutableBytecode executable(gotOwnPath);
		if(virusAnalysisAbort == virusAnalysisInteractive(executable)) {
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "{virusAnalysisAbort == virusAnalysisInteractive((args[0]);} /* With such false positives, shouldn't hook kernel modules (next test is to hook+unhook `exec*` to scan programs on launch). */"));
		}
		const ResultList origPassList = passList, origAbortList = abortList;
		passList.bytecodes.push_back(executable.bytecode);
		abortList.bytecodes.push_back("test");
		produceAbortListSignatures(passList, abortList);
		if(virusAnalysisAbort == virusAnalysisInteractive(executable)) {
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "{virusAnalysisAbort == virusAnalysisInteractive(args[0]);} /* Ignored `signaturesAnalysisCaches`. */"));
		}
		virusAnalysisResetCaches();
		if(virusAnalysisAbort != virusAnalysisInteractive(executable)) {
			throw std::runtime_error(SUSUWU_ERRSTR(SUSUWU_SH_ERROR, "{virusAnalysisAbort != virusAnalysisInteractive(args[0]);} /* This test was supposed to match positive but did not. */"));
		}
		passList = origPassList, abortList = origAbortList;
	}
	const bool originalRootStatus = classSysHasRoot();
	classSysSetRoot(true);
	virusAnalysisHookTests();
	classSysSetRoot(originalRootStatus);
	return true;
}

const bool virusAnalysisInitTests(const ClassIoPath &path, ResultList &passList, ResultList &abortList) {
	const std::string passPath = path + ".passList.config";
	const std::string abortPath = path + ".abortList.config";
	try {
		const std::ifstream config(passPath);
	} catch (std::exception &w) {
		SUSUWU_WARNING("`std::ifstream config(\"" + passPath + "\");` threw \" " + w.what() + "\"; will skip `virusAnalysisDumpTo()` and `virusAnalysisLoadFrom()` tests.");
		return false;
	}
	virusAnalysisDumpTo(passPath, passList);
	virusAnalysisDumpTo(abortPath, abortList);
	SUSUWU_NOTICE("resultListDumpTo(.list = passList, .os = std::cout, .index = true, .whitespace = true, .pascalValues = false);");
	SUSUWU_EXECUTEVERBOSE(resultListDumpTo(passList, std::cout, true, true, false));
	SUSUWU_NOTICE_EXECUTEVERBOSE((resultListDumpTo(/*.list = */abortList, /*.os = */std::cout, /*.index = */false, /*.whitespace = */false, /*.pascalValues = */false), std::cout << std::endl));
	virusAnalysisLoadFrom(passPath, passList);
	virusAnalysisLoadFrom(abortPath, abortList);
	return true;
}
/* `clang-tidy` suppress: NOLINTBEGIN(readability-implicit-bool-conversion) */
const bool virusAnalysisHookTests() {
	const VirusAnalysisHook originalHookStatus = virusAnalysisGetHook();
	VirusAnalysisHook hookStatus = virusAnalysisHook(virusAnalysisHookClear | virusAnalysisHookExec);
	if(virusAnalysisHookExec != hookStatus) {
		throw std::runtime_error("`virusAnalysisHook(virusAnalysisHookClear | virusAnalysisHookExec)` == " + std::to_string(hookStatus));
		return false;
	}
	hookStatus = virusAnalysisHook(virusAnalysisHookClear | virusAnalysisHookNewFile);
	if(virusAnalysisHookNewFile != hookStatus) {
		throw std::runtime_error("`virusAnalysisHook(virusAnalysisHookClear | virusAnalysisHookNewFile)` == " + std::to_string(hookStatus));
		return false;
	}
	hookStatus = virusAnalysisHook(virusAnalysisHookClear);
	if(virusAnalysisHookDefault != hookStatus) {
		throw std::runtime_error("`virusAnalysisHook(virusAnalysisHookClear)` == " + std::to_string(hookStatus));
		return false;
	}
	hookStatus = virusAnalysisHook(virusAnalysisHookExec | virusAnalysisHookNewFile);
	if((virusAnalysisHookExec | virusAnalysisHookNewFile) != hookStatus) {
		throw std::runtime_error("`virusAnalysisHook(virusAnalysisExec | virusAnalysisHookNewFile)` == " + std::to_string(hookStatus));
		return false;
	}
	hookStatus = virusAnalysisHook(virusAnalysisHookClear | originalHookStatus);
	if(originalHookStatus != hookStatus) {
		throw std::runtime_error("`virusAnalysisHook(virusAnalysisHookClear | originalHookStatus)` == " + std::to_string(hookStatus));
		return false;
	}
	return true;
}
#endif /* SUSUWU_UNIT_TESTS */

namespace { /* [misc-use-anonymouse-namespace] */
static const bool virusAnalysisImpl(const PortableExecutable &file) {
	switch(virusAnalysis(file)) {
	case virusAnalysisPass:
		return true; /* launch this */
	case virusAnalysisRequiresReview:
		return (virusAnalysisPass == virusAnalysisManualReview(file));
	default:
		return false; /* abort */
	}
}
}; /* namespace */
const VirusAnalysisHook virusAnalysisHook(VirusAnalysisHook hookStatus) { /* Ignore depth-of-1 recursion: NOLINT(misc-no-recursion) */
	const VirusAnalysisHook originalHookStatus = globalVirusAnalysisHook;
	if(virusAnalysisHookQuery == hookStatus || originalHookStatus == hookStatus) {
		return originalHookStatus;
	}
	if(virusAnalysisHookClear & hookStatus) {
		/* TODO: undo OS-specific "hook"s/"callback"s */
		globalVirusAnalysisHook = virusAnalysisHookDefault;
	}
	if(virusAnalysisHookExec & hookStatus) {
#ifdef SUSUWU_POSIX
		auto lambdaScanExecv = [](const char *pathname, char *const argv[]) { /* NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays); this is the standard function signature. */
			return static_cast<int>(virusAnalysisImpl(PortableExecutable(pathname)));
		};
		classSysKernelSetHook(execv, lambdaScanExecv);
#elif defined(SUSUWU_WIN32) /* def SUSUWU_POSIX else */
		auto lambdaScanCreateProcessA = [](LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR                lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation) {
			return virusAnalysisImpl(PortableExecutable(lpApplicationName));
		};
		classSysKernelSetHook(CreateProcessA, lambdaScanCreateProcessA);
#else /* defined(SUSUWU_WIN32) else */
		SUSUWU_ERROR("virusAnalysisHook(virusAnalysisHookExec) { if(!SUSUWU_POSIX && !SUSUWU_WIN32) { /* TODO: you can contribute or post to https://github.com/SwuduSusuwu/SusuLib/issues/new */ } }");
#endif /* defined(SUSUWU_WIN32) else */
		globalVirusAnalysisHook = (globalVirusAnalysisHook | virusAnalysisHookExec);
	}
	if(virusAnalysisHookNewFile & hookStatus) {
//		classSysKernelSetHook(fwrite, lambdaScanFwrite); /* TODO: OS-specific "hook"/"callback" for new files/downloads */
		globalVirusAnalysisHook = (globalVirusAnalysisHook | virusAnalysisHookNewFile);
	}
	return virusAnalysisGetHook();
} /* `clang-tidy` on: NOLINTEND(readability-implicit-bool-conversion) */

const VirusAnalysisResult virusAnalysis(const PortableExecutable &file) {
	const auto fileHash = classSha2(file.bytecode);
	for(const auto &analysis : virusAnalyses) {
		switch(analysis(file, fileHash)) {
			case virusAnalysisPass:
				return virusAnalysisPass;
			case virusAnalysisRequiresReview:
				return virusAnalysisRequiresReview;
			case virusAnalysisAbort:
				return virusAnalysisAbort;
			case virusAnalysisContinue:
				continue;
		}
	}
	return virusAnalysisPass;
}
const VirusAnalysisResult virusAnalysisRemoteAnalysis(const PortableExecutable &file, const ResultListHash &fileHash) {
	SUSUWU_NOTICE("virusAnalysisRemoteAnalysis: {/* TODO: compatible hosts to upload to */}");
	return virusAnalysisRequiresReview;
}
const VirusAnalysisResult virusAnalysisManualReviewCacheless(const PortableExecutable &file, const ResultListHash &fileHash) {
	SUSUWU_INFO("virusAnalysis(\"" + file.path + "\") {return virusAnalysisRequiresReview;}, what do you wish to do?");
	while(true) {
		std::cout << "Allowed responses: ab(o)rt = `virusAnalysisAbort`, (s)ubmit to remote host for analysis /* TODO */ = `virusAnalysisRequiresReview`, la(u)nch = `virusAnalysisPass`. {'o', 's', or 'u'}. Input response: [s]";
		const char defaultResponse = 's';
		char response = defaultResponse;
		if(!std::cin.get(response)) {
			SUSUWU_INFO("virusAnalysisManualReview(): {(!std::cin.get(response)) /* Input disabled */}, will assume default response.");
		} else if('\n' != response) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		if('\n' == response || '\r' == response) {
			response = defaultResponse;
		}
		switch(response) {
		case 'o':
			return virusAnalysisAbort;
		case 's':
			return virusAnalysisRemoteAnalysis(file, fileHash);;
		case 'u':
			return virusAnalysisPass;
		default:
			SUSUWU_WARNING(std::string("virusAnalysisManualReview(): {\"response: '") + response + "'\" isn't valid. Choose from list (or press <enter> to default to '" + defaultResponse + "')}");
		}
	}
}

const VirusAnalysisResult hashAnalysis(const PortableExecutable &file, const ResultListHash &fileHash) {
	try {
		const auto result = hashAnalysisCaches.at(fileHash);
		return result;
	} catch (...) {
		if(listHasValue(passList.hashes, fileHash)) {
			return hashAnalysisCaches[fileHash] = virusAnalysisPass;
		} else if(listHasValue(abortList.hashes, fileHash)) {
			SUSUWU_NOTICE("hashAnalysis(/*.file =*/ \"" + file.path + "\", /*.fileHash =*/ 0x" + classIoHexStr(fileHash) + ") {return virusAnalysisAbort;} /* due to hash 0x" + classIoHexStr(fileHash) + " (found in `abortList.hashes`). You should treat this as a virus detection if this was not a test. */");
			return hashAnalysisCaches[fileHash] = virusAnalysisAbort;
		} else {
			return hashAnalysisCaches[fileHash] = virusAnalysisContinue; /* continue to next tests */
		}
	}
}

const VirusAnalysisResult signatureAnalysis(const PortableExecutable &file, const ResultListHash &fileHash) {
	try {
		const auto result = signatureAnalysisCaches.at(fileHash);
		return result;
	} catch (...) {
		auto match = listFindSignatureOfValue(abortList.signatures, file.bytecode);
		if(-1 != match.fileOffset) {
			SUSUWU_NOTICE("signatureAnalysis(/*.file =*/ \"" + file.path + "\", /*.fileHash =*/ 0x" + classIoHexStr(fileHash) + ") {return virusAnalysisAbort;} /* due to signature 0x" + classIoHexStr(match.signature) + " found at offset=" + std::to_string(match.fileOffset) + ". You should treat this as a virus detection if this was not a test. */");
			return signatureAnalysisCaches[fileHash] = virusAnalysisAbort;
		}
		return signatureAnalysisCaches[fileHash] = virusAnalysisContinue;
	}
}

void produceAbortListSignatures(const ResultList &passList, ResultList &abortList) {
	abortList.signatures.reserve(abortList.bytecodes.size());
	for(const auto &file : abortList.bytecodes) {
		const auto tuple = listProduceSignature(passList.bytecodes, file);
		const auto itBegin = std::get<0>(tuple),  itEnd = std::get<1>(tuple);
		if(itBegin < itEnd) { /* require `(0 < ResultListSignature.size())` to prevent crashes */
			abortList.signatures.push_back(ResultListSignature(itBegin, itEnd));
		}
	} /* The most simple signature is a substring, but some analyses use regexes. */
}

const std::vector<std::string> importedFunctionsList(const PortableExecutable &file) {
	return {}; /* fixes crash, until importedFunctionsList is implemented/finished */
/* TODO
 * Resources; “Portable Executable” for Windows ( https://learn.microsoft.com/en-us/windows/win32/debug/pe-format https://wikipedia.org/wiki/Portable_Executable ,
 * “Extended Linker Format” for most others such as UNIX/Linuxes ( https://wikipedia.org/wiki/Executable_and_Linkable_Format ),
 * shows how to analyse lists of libraries(.DLL's/.SO's) the SW uses,
 * plus what functions (new syscalls) the SW can goto through `jmp`/`call` instructions.
 *
 * "x86" instruction list for Intel/AMD ( https://wikipedia.org/wiki/x86 ),
 * "aarch64" instruction list for most smartphones/tablets ( https://wikipedia.org/wiki/aarch64 ),
 * shows how to analyse what OS functions the SW goes to without libraries (through `int`/`syscall`, old; most new SW uses `jmp`/`call`.)
 * Plus, instructions lists show how to analyse what args the apps/SW pass to functions/syscalls (simple for constant args such as "push 0x2; call functions;",
 * but if registers/addresses as args such as "push eax; push [address]; call [address2];" must guess what is *"eax"/"[address]"/"[address2]", or use sandboxes.
 *
 * https://www.codeproject.com/Questions/338807/How-to-get-list-of-all-imported-functions-invoked shows how to analyse dynamic loads of functions (if do this, `syscallPotentialDangers[]` does not include `GetProcAddress()`.)
 */
}

const VirusAnalysisResult staticAnalysis(const PortableExecutable &file, const ResultListHash &fileHash) {
	try {
		const auto result = staticAnalysisCaches.at(fileHash);
		return result;
	} catch (...) {
		auto syscallsUsed = importedFunctionsList(file);
		std::sort(syscallPotentialDangers.begin(), syscallPotentialDangers.end());
		std::sort(syscallsUsed.begin(), syscallsUsed.end());
		if(listsIntersect(syscallPotentialDangers, syscallsUsed)) {
			return staticAnalysisCaches[fileHash] = virusAnalysisRequiresReview;
		}
		return staticAnalysisCaches[fileHash] = virusAnalysisContinue;
	}
}

const VirusAnalysisResult sandboxAnalysis(const PortableExecutable &file, const ResultListHash &fileHash) {
	try {
		const auto result = sandboxAnalysisCaches.at(fileHash);
		return result;
	} catch (...) {
#ifdef SUSUWU_POSIX
		execvex("cp -r '/usr/home/sandbox/' '/usr/home/sandbox.bak'"); /* or produce FS snapshot */
		execvex("cp '" + file.path + "' '/usr/home/sandbox/'");
		execvex("chroot '/usr/home/sandbox/' \"strace basename '" + file.path + "'\" >> strace.outputs");
		execvex("mv/ '/usr/home/sandbox/strace.outputs' '/tmp/strace.outputs'");
		execvex("rm -r '/usr/home/sandbox/' && mv '/usr/home/sandbox.bak' '/usr/home/sandbox/'"); /* or restore FS snapshot */
		return sandboxAnalysisCaches[fileHash] = straceOutputsAnalysis("/tmp/strace.outputs");
#else /* ndef SUSUWU_POSIX */
		SUSUWU_ERROR("sandboxAnalysis: {#ifndef SUSUWU_POSIX /* TODO: convert to win32 */}");
		return sandboxAnalysisCaches[fileHash] = virusAnalysisRequiresReview;
#endif /* ndef SUSUWU_POSIX */
	}
}
const VirusAnalysisResult straceOutputsAnalysis(const ClassIoPath &straceOutput) {
		auto straceDump = std::ifstream(straceOutput);
		std::vector<std::string> straceOutputs /*= explodeToList(straceDump, "\n")*/;
		for(std::string straceOutputIt; std::getline(straceDump, straceOutputIt); ) {
			straceOutputs.push_back(straceOutputIt);
		}
		std::sort(stracePotentialDangers.begin(), stracePotentialDangers.end());
		std::sort(straceOutputs.begin(), straceOutputs.end());
		if(listsIntersect(stracePotentialDangers, straceOutputs)) { /* Todo: regex */
			return virusAnalysisRequiresReview;
		}
	return virusAnalysisContinue;
}

void produceAnalysisCns(const ResultList &pass, const ResultList &abort,
const ResultList &unreviewed /* = ResultList(), WARNING! Possible danger to use unreviewed files */,
Cns &cns /* = analysisCns */
) {
	std::vector<std::tuple<ClassIoBytecode, float>> inputsToOutputs;
	const size_t maxPassSize = listMaxSize(pass.bytecodes);
	const size_t maxAbortSize = listMaxSize(abort.bytecodes);
	const size_t maxDepthOfOpcodes = 6666; /* is not max depth of callstack, but of instruction pointer. TODO: compute this */
	const size_t maxWidthOfOpcodes = (maxPassSize > maxAbortSize ? maxPassSize : maxAbortSize);
	cns.setInputMode(cnsModeString);
	cns.setOutputMode(cnsModeFloat);
	cns.setInputNeurons(maxWidthOfOpcodes);
	cns.setOutputNeurons(1);
	cns.setLayersOfNeurons(maxDepthOfOpcodes);
	cns.setNeuronsPerLayer(maxWidthOfOpcodes /* TODO: reduce this */);
	inputsToOutputs.reserve(pass.bytecodes.size());
	for(const auto &bytecodes : pass.bytecodes) {
		inputsToOutputs.push_back({bytecodes, 1.0});
	}
	cns.setupSynapses(inputsToOutputs);
	inputsToOutputs.clear();
	if(!unreviewed.bytecodes.empty()) { /* WARNING! Possible danger to use unreviewed files */
		inputsToOutputs.reserve(unreviewed.bytecodes.size());
		for(const auto &bytecodes : unreviewed.bytecodes) {
			inputsToOutputs.push_back({bytecodes, 1 / 2});
		}
		cns.setupSynapses(inputsToOutputs);
		inputsToOutputs.clear();
	}
	inputsToOutputs.reserve(abort.bytecodes.size());
	for(const auto &bytecodes : abort.bytecodes) {
		inputsToOutputs.push_back({bytecodes, 0.0});
	}
	cns.setupSynapses(inputsToOutputs);
	inputsToOutputs.clear();
}
const float cnsAnalysisScore(const PortableExecutable &file, const ResultListHash &fileHash, const Cns &cns /* = analysisCns */) {
	return cns.processToFloat(file.bytecode);
}
const VirusAnalysisResult cnsAnalysisImpl(const PortableExecutable &file, const ResultListHash &fileHash, const Cns &cns /* = analysisCns */) {
	try {
		const auto result = cnsAnalysisCaches.at(fileHash);
		return result;
	} catch (...) {
		return cnsAnalysisCaches[fileHash] = static_cast<bool>(round(cnsAnalysisScore(file, fileHash, cns))) ? virusAnalysisContinue : virusAnalysisRequiresReview;
	}
}
const VirusAnalysisResult cnsAnalysis(const PortableExecutable &file, const ResultListHash &fileHash) {
	return cnsAnalysisImpl(file, fileHash);
}

void produceVirusFixCns(const ResultList &passOrNull, const ResultList &abortOrNull, Cns &cns /* = virusFixCns */) {
	std::vector<std::tuple<ClassIoBytecode, ClassIoBytecode>> inputsToOutputs;
	const size_t maxDepthOfOpcodes = 6666; /* is not max depth of callstack, but of instruction pointer. TODO: compute this */
	const size_t maxPassSize = listMaxSize(passOrNull.bytecodes);
	const size_t maxAbortSize = listMaxSize(abortOrNull.bytecodes);
	const size_t maxWidthOfOpcodes = (maxPassSize > maxAbortSize ? maxPassSize : maxAbortSize);
	cns.setInputMode(cnsModeString);
	cns.setOutputMode(cnsModeString);
	cns.setInputNeurons(maxAbortSize);
	cns.setOutputNeurons(maxPassSize);
	cns.setLayersOfNeurons(maxDepthOfOpcodes);
	cns.setNeuronsPerLayer(maxWidthOfOpcodes /* TODO: reduce this */);
	assert(passOrNull.bytecodes.size() == abortOrNull.bytecodes.size());
	inputsToOutputs.reserve(passOrNull.bytecodes.size());
	for(size_t x = 0; passOrNull.bytecodes.size() > x; ++x) {
		inputsToOutputs.push_back({abortOrNull.bytecodes[x], passOrNull.bytecodes[x]});
	}
	cns.setupSynapses(inputsToOutputs);
}

const ClassIoBytecode cnsVirusFix(const PortableExecutable &file, const Cns &cns /* = virusFixCns */) {
	return cns.processToString(file.bytecode);
}

/* To process fast (lag less,) use flags which auto-vectorizes/auto-parallelizes; To do `produce*Cns` fast, use TensorFlow's `MapReduce`;
 * https://swudususuwu.substack.com/p/howto-run-devices-phones-laptops
 */
}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_VirusAnalysis_cxx */

