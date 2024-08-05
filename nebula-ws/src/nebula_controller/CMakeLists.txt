cmake_minimum_required(VERSION 3.8)
project(nebula_controller)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories(
  include
  ${EIGEN3_INCLUDE_DIR}
)

include_directories($(EIGEN3_INCLUDE_DIR))

add_executable(controller_3wd_omni_node src/simple_controller.cpp)
ament_target_dependencies(controller_3wd_omni_node rclcpp std_msgs geometry_msgs ${Eigen_LIBRARIES})

install(
  DIRECTORY include
  DESTINATION include
)

install(
  TARGETS controller_3wd_omni_node
  DESTINATION lib/${PROJECT_NAME}
)

install(
  DIRECTORY config launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
