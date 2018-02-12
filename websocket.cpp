#include "websocket.h"


websocket::websocket(QObject *parent) : QObject(parent)
{
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("WebSocket Server"), QWebSocketServer::NonSecureMode, this);
    if (m_pWebSocketServer->listen(QHostAddress::Any, 2345)) {
        connect(m_pWebSocketServer, SIGNAL(newConnection()),this, SLOT(onNewConnection()));
//        connect(m_pWebSocketServer, SIGNAL(closed()), this, SLOT(closed()));
    }

    QTimer *t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(timer()));
    t->start(1000);
}
websocket::~websocket()
{
    if (m_pWebSocketServer->isListening()) {
        m_pWebSocketServer->close();
        qDeleteAll(m_clients.begin(), m_clients.end());
    }
}
void websocket::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &websocket::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &websocket::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &websocket::socketDisconnected);

    pSocket->ignoreSslErrors();

    m_clients << pSocket;
}
void websocket::processTextMessage(QString message){

    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {pClient->sendTextMessage(message);}
    qDebug() <<message;
    for (int i = 0; i < m_clients.length(); i++) {
        m_clients.at(i)->sendTextMessage(message);
    }
}
void websocket::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {pClient->sendBinaryMessage(message);}
}
void websocket::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

void websocket::timer()
{
    QProcess *process = new QProcess();
    cekStatus  = PATH_STATUS;
    process ->start(cekStatus);
    process ->waitForFinished();
    result = process ->readAllStandardOutput();
    list_result = result.split("\n");
    QJsonObject service;
        service["Name"]   = "Monita_Service";
        service["Status"] =  list_result.at(0);
    QJsonArray ls;
        ls.append(service);
        service["Name"]   = "Monita_Service2";
        service["Status"] =  list_result.at(0);
        ls.append(service);
    QJsonObject conf;
         conf["Monita"] = ls;
         conf["Last_Update"]= QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QJsonDocument jsonDoc(conf);
    for(int i=0; i< m_clients.length();i++){
       m_clients.at(i)->sendTextMessage(jsonDoc.toJson());
    }

}
