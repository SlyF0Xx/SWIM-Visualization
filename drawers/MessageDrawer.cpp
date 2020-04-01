//
// Created by slyfox on 28.03.20.
//

#include "MessageDrawer.h"

#include "MemberDrawer2.h"

#include <QtGui/QPainter>
#include <QtCore/QTimer>

int MessageDrawer::s_steps_count = 30;

MessageDrawer::MessageDrawer(const Message & message, int x, int y, int dest_x, int dest_y, QWidget * parent)
    : QWidget(parent), m_x(x), m_y(y), m_message(message)
{
    resize(parent->width(), parent->height());

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
    setObjectName(QString::number(msg_id));

    m_step_x = (dest_x - m_x) / static_cast<double>(MessageDrawer::s_steps_count);
    m_step_y = (dest_y - m_y) / static_cast<double>(MessageDrawer::s_steps_count);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MessageDrawer::update));
    timer->start(100);
}

void MessageDrawer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);


    //painter.translate(width() / 2, height() / 2);
    //painter.scale(side / 200.0, side / 200.0);

    m_x += m_step_x;
    m_y += m_step_y;

    if (m_step++ >= MessageDrawer::s_steps_count) {
        if(std::holds_alternative<PingMessage>(m_message)) {
            const PingMessage & msg = std::get<PingMessage>(m_message);
            msg.to.handle_ping(msg);
        } else if(std::holds_alternative<AckMessage>(m_message)) {
            const AckMessage & msg = std::get<AckMessage>(m_message);
            msg.to.handle_ack(msg);
        } else if(std::holds_alternative<PingReqMessage>(m_message)) {
            const PingReqMessage & msg = std::get<PingReqMessage>(m_message);
            msg.to.handle_ping_req(msg);
        }
        // notify member, delete this
    } else {
        if(std::holds_alternative<PingMessage>(m_message)) {
            QBrush brush(Qt::green);
            painter.setBrush(brush);
        } else if(std::holds_alternative<AckMessage>(m_message)) {
            QBrush brush(Qt::red);
            painter.setBrush(brush);
        } else if(std::holds_alternative<PingReqMessage>(m_message)) {
            QBrush brush(Qt::blue);
            painter.setBrush(brush);
        }

        painter.drawEllipse(m_x, m_y, 8, 8);
    }
}
