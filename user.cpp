#include "user.h"

QString User::encryptPassword()
{
    QString password=hashedPassword;
    QString uuid=account;
    //以uuid为密钥rc4加密password
    QString result;
    for (int i = 0; i < password.size(); ++i) {
        ushort xor_result = static_cast<ushort>(password[i].unicode()) ^ static_cast<ushort>(uuid[i % uuid.size()].unicode());
        result.append(QChar(xor_result));
    }
    return result;
}

User::User(const QString &username, const QString &account, const QString &password, QObject *parent)
    : QObject(parent), username(username), account(account), hashedPassword(password)
{
    apiRequest = new ApiRequest(this);
    apiRequest->setBaseUrl("https://150.158.90.83");
    channel = new MessageChannel(this);
}

User::User(const QString &account, const QString &password, QObject *parent)
    : QObject(parent), account(account), hashedPassword(password)
{
    apiRequest = new ApiRequest(this);
    apiRequest->setBaseUrl("https://150.158.90.83");
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

bool User::enroll(const QString &filePath)
{
    // 读取文件并转换为base64字符串
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "open file failed!";
        return false;
    }
    QByteArray byteArray = file.readAll();
    QString avatar = byteArray.toBase64();
    QFileInfo fileInfo(filePath);
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
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "open file failed!";
        return false;
    }
    QByteArray byteArray = file.readAll();
    QString avatar = byteArray.toBase64();
    QFileInfo fileInfo(filePath);
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
    QString postData = QString("avatar=%1").arg(avatar);
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

bool User::deleteAccount()
{
    ApiResponse response = apiRequest->get("/deleteAccount");
    return response.isSuccess();
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

TaskToken User::getUnifiedTaskToken()
{
    ApiResponse response = apiRequest->get("/getTaskUnifiedToken");
    if (response.isSuccess())
    {
        TaskToken tasktoken(response.getData());
        return tasktoken;
    }
    else
    {
        QString jsonString = R"({"code": 666, "message": "get unified token error!", "data": "Nooo!"})";
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

bool User::updateUser()
{
    QString postData = QString("username=%1&password=%2").arg(this->username).arg(this->hashedPassword);
    ApiResponse response = apiRequest->post("/updateInfo", postData.toUtf8());
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

COSConfig User::getS3Config()
{
    ApiResponse response = apiRequest->get("/s3Config");
    COSConfig config;
    if (response.isSuccess())
    {
        config.bucketName=response.getData()["bucket"].toString();
        config.region=response.getData()["region"].toString();
        config.appId=response.getData()["appid"].toString();
        config.allowPrefix=response.getData()["allowPrefix"].toString();
    }
    else
    {
        emit channel->message(response.message, "Error");
        return config;
    }
    return config;
}

QVector<QString> User::getS3Dirs()
{
    ApiResponse response = apiRequest->get("/tasks");
    QVector<QString> s3Dirs;

    // 检查API响应是否成功
    if (!response.isSuccess())
    {
        qDebug() << "API request failed with message:" << response.message;
        return s3Dirs; // 返回空的s3Dirs
    }
    if(!response.getDatav().isArray())
    {
        qDebug() << "'data' is not an array";
        return s3Dirs; // 返回空的s3Dirs
    }
    // 提取data数组
    QJsonArray tasksArray = response.getDatav().toArray();

    // 检查数组是否为空
    if (tasksArray.isEmpty())
    {
        qDebug() << "Tasks array is empty";
        return s3Dirs; // 返回空的s3Dirs
    }

    QSet<QString> s3DirsSet; // 使用QSet去重

    // 遍历data数组中的每个任务对象
    for (const QJsonValue& taskValue : tasksArray)
    {
        // 检查每个元素是否是对象
        if (!taskValue.isObject())
        {
            qDebug() << "Task is not an object";
            continue; // 跳过非对象元素
        }

        QJsonObject taskObject = taskValue.toObject();

        // 检查是否存在"s3Dir"键
        if (!taskObject.contains("s3Dir"))
        {
            qDebug() << "No 's3Dir' key in task object";
            continue; // 跳过没有"s3Dir"键的对象
        }

        // 提取s3Dir键的值
        QString s3Dir = taskObject["s3Dir"].toString();

        // 检查提取的值是否为空
        if (s3Dir.isEmpty())
        {
            qDebug() << "s3Dir is empty";
            continue; // 跳过空的s3Dir
        }

        // 将提取的s3Dir路径添加到s3DirsSet中去重
        s3DirsSet.insert(s3Dir);
    }

    // 将QSet转换回QVector
    s3Dirs = QVector<QString>(s3DirsSet.begin(), s3DirsSet.end());

    return s3Dirs;
}
