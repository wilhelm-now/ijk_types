# ijk_types

What if unit vectors i, j, and k were types? Could we then define the quaternion multiplication identities instead of hard-coding the full quaternion multiplication? Some say yes.

## How

Types with strong directions are defined like `directed_value<representation, direction>` and multiplicative identities are defined as specializations of `operator*`.

```c++
template<typename T> using I = directed_value<T, I_tag>;
template<typename T> using J = directed_value<T, J_tag>;
template<typename T> using K = directed_value<T, K_tag>;

// i^2 = j^2 = k^2 = -1 enforced by same direction
template<typename T, typename U, typename direction>
constexpr auto operator*(directed_value<T, direction> LHS, directed_value<U, direction> RHS)
{
	return -LHS.value() * RHS.value();
}

// ij = k
template<typename T, typename U>
constexpr auto operator*(I<T> LHS, J<T> RHS) { return K(LHS.value() * RHS.value()); }

// kj = -1
template<typename T, typename U>
constexpr auto operator*(K<T> LHS, J<T> RHS) { return I(-LHS.value() * RHS.value()); }

// and so on for the remaining identities
```

## Weak types

The strong part of the types is the direction, otherwise they are weak and accept changing representation type. The quaternion type is also implicitly created from adding two different directions to each other.

## Literals

User defined literals are cool and their definition in this project should not be surprising.

```c++
using namespace ijk::literals;

static_assert(1_i * 1_j * 1_k == -1, "ijk = -1 identity");
static_assert(1_jf + 1_jl == 2_jl, "float+long double is long double");
```