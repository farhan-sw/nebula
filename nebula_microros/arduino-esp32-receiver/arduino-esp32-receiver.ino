#include <ACAN_ESP32.h>
#include <esp_chip_info.h>
#include <esp_flash.h>
#include <core_version.h> // For ARDUINO_ESP32_RELEASE

static const uint32_t DESIRED_BIT_RATE = 1000UL * 1000UL ; // 1 Mb/s

#define RX GPIO_NUM_16
#define TX GPIO_NUM_17
#define BAUDRATE_SERIAL 115200

/**
 * For Debugging purposes
 */
bool isDebugReceive = false;
static uint32_t lastReceive = 0 ;
#define DEBUG_RECEIVE_INTERVAL 100

void setup_CAN() {
  pinMode (LED_BUILTIN, OUTPUT);
  digitalWrite (LED_BUILTIN, HIGH);
  Serial2.begin (BAUDRATE_SERIAL, SERIAL_8N1, RX, TX); // Set RX2 to GPIO 16 and TX2 to GPIO 17
  delay(100);
  
  // Configure ESP32 CAN
  Serial2.println ("Configure ESP32 CAN") ;
  ACAN_ESP32_Settings settings (DESIRED_BIT_RATE) ;
  settings.mRequestedCANMode = ACAN_ESP32_Settings::LoopBackMode ;
  // settings.mRequestedCANMode = ACAN_ESP32_Settings::NormalMode ;
  settings.mRxPin = GPIO_NUM_21 ; // Optional, default Tx pin is GPIO_NUM_4
  settings.mTxPin = GPIO_NUM_22 ; // Optional, default Rx pin is GPIO_NUM_5
  const uint32_t errorCode = ACAN_ESP32::can.begin (settings) ;
  if (errorCode == 0) {
    Serial2.print ("Bit Rate prescaler: ") ;
    Serial2.println (settings.mBitRatePrescaler) ;
    Serial2.print ("Time Segment 1:     ") ;
    Serial2.println (settings.mTimeSegment1) ;
    Serial2.print ("Time Segment 2:     ") ;
    Serial2.println (settings.mTimeSegment2) ;
    Serial2.print ("RJW:                ") ;
    Serial2.println (settings.mRJW) ;
    Serial2.print ("Triple Sampling:    ") ;
    Serial2.println (settings.mTripleSampling ? "yes" : "no") ;
    Serial2.print ("Actual bit rate:    ") ;
    Serial2.print (settings.actualBitRate ()) ;
    Serial2.println (" bit/s") ;
    Serial2.print ("Exact bit rate ?    ") ;
    Serial2.println (settings.exactBitRate () ? "yes" : "no") ;
    Serial2.print ("Distance            ") ;
    Serial2.print (settings.ppmFromDesiredBitRate ()) ;
    Serial2.println (" ppm") ;
    Serial2.print ("Sample point:       ") ;
    Serial2.print (settings.samplePointFromBitStart ()) ;
    Serial2.println ("%") ;
    Serial2.println ("Configuration OK!");
  } else {
    Serial2.print ("Configuration error 0x") ;
    Serial2.println (errorCode, HEX) ;
  }
}

void send_CAN_message(int16_t linear_x, int16_t linear_y, int16_t angular_z) {
  CANMessage frame;
  frame.id = 0x123; // Example ID
  frame.len = 6; // 2 bytes for each value
  
  frame.data[0] = linear_x & 0xFF; // LSB of linear_x
  frame.data[1] = (linear_x >> 8) & 0xFF; // MSB of linear_x
  frame.data[2] = linear_y & 0xFF; // LSB of linear_y
  frame.data[3] = (linear_y >> 8) & 0xFF; // MSB of linear_y
  frame.data[4] = angular_z & 0xFF; // LSB of angular_z
  frame.data[5] = (angular_z >> 8) & 0xFF; // MSB of angular_z

  const bool ok = ACAN_ESP32::can.tryToSend(frame);
}

