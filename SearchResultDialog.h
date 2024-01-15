#ifndef SEARCHRESULTDIALOG_H
#define SEARCHRESULTDIALOG_H

#include <QDialog>
#include <QString>
#include <QStandardItemModel>
#include <QListView>
#include "CustomItemDelegate.h"
#include <QVBoxLayout>

class SearchResultDialog : public QDialog {
    Q_OBJECT

public:
    SearchResultDialog(const QString &result, QWidget *parent = nullptr);

private slots:
    void handleItemDoubleClicked(const QModelIndex &index);
    void handleItemDeleted(const QModelIndex &rowIndex);
    void handleItemEdited(const QModelIndex &index);
private:
    QString searchString;
    QStandardItemModel *model;
    QListView *dataListView;

    void loadDatabase();
};

#endif // SEARCHRESULTDIALOG_H
