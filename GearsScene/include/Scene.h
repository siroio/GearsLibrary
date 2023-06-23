#ifndef GEARS_SCENE_H
#define GEARS_SCENE_H

namespace Glib::Scene
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

#endif // !GEARS_SCENE_H
