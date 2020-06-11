//
// Created by slyfox on 29.03.20.
//

#pragma once

#include "../structures/IMessageEnvironment.h"
#include "MessageDrawer.h"
#include "MainWidget.h"

class MessageEnvironmentForDrawers : public IMessageEnvironment {
public:
    MessageEnvironmentForDrawers(MainWidget * parent);

    void register_message(const Message & message) override;
    void unregister_message(int id) override;

private:
    MainWidget * m_parent;

    std::map<int, Message> m_messages;
};
