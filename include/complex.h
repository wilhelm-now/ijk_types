#pragma once

#include "directions.h"
#include "type_help.h"

namespace ijk {

	template<std::floating_point T>
	struct complex
	{
		T real{ 0 };
		I<T> imag{ 0 };

		auto operator<=>(complex const&) const = default;

		constexpr complex conjugate() const
		{
			return complex{ real, -imag };
		}
	};

	template<typename stream_t, typename T>
	stream_t& operator<<(stream_t& os, complex<T> const& z)
	{
		return os << "{ " << z.real << ", " << z.imag.value() << "i }";
	}

	// complex + complex
	template<typename T, typename U>
	constexpr auto operator+(complex<T> const& LHS, complex<U> const& RHS)
	{
		return complex{.real = LHS.real + RHS.real, .imag = LHS.imag + RHS.imag};
	}

	// real + imaginary
	template<std::floating_point T, typename U>
	constexpr auto operator+(T const& LHS, I<U> const& RHS)
	{
		using value_t = std::common_type_t<T, U>;
		return complex<value_t>{.real = LHS, .imag = RHS};
	}

	// imaginary + real
	template<typename T, std::floating_point U>
	constexpr auto operator+(I<T> const& LHS, U const& RHS)
	{
		return RHS + LHS;
	}

	// imaginary + complex
	template<typename T, typename U>
	constexpr auto operator+(I<T> const& LHS, complex<U> const& RHS)
	{
		return complex{ RHS.real, RHS.imag + LHS };
	}

	// comlpex + imaginary
	template<typename T, typename U>
	constexpr auto operator+(complex<T> const& LHS, I<U> const& RHS)
	{
		return RHS + LHS;
	}

	// real + complex
	template<std::floating_point T, typename U>
	constexpr auto operator+(T const& LHS, complex<U> const& RHS) 
	{
		return complex{ RHS.real + LHS, RHS.imag };
	}

	// complex + real
	template<typename T, std::floating_point U>
	constexpr auto operator+(complex<T> const& LHS, U const& RHS)
	{
		return RHS + LHS;
	}

	// complex * complex
	template<typename T, typename U>
	constexpr auto operator*(complex<T> const& LHS, complex<U> const& RHS)
	{
		return detail::foiler(LHS.real, LHS.imag)(RHS.real, RHS.imag);
	}

	// complex * imaginary
	template<typename T, typename U>
	constexpr auto operator*(complex<T> const& LHS, I<U> const& RHS)
	{
		return detail::foiler(LHS.real, LHS.imag)(RHS);
	}

	// imaginary * complex
	template<typename T, typename U>
	constexpr auto operator*(I<T> const& LHS, complex<U> const& RHS)
	{
		return RHS * LHS;
	}

	// real * complex
	template<std::floating_point T, typename U>
	constexpr auto operator*(T const& LHS, complex<U> const& RHS)
	{
		return detail::foiler(LHS)(RHS.real, RHS.imag);
	}

	// complex * real
	template<typename T, std::floating_point U>
	constexpr auto operator*(complex<T> const& LHS, U const& RHS)
	{
		return RHS * LHS;
	}

} // ijk