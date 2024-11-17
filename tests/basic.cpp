#include <quat.h>

static_assert(true, "can compile");

static constexpr auto plus_i = I{ 1. };
static constexpr auto plus_j = J{ 1. };
static constexpr auto plus_k = K{ 1. };

static_assert(plus_i == I{ 1. });
static_assert(plus_i != I{ 123.456 });

static_assert(plus_j == -J{ -1. }, "negation");

// Per component addition and subtraction
static_assert(plus_k + plus_k == K{ 2. }, "addition");
static_assert(K{ 3. } - plus_k == K{ 2. }, "subtraction");

// direction identities
static_assert(plus_j * plus_k == plus_i, "jk = i");
static_assert(plus_k * plus_j == -plus_i, "kj = -i");
static_assert(plus_i * plus_k == -plus_j, "ik = -j");
static_assert(plus_k * plus_i == plus_j, "ki = j");
static_assert(plus_i * plus_j == plus_k, "ij = k");
static_assert(plus_j * plus_i == -plus_k, "ji = -k");

static_assert(plus_i * plus_j * plus_k == W{ -1. }, "still not sure if result should be a directed value");

// If it compiles it's ok
int main(){}