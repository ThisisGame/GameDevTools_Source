#include <QtWidgets>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QWebSocket>

#include "maindialog.h"


MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
{
    //判断第一次打开，创建json
    CreateJson();

    //读取保存的数据
    QFile loadFile("./savedata.json");
    if(!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "could't open projects json";
        return;
    }

    QByteArray allData = loadFile.readAll();
    loadFile.close();

    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!";
        return;
    }

    QJsonObject rootObj = jsonDoc.object();



    QVBoxLayout* mainVBoxLayout=new QVBoxLayout();
    mainVBoxLayout->setContentsMargins(10,10,10,10);
    mainVBoxLayout->setSizeConstraint(QLayout::SetFixedSize);

    //服务器地址编辑框
    QHBoxLayout* connectHBoxLayout=new QHBoxLayout();
    mainVBoxLayout->setContentsMargins(10,10,10,10);
    mainVBoxLayout->setSizeConstraint(QLayout::SetFixedSize);

    addressLineEdit=new QLineEdit();

    addressLineEdit->setPlaceholderText(tr("192.168.2.130:50001"));
    addressLineEdit->setText(rootObj["address"].toString());
    connect(addressLineEdit,&QLineEdit::textChanged,[this]
    {
        this->SaveJson();
    });

    connectButton=new QPushButton(tr("连接"));
    connect(connectButton,&QPushButton::clicked,[this]
    {
        this->Connect(addressLineEdit->text());
    });
    disconnectButton=new QPushButton(tr("断开连接"));
    connect(disconnectButton,&QPushButton::clicked,[this]
    {
        this->DisConnect();
    });
    disconnectButton->setEnabled(false);

    connectHBoxLayout->addWidget(addressLineEdit);
    connectHBoxLayout->addWidget(connectButton);
    connectHBoxLayout->addWidget(disconnectButton);

    mainVBoxLayout->addLayout(connectHBoxLayout);

    //发送的Json数据 编辑框
    playeridLineEdit=new QLineEdit();
    playeridLineEdit->setPlaceholderText(tr("玩家ID"));
    playeridLineEdit->setText(rootObj["playerid"].toString());
    connect(playeridLineEdit,&QLineEdit::textChanged,[this]
    {
        this->SaveJson();
    });

    msgidLineEdit=new QLineEdit();
    msgidLineEdit->setPlaceholderText(tr("消息ID，填数字"));
    msgidLineEdit->setText(rootObj["msgid"].toString());
    connect(msgidLineEdit,&QLineEdit::textChanged,[this]
    {
        this->SaveJson();
    });

    sendTextEdit=new QTextEdit();
    sendTextEdit->setPlaceholderText(tr("发给服务器的Json"));
    sendTextEdit->setText(rootObj["json"].toString());
    connect(sendTextEdit,&QTextEdit::textChanged,[this]
    {
        this->SaveJson();
    });

    QPushButton* sendButton=new QPushButton(tr("发送"));
    connect(sendButton,&QPushButton::clicked,[this]
    {
        int msgid=msgidLineEdit->text().toInt();

        this->Send(msgid,playeridLineEdit->text(),sendTextEdit->toPlainText());
    });

    receivedMsgidEdit=new QLineEdit();
    receivedMsgidEdit->setPlaceholderText(tr("服务器回的消息ID"));

    receivedTextEdit=new QTextEdit();
    receivedTextEdit->setPlaceholderText(tr("服务器回的Json"));

    mainVBoxLayout->addWidget(playeridLineEdit);
    mainVBoxLayout->addWidget(msgidLineEdit);
    mainVBoxLayout->addWidget(sendTextEdit);
    mainVBoxLayout->addWidget(sendButton);

    mainVBoxLayout->addWidget(receivedMsgidEdit);
    mainVBoxLayout->addWidget(receivedTextEdit);

    setLayout(mainVBoxLayout);
    setWindowTitle(tr("服务器测试工具"));
}

int bytesToInt(QByteArray bytes) {
    int addr = bytes[0] & 0x000000FF;
    addr |= ((bytes[1] << 8) & 0x0000FF00);
    addr |= ((bytes[2] << 16) & 0x00FF0000);
    addr |= ((bytes[3] << 24) & 0xFF000000);
    return addr;
}

