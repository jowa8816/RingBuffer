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
* @file ring_test.c
* @brief Ring buffer unit testing  
*
* This source file provides support for unit testing a ring buffer.
* The basis of this file was given as part of the ring buffer
* project for ECEN 5813.   
*
* @author Jon Warriner
* @date March 14, 2019
* @version 1.0

*/

#include "ring.h"
#include <stdio.h>
#include <CUnit/Basic.h>

//#define DEBUG		//uncomment this to display detailed buffer info for each step of the tests
#define TEST_LENGTH 8	//Buffer length must be a power of 2.  

ring_t *ring = 0;

#ifdef DEBUG
// Debug function to display the ring buffer parameters and contents
void dispBuf(ring_t *r)
{
int32_t j;

    printf("\r\nLength: %d\r\n", r->Length);
    printf("Ini: %d\r\n", r->Ini);
    printf("Outi: %d\r\n", r->Outi);
    printf("Buffer: \r\n");
    for(j = 0; j < TEST_LENGTH; j++)
    {
        printf("%d\r\n", r->Buffer[j]);
    }
    printf("Entries: %d\r\n\r\n", entries(r));
}
#endif

/* The suite initialization function.
 * Nothing to do in here for now.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite1(void)
{
    return 0;
}

/* The suite cleanup function.
 * Nothing to do in here for now.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite1(void)
{
    return 0;
}

/* Simple test of init().
 * Attempt an invalid and then valid initialization of a ring buffer.
 */
void testINIT(void)
{
    // first try a length of 0.  This is invalid and should return 0
    ring = init(0);
    CU_ASSERT(0 == ring);

    // now try a valid length
    // check that we got a non-zero pointer
    ring = init(TEST_LENGTH);
    CU_ASSERT(0 != ring);

    // check that the structure members make sense after initialization
    CU_ASSERT(ring->Buffer != 0);
    CU_ASSERT(ring->Length == TEST_LENGTH);
    CU_ASSERT(ring->Ini == 0);
    CU_ASSERT(ring->Outi == 0);
}

/* Simple test of insert().
 * First, we will verify the the function correctly returns an error
 * for an invalide pointer.
 * Next, we will attempt to fill the buffer and verify each insertion.
 * Finally, we will attempt one more insertion and verify that we get
 * an error for attempting to insert into a full buffer.  
 * 
 */
void testINSERT(void)
{
int32_t i;
int32_t err;
int32_t ents;
char data = 1;

    // first, pass a NULL pointer for ring.  This should return -1
    err = insert((ring_t *)0, data);
    CU_ASSERT(err == -1);

    // attempt to fill the buffer with incrementing data
    // check the elements of the structure and number of
    // entries after each insertion.
#ifdef DEBUG
    //  Display the ring buffer parameters and contents before we start filling it.
    dispBuf(ring);
#endif  
    for(i = 0; i < TEST_LENGTH; i++)
    {
        // first insert the data and make sure no errors are returned.
        err = insert(ring, data);  
        CU_ASSERT(err == 0);

        // check the structure members that should NOT have changed
        CU_ASSERT(ring->Length == TEST_LENGTH);
        CU_ASSERT(ring->Outi == 0);

        // now check that the input elememt pointer has incremented
        CU_ASSERT(ring->Ini == (i + 1));

        // check that the entries() function works
        ents = entries(ring);
        CU_ASSERT(ents == (i + 1));

        // now check that our data got written to the buffer
        CU_ASSERT(ring->Buffer[i] == data);
        data++;

#ifdef DEBUG
        //  Display the ring buffer parameters and contents during each iteration.
        dispBuf(ring);
#endif  
    }

    // at this point we should have a full buffer
    // let's try to add one more entry.  We should get a -1
    err = insert(ring, data);  
    CU_ASSERT(err == -1);

    // check that the entries() function works
    ents = entries(ring);
    CU_ASSERT(ents == TEST_LENGTH);
}

/* Simple test of extract().
 * First, we will verify the the function correctly returns an error
 * for an invalide pointer.
 * Next, we will attempt to empty the buffer and verify each extraction.
 * Finally, we will attempt one more extraction and verify that we get
 * an error for attempting to remove from and empty buffer.  
 * 
 */
void testREMOVE(void)
{
int32_t i;
int32_t err;
int32_t ents;
char data;

    // first, pass a NULL pointer for ring.  This should return -1
    err = extract((ring_t *)0, &data);
    CU_ASSERT(err == -1);

    // attempt to empty the buffer and check the data
    // check the elements of the structure and number of
    // entries after each extraction.  
#ifdef DEBUG
    // Display the ring buffer parameters and contents before we start emptying it.
    dispBuf(ring);
#endif  
    for(i = 0; i < TEST_LENGTH; i++)
    {
        // first extract the data and make sure no errors are returned.
        err = extract(ring, &data);  
        CU_ASSERT(err == 0);

        // check the structure members that should NOT have changed
        CU_ASSERT(ring->Length == TEST_LENGTH);
        CU_ASSERT(ring->Ini == TEST_LENGTH);

        // now check that the Output elememt pointer has incremented
        CU_ASSERT(ring->Outi == (i + 1));

        // check that the entries() function works
        ents = entries(ring);
        CU_ASSERT(ents == (TEST_LENGTH - (i + 1)));

        // now check that we extracted the expected data
        // remember, we inserted incrementing data in the previous test
        CU_ASSERT(data == (i+1));

#ifdef DEBUG
        //  Display the ring buffer parameters and contents during each iteration.
        dispBuf(ring);
        printf("Data: %d\r\n", data);
#endif  
    }

    // at this point we should have an empty buffer
    // let's try extracting one more time.  We should get a -1
    err = extract(ring, &data);  
    CU_ASSERT(err == -1);

    // check that the entries() function works
    ents = entries(ring);
    CU_ASSERT(ents == 0);
}

