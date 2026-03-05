
#include <iostream>
#include <string>
#include "httplib.h"
#include "engine/object_store.h"

int main()
{
  httplib::Server svr;

  ObjectStore store; // uses default root /tmp/mini_object_store

  svr.Put(R"(/bucket/(.*))", [&store](const httplib::Request &req, httplib::Response &res)
          {
      std::string bucket = req.matches[1];
      std::string err;
      if (!store.put(bucket, err)) {
        if (err == "Bucket already exists") {
          res.status = 409;
          res.set_content(err, "text/plain");
        } else {
          res.status = 500;
          res.set_content(err, "text/plain");
        }
        return;
      }

      res.status = 201;
      res.set_content(std::string("Bucket created: ") + bucket, "text/plain"); });

  svr.Put(R"(/object/([^/]+)/(.*))", [&store](const httplib::Request &req, httplib::Response &res)
          {
      std::string bucket = req.matches[1];
      std::string key = req.matches[2];
      std::string err;
      if (!store.put(bucket, key, req.body, err)) {
        if (err == "Bucket does not exist") {
          res.status = 404;
          res.set_content(err, "text/plain");
        } else if (err == "Object already exists") {
          res.status = 409;
          res.set_content(err, "text/plain");
        } else {
          res.status = 500;
          res.set_content(err, "text/plain");
        }
        return;
      }

      res.status = 201;
      res.set_content(std::string("Stored object: ") + bucket + "/" + key + " size=" + std::to_string(req.body.size()), "text/plain"); });

  svr.Get(R"(/object/([^/]+)/(.*))", [&store](const httplib::Request &req, httplib::Response &res)
          {
      std::string bucket = req.matches[1];
      std::string key = req.matches[2];
      std::string out;
      std::string err;
      if (!store.get(bucket, key, out, err)) {
        if (err == "Object not found") {
          res.status = 404;
          res.set_content(err, "text/plain");
        } else {
          res.status = 500;
          res.set_content(err, "text/plain");
        }
        return;
      }

      res.status = 200;
      res.set_content(out, "application/octet-stream"); });

  std::cout << "Starting server at http://0.0.0.0:8080" << std::endl;
  svr.listen("0.0.0.0", 8080);
  return 0;
}
