#pragma once
#include <unordered_map>
#include <bitset>

namespace Glib
{
    class Layer2D
    {
        using uint = unsigned int;
    public:
        Layer2D()
        {
            for (uint i = 0; i < 32; ++i)
            {
                layerMap_[i].set();
            }
        }

        void Set(uint layer1, uint layer2, bool enable)
        {
            if (layer1 > 32 || layer2 > 32) return;
            layerMap_[layer1][static_cast<uint>(layer2)] = enable;
            layerMap_[layer2][static_cast<uint>(layer1)] = enable;
        }

        bool Validate(uint layer1, uint layer2)
        {
            if (layer1 > 32 || layer2 > 32) return false;
            return layerMap_[layer1][static_cast<uint>(layer2)];
        }
    private:
        std::unordered_map<uint, std::bitset<32>> layerMap_;
    };
}
