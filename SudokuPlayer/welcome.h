#ifndef WELCOME_H
#define WELCOME_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class Welcome;
}

class Welcome : public QWidget
{
    Q_OBJECT

public:
    explicit Welcome(QWidget *parent = nullptr);
    ~Welcome();

private slots:
    void on_easyB_clicked();

    void on_medB_clicked();

    void on_hardB_clicked();

private:
    Ui::Welcome *ui;
    char randomLetterGenerator();
};

#endif // WELCOME_H
