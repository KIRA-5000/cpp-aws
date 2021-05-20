#include <object.hpp>
#include <bucket.hpp>

#include <gflags/gflags.h>

#include <string>
#include <iostream>

DEFINE_string(destination, "", "Destination");
DEFINE_string(akey, "", "Access Key");
DEFINE_string(skey, "", "Secret Key");
DEFINE_string(op, "", "CreateBucket/UploadObject");
DEFINE_string(bucket, "", "Bucket Name");
DEFINE_string(object, "", "Object Name");

int main(int argc, char *argv[])
{
  auto flag = false;

  gflags::SetVersionString("1.0.0");
  gflags::SetUsageMessage("AWS API");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  if (FLAGS_op == "CreateBucket")
  {
    flag = CreateBucket(FLAGS_akey, FLAGS_skey, FLAGS_bucket);
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

  return flag;
}