#ifndef RA2_SCENARIO_CHECKPOINT_HPP
#define RA2_SCENARIO_CHECKPOINT_HPP
struct IStream;
// Nonpolymorphic scenario-state checkpoint;original class name unresolved.
class EnvironmentClass {
public:
 EnvironmentClass();
 ~EnvironmentClass();
 void Capture_004B5AB0();
 void Restore_004B5B40(); // 0x004B5B40 (sets the 50 variables back, then credits, timer and difficulty)
 void Read_004B5C50(IStream *stream);
 void Write_004B5C70(IStream *stream);
 bool Variables[50];
 int Credits; // +34
 int TimerRemaining; // +38
 int Difficulty; // +3c
 unsigned short unknown_40;
};
extern EnvironmentClass unknown_scenario_checkpoint_00855a90;   // 0x00855A90
typedef char ScenarioCheckpoint_size_check[(sizeof(EnvironmentClass)==0x44)?1:-1];
#endif
