from launch import LaunchDescription

from launch_ros.actions import Node

import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    
    static_transform_publisher = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        arguments=["0", "0", "0.07", "0", "0", "0", "1", "base_footprint_ekf", "imu_link_ekf"],
    )
    
    robot_localization = Node(
        package='robot_localization',
        executable='ekf_node',
        name='ekf_filter_node',
        output='screen',
        parameters=[os.path.join(get_package_share_directory('nebula_localization'), 'config', 'ekf.yaml')],
    )
    
    imu_republisher = Node(
        package='nebula_localization',
        executable='imu_republisher',
        name='imu_republisher',
        output='screen',
        remappings=[('imu_republisher/imu', 'imu_ekf'), ('imu_republisher/imu_raw', '/imu/out')],
    )
    
    return LaunchDescription([
        static_transform_publisher,
        robot_localization,
        imu_republisher,
    ])