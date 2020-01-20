// right now, just some wrapper code for random.
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "pi-test.h"
#include "pi-random.h"

#if USE_BAD_RANDOM

unsigned long rpi_rand32(void);
unsigned long fake_random(void) {
    return rpi_rand32();
}

void fake_random_seed(unsigned x) {
}

// check that we get what we expect.
static void fake_random_check(void) {
    // make stdout always flush.
    setvbuf(stdout, NULL, _IONBF, 0);

    // make sure that everyone has the same random.
    assert(1450224440 == fake_random());
}

#else 

static int init_p = 0;

#define STATESIZE 128
static int seed = 0;
static char statebuf[STATESIZE];
static struct pi_random_data r;

void fake_random_seed(unsigned x) {
    init_p = 1;

    memset(&r, 0, sizeof r);
    if(pi_initstate_r(seed, statebuf, STATESIZE, &r))
        assert(0);
    if(pi_srandom_r(seed, &r))
        assert(0);
}

// make sure that everyone has the same random.
static void fake_random_check(void) {
    // make stdout always flush.
    setvbuf(stdout, NULL, _IONBF, 0);

    fake_random_seed(0);
    assert(0x6b8b4567 == fake_random());
    // assert(1450224440 == fake_random());
}

unsigned long fake_random(void) {
    assert(init_p);
    int x;
    if(pi_random_r(&r, &x))
        assert(0);
    return x;
}
#endif

void fake_pi_init(void) {
    fake_random_check();
}
