#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <image_transport/image_transport.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

class CameraNode : public rclcpp::Node
{
public:
  CameraNode()
  : Node("camera_node")
  {
    this->declare_parameter<std::string>("camera_topic", "/camera");
    this->declare_parameter<int>("frame_width", 640);
    this->declare_parameter<int>("frame_height", 480);
    this->declare_parameter<int>("frame_rate", 30);
    this->declare_parameter<int>("camera_index", 0);

    std::string camera_topic;
    int frame_width, frame_height, frame_rate, camera_index;

    this->get_parameter("camera_topic", camera_topic);
    this->get_parameter("frame_width", frame_width);
    this->get_parameter("frame_height", frame_height);
    this->get_parameter("frame_rate", frame_rate);
    this->get_parameter("camera_index", camera_index);

    publisher_ = image_transport::create_publisher(this, camera_topic);

    cap_.open(camera_index);
    cap_.set(cv::CAP_PROP_FRAME_WIDTH, frame_width);
    cap_.set(cv::CAP_PROP_FRAME_HEIGHT, frame_height);
    cap_.set(cv::CAP_PROP_FPS, frame_rate);

    if (!cap_.isOpened()) {
      RCLCPP_ERROR(this->get_logger(), "Could not open video stream");
      rclcpp::shutdown();
    }

    timer_ = this->create_wall_timer(
      std::chrono::milliseconds(1000 / frame_rate),
      std::bind(&CameraNode::publish_frame, this)
    );
  }

private:
  void publish_frame()
  {
    cv::Mat frame;
    cap_ >> frame;

    if (frame.empty()) {
      RCLCPP_WARN(this->get_logger(), "Captured empty frame");
      return;
    }

    auto msg = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", frame).toImageMsg();
    publisher_.publish(*msg);
  }

  image_transport::Publisher publisher_;
  cv::VideoCapture cap_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CameraNode>());
  rclcpp::shutdown();
  return 0;
}
