#include"game_interface.h"

Game::Game(GameMode mode)
{
    //new
    menu=new QDialogButtonBox;
    buttonSP=new QPushButton;
    buttonReset=new QPushButton;
    buttonPrompt=new QPushButton;
    buttonBack=new QPushButton;
    logic=new GameLogic(mode);
    //logic->iniRandomMatrix();
    clock=new QLCDNumber;
    QVBoxLayout* menuLayout=new QVBoxLayout();
    iniGame();

    //游戏背景
    setGeometry(0,0,MAIN_WINDOW_WIDTH,MAIN_WINDOW_HEIGHT);
    setFixedSize(MAIN_WINDOW_WIDTH,MAIN_WINDOW_HEIGHT);
    QPalette windowPal=this->palette();
    setAutoFillBackground(true);
    windowPal.setBrush(QPalette::Background,QBrush(QPixmap(":/image/image/background.jpg")));
    setPalette(windowPal);
    //menu
    menu->setGeometry(MAIN_WINDOW_WIDTH-MENU_WIDTH,0,MENU_WIDTH,MENU_HEIGHT);
    QPalette menuPal=menu->palette();
    menu->setAutoFillBackground(true);
    menuPal.setBrush(QPalette::Background,QBrush(QPixmap(":/image/image/menu.jpg")));
    menu->setPalette(menuPal);
    menu->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    //按钮
    menuLayout->addWidget(buttonSP);
    menuLayout->addWidget(buttonReset);
    menuLayout->addWidget(buttonPrompt);
    menuLayout->addWidget(buttonBack);
    buttonSP->setText("暂停游戏");
    buttonReset->setText("重列");
    buttonPrompt->setText("提示");
    buttonBack->setText("返回");
    //时间
    clock->setGeometry(0,0,CLOCK_WIDTH,CLOCK_HEIGHT);
    clock->setDigitCount(3);
    clock->setSegmentStyle(QLCDNumber::Filled);
    clock->setAutoFillBackground(true);
    QPalette clockPal=clock->palette();
    clockPal.setBrush(clock->backgroundRole(),QBrush(QColor(0xc1,0xff,0xc1)));
    clock->setPalette(clockPal);
    clock->setParent(this);
    if(logic->getMode()==relax)//休闲模式
    {
        //logic->getCountdown()->//TODO
        clock->display("999");
    }

    menuLayout->setContentsMargins(MENU_LEFT_MARGIN,MENU_UP_MARGIN,MENU_RIGHT_MARGIN,MENU_DOWN_MARGIN);
    delete menu->layout();
    menu->setLayout(menuLayout);
    menu->setParent(this);
    menu->show();

    //connect
    connect(buttonSP,SIGNAL(clicked()),this,SLOT(on_ButtonSP()));
    connect(buttonReset,SIGNAL(clicked()),this,SLOT(reset()));
    connect(buttonPrompt,SIGNAL(clicked()),this,SLOT(prompt()));
    connect(buttonBack,SIGNAL(clicked()),this,SLOT(back()));
    connect(logic,SIGNAL(Logic_hideBlock(int,int)),this,SLOT(Game_hideBlock(int,int)));
    for(int i=1;i<=BLOCKS_HEIGHT;++i)
        for(int j=1;j<=BLOCKS_WIDTH;++j)
        {
            connect(blocks[i][j],SIGNAL(clicked()),this,SLOT(blockClicked()));
        }
    connect(logic,SIGNAL(Logic_setDown(int,int)),this,SLOT(Game_setDown(int,int)));
    connect(logic->getCountdown(),SIGNAL(timeout()),this,SLOT(timeUp()));
    connect(logic,SIGNAL(Logic_go()),this,SLOT(go()));
    connect(logic,SIGNAL(Logic_pause()),this,SLOT(pause()));
}

Game::~Game()
{
    delete menu;
}

void Game::iniGame()
{
    blocks.resize(BLOCKS_HEIGHT+2);
    for(int i=1;i<=BLOCKS_HEIGHT;++i)
    {
        blocks[i].resize(BLOCKS_WIDTH+2);
    }
    QSize blockSize(BLOCK_WIDTH,BLOCK_HEIGHT);
    for(int i=1;i<=BLOCKS_HEIGHT;++i)
    {
        for(int j=1;j<=BLOCKS_WIDTH;++j)
        {
            blocks[i][j]=new Block(BLOCK_WIDTH_BEGIN+(j-1)*BLOCK_WIDTH,BLOCK_HEIGHT_BEGIN+(i-1)*BLOCK_HEIGHT,BLOCK_WIDTH,BLOCK_HEIGHT,i,j);
            QIcon iconTmp(logic->strAt(i,j));
            blocks[i][j]->setIconSize(blockSize);
            blocks[i][j]->setIcon(iconTmp);
            blocks[i][j]->setParent(this);
            blocks[i][j]->show();
        }
    }
}

void Game::refresh()
{
    for(int i=1;i<=BLOCKS_HEIGHT;++i)
    {
        for(int j=1;j<=BLOCKS_WIDTH;++j)
        {
            QIcon iconTmp(logic->strAt(i,j));
            blocks[i][j]->setIcon(iconTmp);
            blocks[i][j]->setParent(this);
            if(logic->at(i,j)==0)
                blocks[i][j]->hide();
            else
                blocks[i][j]->show();
        }
    }
}

void Game::on_ButtonSP()
{
    emit SP();
}

void Game::reset()
{
    logic->reset();
    refresh();
}

void Game::prompt()
{
    logic->prompt();
}

void Game::back()
{
    close();//待完善
}

void Game::blockClicked()
{
    Block* p=(Block*)(sender());
    p->setDown(true);
    logic->click(p->getX(),p->getY());
    if(logic->at(p->getX(),p->getY())==0)
    {
//        p->hide();
//        blocks[xClick][yClick]->hide();
        //以上部分已用信号与槽处理
        xClick=0;
        yClick=0;
    }
    else
    {
        xClick=p->getX();
        yClick=p->getY();
    }
}

void Game::Game_setDown(int i,int j)
{
    blocks[i][j]->setDown(true);
    blocks[i][j]->setStyleSheet("background:rgb(188,238,104)");
}

void Game::Game_hideBlock(int i,int j)
{
    blocks[i][j]->hide();
}

void Game::timeUp()
{
    QMessageBox message(QMessageBox::NoIcon,"游戏结束","时间耗尽，请再接再厉！");
    message.exec();
}

void Game::go()
{
    buttonSP->setText("暂停游戏");
}

void Game::pause()
{
    buttonSP->setText("开始游戏");
}

