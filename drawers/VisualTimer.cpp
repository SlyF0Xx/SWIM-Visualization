//
// Created by slyfox on 30.03.20.
//

#include "VisualTimer.h"
#include "MainWidget.h"

VisualTimer::VisualTimer(QObject * parent)
    : QTimer(parent)
{}

void VisualTimer::schedule_periodically(std::function<void()> func)
{
    connect(this, &VisualTimer::timeout, this, [this, func](){
        func();
    });
    start(100);
}

void VisualTimer::cancel()
{
    stop();
}

double VisualTimer::get_time_factor()
{
    return static_cast<MainWidget *>(parent())->get_time_factor();
}
