#ifndef GEARS_GAME_H
#define GEARS_GAME_H

class Game
{
protected:
    Game() = default;
    virtual ~Game() = default;

    virtual void Start() = 0;
    virtual void End() = 0;

public:
    int Run();
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
