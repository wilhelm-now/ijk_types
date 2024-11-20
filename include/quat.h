#pragma once

#include "directions.h"
#include "type_help.h"

#include <type_traits>



template<std::floating_point T>
struct quaternion
{
	T w{0};
	I<T> i{0};
	J<T> j{0};
	K<T> k{0};

public:

	constexpr quaternion conjugate() const
	{
		return quaternion{ w, -i, -j, -k };
	}

	template<ijk::detail::direction_or_floating U>
	constexpr void assign_by_type(U const& u)
	{
		[&u](auto&&... args)
			{
				auto assign = [&u]<typename T>(T & dest)
				{
					if constexpr (ijk::detail::is_same_direction<T, U>)
					{
						dest = u;
					}
				};
				(assign(args), ...);
			}(w, i, j, k);
	}
};

template<typename stream_t, typename T>
stream_t& operator<<(stream_t& os, quaternion<T> const& q)
{
	return os << "{ " << q.w << ", " << q.i.value() << "i, " << q.j.value() << "j, " << q.k.value() << "k }";
}

// plus or minus of different directions implicitly creates a quaternion
// todo restrict to complex or vector

//template<typename T, typename U>
//auto operator+(T const& LHS, U const& RHS) requires !std::is_same_v<T, U>
//{
//
//}


//template<typename T, typename U>
//constexpr quaternion<std::common_type_t<T, U>> operator*(quaternion<T> const& LHS, quaternion<U> const& RHS)
//{
//
//	return {};
//}