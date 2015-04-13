#ifndef GOLOGRAMMA_H
#define GOLOGRAMMA_H

#include "src_config.h"
#include "cuboid.h"

#include <QProgressDialog>

//programm
void _FFTMATR_EXPORT_ loadModel(mslLoader::OBJloader& loader,const std::string& str);

struct _FFTMATR_EXPORT_ cubPair
{
    Cuboid initialCub;
    std::vector<Cuboid> cubs;
};

cubPair _FFTMATR_EXPORT_ buildCub(const mslLoader::OBJloader& loader,const double lengthOfShip,const double numberOfUnit);

struct _FFTMATR_EXPORT_ constDataStruct
{
    unsigned int XY_plane;
    unsigned int XZ_plane;
    unsigned int imageSize;
    unsigned int numberOfUnit;
};

std::string _FFTMATR_EXPORT_ createFileNameDAT(const constDataStruct& data,const std::string& folder);
std::string _FFTMATR_EXPORT_ createFileNamePNG(const constDataStruct& data,const std::string& folder);

void _FFTMATR_EXPORT_ saveFileDAT(constDataStruct data,std::vector<unsigned char>& image, const std::string& str);
void _FFTMATR_EXPORT_ saveFilePNG(std::vector<unsigned char>& image, const std::string& str);

struct _FFTMATR_EXPORT_ generatingData
{
    double lengthOfShip;
    double numberOfUnit;

    double XY_angleMax;
    double XY_angleMin;
    double XY_angleStep;

    double XZ_angleMax;
    double XZ_angleMin;
    double XZ_angleStep;
};

struct _FFTMATR_EXPORT_ returningData
{
    unsigned int XY_angle;
    unsigned int XZ_angle;
    unsigned int rowNumber;
    unsigned int columnNumber;
    unsigned int numberOfUnit;

    QVector<unsigned char> data;
};

void _FFTMATR_EXPORT_ swap_STDtoQT_vector(QVector<unsigned char>& lhs,std::vector<unsigned char>& rhs);

struct _FFTMATR_EXPORT_ generatingDataPlus
{
    QString filename;
    generatingData data;
};

QVector<returningData> _FFTMATR_EXPORT_ generateImages(const generatingData &data,mslLoader::OBJloader &loader,const std::string &folder);

class _FFTMATR_EXPORT_ ImageGenerator: public QObject
{
    Q_OBJECT

public:
    explicit ImageGenerator(const generatingDataPlus&,QObject* parent = 0);

    void loadModel();
    QVector<returningData> generateImages();

private slots:
	void prCancel (void);

private:
    generatingDataPlus imageData;
    mslLoader::OBJloader loader;

signals:
    void startCreating();
    void createOneImage(int value);
    void createAllImages();
};

#endif // GOLOGRAMMA_H
