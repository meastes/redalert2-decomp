// ion.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/ion.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x0051EF10:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x0051EF10-0x0051F0F6); the other
// drafts of this plan are emitted by these includes.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"
#define RA2_ION_STORM_LISTS
#include "shared/lightning_storm.hpp"

// Ion.cpp's file statics with dynamic initializers: the storm cell
// (0x0051F100) and the three storm lists (0x0051F110/0x0051F1B0/0x0051F250,
// each registering its guarded atexit destructor).
Cell StormCoords = unknown_geometry_k;
DynamicVectorClass<AnimClass *> IonStormClass::CloudsManifesting;
DynamicVectorClass<AnimClass *> IonStormClass::CloudsPresent;
DynamicVectorClass<AnimClass *> IonStormClass::BoltsPresent;


// Dynamic initializer (0x0051EF40, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_0051ef10.cpp; no source of its own.


// Dynamic initializer (0x0051EF60, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_0051ef10.cpp; no source of its own.


// Dynamic initializer (0x0051EF80, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_0051ef10.cpp; no source of its own.


// Dynamic initializer (0x0051EFA0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_0051ef10.cpp; no source of its own.


// Dynamic initializer (0x0051EFC0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_0051ef10.cpp; no source of its own.


// Dynamic initializer (0x0051EFE0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_0051ef10.cpp; no source of its own.


// Dynamic initializer (0x0051F020, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_0051ef10.cpp; no source of its own.


// Dynamic initializer (0x0051F050, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_0051ef10.cpp; no source of its own.


// Dynamic initializer (0x0051F080, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_0051ef10.cpp; no source of its own.


// Dynamic initializer (0x0051F0B0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_0051ef10.cpp; no source of its own.


// Dynamic initializer (0x0051F0C0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_0051ef10.cpp; no source of its own.


// Dynamic initializer (0x0051F0E0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_0051ef10.cpp; no source of its own.


// Dynamic initializer of StormCoords (0x0051F100, _$E) emitted by HeaderStatics_diagonal_init_0051ef10.cpp; no source of its own.


// Dynamic initializer of IonStormClass::CloudsManifesting (0x0051F110, _$E) emitted by HeaderStatics_diagonal_init_0051ef10.cpp; no source of its own.


// atexit destructor of IonStormClass::CloudsManifesting (0x0051F150, _$E) emitted by HeaderStatics_diagonal_init_0051ef10.cpp; no source of its own.


// Dynamic initializer of IonStormClass::CloudsPresent (0x0051F1B0, _$E) emitted by HeaderStatics_diagonal_init_0051ef10.cpp; no source of its own.


// atexit destructor of IonStormClass::CloudsPresent (0x0051F1F0, _$E) emitted by HeaderStatics_diagonal_init_0051ef10.cpp; no source of its own.


// Dynamic initializer of IonStormClass::BoltsPresent (0x0051F250, _$E) emitted by HeaderStatics_diagonal_init_0051ef10.cpp; no source of its own.


// atexit destructor of IonStormClass::BoltsPresent (0x0051F290, _$E) emitted by HeaderStatics_diagonal_init_0051ef10.cpp; no source of its own.


// 0x0051F2F0: resets the storm and dominator state, empties the cloud and
// bolt lists and returns the scenario's ambient light.
#define VECTOR_IMPLEMENT_ALL
#include "shared/id_source.hpp"

void __fastcall Init_Storm_0051F2F0()
{
    StormCoords = unknown_geometry_k;
    StormActive = false;
    unknown_dominator_state_a5193c = 0;
    StormStartTime = -1;
    StormDuration = -1;
    unknown_dominator_start_7ddb18 = -1;
    unknown_dominator_delay_7ddb1c = -1;
    StormDeferment = 0;
    StormOwner = 0;
    IonStormClass::CloudsManifesting.Clear();
    IonStormClass::CloudsPresent.Clear();
    IonStormClass::BoltsPresent.Clear();
    unknown_id_source_a3d290->unknown_22b0 = unknown_id_source_a3d290->unknown_22a8;
    Set_Lighting_00520B00(-1, -1, -1, false);
}


// 0x0051F3F0: writes the storm and dominator state, then each list as a count and its
// anim pointers.
#include "shared/com_persist.hpp"


