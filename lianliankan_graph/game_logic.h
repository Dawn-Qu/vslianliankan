#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include<set>
#include<QWidget>
#include<algorithm>
#include<QDebug>
#include<random>
#include<QTimer>
#include"global.h"
#include"block.h"
enum GameMode{basic,relax,level};
enum GameState{going,pausing};
enum direction{horizontal,vertical};//方向
bool cmp(int a[],int b[],int h,int w);

class LogicBlock
{
private:
    int picName;
    int x;
    int y;
    LogicBlock* direction[4];//上下左右
public:
    LogicBlock(){}
    LogicBlock(int picName,int x,int y,LogicBlock* up=nullptr,LogicBlock* down=nullptr,LogicBlock* left=nullptr,LogicBlock* right=nullptr):x(x),y(y),picName(picName)
    {
        direction[0]=up;
        direction[1]=down;
        direction[2]=left;
        direction[3]=right;
    }
    virtual ~LogicBlock(){}
    int getPicName()const{return picName;}
    int getX()const{return x;}
    int getY()const{return y;}
    const LogicBlock* getDirection(int d)const{return this->direction[d];}
    void setPicName(int name)
    {
        if(name==0)
            qDebug()<<"name=0";
        this->picName=name;
    }
    void setX(int x){this->x=x;}
    void setY(int y){this->y=y;}
    void setDirection(LogicBlock* block,int direction)
    {
        this->direction[direction]=block;
    }
    int operator=(int val)
    {
        if(val==0)
            qDebug()<<"name=0";
        picName=val;
        return val;
    }
    const LogicBlock& operator=(const LogicBlock& val)
    {
        if(val.getPicName()==0)
            qDebug()<<"name=0";
        picName=val.picName;
        for(int i=0;i<4;++i)
        {
            direction[i]=val.direction[i];
        }
        return *this;
    }
    bool operator<(const LogicBlock& val)const{return picName<val.picName;}
    bool operator==(const LogicBlock& val)const{return picName==val.picName;}
    bool operator>(const LogicBlock& val)const{return picName>val.picName;}
    bool operator!=(const LogicBlock& val)const{return picName!=val.picName;}
    void swapPicName(LogicBlock& val)
    {
        if(val.getPicName()==0)
            qDebug()<<"name=0";
        int t=val.picName;
        val.picName=picName;
        picName=t;
    }
    void swapLocation(LogicBlock& val)
    {
        int tmp=val.x;
        val.x=x;
        x=tmp;

        tmp=y;
        val.y=y;
        y=tmp;

        for(int i=0;i<4;++i)
        {
            LogicBlock* t=val.direction[i];
            val.direction[i]=direction[i];
            direction[i]=t;
        }
    }
};

class GameLogic:public QObject
{
    Q_OBJECT
private:
    //LogicBlock matrix[(BLOCKS_HEIGHT+2)*(BLOCKS_WIDTH+2)];
    LogicBlock** blocks;
    int xClick,yClick;
    GameState state;
    GameMode mode;
    QTimer* countdown;
    bool dfs(int x,int y,direction thisDirection,int time)const;
    bool dfs(int x1,int y1,int x2,int y2,direction thisDirection,int time)const;
public:
    GameLogic(GameMode mode);
    virtual ~GameLogic();
    void iniRandomMatrix();
    bool isMatched(int x,int y)const;
    bool isMatched(int x1,int y1,int x2,int y2)const;
    bool isVoidLine(int x1,int y1,int x2,int y2,bool isContainExtremePoint=true)const;
    void destroy(int x,int y);
    void prompt()const;
    void reset();
    void click(int i,int j);
    QString strAt(int i,int j)const;
    int at(int i,int j)const;
    const QTimer* getCountdown()const;
    const QTimer* getSec()const;
    GameState getState()const {return state;};
    GameMode getMode()const {return mode;};
signals:
    void Logic_hideBlock(int i,int j);
    void Logic_setDown(int i,int j)const;
    void Logic_pause();
    void Logic_go();
public slots:
    void Logic_SP();
};

#endif // MATRIX_H
