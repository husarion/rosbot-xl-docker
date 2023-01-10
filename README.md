# rosbot-xl-docker

Docker images for ROSbot XL

[![Build a Docker Image](https://github.com/husarion/rosbot-xl-docker/actions/workflows/build_push.yaml/badge.svg)](https://github.com/husarion/rosbot-xl-docker/actions/workflows/build_push.yaml)

Official ROSbot XL docker images built from this repo are available here: https://hub.docker.com/r/husarion/rosbot-xl/tags

- `husarion/rosbot-xl:humble` - the image for a real (physical) robot
<!-- - `husarion/rosbot-xl:humble-simulation` - the image with built-in Gazebo simulation model -->

## Flashing the firmware

Connect ROSbot to a Single Board Computer inside ROSbot XL by using a micro-USB cable. Check under which port the board is listed on your OS (in most cases it's `/dev/ttyUSB0`):

```bash
husarion@husarion:/$ ls -la /dev/ttyUSB*
crw-rw---- 1 root dialout 188, 0 Dec 20 17:57 /dev/ttyUSB0
```

```bash
docker run --rm -it --privileged \
husarion/rosbot-xl:humble \
flash-firmware.py -p /dev/ttyUSB0
```

Set dip switch no. 3 to **"off" state**  (`BOOT0` pin to LOW) and click the `RESET` button to start a newly flashed firmware.

## Building locally

```bash
docker buildx build \
-f Dockerfile.hardware \
--platform linux/amd64 \
-t rosbot-xl-docker-test \
.
```


## ROS API

Available in [rosbot_xl_ros/ROS_API.md](https://github.com/husarion/rosbot_xl_ros/blob/master/ROS_API.md)

## Autonomous Navigation Demo

In a [/demo](/demo) folder your will find an example of how to use ROSbot docker image in a real autonomous navigation use case.

<!-- ![](demo/.docs/rviz_mapping.png) -->
