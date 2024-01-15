// ItemDetailsWidget.h
#ifndef ITEMDETAILSWIDGET_H
#define ITEMDETAILSWIDGET_H

#include <QWidget>
#include <QModelIndex>
#include <QSqlQuery>
#include <QSqlError>

class ItemDetailsWidget : public QWidget {
    Q_OBJECT

public:
    ItemDetailsWidget(const QModelIndex &index, QWidget *parent = nullptr);

signals:
    void itemDeleted(const QModelIndex &rowIndex);
    void itemEdited(const QModelIndex &itemIndex);

private slots:
    void editButtonClicked();
    void deleteButtonClicked();

private:
    QModelIndex itemIndex;
};

#endif // ITEMDETAILSWIDGET_H
