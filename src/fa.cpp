#include <queue>

#include "fa.hpp"

using namespace parsical;

const int FA::ANY_LINK;

FA::StateSet
FA::getClosure(state_id_t state, int ch)
{
    StateSet ret;
    std::queue<state_id_t> q;
    auto &st = getState(state);

    if (ch != ANY_LINK) {
        for (auto iter = st.links.lower_bound(ch), limit = st.links.upper_bound(ch); iter != limit; ++iter) {
            ret.insert(iter->second);
            q.push(iter->second);
        }
    }
    else {
        ret.insert(state);
        q.push(state);
    }

    while (!q.empty()) {
        auto &st = getState(q.front()); q.pop();
        for (auto iter = st.links.lower_bound(ANY_LINK), limit = st.links.upper_bound(ANY_LINK); iter != limit; ++iter) {
            if (!ret.hasState(iter->second)) {
                ret.insert(iter->second);
                q.push(iter->second);
            }
        }
    }

    return ret;
}

std::unique_ptr<FA>
FA::nfa2dfa()
{
    std::unique_ptr<FA> fa(new FA());
    std::map<StateSet, state_id_t> state_map;

    StateSet initial = getClosure(0, ANY_LINK);
    state_map[initial] = fa->createState();
    for (auto &st : initial) {
        if (getState(st).accept) {
            if (!fa->getState(0).accept || getState(st).accept < fa->getState(0).accept) {
                fa->getState(0).accept = getState(st).accept;
            }
        }
    }

    std::queue<StateSet> q;
    q.push(initial);
    while (!q.empty()) {
        StateSet set = std::move(q.front()); q.pop();
        for (int ch = 0; ch < 256; ++ch) {
            StateSet new_set;

            for (auto &old_st : set) {
                new_set.join(getClosure(old_st, ch));
            }

            if (new_set.size()) {
                if (state_map.find(new_set) == state_map.end()) {
                    state_map[new_set] = fa->createState();
                    q.push(new_set);
                }

                auto new_state = state_map[new_set];

                fa->addLink(new_state, state_map[set], ch);
                for (auto &st : new_set) {
                    if (getState(st).accept) {
                        if (!fa->getState(new_state).accept || 
                            getState(st).accept < fa->getState(new_state).accept) {

                            fa->getState(new_state).accept = getState(st).accept;
                        }
                    }
                }
            }
        }
    }

    return fa;
}

void
FA::output(std::ostream &_os) const
{
    for (auto &state : *this) {
        _os << state.id << "\t:" << state.accept << std::endl;

        for (auto &link : state.links) {
            _os << "\t" << (int)link.first;
            if (std::isprint(link.first)) {
                _os << '(' << (char)link.first << ')';
            }
            _os << "\t" << link.second << std::endl;
        }
    }
}
