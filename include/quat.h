#pragma once

#include "directions.h"

template<std::floating_point T>
struct quaternion
{
	W<T> w{0};
	I<T> i{0};
	J<T> j{0};
	K<T> k{0};

public:

	constexpr quaternion conjugate() const
	{
		return quaternion{ w, -i, -j, -k };
	}
};

template<typename T, typename U>
constexpr quaternion<std::common_type_t<T, U>> operator*(quaternion<T> const& LHS, quaternion<U> const& RHS)
{

	return {};
}