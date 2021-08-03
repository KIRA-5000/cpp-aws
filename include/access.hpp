#ifndef ACC_HPP
#define ACC_HPP

#include <util.hpp>

#include <aws/iam/model/UpdateAccessKeyRequest.h>
#include <aws/iam/model/CreateAccessKeyRequest.h>
#include <aws/iam/model/CreateAccessKeyResult.h>
#include <aws/iam/model/UpdateAccessKeyRequest.h>

auto CreateAccessKey(const std::string& accessKey, const std::string& secretKey, const std::string& userName)
{
  bool flag = false;
  Aws::SDKOptions options;
  Aws::IAM::Model::AccessKey accessKeys;

  Aws::InitAPI(options);
  {
    auto iam = InitializeIAMClient(
    Aws::String(accessKey.c_str(), accessKey.size()), 
    Aws::String(secretKey.c_str(), secretKey.size())
    );

    Aws::IAM::Model::CreateAccessKeyRequest create_request;
    create_request.SetUserName(Aws::String(userName.c_str(), userName.size()));

    auto create_outcome = iam.CreateAccessKey(create_request);
    if (!create_outcome.IsSuccess())
    {
      std::cout << "Error creating access key for IAM user " << userName << ":" << create_outcome.GetError().GetMessage() << std::endl;
    }
    else
    {
      const auto &accessKey = create_outcome.GetResult().GetAccessKey();
      std::cout << "Successfully created access key for IAM user " << userName << std::endl << "  aws_access_key_id = " << accessKey.GetAccessKeyId() << std::endl << " aws_secret_access_key = " << accessKey.GetSecretAccessKey() << std::endl;
      flag = true;
    }
  }
   
  Aws::ShutdownAPI(options);

  return ((flag) ? 0 : -1);
}


auto DeleteKey(const std::string& accessKey, const std::string& secretKey, const std::string& userName, const std::string& keyId)
{
  bool flag = false;
  Aws::SDKOptions options;
  Aws::IAM::Model::AccessKey accessKeys;

  Aws::InitAPI(options);
  {
    auto iam = InitializeIAMClient(
    Aws::String(accessKey.c_str(), accessKey.size()), 
    Aws::String(secretKey.c_str(), secretKey.size())
    );

    Aws::IAM::Model::DeleteAccessKeyRequest delete_request;

    delete_request.SetUserName(Aws::String(userName.c_str(), userName.size()));
    delete_request.SetAccessKeyId(Aws::String(keyId.c_str(), keyId.size()));

    auto delete_outcome = iam.DeleteAccessKey(delete_request);
    if (!delete_outcome.IsSuccess())
    {
      std::cout << "Error deleting access key " << keyId << " from user "<< userName << ": " << delete_outcome.GetError().GetMessage() << std::endl;
    }
    else
    {
      std::cout << "Successfully deleted access key " << keyId << " for IAM user " << userName << std::endl;
      flag = true;
    }

  }
  
  Aws::ShutdownAPI(options);

  return ((flag) ? 0 : -1);
}

auto ChangeKeyStatus(const std::string& userName, const std::string& accessKey, const std::string& stat)
{
  bool flag = false;
  Aws::SDKOptions options;

  Aws::InitAPI(options);
  {
    Aws::IAM::IAMClient iam;
    Aws::IAM::Model::UpdateAccessKeyRequest request;

    request.SetUserName(Aws::String(userName.c_str(), userName.size()));
    request.SetAccessKeyId(Aws::String(accessKey.c_str(), accessKey.size()));

    auto status = Aws::IAM::Model::StatusTypeMapper::GetStatusTypeForName(Aws::String(stat.c_str(), stat.size()));

    request.SetStatus(status);

    auto outcome = iam.UpdateAccessKey(request);
    if (outcome.IsSuccess())
    {
      std::cout << "Successfully updated status of access key " << accessKey << " for user " << userName << std::endl;
      flag = true;
    }
    else
    {
      std::cout << "Error updated status of access key " << accessKey << " for user " << userName << ": " << outcome.GetError().GetMessage() << std::endl;
    }
  }

  Aws::ShutdownAPI(options);

  return ((flag) ? 0 : -1);
}

#endif