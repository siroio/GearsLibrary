#include <string>
#include <iostream>
#include <filesystem>
#include <Converter.h>
#include <GLAnimation.h>
#include <GLObject.h>

namespace fs = std::filesystem;
using namespace Glib;

int main(int argc, char* argv[])
{
    //if (argc != 3)
    //{
    //    std::cerr << "Usage: " << " <input_file> <output_path>" << std::endl;
    //    return -1;
    //}

    //ファイルパスの取得と正規化
    //auto inputFile = fs::path{ argv[1] }.lexically_normal();
    //auto outputFile = fs::path{ argv[2] }.lexically_normal();

    auto inputFile = fs::path{ R"(C:\Users\rukar\Desktop\MikuMikuDance_v932x64\Appearance Miku\Appearance Miku.pmx)" }.lexically_normal();
    auto outputFile = fs::path{ R"(C:\Users\rukar\Desktop\MikuMikuDance_v932x64\Appearance Miku)" }.lexically_normal();

    // 相対パスから絶対パスに変換
    outputFile = outputFile.is_absolute() ? outputFile : fs::absolute(outputFile);

    try
    {
        // ファイルの確認
        if (!fs::exists(inputFile)) throw std::runtime_error{ "File is not exist." };
        Converter::Initialize();
        auto inputExt = inputFile.extension().generic_string();
        std::erase(inputExt, '.'); // erase extension "."
        auto converter = Converter::Create(inputExt);

        // ファイルの変換
        if (!converter->LoadFile(inputFile.generic_string()))
        {
            throw std::runtime_error{ "Convert Failed." };
            return -1;
        }

        // ファイルの書き出し
        if (!converter->WriteFile(outputFile.generic_string()))
        {
            throw std::runtime_error{ "File Write Failed." };
            return -1;
        }

        // 出力先表示
        std::cout << "OUTPUT DIR : " << outputFile.generic_string() << std::endl;
    }
    catch (const std::runtime_error& error)
    {
        // エラーメッセージ出力
        std::cerr << error.what() << std::endl;
        return -1;
    }
}
