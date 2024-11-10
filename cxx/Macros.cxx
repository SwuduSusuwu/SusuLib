/* Licenses: allows all uses ("Creative Commons"/"Apache 2") */
#ifndef INCLUDES_cxx_Macros_cxx
#define INCLUDES_cxx_Macros_cxx
/* This is just unit tests. `Macros.hxx` has all which has actual use. */
#include "Macros.hxx" /* IF_SUSUWU_CPLUSPLUS SUSUWU_ASSUME SUSUWU_EXPECTS SUSUWU_ENSURES SUSUWU_NOEXCEPT SUSUWU_NORETURN SUSUWU_STATIC_ASSERT SUSUWU_UNREACHABLE */
#include IF_SUSUWU_CPLUSPLUS(<cstdlib>, <stdlib.h>) /* exit */
#ifdef SUSUWU_CXX11
#	include <type_traits> /* is_empty */
#endif /* def SUSUWU_CXX11 */
namespace Susuwu {
static void macrosNoUniqueAddressTest() {
	typedef class Zero {} Zero;
	class SubClassWithBaseSubobject : public Zero {bool boo;};
	class SubClassWithMemberSubobject {bool boo; public: Zero zero;};
	class SubClassWithMemberSubobjectNoAddress {bool boo; public: SUSUWU_NO_UNIQUE_ADDRESS Zero zero;};
#ifdef SUSUWU_CXX11 /* this is true without C++11, but `std::is_empty` doesn't exist in C++98. */
	SUSUWU_STATIC_ASSERT(std::is_empty<Zero>::value);
#endif /* def SUSUWU_CXX11 */
	SUSUWU_STATIC_ASSERT(sizeof(bool) == sizeof(SubClassWithBaseSubobject));
	SUSUWU_STATIC_ASSERT(sizeof(bool) < sizeof(SubClassWithMemberSubobject));
#ifdef SUSUWU_CXX20 /* `[[no_unique_address]]` */
	SUSUWU_STATIC_ASSERT(sizeof(bool) == sizeof(SubClassWithMemberSubobjectNoAddress));
#else /* def SUSUWU_CXX20 else */
	SUSUWU_STATIC_ASSERT(sizeof(bool) < sizeof(SubClassWithMemberSubobjectNoAddress));
#endif /* def SUSUWU_CXX20 else */
}
static const int noExcept() SUSUWU_NOEXCEPT(true);
SUSUWU_NORETURN static void noReturn();
static const int noExcept() SUSUWU_NOEXCEPT {return 0;}
static void noReturn() {exit(0);} /* NOLINT(concurrency-mt-unsafe): is unreachable code */
static constexpr /* TODO: SUSUWU_CONSTEXPR */ const int macrosTestImpl() SUSUWU_EXPECTS(true) SUSUWU_ENSURES(true) SUSUWU_NOEXCEPT {
	return 0;
}
const int macrosTestsNoexcept() SUSUWU_NOEXCEPT {
	SUSUWU_STATIC_ASSERT(0 == macrosTestImpl());
	SUSUWU_ASSUME(true);
	noExcept();
	if(false) { /* NOLINT(readability-simplify-boolean-expr) */
		SUSUWU_UNREACHABLE;
		noReturn();
	}
	return 0;
}

}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_Macros_cxx */

