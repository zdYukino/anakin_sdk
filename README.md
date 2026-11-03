# anakin_sdk 0.1.0

Public ament/rosidl contract shared by the independent `anakin_env` and `anakin_mc` repositories. Standard ROS messages stay in their official packages. This repository contains no vendor SDK, ONNX Runtime, actuator or inference implementation.

Build on GTR with ROS 2 Humble: `source /opt/ros/humble/setup.bash; colcon build --base-paths .`.
Consumers require exactly version 0.1.0 and compare the installed SDK Git revision with their `anakin_sdk.revision` lock.
See [contract](docs/contract.md) for names, units, transaction rules and QoS.


当前 C++ 运行合同、共同原始 SDK PD、持久等待和实机只读/异步权限见 [contract](docs/contract.md)。Env 默认 real、显式 `--sim`；MC、Env、官方模拟器分别由用户启动。操作说明见 [Env 独立运行说明](../anakin_env/docs/standalone-operation.md)。SDK metadata 记录 Git HEAD；未提交差异必须通过同一源树成套构建两个消费者，不能虚构锁版本。最终批准提交时先提交 SDK，再更新双方锁并重建。
