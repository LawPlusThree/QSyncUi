#include <aws/core/Aws.h>
#include "awsutils.h"

AWSUtils::AWSUtils()
{
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    // 保存options以便稍后关闭SDK
    this->options = options;
}

AWSUtils::~AWSUtils()
{
    Aws::ShutdownAPI(options);
}
