#ifndef POL_HPP
#define POL_HPP

#include <util.hpp>

#include <aws/iam/model/CreatePolicyRequest.h>
#include <aws/iam/model/CreatePolicyResult.h>
#include <aws/iam/model/GetPolicyRequest.h>
#include <aws/iam/model/GetPolicyResult.h>
#include <aws/iam/model/ListPoliciesRequest.h>
#include <aws/iam/model/ListPoliciesResult.h>
#include <aws/iam/model/DeletePolicyRequest.h>
#include <aws/s3/model/PutBucketPolicyRequest.h>
#include <aws/s3/model/GetBucketPolicyRequest.h>
#include <aws/s3/model/DeleteBucketPolicyRequest.h>

auto CreateUserPolicy(const std::string& accessKey, const std::string& secretKey, const std::string& policyName, const std::string& policyPath)
{
  auto flag = false;
  Aws::SDKOptions options;

  auto policyValue = ReadFromFile(policyPath);

  if (policyValue.size() == 0)
  {
    std::cout << "Unable tp read file: " << policyPath << "\n";
    goto end;
  }

  Aws::InitAPI(options);
  {
    auto iam = InitializeIAMClient(
      Aws::String(accessKey.c_str(), accessKey.size()), 
      Aws::String(secretKey.c_str(), secretKey.size())
    );

    Aws::IAM::Model::CreatePolicyRequest request;
    request.SetPolicyName(Aws::String(policyName.c_str(), policyName.size()));
    request.SetPolicyDocument(Aws::String(policyValue.c_str(), policyValue.size()));

    auto outcome = iam.CreatePolicy(request);
    if (!outcome.IsSuccess())
    {
      std::cout << "Error creating policy " << policyName << ": " << outcome.GetError().GetMessage() << std::endl;
    }
    else
    {
      std::cout << "Successfully created policy " << policyName << std::endl;
      flag = true;
    }
  }

end:

  Aws::ShutdownAPI(options);

  return ((flag) ? 0 : -1);
}

auto RetrievePolicy(const std::string& accessKey, const std::string& secretKey, const std::string& policyArn)
{
  auto flag = false;
  Aws::SDKOptions options;

  Aws::InitAPI(options);
  {
    auto iam = InitializeIAMClient(
      Aws::String(accessKey.c_str(), accessKey.size()), 
      Aws::String(secretKey.c_str(), secretKey.size())
    );

    Aws::IAM::Model::GetPolicyRequest request;
    request.SetPolicyArn(Aws::String(policyArn.c_str(), policyArn.size()));

    auto outcome = iam.GetPolicy(request);
    if (!outcome.IsSuccess())
    {
      std::cout << "Error getting policy " << policyArn << ": " << outcome.GetError().GetMessage() << std::endl;
    }
    else
    {
      const auto &policy = outcome.GetResult().GetPolicy();
      std::cout << "Name: " << policy.GetPolicyName() << std::endl; 
      std::cout << "ID: " << policy.GetPolicyId() << std::endl;
      std::cout << "Arn: " << policy.GetArn() << std::endl;
      std::cout << "Description: " << policy.GetDescription() << std::endl;

      flag = true;
    }
  }

  Aws::ShutdownAPI(options);

  return ((flag) ? 0 : -1);
}

auto ListPolicies(const std::string& accessKey, const std::string& secretKey)
{
  auto flag = false;
  Aws::SDKOptions options;

  Aws::InitAPI(options);
  {
    auto iam = InitializeIAMClient(
      Aws::String(accessKey.c_str(), accessKey.size()), 
      Aws::String(secretKey.c_str(), secretKey.size())
    );

    Aws::IAM::Model::ListPoliciesRequest request;

    auto outcome = iam.ListPolicies(request);
    if (!outcome.IsSuccess())
    {
      std::cout << "Failed to list iam policies: " << outcome.GetError().GetMessage() << std::endl;
    }
    else
    {
      const auto &policies = outcome.GetResult().GetPolicies();
      for (const auto &policy : policies)
      {
        std::cout << "PolicyName: " << policy.GetPolicyName() << std::endl;
        std::cout << "PolicyId: " << policy.GetPolicyId() << std::endl;
        std::cout << "PolicyArn: " << policy.GetArn() << std::endl;
        std::cout << "Description: " << policy.GetDescription() << std::endl << std::endl;

        flag = true;
      }
    }
  }

end:

  Aws::ShutdownAPI(options);

  return ((flag) ? 0 : -1);
}

