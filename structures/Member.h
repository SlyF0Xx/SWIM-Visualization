//
// Created by slyfox on 21.03.20.
//

#pragma once

#include "Message.h"
#include "IMessageEnvironment.h"
#include "ITimer.h"
#include "IMemberEnvironment.h"

#include <chrono>
#include <map>
#include <functional>
#include <memory>

class Member {
public:
    Member(int id, IMessageEnvironment & message_env, IMemberEnvironment & member_env, ITimer & timer);

    int get_id() const
    { return m_id; }

    void handle_ping(const PingMessage & msg);
    void handle_ack(const AckMessage & msg);
    void handle_ping_req(const PingReqMessage & msg);

    void run();

    void tick();

    const std::map<int, std::reference_wrapper<Member>> & get_members()
    { return m_members; }

    const std::vector<int> & get_removed_members()
    { return m_removed_members; }

    //Because we don't implement membership
    void set_members(const std::map<int, std::reference_wrapper<Member>> & members)
    { m_members = members; }

    bool is_alive()
    { return m_alive; }

    void set_alive(bool alive)
    { m_alive = alive; }

    std::vector<std::string> get_logs()
    { return m_logs; }

    void stop();
    void start();

private:
    void apply_removed_members(const BaseMessage & message);

    bool roll_alive();
    bool roll_dead();
    bool roll();
    void change_state();

    void recalculate_time_delta();

    void invalidate_member();
    void on_new_period();
    void send_ping_request();

    int m_id;
    bool m_alive = true;

    std::chrono::system_clock::time_point m_last_poll_time;
    std::chrono::nanoseconds m_delta;

    std::map<int, std::reference_wrapper<Member>> m_members;

    IMessageEnvironment & m_message_env;
    IMemberEnvironment & m_member_env;

    ITimer & m_timer;

    std::optional<int> m_poll_target = std::nullopt;
    std::vector<int> m_additional_poll_targets;

    std::vector<int> m_removed_members;
    bool m_req_sent = false;

    static std::chrono::nanoseconds s_period;
    static std::chrono::nanoseconds s_invalidation_period;

    std::vector<std::string> m_logs;

    bool m_stopped = false;
};


