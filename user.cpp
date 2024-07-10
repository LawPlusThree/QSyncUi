#include "user.h"

User::User(const QString &username, const QString &account, const QString &password, QObject *parent)
    : QObject(parent), username(username), account(account), hashedPassword(password)
{
    apiRequest = new ApiRequest(this);
    apiRequest->setBaseUrl("https://syncapi.snakekiss.com");
    channel = new MessageChannel(this);
}

User::User(const QString &account, const QString &password, QObject *parent)
    : QObject(parent), account(account), hashedPassword(password)
{
    apiRequest = new ApiRequest(this);
    apiRequest->setBaseUrl("https://syncapi.snakekiss.com");
    channel = new MessageChannel(this);
}

User::User(const User &user)
{
    username = user.username;
    account = user.account;
    hashedPassword = user.hashedPassword;
    avatarpath = user.avatarpath;
    isLogin = user.isLogin;
    apiRequest = user.apiRequest;
    //apiRequest->setBaseUrl("https://syncapi.snakekiss.com");
    channel = new MessageChannel(this);
}

bool User::enroll()
{
    QString postData = QString("username=%1&email=%2&password=%3").arg(username).arg(account).arg(hashedPassword);
    ApiResponse response = apiRequest->post("/register", postData.toUtf8());
    if (response.isSuccess())
    {
        isLogin = true;
        return true;
    }
    else
    {
        emit channel->message(response.message, "Error");
        return false;
    }
}

bool User::enroll(const QString &avatarpath)
{
    // 读取文件并转换为base64字符串
    QFile file(avatarpath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "open file failed!";
        return false;
    }
    QByteArray byteArray = file.readAll();
    QString avatar = byteArray.toBase64();
    QFileInfo fileInfo(avatarpath);
    QString extension = fileInfo.suffix().toLower();

    if (extension == "png")
    {
        avatar.prepend("data:image/png;base64,");
    }
    else if (extension == "jpg" || extension == "jpeg")
    {
        avatar.prepend("data:image/jpeg;base64,");
    }
    else if (extension == "gif")
    {
        avatar.prepend("data:image/gif;base64,");
    }
    else
    {
        return false; // 图片格式不支持
    }
    // post时保留字符串中的加号
    // avatar.replace("+","%2B");
    // qDebug()<<avatar;
    QString postData = QString("username=%1&email=%2&password=%3&avatar=%4").arg(username).arg(account).arg(hashedPassword).arg(avatar);
    ApiResponse response = apiRequest->post("/register", postData.toUtf8());
    if (response.isSuccess())
    {
        return true;
    }
    else
    {
        emit channel->message(response.message, "Error");
        return false;
    }
}

bool User::login()
{
    QString postData = QString("email=%1&password=%2").arg(account).arg(hashedPassword);
    ApiResponse response = apiRequest->post("/login", postData.toUtf8());
    if (response.isSuccess())
    {
        isLogin = true;
        username = response.getData().value("username").toString();
    }
    else if (response.getCode() == 403)
    {
        isLogin = false;
    }
    avatarpath = response.getData().value("avatar_url").toString();
    qDebug() << "login avatar:" << avatarpath;
    if (response.isSuccess())
    {
        return true;
    }
    else
    {
        emit channel->message(response.message, "Error");
        return false;
    }
}

bool User::forgetPassword()
{
    // 找回密码功能
    return true;
}

bool User::updateAvatar(const QString &filePath)
{
    QImageReader reader(filePath);
    if (!reader.canRead())
    {
        qDebug() << "read picture failed!";
        QString jsonString = R"({"code": 666, "message": "select picture error!", "data": "Nooo!"})";
        QByteArray jsonData = jsonString.toUtf8();
        QJsonDocument document = QJsonDocument::fromJson(jsonData);
        QJsonObject jsonObject = document.object();
        emit updateAvatarResponse(666, jsonObject, "select picture error!");
        return false; // 图片读取失败
    }
    QImage image = reader.read();
    if (image.isNull())
    {
        qDebug() << "read picture failed!";
        QString jsonString = R"({"code": 666, "message": "select picture error!", "data": "Nooo!"})";
        QByteArray jsonData = jsonString.toUtf8();
        QJsonDocument document = QJsonDocument::fromJson(jsonData);
        QJsonObject jsonObject = document.object();
        emit updateAvatarResponse(666, jsonObject, "select picture error!");
        return false; // 图片读取失败
    }
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    image.save(&buffer, reader.format()); // 使用图片的原始格式保存
    // 将字节数据编码为Base64字符串
    QString postData = byteArray.toBase64();
    ApiResponse response = apiRequest->post("/updateAvatar", postData.toUtf8());
    avatarpath = response.getData().value("avatar_url").toString();
    qDebug() << "update avatar:" << avatarpath;
    emit updateAvatarResponse(response.getCode(), response.getData(), response.getMessage());
    if (response.isSuccess())
    {
        return true;
    }
    else
    {
        emit channel->message(response.message, "Error");
        return false;
    }
}

