#pragma once
#include <string_view>
#include <vector>

struct Vector2;
struct Vector3;
struct Vector4;
struct Color;

namespace Glib
{
    /**
     * @brief ImGuiラッパークラス
     */
    class GLGUI
    {
    public:
        /**
         * @brief 間隔を開ける
         */
        static void Space();

        /**
         * @brief セパレータの追加
         */
        static void Separator();

        /**
         * @brief テキストの表示
         * @param text
         */
        static void Text(std::string_view fmt, ...);

        /**
         * @brief ボタンを表示
         * @param label 表示名
         * @param sameLine 同じ行に表示させるか
         * @return 押したか
         */
        static bool Button(std::string_view label, bool sameLine = false);

        /**
         * @brief チェックボックスの表示
         * @param label 表示名
         * @param checked チェック状態
         * @param sameLine 同じ行に表示させるか
         * @return 押したか
         */
        static bool CheckBox(std::string_view label, bool* checked, bool sameLine = false);

        /**
         * @brief ドロップダウンリストの表示
         * @param label 表示名
         * @param curerntItem 選択アイテム
         * @param items 選択肢
         * @return 押したか
         */
        static bool ComboBox(std::string_view label, std::string& curerntItem, const std::vector<std::string>& items);

        static bool TreeNode(std::string_view label, bool leaf = false);
        static void TreePop();

        static bool InputInt(std::string_view label, int* value, bool sameLine = false);
        static bool InputFloat(std::string_view label, float* value, bool sameLine = false);

        static bool DragInt(std::string_view label, int* value, int min = 0, int max = 0, float speed = 1.0f, bool sameLine = false);
        static bool DragFloat(std::string_view label, float* value, float min = 0.0f, float max = 0.0f, float speed = 1.0f, bool sameLine = false);

        static bool SliderInt(std::string_view label, int* value, int min, int max, bool sameLine = false);
        static bool SliderFloat(std::string_view label, float* value, float min, float max, bool sameLine = false);
    };
}
