#include <QPainter>
#include <QDebug>
#include "MuItemDelegate.h"

MuItemDelegate::MuItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

void MuItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid()) {
        painter->save();
        QVariant var = index.data(Qt::UserRole+1);
        MuItemData itemData = var.value<MuItemData>();

        // item 矩形区域
        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth(option.rect.width()-1);
        rect.setHeight(option.rect.height()-1);

        QPainterPath path;
        path.moveTo(rect.topRight());
        path.lineTo(rect.topLeft());
        path.quadTo(rect.topLeft(), rect.topLeft());
        path.lineTo(rect.bottomLeft());
        path.quadTo(rect.bottomLeft(), rect.bottomLeft());
        path.lineTo(rect.bottomRight());
        path.quadTo(rect.bottomRight(), rect.bottomRight());
        path.lineTo(rect.topRight());
        path.quadTo(rect.topRight(), rect.topRight());

        // 鼠标悬停或者选中时改变背景色
        if (option.state.testFlag(QStyle::State_MouseOver)) {
            painter->setPen(QPen(QColor("#ebeced")));
            painter->setBrush(QColor("#ebeced"));
            painter->drawPath(path);
        }
        if (option.state.testFlag(QStyle::State_Selected)) {
            painter->setPen(QPen(QColor("#e3e3e5")));
            painter->setBrush(QColor("#e3e3e5"));
            painter->drawPath(path);
        }

        // 绘制工具名字，工具描述位置区域
        QRectF nameRect = QRect(rect.left()+5, rect.top()+5,  rect.width()-10, 20);
        QRectF descRect = QRect(nameRect.left(), nameRect.bottom()+5, rect.width()-10, 20);

        //painter->drawImage(iconRect, QImage(itemData.iconPath));
        painter->setPen(QPen(Qt::black));
        painter->setFont(QFont("等线", 10));
        if(itemData.jsonObject.contains("name"))
        {
            int state=itemData.jsonObject["state"].toInt();
            switch(state)
            {
            case TOOL_STATE_NOT_DOWNLOAD:
                painter->drawText(nameRect, itemData.jsonObject["name"].toString()+" 未下载");
                break;
            case TOOL_STATE_DOWNLOADING:
            {
                qint64 received=itemData.jsonObject["bytesReceived"].toInt();
                qint64 total=itemData.jsonObject["bytesTotal"].toInt();
                painter->drawText(nameRect, itemData.jsonObject["name"].toString()+" 下载中... "+QString::number(received/1000000.0, 'f', 2)+"/"+QString::number(total/1000000.0, 'f', 2)+"mb");
                break;
            }
            case TOOL_STATE_UNZIP:
//                painter->drawText(nameRect, itemData.jsonObject["name"].toString()+" 未解压");
//                break;
            case TOOL_STATE_UNZIPING:
//                painter->drawText(nameRect, itemData.jsonObject["name"].toString()+" 解压中...");
//                break;
            case TOOL_STATE_UNZIP_FINISH:
                painter->drawText(nameRect, itemData.jsonObject["name"].toString());
                break;
            }
        }

        painter->setPen(QPen(Qt::gray));
        painter->setFont(QFont("等线", 9));
        if(itemData.jsonObject.contains("desc"))
        {
            painter->drawText(descRect, itemData.jsonObject["desc"].toString());
        }


        painter->restore();
    }
}

QSize MuItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QSize(option.rect.width(), 50);
}
