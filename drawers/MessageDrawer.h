//
// Created by slyfox on 28.03.20.
//

#pragma once

#include "../structures/Message.h"

#include <QtWidgets/QWidget>

class MemberDrawer2;

class MessageDrawer : public QWidget {
public:
    MessageDrawer(const Message & message, int x, int y, int dest_x, int dest_y, QWidget * parent);

    void paintEvent(QPaintEvent *);
private:
    double m_x, m_y;

    double m_step_x, m_step_y;

    int m_step = 0;
    static int s_steps_count;

    const Message & m_message;
};

