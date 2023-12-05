#pragma once
#include <vector>
#include <string>

namespace Glib
{
    /**
     * @brief 3D�I�u�W�F�N�g�f�[�^�N���X
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
            int indexStart;
            int indecCount;
            int material;
        };

        struct Material
        {
            float       ambient[4];
            float       diffuse[4];
            float       specular[4];
            float       shininess;
            int         texLength;
            std::string texture;
            int         normalLength;
            std::string normal;
        };

        struct Bone
        {
            int         boneNameLength;
            std::string bonename;
            float       translate[3];
            int         parent;
        };
#pragma pack(pop)

    public:
        GLObject() = default;

        /**
         * @brief �I�u�W�F�N�g���쐬
         * @param vertex ���_
         * @param indices �C���f�b�N�X
         * @param subsets �T�u�Z�b�g
         * @param materials �}�e���A��
         * @param bones
         */
        GLObject(
            const std::vector<Vertex>& vertex,
            const std::vector<unsigned int>& indices,
            const std::vector<Subset>& subsets,
            const std::vector<Material>& materials,
            const std::vector<Bone>& bones);

        /**
         * @brief �t�@�C���̓ǂݍ���
         * @param path
         * @return ���� : true
         * @return ���s : false
         */
        bool ReadFile(std::string_view path);

        /**
         * @brief �t�@�C���֏�������
         * @param path
         * @return ���� : true
         * @return ���s : false
         */
        bool WriteFile(const std::string& path);

    public:
        /**
         * @brief ���_
         */
        std::vector<Vertex>& Vertices();

        /**
         * @brief �C���f�b�N�X
         */
        std::vector<unsigned int>& Indices();

        /**
         * @brief �T�u�Z�b�g
         */
        std::vector<Subset>& Subsets();

        /**
         * @brief �}�e���A��
         */
        std::vector<Material>& Materials();

        /**
         * @brief �{�[��
         */
        std::vector<Bone>& Bones();

    private:
        // == �e��ǂݍ��ݗp�֐� == //

        void ReadHeader(std::ifstream& stream);
        void ReadVertex(std::ifstream& stream);
        void ReadIndex(std::ifstream& stream);
        void ReadSubset(std::ifstream& stream);
        void ReadMaterial(std::ifstream& stream);
        void ReadBone(std::ifstream& stream);

        // == �e�폑�����ݗp�֐� == //

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
