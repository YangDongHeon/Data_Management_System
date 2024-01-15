#include <qboxlayout.h>
#include<QLabel>
#include "DataDialog.h"

DataDialog::DataDialog(DialogMode mode, QWidget *parent) : QDialog(parent) {
    DataDialog::dialogMode = mode;

    QVBoxLayout *layout = new QVBoxLayout(this);

    ShopNameEdit = new QLineEdit(this);
    layout->addWidget(new QLabel("업체명:", this));
    layout->addWidget(ShopNameEdit);

    ModelNumberEdit = new QLineEdit(this);
    layout->addWidget(new QLabel("모델번호:", this));
    layout->addWidget(ModelNumberEdit);

    ModelNameEdit = new QLineEdit(this);
    layout->addWidget(new QLabel("모델이름:", this));
    layout->addWidget(ModelNameEdit);

    Weight14KEdit = new QLineEdit(this);
    layout->addWidget(new QLabel("14K 금중량:", this));
    layout->addWidget(Weight14KEdit);

    GemQuantityEdit = new QLineEdit(this);
    layout->addWidget(new QLabel("알수:", this));
    layout->addWidget(GemQuantityEdit);

    BasicLaborChargeEdit = new QLineEdit(this);
    layout->addWidget(new QLabel("기/추:", this));
    layout->addWidget(BasicLaborChargeEdit);

    GemLaborChargeEdit = new QLineEdit(this);
    layout->addWidget(new QLabel("중/보:", this));
    layout->addWidget(GemLaborChargeEdit);

    DiaChargeEdit = new QLineEdit(this);
    layout->addWidget(new QLabel("랩다이아:", this));
    layout->addWidget(DiaChargeEdit);

    EtcEdit = new QLineEdit(this);
    layout->addWidget(new QLabel("기타정보:", this));
    layout->addWidget(EtcEdit);

    QPushButton *browseButton = new QPushButton("Browse Picture", this);
    connect(browseButton, &QPushButton::clicked, this, &DataDialog::browsePicture);
    layout->addWidget(browseButton);

    QPushButton *actionButton;
    if (dialogMode == DialogMode::Add) {
        actionButton = new QPushButton("Add", this);
        connect(actionButton, &QPushButton::clicked, this, &DataDialog::addData);
        setWindowTitle("Add Data");
    } else {
        actionButton = new QPushButton("Edit", this);
        connect(actionButton, &QPushButton::clicked, this, &DataDialog::updateData);
        setWindowTitle("Edit Data");
    }
    
    layout->addWidget(actionButton);

    setLayout(layout);
}

void DataDialog::browsePicture() {
    QString fileName = QFileDialog::getOpenFileName(this, "Select Picture", QDir::homePath(), "Images (*.png *.jpg *.bmp *.jpeg)");
    if (!fileName.isEmpty()) {
        picturePath = fileName;
        qDebug() << "Selected picture:" << picturePath;
    }
}

void DataDialog::addData() {
    QString ShopName = this -> getShopName();
    QString ModelNumber = this -> getModelNumber();
    QString ModelName = this -> getModelName();
    QString Weight14K = this -> getWeight14K();
    QString Weight18K =  QString::number(Weight14K.toDouble()*1.2);
    QString GemQuantity = this -> getGemQuantity();
    QString BasicLaborCharge = this -> getBasicLaborCharge();
    QString GemLaborCharge = this -> getGemLaborCharge();
    QString DiaCharge = this -> getDiaCharge();
    QString Etc = this -> getEtc();
    QString PicturePath = this -> getPicturePath();

    // Insert data into the database
    QSqlQuery query;
    query.prepare("INSERT INTO items (shop_name, model_number, "
                "model_name, weight_14k, weight_18k, gem_quantity, basic_labor_charge, "
                "gem_labor_charge, dia_charge, etc, picture_path) VALUES"
                " (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(ShopName);
    query.addBindValue(ModelNumber);
    query.addBindValue(ModelName);
    query.addBindValue(Weight14K);
    query.addBindValue(Weight18K);
    query.addBindValue(GemQuantity);
    query.addBindValue(BasicLaborCharge);
    query.addBindValue(GemLaborCharge);
    query.addBindValue(DiaCharge);
    query.addBindValue(Etc);
    query.addBindValue(picturePath);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Data added successfully!");
        accept(); // Close the dialog
    } else {
        QMessageBox::critical(this, "Error", "Failed to add data: " + query.lastError().text());
    }
}

