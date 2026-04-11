/* Attribution (henceforth "*this attribution*", whose syntax is *Markdown*): 2024 [Swudu Susuwu](https://swudususuwu.substack.com)
 * <https://github.com/SwuduSusuwu/SusuLib/> has the newest version of `./cxx/ClassNumeral.hxx` (henceforth "*this source code*").
 * If *this attribution* is shown, *this source code* allows all uses. *This attribution* constitutes the most permissive which is compatible with [*GPLv2*](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) + [*Apache 2*](https://www.apache.org/licenses/LICENSE-2.0.html), which is suitable for personal use (also suitable for school use).
 * If *this attribution* is not professional enough for business use: businesses can use *this source code* through included versions of [*GPLv2*](./LICENSE_GPLv2), [*Apache 2*](./LICENSE), or through both of those. */
#pragma once
#ifndef INCLUDES_cxx_ClassNumeral_hxx
#define INCLUDES_cxx_ClassNumeral_hxx
#include "ClassObject.hxx" /* Object SUSUWU_PURE_VIRTUAL_DEFAULTS() */
#include "Macros.hxx" /* SUSUWU_IF_CPLUSPLUS */
#include <algorithm> /* std::max */
#include SUSUWU_IF_CPLUSPLUS(<cmath>, <math.h>) /* std::abs */
#include SUSUWU_IF_CPLUSPLUS(<cstddef>, <stddef.h>) /* size_t */
#include <limits> /* std::numeric_limits */
#include <tuple> /* std::get std::tuple */
#include <vector> /* std::vector */
namespace Susuwu {

/* Scalar functions of numerals. */
template<typename Numeral>
inline const Numeral numeralRamp(const Numeral num, const Numeral ramp = std::numeric_limits<Numeral>::epsilon()) {
	if(!(num >= ramp)) { /* the `!` form is required to catch `inf` and `nan` */
		return ramp; /* `ramp = epsilon` to prevent "division-by-error" errors */
	}
	return num;
}

/* Vector / tensor functions of numerals. */ /* TODO: containers as templates, to allow all containers which have random access (such as `tensorflow::Tensor`) */
typedef long double NumeralAccum;
template<typename Numeral>
const NumeralAccum numeralAverage(const std::vector<Numeral> &nums) {
	NumeralAccum total = 0;
	for(const auto &num : nums) {
		total += num; /* TODO: catch overflows and recompute with per-iteration division? */
	}
	return total / nums.size(); /* single division formula uses less CPU */
}
template<std::size_t tupleIndex, typename Numeral>
const NumeralAccum numeralAverage(const std::vector<std::tuple<Numeral, Numeral>> &nums) {
	NumeralAccum total = 0;
	for(const auto &num : nums) {
		total += std::get<tupleIndex>(num); /* TODO: catch overflows and recompute with per-iteration division? */
	}
	return total / nums.size();
}
template<typename Numeral>
const Numeral numeralMagnitude(const std::vector<Numeral> &nums) {
	Numeral found = 0;
	for(const auto &num : nums) {
		found = std::max(std::abs(num), found);
	}
	return numeralRamp(found);
}
template<std::size_t tupleIndex, typename Numeral>
const Numeral numeralMagnitude(const std::vector<std::tuple<Numeral, Numeral>> &nums) {
	Numeral found = 0;
	for(const auto &numTuple : nums) {
		const Numeral num = std::get<tupleIndex>(numTuple);
		found = std::max(std::abs(num), found);
	}
	return numeralRamp(found);
}
template<typename Numeral>
const Numeral numeralBalancedMagnitude(const std::vector<Numeral> &nums, const NumeralAccum average) {
	Numeral found = 0;
	for(const auto &num : nums) {
		const Numeral balancedNum = num - average;
		found = std::max(std::abs(balancedNum), found);
	}
	return numeralRamp(found);
}
template<std::size_t tupleIndex, typename Numeral>
const Numeral numeralBalancedMagnitude(const std::vector<std::tuple<Numeral, Numeral>> &nums, const NumeralAccum average) {
	Numeral found = 0;
	for(const auto &numTuple : nums) {
		const Numeral balancedNum = std::get<tupleIndex>(numTuple) - average;
		found = std::max(std::abs(balancedNum), found);
	}
	return numeralRamp(found);
}

/* Structures which hold values used for normalization-functions upon numerals */
/* NOLINTBEGIN(google-explicit-constructor,hicpp-explicit-conversions) */
typedef struct NumeralNormalizers : public Object {
	SUSUWU_VIRTUAL_DEFAULTS(Susuwu::NumeralNormalizers) /* `getName()`, `isPureVirtual()`, `operator==`()`, ... */
	NumeralNormalizers(const NumeralAccum avg = 0.0, const NumeralAccum mag = 1.0) : average(avg), magnitude(mag) {} /* NOLINT(bugprone-easily-swappable-parameters); overzealous heuristic */ /* cppcheck-suppress noExplicitConstructor */
	template<typename Numeral>
	NumeralNormalizers(const std::vector<Numeral> &nums) : average(numeralAverage(nums)), magnitude(numeralBalancedMagnitude(nums, average)) {} /* cppcheck-suppress noExplicitConstructor */
	template<std::size_t tupleIndex, typename Numeral>
	static const NumeralNormalizers fromTuple(const std::vector<std::tuple<Numeral, Numeral>> &nums) {
		const NumeralAccum avg = numeralAverage<tupleIndex>(nums);
		return NumeralNormalizers(avg, numeralBalancedMagnitude<tupleIndex>(nums, avg));
	}
	NumeralAccum average, magnitude;
} NumeralNormalizers;
typedef struct NumeralNormalizersReciprocal : public Object { /* is as `NumeralNormalizers`, except `magnitude` is replaced with `magnitudeReciprocal` */
	SUSUWU_VIRTUAL_DEFAULTS(Susuwu::NumeralNormalizersReciprocal) /* `getName()`, `isPureVirtual()`, `operator==`()`, ... */
	NumeralAccum average, magnitudeReciprocal;
	NumeralNormalizersReciprocal(const NumeralAccum avg = 0.0, const NumeralAccum magRecip = 1.0) : average(avg), magnitudeReciprocal(magRecip) {} /* NOLINT(bugprone-easily-swappable-parameters); overzealous heuristic */ /* cppcheck-suppress noExplicitConstructor */
	template<typename Numeral>
	NumeralNormalizersReciprocal(const std::vector<Numeral> &nums) : average(numeralAverage(nums)), magnitudeReciprocal(1 / numeralBalancedMagnitude(nums, average)) {} /* cppcheck-suppress noExplicitConstructor */
	template<std::size_t tupleIndex, typename Numeral>
	static const NumeralNormalizersReciprocal fromTuple(const std::vector<std::tuple<Numeral, Numeral>> &nums) {
		const NumeralAccum avg = numeralAverage<tupleIndex>(nums);
		return NumeralNormalizersReciprocal(avg, 1 / numeralBalancedMagnitude<tupleIndex>(nums, avg));
	}
	NumeralNormalizersReciprocal(const NumeralNormalizers &normalizers) : average(normalizers.average), magnitudeReciprocal(1 / normalizers.magnitude) {} /* cppcheck-suppress noExplicitConstructor */
	operator NumeralNormalizers() const { return { average, 1 / magnitudeReciprocal }; }
} NumeralNormalizersReciprocal;
/* NOLINTEND(google-explicit-constructor,hicpp-explicit-conversions) */

/* Normalization functions which map vectors / tensors to `[-1, 1]` to suit all activation functions (which include transcendentals such as hyperbolic `tan`) */
template<typename Numeral>
inline const Numeral numeralNormalization(const Numeral num, const NumeralNormalizersReciprocal &normalizers) {
	return (num - normalizers.average) * normalizers.magnitudeReciprocal;
}
template<typename Numeral>
void numeralNormalization(std::vector<Numeral> &nums, const NumeralNormalizersReciprocal &normalizers) {
	for(auto &num : nums) {
		num = numeralNormalization(num, normalizers);
	}
}
template<typename Numeral>
const NumeralNormalizersReciprocal numeralNormalization(std::vector<Numeral> &nums) {
	const NumeralNormalizersReciprocal normalizers(nums);
	numeralNormalization(nums, normalizers);
	return normalizers;
}
#if SUSUWU_CLASSNUMERAL_REDUNDANT /* denormalization is fastest with `magnitudeReciprocal`; implicit conversion makes `magnitude` versions redundant */
template<typename Numeral>
inline const Numeral numeralNormalization(const Numeral num, const NumeralNormalizers &normalizers) {
	return (num - normalizers.average) / normalizers.magnitude;
}
template<typename Numeral>
void numeralNormalization(std::vector<Numeral> &nums, const NumeralNormalizers &normalizers) {
	numeralNormalization(nums, NumeralNormalizersReciprocal(normalizers));
}
#endif /* SUSUWU_CLASSNUMERAL_REDUNDANT */

/* Denormalization functions which map `[-1, 1]` back to original input values */
template<typename Numeral>
inline const Numeral numeralDenormalization(const Numeral num, const NumeralNormalizers &normalizers) {
	return (num * normalizers.magnitude) + normalizers.average;
}
template<typename Numeral>
void numeralDenormalization(std::vector<Numeral> &nums, const NumeralNormalizers &normalizers) {
	for(auto &num : nums) {
		num = numeralDenormalization(num, normalizers);
	}
}
template<typename Numeral>
const NumeralNormalizers numeralDenormalization(std::vector<Numeral> &nums) {
	const NumeralNormalizers normalizers(nums);
	numeralDenormalization(nums, normalizers);
	return normalizers;
}
#if SUSUWU_CLASSNUMERAL_REDUNDANT /* denormalization is fastest with `magnitude`; implicit conversion makes `magnitudeReciprocal` versions redundant */
template<typename Numeral>
inline const Numeral numeralDenormalization(const Numeral num, const NumeralNormalizersReciprocal &normalizers) {
	return (num / normalizers.magnitudeReciprocal) + normalizers.average;
}
template<typename Numeral>
void numeralDenormalization(std::vector<Numeral> &nums, const NumeralNormalizersReciprocal &normalizers) {
	numeralDenormalization(nums, NumeralNormalizers(normalizers));
}
#endif /* SUSUWU_CLASSNUMERAL_REDUNDANT */

}; /* namespace Susuwu */
#endif /* ndef INCLUDES_cxx_ClassNumeral_hxx */

