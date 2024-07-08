#ifndef AWSUTILS_H
#define AWSUTILS_H

#include <QObject>
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/ListObjectsV2Request.h>
#include <aws/s3/model/Object.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <fstream>



class AWSUtils : public QObject
{
    Q_OBJECT
public:
    AWSUtils();
    Aws::SDKOptions options;
    Aws::S3::S3Client  S3Client;
    Aws::Client::ClientConfiguration clientConfig;
    std::shared_ptr<Aws::Auth::AWSCredentialsProvider> provider;
    void setEndpoint(const QString &endpoint){
        this->clientConfig.endpointOverride = endpoint.toStdString();
    };
    void setRegion(const QString &region){
        this->clientConfig.region = region.toStdString();
    };
    bool isSessionValid();
    bool listObjects(const QString &bucketName);
    bool getObject(const QString &objectKey,const QString &fromBucket);
    bool putObject(const QString &bucketName,const QString &fileName);

    void updateSessionCredentials(const QString &accessKeyId, const QString &secretAccessKey, const QString &sessionToken);;
    ~AWSUtils();
};

#endif // AWSUTILS_H
