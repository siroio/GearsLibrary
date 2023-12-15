#pragma once
#include <concepts>
#include <memory>
#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <ranges>
#include <IConverter.h>
#include <mmd/PmxObject.h>

/**
 * @brief ��O�N���X
 */
class UnsupportedFileTypeException : public std::runtime_error
{
public:
    // �R���X�g���N�^�p��
    using std::runtime_error::runtime_error;
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
        RegisterConverter<PmxModel>("pmx");
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
