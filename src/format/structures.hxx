/*
 * Author : Mirza Saadat Baig, <me@saadat.dev>
 * Date   : 07.03.21
 * License: BSD-3-Clause-Attribution
 */
#ifndef PKGX_STRUCTURES_HXX
#define PKGX_STRUCTURES_HXX

// --------------------*  imports  *--------------------
#include <cstdint>


struct __attribute__((packed)) PKGX_HEADER {
    uint32_t magic;
    uint32_t control_sz;
    uint32_t layout_sz;
    uint32_t data_sz;
};
typedef struct PKGX_HEADER pkgx_header_t;


#endif //PKGX_STRUCTURES_HXX