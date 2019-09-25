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

class QWebSocket;
QT_END_NAMESPACE

#include "CJsonObject.hpp"

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    MainDialog(QWidget *parent = nullptr);
    ~MainDialog();

private:
    QWebSocket *webSocket;

    QLineEdit* addressLineEdit;
    QPushButton* connectButton;
    QPushButton* disconnectButton;
    QPushButton* loginButton;

    QLineEdit* playeridLineEdit;
    QLineEdit* msgidLineEdit;
    QTextEdit* sendTextEdit;
    QPushButton* sendButton;

    QLineEdit* receivedMsgidEdit;
    QTextEdit* receivedTextEdit;

    QMap<QString, QString>* msgidNumberStrMap;

    QStringList* ignoreMsgids;

    ///历史列表
    QListView* filenameListView;
    QStandardItemModel* filenameModel;

private:
    void Connect(const QString& varServerAddr);

    void DisConnect();

    ///发送Json字符串给服务器
    void Send(const QString& varMsgIDStr,const QString& varPlayerIDStr,const QString& varJsonStr);

    void SaveJson();

    void CreateJson();

    void LoadGameMessageJson();

    ///保存当前发送的消息
    void SaveMsg(const QString& varMsgIDStr,const QString& varJsonStr);

    void RefreshHistoryFilelist();

    void LoadOneHistory(const QString& varFileName);

private slots:
    void on_filenameListView_clicked(QModelIndex varModelIndex);
};
#endif // MAINWINDOW_H
