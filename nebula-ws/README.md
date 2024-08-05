
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

### References
- [1] [Gamepad by piborg](https://github.com/piborg/Gamepad)