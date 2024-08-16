
## LOG
https://docs.ros.org/en/humble/p/

### 2024-Jul-18
- Install gamepad library
```bash
sudo apt install libopencv-dev
sudo apt install joystick jstest-gtk evtest
sudo apt install ros-humble-ros2-socketcan
```

using reference [gamepad](https://articulatedrobotics.xyz/tutorials/mobile-robot/applications/teleop/)

### 2024-Agus-3
- Jika ada error CMake path dll, lakukan
```bash
echo $AMENT_PREFIX_PATH
echo $CMAKE_PREFIX_PATH
unset AMENT_PREFIX_PATH
unset CMAKE_PREFIX_PATH
```

- Jika error gazebo, lakukan
```bash
killall gzserver
killall gzclient
```
atau edit bashrc
```bash
. /usr/share/gazebo/setup.sh
```

### 2024-Agus-9
- Lokalisasi dan nav2
```bash
ros2 launch nav2_bringup  bringup_launch.py use_sim_time:=True map:=room.yaml 
```
ubah lokasi room.yaml sesuai dengan lokasi file map

- Error topic map gabisa muncul di rviz
-- ubah dulu ke fixed frame /map, kemudian baru add map, ubah ke volatile

### 2024-Agus-10
- Instalasi pointcloud to laser scan
```bash
sudo apt install ros-humble-pointcloud-to-laserscan
ros2 run pointcloud_to_laserscan pointcloud_to_laserscan_node --ros-args --params-file /home/farhan-sw/Documents/Github/nebula/nebula-ws/src/nebula_navigation/config/pointcloud_to_laserscan_params.yaml -r /cloud_in:=/camera/points -r /scan:=/scanner/scan
```

### 2024-Agus-11
- Kinect cuman lewat depth image?
```bash
sudo apt install ros-humble-depthimage-to-laserscan
```
- Fuse IMU sensor
```bash
imu_filter_madgwick
```
https://wiki.ros.org/imu_filter_madgwick

- ROS2 Kinct driver
https://github.com/fadlio/kinect_ros2

### 2024-Agus-14
- Install ROS2 CAN BUS
```bash
sudo apt install ros-humble-ros2-socketcan
```
- Setup Depth Image to Laser Scan
```bash
sudo apt install ros-humble-depthimage-to-laserscan
ros2 run depthimage_to_laserscan depthimage_to_laserscan_node --ros-args --params-file ~/Documents/Github/nebula/nebula-ws/src/nebula_navigation/config/depthimage_to_laserscan_config.yaml   -r depth:=/camera/depth/image_raw   -r depth_camera_info:=/camera/depth/camera_info   -r scan:=/scan
```

### 2024-Agus-16
- ERROR GAZEBO LAMA, ingat
```bash
sudo apt-get install ros-humble-gazebo-ros2-control
```

- Pastiitn semua nav2 sudah terinstall
```bash
sudo apt install ros-humble-navigation2 ros-humble-nav2-bringup
```

- Tambahan turtlebot3
```bash
sudo apt install ros-humble-turtlebot3*
```

- Plotting gunakan plotjuggler
```bash
sudo apt install ros-humble-plotjuggler
sudo apt install ros-humble-plotjuggler-ros
```

### References

#### BASE
- [1] [Gamepad by piborg](https://github.com/piborg/Gamepad)
- [2] [Omni Controller (3wd didnt work)](https://github.com/hijimasa/omni_wheel_controller)
- [3] [Omni Controller (3wd work)](https://github.com/mateusmenezes95/omnidirectional_controllers)
- [4] [Axebot - 3wd](https://github.com/mateusmenezes95/axebot)

#### NAVIGATION & LOCALIZATION
- [1] [Pointcloud to Laser Scan](https://wiki.ros.org/pointcloud_to_laserscan)
- [2] [twist_mux]()
- [3] [Depth Image to Laser Scan](https://github.com/ros-perception/depthimage_to_laserscan)

#### HARDWARE
- [1] [CAN BUS, Ros2 sockerCAN]