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
#include<QSystemTrayIcon>


#include "downloadmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE




class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

    void keyPressEvent(QKeyEvent *) override;

private:
    void SearchToolsInfoFile(const QString& varDirPath);//搜索所有的info.json文件

    void LoadToolsInfoJson();//加载所有的info.json文件

    void RefreshListView(const QList<QJsonObject>& varJsonObjectList);//刷新下拉框

    int Ex_7z(const QJsonObject& jsonObject);//解压7z文件 1正在下载 -1其他错误

    void Download_7z(const QJsonObject& jsonObject);//下载7z文件

    void Start(const QJsonObject& jsonObject);//启动工具

    void ModifyState(const QString& varName,const int varState);//修改状态

    bool Exist_7z(const QJsonObject& jsonObject);//判断压缩文件是否存在;

    bool Exist_Tool(const QJsonObject& jsonObject);//判断文件是否已经解压;

private:
    Ui::Dialog *ui;

    MuItemDelegate *pItemDelegate;
    QStandardItemModel* filenameModel;

    QStringList toolsInfoJsonFilePathList;//工具 info.json 文件地址

    QList<QJsonObject> toolsInfoJsonList;//所有info.json 数据

    bool unzipWorking;//是否在进行解压的工作。

    DownloadManager downloadManager;//下载管理器
private slots:
    void on_toolsInfoListView_clicked(QModelIndex varModelIndex);
    void on_keywordLineEdit_textChanged(const QString &arg1);
    void on_keywordLineEdit_BG_returnPressed();
    void on_keywordLineEdit_returnPressed();
    void on_toolsInfoListView_doubleClicked(const QModelIndex &index);
    void onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void on_quitAction();

private slots:
    void downloadProgress(const QString& name,qint64 bytesReceived, qint64 bytesTotal,const QString& speedDesc);
    void finishedOne(const QString& name);
};
#endif // DIALOG_H
