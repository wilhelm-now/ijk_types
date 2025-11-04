#pragma once

#include <concepts>
#include <type_traits>
#include <utility>
#include <functional>


namespace ijk
{
	namespace detail
	{
		template<typename T>
		concept has_direction = requires {typename T::direction;  };

		template<typename T>
		concept direction_or_floating = has_direction<T> || std::floating_point<T>;

		template<typename F, typename T>
		requires direction_or_floating<std::remove_cvref_t<T>>
		constexpr decltype(auto) apply(F&& f, T&& t)
		{
			return std::invoke(std::forward<F>(f), std::forward<T>(t));
		}

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

		template<typename... Ts>
		struct unique_directions;

		template<typename T, typename... Ts>
		struct unique_directions<T, Ts...> : std::conjunction<std::bool_constant<!is_same_direction<T, Ts>>..., unique_directions<Ts...>> {};

		template<typename T>
		struct unique_directions<T> : std::true_type {};

		template<>
		struct unique_directions<> : std::true_type {};

		template<typename... Ts>
		constexpr bool unique_directions_v = unique_directions<Ts...>::value;

		// The call operator of this struct a callable object to help do FOIL-like operations. 
		// FOIL: First Outside Inside Last multiplication of (a + b)(c + d) = a*c + a*d + b*c + b*d
		// To be used immediately like foiler{}(1, 2_i, 3_j, 4_k)(5, 6_i, 7_j, 8_k)
		// to multiply the quaternions (1, 2i, 3j, 4k) and (5, 6i, 7j, 8k)
		// This is a struct so that it can be used with std::invoke.
		struct foiler
		{
			template<typename... LeftTypes>
			constexpr auto operator()(LeftTypes&&... left_args) const
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
		};

		// Similar to foiler, returns something immedieatly callable with values to assign to assignees.
		template<typename... Assignees>
		constexpr auto assigner_by_direction(Assignees&&... assignees)
		{
			auto assigner = [&](auto&& value) mutable
				{
					auto assign = [] <typename T, typename U>(T& dest, U&& right) {
						static_assert(direction_or_floating<std::remove_cvref_t<U>>, "Right argument is directed value or number");
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

		template<typename Op>
		struct bind_for_compatible_directions
		{
			[[no_unique_address]] Op op = Op{};

			template<typename... BoundTypes>
			constexpr auto operator()(BoundTypes&&... bound) const
			{
				auto impl = [&](auto&& arg) mutable
				//auto impl = [this, ...bound = std::forward<BoundTypes>(bound)](auto&& arg) mutable
					{
						auto impl_impl = [this] <typename T, typename U>(T& bound_left, U && right)
						{
							if constexpr (is_same_direction<T, std::decay_t<U>>)
							{
								op(bound_left, right);
							}
						};
						(impl_impl(bound, arg), ...);
					};
				return [impl](auto&&... args) mutable
					{
						(impl(args), ...);
					};
			}
		};

		constexpr auto directed_add_assign = bind_for_compatible_directions{ [] <typename U>(auto& LHS, U && RHS) { LHS += std::forward<U>(RHS); } };
		constexpr auto directed_subtract_assign = bind_for_compatible_directions{ [] <typename U>(auto& LHS, U && RHS) { LHS -= std::forward<U>(RHS); } };

		template<typename applyable>
		struct print_applyable
		{
			applyable const& value;
		};

		template<typename stream_t, typename T>
		stream_t& operator<<(stream_t& stream, print_applyable<T> const& parent)
		{
			auto impl = [&stream]<typename head_t, typename... tail_t>(head_t && head, tail_t&&... tail)
			{
				stream << std::forward<head_t>(head);
				((stream << ", " << std::forward<tail_t>(tail)), ...);
			};
			apply(impl, parent.value);
			return stream;
		}

	}
}
