/* Dual licenses: choose "Creative Commons" or "Apache 2" (allows all uses) */
#ifndef INCLUDES_cxx_ClassObject_hxx
#define INCLUDES_cxx_ClassObject_hxx
#include "Macros.hxx" /* SUSUWU_CXX20 SUSUWU_DEFAULT SUSUWU_INLINE SUSUWU_OVERRIDE SUSUWU_NULLPTR */
#include <cassert> /* assert */
#include <cstddef> /* size_t */
#include <cstring> /* memcmp memcpy */
#include <ios> /* std::hex */
#include <sstream> /* std::stringstream */
#include <stdexcept> /* std::runtime_error */
#include <string> /* std::string */
/* Gives: `Susuwu::Class` (a C++ port of [`java.lang.Class`](https://docs.oracle.com/javase/8/docs/api/java/lang/class.html)),
 * plus `Susuwu::Object` (a C++ port of [Java's `Object`](https://docs.oracle.com/javase/8/docs/api/java/lang/Object.html) [superclass](https://docs.oracle.com/javase%2Ftutorial%2F/java/IandI/objectclass.html)),
* to [assist future Java ports](https://github.com/SwuduSusuwu/SubStack/issues/10) */
/* Susuwu::Instrumentation` is somewhat analogous to [`java.lang.instrument.Instrumentation` interface](https://docs.oracle.com/javase/8/docs/api/java/lang/instrument/Instrumentation.html). */
namespace Susuwu {
const bool classObjectTests();
const bool classObjectTestsNoexcept() SUSUWU_NOEXCEPT;

typedef class Instrumentation { /* Produced this unaware of `Instrumentation`. TODO: match `Instrumentation` protocols (as `getObjectSize()` does). For now, this is just whatever run-time type information/reflection which does not map to `java.lang.Class`. */
public:
	virtual ~Instrumentation() SUSUWU_DEFAULT /* allows subclasses to release resources */
	virtual const size_t getDynamicSize() const { return 0; /* sum({`S` for `malloc(S)`, `sizeof(S)` for `new S`}) */ }
	virtual const size_t getObjectSize() const { return sizeof(*this); /* sum(sizeof({vptr, data members})) */ }
	virtual const bool hasExtraVirtuals() const { /* if(has `virtual`s other than `Object`'s) { return true; }*/ return false; }
	virtual const bool isPureVirtual() const { return false; } /* in case the compiler does not recognize such classes as "pure virtual", put `return true;` for classes which do not implement all virtuals (or which have broken implementations of virtuals) */
	virtual const bool isInitialized() const { return true; } /* override this if the constructor does not produce objects ready to use */
	virtual const bool isPlainOldData() const { return !(usesDynamicAllocations() || usesFilesystem() || usesNetwork() || usesThreads()); } /* whether or not to allow shallow clones */
	virtual const bool usesDynamicAllocations() const { /* if(uses functions suchas {`malloc`, `new`}) { return true; } */ return false; }
	virtual const bool usesFilesystem() const { /* if(uses functions suchas {`fopen`, `new`}) { return true; } */ return false; }
	virtual const bool usesNetwork() const { /* if(uses functions suchas {`socket`}) { return true; } */ return false; }
	virtual const bool usesThreads() const { /* if(uses functions suchas {`pthread`}) { return true; } */ return false; }
} Instrumentation;

#ifndef SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES /* override with `-DSUSUWU_VIRTUAL_EQUALS_USE_ADDRESES=false` */
# define SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES true /* Default is: Interpret `Java`'s `Object::equals` standard as "Addresses must match". */
#endif /* ndef SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES */
#define SUSUWU_VIRTUAL_OPERATORS_USE_ADDRESSES false /* TODO: inherit from `SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES`? */
#define SUSUWU_VPTR_SZ sizeof(SUSUWU_NULLPTR) /* TODO: use documented constant for `vptr` byte count. */
#define SUSUWU_VPTR_OFFSET SUSUWU_VPTR_SZ /* Notice: assumes that first data member is `vptr`. */
#ifndef SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS /* override with `-DSUSUWU_VIRTUAL_VPTR_COMPARISON=false` to use `hasLayoutOf()` && data members comparison */
# define SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS true /* Default: `Class::operator==` does `typeid` && data members comparison */
#endif /* ndef SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS */
typedef class Class : public Instrumentation { /* Port of `java.lang.Class`. */ /* NOLINT(cppcoreguidelines-special-member-functions, hicpp-special-member-functions): suppress `clang-tidy`'s suggestions of constructors. */
public:
	~Class() SUSUWU_OVERRIDE SUSUWU_DEFAULT /* allows subclasses to release resources */
	virtual const std::string getName() const { return "Susuwu::Class"; } /* returns as value so subclasses can return dynamic values */
	virtual const Class *getSuperclass() const { return SUSUWU_NULLPTR; /* no base for root class */ }
	SUSUWU_INLINE const Class &getSuperclassRef() const { const auto *superclass = getSuperclass(); return SUSUWU_NULLPTR != superclass ? *superclass : *this; /* for promises not to `return nullptr`, use references */ }
	SUSUWU_INLINE const bool hasLayoutOf(const Class &obj) const { return this->isRelatedTo(obj) && this->getObjectSize() == obj.getObjectSize(); }
	SUSUWU_INLINE const bool isAssignableFrom(const Class &obj) const { return this->isInstance(obj); } /* TODO: template to test types without obj */
	virtual const bool isInstance(const Class &obj) const { return SUSUWU_NULLPTR != dynamic_cast<const Class *>(&obj); } /* port of Java's */ /* NOLINT(readability-redundant-casting) */
	SUSUWU_INLINE const bool isRelatedTo(const Class &obj) const { return (this->isInstance(obj) || obj.isInstance(*this)); }
/* `clang-tidy` off: NOLINTBEGIN(fuchsia-overloaded-operator, google-runtime-int) */
#ifdef SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS
	virtual /* const requires C++26 */ bool operator==(const Class &obj) const { return (sizeof(*this) == sizeof(obj) && 0 == memcmp(reinterpret_cast<const void *>(this), reinterpret_cast<const void *>(&obj), sizeof(*this))); } /* warning: first operand of this 'memcmp' call is a pointer to dynamic class 'Object'; vtable pointer will be compared [-Wdynamic-class-memaccess] */
#else /* !SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS */
	virtual /* const requires C++26 */ bool operator==(const Class &obj) const { return this->hasLayoutOf(obj && 0 == memcmp(&reinterpret_cast<const char *>(this)[SUSUWU_VPTR_OFFSET], &reinterpret_cast<const char *>(&obj)[SUSUWU_VPTR_OFFSET], sizeof(*this) - SUSUWU_VPTR_SZ)); }
#endif /* !SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS */
	virtual bool operator!=(const Class &obj) const SUSUWU_FINAL { return !this->operator==(obj); } /* do not override this, but `operator==(const Class &)` */
} Class;
SUSUWU_INLINE const bool instanceof(const Class &obj, const Class &thiso) { return thiso.isInstance(obj); } /* Just as Java's `instanceof`, has operands reversed compared to `isInstance`. */

typedef enum ObjectCloneAs : unsigned char { /* Is extra (not part of Java); accomodates the numerous types of C++ clones. */
	objectCloneAsNone           = 0, /* `!isCloneabble()` */
	objectCloneAsDeep           = 1, /* Recursive `new` */
	objectCloneAsShallow        = 2, /* `memcpy` */
	objectCloneAsReferenceCount = 4, /* `std::shared_ptr` */ /* TODO: `objectCloneAsReferenceCount` with const `clone` */
	objectCloneAsCoW            = 8  /* [Copy-on-Write](https://wikipedia.org/wiki/Copy-on-write)  */
} ObjectCloneAs;
typedef class Object : public Class { /* Port of `java.lang.Object`. */
public:
	virtual const bool isCloneable() const { return objectCloneAsNone != cloneableAs(); }
	virtual const ObjectCloneAs cloneableAs() const { /* returns preferred/default clone mode */
		return isPlainOldData() ? objectCloneAsShallow : objectCloneAsNone; /* crude heuristic, override for complex classes */
	}
	virtual const bool isCloneableAs(ObjectCloneAs cloneAs) const { return cloneableAs() == cloneAs; /* must override if multiple cloneable types */ }
	virtual const Object stackClone() const {
		if(!isCloneableAs(objectCloneAsShallow)) { throw std::runtime_error("`" + getName() + "::stackClone()`: unsupported default use."); }
		return Object(*this);
	}
	virtual Object *clone() const {
//		return &(*(new Object) = stackClone());
		return cloneAs(cloneableAs());
	}
	virtual const Object stackCloneAs(ObjectCloneAs cloneAs) const {
		if(!isCloneableAs(objectCloneAsShallow)) { throw std::runtime_error("`" + getName() + "::stackCloneAs(" + std::to_string(cloneAs) + ")`: unsupported default use."); }
		return Object(*this);
	}
	virtual Object *cloneAs(ObjectCloneAs cloneAs) const {
//		return &(*(new Object) = stackCloneAs(cloneAs));
		if(!isCloneableAs(objectCloneAsShallow)) { throw std::runtime_error("`" + getName() + "::cloneAs(" + std::to_string(cloneAs) + ")`: unsupported default use."); }
		auto clone = ::operator new (getObjectSize()); /* NOLINT(cppcoreguidelines-owning-memory,llvm-qualified-auto,readability-qualified-auto) */
		memcpy(clone, static_cast<const void *>(this), getObjectSize());
		return static_cast<Object *>(clone);
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
	const std::string getName() const SUSUWU_OVERRIDE { return "Susuwu::Object"; }
	virtual const long /* `int` gives `error: cast from pointer to smaller type 'int' loses information` */ hashCode() const { return reinterpret_cast<long>(this); }
	const bool isInstance(const Class &obj) const SUSUWU_OVERRIDE { return SUSUWU_NULLPTR != dynamic_cast<const Object *>(&obj); } /* port of Java's */
/* `clang-tidy` on: NOLINTEND(fuchsia-overloaded-operator, google-runtime-int) */
	virtual const std::string toString() const { std::stringstream os; os << getName() << '@' << std::hex << hashCode(); return os.str(); }
	virtual void notify() {}
	virtual void notifyAll() {}
	virtual void wait() {}
} Object;

/* Is some slowdown to use inheritance+polymorphism with all classes;
 * https://stackoverflow.com/questions/8824587/what-is-the-purpose-of-the-final-keyword-in-c11-for-functions/78680754#78680754 shows howto use `final` (requires C++11) to fix this. If >=C++11, `SUSUWU_FINAL` is `final`, if <C++11, is no-op. */
}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassObject_hxx */

