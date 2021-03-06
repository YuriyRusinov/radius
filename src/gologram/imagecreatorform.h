#ifndef IMAGECREATORFORM_H
#define IMAGECREATORFORM_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

#include "gologramma.h"

namespace Ui {
class imageCreatorForm;
}

class _FFTMATR_EXPORT_ imageCreatorForm : public QDialog
{
    Q_OBJECT

public:
    explicit imageCreatorForm(QWidget *parent = 0);
    ~imageCreatorForm();

private:
    Ui::imageCreatorForm *ui;

    void formInit();
    int controllSpinBox();

private slots:
    void selectFileName();
    void build();

signals:
    void imagesData(generatingDataPlus);
};

#endif // IMAGECREATORFORM_H
