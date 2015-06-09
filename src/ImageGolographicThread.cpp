#include <QtDebug>

#include "ImageGolographicThread.h"

ImageGolographicThread :: ImageGolographicThread (QObject * parent)
    : QThread (parent),
    numb (-1)
{
}

ImageGolographicThread :: ~ImageGolographicThread (void)
{
}

void ImageGolographicThread :: setData (const generatingDataPlus& data)
{
    imageData = data;
    numb = ((imageData.data.XY_angleMax - imageData.data.XY_angleMin)/imageData.data.XY_angleStep+1) *
            ((imageData.data.XZ_angleMax - imageData.data.XZ_angleMin)/imageData.data.XZ_angleStep+1);
    qDebug () << __PRETTY_FUNCTION__ << numb;
}

void ImageGolographicThread :: run (void)
{
    ImageGenerator* generator = new ImageGenerator(imageData);//, this);
    generator->loadModel();
    connect (generator, SIGNAL (imageVal (int)), this, SLOT (setPVal (int)) );
    resD = generator->generateImages();
    delete generator;
    emit getImagesData (resD);
}

int ImageGolographicThread :: getImageNum (void)
{
    return numb;
}

const VGolographic& ImageGolographicThread :: getResults (void) const
{
    return resD;
}

void ImageGolographicThread :: setPVal (int val)
{
    emit setValue (val);
}

void ImageGolographicThread :: setAllVals (int v)
{
    numb = v;
    qDebug () << __PRETTY_FUNCTION__ << numb;
}
