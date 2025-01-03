# ijk_types

What if unit vectors i, j, and k were types? Could we then define the quaternion multiplication identities instead of hard-coding the full quaternion multiplication? Some say yes.

## How

### Directions
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

// kj = -i
template<typename T, typename U>
constexpr auto operator*(K<T> LHS, J<T> RHS) { return I(-LHS.value() * RHS.value()); }

// and so on for the remaining identities, macros shorten implementation.
```

#### Weak types

The strong part of the types is the direction, otherwise they are weak and accept changing representation type. The quaternion type is also implicitly created from adding two different directions to each other.

#### Literals

User defined literals are cool and their definition in this project should not be surprising.

```c++
using namespace ijk::literals;

static_assert(1_i * 1_j * 1_k == -1, "ijk = -1 identity");
static_assert(1_jf + 1_jl == 2_jl, "float+long double is long double");
```

### Quaternions

We then define a quaternion (`quat`) to hold one value for the scalar and vector directions.

```c++
template<typename T>
struct quat
{
	T w{0};
	I<T> i{0};
	J<T> j{0};
	K<T> k{0};
};
```

#### Addition and Multiplication

For multiplcation we assume that any addition of values in different directions means creating a new quaternion. Adding a value `a` in the I direction with a value `b` in the K direction should create a new quaternion `q(w=0, i=a, j=0, k=b)`. Adding two different quaternions is simple component wise addition.

To multiply we use folds instead of for loops. First we create a function that binds each component of the left quaternion and sums the left-multiplication with a single argument. We then apply this function to each component of the right quaternion. 
The implementation of this is put in a function called `foiler` which can take any number of left arguments and returns a function to be used with any number of right arguments. 
Using a helper function that takes a variadic number of arguments makes it easy to implement multiplication by a scalar or directed value.
The name *foiler* comes from the well known first-outside-inside-last polynomial multiplication rule and is more familiar than the *grand sum of the outer product*.

```c++
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

// quaternion * quaternion
template<typename T, typename U>
constexpr auto operator*(quat<T> const& LHS, quat<T> const& RHS)
{
	return foiler(LHS.w, LHS.i, LHS.j, LHS.k)(RHS.w, RHS.i, RHS.j, RHS.k);
}

// quaternion * scalar or directed value
template<typename T, directed_or_numeric U>
constexpr auto operator*(quat<T> const& LHS, U RHS)
{
	return foiler(LHS.w, LHS.i, LHS.j, LHS.k)(RHS);
}
```