bool User::addTask(const QString &localDir, const QString &s3Dir, int syncType, int usedSize, int totalSize)
{
    // 添加云端task
    QString postData = QString("localDir=%1&s3Dir=%2&syncType=%3&usedSize=%4&totalSize=%5")
                           .arg(localDir)
                           .arg(s3Dir)
                           .arg(syncType)
                           .arg(usedSize)
                           .arg(totalSize);
    ApiResponse response = apiRequest->post("/addTask", postData.toUtf8());
    emit addTaskResponse(response.getCode(), response.getData(), response.getMessage());
    return response.isSuccess();
}

QVector<SyncTask> User::getTask()
{
    // 获取云端task
    ApiResponse response = apiRequest->get("/tasks");
    QJsonArray tasksArray = response.getDatav().toArray();
    QVector<SyncTask> tasks;

    for (int i = 0; i < tasksArray.size(); ++i)
    {
        QJsonObject taskObj = tasksArray[i].toObject();
        SyncTask task(taskObj["localDir"].toString(),
                      taskObj["s3Dir"].toString(),
                      taskObj["syncType"].toInt());
        // taskObj["usedSize"].toDouble(),
        // taskObj["totalSize"].toDouble()
        tasks.push_back(task);
    }
    return tasks;
}

TaskToken User::getTaskToken(int id)
{
    ApiResponse response = apiRequest->get(QString("/getTaskToken?task_id=%1").arg(id));
    if (response.isSuccess())
    {
        TaskToken tasktoken(response.getData());
        return tasktoken;
    }
    else
    {
        QString jsonString = R"({"code": 666, "message": "get token error!", "data": "Nooo!"})";
        QByteArray jsonData = jsonString.toUtf8();
        QJsonDocument document = QJsonDocument::fromJson(jsonData);
        QJsonObject jsonObject = document.object();
        TaskToken errortasktoken(jsonObject);
        return errortasktoken;
    }
}

TaskToken User::getTaskTokenByRemote(QString s3Dir)
{
    ApiResponse response = apiRequest->get(QString("/getTaskTokenByS3Dir?s3Dir=%1").arg(s3Dir));
    if (response.isSuccess())
    {
        TaskToken tasktoken(response.getData());
        return tasktoken;
    }
    else
    {
        QString jsonString = R"({"code": 666, "message": "get tokenRemote error!", "data": "Nooo!"})";
        QByteArray jsonData = jsonString.toUtf8();
        QJsonDocument document = QJsonDocument::fromJson(jsonData);
        QJsonObject jsonObject = document.object();
        TaskToken errortasktoken(jsonObject);
        return errortasktoken;
    }
}

bool User::logout()
{
    ApiResponse response = apiRequest->get("/logout");
    if (response.isSuccess())
    {
        return true;
    }
    else
    {
        emit channel->message(response.message, "Error");
        return false;
    }
}

QString User::getS3Location()
{
    if (isLogin)
    {
        ApiResponse response = apiRequest->get("/s3/Info");
        if (response.isSuccess())
        {
            return response.getData().value("endpoint").toString();
        }
        else
        {
            emit channel->message(response.message, "Error");
            return "";
        }
    }
    else
    {
        qDebug() << "pls login before get s3 location!";
        return "";
    }
}

QString User::getUsername()
{
    return username;
}

QString User::getEmail()
{
    return account;
}

QString User::gethashedPassword()
{
    return hashedPassword;
}

QString User::getAvatarPath()
{
    return avatarpath;
}

QString User::getUserHash() const
{
    QByteArray hash = QCryptographicHash::hash(account.toUtf8(), QCryptographicHash::Sha1);
    return hash.toHex();
}

/*QString User::getSession()
bool User::getisLogin()
{
   // return session;
}*/

bool User::updateUser(User &user)
{
    return true;
}
