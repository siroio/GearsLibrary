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
         * @brief オブジェクトの取得
         */
        T* Get();

        /**
         * @brief オブジェクトの返却
         */
        void Release(T*& resource);

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
        size_t AvaiavbleCount() const;

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
        ObjectPool(const ObjectPool<T>&) = delete;
        ObjectPool<T>& operator = (const ObjectPool<T>&) = delete;

    private:
        /* メンバ変数 */

        bool initialized{ false };
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
        initialized = true;
        return true;
    }

    template<class T>
    inline T* ObjectPool<T>::Get()
    {
        std::lock_guard lock{ mutex_ };
        if (!initialized) return nullptr;
        if (availableObjects_.empty()) AddObject(objects_.size());
        T* obj = reinterpret_cast<T*>(availableObjects_.front());
        availableObjects_.pop_front();
        borrowedObjects_.emplace(reinterpret_cast<uintptr_t>(obj));
        return obj;
    }

    template<class T>
    inline void ObjectPool<T>::Release(T*& object)
    {
        std::lock_guard lock{ mutex_ };
        if (!initialized) return;
        auto it = borrowedObjects_.find(reinterpret_cast<uintptr_t>(object));
        if (it != borrowedObjects_.end())
        {
            availableObjects_.push_back(*it);
            borrowedObjects_.erase(it);
            object = nullptr;
        }
    }

    template<class T>
    inline void Glib::ObjectPool<T>::Clear()
    {
        std::lock_guard lock{ mutex_ };
        if (!initialized) return;
        objects_.clear();
        availableObjects_.clear();
        borrowedObjects_.clear();
        callback_ = nullptr;
        initialized = false;
    }

    template<class T>
    inline size_t ObjectPool<T>::Count() const
    {
        std::lock_guard lock{ mutex_ };
        if (!initialized) return 0;
        return objects_.size();
    }

    template<class T>
    inline size_t ObjectPool<T>::UseCount() const
    {
        std::lock_guard lock{ mutex_ };
        if (!initialized) return 0;
        return borrowedObjects_.count();
    }

    template<class T>
    inline size_t ObjectPool<T>::AvaiavbleCount() const
    {
        std::lock_guard lock{ mutex_ };
        if (!initialized) return 0;
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
