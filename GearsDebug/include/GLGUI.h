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

        /**
        * @brief 2Dベクトルを入力するボックスを表示
        * @param label 表示名
        * @param value 2Dベクトルの値
        * @param sameLine 同じ行に表示させるか
        * @return 入力が変更されたかどうか
        */
        static bool InputVector2(std::string_view label, Vector2* value, bool sameLine = false);

        /**
         * @brief 3Dベクトルを入力するボックスを表示
         * @param label 表示名
         * @param value 3Dベクトルの値
         * @param sameLine 同じ行に表示させるか
         * @return 入力が変更されたかどうか
         */
        static bool InputVector3(std::string_view label, Vector3* value, bool sameLine = false);

        /**
         * @brief 4Dベクトルを入力するボックスを表示
         * @param label 表示名
         * @param value 4Dベクトルの値
         * @param sameLine 同じ行に表示させるか
         * @return 入力が変更されたかどうか
         */
        static bool InputVector4(std::string_view label, Vector4* value, bool sameLine = false);

        /**
         * @brief 整数値をドラッグで変更するスライダーを表示
         * @param label 表示名
         * @param value 整数値のポインタ
         * @param speed ドラッグのスピード
         * @param min 最小値
         * @param max 最大値
         * @param sameLine 同じ行に表示させるか
         * @return 値が変更されたかどうか
         */
        static bool DragInt(std::string_view label, int* value, float speed = 1.0f, int min = INT_MIN, int max = INT_MAX, bool sameLine = false);

        /**
         * @brief 浮動小数点数をドラッグで変更するスライダーを表示
         * @param label 表示名
         * @param value 浮動小数点数のポインタ
         * @param speed ドラッグのスピード
         * @param min 最小値
         * @param max 最大値
         * @param sameLine 同じ行に表示させるか
         * @return 値が変更されたかどうか
         */
        static bool DragFloat(std::string_view label, float* value, float speed = 1.0f, float min = -FLT_MAX, float max = FLT_MAX, bool sameLine = false);

        /**
         * @brief 2Dベクトルをドラッグで変更するスライダーを表示
         * @param label 表示名
         * @param value 2Dベクトルのポインタ
         * @param speed ドラッグのスピード
         * @param min 最小値
         * @param max 最大値
         * @param sameLine 同じ行に表示させるか
         * @return 値が変更されたかどうか
         */
        static bool DragVector2(std::string_view label, Vector2* value, float speed = 1.0f, float min = -FLT_MAX, float max = FLT_MAX, bool sameLine = false);

        /**
         * @brief 3Dベクトルをドラッグで変更するスライダーを表示
         * @param label 表示名
         * @param value 3Dベクトルのポインタ
         * @param speed ドラッグのスピード
         * @param min 最小値
         * @param max 最大値
         * @param sameLine 同じ行に表示させるか
         * @return 値が変更されたかどうか
         */
        static bool DragVector3(std::string_view label, Vector3* value, float speed = 1.0f, float min = -FLT_MAX, float max = FLT_MAX, bool sameLine = false);

        /**
         * @brief 4Dベクトルをドラッグで変更するスライダーを表示
         * @param label 表示名
         * @param value 4Dベクトルのポインタ
         * @param speed ドラッグのスピード
         * @param min 最小値
         * @param max 最大値
         * @param sameLine 同じ行に表示させるか
         * @return 値が変更されたかどうか
         */
        static bool DragVector4(std::string_view label, Vector4* value, float speed = 1.0f, float min = -FLT_MAX, float max = FLT_MAX, bool sameLine = false);

        /**
         * @brief 整数値をスライダーで変更するコントロールを表示
         * @param label 表示名
         * @param value 整数値のポインタ
         * @param min 最小値
         * @param max 最大値
         * @param sameLine 同じ行に表示させるか
         * @return 値が変更されたかどうか
         */
        static bool SliderInt(std::string_view label, int* value, int min, int max, bool sameLine = false);

        /**
         * @brief 浮動小数点数をスライダーで変更するコントロールを表示
         * @param label 表示名
         * @param value 浮動小数点数のポインタ
         * @param min 最小値
         * @param max 最大値
         * @param sameLine 同じ行に表示させるか
         * @return 値が変更されたかどうか
         */
        static bool SliderFloat(std::string_view label, float* value, float min, float max, bool sameLine = false);

        /**
         * @brief RGBカラーをピッカーで変更するコントロールを表示
         * @param label 表示名
         * @param color カラーのポインタ
         * @param sameLine 同じ行に表示させるか
         * @return 値が変更されたかどうか
         */
        static bool ColorPicker3(std::string_view label, Color* color, bool sameLine = false);

        /**
         * @brief RGBAカラーをピッカーで変更するコントロールを表示
         * @param label 表示名
         * @param color カラーのポインタ
         * @param sameLine 同じ行に表示させるか
         * @return 値が変更されたかどうか
         */
        static bool ColorPicker4(std::string_view label, Color* color, bool sameLine = false);

        /**
         * @brief RGBカラーを入力するコントロールを表示
         * @param label 表示名
         * @param color カラーのポインタ
         * @param preview プレビューを表示するか
         * @param sameLine 同じ行に表示させるか
         * @return 値が変更されたかどうか
         */
        static bool ColorInput3(std::string_view label, Color* color, bool preview = true, bool sameLine = false);

        /**
         * @brief RGBAカラーを入力するコントロールを表示
         * @param label 表示名
         * @param color カラーのポインタ
         * @param preview プレビューを表示するか
         * @param sameLine 同じ行に表示させるか
         * @return 値が変更されたかどうか
         */
        static bool ColorInput4(std::string_view label, Color* color, bool preview = true, bool sameLine = false);

        /**
         * @brief RGBカラーをバーで表示するコントロールを表示
         * @param label 表示名
         * @param color カラーのポインタ
         * @return 値が変更されたかどうか
         */
        static bool ColorBar3(std::string_view label, Color* color);

        /**
         * @brief RGBAカラーをバーで表示するコントロールを表示
         * @param label 表示名
         * @param color カラーのポインタ
         * @return 値が変更されたかどうか
         */
        static bool ColorBar4(std::string_view label, Color* color);

        /**
         * @brief テキスト入力ボックスを表示
         * @param label ラベル
         * @param buf バッファ
         * @param bufSize バッファサイズ
         * @param multiline 複数行
         * @param multiline 読み取り専用
         * @param multiline 伏せ字
         * @return エンターキーで確定されたか
         */
        static bool InputText(std::string_view label, char* buf, size_t bufSize, bool multiline, bool readOnly, bool password);

        /**
         * @brief 進捗バーを表示
         * @param fraction 進捗率
         * @param size サイズ
         * @param overlay オーバーレイテキスト
         */
        static void ProgressBar(float fraction, const Vector2& size = Vector2{ -FLT_MIN, 0 }, std::string_view overlay = "");

        /**
         * @brief トグルスイッチを表示
         * @param label ラベル
         * @param v トグル状態
         * @param sameLine 同じ行に表示させるか
         * @return トグル状態
         */
        static bool ToggleSwitch(std::string_view label, std::string_view enableText = "On", std::string_view disableText = "Off", bool* toggled, bool sameLine = false);

        /**
         * @brief ツールチップを表示
         * @param text ツールチップテキスト
         */
        static void Tooltip(std::string_view text);
    };
}
