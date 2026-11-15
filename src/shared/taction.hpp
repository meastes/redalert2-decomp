// TActionClass: one action of a trigger type (TAction.cpp, unit 0x006AD100;
// RTTI vtables 0x007AC65C primary, 0x007AC640 IRTTITypeInfo at +4).
// Partial layout (from the constructor 0x006AD370; names are hypotheses):
//   +0x24  int ArrayIndex (-1), +0x28 TActionClass *Next, +0x2C int Type,
//   +0x30  AbstractTypeClass *unknown_30 (swizzled), +0x34..+0x48 ints,
//   +0x4C  AbstractClass *unknown_4c, +0x50 AbstractClass *unknown_50
//          (both swizzled), +0x54 char Text[32], +0x74 int Data (or bool)
//   size 0x78 (slot 12)
#ifndef RA2_SHARED_TACTION_HPP
#define RA2_SHARED_TACTION_HPP

#include "shared/abstract_type.hpp"

extern "C" GUID const CLSID_TActionClass;       // 0x007A1D70
extern "C" char *__cdecl strncpy(char *destination, char const *source, unsigned int count);

class HouseClass;
class ObjectClass;
class TriggerClass;
class Cell;

class TActionClass : public AbstractClass {
public:
    TActionClass();                                                     // 0x006AD370
    TActionClass(NoInitClass const &noinit) : AbstractClass(noinit) {}
    virtual ~TActionClass();                                            // 0x006AD4F0

    virtual HRESULT __stdcall GetClassID(GUID *clsid);                  // 0x006B2C80 slot 3
    virtual HRESULT __stdcall Load(IStream *stream);                    // 0x006B2CC0 slot 5
    virtual HRESULT __stdcall Save(IStream *stream, BOOL clear_dirty);  // 0x006B2D40 slot 6
    virtual void Detach(AbstractClass *target, bool all = true);        // 0x006AD600 slot 10
    virtual int Kind_Of() const;                                        // 0x006B30B0 slot 11
    virtual int Size_Of() const;                                        // 0x006B30A0 slot 12
    virtual void Compute_CRC(WWCRCEngine &crc) const;                   // 0x006B2D60 slot 13
    virtual int Fetch_Heap_ID() const;                                 // 0x006B30C0

