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

// snippet-start:[s3.cpp.list_objects.code]
bool AWSUtils::listObjects(const QString &bucketName) {
    Aws::S3::Model::ListObjectsV2Request request;
    request.WithBucket(bucketName.toStdString());// 设置请求的存储桶名称

    // 用于分页的继续标记
    Aws::String continuationToken;
    // 存储所有对象的向量
    Aws::Vector<Aws::S3::Model::Object> allObjects;
     // 循环处理分页
    do {
        // 如果继续标记不为空，则设置为请求的继续标记
        if (!continuationToken.empty()) {
            request.SetContinuationToken(continuationToken);
        }
        // 调用S3Client的ListObjectsV2方法获取对象列表
        auto outcome = S3Client.ListObjectsV2(request);

        if (!outcome.IsSuccess()) {
            std::cerr << "Error: listObjects: " <<
                outcome.GetError().GetMessage() << std::endl;
            return false;
        } else {
            // 获取请求结果中的对象列表
            Aws::Vector<Aws::S3::Model::Object> objects =
                outcome.GetResult().GetContents();
            // 将获取的对象添加到allObjects向量中
            allObjects.insert(allObjects.end(), objects.begin(), objects.end());
            // 获取下一个继续标记
            continuationToken = outcome.GetResult().GetNextContinuationToken();
        }
    } while (!continuationToken.empty());// 当继续标记为空时，结束循环

    // 输出找到的对象数量
    std::cout << allObjects.size() << " object(s) found:" << std::endl;
    // 遍历allObjects向量，输出每个对象的键
    for (const auto &object: allObjects) {
        std::cout << "  " << object.GetKey() << std::endl;
    }

    return true;
}
// snippet-start:[s3.cpp.get_object.code]
bool AWSUtils::getObject(const QString &objectKey,
                           const QString &fromBucket) {
    Aws::S3::Model::GetObjectRequest request;
    request.SetBucket(fromBucket.toStdString());
    request.SetKey(objectKey.toStdString());

    Aws::S3::Model::GetObjectOutcome outcome =
        S3Client.GetObject(request);

    if (!outcome.IsSuccess()) {
        const Aws::S3::S3Error &err = outcome.GetError();
        std::cerr << "Error: getObject: " <<
            err.GetExceptionName() << ": " << err.GetMessage() << std::endl;
    } else {
        std::cout << "Successfully retrieved '" << objectKey.toStdString() << "' from '"
                  << fromBucket.toStdString() << "'." << std::endl;
    }

    return outcome.IsSuccess();
}
// snippet-start:[s3.cpp.put_object.code]
bool AWSUtils::putObject(const QString &bucketName,
                           const QString &fileName) {

    Aws::S3::Model::PutObjectRequest request;
    request.SetBucket(bucketName.toStdString());
    //We are using the name of the file as the key for the object in the bucket.
    //However, this is just a string and can be set according to your retrieval needs.
    request.SetKey(fileName.toStdString());

    std::shared_ptr<Aws::IOStream> inputData =
        Aws::MakeShared<Aws::FStream>("SampleAllocationTag",
                                      fileName.toStdString().c_str(),
                                      std::ios_base::in | std::ios_base::binary);

    if (!*inputData) {
        std::cerr << "Error unable to read file " << fileName.toStdString() << std::endl;
        return false;
    }

    request.SetBody(inputData);

    Aws::S3::Model::PutObjectOutcome outcome =
        S3Client.PutObject(request);

    if (!outcome.IsSuccess()) {
        std::cerr << "Error: putObject: " <<
            outcome.GetError().GetMessage() << std::endl;
    } else {
        std::cout << "Added object '" << fileName.toStdString() << "' to bucket '"
                  << bucketName.toStdString() << "'.";
    }

    return outcome.IsSuccess();
}
