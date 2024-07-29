#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joy.hpp"
#include "can_msgs/msg/frame.hpp"

class LogicThetaNode : public rclcpp::Node
{
public:
    LogicThetaNode() : Node("logic_theta")
    {
        // Create Publisher for CAN Message
        publisher_ = this->create_publisher<can_msgs::msg::Frame>("/to_can_bus", 10);
        // Create Subscriber for Joy Message
        subscriber_ = this->create_subscription<sensor_msgs::msg::Joy>("/joy", 10, std::bind(&LogicThetaNode::joy_callback, this, std::placeholders::_1));

        // Create Timer for publishing message
        timer_ = this->create_wall_timer(std::chrono::milliseconds(10), std::bind(&LogicThetaNode::publish_message, this));

        // Log the message information Node has been created
        RCLCPP_INFO(this->get_logger(), "Node Logic Theta has been created");
    }

private:
    /**
     * Create publisher, subscriber, and timer
     */
    rclcpp::Publisher<can_msgs::msg::Frame>::SharedPtr publisher_;
    rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr subscriber_;

    /**
     * Create Timer
     */
    rclcpp::TimerBase::SharedPtr timer_;

    /**
     * Create Variable
     */
    int32_t omega_ = 0;
    int32_t time_button_pressed_ = 0;

    /**
     * Subscribe to /joy topic
     * @param msg Joy message
     * Publish the message to /to_can_bus topic
     * @param msg can_msgs::msg::Frame message
     * @brief Fungsi ini akan melakukan penambahan omega dengan limit 360. Misal terdeteksi tombol data[7] ditekan, akan melakukan increment omega
     * increment omega akan membentuk garis lurus selama tombol ditekan dengan memanfaatkan waktu tombol ditekan. Jika mencapai 360, akan direset ke 0.
     * Kebalikannya, jika tombol data[8] ditekan, akan melakukan decrement omega. Jika kedua tombol ditekan, maka omega akan direset ke 0.
     */
    #include <chrono>

    void joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg)
    {
        auto now = this->get_clock()->now();
        int64_t current_time_ms = now.seconds() * 1000 + now.nanoseconds() / 1000000;

        if (msg->buttons[7])
        {
            omega_ = 0;
            time_button_pressed_ = 0;
        }
        else if (msg->buttons[5] == 1)
        {
            if (time_button_pressed_ == 0)
            {
                time_button_pressed_ = current_time_ms;
            }
            omega_ = ((current_time_ms - time_button_pressed_) * 360 / 5 / 1000) % 360; // 120 = 360 / 5
        }
        else if (msg->buttons[4] == 1)
        {
            if (time_button_pressed_ == 0)
            {
                time_button_pressed_ = current_time_ms;
            }
            omega_ = (360 - ((current_time_ms - time_button_pressed_) * 360 / 5 / 1000) % 360) % 360;
        }
        else
        {
            time_button_pressed_ = 0;
        }
    }



    /**
     * Publish message to /to_can_bus topic
     */
    void publish_message()
    {
        auto message = can_msgs::msg::Frame();
        message.id = 23;
        message.dlc = 8;

        message.header.stamp = this->get_clock()->now();
        message.header.frame_id = "frame_id";
        message.is_rtr = false;
        message.is_extended = false;
        message.is_error = false;

        // Fill data with omega_
        message.data[0] = omega_ & 0xFF;
        message.data[1] = (omega_ >> 8) & 0xFF;

        // Set remaining data bytes to 0
        for (int i = 2; i < 8; ++i)
        {
            message.data[i] = 0;
        }

        // Log the message information
        RCLCPP_INFO(this->get_logger(), "Published message: id=%u, is_rtr=%d, is_extended=%d, is_error=%d, dlc=%u, data=[%u, %u, %u, %u, %u, %u, %u, %u]",
                    message.id, message.is_rtr, message.is_extended, message.is_error, message.dlc,
                    message.data[0], message.data[1], message.data[2], message.data[3],
                    message.data[4], message.data[5], message.data[6], message.data[7]);

        publisher_->publish(message);
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<LogicThetaNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
