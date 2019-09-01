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
        "nat.h",
        "pair.h",
        "pie.h",
        "pie_base.h",
    ],
)

cc_test(
    name = "test",
    srcs = [
        "test.cpp",
    ],
    deps = [
        ":pie",
        ":test_main",
    ],
)
