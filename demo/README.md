# Demo

Run autonomous mapping and navigation demo with ROSbot [Navigation2](https://navigation.ros.org/) and [Slam Toolbox](http://wiki.ros.org/slam_toolbox) in Docker.

There are two phases:

1. **Creating a map** - navigation and creating a map by using [slam-toolbox](https://github.com/SteveMacenski/slam_toolbox)
2. **Localization on an already created map** - Navigation based on created map - with [AMCL](https://navigation.ros.org/configuration/packages/configuring-amcl.html)

Both cases are presented in three setups:

1. In a Local Area Network (LAN) - the robot running navigation stack and PC / laptop running RViz are in the same Wi-Fi network.
2. Over the Internet (VPN) - the robot and the laptop can be in separate networks.
3. Gazebo simulation.

> **Prerequisites**
>
> Make sure you have [Docker and Docker-Compose](https://docs.docker.com/desktop/install/linux-install/) installed on your laptop.
>
> If you don't have, here's a quick summary for Ubuntu 20.04 (just click the `copy` button, and paste it to the Linux terminal):
>
> ```bash
> sudo apt-get update && sudo apt-get install -y ca-certificates curl gnupg lsb-release
> ```
>
> ```bash
> sudo mkdir -p /etc/apt/keyrings
> ```
>
> ```bash
> curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
> ```
>
> ```bash
> echo \
>  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu \
>  $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
> ```
>
> ```bash
> sudo apt-get update && sudo apt-get install -y docker-ce docker-ce-cli containerd.io docker-compose-plugin
> ```
>
> Add your user to docker group, so that you will be able to run docker commands without `sudo` (it needs reboot to take effect):
>
> ```bash
> sudo usermod -aG docker $USER
> ```

## Quick start (with a physical ROSbot XL)

### 1. Clone this repo **on your laptop**

```bash
git clone https://github.com/husarion/rosbot-xl-docker/
```

### 2. Prepare `demo/.env` file

```bash
cd rosbot-xl-docker/demo
cp .env.template .env
```

modify it if needed (see comments)

> **Warning**
>
> Make sure you have Docker Compose with at least `v2.3.0` version installed on your PC/laptop
>
> ```bash
> $ docker compose version
> Docker Compose version v2.3.3
> ```
>
> Inline comments in `.env` files are supported from this version

```bash
# ======================================================
# Serial baudrate for rplidar driver
# ======================================================

# RPLIDAR A2
# RPLIDAR_BAUDRATE=115200

# RPLIDAR A3 and RPLIDAR S1
RPLIDAR_BAUDRATE=256000

# ======================================================
# For simulation example you need to use simulation time
# ======================================================

# for a physical ROSbot
USE_SIM_TIME=False

# for a Gazebo simulation
# USE_SIM_TIME=True

# ======================================================
# Mecanum and navigation parameters
# ======================================================

MECANUM=True
# NAV2_PARAMS=nav2_params_mecanum.yaml
AMCL_PARAMS=amcl_params_mecanum.yaml

# MECANUM=False
# NAV2_PARAMS=nav2_params.yaml
# AMCL_PARAMS=amcl_params.yaml

NAV2_PARAMS=nav2_params_pure_pursuit.yaml
```

<!-- todo: can't be changed because of microros -->
If you have other ROS 2 devices running in your LAN network make sure to provide an unique `ROS_DOMAIN_ID` (the default value is `ROS_DOMAIN_ID=0`). Note that if you run the demo example in a **simulation** it is necessary to define the `USE_SIM_TIME` variable to `True`.

### 3. Sync your workspace with the ROSbot

> **Prerequisites**
>
> Install [unison](<https://en.wikipedia.org/wiki/Unison_(software)>), [inotify-tools](https://github.com/inotify-tools/inotify-tools/wiki) and [rsync](https://en.wikipedia.org/wiki/Rsync):
>
> ```bash
> sudo sudo apt-get update && sudo apt-get install -y unison inotify-tools rsync
> ```

In this project you will be asked to modify `.env` and `dds-config.xml` files. You need to have the same changes applied both on your robot and laptop/pc.

In the `demo/` folder, there is a script for auto-syncing of this repo with your ROSbot (so you can clone this repo only on your laptop).

If IP address of your robot in LAN is `10.5.10.64`, run (it uses `rsync` for synchronization):

```bash
rosbot-xl-docker/demo/sync_with_rosbot.sh 10.5.10.64
```

> **Tip no. 1** ????
>
> You can use Husarnet VPN hostname instead of IPv6 address

In order to allow changes on ROSbot to affect demo directory on your PC (for example for viewing saved map) use `--bidirectional` flag (it uses `unison` for that):

```bash
rosbot-xl-docker/demo/sync_with_rosbot.sh 10.5.10.64 --bidirectional
```

### 4. [Optional] VPN config

If in the next steps you want to run VPN configuration to make your system working **not only in LAN but also over the Internet** get your Husarnet Join Code and use it for connecting your ROSbot and laptop to the same Husarnet network.

[Husarnet P2P VPN](https://husarnet.com/) is already preinstalled on your ROSbot. You need to install it on your laptop/pc as well. **Steps on how to connect ROSbot and your laptop to the same Husarnet network are described [here](https://husarion.com/tutorials/howtostart/rosbot2r-quick-start/#remote-access-over-the-internet-vpn)**.

**Preparing `dds-config.xml` file:**

Default DDS discovery using multicasting doesn't work over VPN, therefore IPv6 addresses provided by Husarnet need to be applied to a peer list in a `dds-config.xml` file.

To do so, based on `dds-config.template.xml` file create the `dds-config.xml` file and paste Husarnet IPv6 address of your laptop and ROSbot here:

```xml
<initialPeersList>
    <locator>
        <udpv6>
            <address>replace-it-with-ipv6-addr-of-your-laptop</address>
        </udpv6>
    </locator>
    <locator>
        <udpv6>
            <address>replace-it-with-ipv6-addr-of-your-rosbot</address>
        </udpv6>
    </locator>
</initialPeersList>
```

> **Tip no. 2** ????
>
> There is a simple utility script to generate `dds-config.xml` file for you by just providing Husarnet hostnames of your devices as arguments (if these hostnames are `mylaptop` and `myrosbot`):
>
> ```bash
> ./dds-config-generate.sh mylaptop myrosbot
> ```

> **Tip no. 3** ????
>
> FastDDS version preinstalled in ROS 2 Humble allows you to use Husarnet hostnames instead of IPv6 addresses.

### 5. Create a map

> **Tip no. 4** ????
>
> **Enabling a display**
>
> In order to use GUI of applications running in containers (like rviz) run:
>
> ```bash
> xhost local:root
> ```
>
> on your PC, before starting the containers

Depending on the network configuration (LAN/VPN) execute the chosen pair of commands in the PC and ROSbot's terminal:

<table>

<!-- ------------------------------------------------- -->

<tr>

<th> </th> 
<th> ROSbot </th> 
<th> PC / laptop </th>

</tr>

<!-- ------------------------------------------------- -->

<tr>

<td>
<b>LAN ????</b>
</td>

<td>

```bash
docker compose \
-f compose.rosbot.hardware.yaml \
-f compose.rosbot.mapping.yaml \
-f compose.rosbot.lan.yaml \
up
```

</td>

<td>

```
docker compose \
-f compose.rviz.yaml \
-f compose.rviz.lan.yaml \
up
```

</td>

</tr>

<!-- ------------------------------------------------- -->

<tr>

<td> 
<b>VPN ????</b>
</td>

<td>

```bash
docker compose \
-f compose.rosbot.hardware.yaml \
-f compose.rosbot.mapping.yaml \
-f compose.rosbot.vpn.yaml \
up
```

</td>

<td>

```
docker compose \
-f compose.rviz.yaml \
-f compose.rviz.vpn.yaml \
up
```

</td>

</tr>

<!-- ------------------------------------------------- -->

</table>

After about 35 seconds, you should see the ROSbot model in the Rviz window:

![](./.docs/rviz_mapping.png)

Prepare map with Rviz2 using the **[2D Goal Pose]** button on the top bar.

After you create the map, open a new terminal on ROSbot, navigate to `demo/` folder and execute:

```bash
./map-save.sh
```

Your map has been saved in docker volume and is available in the `maps/` folder.

Mapping phase is completed, you can stop / remove all running containers on ROSbot.

### 6. Localization on an already created map

Depending on the network configuration (LAN/VPN) execute the chosen pair of commands in the PC or ROSbot's terminal:

<table>

<!-- ------------------------------------------------- -->

<tr>

<th> </th> 
<th> ROSbot </th> 
<th> PC / laptop </th>

</tr>

<!-- ------------------------------------------------- -->

<tr>

<td> 
<b>LAN ????</b>
</td>

<td>

```bash
docker compose \
-f compose.rosbot.hardware.yaml \
-f compose.rosbot.localization.yaml \
-f compose.rosbot.lan.yaml \
up
```

</td>

<td>

```
docker compose \
-f compose.rviz.yaml \
-f compose.rviz.lan.yaml \
up
```

</td>

</tr>

<!-- ------------------------------------------------- -->

<tr>

<td> 
<b>VPN ????</b>
</td>

<td>

```bash
docker compose \
-f compose.rosbot.hardware.yaml \
-f compose.rosbot.localization.yaml \
-f compose.rosbot.vpn.yaml \
up
```

</td>

<td>

```
docker compose \
-f compose.rviz.yaml \
-f compose.rviz.vpn.yaml \
up
```

</td>

</tr>

<!-- ------------------------------------------------- -->

</table>

After about 35 seconds, you should see the ROSbot model in the Rviz window in a random place on the map you have previously created:

![](./.docs/rviz_localization_start.png)

By using the **[2D Pose Estimate]** button manualy tell the ROSbot where on the existing map is its starting position:

![](./.docs/rviz_localization_pose_estimate.png)

and tell the ROSbot where to go autonomously with **[2D Goal Pose]** button.

## Other configurations

There are also other configurations of navigation parameters. To use them you have to edit your `.env` file

### Mecanum

Although mecanum wheels will work as well on default configs, you can use these ones to take advantage of additional degree of freedom. It is necessary to change two env variables - `AMCL_PARAMS` (`robot_model_type` switched to omnidirectional) and `NAV2_PARAMS`. In this demo `DWBLocalPlanner` is configured to move only in x and y position ignoring orientation.

### Pure pursuit

This config uses different controller - `RegulatedPurePursuitController` instead of `DWBLocalPlanner` used in default config. When compared to DWB movement should be smoother. To use it you have set `NAV2_PARAMS` to `nav2_params_pure_pursuit.yaml`.

## Quick start (simulation)

Running simulation is very similar to robot config, it only requires setting `USE_SIM_TIME` to `True` in your `.env` file.  

### 1. Clone this repo on your laptop

```bash
git clone https://github.com/husarion/rosbot-xl-docker/
```

### 2. Prepare `demo/.env` file

```bash
cd rosbot-xl-docker/demo
cp .env.template .env
```

And set `USE_SIM_TIME` variable to `True`.

### 3. Create a map

```bash
docker compose \
-f compose.rosbot.mapping.yaml \
-f compose.simulation.nvidia.yaml \
up
```

> **Tip no. 5** ????
>
> There is an alternative config `compose.simulation.yaml` to run on all GPUs, but please note that performance is worse, so if you have NVidia GPU, dedicated config is recommended.

Prepare a map with Rviz2 using the **[2D Goal Pose]** button on the top bar.

After you create the map, open a new terminal, navigate to the `demo/` folder and execute:

```bash
./map-save.sh
```

Your map has been saved in the docker volume and is available in the `maps/` folder.

The mapping phase is completed, you can stop/remove all running containers.

### 6. Localization on an already created map

```bash
docker compose \
-f compose.rosbot.localization.yaml \
-f compose.simulation.nvidia.yaml \
up
```

> **Tip no. 6** ????
>
> As in the previous step, use the `compose.simulation.yaml` config to run on other GPUs.

By using the **[2D Pose Estimate]** button manually tell the ROSbot where on the existing map is its starting position. Then you can use **[2D Goal Pose]** button to navigate ROSbot autonomously.