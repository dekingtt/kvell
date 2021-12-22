#ifndef __KVELL_NONCOPYABLE_H__
#define __KVELL_NONCOPYABLE_H__

namespace kvell
{
    class Noncopyable
    {
    public:
        Noncopyable() = default;

        ~Noncopyable() = default;

        Noncopyable(const Noncopyable &) = delete;

        Noncopyable &operator=(const Noncopyable &) = delete;
    };
} // namespace kvell
#endif