/* Simple test of insert() with wrap-around.
 * Attempt to fill the buffer and verify each insertion.  We previously
 * filled and emptied the buffer so the first insertion should cause
 * the indices to wrap-around.  
 * Finally, we will attempt one more insertion and verify that we get
 * an error for attempting to insert into a full buffer.  
 * 
 */
void testINSERT_wrap(void)
{
int32_t i;
int32_t err;
int32_t ents;
char data = 11;

    // attempt to fill the buffer with incrementing data
    // check the elements of the structure and number of
    // entries after each insertion.
#ifdef DEBUG
    //  Display the ring buffer parameters and contents before we start filling it.
    dispBuf(ring);
#endif  
    for(i = 0; i < TEST_LENGTH; i++)
    {
        // first insert the data and make sure no errors are returned.
        err = insert(ring, data);  
        CU_ASSERT(err == 0);

        // check the structure members that should NOT have changed
        CU_ASSERT(ring->Length == TEST_LENGTH);
        CU_ASSERT(ring->Outi == ring->Length);

        // now check that the input elememt pointer has incremented
        CU_ASSERT(ring->Ini == (i + (TEST_LENGTH + 1)));

        // check that the entries() function works
        ents = entries(ring);
        CU_ASSERT(ents == (i + 1));

        // now check that our data got written to the buffer
        CU_ASSERT(ring->Buffer[i] == data);
        data++;

#ifdef DEBUG
        //  Display the ring buffer parameters and contents during each iteration.
        dispBuf(ring);
#endif  
    }

    // at this point we should have a full buffer
    // let's try to add one more entry.  We should get a -1
    err = insert(ring, data);  
    CU_ASSERT(err == -1);

    // check that the entries() function works
    ents = entries(ring);
    CU_ASSERT(ents == TEST_LENGTH);
}

/* Simple test of extract() with wrap-around.
 * We will attempt to empty the buffer and verify each extraction.  We
 * previously filled, emptied then refilled the buffer.  The first
 * extraction should cause the out index to wrap.  
 * Finally, we will attempt one more extraction and verify that we get
 * an error for attempting to remove from and empty buffer.  
 * 
 */
void testREMOVE_wrap(void)
{
int32_t i;
int32_t err;
int32_t ents;
char data;

    // attempt to empty the buffer and check the data
    // check the elements of the structure and number of
    // entries after each extraction.  
#ifdef DEBUG
    // Display the ring buffer parameters and contents before we start emptying it.
    dispBuf(ring);
#endif  
    for(i = 0; i < TEST_LENGTH; i++)
    {
        // first extract the data and make sure no errors are returned.
        err = extract(ring, &data);  
        CU_ASSERT(err == 0);

        // check the structure members that should NOT have changed
        CU_ASSERT(ring->Length == TEST_LENGTH);
        CU_ASSERT(ring->Ini == (TEST_LENGTH << 1));

        // now check that the Output elememt pointer has incremented
        CU_ASSERT(ring->Outi == (TEST_LENGTH + (i + 1)));

        // check that the entries() function works
        ents = entries(ring);
        CU_ASSERT(ents == (TEST_LENGTH - (i + 1)));

        // now check that we extracted the expected data
        // remember, we inserted incrementing data in the previous test
        CU_ASSERT(data == (i+11));

#ifdef DEBUG
        //  Display the ring buffer parameters and contents during each iteration.
        dispBuf(ring);
        printf("Data: %d\r\n", data);
#endif  
    }

    // at this point we should have an empty buffer
    // let's try extracting one more time.  We should get a -1
    err = extract(ring, &data);  
    CU_ASSERT(err == -1);

    // check that the entries() function works
    ents = entries(ring);
    CU_ASSERT(ents == 0);
}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   /* NOTE - ORDER IS IMPORTANT - MUST TEST init() THEN insert() THEN remove() THEN insert() with wrap */
   /* finally remove() with wrap.  */
   if ((NULL == CU_add_test(pSuite, "test of init()", testINIT)) ||
       (NULL == CU_add_test(pSuite, "test of insert()", testINSERT)) ||
       (NULL == CU_add_test(pSuite, "test of remove()", testREMOVE)) ||
       (NULL == CU_add_test(pSuite, "test of insert() with wrap-around", testINSERT_wrap)) ||
       (NULL == CU_add_test(pSuite, "test of remove() with wrap-around", testREMOVE_wrap))
)
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}

