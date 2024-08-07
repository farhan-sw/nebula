from launch import LaunchDescription
from launch_ros.actions import Node

from launch.actions import DeclareLaunchArgument, GroupAction
from launch.substitutions import LaunchConfiguration
from launch.conditions import IfCondition, UnlessCondition
from launch.substitutions import AndSubstitution, NotSubstitution

def generate_launch_description():
    
    use_simple_controller_arg = DeclareLaunchArgument(
        'use_simple_controller',
        default_value='false',
        description='Use custom controller (true) or default controller (false)',
    )
    
    use_alternate_omni_controller_arg = DeclareLaunchArgument(
        'use_alternate_omni_controller',
        default_value='false',
        description='Use custom omnidirectional controller (true) or default omnidirectional controller (false)',
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
    
    use_alternate_omni_controller = LaunchConfiguration('use_alternate_omni_controller')
    use_simple_controller = LaunchConfiguration('use_simple_controller')
    wheel_radius = LaunchConfiguration('wheel_radius')
    wheel_separation = LaunchConfiguration('wheel_separation')
    
    joint_state_broadcaster_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_state_broadcaster", "--controller-manager", "/controller_manager"]
    )

    # Omni Controller
    omni_controller = GroupAction(
        condition=UnlessCondition(use_simple_controller),
        actions=[
            Node(
                package="controller_manager",
                executable="spawner",
                arguments=["omni_wheel_controller", 
                           "--controller-manager", 
                           "/controller_manager"],
                condition=UnlessCondition(use_alternate_omni_controller)
            ),
            
            Node(
                package="controller_manager",
                executable="spawner",
                arguments=[
                    "omnidirectional_controller",
                    "--controller-manager", 
                    "/controller_manager",
                ],
                condition=IfCondition(use_alternate_omni_controller)
            )
        ]
    )
   
    

    
    # Simple controller
    simple_controller = GroupAction(
        condition=IfCondition(use_simple_controller),
        actions=[
            Node(
                package="controller_manager",
                executable="spawner",
                arguments=["simple_velocity_controller", "--controller-manager", "/controller_manager"],
            ),
            Node(
                package="nebula_controller",
                executable="controller_3wd_omni_node",
                name="controller_3wd_omni_node",
                parameters=[
                    {"wheel_radius": wheel_radius},
                    {"wheel_separation": wheel_separation},
                ],
            ),
            
        ]
    )
    
    
    return LaunchDescription([
        use_alternate_omni_controller_arg,
        use_simple_controller_arg,
        
        wheel_radius_arg,
        wheel_separation_arg,
        
        joint_state_broadcaster_spawner,
        
        simple_controller,
        omni_controller,
        
        
    ])
