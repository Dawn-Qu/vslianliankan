#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //new
    menu=new QDialogButtonBox;
    buttonBasicMode=new QPushButton("基本模式");
    buttonRelaxMode=new QPushButton("休闲模式");
    buttonLevelMode=new QPushButton("关卡模式");
    buttonQuit=new QPushButton("退出游戏");
    QVBoxLayout* menuLayout=new QVBoxLayout();
    //主窗口
    setWindowTitle("欢乐连连看");
    setGeometry(0,0,MAIN_WINDOW_WIDTH,MAIN_WINDOW_HEIGHT);
    setFixedSize(MAIN_WINDOW_WIDTH,MAIN_WINDOW_HEIGHT);
    QPalette windowPal=this->palette();
    this->setAutoFillBackground(true);
    windowPal.setBrush(QPalette::Background,QBrush(QPixmap(":/image/image/background1.jpg")));
    setPalette(windowPal);

    //菜单
    menu->setGeometry(MAIN_WINDOW_WIDTH/2-MENU_WIDTH/2,0,MENU_WIDTH,MENU_HEIGHT);
    QPalette menuPal=menu->palette();
    menu->setAutoFillBackground(true);
    menuPal.setBrush(QPalette::Background,QBrush(QPixmap(":/image/image/menu.jpg")));
    menu->setPalette(menuPal);
    menu->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    menuLayout->addWidget(buttonBasicMode);
    menuLayout->addWidget(buttonRelaxMode);
    menuLayout->addWidget(buttonLevelMode);
    menuLayout->addWidget(buttonQuit);
    menuLayout->setContentsMargins(MENU_LEFT_MARGIN,MENU_UP_MARGIN,MENU_RIGHT_MARGIN,MENU_DOWN_MARGIN);
    delete menu->layout();
    menu->setLayout(menuLayout);
    menu->setParent(this);
    menu->show();
    //connect
    connect(buttonBasicMode,SIGNAL(clicked()),this,SLOT(setBasicMode()));
    connect(buttonRelaxMode,SIGNAL(clicked()),this,SLOT(setRelaxMode()));
    connect(buttonLevelMode,SIGNAL(clicked()),this,SLOT(setLevelMode()));
    connect(buttonQuit,SIGNAL(clicked()),this,SLOT(quit()));
}


void MainWindow::setBasicMode()
{
    game=new Game(basic);
    game->setParent(this);
    game->show();
}

void MainWindow::setRelaxMode()
{
    game=new Game(relax);
    game->setParent(this);
    game->show();
}

void MainWindow::setLevelMode()
{
    game=new Game(level);
    game->setParent(this);
    game->show();
}

void MainWindow::quit()
{
    close();
}

MainWindow::~MainWindow()
{
    delete menu;
}
