#pragma once
#include <string>
#include <vector>
#include <IConverter.h>
#include <mcMath.h>

/**
 * @brief PMXエンコード
*/
enum class EncodeType
{
    UTF16,
    UTF8
};

/**
 * @brief ヘッダー
 */
struct PmxHeader
{
    float version{};
    unsigned char length{};
    EncodeType encode;
    std::vector<unsigned char> Info;
};

/**
 * @brief ウェイトタイプ
 */
enum class WeightType
{
    BDEF1 = 0,
    BDEF2 = 1,
    BDEF4 = 2,
    SDEF = 3
};

/**
 * @brief ウェイト
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
 * @brief IKリンク
 */
struct IKLink
{
    int index{};
    bool existAngleLimited{};
    Vector3f limitAngleMin;
    Vector3f limitAngleMax;
};

/**
 * @brief 頂点データ
 */
struct PmxVertex
{
    // 頂点座標
    Vector3f position;
    // 法線
    Vector3f normal;
    // uv座標
    Vector2f uv;
    // 追加uv座標
    std::vector<Vector2f> additionalUV;
    // ボーンウェイト
    Weight weight;
};

/**
 * @brief 頂点インデックス
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
 * @brief ボーン
*/
struct PmxBone
{
    // ボーン名
    std::string name;
    // ボーン名(英名)
    std::string nameBoneEng;
    // 位置
    Vector3f position;
    // 親
    int parentBoneIndex{};
    // 変形階層
    int transformHierarchy{};
    // ボーンフラグ
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
 * @brief マテリアル
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
