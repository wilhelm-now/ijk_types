#pragma once

#include "directions.h"
#include "type_help.h"

namespace ijk {

	template<std::floating_point T>
	struct vector
	{
		using value_type = T;

		I<T> x{0};
		J<T> y{0};
		K<T> z{0};

		template<typename... Ts>
			requires (detail::unique_directions_v<Ts...> && sizeof...(Ts) < 4)
		constexpr explicit vector(Ts&&... ts)
		{
			detail::assigner_by_direction(x, y, z)(std::forward<Ts>(ts)...);
		}

		auto operator<=>(vector const&) const = default;
	};

	namespace detail
	{
		template<typename T>
		concept vector_direction = is_I<T> || is_J<T> || is_K<T>;

		template<typename T>
		concept is_vector = requires
		{
			typename T::value_type;
			requires std::same_as<vector<typename T::value_type>, T>;
		};

		template<typename T>
		concept is_vectorable = is_vector<T> || vector_direction<T>;

		template<typename F, typename V>
		requires is_vector<std::remove_cvref_t<V>>
		constexpr decltype(auto) apply(F&& f, V&& v)
		{
			return std::invoke(std::forward<F>(f),
				std::forward<V>(v).x,
				std::forward<V>(v).y,
				std::forward<V>(v).z);
		}
	}

	template<detail::vector_direction... Ts>
	vector(Ts...) -> vector<std::common_type_t<detail::value_type<std::remove_cvref_t<Ts>>...>>;


	template<typename stream_t, typename T>
	stream_t& operator<<(stream_t& os, vector<T> const& z)
	{
		return os << '{' << detail::print_applyable{ z } << '}';
	}

	template<typename T, typename U>
	requires detail::is_vectorable<T> && detail::is_vectorable<U>
	constexpr auto operator+(T const& LHS, U const& RHS)
	{
		using namespace detail;
		using value_t = std::common_type_t<value_type<T>, value_type<U>>;
		vector<value_t> res{}; // zero = addidative identity value
		
		auto add_assign = []<typename T, typename U>(T & left, U && right)
		{
			left += std::forward<U>(right);
		};
		auto impl = apply(bind_for_compatible_directions{ add_assign }, res);
		apply(impl, LHS);
		apply(impl, RHS);
		return res;
	}
}