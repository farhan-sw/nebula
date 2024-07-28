#include <Arduino.h>
#include <micro_ros_platformio.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int32_multi_array.h>
#include <geometry_msgs/msg/twist.h>

#if !defined(MICRO_ROS_TRANSPORT_ARDUINO_SERIAL)
#error This example is only available for Arduino framework with serial transport.
#endif

rcl_publisher_t publisher;
rcl_subscription_t subscriber;
std_msgs__msg__Int32MultiArray msg;
geometry_msgs__msg__Twist recv_msg;

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rcl_timer_t timer;

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

// Error handle loop
void error_loop() {
  while (1) {
    delay(100);
  }
}

void timer_callback(rcl_timer_t *timer, int64_t last_call_time) {
  RCLC_UNUSED(last_call_time);
  if (timer != NULL) {
    RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
    for (size_t i = 0; i < msg.data.size; i++) {
      msg.data.data[i]++;
    }
  }
}

void subscription_callback(const void *msgin) {
  const geometry_msgs__msg__Twist *msg = (const geometry_msgs__msg__Twist *)msgin;
  recv_msg = *msg;
  // Handle received message here
  // For example, print the received values
  Serial.print("Received linear x: ");
  Serial.println(recv_msg.linear.x);
  Serial.print("Received angular z: ");
  Serial.println(recv_msg.angular.z);
}

void setup() {
  // Configure serial transport
  Serial.begin(230400); // Increase baud rate
  set_microros_serial_transports(Serial);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(2000);

  allocator = rcl_get_default_allocator();

  // Create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // Create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_platformio_node", "", &support));

  // Create publisher
  RCCHECK(rclc_publisher_init_default(
    &publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32MultiArray),
    "micro_ros_platformio_node_publisher"));

  // Create subscriber
  RCCHECK(rclc_subscription_init_default(
    &subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
    "cmd_vel_joy"));

  // Create timer
  const unsigned int timer_timeout = 50; // Increase timer interval to 50ms (20 Hz)
  RCCHECK(rclc_timer_init_default(
    &timer,
    &support,
    RCL_MS_TO_NS(timer_timeout),
    timer_callback));

  // Create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 2, &allocator));
  RCCHECK(rclc_executor_add_timer(&executor, &timer));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &recv_msg, &subscription_callback, ON_NEW_DATA));

  // Initialize Int32MultiArray message
  msg.data.data = (int32_t *)malloc(3 * sizeof(int32_t)); // Example size of 3
  msg.data.size = 3;
  msg.data.capacity = 3;
  msg.data.data[0] = 0;
  msg.data.data[1] = 1;
  msg.data.data[2] = 2;
}

void loop() {
  delay(100); // Reduce delay in the loop
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100))); // Reduce executor spin time

  // Process data received from subscriber
  // For example, if the received linear x is greater than 0.2 led will turn on otherwise off
  if (recv_msg.linear.x > 0.2) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
