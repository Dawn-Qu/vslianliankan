#include<QtGlobal>
#include<QTime>
#include"game_logic.h"

bool cmp(int a[],int b[],int h,int w)
{
    for(int i=0;i<h*w;++i)
    {

        if(a[i]!=b[i])
            return false;

    }
    return true;
}
GameLogic::GameLogic(GameMode mode)
    :xClick(0),yClick(0),mode(mode)
{
    //Qtimer
    countdown=new QTimer(this);
    if(mode==basic)
    {
        countdown->start(BASIC_TIME_LAST);
        countdown->setSingleShot(true);
    }
    iniRandomMatrix();
}

GameLogic::~GameLogic()
{}

void GameLogic::iniRandomMatrix()
{
    std::default_random_engine rd(time(nullptr));
    LogicBlock* matrix[BLOCKS_HEIGHT+2][BLOCKS_WIDTH+2];
    blocks=new LogicBlock*[(BLOCKS_HEIGHT+2)*(BLOCKS_WIDTH+2)];
    for(int i=0;i<BLOCKS_HEIGHT+2;++i)
        for(int j=0;j<BLOCKS_WIDTH+2;++j)
        {
            matrix[i][j]=new LogicBlock(0,i,j);
        }
    for(int i=1;i<=BLOCKS_HEIGHT;++i)
    {
        for(int j=1;j<=BLOCKS_WIDTH;++j)
        {
            if(j%2==0)
                matrix[i][j]->setPicName(matrix[i][j-1]->getPicName());
            else
                matrix[i][j]->setPicName(rd()%PICTURE_NUM+1);
        }
    }

    //reset();
    for(int i=0;i<RANDOM_TIME;++i)
    {
        int x1=rd()%BLOCKS_HEIGHT+1;
        int x2=rd()%BLOCKS_HEIGHT+1;
        int y1=rd()%BLOCKS_WIDTH+1;
        int y2=rd()%BLOCKS_WIDTH+1;
        matrix[x1][y1]->swapPicName(*(matrix[x2][y2]));
    }

    //link
    for(int i=0;i<BLOCKS_HEIGHT+2;++i)
    {
        for(int j=0;j<BLOCKS_WIDTH+2;++j)
        {
            if(i>=1)
                matrix[i][j]->setDirection(matrix[i-1][j],0);
            if(i<=BLOCKS_HEIGHT)
                matrix[i][j]->setDirection(matrix[i+1][j],1);
            if(j>=1)
                matrix[i][j]->setDirection(matrix[i][j-1],2);
            if(j<=BLOCKS_WIDTH)
                matrix[i][j]->setDirection(matrix[i][j+1],3);
        }
    }

    //make array
    for(int i=0;i<BLOCKS_HEIGHT+2;++i)
        for(int j=0;j<BLOCKS_WIDTH+2;++j)
        {
            qDebug()<<i<<","<<j<<" "<<matrix[i][j]->getX()<<","<<matrix[i][j]->getY()<<" "<<matrix[i][j]->getPicName();
            blocks[i*(BLOCKS_HEIGHT+2)+j]=matrix[i][j];
            //qDebug()<<"赋值后"<<i<<","<<j<<" "<<blocks[i*(BLOCKS_HEIGHT+2)+j]->getX()<<","<<blocks[i*(BLOCKS_HEIGHT+2)+j]->getY()<<" "<<blocks[i*(BLOCKS_HEIGHT+2)+j]->getPicName();
        }

}

bool GameLogic::isVoidLine(int x1,int y1,int x2,int y2,bool isContainExtremePoint)const
{
    //qDebug()<<"isVoidLine"<<x1<<","<<y1<<" "<<x2<<","<<y2;
    if(x1!=x2&&y1!=y2)
        return false;
    if(x1==x2)
    {
        int ymin=std::min(y1,y2);
        int ymax=std::max(y1,y2);
//        for(int i=0;i<(BLOCKS_HEIGHT+2)*(BLOCKS_WIDTH+2);++i)
//        {
//            if(blocks[i]->getX()==x1&&blocks[i]->getY()==ymin)
//                LogicBlock* blockmin=blocks[i];
//            if(blocks[i]->getX()==x1&&blocks[i]->getY()==ymax)
//                LogicBlock* blockmax=blocks[i];
//        }
        if(isContainExtremePoint==true)
        {
            for(const LogicBlock* p=blocks[(BLOCKS_HEIGHT+2)*x1+ymin];p!=blocks[(BLOCKS_HEIGHT+2)*x1+ymax+1];p=p->getDirection(3))
            {
                //qDebug()<<"isVoidLion:"<<p->getX()<<","<<p->getY();
                if(p->getPicName()!=0)//方块不为0
                    return false;
            }
            return true;
        }
        else
        {
            for(const LogicBlock* p=blocks[(BLOCKS_HEIGHT+2)*x1+ymin+1];p!=blocks[(BLOCKS_HEIGHT+2)*x1+ymax];p=p->getDirection(3))
            {
                qDebug()<<"isVoidLion:"<<p->getX()<<","<<p->getY();
                if(p->getPicName()!=0)
                    return false;
            }
            return true;
        }
    }
    else if(y1==y2)
    {
        int xmin=std::min(x1,x2);
        int xmax=std::max(x1,x2);
        if(isContainExtremePoint==true)
        {
            for(const LogicBlock* p=blocks[xmin*(BLOCKS_HEIGHT+2)+y1];p!=blocks[(xmax+1)*(BLOCKS_HEIGHT+2)+y1];p=p->getDirection(1))
            {
                if(p->getPicName()!=0)
                    return false;
            }
            return true;
        }
        else
        {
            for(const LogicBlock* p=blocks[(xmin+1)*(BLOCKS_HEIGHT+2)+y1];p!=blocks[xmax*(BLOCKS_HEIGHT+2)+y1];p=p->getDirection(1))
            {
                if(p->getPicName()!=0)
                    return false;
            }
            return true;
        }
    }
    qDebug()<<"GameLogic::isVoidLine异常";
    return false;
}

