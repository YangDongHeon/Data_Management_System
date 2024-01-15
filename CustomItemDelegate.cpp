#include "CustomItemDelegate.h"
#include <QMouseEvent>
CustomItemDelegate::CustomItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

bool CustomItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index){
    if (event->type() == QEvent::MouseButtonDblClick)
        {
            emit itemDoubleClicked(index);
            return true; // Event handled
        }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

void CustomItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QString ShopName = index.sibling(index.row(), 1).data().toString();
    QString ModelNumber = index.sibling(index.row(), 2).data().toString();
    QString ModelName = index.sibling(index.row(), 3).data().toString();
    
    QString picturePath = index.sibling(index.row(), 11).data().toString();

    // Load the image from the file path
    QPixmap image(picturePath);

    // Draw the image
    QRect imageRect(option.rect.topLeft(), QSize(option.rect.height(), option.rect.height()));
    painter->drawPixmap(imageRect, image, image.rect());

    // Draw the text beside the image
    QRect textRect(option.rect.topLeft() + QPoint(option.rect.height(), 0), option.rect.bottomRight());
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, QString("업체명: %1\n모델번호: %2\n모델이름: %3")
                                                                    .arg(ShopName, ModelNumber, ModelName));
}

QSize CustomItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    // Set the size of each item
    return QSize(300, 100);
}
