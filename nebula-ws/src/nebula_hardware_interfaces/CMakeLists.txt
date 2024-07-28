cmake_minimum_required(VERSION 3.8)
project(nebula_hardware_interfaces)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ffind dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(can_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

add_executable(joystick_can_base src/joystick_can_base.cpp)
ament_target_dependencies(joystick_can_base rclcpp can_msgs geometry_msgs)

install(TARGETS
  joystick_can_base
  DESTINATION lib/${PROJECT_NAME}
)

install(PROGRAMS
  scripts/setup_can.sh
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
