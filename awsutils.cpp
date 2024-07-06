#include <aws/core/Aws.h>
#include "awsutils.h"

AWSUtils::AWSUtils()
{
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    this->options = options;
}

AWSUtils::~AWSUtils()
{
    Aws::ShutdownAPI(options);
}
