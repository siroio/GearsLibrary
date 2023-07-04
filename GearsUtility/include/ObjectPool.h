#pragma once
#include <list>
#include <memory>
#include <mutex>
#include <functional>

namespace Glib
{
    template<class T>
    class ObjectPool
    {
    public:
        /* �R���X�g���N�^ */
        ObjectPool(size_t capacity = 32);

        /**
         * @brief �I�u�W�F�N�g�̎擾
         */
        std::shared_ptr<T> Get();

        /**
         * @brief �I�u�W�F�N�g�̕ԋp
         */
        void Release(std::shared_ptr<T>& resource);

        /**
         * @brief �v�[���̍폜
         */
        void Clear();

        /**
         * @brief �e�ʂ̕ύX
         */
        void Capacity(size_t capacity);

        /**
         * @brief �v�[���̗e��
         */
        size_t Capacity() const;

        /**
         * @brief �v�[�����g�����邩
         */
        bool AutoExpand() const;

        /**
         * @brief �v�[���̊g����؂�ւ�
         */
        void AutoExpand(bool expand);

        /**
         * @brief �g����I�u�W�F�N�g�̐�
         */
        size_t Count() const;

        /**
         * @brief �v�[�����g���邩
         */
        bool PoolEnable() const;

    private:
        /**
         * @brief ���\�[�X�̒ǉ�
         */
        void AddResource();

    private:
        ObjectPool(const ObjectPool<T>&) = delete;
        ObjectPool<T>& operator = (const ObjectPool<T>&) = delete;

    private:
        /* �����o�ϐ� */

        size_t capacity_{ 32 };
        bool autoExpand_{ true };
        std::mutex mutex_;

        /* �v�[�����X�g */

        std::list<std::shared_ptr<T>> pool_;
        std::list<std::shared_ptr<T>> used_;
        std::list<std::shared_ptr<T>> free_;
    };

    template<class T>
    inline ObjectPool<T>::ObjectPool(size_t capacity) :
        capacity_{ capacity }
    {
        std::lock_guard lock{ mutex_ };
        for (size_t i = 0; i < capacity_; i++)
        {
            // ������
            AddResource();
        }
    }

    template<class T>
    inline std::shared_ptr<T> ObjectPool<T>::Get()
    {
        std::lock_guard<std::mutex> lock{ mutex_ };
        // �擾
        if (autoExpand_ && pool_.size() >= capacity_)
        {
            // �g��
            AddResource();
            capacity_++;
            // ���p��Ԃɂ���
            used_.push_back(pool_.back());
        }
        else
        {
            used_.push_back(free_.back());
            free_.pop_back();
        }
        return used_.back();
    }

    template<class T>
    inline void ObjectPool<T>::Release(std::shared_ptr<T>& object)
    {
        std::lock_guard<std::mutex> lock{ mutex_ };
        // �ԋp
        used_.remove(*object);
        free_.push_back(object);
    }

    template<class T>
    inline void Glib::ObjectPool<T>::Clear()
    {
        std::lock_guard<std::mutex> lock{ mutex_ };
        // �폜
        pool_.clear();
        capacity_ = 0;
    }

    template<class T>
    inline void Glib::ObjectPool<T>::Capacity(size_t capacity)
    {
        std::lock_guard<std::mutex> lock{ mutex_ };
        if (capacity_ >= capacity) return;
        capacity_ = capacity;
    }

    template<class T>
    inline size_t Glib::ObjectPool<T>::Capacity() const
    {
        return capacity_;
    }

    template<class T>
    inline bool ObjectPool<T>::AutoExpand() const
    {
        return autoExpand_;
    }

    template<class T>
    inline void ObjectPool<T>::AutoExpand(bool expand)
    {
        std::lock_guard<std::mutex> lock{ mutex_ };
        autoExpand_ = expand;
    }

    template<class T>
    inline size_t ObjectPool<T>::Count() const
    {
        return used_.size();
    }

    template<class T>
    inline bool Glib::ObjectPool<T>::PoolEnable() const
    {
        std::lock_guard<std::mutex> lock{ mutex_ };
        return capacity_ > 0 || pool_.size() > 0 || !pool_.empty();
    }

    template<class T>
    inline void ObjectPool<T>::AddResource()
    {
        auto shared = std::make_shared<T>();
        pool_.push_back(shared);
    }
}
