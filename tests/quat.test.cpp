#include <quat.h>
#include <iostream>

static constexpr quaternion<double> q{};

constexpr auto b = std::is_floating_point_v<float>;

int main(){
	quaternion<double> q{};
	q.assign_by_type(123.456);
	q.assign_by_type(K{ 123.f });

	std::cout << q << '\n';
}