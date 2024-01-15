#ifndef DATADIALOG_H
#define DATADIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QString>
#include <QFileDialog>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

class DataDialog : public QDialog {
    Q_OBJECT

public:

    enum class DialogMode {
        Add,
        Edit
    };

    DataDialog(DialogMode mode, QWidget *parent = nullptr);
    QString getShopName() const;
    QString getModelNumber() const;
    QString getModelName() const;
    QString getWeight14K() const;
    QString getWeight18K() const;
    QString getGemQuantity() const;
    QString getBasicLaborCharge() const;
    QString getGemLaborCharge() const;
    QString getDiaCharge() const;
    QString getEtc() const;
    QString getPicturePath() const;

    void setShopName(const QString &ShopName);
    void setModelNumber(const QString &ModelNumber);
    void setModelName(const QString &ModelName);
    void setWeight14K(const QString &Weight14K);
    void setWeight18K(const QString &Weight18K);
    void setGemQuantity(const QString &GemQuantity);
    void setBasicLaborCharge(const QString &BasicLaborCharge);
    void setGemLaborCharge(const QString &GemLaborCharge);
    void setDiaCharge(const QString &DiaCharge);
    void setEtc(const QString &Etc);
    void setPicturePath(const QString &PicturePath);

    void setId(int id);
private slots:
    void browsePicture();
    void addData();
    void updateData();

private:
    QLineEdit *ShopNameEdit;
    QLineEdit *ModelNumberEdit;
    QLineEdit *ModelNameEdit;
    QLineEdit *Weight14KEdit;
    QLineEdit *Weight18KEdit;
    QLineEdit *GemQuantityEdit;
    QLineEdit *BasicLaborChargeEdit;
    QLineEdit *GemLaborChargeEdit;
    QLineEdit *DiaChargeEdit;
    QLineEdit *EtcEdit;
    QString picturePath;
    DialogMode dialogMode;
    int id;
};
#endif // DATADIALOG_H
