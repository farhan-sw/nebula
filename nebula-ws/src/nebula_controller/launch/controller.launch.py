from launch import LaunchDescription
from launch_ros.actions import Node

from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch.conditions import IfCondition, UnlessCondition

def generate_launch_description():
    
    use_custom_controller_arg = DeclareLaunchArgument(
        'use_custom_controller',
        default_value='false',
        description='Use custom controller (true) or default controller (false)',
    )
    
    wheel_radius_arg = DeclareLaunchArgument(
        'wheel_radius',
        default_value='0.075',
        description='Radius of the wheel',
    )
    
    wheel_separation_arg = DeclareLaunchArgument(
        'wheel_separation',
        default_value='0.23',
        description='Separation between the wheels',
    )
    
    use_custom_controller = LaunchConfiguration('use_custom_controller')
    wheel_radius = LaunchConfiguration('wheel_radius')
    wheel_separation = LaunchConfiguration('wheel_separation')
    
    joint_state_broadcaster_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_state_broadcaster", "--controller-manager", "/controller_manager"]
    )

    omnidirectional_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["omni_wheel_controller", "--controller-manager", "/controller_manager"],
        condition=UnlessCondition(use_custom_controller)
    )
    
    simple_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["simple_velocity_controller", "--controller-manager", "/controller_manager"],
        condition=IfCondition(use_custom_controller)
    )
    
    simple_controller_cpp = Node(
        package="nebula_controller",
        executable="controller_3wd_omni_node",
        name="controller_3wd_omni_node",
        parameters=[
            {"wheel_radius": wheel_radius},
            {"wheel_separation": wheel_separation},
        ],
        condition=IfCondition(use_custom_controller)
    )
    
    return LaunchDescription([
        use_custom_controller_arg,
        wheel_radius_arg,
        wheel_separation_arg,
        joint_state_broadcaster_spawner,
        omnidirectional_controller_spawner,
        simple_controller_spawner,
        simple_controller_cpp,
    ])
