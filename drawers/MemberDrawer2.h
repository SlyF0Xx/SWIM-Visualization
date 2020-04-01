//
// Created by slyfox on 22.03.20.
//

#pragma once

#include "../structures/Member.h"
#include "MessageDrawer.h"

#include <QtWidgets/QWidget>

class MainWidget;

class MemberDrawer2 : public QWidget {
Q_OBJECT
public:
    MemberDrawer2(Member & member, int x, int y, MainWidget * parent);

    //~MemberDrawer2();

    void paintEvent(QPaintEvent *);

    int get_x() const
    { return m_x; }

    int get_y() const
    { return m_y; }

    Member & get_member()
    { return m_member;};

    //void contextMenuEvent(QContextMenuEvent *event) override;
public Q_SLOTS:
    void ShowContextMenu(const QPoint &pos);
    void showDetails();
    void setAlive();
    void setDead();

private:
    int m_x, m_y;
    Member & m_member;
};


