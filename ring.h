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
* @file ring.h
* @brief An abstraction for the ring buffer functions
*
* This header file provides an abstraction of the functions to
* manipulate a ring buffer. The basis of this file was given as
* part of the ring buffer project for ECEN 5813.  
*
* @author Jon Warriner
* @date March 14 2019
* @version 1.0
*
*/

#ifndef RING_H
#define RING_H

#include <stdint.h>

/**
* define the ring buffer structured data type
*/
typedef struct
{
    char *Buffer;
    int32_t Length;
    int32_t Ini;
    int32_t Outi;
}ring_t;

/**
* @brief Create a new ring buffer of "length" chars
*
* Given a length (in chars) return a pointer to a new
* ring_t type.  Return 0 on failure.  
*
* @param length Length of buffer in chars
*
* @return pointer to ring_t type or 0 on failure
*/
ring_t *init( int32_t length );

/**
* @brief Insert a new char into the buffer
*
* Given a pointer to an existing ring buffer and a piece of
* data, insert the new data into the buffer.  
*
* @param ring_t Pointer to an already initialized ring buffer
* @param data   New data to add to the buffer
*
* @return 0 on success, -1 on failure
*/
int32_t insert( ring_t *ring, char data );

/**
* @brief Extract (remove) the next char from the buffer
*
* Given a pointer to an existing ring buffer and a pointer to
* a data location, extract the oldest piece of data from the
* buffer.  
*
* @param ring_t Pointer to an already initialized ring buffer
* @param data   Pointer to a location to write the extracted data
*
* @return 0 on success, -1 on failure
*/
int32_t extract( ring_t *ring, char *data );

/**
* @brief Return the number of entries in the buffer
*
* Given a pointer to an existing ring buffer return the number
* of entries in that buffer.    
*
* @param ring_t Pointer to an already initialized ring buffer
*
* @return number of buffer entries, returns -1 on error
*/
int32_t entries( ring_t *ring );

#endif
