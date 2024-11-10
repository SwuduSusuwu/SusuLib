/* Licenses: allows all uses ("Creative Commons"/"Apache 2") */
#ifndef INCLUDES_cxx_Macros_cxx
#define INCLUDES_cxx_Macros_cxx
/* This is just unit tests. `Macros.hxx` has all which has actual use. */
#include "Macros.hxx" /* IF_SUSUWU_CPLUSPLUS SUSUWU_ASSUME SUSUWU_CONSTEXPR SUSUWU_EXPECTS SUSUWU_ENSURES SUSUWU_NOEXCEPT SUSUWU_NORETURN SUSUWU_STATIC_ASSERT SUSUWU_UNREACHABLE */
#include IF_SUSUWU_CPLUSPLUS(<cstdlib>, <stdlib.h>) /* exit */
#ifdef SUSUWU_CXX11
#	include <type_traits> /* is_empty */
#endif /* def SUSUWU_CXX11 */
namespace Susuwu { /* NOLINTBEGIN(misc-use-anonymous-namespace): can't use functions if anonymouse */
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

SUSUWU_NORETURN static void macrosNoReturn();
SUSUWU_NORETURN static void macrosNoReturn() {exit(0);} /* NOLINT(concurrency-mt-unsafe): is unreachable code */
SUSUWU_CONSTEXPR static const bool macrosDeclarationAttributes() SUSUWU_EXPECTS(true) SUSUWU_ENSURES(true) SUSUWU_NOEXCEPT;
SUSUWU_CONSTEXPR static const bool macrosDeclarationAttributes() SUSUWU_EXPECTS(true) SUSUWU_ENSURES(true) SUSUWU_NOEXCEPT {
	return true;
} /* NOLINTEND(misc-use-anonymous-namespace): `clang-tidy` back on */
SUSUWU_STATIC_ASSERT(true); /* test at file-scope, which `assert()` does not support */
const int macrosTestsNoexcept() SUSUWU_NOEXCEPT {
	SUSUWU_STATIC_ASSERT(true); /* test at function-scope, which some custom static asserts can't do */
	SUSUWU_ASSUME(true);
	macrosDeclarationAttributes();
	if(false) { /* NOLINT(readability-simplify-boolean-expr) */
		SUSUWU_UNREACHABLE;
		macrosNoReturn();
	}
	return 0;
}

}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_Macros_cxx */

