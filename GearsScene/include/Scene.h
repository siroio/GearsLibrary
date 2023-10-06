#pragma once

namespace Glib
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
