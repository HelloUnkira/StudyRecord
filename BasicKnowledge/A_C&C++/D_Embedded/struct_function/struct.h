//C std lib
#include <stdint.h>

//struct define
typedef struct test_body
{
	uint8_t        	argument1;               //comment1
	uint16_t       	argument2;               //comment2
	uint32_t       	argument3;               //comment3
	uint64_t       	argument4;               //comment4
	float          	argument5;               //comment5
	double         	argument6;               //comment6

} test;

//set method
static inline void test_set_argument1(test *instance, uint8_t argument1)
{
	instance->argument1 = argument1;
}

static inline void test_set_argument2(test *instance, uint16_t argument2)
{
	instance->argument2 = argument2;
}

static inline void test_set_argument3(test *instance, uint32_t argument3)
{
	instance->argument3 = argument3;
}

static inline void test_set_argument4(test *instance, uint64_t argument4)
{
	instance->argument4 = argument4;
}

static inline void test_set_argument5(test *instance, float argument5)
{
	instance->argument5 = argument5;
}

static inline void test_set_argument6(test *instance, double argument6)
{
	instance->argument6 = argument6;
}
//get method
static inline uint8_t test_get_argument1(test *instance)
{
	return instance->argument1;
}

static inline uint16_t test_get_argument2(test *instance)
{
	return instance->argument2;
}

static inline uint32_t test_get_argument3(test *instance)
{
	return instance->argument3;
}

static inline uint64_t test_get_argument4(test *instance)
{
	return instance->argument4;
}

static inline float test_get_argument5(test *instance)
{
	return instance->argument5;
}

static inline double test_get_argument6(test *instance)
{
	return instance->argument6;
}
