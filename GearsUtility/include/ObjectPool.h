#pragma once
#include <vector>
#include <list>
#include <unordered_set>
#include <memory>
#include <mutex>
#include <functional>

namespace Glib
{
    template<class T>
    class ObjectPool
    {
    public:
        using InitializeCallBack = std::function<void(size_t, T*)>;

        /* �R���X�g���N�^ */
        ObjectPool(size_t count = 32);

        /**
         * @brief ������
        */
        bool Init(size_t count);

        /**
         * @brief �I�u�W�F�N�g�̎擾
         */
        T* Get();

        /**
         * @brief �I�u�W�F�N�g�̕ԋp
         */
        void Release(T*& resource);

        /**
         * @brief �v�[���̍폜
         */
        void Clear();

        /**
         * @brief �I�u�W�F�N�g�̑���
         */
        size_t Count() const;

        /**
         * @brief �R�[���o�b�N�֐��̐ݒ�
         * @param callback
        */
        void SetInitializeCallBack(InitializeCallBack callback);

    private:
        /**
         * @brief �I�u�W�F�N�g�̒ǉ�
         */
        void AddObject(size_t index);

    private:
        ObjectPool(const ObjectPool<T>&) = delete;
        ObjectPool<T>& operator = (const ObjectPool<T>&) = delete;

    private:
        /* �����o�ϐ� */

        bool initialized{ false };
        std::mutex mutex_;

        /* �v�[�����X�g */

        std::vector<std::unique_ptr<T>> objects_;
        std::list<T*> availableObjects_;
        std::unordered_set<T*> borrowedObjects_;
        InitializeCallBack callback_;
    };

    template<class T>
    inline ObjectPool<T>::ObjectPool(size_t count)
    {
        Init(count);
    }

    template<class T>
    inline bool ObjectPool<T>::Init(size_t count)
    {
        std::lock_guard lock{ mutex_ };
        for (auto& it : objects_) it.reset(nullptr);
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
        T* obj = availableObjects_.front();
        availableObjects_.pop_front();
        borrowedObjects_.emplace(obj);
        return obj;
    }

    template<class T>
    inline void ObjectPool<T>::Release(T*& object)
    {
        std::lock_guard lock{ mutex_ };
        if (!initialized) return;
        auto it = borrowedObjects_.find(object);
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
        for (auto& it : objects_)
        {
            it.reset(nullptr);
        }

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
        if (!initialized) return -1;
        return objects_.size();
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
        availableObjects_.push_back(objects_.back().get());
    }
}
