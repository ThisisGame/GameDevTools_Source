#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QStringList>
#include<QList>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QListView>
#include"MuItemDelegate.h"
#include<QStandardItemModel>


QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE



class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    void SearchToolsInfoFile(const QString& varDirPath);//搜索所有的info.json文件

    void LoadToolsInfoJson();//加载所有的info.json文件

    void RefreshListView(const QList<QJsonObject>& varJsonObjectList);//刷新下拉框

private:
    Ui::Dialog *ui;

    QListView* toolsInfoListView;
    MuItemDelegate *pItemDelegate;
    QStandardItemModel* filenameModel;

    QStringList toolsInfoJsonFilePathList;//工具 info.json 文件地址

    QList<QJsonObject> toolsInfoJsonList;//所有info.json 数据
private slots:
    void on_toolsInfoListView_clicked(QModelIndex varModelIndex);
    void on_keywordLineEdit_textChanged(const QString &arg1);
};
#endif // DIALOG_H
