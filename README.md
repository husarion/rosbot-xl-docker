# rosbot-xl-docker

Docker images for ROSbot XL

<!-- Docker Image for ROS Melodic Node providing interface for STM32 firmware over ROS-serial.

`rosbot-xl-docker` contain following ROS packages:

- [rosbot_ros](https://github.com/husarion/rosbot_ros)
- [rosbot_ekf](https://github.com/husarion/rosbot_ekf)

With _docker-compose_ configuration shown in [demo](./demo) it can communicate with hardware of both Rosbot 2.0 and Rosbot 2.0 Pro. -->

## Flashing the firmware

<!-- Firmware if flashed from inside of the container. In order to use specific kinematics flash matching firmware.

### Differential kinematics (normal wheels)

```bash
docker run --rm -it --privileged \
husarion/rosbot:noetic \
/flash-firmware.py /root/firmware_diff.bin
```

### Mecanum kinematics

```bash
docker run --rm -it --privileged \
husarion/rosbot:noetic \
/flash-firmware.py /root/firmware_mecanum.bin
``` -->

## Building locally

<!-- ```bash
docker buildx build \
--platform linux/amd64 \
-t rosbot-docker-test \
.
``` -->

<!-- ## ROS node

Most important nodes published by this docker after launching [rosbot_docker.launch](https://github.com/husarion/rosbot_ros/blob/melodic/src/rosbot_bringup/launch/rosbot_docker.launch) are shown below. -->

### Subscribes

<!-- - `/cmd_vel` (_geometry_msgs/Twist_, **/serial_bridge**) -->

### Publishes

<!-- - `/tf` (_tf2_msgs/TFMessage_, **/rosbot_ekf**)
- `/tf_static` (_tf2_msgs/TFMessage_, **/imu_publisher**, **/laser_publisher**, **/camera_publisher**)
- `/odom` (_nav_msgs/Odometry_, **/rosbot_ekf**)
- `/imu` (_sensor_msgs/Imu_, **/serial_bridge**)
- `/battery` (_sensor_msgs/BatteryState_, **/serial_bridge**)
- `/range/fl` (_sensor_msgs/Range_, **/serial_bridge**)
- `/range/fr` (_sensor_msgs/Range_, **/serial_bridge**)
- `/range/rl` (_sensor_msgs/Range_, **/serial_bridge**)
- `/range/rr` (_sensor_msgs/Range_, **/serial_bridge**)

For more details on what is being published and subscribed by nodes running in this container please refer to launch file and packages:

- [rosbot_ros](https://github.com/husarion/rosbot_ros)
- [rosbot_ekf](https://github.com/husarion/rosbot_ekf)
- [rosbot-stm32-firmware](https://github.com/husarion/rosbot-stm32-firmware) -->

## Autonomous Navigation Demo

In a [/demo](/demo) folder your will find an example of how to use ROSbot docker image in a real autonomous navigation use case.

<!-- ![](demo/autonomous_navigation_mapping/.docs/rviz_mapping.png) -->
