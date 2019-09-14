cc_library(
    name = "test_main",
    srcs = [
        "test_main.cpp",
    ],
    linkstatic = True,
)

cc_library(
    name = "pie",
    hdrs = [
        "atom.h",
        "lambda.h",
        "nat.h",
        "pair.h",
        "pie.h",
        "pie_base.h",
        "prelude.h",
        "var.h",
    ],
)

cc_test(
    name = "test",
    srcs = [
        "test.cpp",
        "test1.cpp",
        "test2.cpp",
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
