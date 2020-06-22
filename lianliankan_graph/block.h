#ifndef BLOCK_H
#define BLOCK_H
#include<QPushButton>
class Block:public QPushButton
{
    Q_OBJECT
public:
    Block(){};
    Block(int a,int b,int w,int h,int valX,int valY)
    {
        setGeometry(a,b,w,h);
        x=valX;
        y=valY;
    }
    ~Block(){}
    void setX(int val){x=val;}
    void setY(int val){y=val;}
    int getX()const{return x;}
    int getY()const{return y;}
private:
    int x;
    int y;
};

#endif // BLOCK_H
