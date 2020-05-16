//
// Created by slyfox on 28.03.20.
//

#pragma once

#include "MemberDrawer2.h"

#include <QtWidgets/QWidget>

#include <functional>

class MessageEnvironmentForDrawers;

class MainWidget : public QWidget {
Q_OBJECT
public:
    MainWidget(int size);

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

    void set_env(MessageEnvironmentForDrawers * env)
    { m_env = env; }

public Q_SLOTS:
    void ShowContextMenu(const QPoint &pos);
    void stop();
    void start();
    void set_time_factor(int value);

private:
    std::map<int, std::reference_wrapper<MemberDrawer2>> m_member_drawers;
    std::optional<int> m_selected_member;
    double m_time_factor = 3.0;
    MessageEnvironmentForDrawers * m_env;
};
