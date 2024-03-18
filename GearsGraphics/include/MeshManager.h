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
         * @brief メッシュを読み込む
         * @param id
         * @param path
         * @return
         */
        static bool Load(unsigned int id, std::string_view path);

        /**
         * @brief メッシュがロードされているか調べる
         * @param id
         */
        static bool Contains(unsigned int id);

        /**
         * @brief メッシュのボーンを取得
         * @param id
         */
        static const std::vector<Bone>& Bone(unsigned int id);

        /**
         * @brief メッシュを取得
         * @param id
        */
        static const Mesh& Mesh(unsigned int id);

        /**
         * @brief 描画
         */
        void Draw(unsigned int id) const;

        /**
         * @brief 影を描画
         */
        void DrawShadow(unsigned int id) const;

    };
}
