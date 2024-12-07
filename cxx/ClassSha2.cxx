/* Dual licenses: choose "Creative Commons" or "Apache 2" (allows all uses) */
#ifndef INCLUDES_cxx_ClassSha2_cxx
#define INCLUDES_cxx_ClassSha2_cxx
#include "ClassPortableExecutable.hxx" /* FileBytecode FileHash */
#include "ClassSha2.hxx"
#include "ClassSys.hxx" /* classSysHexStr classSysUSecondClock templateCatchAll */
#include "Macros.hxx" /* SUSUWU_IF_CPLUSPLUS SUSUWU_NOEXCEPT SUSUWU_NOTICE_EXECUTE SUSUWU_INFO SUSUWU_NOTICE SUSUWU_WARNING */
#include SUSUWU_IF_CPLUSPLUS(<climits>, <limits.h>) /* CHAR_BIT */
#include <stdexcept> /* std::runtime_error */
#include <string> /* std::to_string */
extern "C" {
/* Uses https://www.rfc-editor.org/rfc/rfc6234#section-8.2.2 */
#include "../c/rfc6234/sha.h" /* SHA256HashSize, SHA256Context, SHA256Reset, SHA256Input, SHA256Result, SHA1*, SHA512* */
}
namespace Susuwu {
ClassSha2 classSha2 = classSha256;
/* const */ FileHash /* 128 bits, not null-terminated */ classSha1(const FileBytecode &bytecode) {
	FileHash result;
	SHA1Context context;
	result.resize(SHA1HashSize);
	SHA1Reset(&context); /* If `undefined symbol: SHA1Reset`, affix `classSha1.o` to `${LD_FLAGS}` */
	SHA1Input(&context, reinterpret_cast<const unsigned char *>(&bytecode[0]), bytecode.size());
	SHA1Result(&context, reinterpret_cast<unsigned char *>(&result[0]));
	return result;
}
/* const */ FileHash /* 256 bits, not null-terminated */ classSha256(const FileBytecode &bytecode) {
	FileHash result;
	SHA256Context context;
	result.resize(SHA256HashSize);
	SHA256Reset(&context); /* If `undefined symbol: SHA256Reset`, affix `classSha224-256.o` to `${LD_FLAGS}` */
	SHA256Input(&context, reinterpret_cast<const unsigned char *>(&bytecode[0]), bytecode.size());
	SHA256Result(&context, reinterpret_cast<unsigned char *>(&result[0]));
	return result;
}
/* const */ FileHash /* 512 bits, not null-terminated */ classSha512(const FileBytecode &bytecode) {
	FileHash result;
	SHA512Context context;
	result.resize(SHA512HashSize);
	SHA512Reset(&context); /* If `undefined symbol: SHA512Reset`, affix `sha384-512.o` to `${LD_FLAGS}` */
	SHA512Input(&context, reinterpret_cast<const unsigned char *>(&bytecode[0]), bytecode.size());
	SHA512Result(&context, reinterpret_cast<unsigned char *>(&result[0]));
	return result;
}

const bool classSha2Tests() { /* is just to test glue code (which wraps rfc6234). Use `../c/rfc6234/shatest.c` to test rfc6234. */
	const char nulls[65536 /* 65536 == 2^16 == 64kb */] = {0};
	std::string nullStr(nulls, &nulls[65536]);
	const ClassSysUSeconds tsDrift = classSysUSecondClock(), ts2Drift = classSysUSecondClock() - tsDrift, ts = classSysUSecondClock();
	const FileHash hash = classSha2(nullStr);
	const ClassSysUSeconds ts2 = classSysUSecondClock() - ts2Drift;
	const std::string hashStrCompute = "0x" + classSysHexStr(hash);
	const std::string hashStrTrue = "0xde2f256064a0af797747c2b97505dc0b9f3df0de4f489eac731c23ae9ca9cc31";
	if(ts == ts2) {
		SUSUWU_WARNING("0 ms (0 μs) to compute `classSha2(std::string(nulls, &nulls[65536])) == " + hashStrCompute + "` = inf mbps");
	} else {
		SUSUWU_INFO(std::to_string((ts2 - ts) / 1000) + " ms (" + std::to_string(ts2 - ts) + " μs) to compute `classSha2(std::string(nulls, &nulls[65536])) == " + hashStrCompute + "` = " + std::to_string(float(65536) * CHAR_BIT /* to bits */ / (ts2 - ts) /* to bpμs */ * 1000000 /* to bps */ / (1 << 20) /* to mbps */) + "mbps");
	}
	SUSUWU_NOTICE("This `classSha2()` is from `./build.sh --debug`: `./build.sh --release` has 2x this throughput");
	if(0 == hash.size()) {
		throw std::runtime_error(SUSUWU_ERRSTR(ERROR, "`0 == classSha2(std::string()).size();"));
	} else if(hashStrTrue.size() != hashStrCompute.size() && classSha256 == classSha2) {
		throw std::runtime_error(SUSUWU_ERRSTR(ERROR, "`classSha2 = classSha256;`, but `(" + std::to_string(hash.size()) + " == sha2(std::string()).size())`"));
	} else if(hashStrTrue.size() != hashStrCompute.size()) {
		SUSUWU_INFO("`(classSha256 != classSha2)`, `(" + std::to_string(hash.size()) + " == sha2(std::string()).size())`");
	} else if(hashStrTrue != hashStrCompute) {
		throw std::runtime_error(SUSUWU_ERRSTR(ERROR, "classSha2(char nulls[65535] = {0}) did not compute " + hashStrTrue));
	}
	return true;
}
const bool classSha2TestsNoexcept() SUSUWU_NOEXCEPT {return templateCatchAll(classSha2Tests, "classSha2Tests()");}

}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassSha2_cxx */

