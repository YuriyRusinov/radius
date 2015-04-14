#include <QImage>
#include <QPixmap>
#include <QtDebug>
#include "golographicModel.h"

GolographicModel :: GolographicModel (const QVector<golographicData>& imData, QObject * parent)
    : QAbstractTableModel (parent),
    gData (imData)
{
}

GolographicModel :: ~GolographicModel (void)
{
}

int GolographicModel :: columnCount (const QModelIndex& /*parent*/) const
{
    return 3;
}

int GolographicModel :: rowCount (const QModelIndex& /*parent*/) const
{
    int n = gData.size ();
    return n;
}

QVariant GolographicModel :: data (const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant ();

    golographicData rGol = gData[index.row()];
    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
            return tr("Azimuth is %1").arg(rGol.XY_angle);
        else if (index.column() == 1)
            return tr("Obliquity is %1").arg (rGol.XZ_angle);
        else
            return QVariant();
    }
    else if (role == Qt::DecorationRole && index.column () == 2)
    {
        QByteArray bData;
        bData.clear ();
        int nWidth = rGol.rowNumber;
        int nHeight = rGol.columnNumber;
        int ncount (0);
        QImage pIm (nWidth, nHeight, QImage::Format_ARGB32);
        QVector<QRgb> grayscale;

        for (int i = 0; i < 256; ++i)
            grayscale.append(qRgb(i, i, i));
        for (int i=0; i<nWidth; i++)
            for (int j=0; j<nHeight; j++)
            {
                uint c = (uint)rGol.data.at(ncount++);//.toUInt();
                c *= 255;
                pIm.setPixel(i, j, qRgb(c,c,c));
            }
        return QVariant (QPixmap::fromImage (pIm));
    }
    else
        return QVariant ();
}

QVariant GolographicModel :: headerData (int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return tr("Azimuth");

            case 1:
                return tr("Obliquity");

            case 2:
                return tr("Image of ship");

            default:
                return QVariant();
        }
    }
    return QVariant();
}


