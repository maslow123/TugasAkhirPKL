#include "websocket.h"

service::service()
{
    cekStatus  = PATH_STATUS;
    process ->start(cekStatus);
    process ->waitForFinished();
    result = process ->readAllStandardOutput();
    list_result = result.split("\n");

    QJsonObject conf;
    conf["periode"] = 5000;
    conf["port"] = 2345;
    QJsonObject service;
        service["status"] = list_result.at(0);
        service["restart"] = "restart.sh";
        service["nama"] = "Monita_Service";
    QJsonArray listService;
        listService.append(service);
        service["status"] = list_result.at(0);
        service["restart"] = "restart2.sh";
        service["nama"] = "Monita_Service2";
        listService.append(service);
        conf["Service"] = listService;
    QJsonObject json;
        json["Config"] = conf;
    QJsonDocument jsonDoc(json);

    QFile loadFile(PATH_MONITA);
    QFile fileJson(PATH_JSON);
    QFile fileBin (PATH_BIN);
    if(!loadFile.exists()){
        QDir dir;
        dir.mkpath(PATH_MONITA);
//         ready to logic !!
        if(!fileJson.exists() && !fileBin.exists()){
             qDebug()<< "Data is successfull to create.."<<endl;
             Q_ASSERT(fileJson.open(QFile::WriteOnly));
             Q_ASSERT(fileBin. open(QFile::WriteOnly));
             fileJson.write(jsonDoc.toJson());
             fileBin.write (jsonDoc.toBinaryData());
             fileJson.close();
             fileBin.close();
             if(fileJson.exists() && fileBin.exists()){
                  qDebug()<<"The result of data is..."<<endl;
                  Q_ASSERT(fileJson.open(QFile::ReadOnly));
                  Q_ASSERT(fileBin. open(QFile::ReadOnly));

                  QJsonDocument docJson =  QJsonDocument::fromJson(fileJson.readAll());
                  QJsonDocument docBin  =  QJsonDocument::fromBinaryData(fileBin.readAll());
//        reading the data that was made
                  qDebug()<<"Result 1 : "<<docJson<<endl;
                  qDebug()<<"Result 2 : "<<jsonDoc<<endl;
            }else{qDebug()<<"Something wrong in project ..";}
        }else{
             qDebug()<<"Reading the data ...  "<<endl;
             Q_ASSERT(fileJson.open(QFile::ReadOnly));
             Q_ASSERT(fileBin. open(QFile::ReadOnly));
             QJsonDocument docJson = QJsonDocument::fromJson(fileJson.readAll());
             QJsonDocument docBin  = QJsonDocument::fromBinaryData(fileBin.readAll());
             qDebug()<<"Result 1 : "<<docJson<<endl;
             qDebug()<<"Result 2 : "<<docBin<<endl;
        }
    }else{
        if(fileJson.exists() && fileBin.exists()){
             qDebug()<< "Data is success to change..."<<endl;
             Q_ASSERT(fileJson.open(QFile::WriteOnly));
             Q_ASSERT(fileBin. open(QFile::WriteOnly));
             fileJson.write(jsonDoc.toJson());
             fileBin.write (jsonDoc.toBinaryData());
             fileJson.close();
             fileBin.close();
             Q_ASSERT(fileJson.open(QFile::ReadOnly));
             Q_ASSERT(fileBin. open(QFile::ReadOnly));
             QJsonDocument docJson = QJsonDocument::fromJson(fileJson.readAll());
             QJsonDocument docBin  = QJsonDocument::fromBinaryData(fileBin.readAll());
             qDebug()<<"Result 1 : "<<docJson<<endl;
             qDebug()<<"Result 2 : "<<docBin<<endl;
       }else{
             qDebug()<< "Data is successfull to create.."<<endl;
             Q_ASSERT(fileJson.open(QFile::WriteOnly));
             Q_ASSERT(fileBin. open(QFile::WriteOnly));
             fileJson.write(jsonDoc.toJson());
             fileBin.write (jsonDoc.toBinaryData());
             fileJson.close();
             fileBin.close();

             Q_ASSERT(fileJson.open(QFile::ReadOnly));
             Q_ASSERT(fileBin. open(QFile::ReadOnly));
             QJsonDocument docJson = QJsonDocument::fromJson(fileJson.readAll());
             QJsonDocument docBin  = QJsonDocument::fromBinaryData(fileBin.readAll());
             qDebug()<<"Result 1 : "<<docJson<<endl;
             qDebug()<<"Result 2 : "<<docBin<<endl;
          }
      }
 }
timer::timer()
{
    QTimer *t = new QTimer(this);
    connect(t, SIGNAL(timeout()),this,SLOT(time()));
    t->start(5000); // 5 second
}
void timer::time()
{
    QProcess *proses = new QProcess();
    QString cekStatus = PATH_CEK_STATUS;
    proses ->start(cekStatus);
    proses ->waitForFinished();
    QString result = proses ->readAllStandardOutput();
    QStringList list_result = result.split("\n");
    qDebug()<<list_result.at(0);
}

