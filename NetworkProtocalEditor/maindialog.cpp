#include <QtWidgets>
#include "maindialog.h"

MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(QMargins());
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    QScrollArea* scrollArea=new QScrollArea();
    scrollArea->setMinimumHeight(900);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

    QWidget *protocalWidget =this->CreateProtocal();

    scrollArea->setWidget(protocalWidget);

    mainLayout->addWidget(scrollArea);

    setLayout(mainLayout);

    setWindowTitle(tr("网络协议记录"));
}

QWidget* MainDialog::CreateProtocalPart(const QString& varTypeStr)
{
    QLabel *typeLabel=new QLabel(varTypeStr);

    QLabel *messageidTipsLabel=new QLabel(tr("消息ID:"));
    QLineEdit *messageidLineEdit=new QLineEdit();
    messageidLineEdit->setPlaceholderText(tr("GM_WORLDBOSS_EXIT_RETURN"));

    QLabel  *protocalTipsLabel=new QLabel(tr("消息结构:"));
    QTextEdit *protocalTextEdit=new QTextEdit();
    protocalTextEdit->setPlaceholderText(tr("message GM_WorldBoss_Exit_Return"));
    protocalTextEdit->setContentsMargins(QMargins());

    QVBoxLayout *protocalVBoxLayout = new QVBoxLayout;
    protocalVBoxLayout->setContentsMargins(QMargins());
    protocalVBoxLayout->setSizeConstraint(QLayout::SetFixedSize);


    protocalVBoxLayout->addWidget(typeLabel);
    protocalVBoxLayout->addWidget(messageidTipsLabel);
    protocalVBoxLayout->addWidget(messageidLineEdit);
    protocalVBoxLayout->addWidget(protocalTipsLabel);
    protocalVBoxLayout->addWidget(protocalTextEdit);

    QFrame* containerWidget=new QFrame();
    containerWidget->setFrameStyle(QFrame::Box);
    containerWidget->setLayout(protocalVBoxLayout);

    return containerWidget;
}

QWidget* MainDialog::CreateProtocal()
{
    QGridLayout *protocalGridLayout = new QGridLayout;
    protocalGridLayout->setContentsMargins(QMargins());
    protocalGridLayout->setSizeConstraint(QLayout::SetFixedSize);
    protocalGridLayout->setContentsMargins(10,10,10,10);

    QWidget *clientProtocalWidget=this->CreateProtocalPart(tr("客户端"));
    QWidget *serverProtocalWidget=this->CreateProtocalPart(tr("服务端"));
    QPushButton* addServerPartPushButton=new QPushButton(tr("添加服务器返回消息"));

    connect(addServerPartPushButton,&QPushButton::clicked,[=]()
    {
        qDebug()<<"click add";
        QWidget *serverProtocalWidget=this->CreateProtocalPart(tr("服务端"));

        protocalGridLayout->removeWidget(addServerPartPushButton);

        protocalGridLayout->addWidget(serverProtocalWidget,protocalGridLayout->rowCount(),2);
        protocalGridLayout->addWidget(addServerPartPushButton,protocalGridLayout->rowCount(),2);
    });

    protocalGridLayout->addWidget(clientProtocalWidget,0,0);
    protocalGridLayout->addWidget(serverProtocalWidget,0,2);
    protocalGridLayout->addWidget(addServerPartPushButton,1,2);

    QFrame* containerWidget=new QFrame();
    containerWidget->setFrameStyle(QFrame::Box);
    containerWidget->setLayout(protocalGridLayout);
    return containerWidget;
}

MainDialog::~MainDialog()
{
}

