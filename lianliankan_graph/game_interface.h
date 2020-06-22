#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H
#include<QWidget>
#include<QDialogButtonBox>
#include<QLayout>
#include<QPushButton>
#include<QTimer>
#include<QTime>
#include<QtGlobal>
#include<QVector>
#include<QMessageBox>
#include<QLCDNumber>
#include"global.h"
#include"block.h"
#include"game_logic.h"

class Game:public QWidget
{
    Q_OBJECT

public:
    Game(GameMode mode);
    ~Game();
    void iniGame();
    void refresh();
public slots:
    void on_ButtonSP();
    void reset();
    void prompt();
    void back();
    void blockClicked();
    void Game_hideBlock(int i,int j);
    void Game_setDown(int i,int j);
    void timeUp();
    void go();
    void pause();
signals:
    void SP();

private:
    int xClick,yClick;
    QDialogButtonBox* menu;
    QPushButton *buttonSP;
    QPushButton *buttonReset;
    QPushButton *buttonPrompt;
    QPushButton *buttonBack;
    QLCDNumber *clock;
    QVector<QVector<Block*> > blocks;
    GameLogic* logic;
};


#endif // GAMEINTERFACE_H
