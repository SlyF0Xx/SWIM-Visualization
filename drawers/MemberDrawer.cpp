//
// Created by slyfox on 21.03.20.
//

#include "MemberDrawer.h"

#include <QBrush>
#include <QString>

MemberDrawer::MemberDrawer(const Member & member, int x, int y) :
        QGraphicsEllipseItem(x, y, 10, 10),
        m_member(member),
        m_label(QString::number(m_member.get_id()), this)
{
        setBrush(Qt::yellow);
}
