#include <user.hpp>
#include <policy.hpp>
#include <access.hpp>
#include <object.hpp>
#include <bucket.hpp>
#include <settings.hpp>

#include <gflags/gflags.h>

#include <string>
#include <iostream>

DEFINE_string(user, "", "UserName");
DEFINE_string(destination, "", "Destination");
DEFINE_string(akey, "", "Access Key");
DEFINE_string(skey, "", "Secret Key");
DEFINE_string(op, "", "CreateBucket/UploadObject");
DEFINE_string(bucket, "", "Bucket Name");
DEFINE_string(object, "", "Object Name");
DEFINE_string(policyPath, "", "Policy Path");
DEFINE_string(policy, "", "Policy Name");
DEFINE_string(policyArn, "", "PolicyArn");
DEFINE_bool(choice, false, "");
DEFINE_int32(duration, 0, "");

int main(int argc, char *argv[])
{
  auto flag = false;

  gflags::SetVersionString("1.0.0");
  gflags::SetUsageMessage("AWS API");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  if (FLAGS_op == "CreateUser")
  {
    flag = CreateUser(FLAGS_akey, FLAGS_skey, FLAGS_user, FLAGS_policyArn);
  }
  else if (FLAGS_op == "DeleteUser")
  {
    flag = DeleteUser(FLAGS_akey, FLAGS_skey, FLAGS_user);
  }
  else if (FLAGS_op == "ListUsers")
  {
    flag = ListUsers(FLAGS_akey, FLAGS_skey);
  }
  else if (FLAGS_op == "CreatePolicy")
  {
    flag = CreateUserPolicy(FLAGS_akey, FLAGS_skey, FLAGS_policy, FLAGS_policyPath);
  }
  else if (FLAGS_op == "RetrievePolicy")
  {
    flag = RetrievePolicy(FLAGS_akey, FLAGS_skey, FLAGS_policyArn);
  }
  else if (FLAGS_op == "ListPolicies")
  {
    flag = ListPolicies(FLAGS_akey, FLAGS_skey);
  }
  else if (FLAGS_op == "DeletePolicy")
  {
    flag = DeletePolicy(FLAGS_akey, FLAGS_skey, FLAGS_policyArn);
  }
  else if (FLAGS_op == "CreateBucket")
  {
    flag = CreateBucket(FLAGS_akey, FLAGS_skey, FLAGS_bucket);
  }
  else if (FLAGS_op == "DeleteBucket")
  {
    flag = DeleteBucket(FLAGS_akey, FLAGS_skey, FLAGS_bucket);
  }
  else if (FLAGS_op == "UploadObject")
  {
    flag = UploadObject(FLAGS_akey, FLAGS_skey, FLAGS_bucket, FLAGS_object);
  }
  else if (FLAGS_op == "ListObject")
  {
    flag = ListObject(FLAGS_akey, FLAGS_skey, FLAGS_bucket);
  }
  else if (FLAGS_op == "DeleteObject")
  {
    flag = DeleteObject(FLAGS_akey, FLAGS_skey, FLAGS_bucket, FLAGS_object);
  }
  else if(FLAGS_op == "DownloadObject")
  {
    flag = DownloadObject(FLAGS_akey, FLAGS_skey, FLAGS_bucket, FLAGS_object, FLAGS_destination);
  }
  else if (FLAGS_op == "MultipartUpload")
  {
    flag =  MultipartUpload(FLAGS_akey, FLAGS_skey, FLAGS_bucket, FLAGS_object);
  }
  else if (FLAGS_op == "SetLifeCycle")
  {
    flag = LifeCycle(FLAGS_akey, FLAGS_skey, FLAGS_bucket, FLAGS_object, FLAGS_choice, "", "", FLAGS_duration);
  }
  else if (FLAGS_op == "ListAccessKeys")
  {
    flag = ListAccessKeys(FLAGS_akey, FLAGS_skey, FLAGS_user);
  }
  else if (FLAGS_op == "PutBucketPolicy")
  {
    flag = PutBucketPolicy(FLAGS_bucket, FLAGS_akey, FLAGS_skey, FLAGS_policyPath);
  }
  else if(FLAGS_op == "GetBucketPolicy")
  {
    flag = GetBucketPolicy(FLAGS_bucket, FLAGS_akey, FLAGS_skey);
  }
  else if (FLAGS_op == "DeleteBucketPolicy")
  {
    flag = DeleteBucketPolicy(FLAGS_bucket, FLAGS_akey, FLAGS_skey);
  }

  return flag;
}