#ifndef DETAILS_H
#define DETAILS_H

#include <QtWidgets/QWidget>

namespace Ui {
class Details;
}

class Details : public QWidget
{
    Q_OBJECT

public:
    explicit Details(QWidget *parent = nullptr);
    ~Details();

private:
    Ui::Details *ui;
};

#endif // DETAILS_H
