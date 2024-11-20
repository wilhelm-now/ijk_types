#include <quat.h>
#include <iostream>

static constexpr quaternion<double> q{};

constexpr auto b = std::is_floating_point_v<float>;

static_assert(ijk::detail::has_direction<K<float>>);
static_assert(std::is_same_v<ijk::detail::meta_direction<float>, double>);

int main(){
	quaternion<double> q{};
	q.assign_by_type(123.456);
	q.assign_by_type(K{ 789.f });
	q.assign_by_type(J{ 21.37 });

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
}