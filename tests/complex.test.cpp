#include <complex.h>
using namespace ijk::literals;

template<typename T>
constexpr bool is_complex(T) { return false; }

template<typename T>
constexpr bool is_complex(ijk::complex<T>) { return true; }

static_assert(!is_complex(123.456));
static_assert(!is_complex(3_i));

static_assert(is_complex(ijk::complex{ 1.0, 1_i }));
static_assert(is_complex(ijk::complex{ 2_i, 0.5f }));
static_assert(is_complex(ijk::complex{ 2_if }));
static_assert(is_complex(ijk::complex{ 3.f }));

static_assert(is_complex(123.456f + 3_i));
static_assert(is_complex(-456_if + 789.0));
static_assert(!is_complex(12.34 + 56.789f));
static_assert(!is_complex(1_i + 2_if));

static constexpr auto comp = 1.0 + 1_i;
static_assert(is_complex(comp));
static_assert(is_complex(comp * comp));
static_assert(is_complex(comp * comp.conjugate()));

static_assert(is_complex(comp + 2_i));
static_assert(is_complex(3_i + comp + 4_i));
static_assert(is_complex(4.5f + comp));

static_assert(is_complex(comp * 3_i));
static_assert(is_complex(4_i * comp));

static_assert(is_complex(123.456 * comp));
static_assert(is_complex(comp * 1.f));

static_assert(comp * 2.0 == 2.0 + 2_i);
static_assert(3.f * comp == 3.0 + 3_i);

static_assert(comp * 4_i == -4.0 + 4_i);
static_assert(5_i * comp == -5.0 + 5_i);

int main(){}