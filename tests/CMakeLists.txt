cmake_minimum_required(VERSION 3.13)

enable_testing()

add_executable(messagethreadpool_perf
  src/messagethreadpool_perf.cc
)
target_link_libraries(messagethreadpool_perf
  bcl
)

add_executable(threadpool_class_perf
  src/threadpool_class_perf.cc
)
target_link_libraries(threadpool_class_perf
  bcl
)

add_executable(threadpool_lambda_perf
  src/threadpool_lambda_perf.cc
)
target_link_libraries(threadpool_lambda_perf
  bcl
)

add_executable(bcl_tests
  src/socketaddress_test.cc
  src/ipaddress_test.cc
)
target_link_libraries(bcl_tests
  PRIVATE
    bcl
)

add_dependency(TARGET bcl_tests LIB gtest gtest_main VCPKG_PKG GTEST VCPKG_LIB GTest::gtest GTest::gtest_main)