load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "generate_objects",
    srcs = ["tests/generate_objects.cpp"],
    hdrs = ["generate_objects.h"],
    copts = ["-std=c++17"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "client_uploader",
    srcs = ["main.cpp"],
    deps = [":generate_objects"],
    copts = ["-std=c++17"],
    visibility = ["//visibility:public"],
)
