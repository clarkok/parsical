#ifndef _PARSICAL_COMPARABLE_SET_HPP_
#define _PARSICAL_COMPARABLE_SET_HPP_

#include <set>
#include <cstddef>

namespace parsical {

template <typename T>
class ComparableSet
{
    std::set<T> _set;
public:
    ComparableSet() = default;
    ComparableSet(const ComparableSet<T> &) = default;
    ComparableSet(ComparableSet<T> &&) = default;

    ~ComparableSet() = default;

    inline void
    insert(T item)
    { _set.insert(item); }

    inline bool
    has(T item)
    { return _set.find(item) != _set.end(); }

    inline size_t
    size() const
    { return _set.size(); }

    inline void
    join(const ComparableSet<T> &b)
    { _set.insert(b.begin(), b.end()); }

    inline auto
    begin() -> decltype(ComparableSet<T>::_set.begin())
    { return _set.begin(); }

    inline auto
    begin() const -> decltype(ComparableSet<T>::_set.cbegin())
    { return _set.cbegin(); }

    inline auto
    end() -> decltype(ComparableSet<T>::_set.end())
    { return _set.end(); }

    inline auto
    end() const -> decltype(ComparableSet<T>::_set.cend())
    { return _set.cend(); }

    inline bool
    operator < (const ComparableSet<T> &b) const
    {
        auto ia = begin();
        auto ib = b.begin();

        while (ia != end() && ib != b.end()) {
            if (*ia != *ib) {
                return *ia < *ib;
            }
            ++ia; ++ib;
        }

        if (ib != b.end()) {
            return true;
        }
        else {
            return false;
        }
    }

    inline bool
    operator == (const ComparableSet<T> &b) const
    {
        if (size() != b.size()) { return false; }

        auto ia = begin();
        auto ib = b.begin();
        while (ia != end() && ib != b.end()) {
            if (*ia != *ib) {
                return false;
            }
            ++ia; ++ib;
        }

        return (ia == end() && ib == b.end());
    }
};

}

#endif
