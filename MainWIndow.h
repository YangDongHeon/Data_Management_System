#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QListView>
#include <QLineEdit>
#include "CustomItemDelegate.h"
#include "ItemDetailsWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

signals:


private slots:
    void searchButtonClicked();
    void addButtonClicked();
    void handleItemDoubleClicked(const QModelIndex &index);
    void handleItemDeleted(const QModelIndex &rowIndex);
    void handleItemEdited(const QModelIndex &index);

private:
    void setupUI();
    void setupDatabase();
    void loadDatabase();

    QStandardItemModel *model;
    QListView *dataListView;
    QLineEdit *searchBox;
    QString searchtext;
};

#endif // MAINWINDOW_H