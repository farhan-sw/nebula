
## LOG

### 2024-Jul-18
- Install gamepad library
```bash
sudo apt install joystick jstest-gtk evtest
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

### References

#### BASE
- [1] [Gamepad by piborg](https://github.com/piborg/Gamepad)
- [2] [Omni Controller (3wd didnt work)](https://github.com/hijimasa/omni_wheel_controller)
- [3] [Omni Controller (3wd work)](https://github.com/mateusmenezes95/omnidirectional_controllers)
- [4] [Axebot - 3wd](https://github.com/mateusmenezes95/axebot)

#### NAVIGATION & LOCALIZATION