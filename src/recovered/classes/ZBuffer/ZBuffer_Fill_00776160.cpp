#include "shared/blit.hpp"
void ZBuffer::Fill(unsigned short color) { Surface->Fill_Rect(Rect(0,0,Width,Height),color); }
