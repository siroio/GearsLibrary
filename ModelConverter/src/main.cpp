#include <string>
#include <iostream>
#include <filesystem>
#include <Converter.h>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    //if (argc != 3)
    //{
    //    std::cerr << "Usage: " << " <input_file> <output_path>" << std::endl;
    //    return -1;
    //}

    ////�t�@�C���p�X�̎擾�Ɛ��K��
    //auto inputFile = fs::path{ argv[1] }.lexically_normal();
    //auto outputFile = fs::path{ argv[2] }.lexically_normal();

    auto inputFile = fs::path{ R"(C:\Users\rukar\Desktop\MikuMikuDance_v932x64\Appearance Miku\Appearance Miku.pmx)" };
    auto outputFile = fs::path{ R"(C:\Users\rukar\Desktop\MikuMikuDance_v932x64\Appearance Miku\Appearance Miku)" };

    //auto inputFile = fs::path{ R"(C:\Users\rukar\Desktop\MikuMikuDance_v932x64\�������ł������[�V����\�ɂڃ~�N�����\�ɂڃ~�N����_�������ł���.vmd)" };
    //auto outputFile = fs::path{ R"(C:\Users\rukar\Desktop\MikuMikuDance_v932x64\�������ł������[�V����\�ɂڃ~�N�����\�ɂڃ~�N����_�������ł���)" };


    // ���΃p�X�����΃p�X�ɕϊ�
    outputFile = outputFile.is_absolute() ? outputFile : fs::absolute(outputFile);

    try
    {
        // �t�@�C���̊m�F
        if (!fs::exists(inputFile)) throw std::runtime_error{ "File is not exist." };
        Converter::Initialize();
        auto inputExt = inputFile.extension().generic_string();
        std::erase(inputExt, '.'); // erase extension "."
        auto converter = Converter::Create(inputExt);

        // �t�@�C���̕ϊ�
        if (!converter->LoadFile(inputFile.generic_string()))
        {
            throw std::runtime_error{ "Convert Failed." };
            return -1;
        }

        // �t�@�C���̏����o��
        if (!converter->WriteFile(outputFile.generic_string()))
        {
            throw std::runtime_error{ "File Write Failed." };
            return -1;
        }

        // �o�͐�\��
        std::cout << "OUTPUT DIR : " << outputFile.generic_string() << std::endl;
    }
    catch (const std::runtime_error& error)
    {
        // �G���[���b�Z�[�W�o��
        std::cerr << error.what() << std::endl;
        return -1;
    }
}
