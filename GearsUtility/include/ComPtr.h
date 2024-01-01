#pragma once
#include <wrl/client.h>

/**
 * @brief ComPtr ヘッダー
 */
template<class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

