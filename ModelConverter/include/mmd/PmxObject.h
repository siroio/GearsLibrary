#pragma once
#include <string>
#include <vector>
#include <IModel.h>


struct PmxHeader
{
    char pmx[4];
    float version;
    unsigned char headerSize;
    unsigned char encoding;
    unsigned char additionalUV;
    unsigned char vertexIndexSize;
    unsigned char textureIndexSize;
    unsigned char materialIndexSize;
    unsigned char boneIndexSize;
    unsigned char morpthIndexSize;
    unsigned char rigidbodyIndexSize;
};

struct PmxVertex
{

};

struct PmxMaterial
{
    std::string name;
};

struct PmxBone
{
    std::string name;
    float position[3];
    int parentBoneIndex;
};

class PmxModel : public IModel<PmxModel>
{
    void LoadModel(const std::string_view path) override;
    const PmxModel& GetData() const override;
};
