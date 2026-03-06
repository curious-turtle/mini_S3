#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <sstream>
#include "httplib.h"
#include "generate_objects.h"

namespace fs = std::filesystem;

int main(int argc, char **argv)
{
    std::string data_dir = "/tmp/client_object_store";
    std::string server = "localhost:8080";
    std::string bucket = "default";

    for (int i = 1; i < argc; ++i)
    {
        std::string a = argv[i];
        const std::string p1 = "--data_dir=";
        const std::string p2 = "--server=";
        const std::string p3 = "--bucket=";
        const std::string p4 = "--generate";
        if (a.rfind(p1, 0) == 0)
            data_dir = a.substr(p1.size());
        else if (a.rfind(p2, 0) == 0)
            server = a.substr(p2.size());
        else if (a.rfind(p3, 0) == 0)
            bucket = a.substr(p3.size());
        else if (a == p4)
        {
            // will trigger generation after parsing args
        }
    }

    std::string host = "localhost";
    int port = 8080;
    // parse server as host:port
    auto pos = server.find(':');
    if (pos != std::string::npos)
    {
        host = server.substr(0, pos);
        try
        {
            port = std::stoi(server.substr(pos + 1));
        }
        catch (...)
        {
            port = 8080;
        }
    }
    else if (!server.empty())
    {
        host = server;
    }

    try
    {
        fs::create_directories(data_dir);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to access data_dir: " << e.what() << std::endl;
        return 1;
    }

    httplib::Client cli(host.c_str(), port);
    cli.set_connection_timeout(5); // seconds

    // Create bucket
    {
        // Optionally generate files if directory is empty or --generate passed
        bool do_generate = false;
        for (int i = 1; i < argc; ++i)
            if (std::string(argv[i]) == "--generate")
                do_generate = true;
        if (do_generate)
        {
            std::cout << "Generating test files in " << data_dir << std::endl;
            if (!generate_test_files(data_dir))
            {
                std::cerr << "File generation failed" << std::endl;
                return 3;
            }
        }

        std::ostringstream path;
        path << "/bucket/" << bucket;
        auto res = cli.Put(path.str().c_str(), std::string(), "text/plain");
        if (!res)
        {
            std::cerr << "Failed to contact server to create bucket" << std::endl;
            return 2;
        }
        if (res->status == 201)
            std::cout << "Created bucket: " << bucket << std::endl;
        else if (res->status == 409)
            std::cout << "Bucket already exists: " << bucket << std::endl;
        else
            std::cout << "Bucket create returned status " << res->status << ": " << res->body << std::endl;
    }

    // Iterate files and PUT
    size_t uploaded = 0;
    for (auto &entry : fs::directory_iterator(data_dir))
    {
        if (!entry.is_regular_file())
            continue;
        fs::path p = entry.path();
        std::string filename = p.filename().string();

        std::ifstream ifs(p, std::ios::binary);
        if (!ifs)
        {
            std::cerr << "Failed to open file: " << p << std::endl;
            continue;
        }
        std::string data((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

        std::ostringstream path;
        path << "/object/" << bucket << "/" << filename;

        auto res = cli.Put(path.str().c_str(), data, "application/octet-stream");
        if (!res)
        {
            std::cerr << "Failed to contact server for object " << filename << std::endl;
            continue;
        }

        if (res->status == 201)
        {
            std::cout << "Uploaded " << filename << " (" << data.size() << " bytes)" << std::endl;
            ++uploaded;
        }
        else if (res->status == 409)
        {
            std::cout << "Skipped (exists) " << filename << std::endl;
        }
        else
        {
            std::cout << "Upload failed " << filename << " status=" << res->status << " body=" << res->body << std::endl;
        }
    }

    std::cout << "Done. Uploaded: " << uploaded << std::endl;
    return 0;
}
