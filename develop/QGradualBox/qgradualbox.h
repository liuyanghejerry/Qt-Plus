#ifndef QGRADUALBOX_H
#define QGRADUALBOX_H

#include <QtGui/QWidget>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QQueue>
#include <QDebug>

class QGradualBox : public QWidget
{
    Q_OBJECT

public:
    QGradualBox();
    QGradualBox(const QString & pMessage);
    ~QGradualBox();

    void showText(const QString & pMessage);

    void setFontColor(QColor pColor){mFontColor=pColor;}
    void setBackgroundColor(QColor pColor){mBgColor=pColor;}
    void setBorderColor(QColor pColor){mBorderColor=pColor;}
    void setMaxOpacity(qreal tOpacity){mOpacity=tOpacity;}
    void setDelay(int pDelay){mDelay=pDelay;}

protected:
    virtual void paintEvent ( QPaintEvent * event );
    //virtual void showEvent ( QShowEvent * event );
    virtual void timerEvent(QTimerEvent *event);

private:
    void produceMessage();

private:
    //int mX;
    //int mY;
    //int mWidth;
    //int mHeight;
    qreal mOpacity;//default 0.7
    qreal mtOpacity;//temprorary use
    mutable QQueue<QString> mMsgQueue;
    mutable QString mMessage;
    //QFont mFont;
    QColor mBgColor;
    QColor mFontColor;
    QColor mBorderColor;
    //QTimer *mTimer; //replaced by QObject::startTime(int)
    int mDelay;//per 85 ms
    mutable int mTimerRunning;//-1 for none.
    mutable int mStatus;//0 for not shown;1 for fade in; 2 for showing; 3 for fade out; 4 for done;
};

#endif // QGRADUALBOX_H
