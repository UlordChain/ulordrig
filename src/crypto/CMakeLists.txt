cmake_minimum_required(VERSION 2.8)

set(HEADERS_CRYPTO
    aes128.h
    blake2_impl.h
    blake2s.h
    blake2s256.h
    camellia128.h
    common.h
    crc32.h
    des.h
    gost.h
    haval5_256.h
    hmac_md5.h
    jtr_crc32.h
    jtr_gost.h
    jtr_sph_haval.h
    jtr_sph_skein.h
    jtr_sph_types.h
    keccak1600.h
    my_rand48_r.h
    my_time.h
    oneWayFunction.h
    PoW.h
    rc4.h
    ripemd160.h
    sha1.h
    sha3_256.h
    sha256.h
    sha512.h
    skein512_256.h
    whirlpool.h
	cryptoHello.h
   )

set(SOURCES_CRYPTO
    aes128.c
    blake2s.c
    blake2s256.c
    camellia128.c
    common.c
    crc32.c
    des.c
    gost.c
    keccak1600.c
    haval5_256.c
    hmac_md5.c
    jtr_crc32.c
    jtr_gost.c
    jtr_haval.c
    jtr_skein.c
    my_time.c
    oneWayFunction.c
    PoW.c
    rc4.c
    ripemd160.c
    sha1.c
    sha3_256.c
    sha256.c
    sha512.c
    skein512_256.c
    whirlpool.c
 #   jtr_haval_helper.c
	cryptoHello.cpp
   )

#include_directories(/usr/include)
include_directories(..)
link_directories("/usr/lib/x86_64-linux-gnu")
find_library(RUNTIME_LIB ssl /usr/lib/x86_64-linux-gnu NO_DEFAULT_PATH)
find_library(RUNTIME_LIB crypto /usr/lib/x86_64-linux-gnu NO_DEFAULT_PATH)
find_library(RUNTIME_LIB gomp /usr/lib/gcc/x86_64-linux-gnu/5.4.0 NO_DEFAULT_PATH)
link_libraries(${RUNTIME_LIB})


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb -fopenmp")

add_library(cryptoHello ${HEADERS_CRYPTO} ${SOURCES_CRYPTO}) 
target_link_libraries(cryptoHello ssl crypto gomp)

#add_executable(PoW main.cpp ${HEADERS_CRYPTO} ${SOURCES_CRYPTO})
#target_link_libraries(PoW ssl crypto gomp)
