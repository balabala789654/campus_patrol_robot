from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():

    time_sync_ndoe = Node(
            package='time_sync',
            executable='time_sync',
            name='time_sync',
            output='screen',
        )
    
    ld = LaunchDescription()
    ld.add_action(time_sync_ndoe)
    return ld