# Contract v3

## Ownership and standard payloads

Only env requests ordinary inference switches, manages execution authorization, motor indexing, CRC, gains and limits. Only mc publishes final ControlTarget after model adapters and a named-joint mux. No software PD or duplicate action scaling occurs in env. Canonical order is defined by joint_names() and mirrors the 29-motor G1 HG order at pinned Unitree SDK2. Subsets and model orders must use joint names, including the six missing CR7 wrists. Units: rad, rad/s, m/s, rad/s and ROS xyzw orientation. Unitree wxyz conversion occurs only in env. Gyroscope is body-local. Frame IDs describe coordinate frames only.

RobotStateFrame embeds official sensor_msgs/JointState, Imu and Joy; geometry_msgs/TwistStamped; nav_msgs/Odometry; diagnostic_msgs/DiagnosticArray. ControlTarget embeds official trajectory_msgs/JointTrajectory. UUIDs and timestamps use official definitions. No standard messages are copied or renamed. Optional odometry/remote data retain original nested stamps and validity bits. The outer timestamp identifies the joint/IMU sample acquisition. No validity bit means unavailable, never an invented zero observation.

## Transaction

New env session and mc instance UUIDs are generated at process start. Startup is disarmed. Env discovers mc instance and owns an increasing epoch. Before sending E+1, env locally invalidates all E commands; a timeout cannot restore E. A requested fallback uses a new epoch. SwitchInferenceMode returns accepted when queued; it does not indicate target_ready, target_active or completion. Same session/request UUID with exactly the same fields is idempotent and returns its recorded result. Changing any field under that ID is ID_CONFLICT. Retry the identical request after a timeout; never silently allocate a fresh ID. Invalid requests are recorded too. A bounded transaction journal must refuse new requests on exhaustion rather than evict IDs and accept conflicting reuse within a session.

MC switches at a frame boundary using a state from the requested session/epoch. Target context resets independently; history warmup cannot mutate source context. A profile may allow fresh source-loco continuation in E+1. This must run inference on new E+1 states, never relabel a cached target. target_ready, source_continuation, transition_entry and target_active are independent facts. Mux works on canonical absolute positions and explicit named masks; it never blends heterogeneous raw actions. Motion completion is distinct from transition completion.

Each transaction retains cumulative first/last sequence per phase and a published mask. MC publishes TARGET_ACTIVE first, then marks status target_active and its sequence. Env may receive only the latest active sample and acknowledge it using target_seq_seen/phase_mask_seen. Skipping the first sample cannot deadlock completion. Status publication and env receipt/validation do not prove physical execution. A restarted mc changes instance, invalidates commands, and requires explicit new authorization.

## Timing and QoS

Default design targets are mc 50 Hz and env 500 Hz, pending measured validation. Planned tick deadlines govern work; skip missed periods instead of bursting to catch up. ROS timestamps and local monotonic safety deadlines remain separate. `use_sim_time` stays false without an official /clock publisher.

Target/state latest data use keep-last depth 1, best effort and volatile durability. Status transactions and services use reliable volatile delivery with bounded depth. Reliability does not imply timely delivery. The target TTL is measured from the source-state acquisition recorded locally by env; receipt, retry and DDS redelivery cannot extend it. Env validates source sequence, age, session, mc instance, epoch, request, schema, model, gain profile and finite SDK-representable references before execution; legacy mechanical profiles additionally enforce their position/continuity rules. No infinite hold on invalid/expired targets. Stop is local and does not wait for mc ACK. Device watchdog and physical remote-loss semantics remain unverified; this contract is not a physical-safety certification.

## Initial scope

G1 ASAP loco actor_obs[1,500] -> action[1,12], independent four-frame grouped history; ASAP CR7 actor_obs[1,380] -> action[1,23]. MC clips raw action at +/-100, scales once by 0.25, adds model defaults and maps once to canonical29. CR7's independently recorded Python failures remain failures until revalidated. Other ONNX policies, TorchScript, hands, FK and physical execution are outside the first implementation milestone.

## Common native PD execution

The default native contract is `g1_sdk_pd_v1` / `PD_REFERENCE`, negotiated identically in state, switch request/response, inference status and target. Both sim and real use the same HG SDK q, dq, Kp, Kd, tau, PR mode, learned machine mode, canonical motor order and CRC. The reference is the original model's final startup/transition mux value. Env does not project it into mechanical position bounds, add compensation, rescale actions or select a different numerical algorithm for simulation. Nonfinite values and values that cannot narrow to finite SDK floats are rejected. Measured-state validity and configurable actuation guards are separate from reference encoding.

The shared gain tables retain their historical names: `g1_asap_sim_v1` is the original frozen table; `g1_asap_merged_sim_v1` preserves the original Python environment merge. Loco overrides its twelve action joints; CR7 overrides twenty-three joints and retains the base gains at its six missing wrists. The actual producing model selects its gains during handover. No gain numbers or policy/transition math changed with the common profile. Historical names do not certify these gains for hardware.

CRC uses explicit HG words with zero padding. Env normalizes padding and CRC on the final command object after copies, immediately before passing it to the SDK writer. Unused motor slots remain disabled. Release writes a finite zero-gain/zero-feed-forward packet before publication stops, clearing a simulator's cached actuation command; it is conditional on prior local writes and ownership, so readonly real startup sends no first release packet. A writer receipt proves publication only, not motor execution.

## Persistent waiting and inference permission

Env, MC and simulator remain independent persistent processes. There is no timed application exit, simulator launch/reset command, reset notification service or SDK-tick-triggered policy initialization. Duplicate SDK ticks do not renew physical acquisition time/generation. A backwards sim tick increments diagnostics while Env state sequences continue independently; session, epoch, selected mode and model histories are unchanged.

