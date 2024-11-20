#pragma once

#include <concepts>
#include <type_traits>

namespace ijk
{
	namespace detail
	{
		template<typename T>
		concept has_direction = requires {typename T::direction;  };

		template<typename T>
		concept direction_or_floating = has_direction<T> || std::floating_point<T>;

		template<typename T>
		struct get_direction
		{
			using type = T::direction;
		};

		template<direction_or_floating T>
		using meta_direction = std::conditional_t<has_direction<T>, get_direction<T>, std::type_identity<double>>::type; // double used as "direction" type of real axis

		template<typename T, typename U>
		concept is_same_direction = std::same_as<meta_direction<T>, meta_direction<U>>;
	}
}