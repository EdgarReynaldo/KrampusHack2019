



#ifndef BinStream_HPP
#define BinStream_HPP

extern const unsigned int endian_value;
extern const bool big_endian;
extern const bool little_endian;

#include <vector>
#include <cstring>

class BStream {
   
   std::vector<unsigned char> bytes;
   
   void PushData(const void* dat , unsigned int sz , bool reverse);
   
public :

   template <class T>
   BStream& operator<<(const T& t);
   template <class T>
   BStream& operator<<(const T* t);
};






template <class T>
BStream& BStream::operator<<(const T& t) {
   this->PushData((const void*)t , sizeof(t) , big_endian);
   return *this;
}



template <class T>
BStream& BStream::operator<<(const T* t) {
   (*this) << *t;
   return *this;
}



template <>
BStream& BStream::operator<<(const char* str) {
   this->PushData((const void*)str , strlen(str) + 1 , false);
   return *this;
}




#endif // BinStream_HPP
