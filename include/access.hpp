#ifndef ACC_HPP
#define ACC_HPP

#include <util.hpp>

#include <aws/iam/model/UpdateAccessKeyRequest.h>

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