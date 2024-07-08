#ifndef SIGNHELPER_H
#define SIGNHELPER_H

#include <QString>
#include <QNetworkRequest>
#include <QCryptographicHash>
#include <QDateTime>
#include <QUrl>
#include <QMap>
#include <QList>
#include <QByteArray>
#include <QUrlQuery>

class SignHelper {
public:
    SignHelper(const QString &appId, const QString &secretId, const QString &secretKey)
        : appId(appId), secretId(secretId), secretKey(secretKey) {}

    QString generateSignature(QNetworkRequest &request, int signDurationSeconds);

private:
    QString appId;
    QString secretId;
    QString secretKey;

    QString hmacSha1(const QString &key, const QString &message);
    QString sha1(const QString &message);
    QString urlEncode(const QString &value);
    QString getKeyTime(int signDurationSeconds);
};

QString SignHelper::generateSignature(QNetworkRequest &request, int signDurationSeconds) {
    // 获取当前 Unix 时间戳
    qint64 currentTime = QDateTime::currentSecsSinceEpoch();
    qint64 endTime = currentTime + signDurationSeconds;
    QString keyTime = QString("%1;%2").arg(currentTime).arg(endTime);

    // 获取请求方法和路径
    QString method = request.attribute(QNetworkRequest::CustomVerbAttribute).toString().toLower();
    QString uriPath = request.url().path();

    // 生成 HttpParameters 和 UrlParamList
    QUrlQuery query(request.url());
    QMap<QString, QString> queryParams;
    for (const auto &item : query.queryItems()) {
        queryParams.insert(urlEncode(item.first.toLower()), urlEncode(item.second));
    }

    QStringList keyList = queryParams.keys();
    std::sort(keyList.begin(), keyList.end());

    QString httpParameters;
    QString urlParamList;
    for (const auto &key : keyList) {
        httpParameters += key + "=" + queryParams.value(key) + "&";
        urlParamList += key + ";";
    }
    httpParameters.chop(1); // Remove the last '&'
    urlParamList.chop(1);   // Remove the last ';'

    // 生成 HttpHeaders 和 HeaderList
    QList<QByteArray> headerList = request.rawHeaderList();
    QMap<QString, QString> headerParams;
    for (const auto &header : headerList) {
        headerParams.insert(urlEncode(header.toLower()), urlEncode(request.rawHeader(header)));
    }

    keyList = headerParams.keys();
    std::sort(keyList.begin(), keyList.end());

    QString httpHeaders;
    QString headerListStr;
    for (const auto &key : keyList) {
        httpHeaders += key + "=" + headerParams.value(key) + "&";
        headerListStr += key + ";";
    }
    httpHeaders.chop(1);  // Remove the last '&'
    headerListStr.chop(1); // Remove the last ';'

    // 生成 HttpString
    QString httpString = QString("%1\n%2\n%3\n%4\n").arg(method, uriPath, httpParameters, httpHeaders);

    // 生成 StringToSign
    QString httpStringSha1 = sha1(httpString);
    QString stringToSign = QString("sha1\n%1\n%2\n").arg(keyTime, httpStringSha1);

    // 生成 SignKey
    QString signKey = hmacSha1(secretKey, keyTime);

    // 生成 Signature
    QString signature = hmacSha1(signKey, stringToSign);

    // 构造最终签名字符串
    QString signatureString = QString("q-sign-algorithm=sha1&q-ak=%1&q-sign-time=%2&q-key-time=%3&q-header-list=%4&q-url-param-list=%5&q-signature=%6")
                                  .arg(secretId, keyTime, keyTime, headerListStr, urlParamList, signature);

    return signatureString;
}

QString SignHelper::hmacSha1(const QString &key, const QString &message) {
    QByteArray keyBytes = key.toUtf8();
    QByteArray messageBytes = message.toUtf8();

    int blockSize = 64; // HMAC-SHA1 block size is 64 bytes

    if (keyBytes.size() > blockSize) {
        keyBytes = QCryptographicHash::hash(keyBytes, QCryptographicHash::Sha1);
    }

    if (keyBytes.size() < blockSize) {
        keyBytes = keyBytes.leftJustified(blockSize, 0x00);
    }

    QByteArray oKeyPad(blockSize, 0x5c);
    QByteArray iKeyPad(blockSize, 0x36);

    for (int i = 0; i < blockSize; ++i) {
        oKeyPad[i] = oKeyPad[i] ^ keyBytes[i];
        iKeyPad[i] = iKeyPad[i] ^ keyBytes[i];
    }

    QByteArray innerHash = QCryptographicHash::hash(iKeyPad + messageBytes, QCryptographicHash::Sha1);
    QByteArray hmac = QCryptographicHash::hash(oKeyPad + innerHash, QCryptographicHash::Sha1);

    return hmac.toHex();
}

QString SignHelper::sha1(const QString &message) {
    QByteArray messageBytes = message.toUtf8();
    QByteArray hash = QCryptographicHash::hash(messageBytes, QCryptographicHash::Sha1);
    return hash.toHex();
}

QString SignHelper::urlEncode(const QString &value) {
    return QUrl::toPercentEncoding(value, "", ";/?:@&=+$,");
}

QString SignHelper::getKeyTime(int signDurationSeconds) {
    qint64 currentTime = QDateTime::currentSecsSinceEpoch();
    qint64 endTime = currentTime + signDurationSeconds;
    return QString("%1;%2").arg(currentTime).arg(endTime);
}


#endif // SIGNHELPER_H
