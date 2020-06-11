//
// Created by slyfox on 28.03.20.
//

#pragma once

#include "MemberDrawer2.h"
#include <structures/IMemberEnvironment.h>

#include <QtWidgets/QMainWindow>

#include <functional>

class MessageEnvironmentForDrawers;

namespace Ui {
class MainWidget;
}

class MainWidget : public QMainWindow,
                   public IMemberEnvironment {
Q_OBJECT
public:
    MainWidget();

    void add_member_drawer(int id, MemberDrawer2 & member_drawer)
    { m_member_drawers.emplace(id, member_drawer); }

    void remove_member_drawer(int id)
    { m_member_drawers.erase(id); }

    const std::map<int, std::reference_wrapper<MemberDrawer2>> & get_member_drawers()
    { return m_member_drawers; }

    std::optional<int> get_selected_member()
    { return m_selected_member; }

    void set_selected_member(const std::optional<int> & selected_member)
    { m_selected_member = selected_member; }

    double get_time_factor()
    { return m_time_factor; }

public Q_SLOTS:
    void ShowContextMenu(const QPoint &pos);

private slots:
    void on_stop_clicked();
    void on_start_clicked();

    void on_die_probability_sliderMoved(int position);
    void on_recover_probability_sliderMoved(int position);

    void on_timespeed_sliderMoved(int position);

private:
    std::map<int, std::reference_wrapper<MemberDrawer2>> m_member_drawers;
    std::optional<int> m_selected_member;
    double m_time_factor = 5.0;

private:
    Ui::MainWidget *ui;
};
