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
