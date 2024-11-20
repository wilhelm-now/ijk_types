#pragma once

#include "directions.h"
#include "type_help.h"

#include <type_traits>



template<std::floating_point T>
struct quaternion
{
	T w{0};
	I<T> i{0};
	J<T> j{0};
	K<T> k{0};

public:

	auto operator<=>(quaternion const&) const = default;

	constexpr quaternion conjugate() const
	{
		return quaternion{ w, -i, -j, -k };
	}

	template<ijk::detail::direction_or_floating U>
	constexpr void assign_by_type(U const& u)
	{
		[&u](auto&&... args)
			{
				auto assign = [&u]<typename T>(T & dest)
				{
					if constexpr (ijk::detail::is_same_direction<T, U>)
					{
						dest = u;
					}
				};
				(assign(args), ...);
			}(w, i, j, k);
	}
};

template<typename stream_t, typename T>
stream_t& operator<<(stream_t& os, quaternion<T> const& q)
{
	return os << "{ " << q.w << ", " << q.i.value() << "i, " << q.j.value() << "j, " << q.k.value() << "k }";
}

// plus or minus of different directions implicitly creates a quaternion
// todo restrict to complex or vector

template<ijk::detail::direction_or_floating T, ijk::detail::direction_or_floating U>
requires (!ijk::detail::is_same_direction<T, U>)
auto operator+(T const& LHS, U const& RHS)
{
	using value_t = std::common_type_t<ijk::detail::value_type<T>, ijk::detail::value_type<U>>;
	quaternion<value_t> q{};
	q.assign_by_type(LHS);
	q.assign_by_type(RHS);
	return q;
}

template<ijk::detail::direction_or_floating T, ijk::detail::direction_or_floating U>
	requires (!ijk::detail::is_same_direction<T, U>)
auto operator-(T const& LHS, U const& RHS)
{
	using value_t = std::common_type_t<ijk::detail::value_type<T>, ijk::detail::value_type<U>>;
	quaternion<value_t> q{};
	q.assign_by_type(LHS);
	q.assign_by_type(-RHS);
	return q;
}

template<typename T, ijk::detail::direction_or_floating U>
constexpr auto operator+(quaternion<T> const& LHS, U const& RHS)
{
	using value_t = std::common_type_t<T, ijk::detail::value_type<U>>;
	quaternion<value_t> q{.w = LHS.w, .i = LHS.i, .j = LHS.j, .k = LHS.k};
	[&RHS](auto&&... args)
		{
			auto plus = [&RHS]<typename T>(T & lhs)
			{
				if constexpr (ijk::detail::is_same_direction<T, U>)
				{
					lhs = lhs + RHS;
				}
			};
			(plus(args), ...);
		}(q.w, q.i, q.j, q.k);
	return q;
}

template<ijk::detail::direction_or_floating U, typename T>
constexpr auto operator+(U const& LHS, quaternion<T> const& RHS)
{
	return RHS + LHS; // commutative addition
}

template<typename T, typename U>
constexpr auto operator+(quaternion<T> const& LHS, quaternion<U> const& RHS)
{
	using value_t = std::common_type_t<T, U>;
	return quaternion<value_t>{.w = LHS.w + RHS.w, .i = LHS.i + RHS.i, .j = LHS.j + RHS.j, .k = LHS.k + RHS.k};
}


template<typename T, typename U>
constexpr auto operator*(quaternion<T> const& LHS, quaternion<U> const& RHS)
{
	auto dir_times_q = [](ijk::detail::direction_or_floating auto lhs, auto const& rhs)
		{
			return (lhs * rhs.w) + (lhs * rhs.i) + (lhs * rhs.j) + (lhs* rhs.k);
		};

	auto impl = [dir_times_q](auto const& lhs, auto const& rhs)
		{
			return [&rhs, dir_times_q](auto&&... largs) { return (dir_times_q(largs, rhs) + ...); }(lhs.w, lhs.i, lhs.j, lhs.k);
		};
	return impl(LHS, RHS);
}