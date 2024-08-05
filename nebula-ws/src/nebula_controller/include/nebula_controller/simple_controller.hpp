#ifndef CONTROLLER_3WD_OMNI_NODE_HPP
#define CONTROLLER_3WD_OMNI_NODE_HPP

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
#include <Eigen/Core>

class Controller3wdOmniNode : public rclcpp::Node // MODIFY NAME
{
public:
    Controller3wdOmniNode(const std::string & name);

private:
    void velocity_callback(const geometry_msgs::msg::TwistStamped & msg);
    rclcpp::Subscription<geometry_msgs::msg::TwistStamped>::SharedPtr velocity_sub_;
    rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr wheel_cmd_pub_;

    double wheel_radius_;
    double wheel_separation_;

    Eigen::Matrix2d speed_conversion_;
};


#endif // CONTROLLER_3WD_OMNI_NODE_HPP