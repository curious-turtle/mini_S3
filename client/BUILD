load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "generate_objects",
    srcs = ["tests/generate_objects.cpp"],
    hdrs = ["tests/generate_objects.h"],
    copts = ["-std=c++17"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "test_client_uploader",
    srcs = ["tests/test_upload_objects_single_user.cpp"],
    deps = [":generate_objects","//:httplib"],
    copts = ["-std=c++17"],
    visibility = ["//visibility:public"],
)
