//
// Created by slyfox on 28.03.20.
//

#pragma once

#include "MemberDrawer2.h"

#include <QtWidgets/QWidget>

#include <functional>

class MainWidget : public QWidget {
Q_OBJECT
public:
    MainWidget();

    void add_member_drawer(int id, MemberDrawer2 & member_drawer)
    { m_member_drawers.emplace(id, member_drawer); }

    void remove_member_drawer(int id)
    { m_member_drawers.erase(id); }

    const std::map<int, std::reference_wrapper<MemberDrawer2>> & get_member_drawers()
    { return m_member_drawers; }

public Q_SLOTS:
    void ShowContextMenu(const QPoint &pos);

private:
    std::map<int, std::reference_wrapper<MemberDrawer2>> m_member_drawers;
};
