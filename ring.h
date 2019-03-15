/*========================================================================
** ring.h
** Circular buffer
** ECEN5813
**========================================================================*/
#ifndef RING_H
#define RING_H

ring_t *init( int length );
int insert( ring_t *ring, char data );
int remove( ring_t *ring, char *data );
int entries( ring_t *ring );

#endif