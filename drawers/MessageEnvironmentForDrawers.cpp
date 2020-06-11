//
// Created by slyfox on 29.03.20.
//

#include "MessageEnvironmentForDrawers.h"

#include "../structures/Message.h"

MessageEnvironmentForDrawers::MessageEnvironmentForDrawers(MainWidget * parent)
    : m_parent(parent)
{}

void MessageEnvironmentForDrawers::register_message(const Message & message)
{
    int msg_id;
    int member_from_id;
    int member_to_id;
    if(std::holds_alternative<PingMessage>(message)) {
        const PingMessage & msg = std::get<PingMessage>(message);
        msg_id = msg.id;
        member_from_id = msg.from;
        member_to_id   = msg.to;
    } else if(std::holds_alternative<AckMessage>(message)) {
        const AckMessage & msg = std::get<AckMessage>(message);
        msg_id = msg.id;
        member_from_id = msg.from;
        member_to_id   = msg.to;
    } else if(std::holds_alternative<PingReqMessage>(message)) {
        const PingReqMessage & msg = std::get<PingReqMessage>(message);
        msg_id = msg.id;
        member_from_id = msg.from;
        member_to_id   = msg.to;
    }

    auto it_to = m_parent->get_member_drawers().find(member_to_id);
    const MemberDrawer2 & to = it_to->second.get();

    auto it_from = m_parent->get_member_drawers().find(member_from_id);
    const MemberDrawer2 & from = it_from->second.get();

    m_messages.emplace(msg_id, message);

    const Message & msg = m_messages.find(msg_id)->second;
    MessageDrawer * drawer = new MessageDrawer(msg, from.get_x() + 20, from.get_y() + 20, to.get_x() + 20, to.get_y() + 20, m_parent);
    drawer->show();
}

void MessageEnvironmentForDrawers::unregister_message(int id)
{
    MessageDrawer * child = m_parent->findChild<MessageDrawer *>("message_" + QString::number(id));
    child->hide();
    child->close();
    child->deleteLater();

    m_messages.erase(id);
}
