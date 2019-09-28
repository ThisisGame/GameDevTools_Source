#include <QtWidgets>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QWebSocket>

#include "maindialog.h"


MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("服务器测试工具"));


    //判断第一次打开，创建json
    CreateJson();

    this->LoadGameMessageJson();

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



    QVBoxLayout* leftVBoxLayout=new QVBoxLayout();
    leftVBoxLayout->setContentsMargins(10,10,10,10);
    leftVBoxLayout->setSizeConstraint(QLayout::SetFixedSize);

    //服务器地址编辑框
    QHBoxLayout* connectHBoxLayout=new QHBoxLayout();
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

    leftVBoxLayout->addLayout(connectHBoxLayout);

    //PlayerID 编辑框
    QHBoxLayout* loginHBoxLayout=new QHBoxLayout();

    playeridLineEdit=new QLineEdit();
    playeridLineEdit->setPlaceholderText(tr("玩家ID"));
    playeridLineEdit->setText(rootObj["playerid"].toString());
    connect(playeridLineEdit,&QLineEdit::textChanged,[this]
    {
        this->SaveJson();
    });

    loginButton=new QPushButton(tr("登录"));
    connect(loginButton,&QPushButton::clicked,[this]
    {
        QString jsonStr;
        jsonStr=QString("{\"m_AccountID\":0,\"m_RoleID\":\"%1\",\"m_randstr\":\"\"}").arg(playeridLineEdit->text());
        this->Send("GM_ROLE_LOGIN",playeridLineEdit->text(),jsonStr);
    });
    loginButton->setEnabled(false);

    loginHBoxLayout->addWidget(playeridLineEdit);
    loginHBoxLayout->addWidget(loginButton);

    leftVBoxLayout->addLayout(loginHBoxLayout);

    msgidLineEdit=new QLineEdit();
    msgidLineEdit->setPlaceholderText(tr("消息ID，填数字"));
    msgidLineEdit->setText(rootObj["msgid"].toString());
    connect(msgidLineEdit,&QLineEdit::textChanged,[this]
    {
        this->SaveJson();
    });

    //发送的Json数据 编辑框
    sendTextEdit=new QTextEdit();
    sendTextEdit->setPlaceholderText(tr("发给服务器的Json"));
    sendTextEdit->setText(rootObj["json"].toString());
    connect(sendTextEdit,&QTextEdit::textChanged,[this]
    {
        this->SaveJson();
    });

    sendButton=new QPushButton(tr("发送"));
    connect(sendButton,&QPushButton::clicked,[this]
    {
        this->Send(msgidLineEdit->text(),playeridLineEdit->text(),sendTextEdit->toPlainText());
    });
    sendButton->setEnabled(false);

    //屏蔽返回消息 编辑框
    QLabel* tipsLabel=new QLabel(tr("下面填屏蔽的消息，一行一个"));
    QTextEdit* ignoreTextEdit=new QTextEdit();
    ignoreTextEdit->setPlaceholderText(tr("屏蔽返回的消息，一行一个"));
    ignoreTextEdit->setText("GM_NOTIFY_CHANGE_xstring\nGM_MULTICOPY_PLAYER_HP_DROP_BROADCAST");
    connect(ignoreTextEdit,&QTextEdit::textChanged,[this,ignoreTextEdit]
    {
        ignoreMsgids->clear();
        QStringList tmpLines=ignoreTextEdit->toPlainText().split("\n");
        ignoreMsgids->append(tmpLines);
    });
    ignoreMsgids=new QStringList();
    ignoreMsgids->append("GM_NOTIFY_CHANGE_xstring");
    ignoreMsgids->append("GM_MULTICOPY_PLAYER_HP_DROP_BROADCAST");


    leftVBoxLayout->addWidget(playeridLineEdit);
    leftVBoxLayout->addWidget(msgidLineEdit);
    leftVBoxLayout->addWidget(sendTextEdit);
    leftVBoxLayout->addWidget(sendButton);
    leftVBoxLayout->addWidget(tipsLabel);
    leftVBoxLayout->addWidget(ignoreTextEdit);

    //中间的历史记录
    QVBoxLayout* histroyVBoxLayout=new QVBoxLayout();
    histroyVBoxLayout->setContentsMargins(0,10,0,10);
    histroyVBoxLayout->setSizeConstraint(QLayout::SetFixedSize);

    QLabel* historyLabel=new QLabel(tr("历史记录"));

    filenameModel= new QStandardItemModel(this);

    filenameListView=new QListView();

    filenameListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    filenameListView->setModel(filenameModel);
    filenameListView->setObjectName(QString::fromUtf8("filenameListView"));

    histroyVBoxLayout->addWidget(historyLabel);
    histroyVBoxLayout->addWidget(filenameListView);

    //右边收到的消息框
    QVBoxLayout* rightVBoxLayout=new QVBoxLayout();
    rightVBoxLayout->setContentsMargins(10,10,10,10);
    rightVBoxLayout->setSizeConstraint(QLayout::SetFixedSize);

    //收到的数据 编辑框
    receivedTextEdit=new QTextEdit();
    receivedTextEdit->setPlaceholderText(tr("服务器回的Json"));

    QPushButton* clearButton=new QPushButton(tr("清空"));
    connect(clearButton,&QPushButton::clicked,[this]
    {
        receivedTextEdit->clear();
    });


    rightVBoxLayout->addWidget(receivedTextEdit);
    rightVBoxLayout->addWidget(clearButton);

    QHBoxLayout* mainHBoxLayout=new QHBoxLayout();
    mainHBoxLayout->setContentsMargins(0,0,0,0);