HRESULT __fastcall Save_Storm_0051F3F0(IStream *stream)
{
    HRESULT result = stream->Write(&StormActive, sizeof(StormActive), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Write(&StormStartTime, sizeof(StormStartTime), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Write(&StormDuration, sizeof(StormDuration), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Write(&StormDeferment, sizeof(StormDeferment), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Write(&StormTimeToEnd, sizeof(StormTimeToEnd), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Write(&unknown_dominator_start_7ddb18, sizeof(unknown_dominator_start_7ddb18), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Write(&unknown_dominator_delay_7ddb1c, sizeof(unknown_dominator_delay_7ddb1c), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Write(&unknown_dominator_state_a5193c, sizeof(unknown_dominator_state_a5193c), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Write(&StormCoords, sizeof(StormCoords), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Write(&StormOwner, sizeof(StormOwner), 0);
    if (FAILED(result)) {
        return result;
    }
    int count;
    count = IonStormClass::CloudsManifesting.Count();
    result = stream->Write(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    for (int i = 0; i < count; i++) {
        AnimClass *anim = IonStormClass::CloudsManifesting[i];
        result = stream->Write(&anim, sizeof(anim), 0);
        if (FAILED(result)) {
            return result;
        }
    }
    count = IonStormClass::CloudsPresent.Count();
    result = stream->Write(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    for (int j = 0; j < count; j++) {
        AnimClass *anim = IonStormClass::CloudsPresent[j];
        result = stream->Write(&anim, sizeof(anim), 0);
        if (FAILED(result)) {
            return result;
        }
    }
    count = IonStormClass::BoltsPresent.Count();
    result = stream->Write(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    for (int k = 0; k < count; k++) {
        AnimClass *anim = IonStormClass::BoltsPresent[k];
        result = stream->Write(&anim, sizeof(anim), 0);
        if (FAILED(result)) {
            return result;
        }
    }
    return result;
}


// 0x0051F5E0: reads what 0x0051F3F0 wrote; the owner and the anim pointers are
// registered for swizzling.
#include "shared/swizzle.hpp"

HRESULT __fastcall Load_Storm_0051F5E0(IStream *stream)
{
    HRESULT result = stream->Read(&StormActive, sizeof(StormActive), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Read(&StormStartTime, sizeof(StormStartTime), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Read(&StormDuration, sizeof(StormDuration), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Read(&StormDeferment, sizeof(StormDeferment), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Read(&StormTimeToEnd, sizeof(StormTimeToEnd), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Read(&unknown_dominator_start_7ddb18, sizeof(unknown_dominator_start_7ddb18), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Read(&unknown_dominator_delay_7ddb1c, sizeof(unknown_dominator_delay_7ddb1c), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Read(&unknown_dominator_state_a5193c, sizeof(unknown_dominator_state_a5193c), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Read(&StormCoords, sizeof(StormCoords), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Read(&StormOwner, sizeof(StormOwner), 0);
    if (FAILED(result)) {
        return result;
    }
    Swizzler.Swizzle((void **)&StormOwner);
    int count;
    result = stream->Read(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    for (int i = 0; i < count; i++) {
        AnimClass *anim;
        result = stream->Read(&anim, sizeof(anim), 0);
        if (FAILED(result)) {
            return result;
        }
        IonStormClass::CloudsManifesting.Add(anim);
    }
    for (i = 0; i < count; i++) {
        Swizzler.Swizzle((void **)&IonStormClass::CloudsManifesting[i]);
    }
    result = stream->Read(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    for (int j = 0; j < count; j++) {
        AnimClass *anim;
        result = stream->Read(&anim, sizeof(anim), 0);
        if (FAILED(result)) {
            return result;
        }
        IonStormClass::CloudsPresent.Add(anim);
    }
    for (j = 0; j < count; j++) {
        Swizzler.Swizzle((void **)&IonStormClass::CloudsPresent[j]);
    }
    result = stream->Read(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    for (int k = 0; k < count; k++) {
        AnimClass *anim;
        result = stream->Read(&anim, sizeof(anim), 0);
        if (FAILED(result)) {
            return result;
        }
        IonStormClass::BoltsPresent.Add(anim);
    }
    for (k = 0; k < count; k++) {
        Swizzler.Swizzle((void **)&IonStormClass::BoltsPresent[k]);
    }
    return result;
}


// 0x0051F920: sets StormActive (hypothesis: used by a load or a trigger).

void __fastcall Set_Storm_Active_0051F920(bool active)
{
    StormActive = active;
}


// 0x0051F930: starts the lightning storm at cell for owner (a random cell the
// map accepts when cell is unknown_geometry_k). With a deferment the storm is
// only armed (the shorter deferment wins). Otherwise a radar event marks the
// cell, the houses that are not the owner's allies get the storm warning, the
// ion lighting comes on (unless the dominator lighting holds) and the player
// is told (Rules LightningPrintText).
#include "shared/house.hpp"
#include "shared/map_view.hpp"
#include "shared/radar_event.hpp"
#include "shared/rules.hpp"
#include "shared/sound_play.hpp"
#include "shared/string_table.hpp"
#include "shared/session_state.hpp"

void __fastcall Unknown_0051F930(int duration, int deferment, Cell cell, HouseClass *owner)
{
    if (cell == unknown_geometry_k) {
        while (!Map.In_Radar(cell)) {
            cell = Cell(Random_Pick(0, Map.unknown_128), Random_Pick(0, Map.unknown_12c));
        }
    }
    StormCoords = cell;
    StormOwner = owner;
    if (StormActive) {
        return;
    }
    if (deferment) {
        if (StormDeferment == 0 || StormDeferment >= deferment) {
            StormDeferment = deferment;
        }
        StormDuration = duration;
        return;
    }
    Unknown_0063B5B0(13, cell);
    StormDuration = duration;
    StormStartTime = Frame;
    StormActive = true;
    for (int index = 0; index < Houses.Count(); index++) {
        if (StormOwner == 0 || !StormOwner->Is_Ally(Houses[index])) {
            if (!Houses[index]->unknown_13d && !unknown_int_a3d548) {
                Houses[index]->unknown_func_4f5840(duration);
            }
        }
    }
    if (unknown_player_a35db4 != 0) {
        unknown_player_a35db4->unknown_5669 = true;
    }
    if (unknown_dominator_state_a5193c != 1) {
        unknown_id_source_a3d290->unknown_22b0 = unknown_id_source_a3d290->IonAmbient;
        Set_Lighting_00520B00(unknown_id_source_a3d290->IonRed * 1000 / 100,
            unknown_id_source_a3d290->IonGreen * 1000 / 100,
            unknown_id_source_a3d290->IonBlue * 1000 / 100, true);
    }
    if (Rules->LightningPrintText) {
        int color = 3;
        if (unknown_player_a35db4 != 0) {
            color = unknown_player_a35db4->ColorSchemeIndex;
        }
        Voc_Play(Rules->StormSound, 1.0f, 0x2000, 0);
        unknown_session_a3d298.Messages.Add_Message(0, 0,
            StringTable::Load_String("TXT_LIGHTNING_STORM", 0, "D:\\Ra2\\Ion.cpp", 465), color, 0x4046, 150, false);
    }
    Map.Flag_To_Redraw(true);
}


// void Unknown_0051FBB0() (0x0051FBB0)
// Ends an active lightning storm at its next update.

void Unknown_0051FBB0()
{
    if (StormActive) {
        StormTimeToEnd = true;
    }
}


// 0x0051FBD0: ends an active storm: the cell and owner are cleared and,
// unless the dominator lighting holds, the scenario's ambient light returns.

void __fastcall Stop_Storm_0051FBD0()
{
    if (StormActive) {
        StormActive = false;
        StormCoords = unknown_geometry_k;
        StormOwner = 0;
        if (unknown_dominator_state_a5193c != 1) {
            unknown_id_source_a3d290->unknown_22b0 = unknown_id_source_a3d290->unknown_22a8;
            Set_Lighting_00520B00(-1, -1, -1, false);
            Map.Flag_To_Redraw(true);
        }
    }
}


// 0x0051FC30: whether a storm is active or armed (deferment pending).

int __fastcall Is_Storm_Pending_0051FC30()
{
    if (StormActive || StormDeferment > 0) {
        return 1;
    }
    return 0;
}


// bool Unknown_0051FC50() (0x0051FC50)
// Whether a lightning storm is active.

bool Unknown_0051FC50()
{
    return StormActive;
}


// 0x0051FC60: true exactly when the original DWORD at 0x00A5193C equals one.
// The state's role and the original developer name remain unknown.
extern "C" unsigned int nuke_flash_state_00a5193c;

bool Unknown_0051FC60()
{
    return nuke_flash_state_00a5193c == 1;
}


// 0x0051fc70: function not yet identified
//
// Body: 15 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: bool unknown_nuke_flash_state_2_0051fc70(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0051fc70 {

extern "C" unsigned int nuke_flash_state_00a5193c;

bool  unknown_nuke_flash_state_2_0051fc70(void)
{
    return nuke_flash_state_00a5193c == 2;
}

}  // namespace seed_0051fc70


class Leaf_51fc80 { public: bool get(); };
bool Leaf_51fc80::get() { return false; }


// 0x0051FC90: adds a storm cloud over cell: a random WeatherConClouds anim,
// raised by half the height of the first WeatherConBolts image (in leptons),
// joins the manifesting clouds and the present clouds. Nothing happens at the
// zero coordinate.
#define SHAPE_FILE_EMPTY_RECT unknown_static_rect
#include "shared/shape_file.hpp"
#include "shared/anim.hpp"
#include "shared/cell.hpp"
#include "shared/tactical.hpp"

void __stdcall Unknown_0051FC90(Cell cell)
{
    CellClass &cellptr = Map[cell];
    int half = ((ShapeFileStruct *)Rules->WeatherConBolts[0]->Get_Image_Data())->Height / 2;
    int height = Unknown_006A29B0(half);
    CoordStruct coord(cell.X * 256 + 128, cell.Y * 256 + 128,
        cellptr.unknown_11b * unknown_geometry_g + height + ((cellptr.unknown_140 & 0x100) ? unknown_geometry_j : 0));
    if (coord == unknown_geometry_l) {
        return;
    }
    int pick = unknown_id_source_a3d290->Random()();
    AnimTypeClass *type = Rules->WeatherConClouds[(unsigned)pick % Rules->WeatherConClouds.Count()];
    AnimClass *anim = new AnimClass(type, coord, 0, 1, 0x600, 0);
    IonStormClass::CloudsManifesting.Add(anim);
    IonStormClass::CloudsPresent.Add(anim);
}


// 0x0051FE50: a lightning bolt strikes the cell under coord: a random
// WeatherConBolts anim at the cell's centre joins the bolt list, a random
// LightningSounds sound plays, the LightningWarhead's damage anim, explosion
// and LightningDamage hit the cell's ground coordinate, and unless an object
// of kind 15 stands there, 2-4 MetallicDebris anims fly when the cell's
// building, occupant or level changed or its land type is 1, 3, 4 or 11.

void __fastcall Storm_Strike_0051FE50(CoordStruct coord)
{
    CellClass &cellptr = Map[coord];
    CoordStruct center = CoordStruct(cellptr.unknown_475d60());
    int pick = unknown_id_source_a3d290->Random()();
    AnimTypeClass *type = Rules->WeatherConBolts[(unsigned)pick % Rules->WeatherConBolts.Count()];
    AnimClass *bolt = new AnimClass(type, center, 0, 1, 0x600, 0);
    IonStormClass::BoltsPresent.Add(bolt);

    Cell cell = cellptr.MapCoords;
    int level = cellptr.unknown_11b;
    CoordStruct where(cell.X * 256 + 128, cell.Y * 256 + 128,
        ((cellptr.unknown_140 & 0x100) ? unknown_geometry_j : 0) + level * unknown_geometry_g);
    if (where == unknown_geometry_l) {
        return;
    }
    if (Rules->LightningSounds.Count() > 0) {
        int sound = unknown_id_source_a3d290->Random()();
        Voc_Play_At(Rules->LightningSounds[(unsigned)sound % Rules->LightningSounds.Count()], where, 0);
    }
    int land = cellptr.unknown_ec;
    int damage = Rules->LightningDamage;
    AnimTypeClass *anim = Select_Damage_Anim(damage, Rules->LightningWarhead, land, &where);
    new AnimClass(anim, where, 0, 1, 0x2600, Get_Explosion_Z_0047fcc0(where));

    bool changed = false;
    ObjectClass *building = cellptr.Cell_Building();
    ObjectClass *object = cellptr.Cell_Techno(Point2D(0, 0), false, 0);
    bool sheltered = object != 0 && object->Kind_Of() == 15;
    if (building == 0 && object == 0) {
        switch (cellptr.unknown_ec) {
        case 1:
        case 3:
        case 4:
        case 11:
            changed = true;
            break;
        }
    }
    Unknown_0047F600(where, damage, Rules->LightningWarhead, 0, 0);
    Unknown_0047E2C0(where, damage, 0, Rules->LightningWarhead, 1);
    if (building != cellptr.Cell_Building() || object != cellptr.Cell_Techno(Point2D(0, 0), false, 0)
        || cellptr.unknown_11b != level) {
        changed = true;
    }
    if (!sheltered && changed) {
        int count = Random_Pick(2, 4);
        for (int index = 0; index < count; index++) {
            new AnimClass(Rules->MetallicDebris[Random_Pick(0, Rules->MetallicDebris.Count() - 1)], where, 0, 1, 0x600, 0);
        }
    }
}


// 0x00520200: the storm's frame logic. The dominator states time out (1 ends
// as 0x00520980 does, 2 returns to 0); bolts half played and present clouds
// played out leave their lists; manifesting clouds past half strike their
// cell (0x0051FE50); with no clouds left a requested end stops the storm. An
// active storm ends after its duration, adds a cloud over its cell every
// LightningHitDelay frames and, every LightningScatterDelay frames, one more
// within LightningCellSpread / 2 cells that the map accepts and that is at
// least LightningSeparation from every present cloud (three tries). A
// deferred storm counts down, starts at zero and warns every 225 frames.
#include "shared/object.hpp"
#include "shared/vox_class.hpp"

void __fastcall Update_Storm_00520200()
{
    if (unknown_dominator_state_a5193c == 1) {
        if (unknown_dominator_delay_7ddb1c != -1 && unknown_dominator_start_7ddb18 + unknown_dominator_delay_7ddb1c < Frame) {
            End_Dominator_Lighting_00520980();
        }
    } else if (unknown_dominator_state_a5193c == 2) {
        if (unknown_dominator_delay_7ddb1c != -1 && unknown_dominator_start_7ddb18 + unknown_dominator_delay_7ddb1c < Frame) {
            unknown_dominator_state_a5193c = 0;
        }
    }

    if (IonStormClass::BoltsPresent.Count() > 0) {
        for (int index = IonStormClass::BoltsPresent.Count() - 1; index >= 0; index--) {
            AnimClass *bolt = IonStormClass::BoltsPresent[index];
            int stage = bolt->Stage;
            if (stage >= ((ShapeFileStruct *)bolt->Class_Of()->Get_Image_Data())->FrameCount / 2) {
                IonStormClass::BoltsPresent.Delete(index);
            }
        }
    }
    for (int index = IonStormClass::CloudsManifesting.Count() - 1; index >= 0; index--) {
        AnimClass *cloud = IonStormClass::CloudsManifesting[index];
        int stage = cloud->Stage;
        if (stage > ((ShapeFileStruct *)cloud->Class_Of()->Get_Image_Data())->FrameCount / 2) {
            Storm_Strike_0051FE50(cloud->Center_Coord());
            IonStormClass::CloudsManifesting.Delete(index);
        }
    }
    if (IonStormClass::CloudsPresent.Count() > 0) {
        for (int index = IonStormClass::CloudsPresent.Count() - 1; index >= 0; index--) {
            AnimClass *cloud = IonStormClass::CloudsPresent[index];
            int stage = cloud->Stage;
            if (stage >= ((ShapeFileStruct *)cloud->Class_Of()->Get_Image_Data())->FrameCount - 1) {
                IonStormClass::CloudsPresent.Delete(index);
            }
        }
    } else if (StormTimeToEnd) {
        Stop_Storm_0051FBD0();
        StormTimeToEnd = false;
    }

    if (StormActive && !StormTimeToEnd) {
        if (StormDuration != -1 && StormStartTime + StormDuration < Frame) {
            StormTimeToEnd = true;
            return;
        }
        if (Frame % Rules->LightningHitDelay == 0) {
            Unknown_0051FC90(StormCoords);
        }
        if (Frame % Rules->LightningScatterDelay == 0) {
            int range = Rules->LightningCellSpread >> 1;
            int tries;
            Cell cell;
            for (tries = 3; tries > 0; tries--) {
                cell = StormCoords;
                cell.X += Random_Pick(-range, range);
                cell.Y += Random_Pick(-range, range);
                bool crowded = false;
                for (int index = 0; index < IonStormClass::CloudsPresent.Count(); index++) {
                    Cell other = Coord_Cell(IonStormClass::CloudsPresent[index]->Center_Coord());
                    int dx = cell.X - other.X;
                    if (dx < 0) {
                        dx = -dx;
                    }
                    int dy = cell.Y - other.Y;
                    if (dy < 0) {
                        dy = -dy;
                    }
                    if (dx + dy < Rules->LightningSeparation) {
                        crowded = true;
                        break;
                    }
                }
                if (Map.In_Radar(cell) && !crowded) {
                    break;
                }
            }
            if (tries > 0) {
                Unknown_0051FC90(cell);
            }
        }
    } else if (StormDeferment > 0) {
        StormDeferment--;
        if (StormDeferment == 0) {
            Unknown_0051F930(StormDuration, 0, StormCoords, StormOwner);
        } else if (StormDeferment % 225 == 0 && Rules->LightningPrintText) {
            int color = 3;
            if (unknown_player_a35db4 != 0) {
                color = unknown_player_a35db4->ColorSchemeIndex;
            }
            Speech_Play_Name("EVA_LightningStormCreated", -1, -1);
            unknown_session_a3d298.Messages.Add_Message(0, 0,
                StringTable::Load_String("TXT_LIGHTNING_STORM_APPROACHING", 0, "D:\\Ra2\\Ion.cpp", 904), color, 0x46, 150,
                false);
        }
    }
}


// 0x00520770: reapplies the dominator or storm lighting (after a load).

void Unknown_00520770()
{
    if (unknown_dominator_state_a5193c == 1) {
        unknown_id_source_a3d290->unknown_22b0 = unknown_id_source_a3d290->unknown_22e0;
        Set_Lighting_00520B00(unknown_id_source_a3d290->unknown_22e4 * 1000 / 100,
            unknown_id_source_a3d290->unknown_22e8 * 1000 / 100,
            unknown_id_source_a3d290->unknown_22ec * 1000 / 100, true);
    } else if (StormActive) {
        unknown_player_a35db4->unknown_5669 = true;
        unknown_id_source_a3d290->unknown_22b0 = unknown_id_source_a3d290->IonAmbient;
        Set_Lighting_00520B00(unknown_id_source_a3d290->IonRed * 1000 / 100,
            unknown_id_source_a3d290->IonGreen * 1000 / 100,
            unknown_id_source_a3d290->IonBlue * 1000 / 100, true);
    }
}


// 0x005208A0: the dominator lighting starts: state 1 for 30 frames, the
// scenario's +0x828 timer restarted at 1 and the dominator ambient applied.

void __fastcall Start_Dominator_Lighting_005208A0()
{
    unknown_dominator_state_a5193c = 1;
    unknown_dominator_delay_7ddb1c = 30;
    unknown_dominator_start_7ddb18 = Frame;
    unknown_id_source_a3d290->unknown_828 = 1;
    unknown_id_source_a3d290->unknown_22b0 = unknown_id_source_a3d290->unknown_22e0;
    Set_Lighting_00520B00(unknown_id_source_a3d290->unknown_22e4 * 1000 / 100,
        unknown_id_source_a3d290->unknown_22e8 * 1000 / 100,
        unknown_id_source_a3d290->unknown_22ec * 1000 / 100, true);
    Map.Flag_To_Redraw(true);
}


// 0x00520980: the dominator lighting ends: state 2 for 15 frames, then the
// storm's or the scenario's ambient light again.

void __fastcall End_Dominator_Lighting_00520980()
{
    unknown_dominator_state_a5193c = 2;
    unknown_dominator_start_7ddb18 = Frame;
    unknown_dominator_delay_7ddb1c = 15;
    if (StormActive) {
        unknown_id_source_a3d290->unknown_22b0 = unknown_id_source_a3d290->IonAmbient;
        Set_Lighting_00520B00(unknown_id_source_a3d290->IonRed * 1000 / 100,
            unknown_id_source_a3d290->IonGreen * 1000 / 100,
            unknown_id_source_a3d290->IonBlue * 1000 / 100, true);
    } else {
        unknown_id_source_a3d290->unknown_22b0 = unknown_id_source_a3d290->unknown_22a8;
        Set_Lighting_00520B00(-1, -1, -1, false);
    }
    Map.Flag_To_Redraw(true);
}


// 0x00520A80: tints the converter of every scheme in the colour-scheme lists
// (0x00A76414) to red, green, blue.
#include "shared/color_scheme.hpp"
#include "shared/light_convert.hpp"

void __fastcall Set_Scheme_Lighting_00520A80(int red, int green, int blue, bool unknown4)
{
    UnknownSchemeListIterator iterator;
    for (DynamicVectorClass<ColorScheme *> *list = iterator.Next(); list != 0; list = iterator.Next()) {
        int count = Unknown_00603C10();
        for (int index = 0; index < count; index++) {
            ((LightConvertClass *)(*list)[index]->Converter)->Set_Tint(red, green, blue, unknown4);
        }
    }
}


// 0x00520B00: tints every light converter, the converters of the colour
// schemes with +0x310 above 1 and those of the colour-scheme lists
// (0x00520A80, inline) to red, green, blue (thousandths; -1 for the scenario
// ambient), then refreshes every map cell.

void __fastcall Set_Lighting_00520B00(int red, int green, int blue, bool unknown4)
{
    int index;
    for (index = 0; index < unknown_LightConvertClass_list_00832398.Count(); index++) {
        unknown_LightConvertClass_list_00832398[index]->Set_Tint(red, green, blue, unknown4);
    }
    for (index = 0; index < ColorSchemes.Count(); index++) {
        if (ColorSchemes[index]->unknown_0x310 > 1) {
            ((LightConvertClass *)ColorSchemes[index]->Converter)->Set_Tint(red, green, blue, unknown4);
        }
    }
    Set_Scheme_Lighting_00520A80(red, green, blue, unknown4);
    Map.Unknown_0049E5E0();
}


// 0x00520BF0: tells the player that a lightning storm is already active
// (Msg:LightningStormActive, in the player's colour).

void __fastcall Storm_Active_Message_00520BF0()
{
    wchar_t const *text = StringTable::Load_String("Msg:LightningStormActive", 0, "D:\\Ra2\\Ion.cpp", 1066);
    int color = 3;
    if (unknown_player_a35db4 != 0) {
        color = unknown_player_a35db4->ColorSchemeIndex;
    }
    unknown_session_a3d298.Messages.Add_Message(0, 0, text, color, 0x4046, 150, false);
}


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<AnimClass *>::Clear (0x00520C40; DynamicVectorClass<AnimClass *> vtable 0x007a4554 slot 3).
// DynamicVectorClass<AnimClass *>::ID_pointer (0x00520C70; DynamicVectorClass<AnimClass *> vtable 0x007a4554 slot 5).
// VectorClass<AnimClass *>::Fetch (0x00520C90; VectorClass<AnimClass *> vtable 0x007a4574 slot 6).
// VectorClass<AnimClass *>::operator equal (0x00520CA0; VectorClass<AnimClass *> vtable 0x007a4574 slot 1).
// VectorClass<AnimClass *>::Resize (0x00520CE0; VectorClass<AnimClass *> vtable 0x007a4574 slot 2).
// VectorClass<AnimClass *>::Clear (0x00520D90; VectorClass<AnimClass *> vtable 0x007a4574 slot 3).
// VectorClass<AnimClass *>::ID_object (0x00520DC0; VectorClass<AnimClass *> vtable 0x007a4574 slot 4).
// VectorClass<AnimClass *>::ID_pointer (0x00520E00; VectorClass<AnimClass *> vtable 0x007a4574 slot 5).
// DynamicVectorClass<AnimClass *>::Resize (0x00520E20; DynamicVectorClass<AnimClass *> vtable 0x007a4554 slot 2).
// DynamicVectorClass<AnimClass *>::ID_object (0x00520ED0; DynamicVectorClass<AnimClass *> vtable 0x007a4554 slot 4).


extern "C" unsigned char vtable_520f00[]; extern "C" void __cdecl release_buffer_520f00(void*); extern "C" void __cdecl release_object_520f00(void*);
class Leaf_520f00 { public: void* get(unsigned int flags); };
void* Leaf_520f00::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_520f00; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_520f00((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_520f00(this); return this; }


extern "C" unsigned char vtable_520f50[]; extern "C" void __cdecl release_buffer_520f50(void*); extern "C" void __cdecl release_object_520f50(void*);
class Leaf_520f50 { public: void* get(unsigned int flags); };
void* Leaf_520f50::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_520f50; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_520f50((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_520f50(this); return this; }
