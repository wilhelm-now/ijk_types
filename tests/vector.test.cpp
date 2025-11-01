#include <ijk/vector.h>
using namespace ijk::literals;


template<typename T>
constexpr bool is_vector(T) { return false; }

template<typename T>
constexpr bool is_vector(ijk::vector<T>) { return true; }

constexpr ijk::vector<float> v;
static_assert(is_vector(v));
static_assert(is_vector(ijk::vector{ 1_k, 2_j, 3_i }));
static_assert(is_vector(ijk::vector{ 2_j }));

constexpr ijk::vector<float> a{ 1_i, 2_j, 3_k };
constexpr ijk::vector<float> b{ 5_i, 4_j, 3_k };

constexpr auto c = a + b;
static_assert(c == ijk::vector<float>{6_i, 6_k, 6_j});

constexpr auto d = c + 2_k;
static_assert(d == ijk::vector<double>{8_k, 6_i, 6_j});

constexpr auto e = 1_j + b + 2_k;
static_assert(e == ijk::vector{ 5_i, 5_j, 5_k });

static_assert(is_vector(3_j + 2_k));
static_assert(is_vector(a + 3_j + 2_k + 2_k + 1_i));


#include <iostream>

int main()
{
	auto c = a + b;
	std::cout << "a: " << a 
		<< "\nb: " << b 
		<< "\nc: " << c << '\n';
}