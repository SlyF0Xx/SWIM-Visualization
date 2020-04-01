//
// Created by slyfox on 21.03.20.
//

#pragma once

#include "Message.h"
#include "IMessageEnvironment.h"
#include "ITimer.h"

#include <chrono>
#include <vector>
#include <functional>
#include <memory>

class Member {
public:
    Member(int id, IMessageEnvironment & message_env, ITimer & timer);

    int get_id() const
    { return m_id; }

    void handle_ping(const PingMessage & msg);
    void handle_ack(const AckMessage & msg);
    void handle_ping_req(const PingReqMessage & msg);

    void run();

    void tick();

    //Because we don't implement membership
    void set_members(const std::vector<std::reference_wrapper<Member>> & members)
    { m_members = members; }

    bool is_alive()
    { return m_alive; }

    void set_alive(bool alive)
    { m_alive = alive; }

private:
    int m_id;
    bool m_alive = true;

    std::chrono::system_clock::time_point m_last_poll_time;

    std::vector<std::reference_wrapper<Member>> m_members;

    IMessageEnvironment & m_message_env;

    ITimer & m_timer;

    std::optional<std::vector<std::reference_wrapper<Member>>::iterator> m_poll_target;
    std::vector<std::reference_wrapper<Member>> m_additional_poll_targets;
    bool m_req_sent = false;

    static std::chrono::seconds s_period;
    static std::chrono::seconds s_invalidation_period;
};


