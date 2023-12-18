#include "time_sync/time_sync.hpp"

Time_sync::Time_sync() : Node("Time_sync"){
    cur_time = Time_sync::get_clock()->now();

    Time_ = create_wall_timer(std::chrono::milliseconds(100), std::bind(&Time_sync::topic_pub_callback, this));
    imu_sub = create_subscription<sensor_msgs::msg::Imu>("/livox/Imu", 10, std::bind(&Time_sync::Imu_sub_callback, this, std::placeholders::_1));
    lidar_sub = create_subscription<livox_interfaces::msg::CustomMsg>("/livox/lidar", 10, std::bind(&Time_sync::lidar_sub_callback, this, std::placeholders::_1));
    imu_pub = create_publisher<sensor_msgs::msg::Imu>("/sync/Imu", 10);
    lidar_pub = create_publisher<livox_interfaces::msg::CustomMsg>("/sync/Lidar", 10);
    RCLCPP_INFO(this->get_logger(), "working");
}
Time_sync::~Time_sync(){

}
void Time_sync::topic_pub_callback(){
    Imu_msg.header.stamp.nanosec = cur_time.nanoseconds();
    Imu_msg.header.stamp.sec = cur_time.seconds();
    lidar_msg.header.stamp.nanosec = cur_time.nanoseconds();
    lidar_msg.header.stamp.sec = cur_time.seconds();

    imu_pub->publish(Imu_msg);
    lidar_pub->publish(lidar_msg);
}   
void Time_sync::Imu_sub_callback(const sensor_msgs::msg::Imu::SharedPtr _msg){
    Imu_msg.angular_velocity.x = _msg->angular_velocity.x;
    Imu_msg.angular_velocity.y = _msg->angular_velocity.y;
    Imu_msg.angular_velocity.z = _msg->angular_velocity.z;
    Imu_msg.angular_velocity_covariance = _msg->angular_velocity_covariance;
    Imu_msg.header.frame_id = _msg->header.frame_id;
    Imu_msg.header.stamp.nanosec = _msg->header.stamp.nanosec;
    Imu_msg.header.stamp.sec = _msg->header.stamp.sec;
    Imu_msg.linear_acceleration.x = _msg->linear_acceleration.x;
    Imu_msg.linear_acceleration.y = _msg->linear_acceleration.y;
    Imu_msg.linear_acceleration.z = _msg->linear_acceleration.z;
    Imu_msg.linear_acceleration_covariance = _msg->linear_acceleration_covariance;
    Imu_msg.orientation.x = _msg->orientation.x;
    Imu_msg.orientation.y = _msg->orientation.y;
    Imu_msg.orientation.z = _msg->orientation.z;
    Imu_msg.orientation.w = _msg->orientation.w;
    Imu_msg.orientation_covariance = _msg->orientation_covariance;
}

void Time_sync::lidar_sub_callback(const livox_interfaces::msg::CustomMsg::SharedPtr _limsg){
    lidar_msg.header.frame_id = _limsg->header.frame_id;
    lidar_msg.header.stamp.nanosec = _limsg->header.stamp.nanosec;
    lidar_msg.header.stamp.sec = _limsg->header.stamp.sec;
    lidar_msg.lidar_id = _limsg->lidar_id;
    lidar_msg.point_num = _limsg->point_num;
    lidar_msg.points = _limsg->points;
    lidar_msg.rsvd = _limsg->rsvd;
    lidar_msg.timebase = _limsg->timebase;
}

sensor_msgs::msg::Imu Time_sync::Imu_msg_cpy(sensor_msgs::msg::Imu _msg){
    sensor_msgs::msg::Imu _Imu;

    _Imu.header.frame_id = _msg.header.frame_id;
    _Imu.linear_acceleration.x = _msg.linear_acceleration.x;
    _Imu.linear_acceleration.y = _msg.linear_acceleration.y;
    _Imu.linear_acceleration.z = _msg.linear_acceleration.z;
    _Imu.linear_acceleration_covariance = _msg.linear_acceleration_covariance;
    _Imu.orientation.x = _msg.orientation.x;
    _Imu.orientation.y = _msg.orientation.y;
    _Imu.orientation.z = _msg.orientation.z;
    _Imu.orientation.w = _msg.orientation.w;
    _Imu.orientation_covariance = _msg.orientation_covariance;    
    _Imu.angular_velocity.x = _msg.angular_velocity.x;
    _Imu.angular_velocity.y = _msg.angular_velocity.y;
    _Imu.angular_velocity.z = _msg.angular_velocity.z;
    _Imu.angular_velocity_covariance = _msg.angular_velocity_covariance;

    return _Imu;
}

livox_interfaces::msg::CustomMsg Time_sync::lidar_msg_cpy(livox_interfaces::msg::CustomMsg _msg){
    livox_interfaces::msg::CustomMsg _lidar;
    _lidar.header.frame_id = _msg.header.frame_id;
    _lidar.lidar_id = _msg.lidar_id;
    _lidar.point_num = _msg.point_num;
    _lidar.points = _msg.points;
    _lidar.rsvd = _msg.rsvd;
    _lidar.timebase = _msg.timebase;

    return _lidar;

}
int main(int argc, char const *argv[])
{
    /* code */
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Time_sync>());
    rclcpp::shutdown();
    return 0;
}