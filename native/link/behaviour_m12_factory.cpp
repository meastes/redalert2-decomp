// Behaviour rows (Scout1111, native M12; NOT EXACT, native/behaviour_rewrites.json):
// FactoryClass's Kind_Of/Size_Of slots (leaves 0x004B9F20/0x004B9F30) and
// the four FactoryClass leaves the sidebar's factory stand-in (shared/sidebar.hpp
// UnknownFactory) calls and no accepted source defines yet. Written from the
// seed decompiles; each reads the real FactoryClass (shared/factory_class_decl.hpp)
// at its own arm64 layout. Reached once the skirmish harness builds a power plant:
// SidebarClass::StripClass::AI 0x0067F050 polls a building strip's factory.
#define RA2_VEIN_STAGE_ACCESSORS   // StageClass::Fetch_Stage (shared/stage.hpp)
#include "shared/factory_class_decl.hpp"

class UnknownFactory {
public:
    bool Unknown_004B9420();
    bool Unknown_004B9430();
    int Unknown_004B98F0();
    int Unknown_004B9940();
};

// 0x004B9420: the factory's +0x58 flag.
bool UnknownFactory::Unknown_004B9420()
{
    return ((FactoryClass *)(void *)this)->unknown_58;
}

// 0x004B9430: the +0x59 flag (the production just completed; hypothesis), cleared on reading.
bool UnknownFactory::Unknown_004B9430()
{
    FactoryClass *factory = (FactoryClass *)(void *)this;
    bool value = factory->unknown_59;
    factory->unknown_59 = false;
    return value;
}

// 0x004B98F0: the build stage (StageClass's value, VC6 +0x24).
int UnknownFactory::Unknown_004B98F0()
{
    return ((FactoryClass *)(void *)this)->Fetch_Stage();
}

// 0x004B9940: the special item being prepared (+0x64), or -1.
int UnknownFactory::Unknown_004B9940()
{
    return ((FactoryClass *)(void *)this)->unknown_64;
}


