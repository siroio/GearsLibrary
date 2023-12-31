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
     * @brief ImGui���b�p�[�N���X
     */
    class GLGUI
    {
    public:
        /**
         * @brief �Ԋu���J����
         */
        static void Space();

        /**
         * @brief �Z�p���[�^�̒ǉ�
         */
        static void Separator();

        /**
         * @brief �e�L�X�g�̕\��
         * @param text
         */
        static void Text(std::string_view fmt, ...);

        /**
         * @brief �{�^����\��
         * @param label �\����
         * @param sameLine �����s�ɕ\�������邩
         * @return ��������
         */
        static bool Button(std::string_view label, bool sameLine = false);

        /**
         * @brief �`�F�b�N�{�b�N�X�̕\��
         * @param label �\����
         * @param checked �`�F�b�N���
         * @param sameLine �����s�ɕ\�������邩
         * @return ��������
         */
        static bool CheckBox(std::string_view label, bool* checked, bool sameLine = false);

        /**
         * @brief �h���b�v�_�E�����X�g�̕\��
         * @param label �\����
         * @param curerntItem �I���A�C�e��
         * @param items �I����
         * @return ��������
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
