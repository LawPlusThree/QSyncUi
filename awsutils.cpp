#include <aws/core/Aws.h>
#include "awsutils.h"

AWSUtils::AWSUtils()
{
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    this->options = options;
}

void AWSUtils::updateSessionCredentials(const QString &accessKeyId, const QString &secretAccessKey, const QString &sessionToken){
    Aws::Auth::AWSCredentials credentials;
    credentials.SetAWSAccessKeyId(accessKeyId.toStdString());
    credentials.SetAWSSecretKey(secretAccessKey.toStdString());
    credentials.SetSessionToken(sessionToken.toStdString());
    provider = std::make_shared<Aws::Auth::SimpleAWSCredentialsProvider>("s3", credentials);
    this->S3Client = Aws::S3::S3Client(this->clientConfig);
}

AWSUtils::~AWSUtils()
{
    Aws::ShutdownAPI(options);
}
