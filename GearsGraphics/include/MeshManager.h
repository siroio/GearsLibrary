#pragma once
#include <string_view>
#include <unordered_map>
#include <Singleton.h>
#include <Mesh.h>

namespace Glib
{
    class MeshManager : public Singleton<MeshManager>
    {
        MeshManager() = default;
        friend MeshManager& Singleton<MeshManager>::Instance();

    public:

        /**
         * @brief ���b�V����ǂݍ���
         * @param id
         * @param path
         * @return
         */
        bool Load(unsigned int id, std::string_view path);

        /**
         * @brief ���b�V�������[�h����Ă��邩���ׂ�
         * @param id
         */
        bool Contains(unsigned int id);

        /**
         * @brief �`��
         */
        void Draw(unsigned int id);

        /**
         * @brief �e��`��
         */
        void DrawShadow(unsigned int id);

    };
}
