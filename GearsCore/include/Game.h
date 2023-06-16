#ifndef GEARS_GAME_H
#define GEARS_GAME_H

/**
 * @brief ���C���̃Q�[���N���X
 */
class Game
{
protected:
    Game() = default;
    virtual ~Game() = default;

    /**
     * @brief �J�n����
     */
    virtual void Start() = 0;

    /**
     * @brief �I������
     */
    virtual void End() = 0;

public:
    /**
     * @brief ���s
     */
    int Run();

    /**
     * @brief �I��
     */
    inline static void Exit()
    {
        isExit = true;
    }

private:
    void RegisterSystem();
    bool Initialize();
    void Update();
    void Draw();
    void Finalize();

private:
    inline static bool isExit{ false };
};

#endif // !GEARS_GAME_H