auto DeletePolicy(const std::string& accessKey, const std::string& secretKey, const std::string& policyArn)
{
  bool flag = false;
  Aws::SDKOptions options;

  Aws::InitAPI(options);
  {
    auto iam = InitializeIAMClient(
      Aws::String(accessKey.c_str(), accessKey.size()), 
      Aws::String(secretKey.c_str(), secretKey.size())
    );

    Aws::IAM::Model::DeletePolicyRequest delete_request;

    delete_request.SetPolicyArn(Aws::String(policyArn.c_str(), policyArn.size()));

    auto delete_outcome = iam.DeletePolicy(delete_request);

    if (!delete_outcome.IsSuccess())
    { 
      std::cout << "Error deleting policy with arn " << policyArn << ": " << delete_outcome.GetError().GetMessage() << std::endl;
    }
    else
    {
      std::cout << "Successfully deleted policy with arn " << policyArn << std::endl;
      flag = true;
    }

  }
  
  Aws::ShutdownAPI(options);

  return ((flag) ? 0 : -1);
}

auto PutBucketPolicy(const std::string& bucketName, const std::string& accessKey, const std::string& secretKey, const std::string& policyPath)
{
  bool flag = false;
  Aws::SDKOptions options;

  auto policyValue = ReadFromFile(policyPath);

  if (policyValue.size() == 0)
  {
    std::cout << "Unable tp read file: " << policyPath << "\n";
    goto end;
  }
  
  Aws::InitAPI(options);
  {
    auto client = InitializeClient(
      Aws::String(accessKey.c_str(), accessKey.size()),
      Aws::String(secretKey.c_str(), secretKey.size())
    );

    Aws::StringStream ss;
    ss << policyValue;

    std::shared_ptr<Aws::StringStream> stream_ptr = Aws::MakeShared<Aws::StringStream>("WriteStream::Upload" /* log id */, ss.str());

    Aws::S3::Model::PutBucketPolicyRequest request;
    request.SetBucket(Aws::String(bucketName.c_str(), bucketName.size()));
    request.SetBody(stream_ptr);

    Aws::S3::Model::PutBucketPolicyOutcome outcome =client.PutBucketPolicy(request);

    if (outcome.IsSuccess()) 
    {
      std::cout << "Set the following policy body for the bucket '" << bucketName << "':" << std::endl << std::endl;
      std::cout << policyValue << std::endl;

      flag = true;
    }
    else 
    {
      std::cout << "Error: PutBucketPolicy: " << outcome.GetError().GetMessage() << std::endl;
    }
  }

end:

  ShutdownAPI(options);

  return ((flag) ? 0 : -1);
}

auto GetBucketPolicy(const std::string& bucketName, const std::string& accessKey, const std::string& secretKey)
{
  auto flag = false;
  Aws::SDKOptions options;

  Aws::InitAPI(options);
  {
    auto client = InitializeClient(
      Aws::String(accessKey.c_str(), accessKey.size()),
      Aws::String(secretKey.c_str(), secretKey.size())
    );

    Aws::S3::Model::GetBucketPolicyRequest request;
    request.SetBucket(Aws::String(bucketName.c_str(), bucketName.size()));
    
    Aws::S3::Model::GetBucketPolicyOutcome outcome = client.GetBucketPolicy(request);

    if (outcome.IsSuccess())
    {
      Aws::StringStream policy_stream;
      Aws::String line;

      outcome.GetResult().GetPolicy() >> line;
      policy_stream << line;

      std::cout << "Policy:" << std::endl << std::endl << policy_stream.str() << std::endl;

      flag = true;
    }
    else
    {
      auto err = outcome.GetError();
      std::cout << "Error: GetBucketPolicy: " << err.GetExceptionName() << ": " << err.GetMessage() << std::endl;
    }
  }
  
  ShutdownAPI(options);

  return ((flag) ? 0 : -1);
}

auto DeleteBucketPolicy(const std::string& bucketName, const std::string& accessKey, const std::string& secretKey)
{
  auto flag = false;
  Aws::SDKOptions options;

  Aws::InitAPI(options);
  {
    auto client = InitializeClient(
      Aws::String(accessKey.c_str(), accessKey.size()),
      Aws::String(secretKey.c_str(), secretKey.size())
    );

    Aws::S3::Model::DeleteBucketPolicyRequest request;
    request.SetBucket(Aws::String(bucketName.c_str(), bucketName.size()));

    Aws::S3::Model::DeleteBucketPolicyOutcome outcome = client.DeleteBucketPolicy(request);

    if (!outcome.IsSuccess())
    {
      auto err = outcome.GetError();
      std::cout << "Error: DeleteBucketPolicy: " << err.GetExceptionName() << ": " << err.GetMessage() << std::endl;
    }
    else
    {
      std::cout << "Policy Successfully deleted.\n";

      flag = true;
    }
  }

  Aws::ShutdownAPI(options);

  return ((flag) ? 0 : -1);
}

#endif