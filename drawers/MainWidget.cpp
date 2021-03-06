//
// Created by slyfox on 28.03.20.
//

#include "MainWidget.h"

#include <QRegularExpression>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>

#include "ui_MainWidget.h"

MainWidget::MainWidget() :
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    resize(1000, 800);

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(ShowContextMenu(QPoint)));
/*
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

    QSlider * slider = new QSlider(Qt::Horizontal, this);
    slider->move(frameGeometry().width() - 400, frameGeometry().height() - 50);
    slider->setMaximum(100);
    slider->setMinimum(1);
    connect(slider, &QSlider::valueChanged, this, &MainWidget::set_time_factor);

    QSpinBox * die_probability = new QSpinBox(this);
    die_probability->move(frameGeometry().width() - 150, frameGeometry().height() - 100);
    die_probability->setMaximum(100);
    die_probability->setMinimum(1);
    connect(die_probability, qOverload<int>(&QSpinBox::valueChanged), this, &MainWidget::set_die_probability);

    QSpinBox * recover_probability = new QSpinBox(this);
    recover_probability->move(frameGeometry().width() - 150, frameGeometry().height() - 150);
    recover_probability->setMaximum(100);
    recover_probability->setMinimum(1);
    connect(recover_probability, qOverload<int>(&QSpinBox::valueChanged), this, &MainWidget::set_recover_probability);
*/
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

void MainWidget::on_stop_clicked()
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

void MainWidget::on_start_clicked()
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

void MainWidget::on_die_probability_sliderMoved(int position)
{
    m_die_probability = 0.01 * position;
}

void MainWidget::on_recover_probability_sliderMoved(int position)
{
    m_recover_probability = 0.01 * position;
}

void MainWidget::on_timespeed_sliderMoved(int position)
{
    m_time_factor = position;
    for (auto & message_drawer : findChildren<MessageDrawer *>(QRegularExpression("message_*"))) {
        message_drawer->change_time_factor(m_time_factor);
    }
}
