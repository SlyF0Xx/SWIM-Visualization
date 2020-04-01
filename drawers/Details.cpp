#include "Details.h"
#include "ui_Details.h"

Details::Details(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Details)
{
    ui->setupUi(this);
}

Details::~Details()
{
    delete ui;
}
