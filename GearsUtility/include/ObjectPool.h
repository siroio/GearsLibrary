#pragma once
#include <vector>
#include <list>
#include <unordered_set>
#include <memory>
#include <mutex>
#include <functional>

namespace Glib
{
    /**
     * @brief オブジェクトプールクラス
     */
    template<class T>
    class ObjectPool
    {
    public:
        using InitializeCallBack = std::function<void(size_t, T*)>;

        /* コンストラクタ */
        ObjectPool() = default;

        /**
         * @brief 初期化
        */
        bool Init(size_t count);

        /**
         * @brief オブジェクトの再初期化
         */
        bool Init(InitializeCallBack init);

        /**
         * @brief オブジェクトの取得
         */
        T* Get();

        /**
         * @brief オブジェクトの返却
         */
        void Release(T*& object);

        /**
         * @brief プールの拡張
         * @param expandSize 拡張量
         */
        void Expand(size_t expandSize);

        /**
         * @brief プールの削除
         */
        void Clear();

        /**
         * @brief オブジェクトの総数
         */
        size_t Count() const;

        /**
         * @brief 利用中オブジェクトの総数
         */
        size_t UseCount() const;

        /**
         * @brief 利用可能オブジェクトの総数
         */
        size_t ValidCount() const;

        /**
         * @brief コールバック関数の設定
         * @param callback
        */
        void SetInitializeCallBack(InitializeCallBack callback);

    private:
        /**
         * @brief オブジェクトの追加
         */
        void AddObject(size_t index);

    private:
        ObjectPool(const ObjectPool&) = delete;
        ObjectPool& operator = (const ObjectPool&) = delete;

    private:
        /* メンバ変数 */

        bool initialized_{ false };
        std::mutex mutex_;

        /* プールリスト */

        std::vector<std::unique_ptr<T>> objects_;
        std::list<uintptr_t> availableObjects_;
        std::unordered_set<uintptr_t> borrowedObjects_;
        InitializeCallBack callback_;
    };

    template<class T>
    inline bool ObjectPool<T>::Init(size_t count)
    {
        std::lock_guard lock{ mutex_ };
        objects_.clear();
        availableObjects_.clear();
        borrowedObjects_.clear();
        objects_.reserve(count);
        for (size_t i = 0; i < count; i++)
        {
            AddObject(i);
        }
        initialized_ = true;
        return true;
    }

    template<class T>
    inline bool ObjectPool<T>::Init(InitializeCallBack init)
    {
        if (!init) return false;
        for (size_t i = 0; i < objects_.size(); i++)
        {
            init(i, objects_[i].get());
        }

        return true;
    }

    template<class T>
    inline T* ObjectPool<T>::Get()
    {
        std::lock_guard lock{ mutex_ };
        if (!initialized_) return nullptr;
        if (availableObjects_.empty()) AddObject(objects_.size());
        uintptr_t obj = availableObjects_.front();
        availableObjects_.pop_front();
        borrowedObjects_.emplace(obj);
        OutputDebugStringA(std::to_string(borrowedObjects_.size()).c_str());
        return reinterpret_cast<T*>(obj);
    }

    template<class T>
    inline void ObjectPool<T>::Release(T*& object)
    {
        std::lock_guard lock{ mutex_ };
        if (!initialized_) return;
        const auto it = borrowedObjects_.find(reinterpret_cast<uintptr_t>(object));
        if (it != borrowedObjects_.end())
        {
            availableObjects_.push_back(*it);
            borrowedObjects_.erase(it);
            object = nullptr;
        }
    }

    template<class T>
    inline void ObjectPool<T>::Expand(size_t expandSize)
    {
        const auto size = objects_.size();
        for (size_t i = 0; i < expandSize; i++)
        {
            AddObject(size + i);
        }
    }

    template<class T>
    inline void Glib::ObjectPool<T>::Clear()
    {
        std::lock_guard lock{ mutex_ };
        if (!initialized_) return;
        objects_.clear();
        availableObjects_.clear();
        borrowedObjects_.clear();
        callback_ = nullptr;
        initialized_ = false;
    }

    template<class T>
    inline size_t ObjectPool<T>::Count() const
    {
        if (!initialized_) return 0;
        return objects_.size();
    }

    template<class T>
    inline size_t ObjectPool<T>::UseCount() const
    {
        if (!initialized_) return 0;
        return borrowedObjects_.size();
    }

    template<class T>
    inline size_t ObjectPool<T>::ValidCount() const
    {
        if (!initialized_) return 0;
        return availableObjects_.size();
    }

    template<class T>
    inline void ObjectPool<T>::SetInitializeCallBack(InitializeCallBack callback)
    {
        std::lock_guard lock{ mutex_ };
        callback_ = callback;
    }

    template<class T>
    inline void ObjectPool<T>::AddObject(size_t index)
    {
        auto obj = std::make_unique<T>();
        if (callback_) callback_(index, obj.get());
        objects_.push_back(std::move(obj));
        availableObjects_.push_back(reinterpret_cast<uintptr_t>(objects_.back().get()));
    }
}
