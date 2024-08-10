# SLAM PROCESS

1. Create Map
Make sure the robot is running
```bash
ros2 launch nav2_bringup  navigation_launch.py use_sim_time:=True
ros2 launch slam_toolbox online_sync_launch.py use_sim_time:=True
```


# ```bash
# ros2 launch nav2_bringup  navigation_launch.py use_sim_time:=True map:=