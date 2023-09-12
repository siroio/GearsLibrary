#pragma once
#include <memory>
#include <string>
#include <WeakPtr.h>
#include <Internal/ISystem.h>
#include <Singleton.h>

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
        template<class T, class... Args>
        static WeakPtr<T> Load(Args... args);

    private:
        WeakPtr<Mesh> LoadMesh(unsigned int id, std::string_view filePath);
        WeakPtr<Animation> LoadAnim(unsigned int id, std::string_view filePath);
    };

    template<class T, class ...Args>
    inline WeakPtr<T> Resource::Load(Args ...args)
    {
        throw std::exception("not implementation.");
    }

    template<>
    inline WeakPtr<Mesh> Resource::Load(unsigned int id, std::string_view filePath)
    {
        return Instance()->LoadMesh(id, filePath);
    }

    template<>
    inline WeakPtr<Animation> Resource::Load(unsigned int id, std::string_view filePath)
    {
        return Instance()->LoadAnim(id, filePath);
    }
}
