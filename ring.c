/*****************************************************************************
* Copyright (C) 2019 by Jon Warriner
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Jon Warriner and the University of Colorado are not liable for
* any misuse of this material.
*
*****************************************************************************/
/**
* @file ring.c
* @brief ring buffer functions  
*
* This source file provides support for manipulating a ring buffer.
* The basis of this file was given as part of the ring buffer
* project for ECEN 5813.   
*
* @author Jon Warriner
* @date March 14, 2019
* @version 1.0

*/

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

    // create the buffer and initialize the structure
    pbuf = malloc(length);
    r = (ring_t *)malloc(sizeof(ring_t));
    if((pbuf != 0) && (r != 0))
    {
        r->Buffer = pbuf;
        r->Length = length;
        r->Ini = 0;
        r->Outi = 0;        
        return(r);
    }
    else
    {
        return 0;
    }

}

int32_t insert( ring_t *ring, char data )
{
    if(ring == 0)
    {
        return(-1);  //invalid pointer
    }
    else if( ring->Ini - ring->Outi < ring->Length ) 
    {
        ring->Buffer[ring->Ini++ & (ring->Length - 1)] = data;
        return 0;
    }
    else
    {
        return(-1);  //buffer is full
    }
}

int32_t extract( ring_t *ring, char *data )
{
    if(ring == 0)
    {
        return(-1);  //invalid pointer
    }
    else if( ring->Outi != ring->Ini )
    {
        *data = ring->Buffer[ring->Outi++ & (ring->Length - 1)];
        return 0;
    }
    else
    {
        return(-1);  //buffer is empty
    }
}

int32_t entries( ring_t *ring )
{
    if(ring == 0)
    {
        return(-1);  //invalid pointer
    }
    else
    {
        return((ring->Ini - ring->Outi));
    }
}

