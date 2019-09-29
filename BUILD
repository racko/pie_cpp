cc_library(
    name = "test_main",
    srcs = [
        "test_main.cpp",
    ],
    linkstatic = True,
)

cc_library(
    name = "base",
    hdrs = [
        "pie_base.h",
    ],
    visibility = ["//:__subpackages__"],
)

cc_library(
    name = "pie",
    hdrs = [
        "absurd.h",
        "atom.h",
        "ind_absurd.h",
        "pie.h",
        "strings.h",
        "trivial.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//:base",
        "//either",
        "//eq",
        "//lambda",
        "//nat",
        "//pair",
        "//prelude",
        "//var",
    ],
)

cc_test(
    name = "test",
    srcs = [
        "test1.cpp",
        "test10.cpp",
        "test11.cpp",
        "test12.cpp",
        "test13.cpp",
        "test14.cpp",
        "test15.cpp",
        "test16.cpp",
        "test2.cpp",
        "test3.cpp",
        "test4.cpp",
        "test5.cpp",
        "test6.cpp",
        "test7.cpp",
        "test8.cpp",
        "test9.cpp",
    ],
    deps = [
        ":pie",
        ":test_main",
    ],
)

load("@bazel_compilation_database//:aspects.bzl", "compilation_database")

compilation_database(
    name = "compdb",
    # ideally should be the same as `bazel info execution_root`.
    exec_root = "/home/racko/.cache/bazel/_bazel_racko/7f0b4c7c4acf6796a46181f3473bdf00/execroot/__main__",
    targets = [
        "//:pie",
    ],
)
