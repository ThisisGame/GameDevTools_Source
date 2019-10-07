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

    void Ex_7z(const QJsonObject& jsonObject);//解压7z文件

    void Start(const QJsonObject& jsonObject);//启动工具

private:
    Ui::Dialog *ui;

    MuItemDelegate *pItemDelegate;
    QStandardItemModel* filenameModel;

    QStringList toolsInfoJsonFilePathList;//工具 info.json 文件地址

    QList<QJsonObject> toolsInfoJsonList;//所有info.json 数据

    bool unzipWorking;//是否在进行解压的工作。
private slots:
    void on_toolsInfoListView_clicked(QModelIndex varModelIndex);
    void on_keywordLineEdit_textChanged(const QString &arg1);
    void on_keywordLineEdit_BG_returnPressed();
    void on_keywordLineEdit_returnPressed();
    void on_toolsInfoListView_doubleClicked(const QModelIndex &index);
    void onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void on_quitAction();
};
#endif // DIALOG_H
