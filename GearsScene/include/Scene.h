#pragma once

namespace Glib
{
    /**
     * @brief シーンクラス
     */
    class Scene
    {
    protected:
        Scene() = default;

    public:
        virtual ~Scene() = default;

    public:
        /**
         * @brief シーン開始
         */
        virtual void Start() = 0;

        /**
         * @brief シーンの終了
         */
        virtual void End() = 0;
    };
}
