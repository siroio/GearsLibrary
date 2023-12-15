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
 * @brief 例外クラス
 */
class UnsupportedFileTypeException : public std::runtime_error
{
public:
    // コンストラクタ継承
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
     * @brief コンバーターの作成
     * @param extension 変換元拡張子
     */
    static std::unique_ptr<IConverter> Create(std::string_view extension) noexcept(false)
    {
        const auto& func = converters[extension.data()];
        return func ? func() : throw UnsupportedFileTypeException("This file is not supported.");
    }

private:
    static inline std::unordered_map<std::string, std::function<std::unique_ptr<IConverter>()>> converters;
};
