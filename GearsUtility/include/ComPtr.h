#pragma once
#include <wrl/client.h>

/**
 * @brief ComPtr �w�b�_�[
 */
template<class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;
