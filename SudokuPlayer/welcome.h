#ifndef WELCOME_H
#define WELCOME_H

#include <QWidget>
#include "mainwindow.h"
#include "QSoundEffect"

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

    void on_MuteMusic_stateChanged(int arg1);

private:
    Ui::Welcome *ui;
    char randomLetterGenerator();
    QSoundEffect* gameMusic;
    void PlayMusic();
    void MuteMusic();
};

#endif // WELCOME_H
