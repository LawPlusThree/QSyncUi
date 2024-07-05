#ifndef AWSUTILS_H
#define AWSUTILS_H

#include <QObject>
//#include <aws/core/Aws.h>
//#include <aws/s3/S3Client.h>
//#include <aws/core/auth/AWSCredentialsProviderChain.h>
//using namespace Aws;
//using namespace Aws::Auth;

class AWSUtils: public QObject
{
    Q_OBJECT
public:
    AWSUtils();
    //Aws::SDKOptions options;
};

#endif // AWSUTILS_H
