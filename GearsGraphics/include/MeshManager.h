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
        bool Load(unsigned int id, std::string_view path);

        /**
         * @brief メッシュがロードされているか調べる
         * @param id
         */
        bool Contains(unsigned int id) const;

        /**
         * @brief メッシュのボーンを取得
         * @param id
         */
        const std::vector<Bone>& Bone(unsigned int id);

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

