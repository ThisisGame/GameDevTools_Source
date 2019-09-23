#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QModelIndex>
QT_BEGIN_NAMESPACE
class QCheckBox;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QScrollArea;

class QListView;
class QStringListModel;
class QStandardItemModel;
class QModelIndex;
class QJsonObject;
QT_END_NAMESPACE

#include "CJsonObject.hpp"

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    MainDialog(QWidget *parent = nullptr);
    ~MainDialog();

private:

    /// 创建一个协议的界面
    /// @variable varTypeStr 标题，客户端 or 服务器
    QWidget* CreateProtocalPart(const QString& varTypeStr,neb::CJsonObject& varProtocalJsonObject);

    /// 创建一对协议的界面
    QWidget* CreateProtocal(neb::CJsonObject& varJsonObject);

    /// 创建协议列表的界面
    void CreateProtocalArray(const QString& varJsonFilePath);

    ///更新Json文件
    void SaveCurrentJsonFile();


private:
    ///右侧协议列表 滚动
    QScrollArea* scrollArea;

    QListView* filenameListView;
    QStandardItemModel* filenameModel;

    QString fileDirPath;
    QString filePath;
    neb::CJsonObject* rootCJsonObject;

private slots:
    void on_chooseDirPushButton_clicked();
    void on_filenameListView_clicked(QModelIndex varModelIndex);
    void on_filenameListView_doubleClicked();
};
#endif // MAINWINDOW_H
