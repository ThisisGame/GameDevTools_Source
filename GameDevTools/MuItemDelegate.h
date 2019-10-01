#ifndef MUITEMDELEGATE_H
#define MUITEMDELEGATE_H



#include <QStyledItemDelegate>

#include <QMetaType>

#include<QJsonObject>

typedef struct {
    QString toolName;
    QJsonObject jsonObject;
} MuItemData;

Q_DECLARE_METATYPE(MuItemData)

class MuItemDelegate : public QStyledItemDelegate
{
public:
    MuItemDelegate(QObject *parent = nullptr);

    // painting
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // MUITEMDELEGATE_H
