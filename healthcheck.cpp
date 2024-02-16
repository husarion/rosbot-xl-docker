#include "fstream"
#include "nav_msgs/msg/odometry.hpp"
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

#define HEALTHCHECK_PERIOD 500ms
#define MSG_VALID_TIME 2s

class HealthCheckNode : public rclcpp::Node {
public:
  HealthCheckNode()
      : Node("healthcheck_rosbot"),
        last_msg_time(std::chrono::steady_clock::now()) {

    // Set custom QoS settings to match the publisher
    auto qos = rclcpp::QoS(rclcpp::QoSInitialization(RMW_QOS_POLICY_HISTORY_KEEP_LAST, 10));
    qos.reliability(RMW_QOS_POLICY_RELIABILITY_RELIABLE);
    qos.durability(RMW_QOS_POLICY_DURABILITY_VOLATILE);

    subscription_ = create_subscription<nav_msgs::msg::Odometry>(
        "odometry/filtered", qos,
        std::bind(&HealthCheckNode::msgCallback, this, std::placeholders::_1));

    timer_ = create_wall_timer(HEALTHCHECK_PERIOD,
                               std::bind(&HealthCheckNode::healthyCheck, this));
  }

private:
  std::chrono::steady_clock::time_point last_msg_time;
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr subscription_;
  rclcpp::TimerBase::SharedPtr timer_;

  void writeHealthStatus(const std::string &status) {
    std::ofstream healthFile("/var/tmp/health_status.txt");
    healthFile << status;
  }

  void msgCallback(const nav_msgs::msg::Odometry::SharedPtr msg) {
    // RCLCPP_DEBUG(get_logger(), "Msg arrived");
    last_msg_time = std::chrono::steady_clock::now();
  }

  void healthyCheck() {
    auto current_time = std::chrono::steady_clock::now();
    auto elapsed_time = current_time - last_msg_time;
    bool is_msg_valid = elapsed_time < MSG_VALID_TIME;

    if (is_msg_valid) {
      writeHealthStatus("healthy");
    } else {
      writeHealthStatus("unhealthy");
    }
  }
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<HealthCheckNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
