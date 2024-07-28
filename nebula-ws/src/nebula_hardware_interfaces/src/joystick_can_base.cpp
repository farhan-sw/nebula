#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "can_msgs/msg/frame.hpp"

class CANJoyNode : public rclcpp::Node // MODIFY NAME
{
public:
    CANJoyNode() : Node("joystick_can_base") // MODIFY NAME
    {
        /**
         * Create Parameter
         * @param cmd_vel_topic Topic for subscribing Twist message
         * @param can_bus_topic Topic for publishing CAN message
         */
        // Create Param for topic subscription and publication
        this->declare_parameter<std::string>("cmd_vel_topic", "/cmd_vel_joy");
        this->declare_parameter<std::string>("can_bus_topic", "/to_can_bus");
        // Create Param for Interval Publishing
        this->declare_parameter<int>("publish_interval", 10); // 10 ms interval
        // Create Param for ID Base, default 22
        this->declare_parameter<int>("can_id_base", 22);
        // Create Param for Scale Angular, Linear X, and Linear Y
        this->declare_parameter<int>("scale_angular", 1000);

        /**
         * Create Subscriber, Publisher, and Timer
         * @param subscriber_ Subscriber for Twist message
         * @param publisher_ Publisher for CAN message
         * @param timer_ Timer for publishing message
         */
        // Create Subscriber
        subscriber_ = this->create_subscription<geometry_msgs::msg::Twist>(
            this->get_parameter("cmd_vel_topic").as_string(), 10, std::bind(&CANJoyNode::twist_callback, this, std::placeholders::_1));
        // Create Publisher
        publisher_ = this->create_publisher<can_msgs::msg::Frame>(
            this->get_parameter("can_bus_topic").as_string(), 10);

        // Create Timer to publish message
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(this->get_parameter("publish_interval").as_int()), std::bind(&CANJoyNode::publish_message, this));

        // Log the message information Node has been created
        RCLCPP_INFO(this->get_logger(), "Node CAN Publisher has been created");
    }

private:
    /**
     * Subscribe to /cmd_vel_joy topic
     * @param msg Twist message
     * Publish the message to /to_can_bus topic
     * @param msg can_msgs::msg::Frame message
     */
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscriber_;
    rclcpp::Publisher<can_msgs::msg::Frame>::SharedPtr publisher_;

    /**
     * Create Timer
     */
    rclcpp::TimerBase::SharedPtr timer_;

    /**
     * Create Int16 Holder for Twist message
     */
    int16_t angular_, linear_x_, linear_y_;

    /**
     * Callback function for Twist message
     * @param msg Twist message
     */
    void twist_callback(const geometry_msgs::msg::Twist::SharedPtr msg){
        angular_ = static_cast<int16_t>(msg->angular.z * this->get_parameter("scale_angular").as_int());
        linear_x_ = static_cast<int16_t>(msg->linear.x * this->get_parameter("scale_angular").as_int());
        linear_y_ = static_cast<int16_t>(msg->linear.y * this->get_parameter("scale_angular").as_int());
    }

    /**
     * Publish CAN message to topic
     * @param message can_msgs::msg::Frame message
     */
    void publish_message(){
        auto message = can_msgs::msg::Frame();
        message.header.stamp = this->get_clock()->now();
        message.header.frame_id = "frame_id";
        message.id = this->get_parameter("can_id_base").as_int(); // ID pesan Anda
        message.is_rtr = false;
        message.is_extended = false;
        message.is_error = false;
        message.dlc = 8; 

        // Convert int16_t to byte array
        message.data[0] = angular_ & 0xFF;
        message.data[1] = (angular_ >> 8) & 0xFF;
        message.data[2] = linear_x_ & 0xFF;
        message.data[3] = (linear_x_ >> 8) & 0xFF;
        message.data[4] = linear_y_ & 0xFF;
        message.data[5] = (linear_y_ >> 8) & 0xFF;
        message.data[6] = 0; // Fill remaining bytes with zero
        message.data[7] = 0; // Fill remaining bytes with zero

        publisher_->publish(message);

        // Log the message information
        RCLCPP_INFO(this->get_logger(), "Published message: id=%u, is_rtr=%d, is_extended=%d, is_error=%d, dlc=%u, data=[%u, %u, %u, %u, %u, %u, %u, %u]",
                    message.id, message.is_rtr, message.is_extended, message.is_error, message.dlc,
                    message.data[0], message.data[1], message.data[2], message.data[3],
                    message.data[4], message.data[5], message.data[6], message.data[7]);
        };
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<CANJoyNode>(); // MODIFY NAME
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}