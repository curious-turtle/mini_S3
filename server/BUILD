load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "server_object_store",
    srcs = [
        "main.cpp",
        "engine/object_store.cpp",
        "engine/object_store.h",
    ],
    deps = ["//:httplib"],
    copts = ["-std=c++17"],
    visibility = ["//visibility:public"],
)
