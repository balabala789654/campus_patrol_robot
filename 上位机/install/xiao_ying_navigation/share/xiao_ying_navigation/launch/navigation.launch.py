import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

def generate_launch_description():

    xiao_ying_navigation = get_package_share_directory('xiao_ying_navigation')
    nav2_bringup_dir = get_package_share_directory('nav2_bringup')

    use_sim_time = LaunchConfiguration('use_sim_time', default='false')
    map_dir = LaunchConfiguration('map',default=os.path.join(xiao_ying_navigation,'map','map.yaml'))
    param_dir = LaunchConfiguration('params_file', default=os.path.join(xiao_ying_navigation, 'param', 'xiao_ying.yaml'))
    rviz_nav2_launch_file_dir = os.path.join(nav2_bringup_dir, 'rviz', 'nav2_default_view.rviz')

    return LaunchDescription([
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource([nav2_bringup_dir, '/launch', '/bringup_launch.py']),
            launch_arguments={
                'map': map_dir,
                'use_sim_time': use_sim_time,
                'params_file': param_dir}.items(),
        ),

        Node(
            package='rviz2',
            executable='rviz2',
            name='rviz2',
            arguments=['-d', rviz_nav2_launch_file_dir],
            parameters=[{'use_sim_time': use_sim_time}],
            output='screen'),
    ])
