// NON-EXACT DRAFT 0x421710 FUN_00421710
// entry 0x421710; exact-match score 99.61 (misses_index best); plan "draft as-is"
// origin lane: Scout766 (build/class-work/Scout766/keep/an_8574.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1069): unknown_slot71 renamed Mark (slot 71, object.hpp); each changed line is marked NATIVE.
// void AnimClass::AI() (0x00421710, slot 23)
// Per-frame logic of an animation (TibSun AnimClass::AI): keeps its sound at
// its position, runs the burning-infantry logic, hides psychic-warning and
// ore-dependent animations, ends a bouncing animation where it lands (water
// splashes, or its expire animation with damage and light, spawned
// animations and tiberium sprouts), drops trailers, removes a drop-zone
// marker under a building, then steps the stage: damage to the object it is
// attached to, the ground effect at the biggest frame, ping-pong, looping and
// chaining into the next type.
#include "shared/anim.hpp"
#include "shared/map_view.hpp"
#include "shared/cell_class.hpp"
#include "shared/house.hpp"
#include "shared/rules.hpp"
#include "shared/sound_play.hpp"
#include "shared/random.hpp"
#include "shared/id_source.hpp"
#include "shared/options.hpp"
#include "shared/overlay.hpp"
#include "shared/overlay_type.hpp"
#include "shared/tactical.hpp"
#include "shared/terrain.hpp"
#include "shared/shape_file.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/crt_string.hpp"
bool __fastcall Building_Player_Detector_Covers_Cell_00438a60(HouseClass const *house, CellClass const *cell);
extern bool unknown_a40b33;
// @ref ?Building_Player_Detector_Covers_Cell_00438a60@@YI_NPBVHouseClass@@PBVCellClass@@@Z=00438a60
// @ref ?unknown_a40b33@@3_NA=00a40b33
// @ref ?Voc_Update_At@@YIPAUAudioEvent@@ABUCoordStruct@@PAUAudioEventHandle@@@Z=007141b0
// @ref ?Rules@@3PAVRulesClass@@A=00839848
// @ref ?unknown_id_source_a3d290@@3PAVUnknownIDSource@@A=00a3d290
// @ref ??RRandom2Class@@QAEHHH@Z=006388a0
// @ref ?Frame@@3HA=00a40d2c
// @ref _unknown_geometry_j=0084c6f8
// @ref __ftol=0077f110
// @ref ??_C@_06FOCG@INVISO?$AA@=007cecc0
// @ref ?Unknown_005D9760@UnknownSpeedA40B18@@QAEHH@Z=005d9760
// @ref ?unknown_speed_a40b18@@3VUnknownSpeedA40B18@@A=00a40b18

