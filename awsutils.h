#ifndef AWSUTILS_H
#define AWSUTILS_H
/*
#include <QObject>
//#include <aws/core/Aws.h>
//#include <aws/core/auth/AWSCredentialsProvider.h>
//#include <aws/s3/S3Client.h>

//using namespace Aws;
//using namespace Aws::Auth;

class AWSUtils : public QObject
{
    Q_OBJECT
public:
    AWSUtils();
    Aws::SDKOptions options;
    Aws::S3::S3Client  S3Client;
    Aws::Client::ClientConfiguration clientConfig;
    void setEndpoint(const QString &endpoint){
        this->clientConfig.endpointOverride = endpoint.toStdString();
    };
    void updateSessionCredentials(const QString &accessKeyId, const QString &secretAccessKey, const QString &sessionToken){
        Aws::Auth::AWSCredentials credentials;
        credentials.SetAWSAccessKeyId(accessKeyId.toStdString());
        credentials.SetAWSSecretKey(secretAccessKey.toStdString());
        credentials.SetSessionToken(sessionToken.toStdString());

        //auto provider = std::make_shared<Aws::Auth::SimpleAWSCredentialsProvider>("s3", credentials);
        //this->S3Client = Aws::S3::S3Client(credentials, this->clientConfig);
    };
    ~AWSUtils();
};
*/
#endif // AWSUTILS_H
