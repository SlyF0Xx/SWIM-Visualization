//
// Created by slyfox on 21.03.20.
//

#include "Member.h"

#include <QtCore/QTimer>

#include <random>
#include <variant>
#include <iostream>
#include <iomanip>

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

template<typename Clock, typename Duration>
std::ostream &operator<<(std::ostream &stream,
                         const std::chrono::time_point<Clock, Duration> &time_point)
{
    const time_t time = Clock::to_time_t(time_point);
    struct tm tm;
    localtime_r(&time, &tm);
    return stream << std::put_time(&tm, "%c"); // Print standard date&time
}

} // unnamed namespace

std::chrono::seconds Member::s_period = std::chrono::seconds(8);
std::chrono::seconds Member::s_invalidation_period = std::chrono::seconds(28);

Member::Member(int id, IMessageEnvironment & message_env, ITimer & timer) :
    m_id(id),
    m_message_env(message_env),
    m_timer(timer)
{}

void Member::apply_removed_members(const BaseMessage & message)
{
    for (auto removed_id : message.members_to_delete) {
        if (m_members.count(removed_id) != 0) {
            std::ostringstream oss;
            oss << std::chrono::system_clock::now() << std::to_string(m_id) << " received from" << message.from << ", that " << removed_id << " is dead";
            m_logs.push_back(oss.str());
        }
        m_removed_members.push_back(removed_id);
        m_members.erase(removed_id);
    }
}

void Member::handle_ping(const PingMessage & msg)
{
    if (m_alive) {
        apply_removed_members(msg);

        AckMessage ack{m_id, msg.from, msg.target};
        ack.members_to_delete = m_removed_members;
        m_message_env.register_message(ack);
    }

    m_message_env.unregister_message(msg.id);
}

void Member::handle_ack(const AckMessage & msg)
{
    if (m_alive) {
        apply_removed_members(msg);

        if (msg.target) {
            AckMessage ack{m_id, *msg.target};
            ack.members_to_delete = m_removed_members;
            m_message_env.register_message(ack);
        } else {
            if (m_poll_target) {
                if (msg.from == *m_poll_target) {
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
        apply_removed_members(msg);

        PingMessage ping{m_id, msg.target, msg.from};
        ping.members_to_delete = m_removed_members;
        m_message_env.register_message(ping);
    }

    m_message_env.unregister_message(msg.id);
}

void Member::stop()
{
    m_last_stop_time = std::chrono::system_clock::now();
    m_stopped = true;
}

void Member::start()
{
    m_stopped = false;
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

    if (m_stopped) {
        auto now = std::chrono::system_clock::now();
        auto delta = now - m_last_stop_time;
        m_last_stop_time = now;
        m_last_poll_time += delta;
    }

    if (auto now = std::chrono::system_clock::now(); now > m_last_poll_time + s_period) {
        if (now > m_last_poll_time + s_invalidation_period) {
            m_last_poll_time = now;
            m_additional_poll_targets.clear();
            m_req_sent = false;

            if (m_poll_target) {
                std::ostringstream oss;
                oss << now <<  std::to_string(m_id) << " find, that " << *m_poll_target << " is dead";
                m_logs.push_back(oss.str());

                m_removed_members.push_back(*m_poll_target);
                m_members.erase(*m_poll_target);
            }

            auto iter = select_randomly(m_members.begin(), m_members.end());
            /*
            auto iter = m_members.begin();
            std::advance(iter, 4);
            */
            m_poll_target = iter->first;

            PingMessage ping{m_id, iter->first};
            ping.members_to_delete = m_removed_members;
            m_message_env.register_message(ping);
        } else if (m_poll_target && !m_req_sent ) {
            auto members = m_members;
            members.erase(*m_poll_target);
            auto iter = select_randomly(members.begin(), members.end());
            /*
            auto iter = members.begin();
            std::advance(iter, 2);
            */
            m_additional_poll_targets.push_back(iter->first);

            PingReqMessage msg_first{m_id, iter->first, *m_poll_target};
            msg_first.members_to_delete = m_removed_members;
            m_message_env.register_message(msg_first);

            members.erase(iter->first);
            iter = select_randomly(members.begin(), members.end());
            /*
            iter = members.begin();
            std::advance(iter, 4);
            */
             m_additional_poll_targets.push_back(iter->first);

            PingReqMessage msg_second{m_id, iter->first, *m_poll_target};
            msg_second.members_to_delete = m_removed_members;
            m_message_env.register_message(msg_second);

            m_req_sent = true;
        }
    }
    // TODO: if minor timout expired - fill additional members

    // else just wait
}
