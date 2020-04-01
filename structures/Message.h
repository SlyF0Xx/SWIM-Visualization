//
// Created by slyfox on 28.03.20.
//

#pragma once

#include <functional>
#include <variant>

class Member;

struct BaseMessage {
    BaseMessage(int from, int to)
        : id(s_id_counter++), from(from), to(to)
    {}

    int id;
    int from;
    int to;

    static int s_id_counter;
};

struct PingMessage : public BaseMessage
{
    PingMessage(int from, int to, std::optional<int> target = std::nullopt)
        : BaseMessage(from, to), target(target)
    {}

    std::optional<int> target;
};

struct AckMessage : public BaseMessage
{
    AckMessage(int from, int to, std::optional<int> target = std::nullopt)
        : BaseMessage(from, to), target(target)
    {}

    std::optional<int> target;
};

struct PingReqMessage : public BaseMessage
{
    PingReqMessage(int from, int to, int target)
        : BaseMessage(from, to), target(target)
    {}

    int target;
};

using Message = std::variant<PingMessage, AckMessage, PingReqMessage>;



