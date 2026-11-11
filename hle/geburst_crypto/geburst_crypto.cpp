#include "portablekit/hle_extension.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>

namespace ext = portablekit::hle_extension;

namespace {
constexpr std::array<std::uint32_t,64> shifts={
7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21};
constexpr std::array<std::uint32_t,64> constants={
0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,0xa679438e,0x49b40821,
0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391};
struct Context { std::uint32_t h[4]{0x67452301,0xefcdab89,0x98badcfe,0x10325476}; std::uint64_t total{}; std::uint8_t buf[64]{}; };
std::uint32_t rol(std::uint32_t x,unsigned n){return (x<<n)|(x>>(32-n));}
void transform(Context &c,const std::uint8_t *p){
 std::uint32_t w[16];for(unsigned i=0;i<16;i++)w[i]=std::uint32_t(p[i*4])|(std::uint32_t(p[i*4+1])<<8)|(std::uint32_t(p[i*4+2])<<16)|(std::uint32_t(p[i*4+3])<<24);
 auto a=c.h[0],b=c.h[1],d=c.h[3],cc=c.h[2];
 for(unsigned i=0;i<64;i++){std::uint32_t f,g;if(i<16){f=(b&cc)|(~b&d);g=i;}else if(i<32){f=(d&b)|(~d&cc);g=(5*i+1)%16;}else if(i<48){f=b^cc^d;g=(3*i+5)%16;}else{f=cc^(b|~d);g=(7*i)%16;}auto t=d;d=cc;cc=b;b+=rol(a+f+constants[i]+w[g],shifts[i]);a=t;}
 c.h[0]+=a;c.h[1]+=b;c.h[2]+=cc;c.h[3]+=d;
}
void update(Context &c,const std::uint8_t *p,std::size_t n){
 auto used=static_cast<std::size_t>(c.total&63);c.total+=n;
 if(used){auto take=std::min(n,64-used);std::memcpy(c.buf+used,p,take);used+=take;p+=take;n-=take;if(used==64)transform(c,c.buf);}
 while(n>=64){transform(c,p);p+=64;n-=64;}if(n)std::memcpy(c.buf,p,n);
}
void digest(Context c,std::uint8_t out[16]){
 const auto bits=c.total*8;std::uint8_t pad[128]{};pad[0]=0x80;
 const auto n=(c.total&63)<56?56-(c.total&63):120-(c.total&63);
 for(unsigned i=0;i<8;i++)pad[n+i]=static_cast<std::uint8_t>(bits>>(i*8));update(c,pad,n+8);
 for(unsigned i=0;i<4;i++)for(unsigned j=0;j<4;j++)out[i*4+j]=static_cast<std::uint8_t>(c.h[i]>>(j*8));
}
bool load(ext::GuestMemory &m,std::uint32_t a,Context &c){
 if(!a||!m.contains(a,96))return false;for(unsigned i=0;i<4;i++)c.h[i]=m.load32(a+i*4);
 c.total=std::uint64_t(m.load32(a+24))|(std::uint64_t(m.load32(a+28))<<32);
 for(unsigned i=0;i<64;i++)c.buf[i]=m.load8(a+32+i);return true;
}
void save(ext::GuestMemory &m,std::uint32_t a,const Context &c){
 for(unsigned i=0;i<4;i++)m.store32(a+i*4,c.h[i]);m.store16(a+20,c.total&63);m.store16(a+22,0);
 m.store32(a+24,static_cast<std::uint32_t>(c.total));m.store32(a+28,static_cast<std::uint32_t>(c.total>>32));
 for(unsigned i=0;i<64;i++)m.store8(a+32+i,c.buf[i]);
}
void run_digest(ext::Runtime &rt,std::uint32_t p,std::uint32_t n,std::uint32_t out,ext::AllegrexContext &ctx){
 auto &m=rt.memory();if(!m.contains(p,n)||!m.contains(out,16)){ext::finish(ctx,0xffffffffu);return;}
 Context c;std::array<std::uint8_t,4096> chunk{};std::uint32_t done=0;
 while(done<n){auto count=std::min<std::uint32_t>(n-done,chunk.size());for(std::uint32_t i=0;i<count;i++)chunk[i]=m.load8(p+done+i);update(c,chunk.data(),count);done+=count;}
 std::uint8_t outbuf[16];digest(c,outbuf);for(unsigned i=0;i<16;i++)m.store8(out+i,outbuf[i]);ext::finish(ctx,0);
}
}

PORTABLEKIT_HLE_EXTENSION(geburst_crypto) {
 registry.add("sceMd5",0x19884A15,"sceMd5BlockInit",[](ext::Runtime &rt,ext::AllegrexContext &ctx){
  auto &m=rt.memory();auto a=ext::arg(ctx,0);if(!a||!m.contains(a,96)){ext::finish(ctx,0xffffffffu);return;}
  Context c;for(unsigned i=0;i<4;i++)m.store32(a+i*4,c.h[i]);m.store16(a+20,0);m.store16(a+22,0);m.store32(a+24,0);m.store32(a+28,0);ext::finish(ctx,0);
 });
 registry.add("sceMd5",0xA30206C2,"sceMd5BlockUpdate",[](ext::Runtime &rt,ext::AllegrexContext &ctx){
  auto &m=rt.memory();auto a=ext::arg(ctx,0),p=ext::arg(ctx,1),n=ext::arg(ctx,2);Context c;
  if(!load(m,a,c)||!m.contains(p,n)){ext::finish(ctx,0xffffffffu);return;}
  const auto before=c.total;std::array<std::uint8_t,4096> chunk{};std::uint32_t done=0;
  while(done<n){auto count=std::min<std::uint32_t>(n-done,chunk.size());for(std::uint32_t i=0;i<count;i++)chunk[i]=m.load8(p+done+i);update(c,chunk.data(),count);done+=count;}
  if(n){auto count=std::min<std::uint32_t>(n,64);auto start=n-count;for(std::uint32_t i=0;i<count;i++)m.store8(a+32+static_cast<std::uint32_t>((before+start+i)&63),m.load8(p+start+i));}
  save(m,a,c);ext::finish(ctx,0);
 });
 registry.add("sceMd5",0x4876AFFF,"sceMd5BlockResult",[](ext::Runtime &rt,ext::AllegrexContext &ctx){
  auto &m=rt.memory();auto a=ext::arg(ctx,0),o=ext::arg(ctx,1);Context c;if(!load(m,a,c)||!m.contains(o,16)){ext::finish(ctx,0xffffffffu);return;}
  std::uint8_t out[16];digest(c,out);for(unsigned i=0;i<16;i++)m.store8(o+i,out[i]);save(m,a,c);ext::finish(ctx,0);
 });
 registry.add("sceMd5",0x98E31A9E,"sceMd5Digest",[](ext::Runtime &rt,ext::AllegrexContext &ctx){run_digest(rt,ext::arg(ctx,0),ext::arg(ctx,1),ext::arg(ctx,2),ctx);});
}