//    mainHBoxLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainHBoxLayout->addLayout(leftVBoxLayout);
    mainHBoxLayout->addStretch();
    mainHBoxLayout->addLayout(histroyVBoxLayout);
    mainHBoxLayout->addLayout(rightVBoxLayout);
    mainHBoxLayout->setStretchFactor(leftVBoxLayout,2);
    mainHBoxLayout->setStretchFactor(histroyVBoxLayout,2);
    mainHBoxLayout->setStretchFactor(rightVBoxLayout,8);

    setLayout(mainHBoxLayout);
    this->resize(1700,700);

    //加载所有历史文件
    this->RefreshHistoryFilelist();

    QMetaObject::connectSlotsByName(this);
}


void MainDialog::LoadGameMessageJson()
{
    //读取保存的数据
    QFile loadFile("./GameMessage.json");
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
    QJsonObject::const_iterator it = rootObj.constBegin();
    QJsonObject::const_iterator end = rootObj.constEnd();

    msgidNumberStrMap=new QMap<QString, QString>();
    //
    while (end != it)
    {
       // it.key();
       // it.value()
        msgidNumberStrMap->insert(it.key(),it.value().toString());
       it++;
    }
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
        loginButton->setEnabled(true);
    });
    connect(webSocket,&QWebSocket::disconnected,[=]
    {
        qDebug()<<"disconnected";
        connectButton->setEnabled(true);
        disconnectButton->setEnabled(false);
        addressLineEdit->setEnabled(true);
        loginButton->setEnabled(false);
        sendButton->setEnabled(false);
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
        QString msgidstr=QString::number(msgid);
        if(msgidNumberStrMap->contains(msgidstr))
        {
            msgidstr=msgidNumberStrMap->value(msgidstr);
        }

        QByteArray jsonByteArray;
        for(int index=44;index<varBinaryMessage.size();index++)
        {
            jsonByteArray.push_back(varBinaryMessage[index]);
        }
        QString jsonStr=jsonByteArray;

        qDebug()<<"msgid:"<<msgidstr;
        qDebug()<<"json:"<<jsonStr;

        if(ignoreMsgids->contains(msgidstr)==false)
        {
            receivedTextEdit->append("MsgID:"+msgidstr);
            receivedTextEdit->append("JSON:"+jsonStr);
            receivedTextEdit->append("---------------------\n");
        }



        if(msgid==412)
        {
            loginButton->setEnabled(false);
            sendButton->setEnabled(true);
        }
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

void MainDialog::Send(const QString& varMsgIDStr,const QString& varPlayerIDStr,const QString& varJsonStr)
{
    if(varJsonStr.size()==0)
    {
        qDebug()<<"empty msg";
        return;
    }

    int msgid=0;

    QMapIterator<QString, QString> it(*msgidNumberStrMap);
    while (it.hasNext()) {
        it.next();
        qDebug() << "QMapIterator " << it.key() << ":" << it.value(); // java风格迭代器
        if(it.value()==varMsgIDStr)
        {
            msgid=it.key().toInt();
            break;
        }
    }
    if(msgid==0)
    {
        msgidLineEdit->setText(tr("找不到这个消息ID"));
        return;
    }

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

        this->SaveMsg(varMsgIDStr,varJsonStr);
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
        }
        else
        {
            if(!loadFile.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                qDebug() << "could't open json for write";
                return;
            }
            loadFile.write(jsonDocument.toJson());
            qDebug()<< "finish!!!!";
            loadFile.close();
        }

    }

    {
        QJsonObject jsonObject;
        jsonObject["10002"]="GM_DIGMINE_UPGRADE_RETURN";

        QJsonDocument jsonDocument(jsonObject);
        QFile loadFile("./GameMessage.json");
        if(loadFile.open(QIODevice::ReadOnly))
        {
            loadFile.close();
            qDebug() << "file exist ,return";
        }
        else
        {
            if(!loadFile.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                qDebug() << "could't open json for write";
                return;
            }
            loadFile.write(jsonDocument.toJson());
            qDebug()<< "finish!!!!";
            loadFile.close();
        }
    }
}

