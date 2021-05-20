#ifndef UTIL_HPP
#define UTIL_HPP

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/core/auth/AWSCredentialsProvider.h>

auto InitializeClient(const Aws::String& accessKey, const Aws::String& secretKey)
{
  Aws::Client::ClientConfiguration clientConfig;
  Aws::Auth::AWSCredentials credentials;

  credentials.SetAWSAccessKeyId(accessKey);
  credentials.SetAWSSecretKey(secretKey);

  return Aws::S3::S3Client(credentials, clientConfig);
}

#endif