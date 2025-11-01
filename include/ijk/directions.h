#pragma once

#include "type_help.h"

#include <concepts>
#include <compare>

namespace ijk {

	template<std::floating_point T, typename dir>
	class directed_value
	{
		T val{ 0 };
	public:
		using direction = dir;
		using value_type = T;

		constexpr explicit directed_value(T v)
			: val(v)
		{
		}

		template<typename U>
		constexpr directed_value(directed_value<U, dir> const& u)
			: val(u.value())
		{
		}

		template<typename U>
		constexpr directed_value& operator=(directed_value<U, dir> const& u)
		{
			val = u.value();
			return *this;
		}

		template<typename U>
		constexpr directed_value(directed_value<U, dir>&& u)
			: val(u.value())
		{
		}

		template<typename U>
		constexpr directed_value& operator=(directed_value<U, dir>&& u)
		{
			val = u.value(); // not expecting any gains from moving arithmetic types
			return *this;
		}

		~directed_value() = default;

		auto operator<=>(directed_value const&) const = default;

		constexpr T value() const
		{
			return val;
		}

		template<typename U>
		constexpr directed_value& operator+=(directed_value<U, dir> u)
		{
			val += u.value();
			return *this;
		}

		template<typename U>
		constexpr directed_value& operator-=(directed_value<U, dir> u)
		{
			val -= u.value();
			return *this;
		}
	};


	template<typename T, typename U, typename direction>
	using common_dir = directed_value<std::common_type_t<T, U>, direction>;

	template<std::floating_point T, std::floating_point U, typename direction>
	constexpr common_dir<T, U, direction> operator+(directed_value<T, direction> const& LHS, directed_value<U, direction> const& RHS)
	{
		return common_dir<T, U, direction>(LHS.value() + RHS.value());
	}

	template<std::floating_point T, typename direction>
	constexpr directed_value<T, direction> operator-(directed_value<T, direction> const& RHS)
	{
		return directed_value<T, direction>(-RHS.value());
	}

	template<std::floating_point T, std::floating_point U, typename direction>
	constexpr common_dir<T, U, direction> operator-(directed_value<T, direction> const& LHS, directed_value<U, direction> const& RHS)
	{
		return common_dir<T, U, direction>(LHS.value() - RHS.value());
	}


#define IJK_NAME_DIRECTION(dir, literal_suffix) \
struct dir##_dir{}; \
template<typename T> \
using dir = directed_value<T, dir##_dir>; \
namespace literals {\
	consteval dir<double> operator""_##literal_suffix(long double value) { return dir<double>(static_cast<double>(value)); } \
	consteval dir<double> operator""_##literal_suffix(unsigned long long value) { return dir<double>(static_cast<double>(value)); }\
	consteval dir<long double> operator""_##literal_suffix##l(long double value) { return dir<double>(static_cast<long double>(value)); } \
	consteval dir<long double> operator""_##literal_suffix##l(unsigned long long value) { return dir<double>(static_cast<long double>(value)); }\
	consteval dir<float> operator""_##literal_suffix##f(long double value) { return dir<double>(static_cast<float>(value)); } \
	consteval dir<float> operator""_##literal_suffix##f(unsigned long long value) { return dir<double>(static_cast<float>(value)); }\
}\
namespace detail {\
	template<typename T> \
	concept is_##dir = has_direction<T> && std::same_as<typename T::direction, dir##_dir>; \
}\
template<typename stream_t, typename T>\
stream_t& operator<<(stream_t& os, directed_value<T, dir##_dir> directed)\
{return os << directed.value() << #literal_suffix; }

	IJK_NAME_DIRECTION(I, i)
	IJK_NAME_DIRECTION(J, j)
	IJK_NAME_DIRECTION(K, k)

		// Now applying quaternion identities
		// ii == jj == kk == ijk == -1 (w in this case)
		// These lead to
		// i = jk = -kj
		// j = -ik = ki
		// k = ij = -ji 

		// ii, jj, kk covered by requiring same direction tag for both arguments
	template<typename T, typename U, typename direction>
	constexpr auto operator*(directed_value<T, direction> const& LHS, directed_value<U, direction> const& RHS)
	{
		return -LHS.value() * RHS.value();
	}

#define IJK_IMPLEMENT_DIRECTION_PRODUCT(LHS_T, RHS_T, RESULT_T, RESULT_SIGN) \
template<typename T, typename U> \
constexpr auto operator*(LHS_T<T> const& LHS, RHS_T<U> const& RHS) \
{ \
	return RESULT_T(RESULT_SIGN LHS.value() * RHS.value()); \
}

	IJK_IMPLEMENT_DIRECTION_PRODUCT(J, K, I, +) // jk = i
	IJK_IMPLEMENT_DIRECTION_PRODUCT(K, J, I, -) // kj = -i
	IJK_IMPLEMENT_DIRECTION_PRODUCT(I, K, J, -) // ik = -j
	IJK_IMPLEMENT_DIRECTION_PRODUCT(K, I, J, +) // ki = j
	IJK_IMPLEMENT_DIRECTION_PRODUCT(I, J, K, +) // ij = k
	IJK_IMPLEMENT_DIRECTION_PRODUCT(J, I, K, -) // ji = -k

	template<std::floating_point T, std::floating_point U, typename direction>
	constexpr auto operator*(T const& LHS, directed_value<U, direction> const& RHS)
	{
		return directed_value<std::common_type_t<T, U>, direction>(LHS * RHS.value());
	}

	template<std::floating_point T, std::floating_point U, typename direction>
	constexpr auto operator*(directed_value<U, direction> const& LHS, T const& RHS)
	{
		return directed_value<std::common_type_t<T, U>, direction>(RHS * LHS.value());
	}

} // namespace ijk