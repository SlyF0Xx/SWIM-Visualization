#include "structures/Member.h"
#include "drawers/MemberDrawer2.h"
#include "drawers/MainWidget.h"
#include "drawers/MessageEnvironmentForDrawers.h"
#include "drawers/VisualTimer.h"

#include <QtWidgets/QPushButton>
#include <QApplication>
#include <QLabel>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVector>

#include <vector>
#include <cmath>
#include <math.h>
#include <QtWidgets/QLayout>
#include <drawers/Details.h>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    MainWidget window;
    window.resize(400, 400);
    window.setWindowTitle
            (QApplication::translate("childwidget", "Child widget"));
    window.show();

    MessageEnvironmentForDrawers message_env(&window);

    std::map<int, std::unique_ptr<Member>> members;

    const int count = 10;
    for (int i = 0; i < count; ++i) {
        VisualTimer * timer = new VisualTimer(&window);

        members.emplace(i, std::make_unique<Member>(i, message_env, *timer));
    }

    const int radius = 100;
    const double angle = 2.0 * M_PI / count;

    for (auto & [id, member] : members) {
        std::map<int, std::reference_wrapper<Member>> member_ref_wrappers;
        for (auto & [cpy_id, cpy_member] : members) {
            if (id != cpy_id) {
                member_ref_wrappers.emplace(cpy_id, *cpy_member);
            }
        }

        member->set_members(member_ref_wrappers);

        double y = std::sin(angle * id);
        double x = std::cos(angle * id);

        //double y = std::sin(angle * i);
        //double x = std::cos(angle * i);

        MemberDrawer2 * drawer = new MemberDrawer2(*member, x * radius + radius, y * radius + radius, &window);
        //drawer->move(i * 40, i * 40);
        drawer->show();
    }

    for (auto & [id, member] : members) {
        member->run();
    }


    return app.exec();
}