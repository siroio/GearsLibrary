﻿#pragma once
#include <Internal/ICamera.h>
#include <Component.h>

namespace Glib::Internal
{
    /**
     * @brief カメラコンポーネントのベースクラス
     */
    class CameraBase : public Component, public Interface::ICamera
    {
    protected:
        CameraBase() = default;
        virtual ~CameraBase() = default;
    };
}

