//
// Created by slyfox on 28.03.20.
//

#pragma once

#include <functional>
#include <variant>

class Member;

struct BaseMessage {
    BaseMessage(Member & from, Member & to)
        : id(s_id_counter++), from(from), to(to)
    {}

    int id;
    Member & from;
    Member & to;

    static int s_id_counter;
};

struct PingMessage : public BaseMessage
{
    PingMessage(Member & from, Member & to, std::optional<std::reference_wrapper<Member>> target = std::nullopt)
        : BaseMessage(from, to), target(target)
    {}

    std::optional<std::reference_wrapper<Member>> target;
};

struct AckMessage : public BaseMessage
{
    AckMessage(Member & from, Member & to, std::optional<std::reference_wrapper<Member>> target = std::nullopt)
        : BaseMessage(from, to), target(target)
    {}

    std::optional<std::reference_wrapper<Member>> target;
};

struct PingReqMessage : public BaseMessage
{
    PingReqMessage(Member & from, Member & to, Member & target)
        : BaseMessage(from, to), target(target)
    {}

    Member & target;
};

using Message = std::variant<PingMessage, AckMessage, PingReqMessage>;