bool GameLogic::dfs(int x,int y,direction thisDirection,int time)const
{
    qDebug()<<"dfs"<<x<<","<<y<<"time="<<time;
    if(x==xClick&&y==yClick)
        return true;
    if(time==4)
        return false;
    if(time==1&&isVoidLine(x,y,xClick,yClick,false))
        return true;
    if(thisDirection==horizontal)
    {
        //for(int i=1;x-i>=0;++i)//左向
        for(const LogicBlock* p=blocks[x*(BLOCKS_HEIGHT+2)+y]->getDirection(2);p!=nullptr;p=p->getDirection(2))
        {
            qDebug()<<"  "<<"dfs"<<p->getX()<<","<<p->getY()<<" "<<p->getPicName();
            if(p->getPicName()!=0&&(p->getX()!=xClick||p->getY()!=yClick))
                break;
            if(dfs(p->getX(),p->getY(),vertical,time+1))
                return true;
        }
        //for(int i=1;x+i<=BLOCKS_WIDTH+1;++i)//右向
        for(const LogicBlock* p=blocks[x*(BLOCKS_HEIGHT+2)+y]->getDirection(3);p!=nullptr;p=p->getDirection(3))
        {
            qDebug()<<"  "<<"dfs"<<p->getX()<<","<<p->getY()<<" "<<p->getPicName();
            if(p->getPicName()!=0&&(p->getX()!=xClick||p->getY()!=yClick))
                break;
            if(dfs(p->getX(),p->getY(),vertical,time+1))
                return true;
        }
        return false;
    }
    if(thisDirection==vertical)
    {
        for(const LogicBlock* p=blocks[x*(BLOCKS_HEIGHT+2)+y]->getDirection(0);p!=nullptr;p=p->getDirection(0))//向上
        {
            qDebug()<<"  "<<"dfs"<<p->getX()<<","<<p->getY()<<" "<<p->getPicName();
            if(p->getPicName()!=0&&(p->getX()!=xClick||p->getY()!=yClick))
                break;
            if(dfs(p->getX(),p->getY(),horizontal,time+1))
                return true;
        }
        //for(int i=1;y+i<=BLOCKS_HEIGHT+1;++i)//向下
        for(const LogicBlock* p=blocks[x*(BLOCKS_HEIGHT+2)+y]->getDirection(1);p!=nullptr;p=p->getDirection(1))//向下
        {
            qDebug()<<"  "<<"dfs"<<p->getX()<<","<<p->getY()<<" "<<p->getPicName();
            if(p->getPicName()!=0&&(p->getX()!=xClick||p->getY()!=yClick))
                break;
            if(dfs(p->getX(),p->getY(),horizontal,time+1))
                return true;
        }
        return false;
    }
    qDebug()<<"GameLogic::dfs异常";
    return false;
}

bool GameLogic::dfs(int x1,int y1,int x2,int y2,direction thisDirection,int time)const
{
    if(x1==x2&&y1==y2)
        return true;
    if(time==4)
        return false;
    if(time==1&&isVoidLine(x1,y1,x2,y2,false))
        return true;
    if(thisDirection==horizontal)
    {
        //for(int i=1;x1-i>=0;++i)//左向
        for(const LogicBlock* p=blocks[x1*(BLOCKS_HEIGHT+2)+y1]->getDirection(2);p!=nullptr;p=p->getDirection(2))
        {
            if(p->getPicName()!=0&&(p->getX()!=x2||p->getY()!=y2))
                break;
            if(dfs(p->getX(),p->getY(),x2,y2,vertical,time+1))
                return true;
        }
        //for(int i=1;x1+i<=BLOCKS_WIDTH+1;++i)//右向
        for(const LogicBlock* p=blocks[x1*(BLOCKS_HEIGHT+2)+y1]->getDirection(3);p!=nullptr;p=p->getDirection(3))
        {
            if(p->getPicName()!=0&&(p->getX()!=x2||p->getY()!=y2))
                break;
            if(dfs(p->getX(),p->getY(),x2,y2,vertical,time+1))
                return true;
        }
        return false;
    }
    if(thisDirection==vertical)
    {
        for(const LogicBlock* p=blocks[x1*(BLOCKS_HEIGHT+2)+y1]->getDirection(0);p!=nullptr;p=p->getDirection(0))//向上
        {
            if(p->getPicName()!=0&&(p->getX()!=x2||p->getY()!=y2))
                break;
            if(dfs(p->getX(),p->getY(),x2,y2,horizontal,time+1))
                return true;
        }
        for(const LogicBlock* p=blocks[x1*(BLOCKS_HEIGHT+2)+y1]->getDirection(1);p!=nullptr;p=p->getDirection(1))//向下
        {
            if(p->getPicName()!=0&&(p->getX()!=x2||p->getY()!=y2))
                break;
            if(dfs(p->getX(),p->getY(),x2,y2,horizontal,time+1))
                return true;
        }
        return false;
    }
    qDebug()<<"GameLogic::dfs异常";
    return false;
}

