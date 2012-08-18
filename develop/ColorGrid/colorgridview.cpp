#include "colorgridview.h"

ColorGridView::ColorGridView(QWidget *parent) :
    QAbstractItemView(parent),
    menu(this),
    colorWheel(0),
    margin(5),
    gridMargin(2),
    gridWidth(16),
    gridCount(10),
    gridsSize((gridMargin+gridWidth)*gridCount, (gridMargin+gridWidth)*gridCount)
{
    // we do not support multi selection or row selection
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectItems);
    horizontalScrollBar()->setRange(0, 0);
    verticalScrollBar()->setRange(0, 0);
    setMouseTracking(true);
    initGridBackgroundImg();
    initContextMenu();
    resize(gridsSize);
}

void ColorGridView::initGridBackgroundImg()
{
    backgroundImg = QImage(gridWidth,gridWidth, QImage::Format_ARGB32_Premultiplied);
    backgroundImg.fill(Qt::white);
    QPainter painter(&backgroundImg);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(224, 224, 224));
    int k = gridWidth/2;
    painter.drawRect(k,0,k,k);
    painter.drawRect(0,k,k,k);
    painter.setBrush(QColor(240, 240, 240));
    painter.drawRect(0,0,k,k);
    painter.drawRect(k,k,k,k);
}

void ColorGridView::initContextMenu()
{
    menu.addAction(tr("Pick"), this,SLOT(colorPicked()));
    menu.addAction(tr("Drop From Wheel"),this,SLOT(colorDroped()));
    menu.addAction(tr("Clear"),this,SLOT(colorCleared()));
    menu.addAction(tr("Cancel"));
    connect(&menu,SIGNAL(aboutToHide()),this,SLOT(repaint()));
}

QSize ColorGridView::sizeHint () const
{
    return QSize(width(), height());
}

QSize ColorGridView::minimumSizeHint () const
{
    return QSize(200, 100);
}

void ColorGridView::mousePressEvent(QMouseEvent *event)
{
    setCurrentIndex(indexAt(event->pos()));
    //    QRect rect = viewport()->rect();
    QRect rect2 = rect();
    rect2.translate(horizontalScrollBar()->value(), verticalScrollBar()->value());

    if(event->button() == Qt::RightButton){
        //TODO: check if colorWheel is ready
        menu.popup(mapToGlobal(event->pos()));
    }
    update(rect2);
}

void ColorGridView::mouseMoveEvent(QMouseEvent *event)
{
    setCurrentIndex(indexAt(event->pos()));
    //    QRect rect = viewport()->rect();
    QRect rect2 = rect();
    rect2.translate(horizontalScrollBar()->value(), verticalScrollBar()->value());
    update(rect2);
    //    update();
}

void ColorGridView::mouseReleaseEvent(QMouseEvent *event)
{
    //
}

void ColorGridView::resizeEvent(QResizeEvent *event)
{
    horizontalScrollBar()->setPageStep(viewport()->width());
    horizontalScrollBar()->setRange(0, qMax(0, gridMargin+gridsSize.width() - viewport()->width()));
    verticalScrollBar()->setPageStep(viewport()->height());
    verticalScrollBar()->setRange(0, qMax(0, gridMargin+gridsSize.height() - viewport()->height()));
}

void ColorGridView::paintEvent(QPaintEvent *event)
{
    // To make it clear, we do not use delegate at all
    //    QItemSelectionModel *selections = selectionModel();
    QStyleOptionViewItem option = viewOptions();
    QStyle::State state = option.state;

    QBrush background = option.palette.base();
    QPen foreground(option.palette.color(QPalette::WindowText));
    QPen textPen(option.palette.color(QPalette::Text));
    QPen highlightedPen(option.palette.color(QPalette::HighlightedText));

    QPainter painter(viewport());
    // close anti aliasing to make it more clear
    //        painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(event->rect(), background);
    painter.setPen(Qt::white);
    QBrush brush;
    brush.setTextureImage(backgroundImg);
    painter.setBrush(brush);

    painter.translate(margin - horizontalScrollBar()->value(),
                      margin - verticalScrollBar()->value());
    for(int i = 0; i<gridCount; ++i){
        for(int j=0; j<gridCount; ++j){
            painter.drawImage(i*(gridMargin+gridWidth),j*(gridMargin+gridWidth),
                              backgroundImg);
        }
    }

    if( !model() )return;
    for (int row = 0; row < model()->rowCount(rootIndex()); ++row) {
        for(int column = 0; column < model()->columnCount(rootIndex()); ++column)
        {
            QModelIndex index = model()->index(row, column, rootIndex());
            QColor color = model()->data(index).value<QColor>();

            int j = index.row();
            int i = index.column();
            if(color.isValid()){
                //draw white background to prevent disturbings of background if transparent color
                painter.setBrush(Qt::white);
                painter.drawRect(i*(gridMargin+gridWidth),j*(gridMargin+gridWidth),
                                 gridWidth,gridWidth);
                painter.setBrush(color);
                painter.drawRect(i*(gridMargin+gridWidth),j*(gridMargin+gridWidth),
                                 gridWidth,gridWidth);
            }
        }
    }
    QModelIndexList list = selectedIndexes();
    if( !list.isEmpty() ){
        QModelIndex index = list.at(0);
        //        Qt::ItemFlags flags = index.flags();
        //        if(flags.testFlag(QItemSelectionModel::Select)){
        painter.setPen(textPen);
        //        }
        //        if(flags.testFlag(QItemSelectionModel::Toggle)){
        //            painter.setPen(highlightedPen);
        //        }
        int row = index.row();
        int column = index.column();
        //        QPointF start(column*(gridMargin+gridWidth),row*(gridMargin+gridWidth));
        //        QRadialGradient gradient(start+QPointF(gridWidth/2,gridWidth/2),0.1,start+QPointF(gridWidth,gridWidth));
        //        gradient.setColorAt(0.5,Qt::transparent);
        //        gradient.setColorAt(1.0,Qt::black);
        //        foreground = QPen(Qt::black);

        //        QBrush brush(gradient);
        //        brush.setStyle(Qt::RadialGradientPattern);
        painter.setBrush(Qt::NoBrush);
        //        painter.setBrush(brush);
        painter.drawRect(column*(gridMargin+gridWidth),row*(gridMargin+gridWidth),
                         gridWidth,gridWidth);
    }
}

