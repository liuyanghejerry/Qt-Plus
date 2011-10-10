#include "qgradualbox.h"

QGradualBox::QGradualBox(QWidget *p)
    : QWidget(p)
{
    mAutoDelete = false;
    setAttribute(Qt::WA_TranslucentBackground,true);
    setWindowFlags(Qt::FramelessWindowHint|Qt::ToolTip);
    mDelay = 50;
    mOpacity = 0.7;
    mtOpacity = 0;
    mStatus = 0;
    mBgColor = Qt::black;
    mFontColor = Qt::white;
    mBorderColor = Qt::transparent;
    mTimerRunning = -1;
}

QGradualBox::QGradualBox(const QString & pMessage,QWidget *p)
    : QWidget(p)
{
    setAttribute(Qt::WA_TranslucentBackground,true);
    setWindowFlags(Qt::FramelessWindowHint|Qt::ToolTip);
    mDelay = 50;
    mOpacity = 0.7;
    mtOpacity = 0;
    mStatus = 0;
    mBgColor = Qt::black;
    mFontColor = Qt::white;
    mBorderColor = Qt::transparent;
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
    painter.setOpacity(mtOpacity);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.setOpacity(mtOpacity);

    painter.drawRoundedRect(rect(),20.0, 15.0);
    painter.setOpacity(mtOpacity+0.3);
    pen.setColor(mFontColor);
    painter.setPen(pen);
    painter.drawText(rect(),mMessage,QTextOption(Qt::AlignHCenter|Qt::AlignVCenter));

}

void QGradualBox::showText(const QString & pMessage)
{
    //

    mMsgQueue.enqueue(pMessage);
    this->setVisible(true);
    if(mStatus==0&&mTimerRunning==-1)produceMessage();
}

void QGradualBox::produceMessage()
{
    //

    if(mMsgQueue.count())
    {
        mMessage = mMsgQueue.dequeue();

        QFontMetrics fm(this->font());
        int mWidth = fm.width(mMessage)+30;//Get the width of the text you want to show
        int mHeight = fm.height()+50;
        resize ( mWidth, mHeight);

        int mX = (QApplication::desktop()->screenGeometry().center() - QPoint((mWidth-50)/2,0)).x();
        int mY = (QApplication::desktop()->screenGeometry().center() - QPoint((mHeight-30)/2,0)).y();
        move(mX,mY*1.5);
        mTimerRunning = startTimer(85);
    }
}

void QGradualBox::timerEvent(QTimerEvent *  event)
{
    if(event->timerId()!=mTimerRunning)return;
    if(mStatus < 2)
    {
        mtOpacity+=0.1;
        update();
        if(mtOpacity>=mOpacity)
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
        mtOpacity-=0.1;
        update();
        if(mtOpacity<=-0.3)
        {
            mtOpacity = -0.3;
            update();
            mStatus = 4;
            return;
        }
    }
    else if(mStatus == 4)
    {
        killTimer(mTimerRunning);
        mStatus = 0;
        mTimerRunning = -1;
        emit textShown();
        if(mStatus==0&&mTimerRunning==-1)produceMessage();
        if(mAutoDelete)this->deleteLater();
        return;
    }

}
