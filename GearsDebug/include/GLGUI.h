#pragma once
#include <string_view>
#include <vector>

#define GL_SLIDER_CLAMP ImGuiSliderFlags_AlwaysClamp
#define GL_ENTER_RETURN ImGuiInputTextFlags_EnterReturnsTrue
#define GL_COLOR_FLAG ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_InputHSV | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float
#define GL_TREE_NODE ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick
#define GL_TREE_LEAF GL_TREE_NODE | ImGuiTreeNodeFlags_Leaf

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

        /**
         * @brief ツリーノードを表示
         * @param label
         * @param leaf
         * @return 押したか
         */
        static bool TreeNode(std::string_view label, bool leaf = false);

        /**
         * @brief ツリーを終了
         */
        static void TreePop();

        /**
         * @brief Int型が入力可能なボックスを表示
         * @param label
         * @param value
         * @param sameLine
         * @return
         */
        static bool InputInt(std::string_view label, int* value, bool sameLine = false);

        /**
         * @brief Float型が入力可能なボックスを表示
         * @param label
         * @param value
         * @param sameLine
         * @return
         */
        static bool InputFloat(std::string_view label, float* value, bool sameLine = false);

        /**
         * @brief Int型が入力可能なボックスを表示
         * @param label
         * @param value
         * @param sameLine
         * @return
         */
        static bool InputVector2(std::string_view label, Vector2* value, bool sameLine = false);
        static bool InputVector3(std::string_view label, Vector3* value, bool sameLine = false);
        static bool InputVector4(std::string_view label, Vector4* value, bool sameLine = false);

        static bool DragInt(std::string_view label, int* value, float speed = 1.0f, int min = 0, int max = 0, bool sameLine = false);
        static bool DragFloat(std::string_view label, float* value, float speed = 1.0f, float min = 0.0f, float max = 0.0f, bool sameLine = false);
        static bool DragVector3(std::string_view label, Vector3* value, float speed = 1.0f, float min = 0.0f, float max = 0.0f, bool sameLine = false);
        static bool DragVector4(std::string_view label, Vector4* value, float speed = 1.0f, float min = 0.0f, float max = 0.0f, bool sameLine = false);

        static bool SliderInt(std::string_view label, int* value, int min, int max, bool sameLine = false);
        static bool SliderFloat(std::string_view label, float* value, float min, float max, bool sameLine = false);

        static bool CoorPicker3(std::string_view label, Color* color, bool sameLine = false);
        static bool CoorPicker4(std::string_view label, Color* color, bool sameLine = false);
    };
}
