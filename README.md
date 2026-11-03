# anakin_sdk 0.1.0

ENV 与 MC 共用的 ROS 2 Humble 接口库，包含消息、服务和公共合同。SDK 没有常驻进程，不包含机器人或推理实现。

独立构建使用原生 CMake，无需 colcon workspace：

```sh
./build.sh --jobs 8
```

需要 Ubuntu 22.04 x86_64、ROS 2 Humble 开发依赖和 CMake 3.19+。可用 `ROS_PREFIX` 指定 ROS 安装，`ANAKIN_PYTHON` 指定带有 ROS 生成工具的 Python（默认 `/usr/bin/python3`）。`./build.sh --help` 列出参数。

产物仅进入本仓的 `build/anakin_sdk/`、`install/anakin_sdk/`、`log/`。消费者通过 `ANAKIN_SDK_PREFIX=/path/to/anakin_sdk/install/anakin_sdk` 选用此安装；版本必须为 0.1.0，实际提交必须与消费者的 `anakin_sdk.revision` 一致。

SDK 元数据在每次 CMake 配置时记录 Git HEAD。发布顺序是先完成并提交 SDK，再更新 ENV/MC 的锁并成套构建。不要将未提交的 SDK 差异视作新的可发布版本。

[接口合同](docs/contract.md) 规定名称、单位、事务、QoS、推理许可和实机权限边界。统一初始化与构建由 [AnakinC](https://github.com/zdYukino/AnakinC) 编排，各子仓仍可独立使用。
