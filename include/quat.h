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

	constexpr T value() const
	{
		return val;
	}
};

template<typename T, typename U, typename direction>
using common_dir = directed_value<std::common_type_t<T, U>, direction>;

template<std::floating_point T, std::floating_point U, typename direction>
constexpr common_dir<T, U, direction> operator+(directed_value<T, direction> const& LHS, directed_value<U, direction> const& RHS)
{
	return common_dir<T, U, direction>(LHS.value() + RHS.value());
}

template<std::floating_point T, typename direction>
constexpr directed_value<T, direction> operator-(directed_value<T, direction> const& RHS)
{
	return directed_value<T, direction>(-RHS.value());
}

template<std::floating_point T, std::floating_point U, typename direction>
constexpr common_dir<T, U, direction> operator-(directed_value<T, direction> const& LHS, directed_value<U, direction> const& RHS)
{
	return common_dir<T, U, direction>(LHS.value() - RHS.value());
}


#define NAME_DIRECTION(dir) \
struct dir##_dir{}; \
template<typename T> \
using dir = directed_value<T, dir##_dir>; \

NAME_DIRECTION(I)
NAME_DIRECTION(J)
NAME_DIRECTION(K)
NAME_DIRECTION(W)  // TODO: decide if this needs a direction or should be a plain number

// Now applying quaternion identities
// ii == jj == kk == ijk == -1 (w in this case)
// These lead to
// i = jk = -kj
// j = -ik = ki
// k = ij = -ji 

// ii, jj, kk covered by requiring same direction tag for 
template<typename T, typename U, typename direction>
constexpr directed_value<std::common_type_t<T, U>, W_dir> operator*(directed_value<T, direction> const& LHS, directed_value<U, direction> const& RHS)
{
	return directed_value<std::common_type_t<T, U>, W_dir>(-LHS.value() * RHS.value());
}

#define MULTIPLY_DIRS(LHS_DIR, RHS_DIR, RESULT_DIR, RESULT_SIGN) \
template<typename T, typename U> \
constexpr directed_value<std::common_type_t<T, U>, RESULT_DIR> operator*(directed_value<T, LHS_DIR> const& LHS, directed_value<U, RHS_DIR> const& RHS) \
{ \
	return directed_value<std::common_type_t<T, U>, RESULT_DIR>(RESULT_SIGN LHS.value() * RHS.value()); \
}

MULTIPLY_DIRS(J_dir, K_dir, I_dir, +) // jk = i
MULTIPLY_DIRS(K_dir, J_dir, I_dir, -) // kj = -i
MULTIPLY_DIRS(I_dir, K_dir, J_dir, -) // ik = -j
MULTIPLY_DIRS(K_dir, I_dir, J_dir, +) // ki = j
MULTIPLY_DIRS(I_dir, J_dir, K_dir, +) // ij = k
MULTIPLY_DIRS(J_dir, I_dir, K_dir, -) // ji = -k