QRect ColorGridView::visualRect(const QModelIndex& index) const
{
    QRect rect(index.column()*(gridWidth+gridMargin), index.row()*(gridWidth+gridMargin),
               gridWidth, gridWidth);
    if (rect.isValid()){
        return QRect(rect.left() - horizontalScrollBar()->value(),
                     rect.top() - verticalScrollBar()->value(),
                     gridWidth, gridWidth);
    }
    else{
        return rect;
    }
}
void ColorGridView::scrollTo(const QModelIndex& index, QAbstractItemView::ScrollHint)
{
    QRect area = rect();
    QRect rect = visualRect(index);

    if (rect.left() < area.left())
        horizontalScrollBar()->setValue(
                    horizontalScrollBar()->value() + rect.left() - area.left() );
    else if (rect.right() > area.right())
        horizontalScrollBar()->setValue(
                    horizontalScrollBar()->value() + rect.right() - area.right() );

    if (rect.top() < area.top())
        verticalScrollBar()->setValue(
                    verticalScrollBar()->value() + rect.top() - area.top() );
    else if (rect.bottom() > area.bottom())
        verticalScrollBar()->setValue(
                    verticalScrollBar()->value() + rect.bottom() - area.bottom() );

    update();
}

QModelIndex ColorGridView::indexAt(const QPoint& p) const
{
    QRect area = viewport()->rect();
    area.translate(horizontalScrollBar()->value(), verticalScrollBar()->value());
    QPoint leftTop = area.topLeft();
    leftTop += p;
    int row = leftTop.y() / (gridMargin+gridWidth);
    int column = leftTop.x() / (gridMargin+gridWidth);
    return model()->index(row,column);
}
QModelIndex ColorGridView::moveCursor(QAbstractItemView::CursorAction action, Qt::KeyboardModifiers)
{
    //FIXME: keyboard navigation not work well
    QModelIndexList list = selectedIndexes();
    if( !list.isEmpty() ){
        QModelIndex index = list.at(0);
        int row  = index.row();
        int column = index.column();
        int rMax = model()->rowCount()-1;
        int cMax = model()->columnCount()-1;
        switch(action){
        case QAbstractItemView::MoveUp:
            row==0?:row--;
            break;
        case QAbstractItemView::MoveDown:
            row==rMax?:row++;
            break;
        case QAbstractItemView::MoveLeft:
            column==0?:column--;
            break;
        case QAbstractItemView::MoveRight:
            column==cMax?:column++;
            break;
        default:
            break;
        }
        return model()->index(row,column);
    }else{
        return QModelIndex();
    }
}

int ColorGridView::horizontalOffset() const
{
    return horizontalScrollBar()->value();
}
int ColorGridView::verticalOffset() const
{
    return verticalScrollBar()->value();
}

bool ColorGridView::isIndexHidden(const QModelIndex &index) const
{
    return false;
}

void ColorGridView::setSelection(const QRect& area, QFlags<QItemSelectionModel::SelectionFlag> flags)
{
    QPoint topleft = area.topLeft();
    int row = topleft.y() / (gridMargin+gridWidth);
    int column = topleft.x() / (gridMargin+gridWidth);
    selectionModel()->select(model()->index(row, column), flags);
    update();
    return;
}

QRegion ColorGridView::visualRegionForSelection(const QItemSelection& selection) const
{
    QModelIndexList list = selection.indexes();
    if( !list.empty() ){
        QModelIndex index = list.at(0);
        QRect rect(index.column()*(gridWidth+gridMargin), index.row()*(gridWidth+gridMargin),
                   gridWidth, gridWidth);
        if (rect.isValid()){
            return QRegion(rect.left() - horizontalScrollBar()->value(),
                           rect.top() - verticalScrollBar()->value(),
                           width(), height());
        }
        else{
            return rect;
        }
    }else{
        return QRegion();
    }
}

void ColorGridView::setColor(int row, int column, const QColor &color)
{
    model()->setData(model()->index(row,column), color);
    repaint();
}

void ColorGridView::colorPicked()
{
    QModelIndexList list = selectedIndexes();
    if( !list.empty() ){
        QModelIndex index = list.at(0);
        QColor c = model()->data(index).value<QColor>();
        if(c.isValid()){
            emit color(c);
            if(colorWheel){
                colorWheel->setColor(c);
            }
//            repaint();
        }
    }
}

void ColorGridView::colorCleared()
{
    QModelIndexList list = selectedIndexes();
    if( !list.empty() ){
        QModelIndex index = list.at(0);
        model()->setData(index, QColor());
        repaint();
    }
}

void ColorGridView::colorDroped()
{
    QModelIndexList list = selectedIndexes();
    if( !list.empty() ){
        QModelIndex index = list.at(0);
        if(colorWheel){
            model()->setData(index, colorWheel->color());
            repaint();
        }
    }
}


void ColorGridView::setColorWheel(ColorWheel *wheel)
{
    if(wheel){
        colorWheel = wheel;
    }
}