    void Build_INI_Entry(char *buffer) const;                           // 0x006AD640 (appends the action's fields)
    void Read_INI();                                                    // 0x006AD860 (the next strtok fields)
    void Set_Text(char const *text);                                    // 0x006B2EC0 (trimmed)
    char const *Get_Text() const;                                       // 0x006B2F20 (0 when empty)
    // Disables every trigger of the type at +0x50; always true (an action handler; name hypothesis).
    // Performs the action (a switch over its type; 0x006ADAE0; TS's operator(), name hypothesis).
    bool Execute(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006ADAE0
    // Calls slot 60 on every active particle system standing on the cell of waypoint +0x44; always true.
    bool TAction_REMOVE_PARTICLE_ANIM(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006AF980
    // BuildingClass::Eject_Occupants(0) on the first living, active building whose tag has the trigger attached; true when found.
    bool TAction_EJECT_OCCUPANTS(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B2FA0
    bool unknown_look_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1B00
    bool TAction_LIGHTNING_STORM(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B25E0
    bool TAction_PLAY_ANIM(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1710
    bool TAction_METEOR_IMPACT(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B19A0
    bool TAction_DZ(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1270
    bool unknown_func_6b1c80(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1C80 (resets the tactical zoom to the rules ZoomInFactor, clears keyboard input, hides the mouse, redraws and sleeps a second)
    bool unknown_func_6b1650(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1650 (sets the map local size from +0x34, refits the radar, clears the cell overlays, shows the buildings and announces the terrain)
    bool TAction_DESTROY_OBJECT(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B14D0 (kills the technos with the trigger attached and scrolls to the cell)
    bool TAction_DISABLE_TRIGGER(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1F90
    // Springs the triggers of the type at +0x50 (forced, no object); always true (name hypothesis).
    bool TAction_FORCE_TRIGGER(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1EC0
    // Action handlers called by the Execute switch (names are hypotheses; each returns true).
    bool unknown_set_session_flag_222b(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006AFB40
    bool unknown_set_session_flag_222a(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006AFB60
    bool unknown_set_session_flag_222c(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006AFB80
    bool unknown_player_4e7ee0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006AFD30
    bool unknown_player_4e82c0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006AFD70
    bool unknown_map_49cc20(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0620
    bool unknown_set_local_variable(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0630
    bool unknown_clear_local_variable(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0650
    bool unknown_show_text(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0440 (shows the action's text in the message list)
    bool unknown_make_ally(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B04C0
    bool unknown_make_enemy(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0500
    bool unknown_set_house_198(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B13E0
    bool unknown_set_house_137(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1400
    bool unknown_set_global_variable(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B2000
    bool unknown_clear_global_variable(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B2020
    bool TAction_PAUSE_SCENARIO(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B2F90 (Hold_Scenario(Data); always true)
    bool TAction_PLAY_MOVIE(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0D30 (Unknown_005A07F0(Data, -1, 1, 1, 1))
    bool TAction_PLAY_INGAME_MOVIE(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0D50 (Unknown_005A0920(Data, false))
    bool TAction_PLAY_INGAME_MOVIE_PAUSE(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0D60 (Unknown_005A0920(Data, true))
    bool TAction_PLAY_SOUND(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0D70 (plays sound Data at full volume)
    bool TAction_SPAWN_TEAM(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1470 (reinforcements of team type +0x30 at its own waypoint)
    bool TAction_REINFORCEMENTS_SPECIAL(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1490 (reinforcements of team type +0x30 at waypoint +0x44)
    bool TAction_ALL_HUNT(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B14B0 (the house of type Data: IHouse::All_To_Hunt)
    bool TAction_SET_AMBIENT_STEP(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B2220 (Rules AmbientChangeStep = DataFloat)
    bool TAction_SET_AMBIENT_RATE(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B2240 (Rules AmbientChangeRate = DataFloat)
    bool TAction_BEGIN_AI_TRIGGERS(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B2290 (the house of type Data (unless -1): +0x13A set)
    bool TAction_STOP_AI_TRIGGERS(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B22B0 (the house of type Data (unless -1): +0x13A cleared)
    bool TAction_CHEER(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B22D0 (the house of type Data (unless -1) cheers)
    bool TAction_FLASH_TEAM(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B2F30 (the team type at +0x30: Flash(Data))
    bool TAction_TALK_BUBBLE(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B2F50 (the first member of the team type's team talks (TechnoClass 0x006D8FD0))
    bool TAction_CHRONO_IN_TEAM(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B2810 (Do_Chrono_Reinforcements(team type +0x30, waypoint +0x44))
    bool TAction_ION_STORM_START(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1A80 (Unknown_0051F930(Data, Rules LightningDeferment, zero cell) unless Unknown_0051FC50)
    bool unknown_func_6b0ac0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0AC0 (adds Data quarter minutes to the mission timer)
    bool unknown_func_6b0b20(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0B20 (takes Data quarter minutes from the mission timer, not below zero)
    bool unknown_func_6b0ba0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0BA0 (sets the mission timer to Data quarter minutes, starts it, clears its label)
    bool TAction_DESTROY_TRIGGER(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1E20 (deletes every trigger of the trigger type at +0x50)
    bool TAction_DESTROY_TAG(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1E70 (deletes every tag of the tag type at +0x4C)
    bool TAction_ENABLE_TRIGGER(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1F10 (enables every trigger of the type at +0x50 that the difficulty allows)
    CoordStruct Waypoint_As_Coord() const;     // 0x006B2E80 (the coordinate of waypoint Data)
    bool unknown_toggle_session_flag_2229(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B2A90
    bool unknown_create_team(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1420
    bool unknown_destroy_teams(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1450
    bool unknown_queue_theme(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1160
    bool unknown_play_voice(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1180
    bool unknown_play_sound(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0D70
    bool unknown_give_units_to_house(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B03E0
    bool unknown_give_technos_to_house(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B03E0
    bool unknown_hunt_idle_feet_not_of_house(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006AFBA0
    bool unknown_hunt_feet_mission_23_of_house(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006AFC20
    bool unknown_hunt_technos_with_tag(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006AFAB0
    bool unknown_hunt_technos_of_house_type(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006AFCA0
    bool unknown_infantry_with_tag_slot294(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0210
    bool unknown_buildings_with_tag_power_down(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0280
    bool unknown_map_55d990_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0800
    bool unknown_reveal_whole_map(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B09B0
    bool unknown_start_mission_timer(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0A20
    bool unknown_map_54e580_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B2830
    bool unknown_map_559fd0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1D70
    bool unknown_63b5b0_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1FD0
    bool unknown_set_session_22a8(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B2260
    bool TAction_ION_LIGHTNING_STRIKE(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006AF950 (Unknown_0051FC90 at the cell of waypoint +0x44)
    bool unknown_particle_system_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006AFA00 (the particle system type Data over the ground at waypoint +0x44)
    bool unknown_set_mission_timer_text(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0C50 (the mission timer label and text from the action text)
    bool unknown_player_4e80d0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006AFD50
    bool unknown_explosion_50_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);    // 0x006B0080 (Unknown_0047F600 with the rules C4 warhead at waypoint Data)
    bool unknown_explosion_100_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B00E0
    bool unknown_explosion_300_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0140
    bool unknown_buildings_with_tag_slot98(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B01A0
    bool unknown_buildings_with_tag_power_up(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B02F0
    bool unknown_technos_with_tag_to_house(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0360
    bool unknown_base_building(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0560 (Red Alert's TACTION_BASE_BUILDING)
    bool unknown_stop_mission_timer(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0A60
    bool TAction_1_SPECIAL(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B11A0 (grants the house's super weapon Data one-time and adds it to the player's sidebar)
    bool TAction_FULL_SPECIAL(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1200 (grants the house's super weapon Data, clears its +0x48, adds it to the player's sidebar)
    bool TAction_WIN(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1360 (the player wins when its house type is Data, else loses)
    bool TAction_LOSE(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1390 (the player loses when its house type is Data, else wins)
    bool unknown_set_house_136(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B13C0 (the house of type Data (unless -1): +0x136 set)
    bool TAction_ZOOM_OUT(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1D00 (undoes 0x006B1C80: zoom 1.0, keyboard cleared, input flag from the pause flag, mouse shown, redraw)
    bool TAction_PLAY_SOUND_AT(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0EC0 (the object at waypoint +0x44 takes sound Data (0x005D5520), else a static sound Data plays there)
    bool TAction_STOP_SOUND_AT(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0F90 (the object at waypoint +0x44 drops its sound, else the static sounds there end)
    bool TAction_CHANGE_SPOTLIGHT_BEHAVIOR(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1D80 (sets the spotlight behavior Data of every living, active spotlight building whose tag has the trigger attached; true when any)
    bool unknown_ion_blast_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B2370 (a new ion blast at waypoint +0x44, ground height)
    bool unknown_center_on_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1BC0 (moves the tactical view to waypoint +0x44, ground height, raised on a bridge)
    bool unknown_sound_at_random_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0D90 (static sound Data at a random set waypoint)
    bool TAction_REVEAL_AROUND_WAYPOINT(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1050 (unless the player's +0x188: Map 0x0054A720 around waypoint Data with the rules RevealTriggerRadius, then 0x0055A300)
    bool unknown_reveal_around_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0670 (Red Alert's TACTION_REVEAL_SOME)
    bool unknown_reveal_zone_of_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B0840 (Red Alert's TACTION_REVEAL_ZONE)
    bool unknown_iron_curtain_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B26D0 (the rules IronCurtainInvokeAnim over waypoint +0x44; slot 82 with the rules IronCurtainDuration on every object in the cells around it)
    bool unknown_weapon_explosion_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B1810 (the explosion of weapon type Data at waypoint +0x44: its damage animation, effects and area damage)
    bool unknown_meteor_shower_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B2040 (METSMALL/METLARGE animations scattered around waypoint +0x44, the count by size Data)
    bool unknown_nuke_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B2400 (a NukePayload bullet dropped from 20000 leptons above waypoint +0x44; false when it cannot be created)
    bool unknown_chem_missile_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B2860 (a ChemLauncher bullet at waypoint +0x44; false when it cannot be created)
    bool unknown_select_sidebar_tab(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B3010 (selects sidebar tab Data when its strip has buildables)
    bool unknown_sidebar_6822b0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell);   // 0x006B3060 (Map.Flash_Cameo with techno type Data and +0x48)
#ifdef RA2_TACTION_EXECUTE_CONTEXT
    bool unknown_action_006af930(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell); // 0x006AF930
    bool TAction_DEACTIVATE_FIRESTORM(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell); // 0x006AF940
    bool TAction_PREFERRED_TARGET(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell); // 0x006B0540
    bool TAction_CHANGE_ZOOM(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell); // 0x006B1640
    bool TAction_ION_STORM_STOP(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell); // 0x006B1AC0
    bool TAction_LOCK_INPUT(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell); // 0x006B1AE0
    bool TAction_UNLOCK_INPUT(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell); // 0x006B1AF0
    bool TAction_SET_AI_TRIGGER_TEAM_RATIO(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell); // 0x006B22F0
    bool TAction_SET_TEAM_AIRCRAFT_RATIO(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell); // 0x006B2310
    bool TAction_SET_TEAM_INFANTRY_RATIO(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell); // 0x006B2330
    bool TAction_SET_TEAM_UNIT_RATIO(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell); // 0x006B2350
#endif
#ifdef RA2_TACTION_DECLARE_DAMAGE
    // Five waypoint blasts and bridge redraw, native006AFD80.
    bool TAction_DAMAGE(HouseClass *, ObjectClass *, TriggerClass *, Cell *);
#endif
#ifdef RA2_TACTION_EXECUTE_CONTEXT
    bool Set_Type_Tech_Level_006b27e0(HouseClass *, ObjectClass *, TriggerClass *, Cell *);
#endif
    static int __fastcall Action_Flags(int type);                       // 0x006B2DF0 (hypothesis)
    // What the value of an action type names (0x006B2AB0; 0x2B, 0x28, 0x0C, 0x22, 0x0B,
    // 0x1A, 0x27, 4, 3, 0x26, 1 are the kinds Build_INI_Entry writes; name hypothesis).
    static int __fastcall Value_Kind(int type);

    int unknown_24;                         // +0x24
    TActionClass *Next;                     // +0x28
    int Type;                               // +0x2C
    AbstractTypeClass *unknown_30;          // +0x30
    int unknown_34;                         // +0x34
    int unknown_38;                         // +0x38
    int unknown_3c;                         // +0x3C
    int unknown_40;                         // +0x40
    int unknown_44;                         // +0x44
    int unknown_48;                         // +0x48
    AbstractClass *unknown_4c;              // +0x4C
    AbstractClass *unknown_50;              // +0x50
    char Text[32];                          // +0x54
#ifdef RA2_TACTION_TYPED_DATA
    union ActionData {
        long Value;
        bool Boolean;
        float Float;
    } Data; // +0x74, the action argument's evidenced int/byte/float views
#else
    union {
        int Data;                           // +0x74
        // The same word read as a flag: the session flag actions 0x006AFB40,
        // 0x006AFB60 and 0x006AFB80 store its low byte unnormalised.
        bool DataBool;                      // +0x74
        // The ambient rate actions (0x006B2220, 0x006B2240) read it as a float.
        float DataFloat;                    // +0x74
    };
#endif
};

// The object in the cell of a coordinate: its building, else its terrain object; none for the unset
// cell (0x006B0E40; Execute calls it, the waypoint sound actions inline it; name hypothesis).
ObjectClass *__fastcall Building_Or_Terrain_At_006B0E40(CoordStruct const &coord);

// Every action (0x00ABFD88; defined in this unit).
extern DynamicVectorClass<TActionClass *> unknown_TActionClass_list_00abfd88;

#endif
