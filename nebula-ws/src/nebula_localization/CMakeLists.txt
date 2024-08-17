cmake_minimum_required(VERSION 3.8)
project(nebula_localization)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(nav_msgs REQUIRED)

# INstall inlcude directory
include_directories(include)
install(
  DIRECTORY include
  DESTINATION include
)

# Add executable for CPP file
add_executable(kalman_filter src/kalman_filter.cpp)
ament_target_dependencies(kalman_filter rclcpp sensor_msgs nav_msgs)

add_executable(imu_republisher src/imu_republisher.cpp)
ament_target_dependencies(imu_republisher rclcpp sensor_msgs nav_msgs)

install(TARGETS
  kalman_filter
  imu_republisher
  DESTINATION lib/${PROJECT_NAME}
)

# Install launch file, urdf file, config file, or other files
install(
  DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME}
)


ament_package()
