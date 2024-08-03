from launch import LaunchDescription
import os
from os import pathsep

from ament_index_python.packages import get_package_share_directory, get_package_prefix
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, SetEnvironmentVariable, IncludeLaunchDescription
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration, Command
from launch_ros.parameter_descriptions import ParameterValue
import xacro
from launch.launch_description_sources import PythonLaunchDescriptionSource

def generate_launch_description():
    # Get the package directories
    axebot_description = get_package_share_directory('axebot_description')
    axebot_description_prefix = get_package_prefix('axebot_description')
    
    # Set GAZEBO_MODEL_PATH environment variable
    model_path = os.path.join(axebot_description, "models")
    model_path += pathsep + os.path.join(axebot_description_prefix, "share")
    env_var = SetEnvironmentVariable('GAZEBO_MODEL_PATH', model_path)
    
    # Declare model argument
    model_arg = DeclareLaunchArgument(
        name='model',
        default_value=os.path.join(axebot_description, "urdf", "axebot.urdf.xacro"),
        description="Absolute path to robot urdf file"
    )
    
    # Get robot description from xacro file
    robot_description_raw = ParameterValue(Command(["xacro ", LaunchConfiguration("model")]), value_type=str)

    # Start robot state publisher node
    robot_state_publisher_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description': robot_description_raw}],
    )
    
    # Start Gazebo server and client
    start_gazebo_server = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory('gazebo_ros'), 'launch', 'gzserver.launch.py')
        )
    )
    start_gazebo_client = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory('gazebo_ros'), 'launch', 'gzclient.launch.py')
        )
    )

    
    # Spawn the robot in Gazebo
    spawn_robot = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        arguments=["-entity", "axebot", "-topic", "robot_description"],
        output='screen'
    )
    
    return LaunchDescription([
        env_var,
        model_arg,
        robot_state_publisher_node,
        start_gazebo_server,
        start_gazebo_client,
        spawn_robot
    ])
