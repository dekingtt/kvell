#ifndef __KVELL_SINGLETON_H__
#define __KVELL_SINGLETON_H__
#include <memory>

namespace kvell
{
    template <class T, class X, int n>
    T &GetInstanceX()
    {
        static T v;
        return v;
    }

    template <class T, class X, int N>
    std::shared_ptr<T> GetInstancePrt()
    {
        static std::shared_ptr<T> v(new T);
        return v;
    }

    template <class T, class X = void, int N = 0>
    class Singleton
    {
    public:
        static T *GetInstance()
        {
            static T v;
            return &v;
        }
    };

    template <class T, class X = void, int N = 0>
    class SingletonPtr
    {
        static std::shared_ptr<T> GetInstance()
        {
            static std::shared_ptr<T> v(new T);
            return v;
        }
    };
} // namespace kvell

#endif