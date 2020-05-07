//
// Created by slyfox on 28.03.20.
//

#include "MainWidget.h"

#include <QRegularExpression>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QPushButton>

MainWidget::MainWidget(int size)
{
    resize(size, size);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(ShowContextMenu(QPoint)));

    QPushButton * stop_button = new QPushButton("stop", this);
    stop_button->setObjectName("stop");
    stop_button->move(frameGeometry().width() - 150, frameGeometry().height() - 50);
    connect(stop_button, SIGNAL(clicked()),
            this, SLOT(stop()));

    QPushButton * start_button = new QPushButton("start", this);
    start_button->setObjectName("start");
    start_button->move(frameGeometry().width() - 250, frameGeometry().height() - 50);
    start_button->setEnabled(false);
    connect(start_button, SIGNAL(clicked()),
            this, SLOT(start()));
}

void MainWidget::stop()
{
    for (auto & message_drawer : findChildren<MessageDrawer *>(QRegularExpression("message_*"))) {
        message_drawer->setEnabled(false);
    }

    for (auto & member_drawer : findChildren<MemberDrawer2 *>(QRegularExpression("member_*"))) {
        member_drawer->get_member().stop();
    }

    QPushButton * stop_button = findChild<QPushButton *>("stop");
    QPushButton * start_button = findChild<QPushButton *>("start");

    stop_button->setEnabled(false);
    start_button->setEnabled(true);
}

void MainWidget::start()
{
    for (auto & message_drawer : findChildren<MessageDrawer *>(QRegularExpression("message_*"))) {
        message_drawer->setEnabled(true);
    }

    for (auto & member_drawer : findChildren<MemberDrawer2 *>(QRegularExpression("member_*"))) {
        member_drawer->get_member().start();
    }

    QPushButton * stop_button = findChild<QPushButton *>("stop");
    QPushButton * start_button = findChild<QPushButton *>("start");

    stop_button->setEnabled(true);
    start_button->setEnabled(false);
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
