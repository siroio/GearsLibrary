#pragma once
#include <memory>
#include <string>
#include <concepts>
#include <WeakPtr.h>
#include <Internal/ISystem.h>
#include <Singleton.h>
#include <functional>

namespace Glib
{
    class Texture;
    class Mesh;
    class Animation;

    class Resource final :
        public Internal::Interface::ISystem,
        public SingletonPtr<Resource>
    {
        Resource() = default;
        friend WeakPtr<Resource> SingletonPtr<Resource>::Instance();

    public:
        bool Initialize();

        template<class T, class... Args>
        static WeakPtr<T> Load(Args... args);

    private:
        WeakPtr<Texture> LoadTex(std::string_view filePath);
        WeakPtr<Mesh> LoadMesh(std::string_view filePath);
        WeakPtr<Animation> LoadAnim(std::string_view filePath);
    };

    template<class T, class ...Args>
    inline WeakPtr<T> Resource::Load(Args ...args)
    {
        throw std::exception("not implementation.");
    }

    template<>
    inline WeakPtr<Texture> Resource::Load(std::string_view filePath)
    {
        return Instance()->LoadTex(filePath);
    }

    template<>
    inline WeakPtr<Mesh> Resource::Load(std::string_view filePath)
    {
        return Instance()->LoadMesh(filePath);
    }

    template<>
    inline WeakPtr<Animation> Resource::Load(std::string_view filePath)
    {
        return Instance()->LoadAnim(filePath);
    }
}
