



#include "BinStream.hpp"
#include "Eagle/Exception.hpp"


const unsigned int endian_value = ('B' << 24 | 'E' << 16 | 'E' << 8 | 'L');
const bool big_endian = endian_value == 0x4245454C;
const bool little_endian = !big_endian;




void BinStream::PushData(const void* dat , unsigned int sz , bool reverse) {
   EAGLE_ASSERT(sz);
   if (!sz) {return;}

   const unsigned int end = bytes.size();
   bytes.resize(bytes.size() + sz);
   if (!reverse) {
      memcpy(&bytes[end] , dat , sz);
      return;
   }

   unsigned char* rdat = (unsigned char*)dat + ((int)sz - 1);
   for (unsigned int i = 0 ; i < sz ; ++i) {
      bytes[end + i] = *(rdat--);
   }
}



template <>
BinStream& BinStream::operator<<(const char* str) {
   this->PushData((const void*)str , strlen(str) + 1 , false);
   return *this;
}



template <>
BinStream& BinStream::operator<<(const std::string& str) {
   (*this) << str.c_str();
   return *this;
}