Env publishes a state sequence only for a new validated physical acquisition. It does not repaint a cached physical sample with a new sequence or timestamp to drive inference. Per-state YAML output gates and actions precede writer decisions. Valid fresh telemetry remains visible in Disarmed/Fault even when actuation posture guards reject execution. Optional measured odometry retains its own stamp and frame/velocity data; a user height command is not a measured height estimate.

For the common profile, `execution_authorized` grants MC work permission only in `MODE_PREPARING`/`MODE_ACTIVE`; it is not an SDK writer authority flag. Preparing frames allow ordinary target warmup before PolicyReady. The configured Preparing fallback retains fixed-stand commands from Hold or damping from other origins until a fresh matching MC target exists. It never relabels or republishes an old policy target. Env completes preparation only after matching validated target/status plus a live current-transaction TARGET_ACTIVE SDK writer receipt.

MC evaluates fresh-new-frame and execution-stage eligibility before all model work. Missing/stale/duplicate/nonpolicy/unpermitted frames wait without policy reset/observe/infer/post_step, timeline/history/count advancement or target publication. The first eligible frame of an explicit new policy transaction retains ordinary target initialization. Reception gaps do not fail a transaction or clear its active/source policy contexts; request wall timeouts belong to Env's configured strict execution guards. A genuine schema/contract error or inference failure remains an explicit failure. Real execution freshness deadlines remain Env-owned.

## Configuration, input and authority boundary

Installed sim.yaml/real.yaml own six-mode routes, ordinary completion events, per-state publication gates, actions and guards; inputs.yaml is common to terminal keyboard, SDL GameController and Unitree input. Ordinary input comes from one selected source; enabled emergency stops share one normalized predicate used by scheduling and final send admission. Simulator ticks do not identify input connections.

Real startup is readonly. Legal explicit alignment starts asynchronous generation-bound acquisition, with bounded begin/poll/cancel operations outside the control loop's RPC work. BeginAlignment waits for the current acquisition plus fresh measured state. Stop/disarm cancels pending authorization; late success must not begin cancelled alignment. Releases never auto-activate a factory motion service. The real backend uses the same G1 HG encoder and configured non-loopback NIC/domain (domain 0 is valid). Its lazy SDK MotionSwitcher adapter uses explicit float-second timeouts and bounded asynchronous CheckMode / ReleaseMode / CheckMode; construction, readonly startup and unowned exit perform neither authority RPC nor LowCmd Write. Cancellation cannot reauthorize from a late result. A rejected or throwing zero-gain release revokes local permission and records an unconfirmed packet outcome; it earns no publication receipt. No SelectMode or automatic factory-service restoration occurs. Production fake lifecycle tests and successful compilation do not claim completed hardware acceptance.

## Historical profile fixtures

`g1_position_strict_sim_v1` / MECHANICAL_POSITION and the project/equivalent `_sim_v1` PD profiles remain explicit historical fixtures. The former bounds mechanical positions; the latter require their sourced velocity/near-limit/saturation budgets and are tested by the legacy reference fixture only. They are not selected by the new native runtime. `encode_reference` owns those projection fixtures and explicitly rejects the common SDK PD profile; the common encoder never enters that algorithm. No B2 software-PD mode exists.

## Developer-build provenance

SDK revision metadata records Git HEAD. An uncommitted SDK contract diff is not a new separately pinned revision. Stage C developer builds rebuild the SDK and both consumers from the same dirty source tree against the existing base HEAD; at the later approved commit batch, commit SDK first, update both source locks to that committed revision and rebuild both consumers. Do not invent a dirty-tree commit hash or claim HEAD-only metadata identifies uncommitted content.

## 启动、安装与测试边界

正常 Env 入口默认 real，`--sim` 显式选择仿真，`--real` 可显式选择实机；旧 `--loopback-sim` 已移除。入口只执行所选安装内的 native Env ELF，MC 和官方模拟器由用户分别启动。`sim.yaml`、`real.yaml`、`inputs.yaml` 从实际 Env ELF 的同一安装前缀加载，支持显式覆盖、启动后不热重载，无当前目录/源码/其他 overlay 回退。MC 的实际入口是 `anakin_mc/en_run.sh`。

各包启动器优先各自独立安装，否则选择共享工作区；`ANAKIN_PREFIX` / `ANAKIN_SDK_PREFIX` 是显式选择。选定 ELF、本地 ament 标记、资源、消费者构建锁和安装 SDK revision 必须一致。SDK metadata 只记录 HEAD，无法单凭它识别未提交源代码；当前共同开发构建应记录这项限制，最终 SDK 提交后再更新双方锁并重建。

默认 sim 为 SDK `lo` / domain 31、ROS domain 87；real 为部署配置的显式网卡 / SDK domain 0、ROS domain 87。ROS 与 SDK domain 互相独立。默认主输入 keyboard；共用配置可选 SDL gamepad 或 Unitree remote。Sim 来源丢失清零移动而保留策略，real 默认 Fault；停止来自所有启用来源的同一规范化谓词。

默认 `offline` CTests 只使用配置/文件、PTY、注入 SDL、记录遥控、假 RPC/写入与 stub ELF；不得把 ROS fixture、模拟器 runner/probe/reset 纳入其中。专用 ROS 夹具以 `offline_fixture_base` 与明确无物理/SDK/actuation 诊断标识，Preparing 的推理许可可以为 true，但所有 SDK publication 字段必须为零；不能以 `execution_authorized=false` 代替无原生通道的边界标识。用户实际仿真/硬件验收与这些软件检查分别记录，没有固定 30 秒或三次 reset 的完成条件。
