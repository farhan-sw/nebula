#include "nebula_firmware/nebula_base_interface.hpp"
#include <hardware_interface/types/hardware_interface_type_values.hpp>




namespace nebula_firmware
{
    NebulaBaseInterface::NebulaBaseInterface()
    {

    }

    /**
     * @brief Destroy the Nebula Base Interface:: Nebula Base Interface object
     */
    NebulaBaseInterface::~NebulaBaseInterface()
    {
        // destructor
    }

    CallbackReturn NebulaBaseInterface::on_init(const hardware_interface::HardwareInfo &hardware_info)
    {
        // on_init
        RCLCPP_INFO(rclcpp::get_logger("NebulaInterface"), "Nebula Base Interface in on_init");

        try{
            device_id_ = std::stoi(hardware_info.hardware_parameters.at("device_id"));
        } catch (...) {
            RCLCPP_ERROR(rclcpp::get_logger("NebulaInterface"), "Failed to get device_id from hardware_parameters");
            return CallbackReturn::FAILURE;
        }
        RCLCPP_INFO(rclcpp::get_logger("NebulaInterface"), "Device ID: %d", device_id_);

        /* ======================= PREPARE HARDWARE INFO ======================= */
        CallbackReturn result = hardware_interface::SystemInterface::on_init(hardware_info);
        if(result != CallbackReturn::SUCCESS)
        {
            return result;
        }
        /* ======================= END PREPARE HARDWARE INFO ======================= */


        /* =========== SUBSCRIBE TO CAN BUS TOPIC AND PUBLISH TO CAN BUS TOPIC ======== */
        try {
            node_ = std::make_shared<rclcpp::Node>("my_can_hardware_interface_node");
            // Create Subscriber
            from_can_sub_ = node_->create_subscription<can_msgs::msg::Frame>(
                FROM_CANBUS_TOPIC, 10, std::bind(&NebulaBaseInterface::from_can_callback, this, std::placeholders::_1));
            // Create Publisher
            to_can_pub_ = node_->create_publisher<can_msgs::msg::Frame>(TO_CANBUS_TOPIC, 10);
        } catch (const std::exception &e) {
            RCLCPP_ERROR(node_->get_logger(), "Exception caught while creating subscriber or publisher: %s", e.what());
        } catch (...) {
            RCLCPP_ERROR(node_->get_logger(), "Unknown exception caught while creating subscriber or publisher");
        }
        /* === END SUBSCRIBE TO CAN BUS TOPIC AND PUBLISH TO CAN BUS TOPIC ============== */

        
        // Reserve vectors
        RCLCPP_INFO(rclcpp::get_logger("NebulaInterface"), "Number of joints: %zu", info_.joints.size());

        try {
            velocity_command_.reserve(info_.joints.size());
            position_state_.reserve(info_.joints.size());
            velocity_state_.reserve(info_.joints.size());
        } catch (const std::exception &e) {
            RCLCPP_ERROR(rclcpp::get_logger("NebulaInterface"), "Exception caught during vector reservation: %s", e.what());
            return CallbackReturn::FAILURE;
        } catch (...) {
            RCLCPP_ERROR(rclcpp::get_logger("NebulaInterface"), "Unknown exception caught during vector reservation");
            return CallbackReturn::FAILURE;
        }

        last_run_ = rclcpp::Clock().now();


        RCLCPP_INFO(rclcpp::get_logger("NebulaInterface"), "Nebula Base Interface on_init success");

        return CallbackReturn::SUCCESS;
    }


    std::vector<hardware_interface::StateInterface> NebulaBaseInterface::export_state_interfaces()
    {
        // export_state_interfaces
        std::vector<hardware_interface::StateInterface> state_interfaces;
        for(size_t i = 0; i < info_.joints.size(); i++)
        {
            state_interfaces.emplace_back(hardware_interface::StateInterface(info_.joints[i].name, 
                hardware_interface::HW_IF_POSITION, &position_state_[i]));
            state_interfaces.emplace_back(hardware_interface::StateInterface(info_.joints[i].name, 
                hardware_interface::HW_IF_VELOCITY, &velocity_state_[i]));
        }

        return state_interfaces;
    }


    std::vector<hardware_interface::CommandInterface> NebulaBaseInterface::export_command_interfaces()
    {
        std::vector<hardware_interface::CommandInterface> command_interfaces;
        for(size_t i = 0; i < info_.joints.size(); i++)
        {
            command_interfaces.emplace_back(hardware_interface::CommandInterface(info_.joints[i].name, 
                hardware_interface::HW_IF_VELOCITY, &velocity_command_[i]));
        }

        return command_interfaces;
    }


