#ifndef STG_HPP
#define STG_HPP

#include <util.hpp>

#include <aws/s3/model/BucketLifecycleConfiguration.h>
#include <aws/s3/model/PutBucketLifecycleConfigurationRequest.h>

auto LifeCycle(const std::string& accessKey, const std::string& secretKey, const std::string& bucketName, const std::string& objectName, bool choice, const std::string& action, const std::string& storageClass = "", int duration = 0)
{
  auto flag = false;
  Aws::SDKOptions options;

  Aws::InitAPI(options);
  {
    auto client = InitializeClient(
      Aws::String(accessKey.c_str(), accessKey.size()),
      Aws::String(secretKey.c_str(), secretKey.size())
    );

    Aws::S3::Model::LifecycleRuleFilter filter;
    // filter.SetPrefix(Aws::String(objectName.c_str(), objectName.size()));

    Aws::S3::Model::LifecycleExpiration expire;
    expire.SetDays(duration);

    Aws::S3::Model::LifecycleRule lifeCycle;
    lifeCycle.SetID("Test");
    lifeCycle.SetFilter(filter);
    lifeCycle.SetExpiration(expire);
    lifeCycle.SetStatus(Aws::S3::Model::ExpirationStatus::Enabled);
    
    Aws::S3::Model::BucketLifecycleConfiguration config;
    config.AddRules(lifeCycle);

    Aws::S3::Model::PutBucketLifecycleConfigurationRequest request;
    request.SetBucket(Aws::String(bucketName.c_str(), bucketName.size()));
    request.SetLifecycleConfiguration(config);

    auto outcome = client.PutBucketLifecycleConfiguration(request);

    if (!outcome.IsSuccess())
    {
      auto err = outcome.GetError();
      std::cout << "Error: PutLifeCycle: " << err.GetExceptionName() << ": " << err.GetMessage() << "\n";
      goto end;
    }

    std::cout << "PutLifeCycle for '" << bucketName << "' was successful.\n";
    flag = true;
  }

end:

  Aws::ShutdownAPI(options);

  return flag;
}

#endif