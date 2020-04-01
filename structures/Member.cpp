//
// Created by slyfox on 21.03.20.
//

#include "Member.h"

#include <QtCore/QTimer>

#include <random>
#include <variant>

namespace {

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    int test_1 = std::distance(start, end) - 1;
    std::uniform_int_distribution<> dis(0, test_1);
    int test = dis(g);
    std::advance(start, test);
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}

bool gen_bool()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::bernoulli_distribution dis;
    return dis(gen);
}

template<typename Iter>
Iter select_randomly_without_one(Iter start, Iter end, Iter without) {
    if (start == without) {
        return select_randomly(++start, end);
    }

    if (end == without + 1) {
        return select_randomly(start, --end);
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());
    Iter first = select_randomly(start, without, gen);
    Iter second = select_randomly(++without, end, gen);

    if (gen_bool()) {
        return first;
    } else {
        return second;
    }
}

template<typename Iter>
Iter select_randomly_without_two(Iter start, Iter end, Iter without_first, Iter without_second) {
    if (start == without_first) {
        return select_randomly_without_one(++start, end, without_second);
    }

    if (start == without_second) {
        return select_randomly_without_one(++start, end, without_first);
    }

    if (end == without_first + 1) {
        return select_randomly_without_one(start, --end, without_second);
    }

    if (end == without_second + 1) {
        return select_randomly_without_one(start, --end, without_first);
    }





    static std::random_device rd;
    static std::mt19937 gen(rd());


    if (without_first + 1 == without_second) {
        Iter first = select_randomly(start, without_first, gen);
        Iter second = select_randomly(++without_second, end, gen);
        if (gen_bool()) {
            return first;
        } else {
            return second;
        }
    } else if (without_second + 1 == without_first) {
        Iter first = select_randomly(start, without_second, gen);
        Iter second = select_randomly(++without_first, end, gen);
        if (gen_bool()) {
            return first;
        } else {
            return second;
        }
    }

    Iter first, second, third;
    if (without_second > without_first) {
        first = select_randomly(start, without_first, gen);
        second = select_randomly(++without_first, without_second, gen);
        third = select_randomly(++without_second, end, gen);
    } else {
        first = select_randomly(start, without_second, gen);
        second = select_randomly(++without_second, without_first, gen);
        third = select_randomly(++without_first, end, gen);
    }

    if (gen_bool()) {
        if (gen_bool()) {
            return first;
        } else {
            return second;
        }
    } else {
        if (gen_bool()) {
            return second;
        } else {
            return third;
        }
    }
}

} // unnamed namespace

std::chrono::seconds Member::s_period = std::chrono::seconds(7);
std::chrono::seconds Member::s_invalidation_period = std::chrono::seconds(30);

Member::Member(int id, IMessageEnvironment & message_env, ITimer & timer) :
    m_id(id),
    m_message_env(message_env),
    m_timer(timer)
{}

void Member::handle_ping(const PingMessage & msg)
{
    if (m_alive) {
        Member &poll_target = msg.from;

        AckMessage message{*this, poll_target, msg.target};
        m_message_env.register_message(message);
    }

    m_message_env.unregister_message(msg.id);
}

void Member::handle_ack(const AckMessage & msg)
{
    if (m_alive) {
        if (msg.target) {
            AckMessage message{*this, msg.target->get()};
            m_message_env.register_message(message);
        } else {
            if (m_poll_target) {
                if (msg.from.get_id() == (*m_poll_target)->get().get_id()) {
                    m_poll_target = std::nullopt;
                }
            }
        }
    }

    m_message_env.unregister_message(msg.id);
}

void Member::handle_ping_req(const PingReqMessage & msg)
{
    if (m_alive) {
        PingMessage message{*this, msg.target, msg.from};
        m_message_env.register_message(message);
    }

    m_message_env.unregister_message(msg.id);
}

void Member::run()
{
    m_timer.schedule_periodically([this](){
        tick();
    });
}

void Member::tick()
{
    if (!m_alive) {
        return;
    }

    if (auto now = std::chrono::system_clock::now(); now > m_last_poll_time + s_period) {
        if (now > m_last_poll_time + s_invalidation_period) {
            m_last_poll_time = now;
            m_additional_poll_targets.clear();
            m_req_sent = false;

            if (!m_poll_target) {
                //TODO: invalidate previous Member;
            }

            auto iter = select_randomly(m_members.begin(), m_members.end());
            m_poll_target = iter;
            //std::vector<std::reference_wrapper<Member>> m_additional_poll_targets

            PingMessage msg{*this, *iter};
            m_message_env.register_message(msg);
        } else if (m_poll_target && !m_req_sent ) {
            auto iter = select_randomly_without_one(m_members.begin(), m_members.end(), *m_poll_target);
            m_additional_poll_targets.push_back(*iter);

            PingReqMessage msg_first{*this, *iter, **m_poll_target};
            m_message_env.register_message(msg_first);

            iter = select_randomly_without_two(m_members.begin(), m_members.end(), *m_poll_target, iter);
            m_additional_poll_targets.push_back(*iter);

            PingReqMessage msg_second{*this, *iter, **m_poll_target};
            m_message_env.register_message(msg_second);

            m_req_sent = true;
        }
    }
    // TODO: if minor timout expired - fill additional members

    // else just wait
}
