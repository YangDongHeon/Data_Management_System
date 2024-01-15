// ItemDetailsWidget.cpp
#include "ItemDetailsWidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlQuery>
#include <iostream>

ItemDetailsWidget::ItemDetailsWidget(const QModelIndex &index, QWidget *parent)
    : QWidget(parent), itemIndex(index) {

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Load data from the model using the provided index
    QString ShopName = index.sibling(index.row(), 1).data().toString();
    QString ModelNumber = index.sibling(index.row(), 2).data().toString();
    QString ModelName = index.sibling(index.row(), 3).data().toString();
    QString Weight14K = index.sibling(index.row(), 4).data().toString();
    QString Weight18K = index.sibling(index.row(), 5).data().toString();
    QString GemQuantity = index.sibling(index.row(), 6).data().toString();
    QString BasicLaborCharge = index.sibling(index.row(), 7).data().toString();
    QString GemLaborCharge = index.sibling(index.row(), 8).data().toString();
    QString DiaCharge = index.sibling(index.row(), 9).data().toString();
    QString Etc = index.sibling(index.row(), 10).data().toString();
    QString picturePath = index.sibling(index.row(), 11).data().toString();

    // Display the picture
    QLabel *pictureLabel = new QLabel(this);
    pictureLabel->setPixmap(QPixmap(picturePath).scaledToWidth(300)); // Adjust the size as needed
    layout->addWidget(pictureLabel);

    // Display information
    QLabel *infoLabel = new QLabel(this);
    QString infoText = QString("업체명: %1\n모델번호: %2\n모델이름: %3\n"
                               "14K금중량: %4      18K금중량: %5\n알수: %6\n"
                               "기/추: %7\n중/보: %8\n랩다이아: %9\n"
                               "기타정보: %10")
                               .arg(ShopName)
                               .arg(ModelNumber)
                               .arg(ModelName)
                               .arg(Weight14K)
                               .arg(Weight18K)
                               .arg(GemQuantity)
                               .arg(BasicLaborCharge)
                               .arg(GemLaborCharge)
                               .arg(DiaCharge)
                               .arg(Etc);

    infoLabel->setText(infoText);
    layout->addWidget(infoLabel);

    // Add buttons for edit and delete
    QPushButton *editButton = new QPushButton("Edit", this);
    connect(editButton, &QPushButton::clicked, this, &ItemDetailsWidget::editButtonClicked);
    layout->addWidget(editButton);

    QPushButton *deleteButton = new QPushButton("Delete", this);
    connect(deleteButton, &QPushButton::clicked, this, &ItemDetailsWidget::deleteButtonClicked);
    layout->addWidget(deleteButton);

    setLayout(layout);
}


void ItemDetailsWidget::deleteButtonClicked() {
    // Implement the delete functionality
    // Display a confirmation dialog and delete the item if confirmed
    int result = QMessageBox::question(this, "Delete Item", "Are you sure you want to delete this item?",
                                       QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        emit itemDeleted(itemIndex);
        close();
    }
}

void ItemDetailsWidget::editButtonClicked() {
    // Emit the itemEdited signal with the index of the item being edited
    emit itemEdited(itemIndex);
    close();
}
