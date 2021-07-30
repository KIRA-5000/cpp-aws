#ifndef USER_HPP
#define USER_HPP

#include <util.hpp>

#include <aws/iam/IAMClient.h>
#include <aws/iam/model/CreateUserRequest.h>
#include <aws/iam/model/CreateUserResult.h>


auto CreateUser(const std::string& accessKey, const std::string& secretKey, const std::string& userName)
{
  bool flag = false;
  Aws::SDKOptions options;

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
      flag = true;
    }
  }
  
  Aws::ShutdownAPI(options);

  return ((flag) ? 0 : -1);
}

#endif