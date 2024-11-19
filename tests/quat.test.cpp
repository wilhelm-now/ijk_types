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
}