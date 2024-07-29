cmake_minimum_required(VERSION 3.8)
project(nebula_vision)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(image_transport REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)

# add executable
add_executable(nebula_vision src/camera_driver.cpp)
ament_target_dependencies(nebula_vision rclcpp sensor_msgs image_transport cv_bridge OpenCV)

install(TARGETS
  nebula_vision
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
