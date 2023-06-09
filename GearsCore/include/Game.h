#ifndef GEARS_GAME_H
#define GEARS_GAME_H

/**
 * @brief メインのゲームクラス
 */
class Game
{
protected:
    Game() = default;
    virtual ~Game() = default;

    /**
     * @brief 開始処理
     */
    virtual void Start() = 0;

    /**
     * @brief 終了処理
     */
    virtual void End() = 0;

public:
    /**
     * @brief 実行
     */
    int Run();

    /**
     * @brief 終了
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
