//
// Created by slyfox on 22.03.20.
//

#include "MemberDrawer2.h"
#include "MainWidget.h"
#include "Details.h"
//#include "Details.h"

#include <QBrush>
#include <QString>
#include <QtGui/QPainter>
#include <QtCore/QTimer>

#include <random>
#include <QtWidgets/QMenu>
#include <QtGui/QTextItem>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QListWidget>

MemberDrawer2::MemberDrawer2(Member & member, int x, int y, MainWidget * parent) :
    QWidget(parent), m_x(x), m_y(y), m_member(member)
{
    move(x, y);
    resize(40, 40);

    setObjectName("member_" + QString::number(m_member.get_id()));
    //resize(parent->width(), parent->height());
    parent->add_member_drawer(m_member.get_id(), *this);

    /*
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(ShowContextMenu(QPoint)));
            */

    //connect(this, &MemberDrawer2::customContextMenuRequested,
    //        this, &MemberDrawer2::ShowContextMenu);
}

/*
MemberDrawer2::~MemberDrawer2()
{
    static_cast<MainWidget*>(parentWidget())->remove_member_drawer(m_member.get_id());
}
*/

void MemberDrawer2::ShowContextMenu(const QPoint &pos)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction action1("", this);
    if (m_member.is_alive()) {
        action1.setText("Kill");
        connect(&action1, SIGNAL(triggered()), this, SLOT(setDead()));
    } else {
        action1.setText("Alive");
        connect(&action1, SIGNAL(triggered()), this, SLOT(setAlive()));
    }
    contextMenu.addAction(&action1);

    QAction action2("Show details", this);
    connect(&action2, SIGNAL(triggered()), this, SLOT(showDetails()));
    contextMenu.addAction(&action2);


    QAction action3("", this);
    if (auto selected_member = static_cast<MainWidget *>(parentWidget())->get_selected_member();
        selected_member && *selected_member == m_member.get_id()) {

        action3.setText("Deselect");
        connect(&action3, SIGNAL(triggered()), this, SLOT(deselect()));
    } else {
        action3.setText("Select");
        connect(&action3, SIGNAL(triggered()), this, SLOT(select()));
    }
    contextMenu.addAction(&action3);

    contextMenu.exec(pos);
    //contextMenu.exec(mapToGlobal(pos));
}

void MemberDrawer2::select()
{
    static_cast<MainWidget *>(parentWidget())->set_selected_member(m_member.get_id());
}

void MemberDrawer2::deselect()
{
    static_cast<MainWidget *>(parentWidget())->set_selected_member(std::nullopt);
}

void MemberDrawer2::showDetails()
{
    Details details;
    QLabel * id_label = details.findChild<QLabel *>("id");
    id_label->setText(QString::number(m_member.get_id()));

    QLabel * state_label = details.findChild<QLabel *>("state");
    state_label->setText(m_member.is_alive() ? "alive" : "dead");

    QListWidget * members_list = details.findChild<QListWidget *>("members");

    for (auto & [id, member] : m_member.get_members()) {
        members_list->addItem(QString::number(id));
    }

    QListWidget * logs_list = details.findChild<QListWidget *>("logs");

    for (auto & log: m_member.get_logs()) {
        logs_list->addItem(QString::fromStdString(log));
    }

    details.exec();
}

void MemberDrawer2::setAlive()
{
    m_member.set_alive(true);
    update();
}

void MemberDrawer2::setDead()
{
    m_member.set_alive(false);
    update();
}

void MemberDrawer2::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    //painter.translate(width() / 2, height() / 2);
    //painter.scale(side / 200.0, side / 200.0);

    if (!m_member.is_alive()) {
        QBrush brush(Qt::red);
        painter.setBrush(brush);
    }

    if (!m_member.get_removed_members().empty()) {
        QBrush brush(Qt::black);
        painter.setBrush(brush);
    }

    painter.drawEllipse(0, 0, 40, 40);
    //painter.drawText(m_x + 17, m_y + 25, QString::number(m_member.get_id()));

    painter.drawText(20, 20, QString::number(m_member.get_id()));
}
