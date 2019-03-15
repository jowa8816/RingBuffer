/*========================================================================
** ring.h
** Circular buffer
** ECEN5813
**========================================================================*/
#ifndef RING_H
#define RING_H

#include <stdint.h>

typedef struct
{
    char *Buffer;
    int32_t Length;
    int32_t Ini;
    int32_t Outi;
}ring_t;

ring_t *init( int32_t length );
int32_t insert( ring_t *ring, char data );
int32_t remove( ring_t *ring, char *data );
int32_t entries( ring_t *ring );

#endif
