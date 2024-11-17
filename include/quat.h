#pragma once

#include <concepts>
#include <compare>

template<std::floating_point T, typename direction>
class directed_value
{
	T val;
public:
	constexpr explicit directed_value(T v)
		: val(v)
	{}

	auto operator<=>(directed_value const&) const = default;

	T value() const
	{
		return val;
	}
};

#define NAME_DIRECTION(dir) \
struct dir##_tag{}; \
template<typename T> \
using dir = directed_value<T, dir##_tag>; \


NAME_DIRECTION(I)
NAME_DIRECTION(J)
NAME_DIRECTION(K)
NAME_DIRECTION(W)