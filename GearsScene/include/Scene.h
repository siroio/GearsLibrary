#ifndef GEARS_SCENE_H
#define GEARS_SCENE_H

namespace Glib::Scene
{
    /**
     * @brief �V�[���N���X
     */
    class Scene
    {
    protected:
        Scene() = default;

    public:
        virtual ~Scene() = default;

    public:
        /**
         * @brief �V�[���J�n
         */
        virtual void Start() = 0;

        /**
         * @brief �V�[���̏I��
         */
        virtual void End() = 0;
    };
}

#endif // !GEARS_SCENE_H
