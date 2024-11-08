/* Dual licenses: choose "Creative Commons" or "Apache 2" (allows all uses) */
#ifndef INCLUDES_cxx_ClassSha2_cxx
#define INCLUDES_cxx_ClassSha2_cxx
#include "ClassPortableExecutable.hxx" /* FileBytecode FileHash */
#include "ClassSha2.hxx"
#include "ClassSys.hxx" /* classSysHexStr classSysUSecondClock templateCatchAll */
#include "Macros.hxx" /* SUSUWU_NOEXCEPT SUSUWU_NOTICE_EXECUTE SUSUWU_PRINT SUSUWU_INFO SUSUWU_NOTICE */
#include <climits> /* CHAR_BIT */
#include <stdexcept> /* std::runtime_error */
#include <string> /* std::to_string */
extern "C" {
/* Uses https://www.rfc-editor.org/rfc/rfc6234#section-8.2.2 */
#include "../c/rfc6234/sha.h" /* SHA256HashSize, SHA256Context, SHA256Reset, SHA256Input, SHA256Result, SHA1*, SHA512*  */
}
namespace Susuwu {
Sha2 sha2 = sha256;
/* const */ FileHash /* 128 bits, not null-terminated */ sha1(const FileBytecode &bytecode) {
	FileHash result;
	SHA1Context context;
	result.resize(SHA1HashSize);
	SHA1Reset(&context); /* If undefined, link sha1.o */
	SHA1Input(&context, reinterpret_cast<const unsigned char *>(bytecode.c_str()), bytecode.size());
	SHA1Result(&context, const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>(result.c_str())));
	return result;
}

/* const */ FileHash /* 256 bits, not null-terminated */ sha256(const FileBytecode &bytecode) {
	FileHash result;
	SHA256Context context;
	result.resize(SHA256HashSize);
	SHA256Reset(&context); /* If undefined, link sha224-256.o */
	SHA256Input(&context, reinterpret_cast<const unsigned char *>(bytecode.c_str()), bytecode.size());
	SHA256Result(&context, const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>(result.c_str())));
	return result;
}

/* const */ FileHash /* 512 bits, not null-terminated */ sha512(const FileBytecode &bytecode) {
	FileHash result;
	SHA512Context context;
	result.resize(SHA512HashSize);
	SHA512Reset(&context); /* If undefined, link sha384-512.o */
	SHA512Input(&context, reinterpret_cast<const unsigned char *>(bytecode.c_str()), bytecode.size());
	SHA512Result(&context, const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>(result.c_str())));
	return result;
}

const bool classSha2Tests() { /* is just to test glue code (which wraps rfc6234). Use `../c/rfc6234/shatest.c` to test rfc6234. */
	const char nulls[65536 /* 65536 == 2^16 == 64kb */] = {0};
	std::string nullStr(nulls, &nulls[65536]);
	const ClassSysUSeconds tsDrift = classSysUSecondClock(), ts2Drift = classSysUSecondClock() - tsDrift, ts = classSysUSecondClock();
	const FileHash hash = sha2(nullStr);
	const ClassSysUSeconds ts2 = classSysUSecondClock() - ts2Drift;
	const std::string hashStrCompute = "0x" + classSysHexStr(hash);
	const std::string hashStrTrue = "0xde2f256064a0af797747c2b9755dcb9f3df0de4f489eac731c23ae9ca9cc31";
	if(ts == ts2) {
		SUSUWU_PRINT(WARNING, "0 ms (0 μs) to compute `sha2(std::string(nulls, &nulls[65536])) == " + hashStrCompute + "` = inf mbps");
	} else {
		SUSUWU_INFO(std::to_string((ts2 - ts) / 1000) + " ms (" + std::to_string(ts2 - ts) + " μs) to compute `sha2(std::string(nulls, &nulls[65536])) == " + hashStrCompute + "` = " + std::to_string(float(65536) * CHAR_BIT /* to bits */ / (ts2 - ts) /* to bpμs */ * 1000000 /* to bps */ / (1 << 20) /* to mbps */) + "mbps");
	}
	SUSUWU_NOTICE("This `sha2()` is from `./build.sh --debug`: `./build.sh --release` has 4x this throughput");
	if(0 == hash.size()) {
		throw std::runtime_error(SUSUWU_ERRSTR(ERROR, "`0 == sha2(std::string()).size();"));
	} else if(hashStrTrue.size() != hashStrCompute.size() && sha256 == sha2) {
		throw std::runtime_error(SUSUWU_ERRSTR(ERROR, "`sha2 = sha256;`, but `(" + std::to_string(hash.size()) + " == sha2(std::string()).size())`"));
	} else if(hashStrTrue.size() != hashStrCompute.size()) {
		SUSUWU_INFO("`(sha256 != sha2)`, `(" + std::to_string(hash.size()) + " == sha2(std::string()).size())`");
	} else if(hashStrTrue != hashStrCompute) {
		throw std::runtime_error(SUSUWU_ERRSTR(ERROR, "sha2(char nulls[65535] = {0}) did not compute " + hashStrTrue));
	}
	return true;
}
const bool classSha2TestsNoexcept() SUSUWU_NOEXCEPT {return templateCatchAll(classSha2Tests, "classSha2Tests()");}

}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassSha2_cxx */

