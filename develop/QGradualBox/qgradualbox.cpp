#include "qgradualbox.h"

QGradualBox::QGradualBox()
    : QWidget(0)
{
    setAttribute(Qt::WA_TranslucentBackground,true);
    setWindowFlags(Qt::FramelessWindowHint|Qt::ToolTip);
    mX = 100;
    mY = 500;
    mWidth = 200;
    mHeight = 30;
    mDelay = 50;
    mOpacity = 0;
    mStatus = 0;
    mBgColor = Qt::black;
    mFontColor = Qt::white;
    mBorderColor = Qt::transparent;
    mFont = this->font();
    mTimerRunning = 0;
}

QGradualBox::QGradualBox(const QString & pMessage)
    : QWidget(0)
{
    setAttribute(Qt::WA_TranslucentBackground,true);
    setWindowFlags(Qt::FramelessWindowHint|Qt::ToolTip);
    mX = 100;
    mY = 500;
    mWidth = 200;
    mHeight = 30;
    mDelay = 50;
    mOpacity = 0;
    mStatus = 0;
    mBgColor = Qt::black;
    mFontColor = Qt::white;
    mBorderColor = Qt::transparent;
    mFont = this->font();
    setGeometry ( mX, mY, mWidth, mHeight );
    mTimerRunning = -1;
    showText(pMessage);
}

QGradualBox::~QGradualBox()
{

}

void QGradualBox::paintEvent ( QPaintEvent * /*event*/ )
{
    QPainter painter(this);
    QPen pen(mBorderColor);
    QBrush brush(mBgColor);
    pen.setWidth(5);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setFont(this->font());
    painter.setOpacity(mOpacity);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.setOpacity(mOpacity);

    painter.drawRoundedRect(rect(),20.0, 15.0);
    painter.setOpacity(mOpacity+0.3);
    pen.setColor(mFontColor);
    painter.setPen(pen);
    painter.drawText(rect(),mMessage,QTextOption(Qt::AlignHCenter|Qt::AlignVCenter));

}

void QGradualBox::showText(const QString & pMessage)
{
    //

    mMsgQueue.enqueue(pMessage);
    this->show();
    if(mStatus==0&&mTimerRunning==-1)produceMessage();
}

void QGradualBox::produceMessage()
{
    //

    if(mMsgQueue.count())
    {
        mMessage = mMsgQueue.dequeue();
        //qDebug()<<"Ok, I delete one message from the queue";

        QFontMetrics fm(mFont);
        int pixelsWide = fm.width(mMessage);//Get the width of the text you want to show
        int pixelsHigh = fm.height();
        mWidth = pixelsWide+30;
        mHeight = pixelsHigh+50;
        resize ( mWidth, mHeight);

        mX = (QApplication::desktop()->screenGeometry().center() - QPoint(pixelsWide/2,0)).x();
        mY = (QApplication::desktop()->screenGeometry().center() - QPoint(pixelsWide/2,0)).y();
        move(mX,mY*1.5);
        //update();
        mTimerRunning = startTimer(85);
        //qDebug() <<"Produce Onece, and the lenth of the queue is "<<mMsgQueue.count();
        //mTimerRunning++;
    }
}

void QGradualBox::timerEvent(QTimerEvent *  event)
{
    if(event->timerId()!=mTimerRunning)return;
    //qDebug() <<"TimerId:"<<event->timerId();
    if(mStatus < 2)
    {
        mOpacity+=0.1;
        update();
        if(mOpacity>=0.7)
        {
            mStatus =2;
            return;
        }
    }
    else if(mStatus == 2)
    {
        static int ti =0;
        if(ti<mDelay)
        {
            ti++;
        }
        else
        {
            mStatus=3;
            ti=0;
            return;
        }
    }
    else if(mStatus == 3)
    {
        mOpacity-=0.1;
        //qDebug()<<"mOpacity:"<<mOpacity;
        update();
        if(mOpacity<=0.0)
        {
            mOpacity = 0;
            update();
            //qDebug()<<"mStatus = 3 and turning to 4";
            mStatus = 4;
            return;
        }
    }
    else if(mStatus == 4)
    {
        killTimer(mTimerRunning);
        //qDebug()<<"mOpacity:"<<mOpacity;
        mStatus = 0;
        mTimerRunning = -1;
        if(mStatus==0&&mTimerRunning==-1)produceMessage();
        return;
    }

}
