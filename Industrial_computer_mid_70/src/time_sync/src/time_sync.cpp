#include "time_sync/time_sync.hpp"

Time_sync::Time_sync() : Node("Time_sync"){
    cur_time = Time_sync::get_clock()->now();

    Time_ = create_wall_timer(std::chrono::milliseconds(100), std::bind(&Time_sync::topic_pub_callback, this));
    imu_sub = create_subscription<sensor_msgs::msg::Imu>("/livox/imu", 10, std::bind(&Time_sync::Imu_sub_callback, this, std::placeholders::_1));
    lidar_sub = create_subscription<livox_interfaces::msg::CustomMsg>("/livox/lidar", 10, std::bind(&Time_sync::lidar_sub_callback, this, std::placeholders::_1));
    imu_pub = create_publisher<sensor_msgs::msg::Imu>("/sync/imu", 10);
    lidar_pub = create_publisher<livox_interfaces::msg::CustomMsg>("/sync/lidar", 10);
    RCLCPP_INFO(this->get_logger(), "working");
}
Time_sync::~Time_sync(){

}
void Time_sync::topic_pub_callback(){
    Imu_msg.header.stamp.nanosec = cur_time.nanoseconds();
    Imu_msg.header.stamp.sec = cur_time.seconds();
    lidar_msg.header.stamp.nanosec = Imu_msg.header.stamp.nanosec;
    lidar_msg.header.stamp.sec = Imu_msg.header.stamp.sec;

    imu_pub->publish(Imu_msg);
    lidar_pub->publish(lidar_msg);

    this->sync_msg_info(lidar_msg, lidar_msg.points[0]);
}   
void Time_sync::Imu_sub_callback(const sensor_msgs::msg::Imu::SharedPtr _msg){
    Imu_msg.set__angular_velocity(_msg->angular_velocity);
    Imu_msg.set__angular_velocity_covariance(_msg->angular_velocity_covariance);
    Imu_msg.set__header(_msg->header);
    Imu_msg.set__linear_acceleration(_msg->linear_acceleration);
    Imu_msg.set__linear_acceleration_covariance(_msg->linear_acceleration_covariance);
    Imu_msg.set__orientation(_msg->orientation);
    Imu_msg.set__orientation_covariance(_msg->orientation_covariance);
}

void Time_sync::lidar_sub_callback(const livox_interfaces::msg::CustomMsg::SharedPtr _limsg){
    lidar_msg.set__header(_limsg->header);
    lidar_msg.set__lidar_id(_limsg->lidar_id);
    lidar_msg.set__point_num(_limsg->point_num);
    lidar_msg.set__points(_limsg->points);
    lidar_msg.set__rsvd(_limsg->rsvd);
    lidar_msg.set__timebase(_limsg->timebase);
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
void Time_sync::sync_msg_info(livox_interfaces::msg::CustomMsg _limsg, livox_interfaces::msg::CustomPoint _msg){
    RCLCPP_DEBUG(this->get_logger(), "frame_id: %s", _limsg.header.frame_id.c_str());
    // RCLCPP_DEBUG(this->get_logger(), "nanosec: %s", _limsg.header.stamp.nanosec.c_str());
    // RCLCPP_DEBUG(this->get_logger(), "sec: %s", _limsg.header.stamp.sec.c_str());
    RCLCPP_DEBUG(this->get_logger(), "lidar_id: %d", _limsg.lidar_id);
    RCLCPP_DEBUG(this->get_logger(), "point_num: %d", _limsg.point_num);
    RCLCPP_DEBUG(this->get_logger(), "rsvd_0: %d", _limsg.rsvd[0]);
    RCLCPP_DEBUG(this->get_logger(), "rsvd_1: %d", _limsg.rsvd[1]);
    RCLCPP_DEBUG(this->get_logger(), "rsvd_2: %d", _limsg.rsvd[2]);
    RCLCPP_DEBUG(this->get_logger(), "timebase: %ld", _limsg.timebase);

    RCLCPP_DEBUG(this->get_logger(), "points_line: %d", _msg.line);
    RCLCPP_DEBUG(this->get_logger(), "points_offset_time: %d", _msg.offset_time);
    RCLCPP_DEBUG(this->get_logger(), "points_reflectovity: %d", _msg.reflectivity);
    RCLCPP_DEBUG(this->get_logger(), "points_tag: %d", _msg.tag);
    RCLCPP_DEBUG(this->get_logger(), "points_x: %f", _msg.x);
    RCLCPP_DEBUG(this->get_logger(), "points_y: %f", _msg.y);
    RCLCPP_DEBUG(this->get_logger(), "points_z: %f", _msg.y);
}
int main(int argc, char const *argv[])
{
    /* code */
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Time_sync>());
    rclcpp::shutdown();
    return 0;
}