#ifndef _TIME_SYNC_HPP
#define _TIME_SYNC_HPP

#include "rclcpp/rclcpp.hpp"
#include "rclcpp/time.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include <livox_interfaces/msg/custom_msg.hpp>

class Time_sync : public rclcpp::Node{
    public:
        Time_sync();
        ~Time_sync();
        void topic_pub_callback();
        void Imu_sub_callback(const sensor_msgs::msg::Imu::SharedPtr _msg);
        void lidar_pub_callback();
        void lidar_sub_callback(const livox_interfaces::msg::CustomMsg::SharedPtr _limsg);

        sensor_msgs::msg::Imu Imu_msg_cpy(sensor_msgs::msg::Imu _msg);
        livox_interfaces::msg::CustomMsg lidar_msg_cpy(livox_interfaces::msg::CustomMsg _msg);

    private:
        rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imu_pub;
        rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_sub;

        rclcpp::Publisher<livox_interfaces::msg::CustomMsg>::SharedPtr lidar_pub;
        rclcpp::Subscription<livox_interfaces::msg::CustomMsg>::SharedPtr lidar_sub;

        rclcpp::TimerBase::SharedPtr Time_;
        sensor_msgs::msg::Imu Imu_msg;
        livox_interfaces::msg::CustomMsg lidar_msg;
        rclcpp::Time cur_time;
        
};

#endif