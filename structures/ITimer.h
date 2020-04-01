//
// Created by slyfox on 30.03.20.
//

#pragma once

#include <functional>

class ITimer {
public:
    virtual void schedule_periodically(std::function<void()> func) = 0;
    virtual void cancel() = 0;
};


