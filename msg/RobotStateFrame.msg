# Snapshot assembled by env. Each asynchronous nested Header preserves its sample time.
uint32 SCHEMA_VERSION=3
uint32 VALID_JOINTS=1
uint32 VALID_IMU=2
uint32 VALID_COMMAND=4
uint32 VALID_RC=8
uint32 VALID_ODOMETRY=16
std_msgs/Header header
uint32 schema_version
string schema_id
unique_identifier_msgs/UUID session_id
unique_identifier_msgs/UUID env_instance_id
uint64 epoch
uint64 state_seq
uint32 sdk_tick
uint32 validity
string gain_profile
bool execution_authorized
sensor_msgs/JointState joints
sensor_msgs/Imu imu
sensor_msgs/Joy remote
geometry_msgs/TwistStamped command
nav_msgs/Odometry odometry
float64 command_base_height
bool command_stand
# Cumulative env reception/validation feedback, never physical-execution confirmation.
unique_identifier_msgs/UUID mc_instance_seen
unique_identifier_msgs/UUID request_id_seen
uint64 target_seq_seen
uint8 phase_mask_seen
diagnostic_msgs/DiagnosticArray diagnostics

# Local execution FSM; construction always starts DISARMED.
uint8 MODE_DISARMED=0
uint8 MODE_ALIGNING=1
uint8 MODE_HOLD=2
uint8 MODE_ACTIVE=3
uint8 MODE_DAMPING=4
uint8 MODE_FAULT=5
uint8 MODE_PREPARING=6
uint8 execution_mode
# Successful local SDK writer publication, distinct from validation above.
# Neither SDK Write success nor simulator receipt proves actuator execution.
unique_identifier_msgs/UUID sdk_published_mc_instance_id
unique_identifier_msgs/UUID sdk_published_request_id
uint64 sdk_published_epoch
uint64 sdk_published_target_seq
uint8 sdk_published_phase_mask
uint64 sdk_publish_count

# Versioned execution contract. Discovery messages carry no execution authorization.
uint8 MECHANICAL_POSITION=0
uint8 PD_REFERENCE=1
string execution_profile
uint8 command_semantics
