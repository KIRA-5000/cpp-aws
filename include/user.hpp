#ifndef USER_HPP
#define USER_HPP

#include <util.hpp>

#include <aws/iam/model/CreateUserRequest.h>
#include <aws/iam/model/CreateUserResult.h>
#include <aws/iam/model/CreateAccessKeyRequest.h>
#include <aws/iam/model/CreateAccessKeyResult.h>
#include <aws/iam/model/DeleteUserRequest.h>
#include <aws/iam/model/ListUsersRequest.h>
#include <aws/iam/model/ListUsersResult.h>
#include <aws/iam/model/ListAccessKeysRequest.h>
#include <aws/iam/model/ListAccessKeysResult.h>
#include <aws/iam/model/DeleteAccessKeyRequest.h>

auto CreateUser(const std::string& accessKey, const std::string& secretKey, const std::string& userName)
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

    Aws::IAM::Model::CreateUserRequest create_request;
    create_request.SetUserName(Aws::String(userName.c_str(), userName.size()));

    auto create_outcome = iam.CreateUser(create_request);
    if (!create_outcome.IsSuccess())
    {
      std::cout << "Error creating IAM user " << userName << ":" << create_outcome.GetError().GetMessage() << std::endl;
    }
    else
    {
      std::cout << "Successfully created IAM user " << userName << std::endl;

      Aws::IAM::Model::CreateAccessKeyRequest request;
      request.SetUserName(Aws::String(userName.c_str(), userName.size()));

      auto outcome = iam.CreateAccessKey(request);

      if (outcome.IsSuccess())
      {
        accessKeys = outcome.GetResult().GetAccessKey();

        std::cout << "Access Key ID: " << accessKeys.GetAccessKeyId() << "\n";
        std::cout << "Secret Access Key: " << accessKeys.GetSecretAccessKey() << "\n";

        flag = true;
      }
      else
      {
        std::cout << "Error creating access key for IAM user " << userName << ":" << outcome.GetError().GetMessage() << std::endl;
      }
    }
  }
  
  Aws::ShutdownAPI(options);

  return ((flag) ? 0 : -1);
}

auto DeleteUser(const std::string& accessKey, const std::string& secretKey, const std::string& userName)
{
  bool flag = false;
  Aws::SDKOptions options;

  Aws::InitAPI(options);
  {
    auto iam = InitializeIAMClient(
      Aws::String(accessKey.c_str(), accessKey.size()), 
      Aws::String(secretKey.c_str(), secretKey.size())
    );

    Aws::IAM::Model::ListAccessKeysRequest request_list;
    request_list.SetUserName(Aws::String(userName.c_str(), userName.size()));

    auto outcome_list = iam.ListAccessKeys(request_list);
    if (!outcome_list.IsSuccess())
    {
      std::cout << "Failed to list access keys for user " << userName << ": " << outcome_list.GetError().GetMessage() << std::endl;
      goto end;
    }

    const auto &keys = outcome_list.GetResult().GetAccessKeyMetadata();

    for (const auto &key : keys)
    {
      auto statusString = Aws::IAM::Model::StatusTypeMapper::GetNameForStatusType(key.GetStatus());
      std::cout << "Deleting Access ID: " << key.GetAccessKeyId() << std::endl;
      std::cout << "Status: " << statusString << std::endl;

      Aws::IAM::Model::DeleteAccessKeyRequest request_delete;
      request_delete.SetUserName(Aws::String(userName.c_str(), userName.size()));
      request_delete.SetAccessKeyId(key.GetAccessKeyId());

      auto outcome_delete = iam.DeleteAccessKey(request_delete);

      if (!outcome_delete.IsSuccess())
      {
        std::cout << "Error deleting access key " << key.GetAccessKeyId() << " from user " << userName << ": " << outcome_delete.GetError().GetMessage() << std::endl;
        goto end;
      }
      else
      {
        std::cout << "Successfully deleted access key " << key.GetAccessKeyId() << " for IAM user " << userName << std::endl;
      }
    }

    Aws::IAM::Model::DeleteUserRequest request;
    request.SetUserName(Aws::String(userName.c_str(), userName.size()));

    auto outcome = iam.DeleteUser(request);

    if (!outcome.IsSuccess())
    {
      std::cout << "Error deleting IAM user " << userName << ": " << outcome.GetError().GetMessage() << std::endl;
      goto end;
    }

    std::cout << "Successfully deleted IAM user " << userName << std::endl;
    flag = true;
  }

end:

  Aws::ShutdownAPI(options);

  return ((flag) ? 0 : -1);
}

auto ListUsers(const std::string& accessKey, const std::string& secretKey)
{
  bool flag = false;
  Aws::SDKOptions options;

  Aws::InitAPI(options);
  {
    auto iam = InitializeIAMClient(
      Aws::String(accessKey.c_str(), accessKey.size()), 
      Aws::String(secretKey.c_str(), secretKey.size())
    );

    Aws::IAM::Model::ListUsersRequest request;

    auto outcome = iam.ListUsers(request);

    if (!outcome.IsSuccess())
    {
      std::cout << "Failed to list iam users:" << outcome.GetError().GetMessage() << std::endl;
      goto end;
    }

    const auto &users = outcome.GetResult().GetUsers();

    for (const auto &user : users)
    {
      std::cout << "UserName: " << user.GetUserName() << std::endl;
      std::cout << "UserId: " << user.GetUserId() << std::endl;
      std::cout << "Arn: " << user.GetArn() << std::endl << std::endl;
    }
  }

end:

  Aws::ShutdownAPI(options);

  return ((flag) ? 0 : -1);
}

#endif