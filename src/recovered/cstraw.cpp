// Westwood cstraw.cpp in one source (twins: mischa85/RedAlert2 src/cstraw.cpp, TibSun cstraw.cpp).
// Parameter names from mischa85/RedAlert2 392e904 src/cstraw.cpp where both define the function (names only).

#include "shared/cache_straw.hpp"
#include "shared/crt_memory.hpp"


int CacheStraw::Get(void *source,int slen)
{
 int total=0;
 if(BufferPtr.Is_Valid() && source && slen>0) {
  do {
   if(Length>0) {
    int count=Length<slen ? Length : slen;
    memmove(source,(char *)BufferPtr.Get_Buffer()+Index,count);
    slen-=count;
    Index+=count;
    total+=count;
    Length-=count;
    source=(char *)source+count;
   }
   if(slen==0) break;
   Length=Straw::Get(BufferPtr.Get_Buffer(),BufferPtr.Get_Size());
   Index=0;
  } while(Length && slen>0);
 }
 return total;
}
