#include "ElaLogPrivate.h"

#include <QDateTime>
#include <QFile>
#include <QMutex>

#ifndef QT_NO_DEBUG
#include <iostream>
#endif
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QTextStream>
#endif
#include "ElaLog.h"
Q_GLOBAL_STATIC(QMutex, messageLogMutex)
Q_GLOBAL_STATIC(QString, logFileNameTime)
ElaLogPrivate::ElaLogPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaLogPrivate::~ElaLogPrivate()
{
}

void ElaLogPrivate::_messageLogHander(QtMsgType type, const QMessageLogContext& ctx, const QString& msg)
{
    if (type > QtCriticalMsg)
    {
        return;
    }
    QString logInfo;
    QString logTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    switch (type)
    {
    case QtDebugMsg:
    {
        logInfo = QString("[信息-%1](函数: %2 , 行数: %3) -> %4").arg(logTime, ctx.function, QString::number(ctx.line), msg);
        break;
    }
    case QtWarningMsg:
    {
        logInfo = QString("[警告-%1](函数: %2 , 行数: %3) -> %4").arg(logTime, ctx.function, QString::number(ctx.line), msg);
        break;
    }
    case QtCriticalMsg:
    {
        logInfo = QString("[错误-%1](函数: %2 , 行数: %3) -> %4").arg(logTime, ctx.function, QString::number(ctx.line), msg);
        break;
    }
    default:
    {
        qCritical("发生致命错误！");
        break;
    }
    }
#ifndef QT_NO_DEBUG
    std::cout << logInfo.toLocal8Bit().constData() << "\n";
#endif
    messageLogMutex->lock();
    QFile logfile;
    ElaLog* log = ElaLog::getInstance();
    if (log->getIsLogFileNameWithTime())
    {
        logfile.setFileName(log->getLogSavePath() + "\\" + log->getLogFileName() + *logFileNameTime + ".txt");
    }
    else
    {
        logfile.setFileName(log->getLogSavePath() + "\\" + log->getLogFileName() + ".txt");
    }
    if (logfile.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream logFileStream(&logfile);
        logFileStream << logInfo << Qt::endl;
        logfile.close();
    }
    messageLogMutex->unlock();
}

void ElaLogPrivate::_clearLogFile()
{
    if (_pIsLogFileNameWithTime)
    {
        QString logTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss");
        logTime.prepend("_");
        logTime.replace(" ", "_");
        logFileNameTime->clear();
        logFileNameTime->append(logTime);
    }
    else
    {
        QFile file(_pLogSavePath + "\\" + _pLogFileName + ".txt");
        if (file.exists())
        {
            if (file.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate))
            {
                file.close();
            }
        }
    }
}
