#include "colorgridmodel.h"

ColorGridModel::ColorGridModel(QObject *parent) :
    QAbstractTableModel(parent),
    capacity(10)
{
    for(int i=0; i<capacity;++i){
        colors.append(QList<QColor>());
        for(int j=0;j<capacity;++j){
            colors[i].append(QColor());
        }
    }
}

int ColorGridModel::rowCount(const QModelIndex &) const
{
    return capacity;
}
int ColorGridModel::columnCount(const QModelIndex &) const
{
    return capacity;
}
QVariant ColorGridModel::data(const QModelIndex &index, int role) const
{
    if( capacity > index.row() ){
        if( capacity > index.column() ){
            QColor color = colors.at( index.row() ).at( index.column() );
            return color;
        }
    }
    return QColor();

}

bool ColorGridModel::setData(const QModelIndex & index, const QVariant & value, int role )
{
    if(index.row()> capacity || index.column()> capacity){
        return false;
    }

    colors[index.row()][index.column()] = value.value<QColor>();
    emit dataChanged(index, index);
    return true;
}

