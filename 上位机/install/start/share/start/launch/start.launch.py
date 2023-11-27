import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

def generate_launch_description():
    livox_mid360_msg_launch = os.path.join(get_package_share_directory('livox_ros_driver2'), 'launch')
    imu_filter_launch = os.path.join(get_package_share_directory('imu_complementary_filter'), 'launch')
    fast_lio_launch = os.path.join(get_package_share_directory('fast_lio'), 'launch')
    livox_to_pointcloud_launch = os.path.join(get_package_share_directory('livox_to_pointcloud2'), 'launch')
    linefit_ground_segmentation_ros2_launch = os.path.join(get_package_share_directory('linefit_ground_segmentation_ros'), 'launch')
    pointcloud_to_laser_launch = os.path.join(get_package_share_directory('pointcloud_to_laserscan'), 'launch')

    rviz_config_dir = os.path.join(
        get_package_share_directory('xiao_ying_navigation'),
        'rviz',
        'show.rviz')

    return LaunchDescription([
        # IncludeLaunchDescription(
        #     PythonLaunchDescriptionSource([os.path.join(livox_mid360_msg_launch, 'msg_MID360_launch.py')]),
        # ),
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource([os.path.join(imu_filter_launch, 'complementary_filter.launch.py')]),
        ),
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource([os.path.join(fast_lio_launch, 'mapping.launch.py')]),
        ),
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource([os.path.join(livox_to_pointcloud_launch, 'livox_to_pointcloud2.launch.py')]),
        ),
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource([os.path.join(linefit_ground_segmentation_ros2_launch, 'segmentation.launch.py')]),
        ),       
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource([os.path.join(pointcloud_to_laser_launch, 'sample_pointcloud_to_lasser_cartographer.launch.py')]),
        ),            
        # Node(
        #     package='rviz2',
        #     executable='rviz2',
        #     name='rviz2',
        #     arguments=['-d', rviz_config_dir],
        #     output='screen'),
    ])
