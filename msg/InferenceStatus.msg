uint8 PREPARING=0
uint8 TRANSITIONING=1
uint8 ACTIVE=2
uint8 FAILED=3
std_msgs/Header header
uint32 schema_version
string schema_id
unique_identifier_msgs/UUID session_id
unique_identifier_msgs/UUID mc_instance_id
unique_identifier_msgs/UUID request_id
uint64 epoch
uint64 source_state_seq
string requested_model
string active_model
string gain_profile
uint8 state
bool accepted
bool target_ready
bool source_continuation
bool transition_entry
bool target_active
bool motion_done
# Index order source_continuation, transition_entry, target_active.
# Cumulative within this transaction; latest-depth=1 may skip any first sequence.
uint64[3] phase_first_seq
uint64[3] phase_last_seq
uint8 published_phase_mask
uint8 env_seen_phase_mask
string detail

# Separate cumulative local SDK writer receipts, not physical-execution ACK.
uint8 env_sdk_published_phase_mask
