#pragma once
#include <vector>
#include <string>

namespace Glib
{
    /**
     * @brief 3Dオブジェクトデータクラス
     */
    class GLObject final
    {
    public:
#pragma pack(push, 1)
        struct Header
        {
            char signature[9];
            float version{};
            char endian[2];
        };

        struct Vertex
        {
            float   position[3];
            float   normal[3];
            float   uv[2];
            int     boneIndex[4];
            float   boneWeight[4];
            float   tangent[4];
        };

        struct Subset
        {
            int indexStart{};
            int indexCount{};
            int material{};
        };

        struct Material
        {
            float       ambient[4];
            float       diffuse[4];
            float       specular[4];
            float       shininess{};
            std::string texture{ "" };
            std::string normal{ "" };
        };

        struct Bone
        {
            std::string boneName{ "" };
            float       translate[3];
            int         parent{};
        };
#pragma pack(pop)

    public:
        GLObject() = default;

        /**
         * @brief オブジェクトを作成
         * @param vertex 頂点
         * @param indices インデックス
         * @param subsets サブセット
         * @param materials マテリアル
         * @param bones
         */
        GLObject(
            const std::vector<Vertex>& vertex,
            const std::vector<unsigned int>& indices,
            const std::vector<Subset>& subsets,
            const std::vector<Material>& materials,
            const std::vector<Bone>& bones);

        /**
         * @brief ファイルの読み込み
         * @param path
         * @return 成功 : true
         * @return 失敗 : false
         */
        bool ReadFile(std::string_view path);

        /**
         * @brief ファイルへ書き込み
         * @param path
         * @return 成功 : true
         * @return 失敗 : false
         */
        bool WriteFile(const std::string_view path);

    public:
        /**
         * @brief 頂点
         */
        std::vector<Vertex>& Vertices();

        /**
         * @brief インデックス
         */
        std::vector<unsigned int>& Indices();

        /**
         * @brief サブセット
         */
        std::vector<Subset>& Subsets();

        /**
         * @brief マテリアル
         */
        std::vector<Material>& Materials();

        /**
         * @brief ボーン
         */
        std::vector<Bone>& Bones();

    private:
        // == 各種読み込み用関数 == //

        void ReadHeader(std::ifstream& file);
        void ReadVertex(std::ifstream& file);
        void ReadIndex(std::ifstream& file);
        void ReadSubset(std::ifstream& file);
        void ReadMaterial(std::ifstream& file);
        void ReadBone(std::ifstream& file);

        // == 各種書き込み用関数 == //

        void WriteHeader(std::ofstream& file);
        void WriteVertex(std::ofstream& file);
        void WriteIndex(std::ofstream& file);
        void WriteSubset(std::ofstream& file);
        void WriteMaterial(std::ofstream& file);
        void WriteBone(std::ofstream& file);

    private:
        std::string                 signature_{ "" };
        float                       version_{ 1.0f };
        std::string                 endianInfo_{ "" };
        std::vector<Vertex>         vertices_{};
        std::vector<unsigned int>   indices_{};
        std::vector<Subset>         subsets_{};
        std::vector<Material>       materials_{};
        std::vector<Bone>           bones_{};
    };
}

