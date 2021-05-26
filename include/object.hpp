#ifndef OBJ_HPP
#define OBJ_HPP

#include <util.hpp>

#include <aws/s3/model/Object.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/UploadPartRequest.h>
#include <aws/s3/model/ListObjectsRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <aws/s3/model/CreateMultipartUploadRequest.h>
#include <aws/s3/model/CompleteMultipartUploadRequest.h>

#include <string>
#include <fstream>
#include <iostream>

auto UploadObject(const std::string& accessKey, const std::string& secretKey, const std::string& bucketName, const std::string& objectName)
{
  auto flag = false;
  Aws::SDKOptions options;

  Aws::InitAPI(options);
  {
    if ((accessKey.size() == 0) && (secretKey.size() == 0))
    {
      std::cout << "Define access key and secret access key.\n";
      goto end;
    }

    auto client = InitializeClient(
      Aws::String(accessKey.c_str(), accessKey.size()),
      Aws::String(secretKey.c_str(), secretKey.size())
    );

    struct stat buffer;

    if (stat(objectName.c_str(), &buffer) == -1)
    {
      std::cout << "Error: PutObject: File '" << objectName << "' does not exist.\n";
      goto end;
    }

    std::cout << "Object: " << objectName << " verified.\n";

    Aws::S3::Model::PutObjectRequest request;
    request.SetBucket(Aws::String(bucketName.c_str(), bucketName.size()));
    request.SetKey(Aws::String(objectName.c_str(), objectName.size()));

    auto input_data = Aws::MakeShared<Aws::FStream>(
      "SampleAllocationTag", 
      objectName.c_str(), 
      std::ios_base::in | std::ios_base::binary
    );

    request.SetBody(input_data);

    auto outcome = client.PutObject(request);

    if (outcome.IsSuccess()) 
    {
      std::cout << "Added object '" << objectName << "' to bucket '" << bucketName << "'.\n";
    }
    else 
    {
      std::cout << "Error: PutObject: " << outcome.GetError().GetMessage() << "\n";
    }
  }

end:

  Aws::ShutdownAPI(options);

  return flag;
}

auto ListObject(const std::string& accessKey, const std::string& secretKey, const std::string& bucketName)
{
  auto flag = false;
  Aws::SDKOptions options;

  Aws::InitAPI(options);
  {
    if ((accessKey.size() == 0) && (secretKey.size() == 0))
    {
      std::cout << "Define access key and secret access key.\n";
      goto end;
    }

    auto client = InitializeClient(
      Aws::String(accessKey.c_str(), accessKey.size()),
      Aws::String(secretKey.c_str(), secretKey.size())
    );

    Aws::S3::Model::ListObjectsRequest request;

    request.WithBucket(Aws::String(bucketName.c_str(), bucketName.size()));

    auto outcome = client.ListObjects(request);

    if (outcome.IsSuccess())
    {
      flag = true;
      std::cout << "Objects in bucket '" << bucketName << "':\n\n";

      auto objects = outcome.GetResult().GetContents();

      for (int i = 0; i < objects.size(); i++)
      {
        std::cout << objects[i].GetKey() << "\n";
      }
    }
    else
    {
      std::cout << "Error: ListObjects: " << outcome.GetError().GetMessage() << std::endl;
      goto end;
    }
  }

end:

  Aws::ShutdownAPI(options);

  return flag;
}

auto DeleteObject(const std::string& accessKey, const std::string& secretKey, const std::string& bucketName, const std::string& objectName)
{
  auto flag = false;
  Aws::SDKOptions options;

  Aws::InitAPI(options);
  {
    auto client = InitializeClient(
      Aws::String(accessKey.c_str(), accessKey.size()),
      Aws::String(secretKey.c_str(), secretKey.size())
    );

    Aws::S3::Model::DeleteObjectRequest request;
    request.WithKey(Aws::String(objectName.c_str(), objectName.size())).WithBucket(Aws::String(bucketName.c_str(), bucketName.size()));

    auto outcome =  client.DeleteObject(request);

    if (!outcome.IsSuccess())
    {
      auto err = outcome.GetError();
      std::cout << "Error: DeleteObject: " << err.GetExceptionName() << ": " << err.GetMessage() << std::endl;
      goto end;
    }
    else
    {
      std::cout << "Object '" << objectName << "' deleted successfully from bucket '" << bucketName << "'\n";
      flag = true;
    }
  }

end:

  Aws::ShutdownAPI(options);
  return flag; 
}

