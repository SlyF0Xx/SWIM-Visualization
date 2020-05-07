//
// Created by slyfox on 28.03.20.
//

#include "MessageDrawer.h"
#include "MainWidget.h"

#include "MemberDrawer2.h"

#include <QtGui/QPainter>
#include <QtCore/QTimer>

#include <cmath>

QLine get_line(const BaseMessage & message, const std::map<int, std::reference_wrapper<MemberDrawer2>> & member_drawers)
{
    return QLine(member_drawers.find(message.from)->second.get().get_x() + 20,
                 member_drawers.find(message.from)->second.get().get_y() + 20,
                 member_drawers.find(message.to)->second.get().get_x() + 20,
                 member_drawers.find(message.to)->second.get().get_y() + 20);
}

int MessageDrawer::s_steps_count = 100;

MessageDrawer::MessageDrawer(const Message & message, int x, int y, int dest_x, int dest_y, MainWidget * parent)
    : QWidget(parent), m_x(x), m_y(y), m_message(message)
{
    resize(parent->width(), parent->height() - 100);

    int msg_id;
    if(std::holds_alternative<PingMessage>(message)) {
        const PingMessage & msg = std::get<PingMessage>(message);
        msg_id = msg.id;
    } else if(std::holds_alternative<AckMessage>(message)) {
        const AckMessage & msg = std::get<AckMessage>(message);
        msg_id = msg.id;
    } else if(std::holds_alternative<PingReqMessage>(message)) {
        const PingReqMessage & msg = std::get<PingReqMessage>(message);
        msg_id = msg.id;
    }
    setObjectName("message_" + QString::number(msg_id));

    m_step_x = (dest_x - m_x) / static_cast<double>(MessageDrawer::s_steps_count);
    m_step_y = (dest_y - m_y) / static_cast<double>(MessageDrawer::s_steps_count);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MessageDrawer::update));
    timer->start(40);
}

Member & MessageDrawer::get_member(int id)
{
    return static_cast<MainWidget *>(parentWidget())->get_member_drawers().find(id)->second.get().get_member();
}

void MessageDrawer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);


    //painter.translate(width() / 2, height() / 2);
    //painter.scale(side / 200.0, side / 200.0);

    if (m_enabled) {
        m_x += m_step_x;
        m_y += m_step_y;
        m_step++;
    }

    if (m_step >= MessageDrawer::s_steps_count) {
        if(std::holds_alternative<PingMessage>(m_message)) {
            const PingMessage & msg = std::get<PingMessage>(m_message);
            get_member(msg.to).handle_ping(msg);
        } else if(std::holds_alternative<AckMessage>(m_message)) {
            const AckMessage & msg = std::get<AckMessage>(m_message);
            get_member(msg.to).handle_ack(msg);
        } else if(std::holds_alternative<PingReqMessage>(m_message)) {
            const PingReqMessage & msg = std::get<PingReqMessage>(m_message);
            get_member(msg.to).handle_ping_req(msg);
        }
        // notify member, delete this
    } else {
        if (auto selected_member = static_cast<MainWidget *>(parentWidget())->get_selected_member()) {
            if(!((std::holds_alternative<PingMessage>(m_message) && std::get<PingMessage>(m_message).from == *selected_member) ||
                 (std::holds_alternative<PingMessage>(m_message) && std::get<PingMessage>(m_message).target == *selected_member) ||
                 (std::holds_alternative<AckMessage>(m_message) && std::get<AckMessage>(m_message).to == *selected_member) ||
                 (std::holds_alternative<AckMessage>(m_message) && std::get<AckMessage>(m_message).target == *selected_member) ||
                 (std::holds_alternative<PingReqMessage>(m_message) && std::get<PingReqMessage>(m_message).from == *selected_member)
            )) {
                return;
            }
        }

        QLine line;
        if(std::holds_alternative<PingMessage>(m_message)) {
            QBrush brush(Qt::green);
            painter.setBrush(brush);

            line = get_line(std::get<PingMessage>(m_message), static_cast<MainWidget *>(parentWidget())->get_member_drawers());
        } else if(std::holds_alternative<AckMessage>(m_message)) {
            QBrush brush(Qt::red);
            painter.setBrush(brush);

            line = get_line(std::get<AckMessage>(m_message), static_cast<MainWidget *>(parentWidget())->get_member_drawers());
        } else if(std::holds_alternative<PingReqMessage>(m_message)) {
            QBrush brush(Qt::blue);
            painter.setBrush(brush);

            line = get_line(std::get<PingReqMessage>(m_message), static_cast<MainWidget *>(parentWidget())->get_member_drawers());
        }

        painter.drawLine(line.x1(), line.y1(), line.x2(), line.y2());
        painter.drawEllipse(m_x, m_y, 8, 8);
    }
}
