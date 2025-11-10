#pragma once

#include "directions.h"
#include "type_help.h"
#include "complex.h"
#include "vector.h"

#include <type_traits>


namespace ijk {
	template<std::floating_point T>
	struct quat;

	namespace detail
	{
		template<typename T>
		concept is_quat = requires
		{
			typename T::value_type;
			requires std::same_as<quat<typename T::value_type>, T>;
		};

		template<typename T>
		concept is_quatable = direction_or_floating<T> || is_complexable<T> || is_vectorable<T> || is_quat<T>;

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

	template<std::floating_point T>
	struct quat
	{
		using value_type = T;

		T w{ 0 };
		I<T> i{ 0 };
		J<T> j{ 0 };
		K<T> k{ 0 };

		auto operator<=>(quat const&) const = default;

		constexpr quat conjugate() const
		{
			return quat{ w, -i, -j, -k };
		}

		template<typename... Ts>
		requires (detail::unique_directions_v<Ts...> && sizeof...(Ts) < 5)
		constexpr explicit quat(Ts&&... ts)
		{
			detail::assigner_by_direction(w, i, j, k)(std::forward<Ts>(ts)...);
		}
	};

	template<detail::is_quatable... Ts>
	quat(Ts...) -> quat<std::common_type_t<detail::value_type<std::remove_cvref_t<Ts>>...>>;

	template<typename stream_t, typename T>
	stream_t& operator<<(stream_t& os, quat<T> const& q)
	{
		return os << '{' << detail::print_applyable{q} << '}';
	}

	// plus or minus of different directions implicitly creates a quaternion
	// todo restrict to complex or vector

	template<detail::is_quatable T, detail::is_quatable U>
	constexpr auto operator+(T const& LHS, U const& RHS)
	{
		using namespace detail;
		using value_t = std::common_type_t<value_type<T>, value_type<U>>;
		quat<value_t> q{LHS};
		apply(apply(directed_add_assign, q), RHS);
		return q;
	}

	template<detail::is_quatable T, detail::is_quatable U>
	constexpr auto operator-(T const& LHS, U const& RHS)
	{
		using namespace detail;
		using value_t = std::common_type_t<value_type<T>, value_type<U>>;
		quat<value_t> q{ LHS };
		apply(apply(directed_subtract_assign, q), RHS);
		return q;
	}

	template<typename T>
	constexpr quat<T> operator-(quat<T> const& RHS)
	{
		return quat<T>{-RHS.w, -RHS.i, -RHS.j, -RHS.k};
	}

	template<detail::is_quatable T, detail::is_quatable U>
	constexpr auto operator*(T const& LHS, U const& RHS)
	{
		using namespace detail;
		return apply(apply(foiler{}, LHS), RHS);
	}

} // namespace ijk