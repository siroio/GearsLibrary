#pragma once
#include <IConverter.h>
#include <mcMath.h>
#include <string>
#include <vector>
#include <unordered_map>

struct KeyFrame
{
    std::string boneName{};
    unsigned int frameNo;
    Vector3f translate;
    Vector4f rotation;
    unsigned char bezier[64];
};

class VmdMotion : public IConverter
{
public:
    virtual bool LoadFile(std::string_view path) override;
    virtual bool WriteFile(std::string_view path) override;

private:
    void ReadKeyFrames(std::ifstream& file);

public:
    std::vector<KeyFrame> keyFrames_;
};
