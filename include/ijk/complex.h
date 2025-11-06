#pragma once

#include "directions.h"
#include "type_help.h"


namespace ijk {
	template<std::floating_point T>
	struct complex;

	namespace detail
	{
		template<typename T>
		concept complex_direction = std::floating_point<T> || is_I<T>;

		template<typename T>
		concept is_complex = requires
		{
			typename T::value_type;
			requires std::same_as<complex<typename T::value_type>, T>;
		};

		template<typename T>
		concept is_complexable = complex_direction<T> || is_complex<T>;

		template<typename F, typename C>
		requires is_complex<std::remove_cvref_t<C>>
		constexpr decltype(auto) apply(F&& f, C&& c)
		{
			return std::invoke(std::forward<F>(f),
				std::forward<C>(c).real,
				std::forward<C>(c).imag);
		}
	}

	template<std::floating_point T>
	struct complex
	{
		using value_type = T;

		T real{ 0 };
		I<T> imag{ 0 };

		template<typename... Ts>
		requires (detail::unique_directions_v<Ts...> && sizeof...(Ts) < 3)
		constexpr explicit complex(Ts&&... ts)
		{
			detail::assigner_by_direction(real, imag)(std::forward<Ts>(ts)...);
		}

		auto operator<=>(complex const&) const = default;

		constexpr complex conjugate() const
		{
			return complex{ real, -imag };
		}
	};

	template<detail::complex_direction... Ts>
	complex(Ts...) -> complex<std::common_type_t<detail::value_type<std::remove_cvref_t<Ts>>...>>;
	
	template<typename stream_t, typename T>
	stream_t& operator<<(stream_t& os, complex<T> const& z)
	{
		return os << '{' << detail::print_applyable{ z } << '}';
	}

	template<detail::is_complexable T, detail::is_complexable U>
	constexpr auto operator+(T const& LHS, U const& RHS)
	{
		using namespace detail;
		using value_t = std::common_type_t<value_type<T>, value_type<U>>;
		complex<value_t> res{ LHS };
		apply(apply(directed_add_assign, res), RHS);
		return res;
	}

	template<detail::is_complexable T, detail::is_complexable U>
	constexpr auto operator-(T const& LHS, U const& RHS)
	{
		using namespace detail;
		using value_t = std::common_type_t<value_type<T>, value_type<U>>;
		complex<value_t> res{ LHS };
		apply(apply(directed_subtract_assign, res), RHS);
		return res;
	}

	template<detail::is_complexable T, detail::is_complexable U>
	constexpr auto operator*(T const& LHS, U const& RHS)
	{
		using namespace detail;
		return apply(apply(foiler{}, LHS), RHS);
	}

} // ijk