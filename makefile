#*****************************************************************************
# Copyright (C) 2019 by Jon Warriner
#
# Redistribution, modification or use of this software in source or binary
# forms is permitted as long as the files maintain this copyright. Users are
# permitted to modify this and use it to learn about the field of embedded
# software. Jon Warriner and the University of Colorado are not liable for
# any misuse of this material.
#
#****************************************************************************
#
# @file makefile
# @brief makefile for the ring buffer test project
#
# This file defines the make steps to build the
# ring buffer test project.  
#
# @author Jon Warriner
# @date March 14, 2019
# @version 1.0
#
ringtest : ring_test.o ring.o
	cc -o ringtest ring_test.o ring.o -lcunit

ring_test.o : ring_test.c ring.h
	cc -Wall -Werror -c ring_test.c

ring.o : ring.c ring.h
	cc -Wall -Werror -c ring.c

clean :
	rm ringtest ring_test.o ring.o
