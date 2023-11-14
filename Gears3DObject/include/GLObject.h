#pragma once
#include <vector>
#include <string>
#include <string_view>

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
            float version;
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
            int startIndex;
            int endIndex;
            int material;
        };

        struct Material
        {
            float       ambient[4];
            float       diffuse[4];
            float       specular[4];
            float       shininess;
            char        texture[256];
            char        normal[256];
        };

        struct Bone
        {
            char        boneName[256];
            float       translate[3];
            int         parent;
        };
#pragma pack(pop)

    public:
        GLObject() = default;

        /**
         * @brief ファイルを読み込む
         * @param path
         */
        GLObject(std::string_view path);

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
        bool WriteFile(std::string_view path);

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

        void ReadHeader(std::ifstream& stream);
        void ReadVertex(std::ifstream& stream);
        void ReadIndex(std::ifstream& stream);
        void ReadSubset(std::ifstream& stream);
        void ReadMaterial(std::ifstream& stream);
        void ReadBone(std::ifstream& stream);

        // == 各種書き込み用関数 == //

        void WriteHeader(std::ofstream& stream);
        void WriteVertex(std::ofstream& stream);
        void WriteIndex(std::ofstream& stream);
        void WriteSubset(std::ofstream& stream);
        void WriteMaterial(std::ofstream& stream);
        void WriteBone(std::ofstream& stream);

    private:
        std::string                 signature_{ "" };
        float                       version_{ 1.0f };
        char                        endianInfo_[2]{ "" };
        std::vector<Vertex>         vertices_{};
        std::vector<unsigned int>   indices_{};
        std::vector<Subset>         subsets_{};
        std::vector<Material>       materials_{};
        std::vector<Bone>           bones_{};
    };
}