bool GameLogic::isMatched(int x,int y)const
{
    if(blocks[x*(BLOCKS_HEIGHT+2)+y]->getPicName()!=blocks[xClick*(BLOCKS_HEIGHT+2)+yClick]->getPicName()
            ||(x==xClick&&y==yClick)
            ||blocks[x*(BLOCKS_HEIGHT+2)+y]->getPicName()==0
            ||blocks[xClick*(BLOCKS_HEIGHT+2)+yClick]->getPicName()==0)//图案不同或双击同一方块
    {
        return false;
    }
    if(dfs(x,y,horizontal,1)||dfs(x,y,vertical,1))
        return true;
    else
        return false;
}

bool GameLogic::isMatched(int x1,int y1,int x2,int y2)const
{
     if(blocks[x1*(BLOCKS_HEIGHT+2)+y1]->getPicName()!=blocks[x2*(BLOCKS_HEIGHT+2)+y2]->getPicName()||(x1==x2&&y1==y2)||blocks[x1*(BLOCKS_HEIGHT+2)+y1]->getPicName()==0||blocks[x2*(BLOCKS_HEIGHT+2)+y2]->getPicName()==0)
        return false;
    if(dfs(x1,y1,x2,y2,horizontal,1)||dfs(x1,y1,x2,y2,vertical,1))
        return true;
    else
        return false;
}

void GameLogic::prompt()const
{
    for(int x=1;x<=BLOCKS_HEIGHT;++x)
        for(int y=1;y<=BLOCKS_WIDTH;++y)
        {
            for(int i=1;i<=BLOCKS_HEIGHT;++i)
                for(int j=1;j<BLOCKS_WIDTH;++j)
                {
                    if(isMatched(x,y,i,j))
                    {
                        emit Logic_setDown(x,y);
                        emit Logic_setDown(i,j);
                        return;
                    }
                }
        }
}

void GameLogic::reset()
{
    int x1,y1,x2,y2;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(int i=0;i<RANDOM_TIME;++i)
    {
        x1=qrand()%BLOCKS_HEIGHT+1;
        x2=qrand()%BLOCKS_HEIGHT+1;
        y1=qrand()%BLOCKS_WIDTH+1;
        y2=qrand()%BLOCKS_WIDTH+1;
        if(blocks[x1*(BLOCKS_HEIGHT+2)+y1]->getPicName()!=0&&blocks[x2*(BLOCKS_HEIGHT+2)+y2]->getPicName()!=0)
        {
            blocks[x1*(BLOCKS_HEIGHT+2)+y1]->swapPicName(*blocks[x2*(BLOCKS_HEIGHT+2)+y2]);
        }
    }
}

void GameLogic::click(int i,int j)
{
    qDebug()<<"click "<<i<<","<<j<<" "<<blocks[i*(BLOCKS_HEIGHT+2)+j]->getPicName();
    if(isMatched(i,j))
    {
        blocks[i*(BLOCKS_HEIGHT+2)+j]->setPicName(0);
        blocks[xClick*(BLOCKS_HEIGHT+2)+yClick]->setPicName(0);
        emit Logic_hideBlock(i,j);
        emit Logic_hideBlock(xClick,yClick);
        xClick=0;
        yClick=0;
    }
    else
    {
        xClick=i;
        yClick=j;
    }
}

QString GameLogic::strAt(int i,int j)const
{
    //qDebug()<<"strAt:"<<i<<","<<j<<" "<<blocks[i*(BLOCKS_HEIGHT+2)+j]->getPicName();
    return ":/image/image/"+QString::number(blocks[i*(BLOCKS_HEIGHT+2)+j]->getPicName())+".png";
}

int GameLogic::at(int i,int j)const
{
    return blocks[i*(BLOCKS_HEIGHT+2)+j]->getPicName();
}

const QTimer* GameLogic::getCountdown() const
{
    return countdown;
}

void GameLogic::Logic_SP()
{
    if(state==going)
    {
        state=pausing;
        emit Logic_pause();
        countdown->stop();
    }
    else
    {
        state=going;
        emit Logic_go();
        countdown->start();
    }
}