void AnimClass::AI()
{
    if (!unknown_178 && Class->StartSound != -1) {
        Voc_Update_At(Center_Coord(), &unknown_180);
    }

    if (Class->IsFlamingGuy) {
        Flaming_Guy_AI();
        ObjectClass::AI();
    }

    if (Class->PsiWarning) {
        unknown_17d = !Building_Player_Detector_Covers_Cell_00438a60((HouseClass const *)unknown_160, &Map[Center_Coord()]);
    }

    if (Class == Rules->Behind) {
        if (!unknown_a40b33) {
            unknown_17d = true;
        } else {
            unknown_17d = false;
        }
    }

    if (Class->HideIfNoOre) {
        CellClass *cell = (CellClass *)unknown_slot105();
        if (cell != 0 && cell->Get_Tiberium_Value() != 0) {
            unknown_17d = false;
        } else {
            unknown_17d = true;
        }
    }

    if (unknown_103 && unknown_104 == Stage) {
        unknown_103 = false;
    }

    if (unknown_174) {
        int bounce_result = unknown_slot116();
        if (bounce_result == 2 || bounce_result == 1) {
            bool water = Map[Position_Coord_004f3f40()].unknown_ec == 2;
            bool bridge = Position_Coord_004f3f40().Z >= Map.Get_Height_GL(Position_Coord_004f3f40()) + unknown_geometry_j;

            if (water && !bridge) {
                if (Class->IsMeteor) {
                    new AnimClass(Rules->SplashList[Rules->SplashList.Count() - 1], Coord + CoordStruct(0, 0, 3), 0, 1, 0x600, 0);
                } else {
                    new AnimClass(Rules->unknown_09c, Position_Coord_004f3f40(), 0, 1, 0x600, 0);
                    new AnimClass(Rules->SplashList[0], Coord + CoordStruct(0, 0, 3), 0, 1, 0x600, 0);
                }
            } else {
                if (Class->ExpireAnim != 0) {
                    Vector3 bouncecoord = unknown_108.Position;
                    new AnimClass(Class->ExpireAnim, CoordStruct((int)bouncecoord.X, (int)bouncecoord.Y, (int)bouncecoord.Z), 0, 1, 0x2600, -30);
                    Unknown_0047E2C0(unknown_108.Unknown_00436F80(), (int)Class->Damage, 0, Class->Warhead, 1);
                    Unknown_0047F600(unknown_108.Unknown_00436F80(), (int)Class->Damage, Class->Warhead, 0, 0);
                }
            }

            if (!water || bridge) {
                CoordStruct coord = unknown_108.Unknown_00436F80();
                if (Class->Spawns != 0 && Class->SpawnCount > 0) {
                    int first = Random_Pick(0, Class->SpawnCount);
                    int count = first + Random_Pick(0, Class->SpawnCount);
                    for (int i = 0; i < count; i++) {
                        new AnimClass(Class->Spawns, coord, 0, 1, 0x600, 0);
                    }
                }

                Rect updaterect(0, 0, 0, 0);
                if (Class->IsTiberium && !bridge) {
                    for (int x = -Class->TiberiumSpreadRadius; x <= Class->TiberiumSpreadRadius; x++) {
                        for (int y = -Class->TiberiumSpreadRadius; y <= Class->TiberiumSpreadRadius; y++) {
                            if ((int)unknown_sqrt_4ba410((double)x * (double)x + (double)y * (double)y) <= Class->TiberiumSpreadRadius) {
                                CellClass *cellptr = &Map[Cell(coord) + AdjacentCell[x & 7]];
                                if (cellptr->Can_Tiberium_Germinate(0) && Class->TiberiumSpawnType != 0) {
                                    new OverlayClass(OverlayTypes[Class->TiberiumSpawnType->ArrayIndex + Random_Pick(0, 3)], cellptr->Fetch_CellID());
                                    cellptr->OverlayData = Random_Pick(0, 2);
                                    Rect overlayrect = cellptr->Overlay_Render_Rect();
                                    overlayrect.Y -= unknown_rect_839630.Y;
                                    updaterect = Union(updaterect, overlayrect);
                                }
                            }
                        }
                    }
                    unknown_tactical_83996c->Register_Dirty_Area(updaterect, false);
                }
            }

            unknown_slot60();
            return;
        }
    }

    if (unknown_7d && !unknown_17b) {
        if (Class->TrailerAnim != 0 && (Class->TrailerSeperation == 1 || (Frame % Class->TrailerSeperation == 0))) {
            new AnimClass(Class->TrailerAnim, Center_Coord(), 1, 1, 0x600, 0);
        }
    }

    if (Rules->DropZoneAnim == Class && Map[Center_Coord()].Cell_Building()) {
        unknown_17b = true;
    }

    if (unknown_17b) {
        unknown_slot60();
        return;
    }

    if (unknown_17c) {
        unknown_17c = false;
        return;
    }

    if (LoopDelay) {
        LoopDelay--;
        if (!LoopDelay) {
            unknown_func_4226d0();
        }
    } else if (unknown_7d) {
        if (Class->IsVeins) {
            unknown_func_422f50();
        }

        if (Class->IsAnimatedTiberium) {
            int overlay = Map[Center_Coord() - CoordStruct(0x180, 0x180, 0)].OverlayIndex;
            if (overlay == -1 || OverlayTypes[overlay]->CellAnim != Class) {
                unknown_17b = true;
            }
        }

        if (Class->End == -1) {
            Class->End = ((ShapeFileStruct const *)Class->unknown_slot39())->FrameCount;
            if (Class->Shadow) {
                Class->End /= 2;
            }
        }
        if (Class->LoopEnd == -1) {
            Class->LoopEnd = Class->End;
        }

        Mark(2);

        if (!unknown_17e && !unknown_102 && Advance_Stage_When_Ready()) {
            int stage = Stage;

            if (Class->Damage > 0 && !unknown_174) {
                if (unknown_b4 != 0 && unknown_b4->Kind_Of() == 0x24) {
                    unknown_168 += Class->Damage * 5;
                } else {
                    unknown_168 += Class->Damage;
                }

                if (unknown_168 >= 1 && !unknown_178) {
                    int damage = (int)unknown_168;
                    unknown_168 -= damage;
                    if (strcmp(Class->ID, "INVISO") == 0) {
                        Unknown_0047E2C0(Center_Coord(), damage, 0, Rules->unknown_c74, 1);
                    } else {
                        Unknown_0047E2C0(Center_Coord(), damage, 0, Rules->FlameDamage2, 1);
                    }
                    if (!unknown_7d) {
                        return;
                    }
                }
            }

            if (Class->unknown_268 && Class->Start + stage == Class->unknown_268 && !unknown_174) {
                unknown_func_4228f0();
            }

            if (Class->PingPong) {
                if ((Loops <= 1 && (stage >= Class->End || stage == 0)) || (Loops > 1 && (stage >= Class->LoopEnd - Class->Start || stage == Class->Start))) {
                    Step = -Step;
                    return;
                }
            }

            if ((Loops <= 1 && stage >= Class->End) || (Loops > 1 && stage >= Class->LoopEnd - Class->Start) || (Class->Shadow && stage >= Class->LoopEnd - Class->Start) || (Class->Reverse && stage <= Class->Start)) {
                if (Loops && Loops != 0xFF) Loops--;
                if (Loops) {
                    if (Class->Reverse) {
                        Stage = Class->LoopEnd;
                    } else {
                        Stage = Class->LoopStart - Class->Start;
                    }
                    if (Class->RandomLoopDelayMin != 0 || Class->RandomLoopDelayMax != 0) {
                        LoopDelay = Random_Pick(Class->RandomLoopDelayMin, Class->RandomLoopDelayMax);
                    }
                } else {
                    if (Class->Next != 0) {
                        Class = Class->Next;

                        if (Class->End == -1) {
                            Class->End = ((ShapeFileStruct const *)Class->unknown_slot39())->FrameCount;
                            if (Class->Shadow) {
                                Class->End /= 2;
                            }
                        }
                        if (Class->LoopEnd == -1) {
                            Class->LoopEnd = Class->End;
                        }

                        unknown_17b = false;
                        Loops = Class->LoopCount;
                        unknown_168 = 0;
                        int delay = Class->Rate;
                        if (Class->RandomRateMin != 0 || Class->RandomRateMax != 0) {
                            delay = Random_Pick(Class->RandomRateMin, Class->RandomRateMax);
                        }
                        if (Class->Normalized) {
                            Set_Rate(Options.Normalize_Delay(delay));
                        } else {
                            Set_Rate(delay);
                        }
                        Stage = Class->Start;
                        unknown_func_4226d0();
                    } else {
                        unknown_159 = true;
                        unknown_slot60();
                    }
                }
            }
        }
    }
}
