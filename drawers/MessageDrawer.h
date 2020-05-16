//
// Created by slyfox on 28.03.20.
//

#pragma once

#include "../structures/Message.h"

#include <QtWidgets/QWidget>

class MemberDrawer2;
class MainWidget;

class MessageDrawer : public QWidget {
public:
    MessageDrawer(const Message & message, int x, int y, int dest_x, int dest_y, MainWidget * parent);

    Member & get_member(int id);

    void setEnabled(bool enabled)
    { m_enabled = enabled; }

    void paintEvent(QPaintEvent *);

    void change_time_factor(double value);

private:
    double m_x, m_y;

    double m_step_x, m_step_y;

    int m_step = 0;
    static int s_steps_count;

    const Message & m_message;

    bool m_enabled = true;
};

