#include "awsutils.h"

AWSUtils::AWSUtils()
{
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    this->options = options;
}

void AWSUtils::updateSessionCredentials(const QString &accessKeyId, const QString &secretAccessKey, const QString &sessionToken){
    provider = std::make_shared<Aws::Auth::SimpleAWSCredentialsProvider>(accessKeyId.toStdString(),secretAccessKey.toStdString(),sessionToken.toStdString());
    this->S3Client = Aws::S3::S3Client(this->clientConfig);
}

AWSUtils::~AWSUtils()
{
    Aws::ShutdownAPI(options);
}