void MainDialog::SaveMsg(const QString& varMsgIDStr,const QString& varJsonStr)
{
    QString messageidStr=varMsgIDStr;
    QString sendStr=varJsonStr;

    QJsonObject jsonObject;
    jsonObject["msgid"]=messageidStr;
    jsonObject["json"]=sendStr;




    QJsonDocument jsonDocument(jsonObject);
    QFile loadFile("./history/"+messageidStr+".json");
    if(!loadFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "could't open json for write,filename:"<<messageidStr;
        return;
    }
    loadFile.write(jsonDocument.toJson());
    qDebug()<< "finish!!!!";
    loadFile.close();

    this->RefreshHistoryFilelist();
}

void MainDialog::RefreshHistoryFilelist()
{
    {
        QDir dir;
        dir.cd("./");
        if(!dir.exists("history")){
            bool ismkdir = QDir().mkdir("history");
            if(!ismkdir)
                qDebug() << "Create path fail" << endl;
            else
                qDebug() << "Create fullpath success" << endl;
        }
        else{
            qDebug() << "fullpath exist" << endl;
        }
    }


    filenameModel->clear();
    QDir dir;
    dir.cd(("./history"));
    dir.setFilter(QDir::Files);

    QFileInfoList fileInfoList= dir.entryInfoList();
    for(int index=0;index<fileInfoList.length();index++)
    {
        QString fileName=fileInfoList.at(index).fileName();
        qDebug()<<fileName;

        fileName=fileName.split(".")[0];
        QStandardItem* standardItem=new QStandardItem(fileName);
        filenameModel->appendRow(standardItem);
    }
}

void MainDialog::on_filenameListView_clicked(QModelIndex varModelIndex)
{
    qDebug()<<"on_filenameListView_clicked"<<varModelIndex.data();

    this->LoadOneHistory(varModelIndex.data().toString());
}

void MainDialog::LoadOneHistory(const QString& varFileName)
{
    //读取保存的数据
    QFile loadFile("./history/"+varFileName+".json");
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
    //设置编辑框
    msgidLineEdit->setText(rootObj["msgid"].toString());
    sendTextEdit->setText(rootObj["json"].toString());
}

MainDialog::~MainDialog()
{
//    if(webSocket!=nullptr)
//    {
//        webSocket->close();
//        webSocket=nullptr;
//    }
}

