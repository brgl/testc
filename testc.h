/*
 * Copyright (C) 2013 Bartosz Golaszewski <bartekgola@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef __TESTC__
#define __TESTC__

#define TESTC_PRINTF_FUNC(FORMAT, PARAMS)				\
		__attribute__((format(printf, FORMAT, PARAMS)))

#define TESTC_ATSTART_FIRST __attribute__((constructor(1000)))
#define TESTC_ATSTART_LAST __attribute__((constructor(2000)))

#define TESTC_UNUSED __attribute__((unused))

void testc_print(const char* fmt, ...) TESTC_PRINTF_FUNC(1, 2);
void testc_printerr(const char* fmt, ...) TESTC_PRINTF_FUNC(1, 2);

typedef int (*testc_testfunc)(void);

struct testc_listelem
{
	struct testc_listelem* next;
	const char* name;
	testc_testfunc testfunc;
};

void testc_registertest(struct testc_listelem* test);

#define TESTC_DEFINE_TEST(NAME)						\
	static int __##NAME##_test(void);				\
	static struct testc_listelem __##NAME##_elem = {		\
		.name = #NAME,						\
		.testfunc = __##NAME##_test,				\
	};								\
	static void TESTC_ATSTART_LAST __##NAME##_register(void)	\
	{								\
		testc_registertest(&__##NAME##_elem);			\
	}								\
	static int __##NAME##_test(void)

#define TESTC_BEGINTEST int __test_retval = 0

#define TESTC_ENDTEST							\
	do {								\
		return __test_retval;					\
	} while (0)

/*
 * Both 'goto __finally' and 'break' are here only to get rid
 * of compiler warnings.
 */
#define TESTC_FINALLY							\
	do {								\
		goto __finally;						\
		__finally:						\
			break;						\
	} while (0)

#define TESTC_PRINTASSERTFAIL						\
	do {								\
		testc_printerr(						\
			"Assertion failed in file %s, line %d!",	\
			__FILE__, __LINE__);				\
	} while (0)

#define TESTC_ASSERTION_FAILED						\
	do {								\
		__test_retval = -1;					\
		goto __finally;						\
	} while (0)

/*
 * Assertions.
 *
 * For now we only offer assertions which cause tests to fail
 * immediately. In the future 'expects' might be added too -
 * similar to those offered by the gtest framework.
 */

#define TESTC_ASSERT_EQ(EXPECTED, ACTUAL)				\
	do {								\
		if ((EXPECTED) != (ACTUAL)) {				\
			TESTC_PRINTASSERTFAIL;				\
			testc_printerr("'%s' isn't equal to '%s'",	\
						#EXPECTED, #ACTUAL);	\
			TESTC_ASSERTION_FAILED;				\
		}							\
	} while (0)

#define TESTC_ASSERT_NOTEQ(EXPECTED, ACTUAL)				\
	do {								\
		if ((EXPECTED) == (ACTUAL)) {				\
			TESTC_PRINTASSERTFAIL;				\
			testc_printerr("'%s' is equal to '%s'",		\
						#EXPECTED, #ACTUAL);	\
			TESTC_ASSERTION_FAILED;				\
		}							\
	} while (0)

#define TESTC_ASSERT_NOTNULL(PTR)					\
	do {								\
		if ((PTR) == NULL) {					\
			TESTC_PRINTASSERTFAIL;				\
			testc_printerr("'%s' is NULL!", #PTR);		\
			TESTC_ASSERTION_FAILED;				\
		}							\
	} while (0)

#define TESTC_ASSERT_NULL(PTR)						\
	do {								\
		if ((PTR) != NULL) {					\
			TESTC_PRINTASSERTFAIL;				\
			testc_printerr("'%s' is not NULL!", #PTR);	\
			TESTC_ASSERTION_FAILED;				\
		}							\
	} while (0)

#define TESTC_ASSERT_FALSE(STATEMENT)					\
	do {								\
		if ((STATEMENT)) {					\
			TESTC_PRINTASSERTFAIL;				\
			testc_printerr(					\
				"Statement '%s' evaluated to true!",	\
				#STATEMENT);				\
			TESTC_ASSERTION_FAILED;				\
		}							\
	} while (0)

#define TESTC_ASSERT_TRUE(STATEMENT)					\
	do {								\
		if (!(STATEMENT)) {					\
			TESTC_PRINTASSERTFAIL;				\
			testc_printerr(					\
				"Statement '%s' evaluated to false!",	\
				#STATEMENT);				\
			TESTC_ASSERTION_FAILED;				\
		}							\
	} while (0)

#define TESTC_ASSERT_STREQ(STR1, STR2)					\
	do {								\
		if (strcmp(STR1, STR2) != 0) {				\
			TESTC_PRINTASSERTFAIL;				\
			testc_printerr(					\
				"Strings '%s' and '%s' "		\
				"are not the same", STR1, STR2);	\
			TESTC_ASSERTION_FAILED;				\
		}							\
	} while (0)

#define TESTC_ASSERT_STRNOTEQ(STR1, STR2)				\
	do {								\
		if (strcmp(STR1, STR2) == 0) {				\
			TESTC_PRINTASSERTFAIL;				\
			testc_printerr(					\
				"Strings '%s' and '%s' "		\
				"are the same, but were "		\
				"expected to differ", STR1, STR2);	\
			TESTC_ASSERTION_FAILED;				\
		}							\
	} while (0)

#endif /* __TESTC__ */

