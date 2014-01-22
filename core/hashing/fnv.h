/* This is a variation of the fnv header file found at 
 * http://isthe.com/chongo/src/fnv/fnv.h
 * I have changed things to suite my needs, however all props go to them.
 */

#pragma once

static const Fnv32_t FNV0_32_INIT = (Fnv32_t)0;
static const Fnv32_t FNV1_32_INIT = (Fnv32_t)0x811c9dc5;
static const Fnv32_t FNV1_32A_INIT = FNV1_32_INIT;

static const Fnv64_t FNV0_64_INIT = (Fnv64_t)0;
static const Fnv64_t FNV1_64_INIT = (Fnv64_t)0xcbf29ce484222325ULL;
static const Fnv64_t FNV1_64A_INIT = FNV1_64_INIT;

/*
 * hash types
 */
enum fnv_type {
    FNV_NONE = 0, /* invalid FNV hash type */
    FNV0_32 = 1,  /* FNV-0 32 bit hash */
    FNV1_32 = 2,  /* FNV-1 32 bit hash */
    FNV1a_32 = 3, /* FNV-1a 32 bit hash */
    FNV0_64 = 4,  /* FNV-0 64 bit hash */
    FNV1_64 = 5,  /* FNV-1 64 bit hash */
    FNV1a_64 = 6, /* FNV-1a 64 bit hash */
};


/*
 * these test vectors are used as part o the FNV test suite
 */
struct test_vector {
    void *buf;    /* start of test vector buffer */
    int len;    /* length of test vector */
};
struct fnv0_32_test_vector {
    struct test_vector *test; /* test vector buffer to hash */
    Fnv32_t fnv0_32;    /* expected FNV-0 32 bit hash value */
};
struct fnv1_32_test_vector {
    struct test_vector *test; /* test vector buffer to hash */
    Fnv32_t fnv1_32;    /* expected FNV-1 32 bit hash value */
};
struct fnv1a_32_test_vector {
    struct test_vector *test; /* test vector buffer to hash */
    Fnv32_t fnv1a_32;   /* expected FNV-1a 32 bit hash value */
};
struct fnv0_64_test_vector {
    struct test_vector *test; /* test vector buffer to hash */
    Fnv64_t fnv0_64;    /* expected FNV-0 64 bit hash value */
};
struct fnv1_64_test_vector {
    struct test_vector *test; /* test vector buffer to hash */
    Fnv64_t fnv1_64;    /* expected FNV-1 64 bit hash value */
};
struct fnv1a_64_test_vector {
    struct test_vector *test; /* test vector buffer to hash */
    Fnv64_t fnv1a_64;   /* expected FNV-1a 64 bit hash value */
};


/*
 * external functions
 */
/* hash_32a.c */
extern Fnv32_t fnv_32a_buf(void *buf, size_t len, Fnv32_t hashval = FNV1_32A_INIT);
extern Fnv32_t fnv_32a_str(const char_t *buf, Fnv32_t hashval = FNV1_32A_INIT);

/* hash_64a.c */
extern Fnv64_t fnv_64a_buf(void *buf, size_t len, Fnv64_t hashval = FNV1_64A_INIT);
extern Fnv64_t fnv_64a_str(const char_t *buf, Fnv64_t hashval = FNV1_64A_INIT);
