#pragma once
#include <concepts>
#include <memory>
#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <ranges>
#include <IConverter.h>
#include <mmd/PmxObject.h>
#include <mmd/VmdObject.h>

/**
 * @brief ��O�N���X
 */
class UnsupportedFileTypeException : public std::runtime_error
{
public:
    // �p���R���X�g���N�^
    using std::runtime_error::runtime_error;
};

class IllegalConverterException : public std::logic_error
{
public:
    // �p���R���X�g���N�^
    using std::logic_error::logic_error;
};

class Converter
{
public:
    template <class T> requires std::derived_from<T, IConverter>
    static void RegisterConverter(std::string_view extension)
    {
        converters.emplace(extension, []()
        {
            return std::make_unique<T>();
        });
    };

    static void Initialize()
    {
        // �ϊ��p�̃N���X��o�^
        RegisterConverter<PmxModel>("pmx");
        RegisterConverter<VmdMotion>("vmd");
    }

    /**
     * @brief �R���o�[�^�[�̍쐬
     * @param extension �ϊ����g���q
     */
    static std::unique_ptr<IConverter> Create(std::string_view extension) noexcept(false)
    {
        const auto& func = converters[extension.data()];
        return func ? func() : throw UnsupportedFileTypeException("This file is not supported.");
    }

private:
    static inline std::unordered_map<std::string, std::function<std::unique_ptr<IConverter>()>> converters;
};
