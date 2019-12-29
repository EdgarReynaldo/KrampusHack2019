



#include "BinStream.hpp"
#include "Eagle/Exception.hpp"


const unsigned int endian_value = ('B' << 24 | 'E' << 16 | 'E' << 8 | 'L');
const bool big_endian = endian_value & 0x4245454C;
const bool little_endian = !big_endian;




void BinStream::PushData(const void* dat , unsigned int sz , bool reverse) {
   EAGLE_ASSERT(sz);
   if (!sz) {return;}


   bytes.reserve(bytes.size() + sz);
   if (!reverse) {
      memcpy(&bytes[bytes.size()] , dat , sz);
      return;
   }

   unsigned char* rdat = (unsigned char*)dat + ((int)sz - 1);
   for (unsigned int i = 0 ; i < sz ; ++i) {
      bytes.push_back(*(rdat--));
   }
}



