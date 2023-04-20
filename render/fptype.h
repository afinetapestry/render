#pragma once

#define fptype float

//#define random() ((fptype)std::rand() / (RAND_MAX + 1))

#ifdef __cplusplus
extern "C" {
#endif

fptype random();

#ifdef __cplusplus
}
#endif
