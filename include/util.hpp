#ifndef UTIL_HPP
#define UTIL_HPP

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/core/auth/AWSCredentialsProvider.h>

#include <fstream>
#include <sstream>
#include <iostream>

auto InitializeClient(const Aws::String& accessKey, const Aws::String& secretKey)
{
  Aws::Client::ClientConfiguration clientConfig;

  clientConfig.proxyHost = Aws::String("127.0.0.1");
  clientConfig.proxyPort = 5555;
  
  Aws::Auth::AWSCredentials credentials;

  credentials.SetAWSAccessKeyId(accessKey);
  credentials.SetAWSSecretKey(secretKey);

  return Aws::S3::S3Client(credentials, clientConfig);
}

auto ReadFromFile(const std::string& file)
{
  std::ifstream hFile(file);
  std::string str;

  if(hFile) 
  {
    std::ostringstream ss;
    ss << hFile.rdbuf();
    str = ss.str();
  }
  
  return str;
}

#endif