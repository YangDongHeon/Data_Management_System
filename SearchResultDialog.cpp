#include "SearchResultDialog.h"
#include <QSqlQuery>
#include <QLabel>
#include <iostream>
#include "ItemDetailsWidget.h"
#include <qmessagebox.h>
#include "DataDialog.h"


SearchResultDialog::SearchResultDialog(const QString &result, QWidget *parent) : QDialog(parent), searchString(result) {
    //initialize
    searchString = result;
    model = new QStandardItemModel(this);
    dataListView = new QListView(this);
    dataListView->setModel(model);
    CustomItemDelegate *customDelegate = new CustomItemDelegate(this);
    dataListView->setItemDelegate(customDelegate);

    connect(customDelegate, &CustomItemDelegate::itemDoubleClicked, this, &SearchResultDialog::handleItemDoubleClicked);

    //loadSearchData from database
    loadDatabase();

    //Design Dialog
    setWindowTitle("Search Result");
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(dataListView);
    setLayout(layout);
}

void SearchResultDialog::loadDatabase() {
     // Load existing data from the database and update the model
    model->clear();
    model->setHorizontalHeaderLabels(QStringList() <<"Id" << "ShopName" << "ModelNumber" 
                                                << "ModelName" << "Weight14K" << "Weight18K"
                                                << "GemQuantity" << "BasicLaborCharge" << "GemLaborCharge" 
                                                << "DiaCharge" << "Etc" << "picturePath");

    QSqlQuery query;
    query.prepare("SELECT * FROM items WHERE shop_name LIKE :shopName "
              "OR model_number LIKE :modelNumber OR model_name LIKE :modelName");
    query.bindValue(":shopName", "%" + searchString + "%");
    query.bindValue(":modelNumber", "%" + searchString + "%");
    query.bindValue(":modelName", "%" + searchString + "%");
    query.exec();

    while (query.next()) {
        int Id = query.value("id").toInt();
        QString ShopName = query.value("shop_name").toString();
        QString ModelNumber = query.value("model_number").toString();
        QString ModelName = query.value("model_name").toString();
        QString Weight14K = query.value("weight_14k").toString();
        QString Weight18K = query.value("weight_18k").toString();
        QString GemQuantity = query.value("gem_quantity").toString();
        QString BasicLaborCharge = query.value("basic_labor_charge").toString();
        QString GemLaborCharge = query.value("gem_labor_charge").toString();
        QString DiaCharge = query.value("dia_charge").toString();
        QString Etc = query.value("etc").toString();
        QString picturePath = query.value("picture_path").toString();

        QList<QStandardItem *> rowItems;
        rowItems << new QStandardItem(QString::number(Id))
                 << new QStandardItem(ShopName)
                 << new QStandardItem(ModelNumber)
                 << new QStandardItem(ModelName)
                 << new QStandardItem(Weight14K)
                 << new QStandardItem(Weight18K)
                 << new QStandardItem(GemQuantity)
                 << new QStandardItem(BasicLaborCharge)
                 << new QStandardItem(GemLaborCharge)
                 << new QStandardItem(DiaCharge)
                 << new QStandardItem(Etc)
                 << new QStandardItem(picturePath);

        model->appendRow(rowItems);
    }
};

void SearchResultDialog::handleItemDoubleClicked(const QModelIndex &index) {
    ItemDetailsWidget* detailsWidget = new ItemDetailsWidget(index,this);
    connect(detailsWidget, &ItemDetailsWidget::itemDeleted, this, &SearchResultDialog::handleItemDeleted);
    connect(detailsWidget, &ItemDetailsWidget::itemEdited, this, &SearchResultDialog::handleItemEdited);
    // Create a QDialog to contain the ItemDetailsWidget
    QDialog *detailsDialog = new QDialog(this);
    detailsDialog->setWindowTitle("Item Details");

    // Create a layout for the detailsDialog
    QVBoxLayout *layout = new QVBoxLayout(detailsDialog);
    layout->addWidget(detailsWidget);

    // Show the detailsDialog
    detailsDialog->exec();
}

void SearchResultDialog::handleItemDeleted(const QModelIndex &rowIndex) {
    int itemId = rowIndex.sibling(rowIndex.row(), 0).data().toInt(); // Assuming 0 is the column index of 'id'

    // Delete the item from the database using the itemId
    QSqlQuery query;
    query.prepare("DELETE FROM items WHERE id = :itemId");
    query.bindValue(":itemId", itemId);

    if (query.exec()) {
        // Successful deletion
        loadDatabase(); // Reload the data after deletion
    } else {
        // Error handling
        QMessageBox::critical(this, "Error", "Failed to delete item: " + query.lastError().text());
    }
    loadDatabase();
}

void SearchResultDialog::handleItemEdited(const QModelIndex &index) {
    // Retrieve the data from the model using the provided index
    int Id = index.sibling(index.row(),0).data().toInt();
    QString ShopName = index.sibling(index.row(), 1).data().toString();
    QString ModelNumber = index.sibling(index.row(), 2).data().toString();
    QString ModelName = index.sibling(index.row(), 3).data().toString();
    QString Weight14K = index.sibling(index.row(), 4).data().toString();
    QString GemQuantity = index.sibling(index.row(), 6).data().toString();
    QString BasicLaborCharge = index.sibling(index.row(), 7).data().toString();
    QString GemLaborCharge = index.sibling(index.row(), 8).data().toString();
    QString DiaCharge = index.sibling(index.row(), 9).data().toString();
    QString Etc = index.sibling(index.row(), 10).data().toString();
    QString picturePath = index.sibling(index.row(), 11).data().toString();


    // Create a dialog for editing the data
    DataDialog editDataDialog(DataDialog::DialogMode::Edit, this);
    editDataDialog.setWindowTitle("Edit Data");
    
    // Set the initial values in the dialog
    editDataDialog.setShopName(ShopName);
    editDataDialog.setModelNumber(ModelNumber);
    editDataDialog.setModelName(ModelName);
    editDataDialog.setWeight14K(Weight14K);
    editDataDialog.setGemQuantity(GemQuantity);
    editDataDialog.setBasicLaborCharge(BasicLaborCharge);
    editDataDialog.setGemLaborCharge(GemLaborCharge);
    editDataDialog.setDiaCharge(DiaCharge);
    editDataDialog.setEtc(Etc);
    editDataDialog.setPicturePath(picturePath);
    editDataDialog.setId(Id);

    // Show the dialog
    editDataDialog.exec();
    loadDatabase();
}
