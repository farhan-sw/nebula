#include "nebula_localization/kalman_filter.hpp"

using std::placeholders::_1;

KalmanFilter::KalmanFilter(const std::string & name) : Node(name)
    , is_first_odom_(true)
    , motion_(0.0)
    , imu_angular_z_(0.0)
    , last_angular_z_(0.0)
    , mean_(0.0)
    , variance_(1000.0)
    , motion_variance_(1.0)
    , measurement_variance_(0.5)
{
    odom_sub_ = this->create_subscription<nav_msgs::msg::Odometry>("kalman_filter/odom_noisy", 10, std::bind(&KalmanFilter::odom_callback, this, _1));
    imu_sub_ = this->create_subscription<sensor_msgs::msg::Imu>("kalman_filter/imu", 10, std::bind(&KalmanFilter::imu_callback, this, _1));
    odom_pub_ = this->create_publisher<nav_msgs::msg::Odometry>("kalman_filter/odom_kalman", 10);

}

void KalmanFilter::odom_callback(const nav_msgs::msg::Odometry &odom)
{
    kalman_odom_ = odom;

    if (is_first_odom_)
    {
        mean_ = odom.twist.twist.angular.z;
        last_angular_z_ = odom.twist.twist.angular.z;
        is_first_odom_ = false;
        return;
    }

    motion_ = odom.twist.twist.angular.z - last_angular_z_;

    state_prediction();
    measurement_update();

    kalman_odom_.twist.twist.angular.z = mean_;
    odom_pub_->publish(kalman_odom_);

    // Update last_angular_z_
    last_angular_z_ = odom.twist.twist.angular.z;
}

void KalmanFilter::imu_callback(const sensor_msgs::msg::Imu &imu)
{
    imu_angular_z_ = imu.angular_velocity.z;
}

void KalmanFilter::state_prediction()
{
    // Predict the state
    mean_ = mean_ + motion_;
    variance_ = variance_ + motion_variance_;
}

void KalmanFilter::measurement_update()
{
    // Kalman gain
    double kalman_gain = variance_ / (variance_ + measurement_variance_);

    // Update mean and variance
    mean_ = mean_ + kalman_gain * (imu_angular_z_ - mean_);
    variance_ = (1 - kalman_gain) * variance_;
}


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<KalmanFilter>("kalman_filter");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}