void receive_CAN_message() {
    CANMessage frame;
    while (ACAN_ESP32::can.receive(frame)) {
        int16_t linear_x = frame.data[1] << 8 | frame.data[0];
        int16_t linear_y = frame.data[3] << 8 | frame.data[2];
        int16_t angular_z = frame.data[5] << 8 | frame.data[4];
        
        Serial2.print("Received: ");
        Serial2.print("linear_x: ");
        Serial2.print(linear_x);
        Serial2.print(", linear_y: ");
        Serial2.print(linear_y);
        Serial2.print(", angular_z: ");
        Serial2.println(angular_z);
    }
}


// ----------------------------------------------------------------------------------------

#include <micro_ros_arduino.h>
#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <geometry_msgs/msg/twist.h>
#include <rmw_microros/rmw_microros.h>

#define LED_PIN 2
#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){return false;}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define EXECUTE_EVERY_N_MS(MS, X)  do { \
  static volatile int64_t init = -1; \
  if (init == -1) { init = uxr_millis();} \
  if (uxr_millis() - init > MS) { X; init = uxr_millis();} \
} while (0)\

rcl_subscription_t subscriber;
geometry_msgs__msg__Twist msg;
rclc_executor_t executor;
rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;

enum states {
  WAITING_AGENT,
  AGENT_AVAILABLE,
  AGENT_CONNECTED,
  AGENT_DISCONNECTED
} state;

void error_loop() {
  while(1) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(100);
  }
}

void subscription_callback(const void *msgin) {
  const geometry_msgs__msg__Twist * msg = (const geometry_msgs__msg__Twist *)msgin;
  int16_t linear_x = (int16_t)(msg->linear.x * 100);
  int16_t linear_y = (int16_t)(msg->linear.y * 100);
  int16_t angular_z = (int16_t)(msg->angular.z * 100);
  
  send_CAN_message(linear_x, linear_y, angular_z);
}

bool create_entities() {
  allocator = rcl_get_default_allocator();

  // Create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // Create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_arduino_node", "", &support));

  // QoS configuration
  rmw_qos_profile_t qos_profile = rmw_qos_profile_default;
  qos_profile.history = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
  qos_profile.depth = 10;
  qos_profile.reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE;

  // Create subscriber with QoS
  RCCHECK(rclc_subscription_init(
    &subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
    "cmd_vel_joy",
    &qos_profile));

  // Create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &msg, &subscription_callback, ON_NEW_DATA));

  return true;
}

void destroy_entities() {
  rmw_context_t * rmw_context = rcl_context_get_rmw_context(&support.context);
  (void) rmw_uros_set_context_entity_destroy_session_timeout(rmw_context, 0);

  rcl_subscription_fini(&subscriber, &node);
  rclc_executor_fini(&executor);
  rcl_node_fini(&node);
  rclc_support_fini(&support);
}

void setup_microros() {
  set_microros_transports();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  
  state = WAITING_AGENT;
  delay(2000);
}

void loop_microros() {
  switch (state) {
    case WAITING_AGENT:
      EXECUTE_EVERY_N_MS(500, state = (RMW_RET_OK == rmw_uros_ping_agent(100, 1)) ? AGENT_AVAILABLE : WAITING_AGENT;);
      break;
    case AGENT_AVAILABLE:
      state = (true == create_entities()) ? AGENT_CONNECTED : WAITING_AGENT;
      if (state == WAITING_AGENT) {
        destroy_entities();
      };
      break;
    case AGENT_CONNECTED:
      EXECUTE_EVERY_N_MS(200, state = (RMW_RET_OK == rmw_uros_ping_agent(100, 1)) ? AGENT_CONNECTED : AGENT_DISCONNECTED;);
      if (state == AGENT_CONNECTED) {
        rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
      }
      break;
    case AGENT_DISCONNECTED:
      destroy_entities();
      state = WAITING_AGENT;
      break;
    default:
      break;
  }
}

// ----------------------------------------------------------------------------------------

void setup() {
  setup_microros();
  setup_CAN();
}

void loop() {
  loop_microros();

  isDebugReceive = false;
    if(lastReceive < millis()){
        lastReceive += DEBUG_RECEIVE_INTERVAL;
        digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
        if(isDebugReceive) {
        receive_CAN_message();
        }
    }

}