void DataDialog::updateData(){
    QString ShopName = this -> getShopName();
    QString ModelNumber = this -> getModelNumber();
    QString ModelName = this -> getModelName();
    QString Weight14K = this -> getWeight14K();
    QString Weight18K =  QString::number(Weight14K.toDouble()*1.2);
    QString GemQuantity = this -> getGemQuantity();
    QString BasicLaborCharge = this -> getBasicLaborCharge();
    QString GemLaborCharge = this -> getGemLaborCharge();
    QString DiaCharge = this -> getDiaCharge();
    QString Etc = this -> getEtc();
    QString PicturePath = this -> getPicturePath();
    // Update the database with the edited values
    QSqlQuery query;
    
    query.prepare("UPDATE items SET shop_name = :ShopName, model_number = :ModelNumber, "
                "model_name = :ModelName, weight_14k = :Weight14K, weight_18k = :Weight18K, "
                "gem_quantity = :GemQuantity, basic_labor_charge = :BasicLaborCharge, gem_labor_charge = :GemLaborCharge, "
                "dia_charge = :DiaCharge, etc = :Etc, "
                "picture_path = :picturePath "
                "WHERE id = :id");
    query.bindValue(":ShopName", ShopName);
    query.bindValue(":ModelNumber", ModelNumber);
    query.bindValue(":ModelName", ModelName);
    query.bindValue(":Weight14K", Weight14K);
    query.bindValue(":Weight18K", Weight18K);
    query.bindValue(":GemQuantity", GemQuantity);
    query.bindValue(":BasicLaborCharge", BasicLaborCharge);
    query.bindValue(":GemLaborCharge", GemLaborCharge);
    query.bindValue(":DiaCharge", DiaCharge);
    query.bindValue(":Etc", Etc);
    query.bindValue(":picturePath", picturePath);
    query.bindValue(":id", id);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Data Edited successfully!");
        accept(); // Close the dialog
    } else {
        QMessageBox::critical(this, "Error", "Failed to Edit data: " + query.lastError().text());
    }

}


// Implement the get~() functions
QString DataDialog::getShopName() const {
    return ShopNameEdit->text();
}

QString DataDialog::getModelNumber() const {
    return ModelNumberEdit->text();
}

QString DataDialog::getModelName() const {
    return ModelNameEdit->text();
}

QString DataDialog::getWeight14K() const {
    return Weight14KEdit->text();
}

QString DataDialog::getGemQuantity() const {
    return GemQuantityEdit->text();
}

QString DataDialog::getBasicLaborCharge() const {
    return BasicLaborChargeEdit->text();
}

QString DataDialog::getGemLaborCharge() const {
    return GemLaborChargeEdit->text();
}

QString DataDialog::getDiaCharge() const {
    return DiaChargeEdit->text();
}

QString DataDialog::getEtc() const {
    return EtcEdit->text();
}

QString DataDialog::getPicturePath() const {
    return picturePath;
}

void DataDialog::setShopName(const QString &ShopName) {
    ShopNameEdit->setText(ShopName);
}

void DataDialog::setModelNumber(const QString &ModelNumber) {
    ModelNumberEdit->setText(ModelNumber);
}

void DataDialog::setModelName(const QString &ModelName) {
    ModelNameEdit->setText(ModelName);
}

void DataDialog::setWeight14K(const QString &Weight14K) {
    Weight14KEdit->setText(Weight14K);
}

void DataDialog::setGemQuantity(const QString &GemQuantity) {
    GemQuantityEdit->setText(GemQuantity);
}

void DataDialog::setBasicLaborCharge(const QString &BasicLaborCharge) {
    BasicLaborChargeEdit->setText(BasicLaborCharge);
}

void DataDialog::setGemLaborCharge(const QString &GemLaborCharge) {
    GemLaborChargeEdit->setText(GemLaborCharge);
}

void DataDialog::setDiaCharge(const QString &DiaCharge) {
    DiaChargeEdit->setText(DiaCharge);
}

void DataDialog::setEtc(const QString &Etc) {
    EtcEdit->setText(Etc);
}

void DataDialog::setPicturePath(const QString &PicturePath) {
    picturePath = PicturePath;
}

void DataDialog::setId(int id){
    this->id = id;
}