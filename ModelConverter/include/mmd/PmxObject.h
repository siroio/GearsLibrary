#pragma once
#include <string>
#include <vector>
#include <IConverter.h>
#include <mcMath.h>

/**
 * @brief PMX�G���R�[�h
*/
enum class EncodeType
{
    UTF16,
    UTF8
};

/**
 * @brief �w�b�_�[
 */
struct PmxHeader
{
    float version{};
    unsigned char length{};
    EncodeType encode;
    std::vector<unsigned char> Info;
};

/**
 * @brief �E�F�C�g�^�C�v
 */
enum class WeightType
{
    BDEF1 = 0,
    BDEF2 = 1,
    BDEF4 = 2,
    SDEF = 3
};

/**
 * @brief �E�F�C�g
 */
struct Weight
{
    WeightType type;
    union
    {
        struct
        {
            int bone1;
            int bone2;
            int bone3;
            int bone4;
        };
        int bones[4];
    };

    union
    {
        struct
        {
            float weight1;
            float weight2;
            float weight3;
            float weight4;
        };
        float weights[4];
    };


    Vector3f c;
    Vector3f r0;
    Vector3f r1;
};

/**
 * @brief IK�����N
 */
struct IKLink
{
    int index{};
    bool existAngleLimited{};
    Vector3f limitAngleMin;
    Vector3f limitAngleMax;
};

/**
 * @brief ���_�f�[�^
 */
struct PmxVertex
{
    // ���_���W
    Vector3f position;
    // �@��
    Vector3f normal;
    // uv���W
    Vector2f uv;
    // �ǉ�uv���W
    std::vector<Vector2f> additionalUV;
    // �{�[���E�F�C�g
    Weight weight;
};

/**
 * @brief ���_�C���f�b�N�X
 */
struct PmxSurface
{
    int vertexIndex;
};

enum class PmxBoneFlag : unsigned int
{
    TargetShowMode = 0x0001,
    AllowRotate = 0x0002,
    AllowTranslate = 0x0004,
    Visible = 0x0008,
    AllowControl = 0x0010,
    IK = 0x0020,
    AppendLocal = 0x0080,
    AppendRotate = 0x0100,
    AppendTranslate = 0x0200,
    FixedAxis = 0x0400,
    LocalAxis = 0x800,
    DeformAfterPhysics = 0x1000,
    DeformOuterParent = 0x2000,
};

/**
 * @brief �{�[��
*/
struct PmxBone
{
    // �{�[����
    std::string name;
    // �{�[����(�p��)
    std::string nameBoneEng;
    // �ʒu
    Vector3f position;
    // �e
    int parentBoneIndex{};
    // �ό`�K�w
    int transformHierarchy{};
    // �{�[���t���O
    PmxBoneFlag flag;

    Vector3f offset;
    int childrenIndex{};
    int impartParentIndex{};
    float impartRate{};
    Vector3f fixedAxis;
    Vector3f localAxisX;
    Vector3f localAxisZ;
    int externalParentKey{};
    int ikTargetIndex{};
    int ikLoopCount{};
    float ikUnitAngle{};
    std::vector<IKLink> ikLinks;
};

/**
 * @brief �}�e���A��
 */
struct PmxMaterial
{
    Vector4f diffuse;
    Vector3f specular;
    float specularFactor{};
    Vector3f ambient;

    int textureIndex{};
    int toonTextureIndex{};
    int vertexNum{};
};


class PmxModel : public IConverter
{
private:
    bool ReadPmxHeader(std::ifstream& pmxFile, PmxHeader& header);
    bool ReadVertices(std::ifstream& pmxFile, const PmxHeader& header);
    bool ReadSurfaces(std::ifstream& pmxFile, const PmxHeader& header);
    bool ReadTextures(std::ifstream& pmxFile, const PmxHeader& header);
    bool ReadMaterials(std::ifstream& pmxFile, const PmxHeader& header);
    bool ReadBones(std::ifstream& pmxFile, const PmxHeader& header);

public:
    virtual bool LoadFile(std::string_view path) override;
    virtual bool WriteFile(std::string_view path) override;

private:
    std::vector<PmxVertex> vertices;
    std::vector<PmxSurface> surfaces;
    std::vector<std::string> texturePath;
    std::vector<PmxMaterial> materials;
    std::vector<PmxBone> bones;
};
