#ifndef CUSTOMITEMDELEGATE_H
#define CUSTOMITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QPixmap>

class CustomItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    CustomItemDelegate(QObject *parent = nullptr);
signals:
    void itemDoubleClicked(const QModelIndex &index);

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // CUSTOMITEMDELEGATE_H