void MainDialog::Connect(const QString &varServerAddr)
{
    if(varServerAddr.size()==0)
    {
        qDebug()<<"address error";
        return;
    }
    qDebug()<<"connect:"<<varServerAddr;
    webSocket=new QWebSocket();
    connect(webSocket,&QWebSocket::connected,[=]
    {
        qDebug()<<"connect success";
        connectButton->setEnabled(false);
        disconnectButton->setEnabled(true);
        addressLineEdit->setEnabled(false);
    });
    connect(webSocket,&QWebSocket::disconnected,[=]
    {
        qDebug()<<"disconnected";
        connectButton->setEnabled(true);
        disconnectButton->setEnabled(false);
        addressLineEdit->setEnabled(true);
    });
    connect(webSocket,&QWebSocket::binaryMessageReceived,[=](const QByteArray& varBinaryMessage)
    {
        qDebug()<<"binaryMessageReceived:"<<varBinaryMessage;
        QByteArray msgidByteArray;
        for(int index=0;index<4;index++)
        {
            msgidByteArray.push_back(varBinaryMessage[index]);
        }
        int msgid=bytesToInt(msgidByteArray);

        QByteArray jsonByteArray;
        for(int index=44;index<varBinaryMessage.size();index++)
        {
            jsonByteArray.push_back(varBinaryMessage[index]);
        }
        QString jsonStr=jsonByteArray;

        qDebug()<<"msgid:"<<msgid;
        qDebug()<<"json:"<<jsonStr;
        receivedMsgidEdit->setText(QString::number(msgid));
        receivedTextEdit->setPlainText(jsonStr);
    });


    webSocket->open((QUrl("ws://"+varServerAddr)));
}

QByteArray intToByte(int i)
{
    QByteArray abyte0;
    abyte0.resize(4);
    abyte0[0] = (uchar)  (0x000000ff & i);
    abyte0[1] = (uchar) ((0x0000ff00 & i) >> 8);
    abyte0[2] = (uchar) ((0x00ff0000 & i) >> 16);
    abyte0[3] = (uchar) ((0xff000000 & i) >> 24);
    return abyte0;
}

void MainDialog::Send(int varMsgID,const QString& varPlayerIDStr,const QString& varJsonStr)
{
    if(varJsonStr.size()==0)
    {
        qDebug()<<"empty msg";
        return;
    }

    int msgid=varMsgID;

    QString playerid=varPlayerIDStr;
    QString jsonStr=varJsonStr;
    QByteArray jsonByteArray(jsonStr.toUtf8());

    int msgsize=4+4+36+jsonByteArray.size();

    QByteArray sendByteArray;
    sendByteArray.push_back(intToByte(msgid));
    sendByteArray.push_back(intToByte(msgsize));
    sendByteArray.push_back(playerid.toUtf8());
    sendByteArray.push_back(jsonByteArray);

    if(webSocket!=nullptr)
    {
        qDebug()<<"send:"<<sendByteArray;
        webSocket->sendBinaryMessage(sendByteArray);
        qDebug()<<"send success";
    }
}

void MainDialog::DisConnect()
{
    if(webSocket!=nullptr)
    {
        webSocket->close();
    }
    qDebug()<<"disconnect";
}

void MainDialog::SaveJson()
{
    QJsonObject jsonObject;
    jsonObject["address"]=addressLineEdit->text();
    jsonObject["msgid"]=msgidLineEdit->text();
    jsonObject["playerid"]=playeridLineEdit->text();
    jsonObject["json"]=sendTextEdit->toPlainText();

    QJsonDocument jsonDocument(jsonObject);
    QFile loadFile("./savedata.json");
    if(!loadFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "could't open projects json";
        return;
    }
    loadFile.write(jsonDocument.toJson());
    qDebug()<< "finish!!!!";
    loadFile.close();
}

void MainDialog::CreateJson()
{
    QJsonObject jsonObject;
    jsonObject["address"]="192.168.2.130:50002";
    jsonObject["msgid"]="22700";
    jsonObject["playerid"]="5445d18e-8790-11e9-8503-d017c2a84e2c";
    jsonObject["json"]="{}";

    QJsonDocument jsonDocument(jsonObject);
    QFile loadFile("./savedata.json");
    if(loadFile.open(QIODevice::ReadOnly))
    {
        loadFile.close();
        qDebug() << "file exist ,return";
        return;
    }
    if(!loadFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "could't open json for write";
        return;
    }
    loadFile.write(jsonDocument.toJson());
    qDebug()<< "finish!!!!";
    loadFile.close();
}


MainDialog::~MainDialog()
{
//    if(webSocket!=nullptr)
//    {
//        webSocket->close();
//        webSocket=nullptr;
//    }
}

