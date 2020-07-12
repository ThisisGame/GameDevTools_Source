#ifndef MUITEMDELEGATE_H
#define MUITEMDELEGATE_H



#include <QStyledItemDelegate>

#include <QMetaType>

#include<QJsonObject>

enum TOOL_STATE
{
    TOOL_STATE_NOT_DOWNLOAD=0,//未下载
    TOOL_STATE_DOWNLOADING=1,//下载中
    TOOL_STATE_UNZIP,//下载完成未解压
    TOOL_STATE_UNZIPING,//解压中
    TOOL_STATE_UNZIP_FINISH,//解压完成
};

typedef struct {
    QString toolName;
    QJsonObject jsonObject;
    qint64 bytesReceived;
    qint64 bytesTotal;
    int state;
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
