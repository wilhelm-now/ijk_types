#pragma once

#include <concepts>
#include <type_traits>
#include <utility>


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

		template<typename T>
		struct get_value_type
		{
			using type = T::value_type;
		};

		template<typename T>
		concept has_value_type = requires {typename T::value_type; };

		template<typename T>
		using value_type = std::conditional_t<has_value_type<T>, get_value_type<T>, std::type_identity<T>>::type;

		// Returns a callable object to help do FOIL-like operations. 
		// FOIL: First Outside Inside Last multiplication of (a + b)(c + d) = a*c + a*d + b*c + b*d
		// To be used immediately like foiler(1, 2_i, 3_j, 4_k)(5, 6_i, 7_j, 8_k)
		// to multiply the quaternions (1, 2i, 3j, 4k) and (5, 6i, 7j, 8k)
		template<typename... LeftTypes>
		constexpr auto foiler(LeftTypes&&... left_args)
		{
			// Function that multiplies a single right argument by all left arguments
			auto left_multiply = [...largs = std::forward<LeftTypes>(left_args)](auto&& right)
				{
					return ((largs * right) + ...);
				};

			// Function that sums every left argument multiplied by every right argument
			auto for_right_args = [left_multiply](auto&&... right_args)
				{
					return (left_multiply(right_args) + ...);
				};

			return for_right_args;
		}

		// Similar to foiler, returns something immedieatly callable with values to assign to assignees.
		template<typename... Assignees>
		constexpr auto assigner_by_direction(Assignees&&... assignees)
		{
			auto assigner = [&](auto&& value) mutable
				{
					auto assign = [] <typename T, typename U>(T& dest, U&& right) {
						if constexpr (is_same_direction<T, std::decay_t<U>>)
						{
							dest = std::forward<U>(right);
						}
					};
					(assign(assignees, value), ...);
				};

			return [assigner](auto&&... values) mutable
				{
					(assigner(values), ...);
				};
		}
	}
}
