uint32 SCHEMA_VERSION=3
uint8 SOURCE_CONTINUATION=1
uint8 TRANSITION_ENTRY=2
uint8 TARGET_ACTIVE=4
std_msgs/Header header
uint32 schema_version
string schema_id
unique_identifier_msgs/UUID session_id
unique_identifier_msgs/UUID mc_instance_id
unique_identifier_msgs/UUID request_id
uint64 epoch
uint64 seq
uint64 source_state_seq
string model_id
string gain_profile
uint32 validity
# TTL starts at the source-state acquisition recorded by env, never at reception.
builtin_interfaces/Duration valid_for
uint8 phase
# Canonical29 absolute radian mux references, exactly one point; velocities/effort empty.
# MECHANICAL_POSITION requires bounded positions. PD_REFERENCE is virtual PD q_v
# after startup/transition mux; only the negotiated simulation profile may encode it.
trajectory_msgs/JointTrajectory target

# Versioned execution contract. Discovery messages carry no execution authorization.
uint8 MECHANICAL_POSITION=0
uint8 PD_REFERENCE=1
string execution_profile
uint8 command_semantics
