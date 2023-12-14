#include "control_msgs/msg/dynamic_joint_state.hpp"
#include "fstream"
#include "nav_msgs/msg/odometry.hpp"
#include "rclcpp/rclcpp.hpp"
#include "vector"

using namespace std::chrono_literals;

#define LOOP_PERIOD 500ms
#define MSG_VALID_TIME 2s

std::chrono::steady_clock::time_point last_odom_msg_time;
std::chrono::steady_clock::time_point last_joint_msg_time;

void write_health_status(const std::string &status) {
  std::ofstream healthFile("/var/tmp/health_status.txt");
  healthFile << status;
}

void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg) {
  last_odom_msg_time = std::chrono::steady_clock::now();
}

void joint_callback(const control_msgs::msg::DynamicJointState::SharedPtr msg) {

  std::vector<std::string> expected_joint_names = {
      "fl_wheel_joint",
      "fr_wheel_joint",
      "rl_wheel_joint",
      "rr_wheel_joint",
      "gripper_left_joint",
      "joint1",
      "joint2",
      "joint3",
      "joint4",
      "imu",
  };

  for (const auto &joint_name : msg->joint_names) {
    if (std::find(expected_joint_names.begin(), expected_joint_names.end(),
                  joint_name) == expected_joint_names.end()) {
      return;
    }
  }

  last_joint_msg_time = std::chrono::steady_clock::now();
}

void healthy_check() {
  std::chrono::steady_clock::time_point current_time =
      std::chrono::steady_clock::now();
  std::chrono::duration<double> odom_elapsed_time =
      current_time - last_odom_msg_time;
  std::chrono::duration<double> joint_elapsed_time =
      current_time - last_joint_msg_time;
  bool is_odom_valid = odom_elapsed_time < MSG_VALID_TIME;
  bool are_joints_valid = joint_elapsed_time < MSG_VALID_TIME;

  if (is_odom_valid && are_joints_valid) {
    write_health_status("healthy");
  } else {
    write_health_status("unhealthy");
  }
}

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("healthcheck_manipulation");
  auto odom_sub = node->create_subscription<nav_msgs::msg::Odometry>(
      "odometry/filtered", rclcpp::SensorDataQoS().keep_last(1), odom_callback);
  auto joint_sub =
      node->create_subscription<control_msgs::msg::DynamicJointState>(
          "dynamic_joint_states", rclcpp::SystemDefaultsQoS().keep_last(1), joint_callback);

  while (rclcpp::ok()) {
    rclcpp::spin_some(node);
    healthy_check();
    std::this_thread::sleep_for(LOOP_PERIOD);
  }

  return 0;
}
