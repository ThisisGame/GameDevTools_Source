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

    QLineEdit* playeridLineEdit;
    QLineEdit* msgidLineEdit;
    QTextEdit* sendTextEdit;

    QLineEdit* receivedMsgidEdit;
    QTextEdit* receivedTextEdit;

private:
    void Connect(const QString& varServerAddr);

    void DisConnect();

    ///发送Json字符串给服务器
    void Send(int varMsgID,const QString& varPlayerIDStr,const QString& varJsonStr);

    void SaveJson();

    void CreateJson();
private slots:

};
#endif // MAINWINDOW_H
