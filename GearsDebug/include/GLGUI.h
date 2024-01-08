#pragma once
#include <string_view>
#include <vector>
#include <float.h>

#define GL_SLIDER_CLAMP ImGuiSliderFlags_AlwaysClamp
#define GL_ENTER_RETURN ImGuiInputTextFlags_EnterReturnsTrue
#define GL_COLOR_PICK_FLAG ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_InputHSV | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float
#define GL_COLOR_INPUT_FLAG ImGuiColorEditFlags_InputRGB 
#define GL_COLOR_BAR_FLAG ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_DisplayRGB
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
         * @brief 折りたたみ可能なヘッダーを表示
         * @param label
         * @return 開いているか
        */
        static bool TabHeader(std::string_view label, bool open = false);

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
        static bool TreeNode(std::string_view label, bool leaf = false, bool openNode = false);

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

        static bool InputVector2(std::string_view label, Vector2* value, bool sameLine = false);
        static bool InputVector3(std::string_view label, Vector3* value, bool sameLine = false);
        static bool InputVector4(std::string_view label, Vector4* value, bool sameLine = false);

        static bool DragInt(std::string_view label, int* value, float speed = 1.0f, int min = INT_MIN, int max = INT_MAX, bool sameLine = false);
        static bool DragFloat(std::string_view label, float* value, float speed = 1.0f, float min = -FLT_MAX, float max = FLT_MAX, bool sameLine = false);
        static bool DragVector3(std::string_view label, Vector3* value, float speed = 1.0f, float min = -FLT_MAX, float max = FLT_MAX, bool sameLine = false);
        static bool DragVector4(std::string_view label, Vector4* value, float speed = 1.0f, float min = -FLT_MAX, float max = FLT_MAX, bool sameLine = false);

        static bool SliderInt(std::string_view label, int* value, int min, int max, bool sameLine = false);
        static bool SliderFloat(std::string_view label, float* value, float min, float max, bool sameLine = false);

        static bool CoorPicker3(std::string_view label, Color* color, bool sameLine = false);
        static bool CoorPicker4(std::string_view label, Color* color, bool sameLine = false);
        static bool ColorInput3(std::string_view label, Color* color, bool preview = true, bool sameLine = false);
        static bool ColorInput4(std::string_view label, Color* color, bool preview = true, bool sameLine = false);
        static bool ColorBar3(std::string_view label, Color* color);
        static bool ColorBar4(std::string_view label, Color* color);
    };
}
