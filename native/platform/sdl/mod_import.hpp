// SPDX-License-Identifier: GPL-3.0-or-later
// Mods in the app (lane C16, mod support level 3): platform/sdl/mod_import.cpp.
#pragma once
#include <string>

namespace mods {
// Unpacks each <name>.zip in mods_dir into mods_dir/<name> unless that folder exists (Apple
// platforms; zlib). The .zip is left where it is. Returns how many were unpacked.
int import_zips(const std::string &mods_dir);
// The mod's mod.ini [Mod] Name= and Description= (each left as it is when absent); false without one.
bool manifest(const std::string &mod_dir, std::string *name, std::string *description);
}
