//
// Created by slyfox on 29.03.20.
//

#pragma once

#include "Message.h"

class IMessageEnvironment {
public:
    virtual void register_message(const Message & message) = 0;
    virtual void unregister_message(int id) = 0;
};

