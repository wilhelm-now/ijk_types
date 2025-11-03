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

static_assert(a == ijk::vector<float>(a));
static_assert(a == ijk::vector<float>{a});

constexpr auto c = a + b;
static_assert(c == ijk::vector<float>{6_i, 6_k, 6_j});

constexpr auto d = c + 2_k;
static_assert(d == ijk::vector<double>{8_k, 6_i, 6_j});
static_assert(d - 2_k == ijk::vector(6_k, 6_j, 6_i));

constexpr auto e = 1_j + b + 2_k;
static_assert(e == ijk::vector{ 5_i, 5_j, 5_k });

static_assert(is_vector(3_j + 2_k));
static_assert(is_vector(a + 3_j + 2_k + 2_k + 1_i));

static_assert(e - e == ijk::vector<double>{});
static_assert(e + e - e == e);
static_assert(3_j + e - 3_j == e);

static_assert(a + a + a == 3.0 * a);
static_assert(a - a == b * 0.0f);

static_assert(a / 1.0 == a);
static_assert((c + c) / 2.0 == c);

#include <iostream>

int main()
{
	auto c = a + b;
	std::cout << "a: " << a 
		<< "\nb: " << b 
		<< "\nc: " << c << '\n';
}