#include <ijk/quat.h>
#include <iostream>

using namespace ijk;
using namespace ijk::literals;

static constexpr quat<double> q{};

constexpr auto b = std::is_floating_point_v<float>;

static_assert(ijk::detail::has_direction<K<float>>);
static_assert(std::is_same_v<ijk::detail::meta_direction<float>, double>);

static constexpr auto qi = quat<double>{ 1_i };
static constexpr auto qj = quat<double>{ 1_j };
static constexpr auto qk = quat<double>{ 1_k };
static_assert(qi * qj == qk, "ij = k again but with quaternions that only have one component");
static_assert(qj * qi == -qk);

constexpr auto q1 = 1. + 2_i + 3_j + 4_k;
constexpr auto q2 = 5. + 6_i + 7_j + 8_k;
static_assert(q1 * q2 == -60. + 12_i + 30_j + 24_k);
static_assert(q2 * q1 == -60. + 20_i + 14_j + 32_k);

static_assert(2. * q1 == q1 + q1, "scalar multiplication");
static_assert(q2 * 3. == q2 + q2 + q2, "more scalar multiplication");

static_assert(qi * 1_j == qk, "ij = k but without quaternion right argument");
static_assert(1_j * qi == -qk, "ji = -k without quaternion left argument");
static_assert(1_i * 1_j == 1_k, "ij = k but still only using directed values not quaternions");
static_assert(std::same_as<decltype(1_i * 1_j), ijk::K<double>>, "directed value multiplication gives another directed value instead of quaternion");
static_assert(std::same_as<decltype(1_i * 2_i), double>, "but multiplication of same direction gives scalar");

int main(){
	ijk::quat<double> q{ 123.456, K{ 789.f }, J{ 21.37 } };

	std::cout << q << '\n';

	auto m = J{ 12.34 };
	auto n = K{ 56.78f };
	auto sum = m + n;
	std::cout << sum << '\n';
	
	auto p = I{ 555.555 };
	auto diff = n - p;
	std::cout << diff << '\n';

	auto more_parts = m + n + p;
	std::cout << more_parts << '\n';
	std::cout << sum + more_parts << '\n';

	auto product = sum * diff;
	std::cout << "sum*diff=" << sum << "*" << diff << "\n\t=" << product << '\n';

	constexpr auto v = ijk::vector{ 1_i, 3_k, 2_jl };
	constexpr auto qv = ijk::quat{ v, 10.f };
	std::cout << "vector: " << v << ", vector and real: " << qv << '\n';
}