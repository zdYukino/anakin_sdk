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

Target/state latest data use keep-last depth 1, best effort and volatile durability. Status transactions and services use reliable volatile delivery with bounded depth. Reliability does not imply timely delivery. The target TTL is measured from the source-state acquisition recorded locally by env; receipt, retry and DDS redelivery cannot extend it. Env validates source sequence, age, session, mc instance, epoch, request, schema, model, gain profile, position bounds and continuity before execution. No infinite hold on invalid/expired targets. Stop is local and does not wait for mc ACK. Device watchdog and physical remote-loss semantics remain unverified; this contract is not a physical-safety certification.

## Initial scope

G1 ASAP loco actor_obs[1,500] -> action[1,12], independent four-frame grouped history; ASAP CR7 actor_obs[1,380] -> action[1,23]. MC clips raw action at +/-100, scales once by 0.25, adds model defaults and maps once to canonical29. CR7's independently recorded Python failures remain failures until revalidated. Other ONNX policies, TorchScript, hands, FK and physical execution are outside the first implementation milestone.

## Native simulation publication and authorization

Version 2 appends an explicit execution-mode enum and SDK publication identities, epoch, target sequence, cumulative phase mask and count to RobotStateFrame. MC reports a distinct cumulative env_sdk_published_phase_mask. Reception/validation and SDK writer success are separate facts; neither proves actuator execution. All three repositories must rebuild against the same locked SDK commit.

The only native backend is explicitly selected unitree_loopback_sim on an inspected active lo interface and a nonzero SDK domain. It has no hardware path. Construction starts disarmed. Fresh RC L1+R1+X first authorizes a two-second measured-pose alignment, then holds; a second fresh edge requests loco inference. A/B locally revokes targets, with at most 100 ms of damping and no wait for MC. R1+Up requests mimic, R1+Down requests loco, and L1+R1+Y requests a new epoch for the current model. Terminal faults require a process restart and explicit authorization.

The sender is a separate C++ monotonic 2 ms thread; it skips missed periods and checks state age, RC age, transaction and source TTL before SDK writes. Duplicate SDK ticks cannot renew source age. Feedback is finite and CRC checked; absent remote/odometry has no validity bit. Physical remote-loss/watchdog semantics remain unverified.

## Explicit simulation gain profiles

`g1_asap_sim_v1` preserves the original frozen full-ASAP 29-joint table for reproducibility. `g1_asap_merged_sim_v1` reproduces the unchanged Python environment merge: start with G1_29DoF and override only the actual producing policy's action_dof. Loco overrides twelve leg joints; CR7 overrides twenty-three joints and retains the base environment gains at its six missing wrists. Profile data is shared metadata in simulation_profile.hpp; env owns profile authorization, selection using the actual emitted model/phase, CRC and all SDK commands. The gain profile is identical in env state, request, status and target. These profiles have identical existing position bounds; neither introduces output clipping or a hardware backend.

## Explicit reference semantics (v3)

State, switch request/queued response, status and target carry an identical execution_profile and command_semantics. Unknown profile/type pairs are rejected, including reuse of a request UUID with different semantics. The default g1_position_strict_sim_v1 / MECHANICAL_POSITION retains the original bounded-position negative check. g1_pd_reference_project_sim_v1 and g1_pd_reference_equivalent_sim_v1 use PD_REFERENCE and require the merged gain profile. MC publishes the unchanged final startup/transition mux reference q_v; raw action/history are never replaced with projected or encoded values. Env alone owns mechanical projection, gains, compensation, budgets, CRC and SDK writes.

A projects q_s into the mechanical range with zero feed-forward and explicitly changes PD effort. B1 uses q_s=clip(q_v,l,u), tau_ff=Kp*(q_v-q_s) with the same SDK Kp/Kd and dq_ref=0. The float32 encoder uses the representable SDK q_v and in-range q_s, then accounts for feed-forward rounding. This algebra is not a safety proof. No B2 software-PD mode exists. Both new profiles require complete, sourced velocity/near-limit/saturation budgets before native SDK initialization; an incomplete frozen budget must fail closed.
