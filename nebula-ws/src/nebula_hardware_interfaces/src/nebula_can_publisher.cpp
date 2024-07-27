#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/header.hpp"
#include "can_msgs/msg/frame.hpp" // Include the message definition

class CanSendNode : public rclcpp::Node
{
public:
    CanSendNode() : Node("nebula_can"), int1_(0), int2_(0), int3_(0)
    {
        publisher_ = this->create_publisher<can_msgs::msg::Frame>("/to_can_bus", 10);
        subscriber_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "/cmd_vel_joy", 10, std::bind(&CanSendNode::twist_callback, this, std::placeholders::_1));
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(10), std::bind(&CanSendNode::publish_message, this)); // 10 ms interval
    }

private:
    void twist_callback(const geometry_msgs::msg::Twist::SharedPtr msg)
    {
        int1_ = static_cast<int16_t>(msg->angular.z * 1000); // Scale angular.z
        int2_ = static_cast<int16_t>(msg->linear.x * 1000); // Scale linear.x
        int3_ = static_cast<int16_t>(msg->linear.y * 1000); // Scale linear.y
    }

    void publish_message()
    {
        auto message = can_msgs::msg::Frame();
        message.header.stamp = this->get_clock()->now();
        message.header.frame_id = "frame_id";
        message.id = 22; // ID pesan Anda
        message.is_rtr = false;
        message.is_extended = false;
        message.is_error = false;
        message.dlc = 8; // 3 x int16 = 6 bytes

        // Convert int16_t to byte array
        message.data[0] = int1_ & 0xFF;
        message.data[1] = (int1_ >> 8) & 0xFF;
        message.data[2] = int2_ & 0xFF;
        message.data[3] = (int2_ >> 8) & 0xFF;
        message.data[4] = int3_ & 0xFF;
        message.data[5] = (int3_ >> 8) & 0xFF;
        message.data[6] = 0; // Fill remaining bytes with zero
        message.data[7] = 0; // Fill remaining bytes with zero

        publisher_->publish(message);

        // Log the message information
        RCLCPP_INFO(this->get_logger(), "Published message: id=%u, is_rtr=%d, is_extended=%d, is_error=%d, dlc=%u, data=[%u, %u, %u, %u, %u, %u, %u, %u]",
                    message.id, message.is_rtr, message.is_extended, message.is_error, message.dlc,
                    message.data[0], message.data[1], message.data[2], message.data[3],
                    message.data[4], message.data[5], message.data[6], message.data[7]);
    }

    rclcpp::Publisher<can_msgs::msg::Frame>::SharedPtr publisher_;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscriber_;
    rclcpp::TimerBase::SharedPtr timer_;
    int16_t int1_, int2_, int3_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<CanSendNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
