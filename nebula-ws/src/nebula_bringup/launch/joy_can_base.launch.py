from launch import LaunchDescription
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration
from launch.actions import DeclareLaunchArgument

import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    config_dir = get_package_share_directory('nebula_hardware_interfaces')
    params_file = os.path.join(config_dir, 'config', 'joy_can_base.yaml')
    
    # PRint params_file
    print(params_file)
    
    canjoy_node = Node(
        package='nebula_hardware_interfaces',
        executable='joystick_can_base',  # Ganti dengan nama executable Anda
        name='CANJoyNode',
        parameters=[params_file],
        output='screen'
    )
    
    return LaunchDescription([
        canjoy_node,
    ])

if __name__ == '__main__':
    generate_launch_description()
