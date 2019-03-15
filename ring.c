/*========================================================================
** Circular buffer
** ECEN5813
**========================================================================*/
#include "ring.h"
#include <stdlib.h>

ring_t *init( int32_t length )
{
char *pbuf = 0;
ring_t *r = 0;

    // make sure the requested length makes sense
    if(length <= 0)
    {
        return 0;
    }

    // create the buffer
    pbuf = malloc(length);
    if(pbuf)
    {
        r = (ring_t *)malloc(sizeof(ring_t));
        return(r);
    }
    else
    {
        return 0;
    }

}

int32_t insert( ring_t *ring, char data )
{
    if( ring->Ini - ring->Outi < ring->Length ) 
    {
        ring->Buffer[ring->Ini++ & ring->Length] = data;
    }

    return 0;
}

int32_t remove( ring_t *ring, char *data )
{
    if( ring->Outi != ring->Ini )
    {
        *data = ring->Buffer[ring->Outi++ & ring->Length];
    }

    return 0;
}

int32_t entries( ring_t *ring )
{
    return 0;
}

