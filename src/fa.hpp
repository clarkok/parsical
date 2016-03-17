#ifndef _PARSICAL_FA_HPP_
#define _PARSICAL_FA_HPP_

#include <vector>
#include <map>
#include <set>
#include <memory>
#include <cassert>
#include <iostream>

namespace parsical {

class FA
{
public:
    typedef unsigned int state_id_t;
    static const int EMPTY_LINK = -1;

    struct Link
    {
        int ch;
        state_id_t  dst;

        Link(int ch, int dst)
            : ch(ch), dst(dst)
        { }
    };

    struct State
    {
        state_id_t id;
        int accept;

        std::multimap<int, state_id_t> links;

    private:
        State(state_id_t id, int accept)
            : id(id), accept(accept)
        { }

        friend class FA;
    };

    class StateSet
    {
        std::set<FA::state_id_t> _set;
    public:
        StateSet() = default;
        StateSet(const StateSet &) = default;
        StateSet(StateSet &&) = default;

        ~StateSet() = default;

        void
        insert(FA::state_id_t item)
        { _set.insert(item); }

        bool
        hasState(FA::state_id_t state)
        { return _set.find(state) != _set.end(); }

        auto
        begin() -> decltype(StateSet::_set.begin())
        { return _set.begin(); }

        auto
        begin() const -> decltype(StateSet::_set.cbegin())
        { return _set.cbegin(); }

        auto
        end() -> decltype(StateSet::_set.end())
        { return _set.end(); }

        auto
        end() const -> decltype(StateSet::_set.cend())
        { return _set.cend(); }

        size_t
        size() const
        { return _set.size(); }

        void
        join(const StateSet &b)
        { _set.insert(b.begin(), b.end()); }

        bool
        operator < (const StateSet &b) const
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

        bool
        operator == (const StateSet &b) const
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

private:
    std::vector<State> _states;

    StateSet getClosure(state_id_t state, int ch);

public:
    FA() = default;

    inline state_id_t
    createState(int accept = 0)
    {
        state_id_t id;
        _states.emplace_back(State(id = _states.size(), accept));
        return id;
    }

    inline void
    addLink(state_id_t dst, state_id_t src, int ch)
    {
        assert(dst < _states.size());
        assert(src < _states.size());

        _states[src].links.emplace(ch, dst);
    }

    inline State &
    getState(state_id_t index)
    { return _states[index]; }

    inline auto
    begin() -> decltype(FA::_states.begin())
    { return _states.begin(); }

    inline auto
    end() -> decltype(FA::_states.end())
    { return _states.end(); }

    inline auto
    begin() const -> decltype(FA::_states.cbegin())
    { return _states.cbegin(); }

    inline auto
    end() const -> decltype(FA::_states.cend())
    { return _states.cend(); }

    inline size_t
    stateCount() const
    { return _states.size(); }

    std::unique_ptr<FA> nfa2dfa();

    void output(std::ostream &_os) const;
};

}

#endif
