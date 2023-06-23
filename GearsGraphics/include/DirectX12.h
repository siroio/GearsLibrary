#ifndef GEARS_DIRECTX_12_H
#define GEARS_DIRECTX_12_H

#include <Singleton.h>
#include <Internal/ISystem.h>

namespace Glib
{
    class DirectX12 :
        Glib::SingletonPtr<DirectX12>,
        Glib::Internal::Interface::ISystem
    {

    };
}

#endif // !GEARS_DIRECTX_12_H
