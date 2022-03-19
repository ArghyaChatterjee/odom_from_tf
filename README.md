# odom_from_tf
This repository contains code which listens to tf tree and publish odometry message as a topic (useful for packages like cartographer ros). 

## Inner Working
This node will listen to the tf tree (parent frame `odom` and child frame `base_link`) published by any node and publish the transform between parent frame `odom` and child frame `base_link` as `nav_msgs::Odometry` message in `/odom` topic at a rate of 200 hz.

## Start the Odom from TF package

1. Clone the repo inside your catkin workspace. 
```
cd catkin_ws
https://github.com/ArghyaChatterjee/odom_from_tf.git
```
2. If you don't have necessary dependencies installed, Run: 
```
cd catkin_ws
rosdep install --from-paths src --ignore-src --rosdistro melodic -r -y
```
3. Compile the sub workspace of whole workspace 
```
catkin_make --only-pkg-with-deps odom_from_tf
```
4. Source your workspace 
```
source devel/setup.bash
```
5. Make sure that the `tf` publishing node is publishing the transform between desired frames (`/odom` to `/base_link`). 
6. Start the odometry listener node from tf tree:
```
roslaunch odom_from_tf publish_odom.launch
```

