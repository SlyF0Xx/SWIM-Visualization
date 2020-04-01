//
// Created by slyfox on 30.03.20.
//

#include "VisualTimer.h"

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
