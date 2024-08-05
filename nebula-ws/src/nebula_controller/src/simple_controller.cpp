#include "nebula_controller/simple_controller.hpp"


Controller3wdOmniNode(const std::string & name)
    : Node(name)
{
    declare_parameter("wheel_radius", 0.075);
    declare_parameter("wheel_separation", 0.28867);

    wheel_radius_ = get_parameter("wheel_radius").as_double();
    wheel_separation_ = get_parameter("wheel_separation").as_double();

    RCLCPP_INFO(this->get_logger(), "Using with wheel radius: %f and wheel separation: %f", wheel_radius_, wheel_separation_);

    wheel_cmd_pub_ = this->create_publisher<std_msgs::msg::Float64MultiArray>("wheel_cmd", 10);
}