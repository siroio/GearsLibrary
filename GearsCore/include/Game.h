#pragma once

namespace Glib
{
    /**
 * @brief メインゲームクラス
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
        /**
         * @brief システムの登録
        */
        void RegisterSystem();

        /**
         * @brief 初期化
        */
        bool Initialize();

        /**
         * @brief 更新
        */
        void Update();

        /**
         * @brief 描画
        */
        void Draw();

        /**
         * @brief 終了処理
        */
        void Finalize();

    private:
        inline static bool isExit{ false };
    };
}

