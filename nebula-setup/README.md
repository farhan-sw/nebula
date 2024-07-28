# SETUP PROJECT NEBULA

[Table of Contents](#table-of-contents)
- [SETUP ROS2 DEPENDENCIES](#setup-ros2-dependencies)
- [Setup Micro-ROS on ESP32](#setup-micro-ros-on-esp32)
  - [Setup ROS2 Workspace](#setup-ros2-workspace)
  - [Setup micro-ROS on ESP32 using PlatformIO](#setup-micro-ros-on-esp32-using-platformio)
  - [Run micro-ROS on ROS2](#run-micro-ros-on-ros2)
- [Setup CAN Adapter](#setup-can-adapter)

## SETUP ROS2 DEPENDENCIES
[back to top](#table-of-contents)
1. Install ROS2 CanMsgs
```bash
sudo apt install ros-humble-can-msgs
```
2. Install ROS2 Joy
```bash
sudo apt install joystick jstest-gtk evtest
```
validate the installation by running the following command:
```bash
ros2 run joy joy_enumerate_devices
```

## SETUP MICRO-ROS ON ESP32
[back to top](#table-of-contents)
### Setup ROS2 Workspace
[Guide](https://www.hackster.io/514301/micro-ros-on-esp32-using-arduino-ide-1360ca)

To set up microROS, follow these steps:

1. Create a workspace and download the micro-ROS tools:
Make a dir different from nebula_ws:
```
cd
mkdir microros_ws
cd microros_ws
```

2. Clone the micro-ROS src file from GitHub:
```
git clone -b $ROS_DISTRO https://github.com/micro-ROS/micro_ros_setup.git src/micro_ros_setup
```

3. Install rosdep:
```
sudo apt install python3-rosdep2
```

4. Update dependencies using rosdep:
```
sudo apt update && rosdep update
rosdep install --from-paths src --ignore-src -y
```

5. Install pip:
```
sudo apt-get install python3-pip
```

6. Build micro-ROS tools and source them:
```
colcon build
source install/local_setup.bash
```

7. To install the micro-ROS Agent, run the following command:
```
ros2 run micro_ros_setup create_agent_ws.sh
```

8. Build the agent packages and source the installation:
```
ros2 run micro_ros_setup build_agent.sh
```

9. To use micro-ROS commands, run the following command in every new terminal:
```
source ~/microros_ws/install/local_setup.bash
```

To avoid running the source command every time you open a new terminal, you can add it to the bashrc file.

### Setup micro-ROS on ESP32 using PlatformIO

https://github.com/micro-ROS/micro_ros_platformio

1. Install Reqruiements:
```
sudo apt install -y git cmake python3-pip
```
2. Edit platformio.ini file:
```
[env:esp32doit-devkit-v1]
platform = espressif32
board = esp32doit-devkit-v1
framework = arduino
board_microros_distro = humble
board_microros_transport = serial
lib_deps =
    https://github.com/micro-ROS/micro_ros_platformio
```

### Run micro-ROS on ROS2
```bash
ros2 run micro_ros_agent micro_ros_agent serial --dev /dev/ttyUSB0
```

## SETUP CAN ADAPTER
[back to top](#table-of-contents)

### Install can-utils
```bash
sudo apt install can-utils
```

### Buat Skrip Dapat Dieksekusi:
```bash
chmod +x setup_can.sh
```

### Jalankan Skrip:
Jalankan skrip dengan perintah berikut:
```bash
./setup_can.sh
```
