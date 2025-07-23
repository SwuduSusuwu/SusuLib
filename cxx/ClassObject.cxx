/* (C) 2024 Swudu Susuwu, dual licenses: choose [GPLv2](./LICENSE_GPLv2) or [Apache 2](./LICENSE), allows all uses. */
#ifndef INCLUDES_cxx_ClassObject_cxx
#define INCLUDES_cxx_ClassObject_cxx
#include "Macros.hxx" /* SUSUWU_ERROR SUSUWU_IF_CPLUSPLUS SUSUWU_PURE_VIRTUAL_DEFAULTS SUSUWU_VIRTUAL_DEFAULTS SUSUWU_UNIT_TESTS */
#if SUSUWU_UNIT_TESTS
#include "ClassIo.hxx" /* gsl::owner */
#include "ClassObject.hxx" /* Class Object SUSUWU_CLASS_DEFAULTS SUSUWU_VIRTUAL_OPERATORS_USE_ADDRESSES SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS SUSUWU_VIRTUAL_DEFAULTS */
#include "ClassSys.hxx" /* templateCatchAll */
//#include <memory> /* std::unique_ptr std::make_unique */
#include SUSUWU_IF_CPLUSPLUS(<cstddef>, <stddef.h>) /* size_t */
//#include <stdexcept> /* std::runtime_error */
#include <string> /* std::string */
namespace Susuwu {
namespace { /* [misc-use-anonymous-namespace] */
static const bool classIsValid(const Class *clas) {
	bool result = true;
	if(!clas->isInstance(*clas)) {
		SUSUWU_ERROR("classIsValid() { if(!clas->isInstance(*clas)) { /* `" + clas->getName() + "::isInstance` false negative. */ } }");
		result = false;
	}
	if(clas->isPureVirtual()) {
		SUSUWU_ERROR("classIsValid() { if(clas->isPureVirtual()) { /* `" + clas->getName() + "::isPureVirtual` false positive. */ } }");
		result = false;
	}
	if(!clas->isInitialized()) {
		SUSUWU_ERROR("classIsValid() { if(!clas->isInitialized()) { /* `" + clas->getName() + "::isInitialized` false negative. */ } }");
		result = false;
	}
	return result;
}
static const bool classTestsCommutative(const Class *class1, const Class *class2) { /* TODO: call this "Symmetric"? */
	bool result = classIsValid(class1) && classIsValid(class2);
	if(class1->operator!=(*class2) != class2->operator!=(*class1)) {
		SUSUWU_ERROR("classTestsCommutative() { if(class1->operator!=(*class2) != class2->operator!=(*class1)) { /* `" + class1->getName() + "::operator!=` not commutative to `" + class2->getName() + "::operator!=`. */ } }");
		result = false;
	}
	if(class1->operator==(*class2) != class2->operator==(*class1)) {
		SUSUWU_ERROR("classTestsCommutative() { if(class1->operator==(*class2) != class2->operator==(*class1)) { /* `" + class1->getName() + "::operator==` not commutative to `" + class2->getName() + "::operator==`. */ } }");
		result = false;
	}
	if(class1->operator==(*class2) == class1->operator!=(*class2)) {
		SUSUWU_ERROR("classTestsCommutative() { if(class1->operator==(*class2/*" + class2->getName() + "*/ == class1->operator!=(*class2)) { /* `" + class1->getName() + "::operator==` not anticommuttative to `" + class1->getName() + "::operator!=`. */ } }");
		result = false;
	}
	if(class1->isRelatedTo(*class2) != class2->isRelatedTo(*class1)) {
		SUSUWU_ERROR("classTestsCommutative() { if(class1->isRelatedTo(*class2) != class2->isRelatedTo(*class1)) { /* `" + class1->getName() + "::isRelatedTo` not commutative to `" + class2->getName() + "::isRelatedTo`. */ } }");
		result = false;
	}
	if(instanceof(*class2, *class1) != class1->isInstance(*class2)) {
		SUSUWU_ERROR("classTestsCommutative() { if(instanceof(*class2, *class1) != class1->isInstance(*class2))) { /* `instanceof` has logic error. */ } }");
		result = false;
	}
	if(class1->isRelatedTo(*class2) != (class1->isInstance(*class2) || class2->isInstance(*class1))) {
		SUSUWU_ERROR("classTestsCommutative() { if(class1->isRelatedTo(*class2) != (class1->isInstance(*class2) || class2->isInstance(*class1))) { /* `" + class1->getName() + "::isRelatedTo` has logic error. */ } }");
		result = false;
	}
	return result;
}
static const bool classTestsMatch(const Class *class1, const Class *class2) {
	bool result = classTestsCommutative(class1, class2);
	if(class1->operator!=(*class2)) {
		SUSUWU_ERROR("classTestsMatch() { if(class1->operator!=(*class2 /*" + class2->getName() + "*/)) { /* `" + class1->getName() + "::operator!=` false negative. */ } }");
		result = false;
	}
	if(!class1->isRelatedTo(*class2)) {
		SUSUWU_ERROR("classTestsMatch() { if(!class1->isRelatedTo(*class2)) { /* `" + class1->getName() + "::isRelatedTo` false negative. */ } }");
		result = false;
	}
	if(class1->getObjectSize() != class2->getObjectSize()) {
		SUSUWU_ERROR("classTestsMatch() { if(class1->getObjectSize() != class2->getObjectSize())) { /* `" + class1->getName() + "::getObjectSize()` not commutative to `" + class2->getName() + "::getObjectSize()`. */ } }");
		result = false;
	}
	return result;
}
static const bool classTestsMismatch(const Class *class1, const Class *class2) {
	bool result = classTestsCommutative(class1, class2);
	if(class1->operator==(*class2)) {
		SUSUWU_ERROR("classTestsMismatch() { if(class1->operator==(*class2 /*" + class2->getName() + "*/)) { /* `" + class1->getName() + "::operator==` false positive. */ } }");
		result = false;
	}
#if !SUSUWU_VIRTUAL_OPERATORS_USE_ADDRESSES /* If you interpret `Java`'s standard as "Addresses must match" for `operator==`. */
	if(class1->getName() == class2->getName()) {
		SUSUWU_ERROR(std::string("classTestsMismatch() { if(class1->getName() == class2->getName()) { /* `") + typeid(class1).name() + "::getName() == \"" + class1->getName() + "\"`, but also `" + typeid(class2).name() + "::getName() == \"" + class2->getName() + "\"` */ } }");
		result = false;
	}
	if(class1->isInstance(*class2) && class2->isInstance(*class1)) {
		SUSUWU_ERROR("classTestsMismatch() { if(class1->isInstance(*class2) && class2->isInstance(*class1)) { /* `" + class1->getName() + "::isInstance` false positive, or `" + class2->getName() + "::isInstance` false positive. */ } }");
		result = false;
	}
#endif /* SUSUWU_VIRTUAL_OPERATORS_USE_ADDRESSES */
	return result;
}
static const bool objectIsValid(const Object *obj) {
	bool result = classIsValid(obj);
	gsl::owner<class Object *> objectClone = obj->clone();
	result &=	classTestsMatch(objectClone, obj) /* reflexive */;
	delete objectClone;
	return result;
}
static const bool objectTestsCommutative(const Object *object1, const Object *object2) {
	bool result = objectIsValid(object1) && objectIsValid(object2); //classTestsCommutative(object1, object2); /* redundant, due to `result = classTestsM*`. */
	if(object1->equals(*object2) != object2->equals(*object1)) {
		SUSUWU_ERROR("objectTestsCommutative() { if(object1->equals(*object2) != object2->equals(*object1)) { /* `" + object1->getName() + "::equals()` is not commutative to `" + object2->getName() + "::equals()`. */ } }");
		result = false;
	}
	if((object1->hashCode() == object2->hashCode()) != (object1->toString() == object2->toString())) {
		SUSUWU_ERROR("objectTestsCommutative() { if((object1->hashCode() == object2->hashCode()) != (object1->toString() == object2->toString())) { /* Impossible with default `hashCode()` (which is address comparison) */ } }");
		result = false;
	}
	return result;
}
static const bool objectTestsMatch(const Object *object1, const Object *object2) {
	bool result = objectTestsCommutative(object1, object2);
	result &= classTestsMatch(object1, object2);
	if(!object1->equals(*object2)) {
		SUSUWU_ERROR("objectTestsMatch() { if(!object1->equals(*object2 /*" + object2->getName() + "*/)) { /* `" + object1->getName() + "::equals` false negative. */ } }");
		result = false;
	}
	return result;
}
static const bool objectTestsMismatch(const Object *object1, const Object *object2) {
	bool result = objectTestsCommutative(object1, object2);
#if !SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES /* Interprets `Java`'s standard as "Addresses must match" for `equals`. */
	result &= classTestsMismatch(object1, object2);
#endif /* SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES else */
	if(object1->equals(*object2)) {
		SUSUWU_ERROR("objectTestsMismatch() { if(object1->equals(*object2 /*" + object2->getName() + "*/)) { /* `" + object1->getName() + "::equals` false positive. */ } }");
		result = false;
	}
	if(object1->hashCode() == object2->hashCode()) {
		SUSUWU_ERROR("objectTestsMismatch() { if(object1->hashCode() == object2->hashCode()) { /* `" + object1->getName() + "::hashCode()` not unique. */ } }");
		result = false;
	}
	if(object1->toString() == object2->toString()) {
		SUSUWU_ERROR("objectTestsMismatch() { if(object1->toString() == object2->toString()) { /* `" + object1->getName() + "::toString()` not unique. */ } }");
		result = false;
	}
	return result;
}
class SubClass : public Class { SUSUWU_CLASS_DEFAULTS(SubClass) };
class SubClassWithMemberObject : public Class { public: SUSUWU_CLASS_DEFAULTS(SubClassWithMemberObject) bool memberObject = true; };
class SubObject : public Object { SUSUWU_VIRTUAL_DEFAULTS(SubObject) };
class SubObjectWithMemberObject : public Object { public: SUSUWU_VIRTUAL_DEFAULTS(SubObjectWithMemberObject) bool memberObject = true; };
}; /* namespace */
const bool classObjectTests() {
	bool result = true;
	gsl::owner<const Class *> newClass = new Class(),
		newSubClass = new SubClass(),
		newSubClassWithMemberObject = new SubClassWithMemberObject();
	gsl::owner<const Object *> newObject = new Object(),
		newSubObject = new SubObject(),
		newSubObjectWithMemberObject = new SubObjectWithMemberObject();
	const Class class2 = Class();
	const Object object2 = Object();

	result &=	classTestsMatch(newClass, newClass) /* reflexive */;
	result &=	classTestsMatch(newSubClass, newSubClass) /* reflexive */;
	result &=	classTestsMatch(newSubClassWithMemberObject, newSubClassWithMemberObject) /* reflexive */;
	result &=	classTestsMatch(newObject, newObject) /* reflexive */;
	result &=	classTestsMatch(newSubObject, newSubObject) /* reflexive */;
	result &=	classTestsMatch(newSubObjectWithMemberObject, newSubObjectWithMemberObject) /* reflexive */;
	result &=	classTestsMismatch(newClass, newSubObjectWithMemberObject) /* `getObjectSize()` mismatch */;
	result &=	classTestsMismatch(newClass, newSubClassWithMemberObject) /* `getObjectSize()` mismatch */;
	result &=	classTestsMismatch(newSubClass, newSubObjectWithMemberObject) /* `getObjectSize()` mismatch */;
	result &=	classTestsMismatch(newSubClass, newSubClassWithMemberObject) /* `getObjectSize()` mismatch */;
	result &=	classTestsMismatch(newObject, newSubClassWithMemberObject) /* `getObjectSize()` mismatch */;
	result &=	classTestsMismatch(newObject, newSubObjectWithMemberObject) /* `getObjectSize()` mismatch */;
	result &=	classTestsMismatch(newSubObject, newSubClassWithMemberObject) /* `getObjectSize()` mismatch */;
	result &=	classTestsMismatch(newSubObject, newSubObjectWithMemberObject) /* `getObjectSize()` mismatch */;
#if SUSUWU_VIRTUAL_OPERATORS_USE_ADDRESSES /* If you interpret `Java`'s standard as "Addresses must match" for `operator==`. */
	SUSUWU_PRAGMA(message("TODO: `SUSUWU_VIRTUAL_OPERATORS_USE_ADDRESSES`. [Ask if you want this](https://github.com/SwuduSusuwu/SusuLib/issues/10 if you want this.)"))
	result &=	classTestsMismatch(newClass, &class2);
	result &=	classTestsMismatch(newObject, &object2);
#else /* SUSUWU_VIRTUAL_OPERATORS_USE_ADDRESSES else */
	result &=	classTestsMatch(newClass, &class2) /* reflexive */;
	result &=	classTestsMatch(newObject, &object2) /* reflexive */;
#endif /* SUSUWU_VIRTUAL_OPERATORS_USE_ADDRESSES else */
#if SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS /* If you interpret `Java`'s standard as "`typeid` must match". */
	result &=	classTestsMismatch(newClass, newSubClass);
	result &=	classTestsMismatch(newClass, newObject);
	result &=	classTestsMismatch(newClass, newSubObject);
	result &=	classTestsMismatch(newClass, &object2);
	result &=	classTestsMismatch(newObject, newSubObject);
	result &=	classTestsMismatch(newObject, &class2);
#else /* SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS else */
	SUSUWU_PRAGMA(message("Warning: `-DSUSUWU_VIRTUAL_OPERATORS_USE_VPTRS=false` is experimental"))
	result &=	classTestsMatch(newClass, newSubClass) /* transitive */;
	result &=	classTestsMatch(newClass, newObject) /* transitive */;
	result &=	classTestsMatch(newClass, newSubObject) /* transitive */;
	result &=	classTestsMatch(newClass, &object2) /* transitive */;
	result &=	classTestsMatch(newObject, &class2) /* transitive */;
	result &=	classTestsMatch(newObject, newSubObject) /* transitive */;
#endif /* SUSUWU_VIRTUAL_OPERATORS_USE_VPTRS else */
#if SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES /* Interprets `Java`'s standard as "Addresses must match" for `equals`. */
	result &=	objectTestsMismatch(newObject, &object2);
#else /* SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES else */
	SUSUWU_PRAGMA(message("Notice: used `-DSUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES=false`."))
	result &=	objectTestsMatch(newObject, &object2) /* reflexive */;
#endif /* SUSUWU_VIRTUAL_EQUALS_USE_ADDRESSES else */
	result &=	objectTestsMatch(newObject, newObject);
	result &=	objectTestsMismatch(newObject, newSubObjectWithMemberObject);
	if(newClass->getName() != class2.getName()) {
		SUSUWU_ERROR("classObjectTests() { if(newClass->getName() != class2.getName()) { /* `Class::getName()` virtual error */ } }");
		result = false;
	}
	if(newObject->getName() != object2.getName()) {
		SUSUWU_ERROR("classObjectTests() { if(newObject->getName() != object2.getName()) { /* `Object::getName()` virtual error */ } }");
		result = false;
	}

	delete newSubObjectWithMemberObject;
	delete newSubClassWithMemberObject;
	delete newSubObject;
	delete newSubClass;
	delete newObject;
	delete newClass;
	return result;
}
const bool classObjectTestsNoexcept() SUSUWU_NOEXCEPT { return templateCatchAll(classObjectTests, "classObjectTests()"); } /* cppcheck-suppress throwInNoexceptFunction */

}; /* namespace Susuwu */
#endif /* SUSUWU_UNIT_TESTS */
#endif /* ndef INCLUDES_cxx_ClassObject_cxx */

