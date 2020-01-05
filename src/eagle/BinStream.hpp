



#ifndef BinStream_HPP
#define BinStream_HPP

extern const unsigned int endian_value;
extern const bool big_endian;
extern const bool little_endian;

#include <vector>
#include <string>
#include <cstring>

class BinStream {
   
   std::vector<unsigned char> bytes;
   
   void PushData(const void* dat , unsigned int sz , bool reverse);
   
public :

   const void* Data() const {return &bytes[0];}
   unsigned int Size() const {return bytes.size();}

   template <class T>
   BinStream& operator<<(const T* t);

   template <class T>
   BinStream& operator<<(const T& t);

};



template <class T>
BinStream& BinStream::operator<<(const T* t) {
   (*this) << *t;
   return *this;
}



template <class T>
BinStream& BinStream::operator<<(const T& t) {
   this->PushData((const void*)t , sizeof(t) , big_endian);
   return *this;
}

template <>
BinStream& BinStream::operator<<(const char* str);

template <>
BinStream& BinStream::operator<<(const std::string& str);















#endif // BinStream_HPP
