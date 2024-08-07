#ifndef NEBULA_BASE_INTERFACE_HPP_
#define NEBULA_BASE_INTERFACE_HPP_

#include <rclcpp/rclcpp.hpp>
#include <hardware_interface/system_interface.hpp>
#include <rclcpp_lifecycle/node_interfaces/lifecycle_node_interface.hpp>
#include <rclcpp_lifecycle/state.hpp>

#include <vector>
#include <string>

#include "can_msgs/msg/frame.hpp"
#include "nebula_firmware/CAN_COMMAND_ID.hpp"

namespace nebula_firmware
{
    // using CallbackReturn
    using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

    class NebulaBaseInterface : public hardware_interface::SystemInterface
    {
        public:
            NebulaBaseInterface();
            virtual ~NebulaBaseInterface();

            virtual CallbackReturn on_init(const hardware_interface::HardwareInfo &hardware_info) override;

            virtual std::vector<hardware_interface::StateInterface> export_state_interfaces() override;
            virtual std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;

            virtual CallbackReturn on_activate(const rclcpp_lifecycle::State &previous_state) override;
            virtual CallbackReturn on_deactivate(const rclcpp_lifecycle::State &previous_state) override;

            virtual hardware_interface::return_type read(const rclcpp::Time &time, const rclcpp::Duration &period) override;
            virtual hardware_interface::return_type write(const rclcpp::Time &time, const rclcpp::Duration &period) override;

        private:
            can_msgs::msg::Frame can_frame_write_;

            std::vector<double> velocity_command_;
            std::vector<double> position_state_;
            std::vector<double> velocity_state_;

            int16_t velocity_int[4];
            int16_t velocity_command_int[4];

            rclcpp::Time last_run_;

            std::shared_ptr<rclcpp::Node> node_;
            rclcpp::Subscription<can_msgs::msg::Frame>::SharedPtr from_can_sub_;
            rclcpp::Publisher<can_msgs::msg::Frame>::SharedPtr to_can_pub_;

            int device_id_=1;

            void from_can_callback(const can_msgs::msg::Frame::SharedPtr msg);


    };
}  // namespace nebula_firmware



#endif  // NEBULA_BASE_INTERFACE_HPP_