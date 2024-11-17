#include <quat.h>

static_assert(true, "can compile");

static constexpr auto plus_i = I{ 1. };
static constexpr auto plus_j = J{ 1. };
static constexpr auto plus_k = K{ 1. };

static_assert(plus_i == I{ 1. });
static_assert(plus_i != I{ 123.456 });

int main(){}