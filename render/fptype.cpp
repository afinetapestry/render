#include <random>

#include "fptype.h"

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<fptype> distr(0, 1);

fptype random() {
	return distr(gen);
}
