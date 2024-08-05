#include "nebula_controller/simple_controller.hpp"
#include <Eigen/Geometry>

using std::placeholders::_1;


Controller3wdOmniNode::Controller3wdOmniNode(const std::string & name)
    : Node(name)
{
    declare_parameter("wheel_radius", 0.075);
    declare_parameter("wheel_separation", 0.28867);

    wheel_radius_ = get_parameter("wheel_radius").as_double();
    wheel_separation_ = get_parameter("wheel_separation").as_double();

    RCLCPP_INFO(this->get_logger(), "Using with wheel radius: %f and wheel separation: %f", wheel_radius_, wheel_separation_);

    wheel_cmd_pub_ = this->create_publisher<std_msgs::msg::Float64MultiArray>("/simple_velocity_controller/commands", 10);
    velocity_sub_ = this->create_subscription<geometry_msgs::msg::TwistStamped>("/nebula_controller/cmd_vel", 10, 
        std::bind(&Controller3wdOmniNode::velocity_callback, this, _1));

    // Initialize speed conversion matrix for 3WD omniwheel
    speed_conversion_ << 
        1.0, -1.0 / std::sqrt(3.0), -wheel_separation_ / (3.0 * wheel_radius_),
        0.0, 2.0 / std::sqrt(3.0), -wheel_separation_ / (3.0 * wheel_radius_),
        -1.0, -1.0 / std::sqrt(3.0), -wheel_separation_ / (3.0 * wheel_radius_);

}

void Controller3wdOmniNode::velocity_callback(const geometry_msgs::msg::TwistStamped & msg)
{
    Eigen::Vector3d robot_speed(msg.twist.linear.x, msg.twist.linear.y, msg.twist.angular.z);
    // Eigen::Vector3d wheel_speed = speed_conversion_.inverse() * robot_speed;

    // std_msgs::msg::Float64MultiArray wheel_cmd;
    // wheel_cmd.data = {wheel_speed(0), wheel_speed(1), wheel_speed(2)};
    // wheel_cmd_pub_->publish(wheel_cmd);

    /**
     * void inverseKinematics(float Vy, float Vx, float W, float theta, float *wm_front, float *wm_BL, float *wm_BR){ 
            *wm_front = -Vx + (L * W);
            *wm_BL = sin(theta * M_PI / 180.0) * Vx - cos(theta * M_PI / 180.0) * Vy + (L * W);
            *wm_BR = sin(theta * M_PI / 180.0) * Vx + cos(theta * M_PI / 180.0) * Vy + (L * W);

        }
     */

    double Vy = msg.twist.linear.y;
    double Vx = msg.twist.linear.x;
    double W = msg.twist.angular.z;
    double L = wheel_separation_;

    double wm_front = -Vx + (L * W);
    double wm_BL = sin(M_PI / 180.0) * Vx - cos(M_PI / 180.0) * Vy + (L * W);
    double wm_BR = sin(M_PI / 180.0) * Vx + cos(M_PI / 180.0) * Vy + (L * W);

    std_msgs::msg::Float64MultiArray wheel_cmd;
    wheel_cmd.data = {wm_front, wm_BL, wm_BR};
    wheel_cmd_pub_->publish(wheel_cmd);

}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Controller3wdOmniNode>("controller_3wd_omni_node");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}