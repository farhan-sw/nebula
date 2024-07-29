from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource, \
    FrontendLaunchDescriptionSource
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    # Path ke launch file pertama
    launch_file_dir_1 = os.path.join(
        get_package_share_directory('nebula_bringup'), 'launch', 'joy_can_base.launch.py'
    )
    
    # Path ke launch file kedua
    launch_file_dir_2 = os.path.join(
        get_package_share_directory('ros2_socketcan'), 'launch', 'socket_can_bridge.launch.xml'
    )
    
    return LaunchDescription([
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(launch_file_dir_1)
        ),
        IncludeLaunchDescription(
            FrontendLaunchDescriptionSource(launch_file_dir_2)
        ),
    ])
