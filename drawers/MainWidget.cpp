//
// Created by slyfox on 28.03.20.
//

#include "MainWidget.h"

#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>

MainWidget::MainWidget()
{
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(ShowContextMenu(QPoint)));
}

void MainWidget::ShowContextMenu(const QPoint &pos)
{
    for (auto & [id, member] : m_member_drawers) {
        if (pos.x() >= member.get().get_x() && pos.x() <= member.get().get_x() + 40 &&
            pos.y() >= member.get().get_y() && pos.y() <= member.get().get_y() + 40) {
            member.get().ShowContextMenu(pos);
            break;
        }
    }
    /*
    QMenu contextMenu(tr("Context menu"), this);

    QAction action1("Remove Data Point", this);
    connect(&action1, SIGNAL(triggered()), this, SLOT(removeDataPoint()));
    contextMenu.addAction(&action1);

    contextMenu.exec(mapToGlobal(pos));
     */
}
