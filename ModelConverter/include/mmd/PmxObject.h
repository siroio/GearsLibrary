#pragma once
#include <string>
#include <vector>
#include <IModel.h>
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
    float version;
    unsigned char length;
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
    int index;
    bool existAngleLimited;
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
    int parentBoneIndex;
    // 変形階層
    int transformHierarchy;
    // ボーンフラグ
    unsigned int flag;

    Vector3f offset;
    int childrenIndex;
    int impartParentIndex;
    float impartRate;
    Vector3f fixedAxis;
    Vector3f localAxisX;
    Vector3f localAxisZ;
    int externalParentKey;
    int ikTargetIndex;
    int ikLoopCount;
    float ikUnitAngle;
    std::vector<IKLink> ikLinks;
};

enum BoneFlagMask
{
    // 接続先
    ACCESS_POINT = 0x0001,
    // IK
    IK = 0x0020,
    // 回転付与
    IMPART_ROTATION = 0x0100,
    // 移動付与
    IMPART_TRANSLATION = 0x0200,
    // 軸固定
    AXIS_FIXING = 0x0400,
    // ローカル軸
    LOCAL_AXIS = 0x0800,
    // 外部親変形
    EXTERNAL_PARENT_TRANS = 0x2000,
};

/**
 * @brief マテリアル
 */
struct PmxMaterial
{
    Vector4f diffuse;
    Vector3f specular;
    float specularFactor;
    Vector3f ambient;

    int textureIndex;
    int toonTextureIndex;
    int vertexNum;
};


class PmxModel : public IModel
{
public:
    virtual bool LoadModel(std::string_view path) override;
    virtual bool WriteModel(std::string_view path) override;

private:
    std::vector<PmxVertex> vertices;
    std::vector<PmxSurface> surfaces;
    std::vector<std::string> texturePath;
    std::vector<PmxMaterial> materials;
    std::vector<PmxBone> bones;
};
