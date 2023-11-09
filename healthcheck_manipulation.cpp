#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "control_msgs/msg/dynamic_joint_state.hpp"
#include <iostream>
#include <vector>

using namespace std::chrono_literals;

#define TIMEOUT 2s

bool odom_msg_received = false;
bool joint_msg_ok = false;

void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg)
{
  std::cout << "Odom message received" << std::endl;
  odom_msg_received = true;
  if (odom_msg_received && joint_msg_ok)
  {
    rclcpp::shutdown();
  }
}

void joint_callback(const control_msgs::msg::DynamicJointState::SharedPtr msg)
{
  std::cout << "Joint message received: ";

  std::vector<std::string> expected_joint_names = {
    "fl_wheel_joint", "fr_wheel_joint", "rl_wheel_joint", "rr_wheel_joint", "gripper_left_joint",
    "joint1",         "joint2",         "joint3",         "joint4",         "imu",
  };

  for (const auto& joint_name : msg->joint_names)
  {
    if (std::find(expected_joint_names.begin(), expected_joint_names.end(), joint_name) == expected_joint_names.end())
    {
      std::cout << "Failed" << std::endl;
      return;
    }
  }

  std::cout << "Success" << std::endl;
  joint_msg_ok = true;
  if (odom_msg_received && joint_msg_ok)
  {
    rclcpp::shutdown();
  }
}

void timeout_callback()
{
  std::cout << "Timeout" << std::endl;
  rclcpp::shutdown();
}

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>("healthcheck_node");
  auto odom_sub =
      node->create_subscription<nav_msgs::msg::Odometry>("/odometry/filtered", rclcpp::SensorDataQoS(), odom_callback);
  auto joint_sub = node->create_subscription<control_msgs::msg::DynamicJointState>(
      "/dynamic_joint_states", rclcpp::SystemDefaultsQoS(), joint_callback);
  auto timer = node->create_wall_timer(TIMEOUT, timeout_callback);

  rclcpp::spin(node);
  return (odom_msg_received && joint_msg_ok) ? EXIT_SUCCESS : EXIT_FAILURE;
}
