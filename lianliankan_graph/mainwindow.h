#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QDialogButtonBox>
#include<QPushButton>
#include<QLayout>
#include<QLabel>
#include"global.h"
#include"game_interface.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void setBasicMode();
    void setRelaxMode();
    void setLevelMode();
    void quit();


private:
    Game* game;
    QDialogButtonBox* menu;
    QPushButton* buttonBasicMode;
    QPushButton* buttonRelaxMode;
    QPushButton* buttonLevelMode;
    QPushButton* buttonQuit;
};

#endif // MAINWINDOW_H
