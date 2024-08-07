import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    # Mengatur path untuk hardware_interface
    hardwareinterface_path = os.path.join(
        get_package_share_directory("nebula_firmware"),
        "launch",
        "hardware_interface.launch.py"
    )
    hardwareinterface = IncludeLaunchDescription(
        launch_description_source=hardwareinterface_path
    )

    # Mengatur path untuk controller
    controller_path = os.path.join(
        get_package_share_directory("nebula_controller"),
        "launch",
        "controller.launch.py"
    )
    controller = IncludeLaunchDescription(
        launch_description_source=controller_path,
        launch_arguments={
            "use_simple_controller": LaunchConfiguration("use_simple_controller", default="False"),
            "use_alternate_omni_controller": LaunchConfiguration("use_alternate_omni_controller", default="False")
        }.items()
    )
    
    # Mengatur path untuk joystick_teleop
    joystick_path = os.path.join(
        get_package_share_directory("nebula_controller"),
        "launch",
        "joystick_teleop.launch.py"
    )
    joystick = IncludeLaunchDescription(
        launch_description_source=joystick_path
    )
    
    # Mengembalikan LaunchDescription dengan semua deskripsi peluncuran
    return LaunchDescription([
        hardwareinterface,
        controller,
        joystick,
    ])
