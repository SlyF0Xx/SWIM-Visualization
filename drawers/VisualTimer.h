//
// Created by slyfox on 30.03.20.
//

#pragma once

#include "../structures/ITimer.h"

#include <QtCore/QTimer>

class VisualTimer : public ITimer,
                    public QTimer {
public:
    VisualTimer(QObject * parent);

    void schedule_periodically(std::function<void()> func) override;

    void cancel() override;
};

