#ifndef BKT_HPP
#define BKT_HPP

#include <util.hpp>

#include <aws/s3/model/CreateBucketRequest.h>
#include <aws/s3/model/BucketLocationConstraint.h>

auto CreateBucket(const std::string& accessKey, const std::string& secretKey, const std::string& bucketName)
{
  auto flag = false;
  Aws::SDKOptions options;

  Aws::InitAPI(options);
  {
    std::cout << "Client Created\n";

    if ((accessKey.size() == 0) && (secretKey.size() == 0))
    {
      std::cout << "Define access key and secret access key.\n";
      goto end;
    }

    auto client = InitializeClient(
      Aws::String(accessKey.c_str(), accessKey.size()),
      Aws::String(secretKey.c_str(), secretKey.size())
    );

    std::cout << "Client Initialized\n";

    Aws::S3::Model::CreateBucketRequest request;
    request.SetBucket(Aws::String(bucketName.c_str(), bucketName.size()));

    auto outcome = client.CreateBucket(request);

    if (!outcome.IsSuccess())
    {
      auto err = outcome.GetError();
      std::cout << "Error: CreateBucket: " << err.GetExceptionName() << ": " << err.GetMessage() << "\n";
      goto end;
    }

    std::cout << "Create Bucket: Succedded\n";
    flag = true;
  }

end:

  Aws::ShutdownAPI(options);

  return flag;
}

#endif