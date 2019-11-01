#pragma once

#ifdef DEBUG
#define LOG(x) std::cout << x
#else
#define LOG(x)
#endif	// DEBUG
