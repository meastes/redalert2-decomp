#define RA2_COORD_TPOINT3D  // TibSun Coord profile for the Techno unit (shared/coord.hpp, shared/selection_coord.hpp; 0x006C2C40, 0x006C3950)
#define RA2_COORD_CELL_BASE_INIT
#define RA2_TPOINT3D_DIVIDE  // TibSun TPoint3D::operator/ (0x006D9770 is its out-of-line copy here)
#define RA2_TPOINT3D_DIFFERENCE  // TibSun TPoint3D::operator- (In_Range 0x006C4BB0)
#define RA2_PARTICLE_SYSTEM_POINT3D  // ParticleSystemClass takes the point sum unconverted (AI 0x006C74C0)
#define RA2_COORD_AS_CELL  // TibSun member Cell comparisons (shared/coord.hpp; Get_Z_Fudge_Tunnel 0x006CFB60)
#define RA2_DIR_AS_FACING  // DirType::As_Facing and DIR_N/DIR_W (shared/dir_type.hpp; 0x006CFB60)
#define RA2_CDTIMER_CONVERSION_VALUE_BODY
#define RA2_POINT2D_SCALE_OPERATOR
#define RA2_CDTIMER_BASIC_BASE  // Westwood BasicTimerClass base (shared/cd_timer.hpp; TechnoClass(NoInitClass) 0x006C1F10)
#define RA2_STAGE_PLAIN_TIMER  // StageClass timer as a plain CDTimerClass (shared/stage.hpp; 0x006C1F10)
// Surface::Surface(int, int) (0x0049ED90)
// Inline constructor defined in the shared header (surface.hpp).
// No source of its own: emitted by this unit (TechnoClass::Blit_Voxel 0x006D29D0 calls it out of line).
