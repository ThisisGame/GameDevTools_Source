#include "datareceive.h"
#include <QDebug>

DataReceive::DataReceive(QObject *parent):QObject(parent)
{
    dataRecvWS = Q_NULLPTR;
    connectStatus = false;
    m_timer = new QTimer();
}

DataReceive::~DataReceive(){

}

/**
 * @breaf 创建WebSocket连接
 */
void DataReceive::createDataRecvWS(){
    if(dataRecvWS == Q_NULLPTR){
        dataRecvWS = new QWebSocket();
        qDebug()<<"create websocket!";
        connect(dataRecvWS,&QWebSocket::disconnected,this,DataReceive::onDisConnected,Qt::AutoConnection);
        connect(dataRecvWS,&QWebSocket::textMessageReceived,this,DataReceive::onTextReceived,Qt::AutoConnection);
        connect(dataRecvWS,&QWebSocket::connected,this,DataReceive::onConnected,Qt::AutoConnection);
        connect(m_timer,m_timer->timeout,this,DataReceive::reconnect,Qt::AutoConnection);
        dataRecvWS->open(QUrl("ws://192.168.0.17:60000/data"));
    }
}

/**
 * @breaf 判断连接状态
 * @note  当连接成功后，触发该函数
 */
void DataReceive::onConnected(){
    qDebug()<<"DataReveive websocket is already connect!";
    connectStatus = true;
    m_timer->stop();
    qDebug()<<"Address："<<dataRecvWS->peerAddress();


}

/**
 * @breaf 数据处理函数
 * @param msg，数据信息
 * @note  当收到服务端发送的数据时，触发该函数
 */
void DataReceive::onTextReceived(QString msg){
    qDebug()<<"----------------data-----------------";
    qDebug()<<msg;
}

/**
 * @breaf 连接断开
 * @note  当连接断开时，触发该函数
 */
void DataReceive::onDisConnected(){
    qDebug()<<"DataReceive websocket is disconnected!!!";
    connectStatus = false;
    m_timer->start(3000);/*-<当连接失败时，触发重连计时器，设置计数周期为3秒 */
}

/**
 * @breaf 周期重连函数
 * @note  连接建立成功时，调用该函数
 */
void DataReceive::reconnect(){
    qDebug()<<"try to reconnect!";
    dataRecvWS->abort();
    dataRecvWS->open(QUrl("ws://192.168.0.17:60000/data"));
}
