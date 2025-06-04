#pragma once

#include "directions.h"
#include "type_help.h"


namespace ijk {
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

		template<is_complexable T>
		constexpr auto real_or_zero(T const& maybe_real)
		{
			if constexpr (is_complex<T>) { return maybe_real.real; }
			else if constexpr (is_I<T>) { return typename T::value_type{0}; }
			else { return maybe_real; }
		}

		template<is_complexable T>
		constexpr auto imag_or_zero(T const& maybe_imag)
		{
			if constexpr (is_complex<T>) { return maybe_imag.imag; }
			else if constexpr (is_I<T>) { return maybe_imag; }
			else { return I<T>{ 0 }; }
		}

		template<typename F, typename C>
		requires is_complex<std::remove_cvref_t<C>>
		constexpr decltype(auto) apply(F&& f, C&& c)
		{
			return std::invoke(std::forward<F>(f), 
				std::forward<C>(c).real, 
				std::forward<C>(c).imag);
		}
	}

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
		return complex<value_t>{
			real_or_zero(LHS) + real_or_zero(RHS), 
			imag_or_zero(LHS) + imag_or_zero(RHS) };
	}

	template<detail::is_complexable T, detail::is_complexable U>
	constexpr auto operator-(T const& LHS, U const& RHS)
	{
		using namespace detail;
		using value_t = std::common_type_t<value_type<T>, value_type<U>>;
		return complex<value_t>{
			real_or_zero(LHS) - real_or_zero(RHS),
			imag_or_zero(LHS) - imag_or_zero(RHS) };
	}

	template<detail::is_complexable T, detail::is_complexable U>
	constexpr auto operator*(T const& LHS, U const& RHS)
	{
		using namespace detail;
		return apply(apply(foiler{}, LHS), RHS);
	}

} // ijk