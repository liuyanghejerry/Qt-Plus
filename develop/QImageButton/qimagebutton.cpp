#include "qimagebutton.h"

QImageButton::QImageButton(const QString &normal, const QString &hovered, const QString &pressed, const QString &checked)
    : myPic(normal), myHoveredPic(hovered), myPressedPic(pressed), lastUnderMouse(-1), bpressed(false)
{
    setFixedSize(myPic.size());
#if defined(WIN32) || defined(WIN64)
    setMask(::mask(myPic));
#endif
    lastState = Normal;

    /* Both are necessary for some styles */
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover, true);

    if (checked != "")
        myCheckedPic = QPixmap(checked);
}

void QImageButton::changePics(const QString &normal, const QString &hovered,const QString &pressed, const QString &checked)
{
    myPic = QPixmap(normal);
    myHoveredPic = QPixmap(hovered);
    myPressedPic = QPixmap(pressed);
    if (checked != "")
        myCheckedPic = QPixmap(checked);

#if defined(WIN32) || defined(WIN64)
    setMask(lastState == Checked ? ::mask(myCheckedPic) : (lastState == Normal ? ::mask(myPic) : (lastState == Pressed ? ::mask(myPressedPic) : ::mask(myHoveredPic))));
#endif

    update();
}

void QImageButton::mousePressEvent(QMouseEvent *e)
{
    bpressed = true;
    QAbstractButton::mousePressEvent(e);
}

void QImageButton::mouseReleaseEvent(QMouseEvent *e)
{
    bpressed = false;
    QAbstractButton::mouseReleaseEvent(e);
}

QSize QImageButton::sizeHint() const
{
    return myPic.size();
}

QSize QImageButton::minimumSizeHint() const
{
    return sizeHint();
}

QSize QImageButton::maximumSize() const
{
    return sizeHint();
}

void QImageButton::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    int newState;
    if ((this->isChecked()) && !myCheckedPic.isNull()) {
        newState = Checked;
        painter.drawPixmap(e->rect(), myCheckedPic, e->rect());
    }else if(this->isDown () && !myPressedPic.isNull()) {
        newState = Pressed;
        painter.drawPixmap(e->rect(), myPressedPic, e->rect());
    }else {
        if (!underMouse()) {
            newState = Normal;
            painter.drawPixmap(e->rect(), myPic, e->rect());
        }
        else {
            newState = Hovered;
            painter.drawPixmap(e->rect(), myHoveredPic, e->rect());
        }
    }

    if (newState != lastState) {
        lastState = newState;
#if defined(WIN32) || defined(WIN64)
        setMask(lastState == Checked ? ::mask(myCheckedPic) : (lastState == Normal ? ::mask(myPic) : (lastState == Pressed ? ::mask(myPressedPic) : ::mask(myHoveredPic))));
#endif
    }

    lastUnderMouse = underMouse();
}

void QImageButton::mouseMoveEvent(QMouseEvent *)
{
    if (int(underMouse()) == lastUnderMouse)
        return;
    update();
}
