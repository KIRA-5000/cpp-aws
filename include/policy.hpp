#ifndef POL_HPP
#define POL_HPP

#include <util.hpp>

#include <aws/iam/model/CreatePolicyRequest.h>
#include <aws/iam/model/CreatePolicyResult.h>
#include <aws/iam/model/GetPolicyRequest.h>
#include <aws/iam/model/GetPolicyResult.h>
#include <aws/iam/model/ListPoliciesRequest.h>
#include <aws/iam/model/ListPoliciesResult.h>

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

#endif