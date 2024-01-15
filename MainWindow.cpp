#include "MainWIndow.h"
#include "DataDialog.h"
#include "SearchResultDialog.h"
#include "CustomItemDelegate.h"
#include "ItemDetailsWidget.h"
#include <QLabel>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
    setupDatabase();
    loadDatabase();
    resize(800, 600);
}

void MainWindow::setupUI() {
    // Set up the main layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Search components
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchBox = new QLineEdit(this);
    QPushButton *searchButton = new QPushButton("Search", this);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchButtonClicked);
    searchLayout->addWidget(searchBox);
    searchLayout->addWidget(searchButton);

    // Add button
    QPushButton *addButton = new QPushButton("Add", this);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addButtonClicked);

    // Data list
    model = new QStandardItemModel(this);
    dataListView = new QListView(this);
    dataListView->setModel(model);

    CustomItemDelegate *customDelegate = new CustomItemDelegate(this);
    dataListView->setItemDelegate(customDelegate);
    connect(customDelegate, &CustomItemDelegate::itemDoubleClicked, this, &MainWindow::handleItemDoubleClicked);

    // Add components to the main layout
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(addButton);
    mainLayout->addWidget(dataListView);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

void MainWindow::setupDatabase() {
    // Open the SQLite database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data.sqlite");

    if (!db.open()) {
        QMessageBox::critical(nullptr, "Database Error", "Couldn't open the database: " + db.lastError().text());
        return;
    }

    // Ensure the table is created
    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS items ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "shop_name TEXT, "
                    "model_number TEXT, "
                    "model_name TEXT, "
                    "weight_14k TEXT, "
                    "weight_18k TEXT, "
                    "gem_quantity TEXT, "
                    "basic_labor_charge, "
                    "gem_labor_charge TEXT, "
                    "dia_charge TEXT, "
                    "etc TEXT, "
                    "picture_path TEXT)")) {
        QMessageBox::critical(nullptr, "Database Error", "Failed to create table: " + query.lastError().text());
        return;
    }
}

void MainWindow::loadDatabase() {
    // Load existing data from the database and update the model
    model->clear();
    model->setHorizontalHeaderLabels(QStringList() <<"Id" << "ShopName" << "ModelNumber" 
                                                << "ModelName" << "Weight14K" << "Weight18K"
                                                << "GemQuantity" << "BasicLaborCharge" << "GemLaborCharge" 
                                                << "DiaCharge" << "Etc" << "picturePath");

    QSqlQuery query("SELECT * FROM items");
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
}

void MainWindow::searchButtonClicked() {
    searchtext = searchBox->text();
    std::cout << searchtext.toStdString() << std::endl;
    // Perform your search based on 'searchName'
    // You can customize this part based on your needs
    SearchResultDialog resultDialog(searchtext, this);
    resultDialog.exec();
    loadDatabase();
}

void MainWindow::addButtonClicked() {
    DataDialog addDataDialog(DataDialog::DialogMode::Add , this);
    if (addDataDialog.exec() == QDialog::Accepted) {
        loadDatabase(); // Reload the data after adding new data
    }
}

void MainWindow::handleItemDoubleClicked(const QModelIndex &index) {
    ItemDetailsWidget* detailsWidget = new ItemDetailsWidget(index,this);
    connect(detailsWidget, &ItemDetailsWidget::itemDeleted, this, &MainWindow::handleItemDeleted);
    connect(detailsWidget, &ItemDetailsWidget::itemEdited, this, &MainWindow::handleItemEdited);
    // Create a QDialog to contain the ItemDetailsWidget
    QDialog *detailsDialog = new QDialog(this);
    detailsDialog->setWindowTitle("Item Details");

    // Create a layout for the detailsDialog
    QVBoxLayout *layout = new QVBoxLayout(detailsDialog);
    layout->addWidget(detailsWidget);

    // Show the detailsDialog
    detailsDialog->exec();
}

void MainWindow::handleItemDeleted(const QModelIndex &rowIndex) {
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

void MainWindow::handleItemEdited(const QModelIndex &index) {
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