#ifndef STG_HPP
#define STG_HPP

#include <util.hpp>

#include <aws/s3/model/BucketLifecycleConfiguration.h>

auto LifeCycle(const std::string& accessKey, const std::string& secretKey, const std::string& bucketName, const std::string& objectName, bool choice, const std::string& action, const std::string& storageClass = "", const std::string& duration = "")
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
    filter.SetPrefix(Aws::String(objectName.c_str(), objectName.size()));

    Aws::S3::Model::LifecycleRule lifeCycle;
    lifeCycle.SetID("Test");
    lifeCycle.SetFilter(filter);
    
    
    Aws::S3::Model::BucketLifecycleConfiguration config;


  }
}

#endif