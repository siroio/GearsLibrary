#pragma once
#include <fstream>

namespace Glib
{
    /**
     * @brief バイナリファイルへ書き出し
     * @param file バイナリファイル
     * @param data データへのポインタ
     * @param length データ長
     */
    inline void WriteToBinary(std::ofstream& file, const void* const data, const int length)
    {
        file.write(reinterpret_cast<const char*>(data), length);
    }

    /**
     * @brief バイナリから読み出し
     * @param file バイナリファイル
     * @param data データへのポインタ
     * @param length データ長
     */
    inline void ReadForBinary(std::ifstream& file, void* const data, const int length)
    {
        file.read(reinterpret_cast<char*>(data), length);
    }
}

