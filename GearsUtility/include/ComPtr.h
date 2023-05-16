#ifndef GEARS_COMPTR_H
#define GEARS_COMPTR_H

#include <wrl/client.h>

template<class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

#endif // GEARS_COMPTR_H
