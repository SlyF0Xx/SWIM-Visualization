//
// Created by slyfox on 21.03.20.
//

#pragma once

#include "../structures/Member.h"

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QLabel>
#include <QWidget>

class MemberDrawer : public QGraphicsEllipseItem {
public:
    MemberDrawer(const Member & member, int x, int y);

private:
    const Member & m_member;

    QGraphicsSimpleTextItem m_label;
};