auto DownloadObject(const std::string& accessKey, const std::string& secretKey, const std::string& bucketName, const std::string& objectName, const std::string& dest)
{
  auto flag = false;
  Aws::SDKOptions options;

  Aws::InitAPI(options);
  {
    auto client = InitializeClient(
      Aws::String(accessKey.c_str(), accessKey.size()),
      Aws::String(secretKey.c_str(), secretKey.size())
    );

    Aws::S3::Model::GetObjectRequest request;
    request.WithBucket(Aws::String(bucketName.c_str(), bucketName.size()));
    request.WithKey(Aws::String(objectName.c_str(), objectName.size()));

    auto outcome =  client.GetObject(request);

    if (outcome.IsSuccess())
    {
      auto& retrieved_file = outcome.GetResultWithOwnership().GetBody();

      Aws::OFStream localFile;

      localFile.open(dest + objectName, std::ios::out | std::ios::binary);
      localFile << outcome.GetResult().GetBody().rdbuf();

      localFile.close();

      std::cout << "File Download Complete\n";
      std::cout << "File Name: " << dest + objectName << "\n";
    }
    else
    {
      auto err = outcome.GetError();
      std::cout << "Error: GetObject: " << err.GetExceptionName() << ": " << err.GetMessage() << "\n";
      goto end;
    }
  }

end:

  Aws::ShutdownAPI(options);
  return flag; 
}

auto MultipartUpload(const std::string& accessKey, const std::string& secretKey, const std::string& bucketName, const std::string& objectName)
{
  Aws::SDKOptions options;

  Aws::InitAPI(options);
  {
    auto client = InitializeClient(
      Aws::String(accessKey.c_str(), accessKey.size()),
      Aws::String(secretKey.c_str(), secretKey.size())
    );

    Aws::S3::Model::CreateMultipartUploadRequest init_request;
    init_request.WithKey(Aws::String(objectName.c_str(), objectName.size())).WithBucket(Aws::String(bucketName.c_str(), bucketName.size()));
    init_request.SetContentType("text/plain");

    auto createMultipartUploadOutcome = client.CreateMultipartUpload(init_request);
    auto upload_id = createMultipartUploadOutcome.GetResult().GetUploadId();

    std::cout << "multiplarts upload id is:" << upload_id << "\n";

    Aws::S3::Model::UploadPartRequest my_request;
    my_request.WithKey(Aws::String(objectName.c_str(), objectName.size())).WithBucket(Aws::String(bucketName.c_str(), bucketName.size()));
    my_request.SetPartNumber(1);
    my_request.SetUploadId(Aws::String(upload_id.c_str(), upload_id.size()));

    std::cout << "Part 1 upload is successful\n";

    Aws::StringStream ss;
    ss << "to upload";

    std::shared_ptr<Aws::StringStream> stream_ptr = Aws::MakeShared<Aws::StringStream>("WriteStream::Upload" /* log id */, ss.str());

    my_request.SetBody(stream_ptr);

    auto uploadPartOutcomeCallable1 = client.UploadPartCallable(my_request);
    auto outcome = uploadPartOutcomeCallable1.get();
    auto etag = outcome.GetResult().GetETag();

    std::cout << "ETag value is:" << etag << std::endl;

    Aws::S3::Model::CompletedPart completedPart1;
    completedPart1.SetPartNumber(1);
    completedPart1.SetETag(etag);

    Aws::S3::Model::CompleteMultipartUploadRequest completeMultipartUploadRequest;
    completeMultipartUploadRequest.WithKey(Aws::String(objectName.c_str(), objectName.size())).WithBucket(Aws::String(bucketName.c_str(), bucketName.size()));
    completeMultipartUploadRequest.SetUploadId(Aws::String(upload_id.c_str(), upload_id.size()));

    Aws::S3::Model::CompletedMultipartUpload completedMultipartUpload;
    completedMultipartUpload.AddParts(completedPart1);
    
    completeMultipartUploadRequest.WithMultipartUpload(completedMultipartUpload);

    auto completeMultipartUploadOutcome = client.CompleteMultipartUpload(completeMultipartUploadRequest);

    if (!completeMultipartUploadOutcome.IsSuccess()) 
    {
      auto error = completeMultipartUploadOutcome.GetError();
      std::stringstream ss;
      ss << error << error.GetExceptionName() << ": " << error.GetMessage() << "\n";

      std::cout << ss.str() << "\n";
    }
    else
    {
      std::cout << "Multipart Upload is Sucessful\n";
    }
  }
    
  Aws::ShutdownAPI(options);
  return false;
}

#endif