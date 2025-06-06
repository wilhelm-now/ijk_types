#pragma once

#include "directions.h"
#include "type_help.h"

#include <type_traits>


namespace ijk {

	template<std::floating_point T>
	struct quat
	{
		T w{ 0 };
		I<T> i{ 0 };
		J<T> j{ 0 };
		K<T> k{ 0 };

	public:
		using value_type = T;

		auto operator<=>(quat const&) const = default;

		constexpr quat conjugate() const
		{
			return quat{ w, -i, -j, -k };
		}

		template<detail::direction_or_floating U>
		constexpr void assign_by_type(U const& u)
		{
			detail::assigner_by_direction(w, i, j, k)(u);
		}
	};

	namespace detail
	{
		template<typename T>
		concept is_quat = requires
		{
			typename T::value_type;
			requires std::same_as<quat<typename T::value_type>, T>;
		};

		template<typename T>
		concept is_quatable = direction_or_floating<T> || is_quat<T>;

		template<typename F, typename Q>
		requires is_quat<std::remove_cvref_t<Q>>
		constexpr decltype(auto) apply(F&& f, Q&& q)
		{
			return std::invoke(std::forward<F>(f), 
				std::forward<Q>(q).w,
				std::forward<Q>(q).i,
				std::forward<Q>(q).j,
				std::forward<Q>(q).k);
		}
	}

	template<typename stream_t, typename T>
	stream_t& operator<<(stream_t& os, quat<T> const& q)
	{
		return os << '{' << detail::print_applyable{q} << '}';
	}

	// plus or minus of different directions implicitly creates a quaternion
	// todo restrict to complex or vector

	template<detail::direction_or_floating T, detail::direction_or_floating U>
		requires (!detail::is_same_direction<T, U>)
	constexpr auto operator+(T const& LHS, U const& RHS)
	{
		using value_t = std::common_type_t<detail::value_type<T>, detail::value_type<U>>;
		quat<value_t> q{};
		q.assign_by_type(LHS);
		q.assign_by_type(RHS);
		return q;
	}

	template<detail::direction_or_floating T, detail::direction_or_floating U>
		requires (!detail::is_same_direction<T, U>)
	constexpr auto operator-(T const& LHS, U const& RHS)
	{
		using value_t = std::common_type_t<detail::value_type<T>, detail::value_type<U>>;
		quat<value_t> q{};
		q.assign_by_type(LHS);
		q.assign_by_type(-RHS);
		return q;
	}

	template<typename T>
	constexpr quat<T> operator-(quat<T> const& RHS)
	{
		return quat<T>{.w = -RHS.w, .i = -RHS.i, .j = -RHS.j, .k = -RHS.k};
	}

	template<typename T, detail::direction_or_floating U>
	constexpr auto operator+(quat<T> const& LHS, U const& RHS)
	{
		using value_t = std::common_type_t<T, detail::value_type<U>>;
		quat<value_t> q{ .w = LHS.w, .i = LHS.i, .j = LHS.j, .k = LHS.k };
		[&RHS](auto&&... args)
			{
				auto plus = [&RHS]<typename T>(T & lhs)
				{
					if constexpr (detail::is_same_direction<T, U>)
					{
						lhs = lhs + RHS;
					}
				};
				(plus(args), ...);
			}(q.w, q.i, q.j, q.k);
		return q;
	}

	template<detail::direction_or_floating U, typename T>
	constexpr auto operator+(U const& LHS, quat<T> const& RHS)
	{
		return RHS + LHS; // commutative addition, LHS + RHS = RHS + LHS
	}

	template<typename T, typename U>
	constexpr auto operator+(quat<T> const& LHS, quat<U> const& RHS)
	{
		using value_t = std::common_type_t<T, U>;
		return quat<value_t>{.w = LHS.w + RHS.w, .i = LHS.i + RHS.i, .j = LHS.j + RHS.j, .k = LHS.k + RHS.k};
	}

	template<detail::is_quatable T, detail::is_quatable U>
	constexpr auto operator*(T const& LHS, U const& RHS)
	{
		using namespace detail;
		return apply(apply(foiler{}, LHS), RHS);
	}

} // namespace ijk