    CallbackReturn NebulaBaseInterface::on_activate(const rclcpp_lifecycle::State &previous_state)
    {
        RCLCPP_INFO(rclcpp::get_logger("NebulaInterface"), "Nebula Base Interface in on_activate");

        // Create initial value
        velocity_command_ = {0.0, 0.0, 0.0};
        position_state_ = {0.0, 0.0, 0.0};
        velocity_state_ = {0.0, 0.0, 0.0};

        RCLCPP_INFO(rclcpp::get_logger("NebulaInterface"), "timer started, ready to publish");
        return CallbackReturn::SUCCESS;
        
        
    }
    CallbackReturn NebulaBaseInterface::on_deactivate(const rclcpp_lifecycle::State &previous_state)
    {
        RCLCPP_INFO(rclcpp::get_logger("NebulaInterface"), "Nebula Base Interface in on_deactivate");
        try{
            from_can_sub_.reset();
            to_can_pub_.reset();
            node_.reset();
        } catch (const std::exception &e) {
            RCLCPP_ERROR(node_->get_logger(), "Exception caught while cancelling timer: %s", e.what());
            return CallbackReturn::FAILURE;
        } catch (...) {
            RCLCPP_ERROR(node_->get_logger(), "Unknown exception caught while cancelling timer");
            return CallbackReturn::FAILURE;
        }
        return CallbackReturn::SUCCESS;

    }

    hardware_interface::return_type NebulaBaseInterface::read(const rclcpp::Time &time, const rclcpp::Duration &period)
    {
        // Baca data dari CAN dan update state
        rclcpp::spin_some(node_);  // Jalankan spin untuk memproses callback

        return hardware_interface::return_type::OK;
    }

    hardware_interface::return_type NebulaBaseInterface::write(const rclcpp::Time &time, const rclcpp::Duration &period)
    {
        // Callback untuk publish data ke CAN
        can_frame_write_.header.stamp = rclcpp::Clock().now();
        can_frame_write_.header.frame_id = "frame_id";
        can_frame_write_.is_rtr = false;
        can_frame_write_.is_extended = false;
        can_frame_write_.is_error = false;
        can_frame_write_.dlc = 8;

        /* KONSTRUKSI MSG ID */
        // bit ke 0-4 diisi CMD_ID, bit ke 5-10 diisi device_id_
        can_frame_write_.id = SET_BASE_VELOCITY | (device_id_ << 5);

        // Kalikan velocity_command_ dengan 1000 dan konversi menjadi int16
        for (size_t i = 0; i < 3; ++i)
        {
            velocity_command_int[i] = static_cast<int16_t>(velocity_command_[i] * 1000);
        }

        // Ubah int16_t menjadi byte array dan masukkan ke dalam pesan CAN
        for (size_t i = 0; i < 3; ++i)
        {
            can_frame_write_.data[2 * i]     = static_cast<uint8_t>(velocity_command_int[i] & 0xFF);
            can_frame_write_.data[2 * i + 1] = static_cast<uint8_t>((velocity_command_int[i] >> 8) & 0xFF);
        }

        // Isi 6-7 dengan 0
        can_frame_write_.data[6] = 0;
        can_frame_write_.data[7] = 0;

        try
        {
            to_can_pub_->publish(can_frame_write_);
        }
        catch (const std::exception &e)
        {
            RCLCPP_ERROR(node_->get_logger(), "Exception caught while publishing message: %s", e.what());
        }
        catch (...)
        {
            RCLCPP_ERROR(node_->get_logger(), "Unknown exception caught while publishing message");
        }

        return hardware_interface::return_type::OK;
    }

    void NebulaBaseInterface::from_can_callback(const can_msgs::msg::Frame::SharedPtr msg)
    {
        auto dt = (rclcpp::Clock().now() - last_run_).seconds();

        // Callback untuk membaca data dari CAN
        // Pastikan 5 bit pertama dari msg->id sama dengan GET_BASE_VELOCITY
        if((msg->id & 0x1F) == GET_BASE_VELOCITY)
        {
            /* EKSTRAK VELOCITY */
            for (size_t i = 0; i < 3; ++i)
            {
                velocity_int[i] = static_cast<int16_t>(msg->data[2 * i] | (msg->data[2 * i + 1] << 8));
                velocity_state_.at(i) = static_cast<double>(velocity_int[i]) / 1000.0;
            }

            /* EKSTRAK POSISI (based on dt) */
            for (size_t i = 0; i < 3; ++i)
            {
                position_state_.at(i) += velocity_state_.at(i) * dt;
            }
        }

        last_run_ = rclcpp::Clock().now();
    }


}


#include <pluginlib/class_list_macros.hpp>
PLUGINLIB_EXPORT_CLASS(nebula_firmware::NebulaBaseInterface, hardware_interface::SystemInterface)