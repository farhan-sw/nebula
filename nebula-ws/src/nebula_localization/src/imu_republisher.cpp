#include "rclcpp/rclcpp.hpp"
#include <sensor_msgs/msg/imu.hpp>

using namespace std::chrono_literals;
using std::placeholders::_1;


class ImuRepublisherNode : public rclcpp::Node // MODIFY NAME
{
public:
    ImuRepublisherNode() : Node("imu_republisher") // MODIFY NAME
    {
        // Create Publisher and Subs for republishing IMU data
        imu_pub_ = this->create_publisher<sensor_msgs::msg::Imu>("imu_republisher/imu", 10); // MODIFY NAME
        imu_sub_ = this->create_subscription<sensor_msgs::msg::Imu>("imu_republisher/imu_raw", 10, std::bind(&ImuRepublisherNode::imu_callback, this, _1)); // MODIFY NAME
    }

private:
    rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imu_pub_; // MODIFY NAME
    rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_sub_; // MODIFY NAME

    void imu_callback(const sensor_msgs::msg::Imu::SharedPtr msg)
    {
        // Republish IMU data
        sensor_msgs::msg::Imu new_imu_msg;
        new_imu_msg = *msg;

        new_imu_msg.header.frame_id = "base_footprint_ekf";
        imu_pub_->publish(new_imu_msg);
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ImuRepublisherNode>(); // MODIFY NAME
    rclcpp::sleep_for(1s);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
