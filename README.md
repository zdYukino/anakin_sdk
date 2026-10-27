# anakin_sdk 0.1.0

Public ament/rosidl contract shared by the independent `anakin_env` and `anakin_mc` repositories. Standard ROS messages stay in their official packages. This repository contains no vendor SDK, ONNX Runtime, actuator or inference implementation.

Build on GTR with ROS 2 Humble: `source /opt/ros/humble/setup.bash; colcon build --base-paths .`.
Consumers require exactly version 0.1.0 and compare the installed SDK Git revision with their `anakin_sdk.revision` lock.
See [contract](docs/contract.md) for names, units, transaction rules and QoS.
