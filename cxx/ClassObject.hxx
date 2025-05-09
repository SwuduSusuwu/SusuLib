/* Attribution (henceforth "*this attribution*", whose syntax is *Markdown*): 2024 [Swudu Susuwu](https://swudususuwu.substack.com)
 * <https://github.com/SwuduSusuwu/SusuLib/> has the newest version of `./cxx/ClassObject.hxx` (henceforth "*this source code*").
 * If *this attribution* is shown, *this source code* allows all uses. *This attribution* constitutes the most permissive which is compatible with [*GPLv2*](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) + [*Apache 2*](https://www.apache.org/licenses/LICENSE-2.0.html), which is suitable for personal use (also suitable for school use).
 * If *this attribution* is not professional enough for business use: businesses can use *this source code* through included versions of [*GPLv2*](./LICENSE_GPLv2), [*Apache 2*](./LICENSE), or through both of those. */
#pragma once
#ifndef INCLUDES_cxx_ClassObject_hxx
#define INCLUDES_cxx_ClassObject_hxx
#include "ClassIo.hxx" /* classIoHexStr gsl::owner */
#include "Macros.hxx" /* SUSUWU_C11 SUSUWU_CXX11 SUSUWU_CXX20 SUSUWU_DEFAULT SUSUWU_FINAL SUSUWU_IF_CPLUSPLUS SUSUWU_INLINE SUSUWU_INTPTR SUSUWU_NOEXCEPT SUSUWU_NULLPTR SUSUWU_OVERRIDE SUSUWU_UNIT_TESTS */
#include SUSUWU_IF_CPLUSPLUS(<cassert>, <assert.h>) /* assert */
#include SUSUWU_IF_CPLUSPLUS(<cstddef>, <stddef.h>) /* size_t */
#include SUSUWU_IF_CPLUSPLUS(<cstring>, <string.h>) /* memcmp memcpy */
#include <new> /* ::operator new */
#include <stdexcept> /* std::runtime_error */
#include <string> /* std::string */
/* Gives: `Susuwu::Class` (a C++ port of [`java.lang.Class`](https://docs.oracle.com/javase/8/docs/api/java/lang/Class.html)),
 * plus `Susuwu::Object` (a C++ port of [Java's `Object`](https://docs.oracle.com/javase/8/docs/api/java/lang/Object.html) [superclass](https://docs.oracle.com/javase%2Ftutorial%2F/java/IandI/objectclass.html)),
* to [assist future Java ports](https://github.com/SwuduSusuwu/SusuLib/issues/10) */
/* Susuwu::Instrumentation` is somewhat analogous to [`java.lang.instrument.Instrumentation` interface](https://docs.oracle.com/javase/8/docs/api/java/lang/instrument/Instrumentation.html). */
namespace Susuwu {
#if SUSUWU_UNIT_TESTS
const bool classObjectTests();
const bool classObjectTestsNoexcept() SUSUWU_NOEXCEPT;
#endif /* SUSUWU_UNIT_TESTS */

typedef class Instrumentation { /* Produced this unaware of `Instrumentation`. TODO: match `Instrumentation` protocols (as `getObjectSize()` does). For now, this is just whatever run-time type information/reflection which does not map to `java.lang.Class`. */
public:
	virtual ~Instrumentation() SUSUWU_DEFAULT /* allows subclasses to release resources */
	virtual const size_t getDynamicSize() const { return 0; /* sum({`S` for `malloc(S)`, `sizeof(S)` for `new S`}) */ }
	virtual const size_t getObjectSize() const { return sizeof(*this); /* sum(sizeof({vptr, data members})) */ }
	virtual const bool hasExtraVirtuals() const { /* if(has `virtual`s other than `Object`'s) { return true; }*/ return false; }
/* `clang-tidy` off: NOLINTBEGIN(cppcoreguidelines-macro-usage, fuchsia-overloaded-operator) */
#define SUSUWU_INSTRUMENTATION_ISPUREVIRTUAL(SUBCLASS) const bool isPureVirtual() const SUSUWU_OVERRIDE { return false; }
#define SUSUWU_INSTRUMENTATION_ISPUREVIRTUAL_PURE_VIRTUAL(SUBCLASS) const bool isPureVirtual() const SUSUWU_OVERRIDE { return typeid(SUBCLASS) == typeid(this); }
	virtual const bool isPureVirtual() const { return false; } /* in case the compiler does not recognize such classes as "pure virtual", use `SUSUWU_INSTRUMENTATION_ISPUREVIRTUAL_PURE_VIRTUAL(SubClass)` for `SubClass` which does not implement all virtuals (or which has broken implementations of virtuals) */
	virtual const bool isInitialized() const { return true; } /* override this if the constructor does not produce objects ready to use */
	virtual const bool isPlainOldData() const { return !(usesDynamicAllocations() || usesFilesystem() || usesNetwork() || usesThreads()); } /* whether or not to allow shallow clones */
	virtual const bool usesDynamicAllocations() const { /* if(uses functions suchas {`malloc`, `new`}) { return true; } */ return false; }
	virtual const bool usesFilesystem() const { /* if(uses functions suchas {`fopen`, `new`}) { return true; } */ return false; }
	virtual const bool usesNetwork() const { /* if(uses functions suchas {`socket`}) { return true; } */ return false; }
	virtual const bool usesThreads() const { /* if(uses functions suchas {`pthread`}) { return true; } */ return false; }
} Instrumentation;

#ifndef SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES /* override with `-DSUSUWU_VIRTUAL_EQUALS_USE_ADDRESES=false` */
#	define SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES true /* Default is: Interpret `Java`'s `Object::equals` standard as "Addresses must match". */
#endif /* ndef SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES */
#define SUSUWU_VIRTUAL_OPERATORS_USE_ADDRESSES false /* TODO: inherit from `SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES`? */
#define SUSUWU_VPTR_SZ sizeof(SUSUWU_NULLPTR) /* TODO: use documented constant for `vptr` byte count. */
#define SUSUWU_VPTR_OFFSET SUSUWU_VPTR_SZ /* Notice: assumes that first data member is `vptr`. */
#ifndef SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS /* override with `-DSUSUWU_VIRTUAL_VPTR_COMPARISON=false` to use `hasLayoutOf()` && data members comparison */
#	define SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS true /* Default: `Class::operator==` does `typeid` && data members comparison */
#endif /* ndef SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS */

#define SUSUWU_VIRTUAL_OPERATOREQUALTO_WITH_VPTR { return typeid(*this) == typeid(obj) && 0 == memcmp(reinterpret_cast<const void *>(this), reinterpret_cast<const void *>(&obj), this->getObjectSize()); } /* cast silences "warning: first operand of this 'memcmp' call is a pointer to dynamic class 'Object'; vtable pointer will be compared [-Wdynamic-class-memaccess]" */
#define SUSUWU_VIRTUAL_OPERATOREQUALTO_WITHOUT_VPTR { return (this->hasLayoutOf(obj) && 0 == memcmp(reinterpret_cast<const char *>(this) + SUSUWU_VPTR_OFFSET, reinterpret_cast<const char *>(&obj) + SUSUWU_VPTR_OFFSET, this->getObjectSize() - SUSUWU_VPTR_SZ)); } /* NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic) */

#ifdef SUSUWU_CXX20
#	define SUSUWU_VIRTUAL_OPERATOREQUALTO_DEFAULT SUSUWU_DEFAULT
#else /* !C++20 */
#	define SUSUWU_VIRTUAL_OPERATOREQUALTO_DEFAULT SUSUWU_VIRTUAL_OPERATOREQUALTO_WITH_VPTR
#endif /* !C++20 */
#define SUSUWU_SUBCLASS_OPERATOREQUALTO(SUBCLASS) bool operator==(const SUBCLASS &obj) const SUSUWU_VIRTUAL_OPERATOREQUALTO_DEFAULT /* Usage: `SUSUWU_SUBCLASS_OPERATOREQUALTO(SubClass);`, for all `SubClass : public Class` or `SubClass : public Object` which do not have their own `operator==(const SubClass &)` */

#define SUSUWU_VIRTUAL_ virtual /* Is used to mark base class functions `virtual`. */
#define SUSUWU_CLASS_OVERRIDE /* For base class functions is no-op. */
#if SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS
#	define SUSUWU_CLASS_OPERATOREQUALTO(SUBCLASS) SUSUWU_VIRTUAL_ bool operator==(const Class &obj) const SUSUWU_CLASS_OVERRIDE SUSUWU_VIRTUAL_OPERATOREQUALTO_WITH_VPTR
#else /* else !SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS */
#	define SUSUWU_CLASS_OPERATOREQUALTO(SUBCLASS) SUSUWU_VIRTUAL_ bool operator==(const Class &obj) const SUSUWU_CLASS_OVERRIDE SUSUWU_VIRTUAL_OPERATOREQUALTO_WITHOUT_VPTR
#endif /* else !SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS */
#define SUSUWU_CLASS_GETNAME(SUBCLASS) SUSUWU_VIRTUAL_ const std::string /* returns as value so subclasses can return dynamic values */ getName() const SUSUWU_CLASS_OVERRIDE { return #SUBCLASS; }
#define SUSUWU_CLASS_GETOBJECTSIZE(SUBCLASS) SUSUWU_VIRTUAL_ const size_t getObjectSize() const SUSUWU_OVERRIDE { return sizeof(SUBCLASS); } /* Run-time type information */
#define SUSUWU_CLASS_ISINSTANCE(SUBCLASS) SUSUWU_VIRTUAL_ const bool isInstance(const Class &obj) const SUSUWU_CLASS_OVERRIDE { auto ptr = dynamic_cast<const SUBCLASS *>(&obj); return SUSUWU_NULLPTR != ptr; } /* port of Java's */
#define SUSUWU_CLASS_DEFAULTS(SUBCLASS) /* Usage: `class SubClass : public Class { SUSUWU_CLASS_DEFAULTS(SubClass) };` */ \
	SUSUWU_CLASS_GETNAME(SUBCLASS) \
	SUSUWU_CLASS_GETOBJECTSIZE(SUBCLASS)\
	SUSUWU_CLASS_OPERATOREQUALTO(SUBCLASS) /* If you don't want it to override `operator==(const Class &)` for you; do `#define SUSUWU_CLASS_OPERATOREQUALTO(noop) ;` */\
	SUSUWU_CLASS_ISINSTANCE(SUBCLASS)
typedef class Class : public Instrumentation { /* Port of `java.lang.Class`. */ /* NOLINT(cppcoreguidelines-special-member-functions, hicpp-special-member-functions): suppress `clang-tidy`'s suggestions of constructors. */
public:
	~Class() SUSUWU_OVERRIDE SUSUWU_DEFAULT /* allows subclasses to release resources */
	SUSUWU_CLASS_DEFAULTS(Susuwu::Class)
	virtual const Class *getSuperclass() const { return SUSUWU_NULLPTR; /* no base for root class */ }
	SUSUWU_INLINE const Class &getSuperclassRef() const { const auto *superclass = getSuperclass(); return SUSUWU_NULLPTR != superclass ? *superclass : *this; /* for promises not to `return nullptr`, use references */ }
	SUSUWU_INLINE const bool hasLayoutOf(const Class &obj) const { return this->isRelatedTo(obj) && this->getObjectSize() == obj.getObjectSize(); }
	SUSUWU_INLINE const bool isAssignableFrom(const Class &obj) const { return this->isInstance(obj); } /* TODO: template to test types without obj */
	SUSUWU_INLINE const bool isRelatedTo(const Class &obj) const { return (this->isInstance(obj) || obj.isInstance(*this)); }
	virtual bool operator!=(const Class &obj) const SUSUWU_FINAL { return !this->operator==(obj); } /* do not override this, but `operator==(const Class &)` */
} Class;
SUSUWU_INLINE const bool instanceof(const Class &obj, const Class &thiso) { return thiso.isInstance(obj); } /* Just as Java's `instanceof`, has operands reversed compared to `isInstance`. */
#undef SUSUWU_CLASS_OVERRIDE                  /*     Was no-op for base functions, ... */
#define SUSUWU_CLASS_OVERRIDE SUSUWU_OVERRIDE /* ... but is `override` for subclasses. */

#define SUSUWU_OBJECT_OVERRIDE /* For base class functions is no-op. */
#define SUSUWU_VIRTUAL_DEFAULTS(SUBCLASS) /* Usage: `class SubClass : public Object { SUSUWU_VIRTUAL_DEFAULTS(SubClass) };` */ \
	SUSUWU_CLASS_DEFAULTS(SUBCLASS) /* If you don't want it to override `operator==(const Class &)` for you; do `#define SUSUWU_CLASS_OPERATOREQUALTO(Q) ;` */\
	SUSUWU_INSTRUMENTATION_ISPUREVIRTUAL(SUBCLASS)

#define SUSUWU_PURE_VIRTUAL_DEFAULTS(SUBCLASS) /* Usage: `class PureVirtualSubClass : public Object { SUSUWU_PURE_VIRTUAL_DEFAULTS(PureVirtualSubClass) };` */ \
	SUSUWU_CLASS_DEFAULTS(SUBCLASS) \
	SUSUWU_INSTRUMENTATION_ISPUREVIRTUAL_PURE_VIRTUAL(SUBCLASS)
typedef enum ObjectCloneAs : unsigned char { /* Is extra (not part of Java); accomodates the numerous types of C++ clones. */
	objectCloneAsNone           = 0, /* `!isCloneabble()` */
	objectCloneAsDeep           = 1, /* Recursive `new` */ /* [If overload `operator new`, set `-flifetime-dse=1](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#index-fno-lifetime-dse) */
	objectCloneAsShallow        = 2, /* `memcpy` */
	objectCloneAsReferenceCount = 4, /* `std::shared_ptr` */ /* TODO: `objectCloneAsReferenceCount` with const `clone` */
	objectCloneAsCoW            = 8  /* [Copy-on-Write](https://wikipedia.org/wiki/Copy-on-write)  */
} ObjectCloneAs;
typedef class Object : public Class { /* Port of `java.lang.Object`. */
public:
	SUSUWU_VIRTUAL_DEFAULTS(Susuwu::Object) /* use `SUSUWU_PURE_VIRTUAL_DEFAULTS(SubClass)` if `SubClass` is pure virtual */
/* `clang-tidy` on: NOLINTEND(cppcoreguidelines-macro-usage, fuchsia-overloaded-operator) */
	virtual const bool isCloneable() const { return objectCloneAsNone != cloneableAs(); }
	virtual const ObjectCloneAs cloneableAs() const { /* returns preferred/default clone mode */
		return isPlainOldData() ? objectCloneAsShallow : objectCloneAsNone; /* crude heuristic, override for complex classes */
	}
	virtual const bool isCloneableAs(ObjectCloneAs cloneAs) const { return cloneableAs() == cloneAs; /* must override if multiple cloneable types */ }
	virtual const Object stackClone() const {
		if(!isCloneableAs(objectCloneAsShallow)) { throw std::runtime_error("`" + getName() + "::stackClone()`: unsupported default use."); }
		return Object(*this);
	}
	virtual gsl::owner<Object *> clone() const {
//		return &(*(new Object) = stackClone());
		return cloneAs(cloneableAs());
	}
	virtual const Object stackCloneAs(ObjectCloneAs cloneAs) const {
		if(!isCloneableAs(objectCloneAsShallow)) { throw std::runtime_error("`" + getName() + "::stackCloneAs(" + std::to_string(cloneAs) + ")`: unsupported default use."); }
		return Object(*this);
	}
	virtual gsl::owner<Object *> cloneAs(ObjectCloneAs cloneAs) const {
//		return &(*(new Object) = stackCloneAs(cloneAs));
		if(!isCloneableAs(objectCloneAsShallow)) { throw std::runtime_error("`" + getName() + "::cloneAs(" + std::to_string(cloneAs) + ")`: unsupported default use."); }
		auto clonePtr = ::operator new(getObjectSize()); /* NOLINT(cppcoreguidelines-owning-memory) */
		memcpy(clonePtr, static_cast<const void *>(this), getObjectSize());
		return static_cast<Object *>(clonePtr);
	}
#if SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES /* If you interpret `Java`'s standard as "Addresses must match". */
	virtual const bool equals(const Object &obj) const { return this == &obj; } /* Java's contract requires you to override this version of `equals` */
#else /* SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES else */
	virtual const bool equals(const Object &obj) const { return this->getClass().operator==(obj); } /* if you want to override `equals`, just override `operator==(const Class *)`, which will do both */
#endif /* SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES else */
	virtual void finalize() {
		this->~Object();
	}
	const Class &getClass() const { return *this; }
	virtual const SUSUWU_INTPTR hashCode() const { return reinterpret_cast<SUSUWU_INTPTR>(this); }
	virtual const std::string toString() const {
		return getName() + '@' + classIoHexStr(hashCode()); /* TODO: if `SUSUWU_HEX_DOES_PREFIX`, remove "0x"? */
	}
	virtual void notify() {}
	virtual void notifyAll() {}
	virtual void wait() {}
} Object;
#undef SUSUWU_VIRTUAL_  /* Was used to mark base functions `virtual` ... */
#define SUSUWU_VIRTUAL_ /*            ... , but is no-op for subclasses. */
#undef SUSUWU_OBJECT_OVERRIDE                  /*     Was no-op for base functions, ... */
#define SUSUWU_OBJECT_OVERRIDE SUSUWU_OVERRIDE /* ... but is `override` for subclasses. */

/* Is some slowdown to use inheritance+polymorphism with all classes;
 * `--release` sets `-o2`, [`-o2` sets `-devirtualize`, which convert some polymorphic calls into constant calls](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#index-fdevirtualize)
 * [`-devirtualize` is improved if functions use `final` (which requires C++11)](https://stackoverflow.com/questions/8824587/what-is-the-purpose-of-the-final-keyword-in-c11-for-functions/78680754#78680754]. If >=C++11, `SUSUWU_FINAL` is `final`, if <C++11, is no-op. */
}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassObject_hxx */

