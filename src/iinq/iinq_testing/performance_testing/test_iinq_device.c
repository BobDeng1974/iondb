/******************************************************************************/
/**
@file		test_iinq_device.c
@author		IonDB Project
@brief		Iinq tests to test scalability.
@copyright	Copyright 2017
			The University of British Columbia,
			IonDB Project Contributors (see AUTHORS.md)
@par Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

@par 1.Redistributions of source code must retain the above copyright notice,
	this list of conditions and the following disclaimer.

@par 2.Redistributions in binary form must reproduce the above copyright notice,
	this list of conditions and the following disclaimer in the documentation
	and/or other materials provided with the distribution.

@par 3.Neither the name of the copyright holder nor the names of its contributors
	may be used to endorse or promote products derived from this software without
	specific prior written permission.

@par THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
	ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
	LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
	ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
*/
/******************************************************************************/

#include "test_iinq_device.h"

#define MIN_RECORDS							100
#define MAX_RECORDS							1000000
#define RECORD_INCREMENT					100

#define OUTPUT_QUERY_RESULTS				0
#define OUTPUT_TIMES						1
#define OUTPUT_SQL_STATEMENTS				1
#define OUTPUT_INSERT_PROGRESS				0
#define OUTPUT_INSERT_PROGRESS_FREQUENCY	100

#define EXECUTE_DICTIONARY_DIRECTLY			1

ion_boolean_t skip_order_by = boolean_false;

void
test_create_table1(
	planck_unit_test_t *tc
) {
#if OUTPUT_SQL_STATEMENTS
	printf("CREATE TABLE Table1 (ID INT, CharValue VARCHAR(30), IntValue INT, primary key(ID));\n");
#endif

	volatile unsigned long start_time, end_time;

	start_time = ion_time();

	ion_err_t error =
/*		  SQL_execute("CREATE TABLE Table1 (ID INT, CharValue VARCHAR(30), IntValue INT, primary key(ID));"); */
		create_table(0, key_type_numeric_signed, sizeof(int), IINQ_BITS_FOR_NULL(3) + (sizeof(int) * 2) + (sizeof(char) * 31));

	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, err_ok, error);

	ion_dictionary_t			dictionary;
	ion_dictionary_handler_t	handler;

	end_time = ion_time();
#if OUTPUT_TIMES
	printf("Create Table time taken: %lu\n", end_time - start_time);
#endif

	iinq_open_source(0, &dictionary, &handler);
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, key_type_numeric_signed, dictionary.instance->key_type);
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, sizeof(int), dictionary.instance->record.key_size);
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, (sizeof(int) * 2) + (sizeof(char) * 31) + IINQ_BITS_FOR_NULL(3), dictionary.instance->record.value_size);
	PLANCK_UNIT_ASSERT_TRUE(tc, iinq_is_key_field(0, 1));
	PLANCK_UNIT_ASSERT_FALSE(tc, iinq_is_key_field(0, 2));
	PLANCK_UNIT_ASSERT_FALSE(tc, iinq_is_key_field(0, 3));
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, iinq_int, iinq_get_field_type(0, 1));
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, iinq_null_terminated_string, iinq_get_field_type(0, 2));
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, iinq_int, iinq_get_field_type(0, 3));
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, IINQ_BITS_FOR_NULL(3), iinq_calculate_offset(0, 1));
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, IINQ_BITS_FOR_NULL(3) + sizeof(int), iinq_calculate_offset(0, 2));
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, IINQ_BITS_FOR_NULL(3) + sizeof(int) + sizeof(char) * 31, iinq_calculate_offset(0, 3));
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, IINQ_BITS_FOR_NULL(3) + sizeof(int) * 2 + sizeof(char) * 31, iinq_calculate_offset(0, 4));
	ion_close_dictionary(&dictionary);
}

void
test_insert_records_table1(
	planck_unit_test_t	*tc,
	int					num_records
) {
	intmax_t				i, n;
	volatile unsigned long	start_time, end_time;

	start_time = ion_time();

	for (i = 0; i < num_records; i++) {
		iinq_execute_instantaneous(iinq_insert_0(IONIZE(i, int), "regInsert", IONIZE(-i, int)));
#if OUTPUT_INSERT_PROGRESS

		if ((i % OUTPUT_INSERT_PROGRESS_FREQUENCY == 0) || (i == num_records - 1))
#if defined(ARDUINO)
		{
			printf("Inserting regular (%d): %d%%\n", num_records, (i + 1) * 100 / num_records);
		}

#else
		{
			printf("Inserting regular (%d): %d%%\r", num_records, (i + 1) * 100 / num_records);
		}
		fflush(stdout);
#endif
#endif
	}

	printf("\n");

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("%d records inserted. Time taken: %lu\n\n", num_records, end_time - start_time);
#endif
}

void
test_insert_records_prep_table1(
	planck_unit_test_t	*tc,
	int					num_records
) {
	uintmax_t				i;
	volatile unsigned long	start_time, end_time;

	start_time = ion_time();

/*  iinq_prepared_sql *p = SQL_prepare("INSERT INTO Table1 VALUES (?, ?, ?);"); */
	iinq_prepared_sql *p = iinq_insert_0(NULL, NULL, NULL);

	for (i = 0; i < num_records; i++) {
		iinq_set_param(p, 1, IONIZE(i, int));
		iinq_set_param(p, 2, "prepInsert");
		iinq_set_param(p, 3, IONIZE(-i, int));

		iinq_execute_prepared(p);
#if OUTPUT_INSERT_PROGRESS

		if ((i % OUTPUT_INSERT_PROGRESS_FREQUENCY == 0) || (i == num_records - 1))
#if defined(ARDUINO)
		{
			printf("Inserting prepared (%d): %d%%\n", num_records, (i + 1) * 100 / num_records);
		}

#else
		{
			printf("Inserting prepared (%d): %d%%\r", num_records, (i + 1) * 100 / num_records);
		}
		fflush(stdout);
#endif
#endif
	}

	iinq_close_statement(p);

	end_time = ion_time();

	printf("\n");

#if OUTPUT_TIMES
	printf("%d prepared records inserted. Time taken: %lu\n\n", num_records, end_time - start_time);
#endif
}

void
test_select_all_records_table1(
	planck_unit_test_t *tc
) {
	volatile unsigned long start_time, end_time;

#if OUTPUT_SQL_STATEMENTS
	printf("SELECT * FROM Table1\n");
#endif

	start_time = ion_time();

/*  iinq_result_set_t *rs1 = SQL_select("SELECT * FROM Table1;"); */
	iinq_result_set_t *rs1 = iinq_init_result_set(iinq_projection_init(iinq_dictionary_init(0, 3, predicate_all_records), 3, IINQ_PROJECTION_LIST(1, 2, 3)));

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Init time taken: %lu\n", end_time - start_time);
#endif

	if ((NULL == rs1) || (err_ok != rs1->status.error)) {
		PLANCK_UNIT_SET_FAIL(tc);
	}

	start_time = ion_time();

	while (iinq_next(rs1)) {
#if OUTPUT_QUERY_RESULTS
		printf("ID: %i, ", *iinq_get_int(rs1, 1));
		printf("CharValue: %s, ", iinq_get_string(rs1, 2));
		printf("IntValue: %d\n", *iinq_get_int(rs1, 3));
#endif
	}

#if OUTPUT_QUERY_RESULTS
	printf("\n");
#endif

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Iteration time taken: %lu\n", end_time - start_time);
#endif

	start_time = ion_time();

	iinq_close_result_set(rs1);

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Closing time taken: %lu\n", end_time - start_time);
#endif

	PLANCK_UNIT_ASSERT_TRUE(tc, NULL == rs1);

#if EXECUTE_DICTIONARY_DIRECTLY
	start_time = ion_time();

	ion_dictionary_t			dictionary;
	ion_dictionary_handler_t	handler;

	ion_init_master_table();
	ion_open_dictionary(&handler, &dictionary, 1);

	ion_dict_cursor_t	*cursor = NULL;
	ion_predicate_t		predicate;

	dictionary_build_predicate(&predicate, predicate_all_records);
	dictionary_find(&dictionary, &predicate, &cursor);

	ion_record_t record;

	record.key		= malloc(dictionary.instance->record.key_size);
	record.value	= malloc(dictionary.instance->record.value_size);

	while (cs_cursor_active == cursor->next(cursor, &record) || cs_cursor_initialized == cursor->status) {
#if OUTPUT_QUERY_RESULTS
		printf("ID: %i, ", NEUTRALIZE((char *) record.value + iinq_calculate_offset(0, 1), int));
		printf("CharValue: %s, ", (char *) record.value + iinq_calculate_offset(0, 2));
		printf("IntValue: %d\n", NEUTRALIZE((char *) record.value + iinq_calculate_offset(0, 3), int));
#endif
	}

	end_time = ion_time();
	printf("Direct dictionary time: %lu\n", end_time - start_time);
	free(record.key);
	free(record.value);
	cursor->destroy(&cursor);
	ion_close_dictionary(&dictionary);
	end_time = ion_time();
#endif

	printf("\n");
}

void
test_select_all_where_equal_int_table1(
	planck_unit_test_t *tc
) {
	volatile unsigned long start_time, end_time;

#if OUTPUT_SQL_STATEMENTS
	printf("SELECT * FROM Table1 WHERE IntValue = 50\n");
#endif

	start_time = ion_time();

/*  iinq_result_set_t *rs1 = SQL_select("SELECT * FROM Table1 WHERE IntValue = 50;"); */
	iinq_result_set_t *rs1 = iinq_init_result_set(iinq_projection_init(iinq_selection_init(iinq_dictionary_init(0, 3, predicate_all_records), 1, IINQ_CONDITION_LIST(IINQ_CONDITION(3, iinq_equal, IONIZE(50, int)))), 3, IINQ_PROJECTION_LIST(1, 2, 3)));

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Init time taken: %lu\n", end_time - start_time);
#endif

	if ((NULL == rs1) || (err_ok != rs1->status.error)) {
		PLANCK_UNIT_SET_FAIL(tc);
	}

	start_time = ion_time();

	while (iinq_next(rs1)) {
#if OUTPUT_QUERY_RESULTS
		printf("ID: %i, ", *iinq_get_int(rs1, 1));
		printf("CharValue: %s, ", iinq_get_string(rs1, 2));
		printf("IntValue: %d\n", *iinq_get_int(rs1, 3));
#endif
	}

#if OUTPUT_QUERY_RESULTS
	printf("\n");
#endif

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Iteration time taken: %lu\n", end_time - start_time);
#endif

	start_time = ion_time();

	iinq_close_result_set(rs1);

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Closing time taken: %lu\n\n", end_time - start_time);
#endif

	PLANCK_UNIT_ASSERT_TRUE(tc, NULL == rs1);
}

void
test_select_all_where_equal_int_key_table1(
	planck_unit_test_t *tc
) {
	volatile unsigned long start_time, end_time;

#if OUTPUT_SQL_STATEMENTS
	printf("SELECT * FROM Table1 WHERE ID = 50\n");
#endif

	start_time = ion_time();

/*  iinq_result_set_t *rs1 = SQL_select("SELECT * FROM Table1 WHERE ID = 50;"); */
	iinq_result_set_t *rs1 = iinq_init_result_set(iinq_projection_init(iinq_dictionary_init(0, 3, predicate_equality, IONIZE(50, int)), 3, IINQ_PROJECTION_LIST(1, 2, 3)));

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Init time taken: %lu\n", end_time - start_time);
#endif

	if ((NULL == rs1) || (err_ok != rs1->status.error)) {
		PLANCK_UNIT_SET_FAIL(tc);
	}

	start_time = ion_time();

	while (iinq_next(rs1)) {
#if OUTPUT_QUERY_RESULTS
		printf("ID: %i, ", *iinq_get_int(rs1, 1));
		printf("CharValue: %s, ", iinq_get_string(rs1, 2));
		printf("IntValue: %d\n", *iinq_get_int(rs1, 3));
#endif
	}

#if OUTPUT_QUERY_RESULTS
	printf("\n");
#endif

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Iteration time taken: %lu\n", end_time - start_time);
#endif

	start_time = ion_time();

	iinq_close_result_set(rs1);

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Closing time taken: %lu\n", end_time - start_time);
#endif

	PLANCK_UNIT_ASSERT_TRUE(tc, NULL == rs1);

#if EXECUTE_DICTIONARY_DIRECTLY
	start_time = ion_time();

	ion_dictionary_t			dictionary;
	ion_dictionary_handler_t	handler;

	ion_init_master_table();
	ion_open_dictionary(&handler, &dictionary, 1);

	ion_dict_cursor_t	*cursor = NULL;
	ion_predicate_t		predicate;

	dictionary_build_predicate(&predicate, predicate_equality, IONIZE(50, int));
	dictionary_find(&dictionary, &predicate, &cursor);

	ion_record_t record;

	record.key		= malloc(dictionary.instance->record.key_size);
	record.value	= malloc(dictionary.instance->record.value_size);

	while (cs_cursor_active == cursor->next(cursor, &record) || cs_cursor_initialized == cursor->status) {
#if OUTPUT_QUERY_RESULTS
		printf("ID: %i, ", NEUTRALIZE((char *) record.value + iinq_calculate_offset(0, 1), int));
		printf("CharValue: %s, ", (char *) record.value + iinq_calculate_offset(0, 2));
		printf("IntValue: %d\n", NEUTRALIZE((char *) record.value + iinq_calculate_offset(0, 3), int));
#endif
	}

	end_time = ion_time();
	printf("Direct dictionary time: %lu\n", end_time - start_time);
	free(record.key);
	free(record.value);
	cursor->destroy(&cursor);
	ion_close_dictionary(&dictionary);
	end_time = ion_time();
#endif

	printf("\n");
}

void
test_select_all_where_range_int_table1(
	planck_unit_test_t *tc
) {
	volatile unsigned long start_time, end_time;

#if OUTPUT_SQL_STATEMENTS
	printf("SELECT * FROM Table1 WHERE IntValue >= 25 AND IntValue <= 75\n");
#endif

	start_time = ion_time();

/*  iinq_result_set_t *rs1 = SQL_select("SELECT * FROM Table1 WHERE IntValue >= 25 AND IntValue <= 75;"); */
	iinq_result_set_t *rs1 = iinq_init_result_set(iinq_projection_init(iinq_selection_init(iinq_dictionary_init(0, 3, predicate_all_records), 2, IINQ_CONDITION_LIST(IINQ_CONDITION(3, iinq_less_than_equal_to, IONIZE(75, int)), IINQ_CONDITION(3, iinq_greater_than_equal_to, IONIZE(25, int)))), 3, IINQ_PROJECTION_LIST(1, 2, 3)));

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Init time taken: %lu\n", end_time - start_time);
#endif

	if ((NULL == rs1) || (err_ok != rs1->status.error)) {
		PLANCK_UNIT_SET_FAIL(tc);
	}

	start_time = ion_time();

	while (iinq_next(rs1)) {
#if OUTPUT_QUERY_RESULTS
		printf("ID: %i, ", *iinq_get_int(rs1, 1));
		printf("CharValue: %s, ", iinq_get_string(rs1, 2));
		printf("IntValue: %d\n", *iinq_get_int(rs1, 3));
#endif
	}

#if OUTPUT_QUERY_RESULTS
	printf("\n");
#endif

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Iteration time taken: %lu\n", end_time - start_time);
#endif

	start_time = ion_time();

	iinq_close_result_set(rs1);

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Closing time taken: %lu\n\n", end_time - start_time);
#endif

	PLANCK_UNIT_ASSERT_TRUE(tc, NULL == rs1);
}

void
test_select_all_where_range_int_key_table1(
	planck_unit_test_t *tc
) {
	volatile unsigned long start_time, end_time;

#if OUTPUT_SQL_STATEMENTS
	printf("SELECT * FROM Table1 WHERE ID >= 25 AND ID <= 75\n");
#endif

	start_time = ion_time();

/*  iinq_result_set_t *rs1 = SQL_select("SELECT * FROM Table1 WHERE ID >= 25 AND ID <= 75;"); */
	iinq_result_set_t *rs1 = iinq_init_result_set(iinq_projection_init(iinq_dictionary_init(0, 3, predicate_range, IONIZE(75, int), IONIZE(25, int)), 3, IINQ_PROJECTION_LIST(1, 2, 3)));

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Init time taken: %lu\n", end_time - start_time);
#endif

	if ((NULL == rs1) || (err_ok != rs1->status.error)) {
		PLANCK_UNIT_SET_FAIL(tc);
	}

	start_time = ion_time();

	while (iinq_next(rs1)) {
#if OUTPUT_QUERY_RESULTS
		printf("ID: %i, ", *iinq_get_int(rs1, 1));
		printf("CharValue: %s, ", iinq_get_string(rs1, 2));
		printf("IntValue: %d\n", *iinq_get_int(rs1, 3));
#endif
	}

#if OUTPUT_QUERY_RESULTS
	printf("\n");
#endif

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Iteration time taken: %lu\n", end_time - start_time);
#endif

	start_time = ion_time();

	iinq_close_result_set(rs1);

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Closing time taken: %lu\n", end_time - start_time);
#endif

	PLANCK_UNIT_ASSERT_TRUE(tc, NULL == rs1);

#if EXECUTE_DICTIONARY_DIRECTLY
	start_time = ion_time();

	ion_dictionary_t			dictionary;
	ion_dictionary_handler_t	handler;

	ion_init_master_table();
	ion_open_dictionary(&handler, &dictionary, 1);

	ion_dict_cursor_t	*cursor = NULL;
	ion_predicate_t		predicate;

	dictionary_build_predicate(&predicate, predicate_range, IONIZE(25, int), IONIZE(75, int));
	dictionary_find(&dictionary, &predicate, &cursor);

	ion_record_t record;

	record.key		= malloc(dictionary.instance->record.key_size);
	record.value	= malloc(dictionary.instance->record.value_size);

	while (cs_cursor_active == cursor->next(cursor, &record) || cs_cursor_initialized == cursor->status) {
#if OUTPUT_QUERY_RESULTS
		printf("ID: %i, ", NEUTRALIZE((char *) record.value + iinq_calculate_offset(0, 1), int));
		printf("CharValue: %s, ", (char *) record.value + iinq_calculate_offset(0, 2));
		printf("IntValue: %d\n", NEUTRALIZE((char *) record.value + iinq_calculate_offset(0, 3), int));
#endif
	}

	end_time = ion_time();
#if OUTPUT_TIMES
	printf("Direct dictionary time: %lu\n", end_time - start_time);
#endif
	free(record.key);
	free(record.value);
	cursor->destroy(&cursor);
	ion_close_dictionary(&dictionary);
	end_time = ion_time();
#endif

	printf("\n");
}

void
test_select_field_list_table1(
	planck_unit_test_t *tc
) {
	volatile unsigned long start_time, end_time;

#if OUTPUT_SQL_STATEMENTS
	printf("SELECT IntValue, ID FROM Table1\n");
#endif

	start_time = ion_time();

/*  iinq_result_set_t *rs1 = SQL_select("SELECT IntValue, ID FROM Table1;"); */
	iinq_result_set_t *rs1 = iinq_init_result_set(iinq_projection_init(iinq_dictionary_init(0, 3, predicate_all_records), 2, IINQ_PROJECTION_LIST(3, 1)));

	end_time = ion_time();
#if OUTPUT_TIMES
	printf("Init time taken: %lu\n", end_time - start_time);
#endif

	if ((NULL == rs1) || (err_ok != rs1->status.error)) {
		PLANCK_UNIT_SET_FAIL(tc);
	}

	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, 2, rs1->head->instance->num_fields);

	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, 3, rs1->head->instance->field_info[0].field_num);
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, 1, rs1->head->instance->field_info[1].field_num);

	start_time = ion_time();

	while (iinq_next(rs1)) {
#if OUTPUT_QUERY_RESULTS
		printf("IntValue: %d\n", *iinq_get_int(rs1, 1));
		printf("ID: %i, ", *iinq_get_int(rs1, 2));
#endif
	}

#if OUTPUT_QUERY_RESULTS
	printf("\n");
#endif

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Iteration time taken: %lu\n", end_time - start_time);
#endif

	start_time = ion_time();

	iinq_close_result_set(rs1);

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Closing time taken: %lu\n\n", end_time - start_time);
#endif

	PLANCK_UNIT_ASSERT_TRUE(tc, NULL == rs1);
}

void
test_select_all_where_str_equal_table1(
	planck_unit_test_t *tc
) {
	volatile unsigned long start_time, end_time;

#if OUTPUT_SQL_STATEMENTS
	printf("SELECT * FROM Table1 WHERE CharValue = 'prepInsert'\n");
#endif

	start_time = ion_time();

/*  iinq_result_set_t *rs1 = SQL_select("SELECT * FROM Table1 WHERE CharValue = 'prepInsert';"); */
	iinq_result_set_t *rs1 = iinq_init_result_set(iinq_projection_init(iinq_selection_init(iinq_dictionary_init(0, 3, predicate_all_records), 1, IINQ_CONDITION_LIST(IINQ_CONDITION(2, iinq_equal, "prepInsert"))), 3, IINQ_PROJECTION_LIST(1, 2, 3)));

	end_time = ion_time();
#if OUTPUT_TIMES
	printf("Init time taken: %lu\n", end_time - start_time);
#endif

	if ((NULL == rs1) || (err_ok != rs1->status.error)) {
		PLANCK_UNIT_SET_FAIL(tc);
	}

	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, 3, rs1->head->instance->num_fields);

	start_time = ion_time();

	while (iinq_next(rs1)) {
#if OUTPUT_QUERY_RESULTS
		printf("ID: %i, ", *iinq_get_int(rs1, 1));
		printf("CharValue: %s, ", iinq_get_string(rs1, 2));
		printf("IntValue: %d\n", *iinq_get_int(rs1, 3));
#endif
	}

#if OUTPUT_QUERY_RESULTS
	printf("\n");
#endif

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Iteration time taken: %lu\n", end_time - start_time);
#endif

	start_time = ion_time();

	iinq_close_result_set(rs1);

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Closing time taken: %lu\n\n", end_time - start_time);
#endif

	PLANCK_UNIT_ASSERT_TRUE(tc, NULL == rs1);
}

void
test_select_all_where_str_not_equal_table1(
	planck_unit_test_t *tc
) {
	volatile unsigned long start_time, end_time;

#if OUTPUT_SQL_STATEMENTS
	printf("SELECT * FROM Table1 WHERE CharValue <> 'prepInsert'\n");
#endif

	start_time = ion_time();

/*  iinq_result_set_t *rs1 = SQL_select("SELECT * FROM Table1 WHERE CharValue <> 'prepInsert';"); */
	iinq_result_set_t *rs1 = iinq_init_result_set(iinq_projection_init(iinq_selection_init(iinq_dictionary_init(0, 3, predicate_all_records), 1, IINQ_CONDITION_LIST(IINQ_CONDITION(2, iinq_not_equal, "prepInsert"))), 3, IINQ_PROJECTION_LIST(1, 2, 3)));

	end_time = ion_time();
#if OUTPUT_TIMES
	printf("Init time taken: %lu\n", end_time - start_time);
#endif

	if ((NULL == rs1) || (err_ok != rs1->status.error)) {
		PLANCK_UNIT_SET_FAIL(tc);
	}

	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, 3, rs1->head->instance->num_fields);

	start_time = ion_time();

	while (iinq_next(rs1)) {
#if OUTPUT_QUERY_RESULTS
		printf("ID: %i, ", *iinq_get_int(rs1, 1));
		printf("CharValue: %s, ", iinq_get_string(rs1, 2));
		printf("IntValue: %d\n", *iinq_get_int(rs1, 3));
#endif
	}

#if OUTPUT_QUERY_RESULTS
	printf("\n");
#endif

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Iteration time taken: %lu\n", end_time - start_time);
#endif

	start_time = ion_time();

	iinq_close_result_set(rs1);

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Closing time taken: %lu\n\n", end_time - start_time);
#endif

	PLANCK_UNIT_ASSERT_TRUE(tc, NULL == rs1);
}

void
test_select_all_where_int_key_less_than_table1(
	planck_unit_test_t *tc
) {
	volatile unsigned long start_time, end_time;

#if OUTPUT_SQL_STATEMENTS
	printf("SELECT * FROM Table1 WHERE IntValue < 50\n");
#endif

	start_time = ion_time();

/*  iinq_result_set_t *rs1 = SQL_select("SELECT * FROM Table1 WHERE IntValue < 50;"); */
	iinq_result_set_t *rs1 = iinq_init_result_set(iinq_projection_init(iinq_selection_init(iinq_dictionary_init(0, 3, predicate_all_records), 1, IINQ_CONDITION_LIST(IINQ_CONDITION(3, iinq_less_than, IONIZE(50, int)))), 3, IINQ_PROJECTION_LIST(1, 2, 3)));

	end_time = ion_time();
#if OUTPUT_TIMES
	printf("Init time taken: %lu\n", end_time - start_time);
#endif

	if ((NULL == rs1) || (err_ok != rs1->status.error)) {
		PLANCK_UNIT_SET_FAIL(tc);
	}

	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, 3, rs1->head->instance->num_fields);

	start_time = ion_time();

	while (iinq_next(rs1)) {
#if OUTPUT_QUERY_RESULTS
		printf("ID: %i, ", *iinq_get_int(rs1, 1));
		printf("CharValue: %s, ", iinq_get_string(rs1, 2));
		printf("IntValue: %d\n", *iinq_get_int(rs1, 3));
#endif
	}

#if OUTPUT_QUERY_RESULTS
	printf("\n");
#endif

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Iteration time taken: %lu\n", end_time - start_time);
#endif

	start_time = ion_time();

	iinq_close_result_set(rs1);

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Closing time taken: %lu\n\n", end_time - start_time);
#endif

	PLANCK_UNIT_ASSERT_TRUE(tc, NULL == rs1);
}

void
test_select_all_where_int_key_less_than__equal_table1(
	planck_unit_test_t *tc
) {
	volatile unsigned long start_time, end_time;

#if OUTPUT_SQL_STATEMENTS
	printf("SELECT * FROM Table1 WHERE IntValue <= 50\n");
#endif

	start_time = ion_time();

/*  iinq_result_set_t *rs1 = SQL_select("SELECT * FROM Table1 WHERE IntValue <= 50;"); */
	iinq_result_set_t *rs1 = iinq_init_result_set(iinq_projection_init(iinq_selection_init(iinq_dictionary_init(0, 3, predicate_all_records), 1, IINQ_CONDITION_LIST(IINQ_CONDITION(3, iinq_less_than_equal_to, IONIZE(50, int)))), 3, IINQ_PROJECTION_LIST(1, 2, 3)));

	end_time = ion_time();
#if OUTPUT_TIMES
	printf("Init time taken: %lu\n", end_time - start_time);
#endif

	if ((NULL == rs1) || (err_ok != rs1->status.error)) {
		PLANCK_UNIT_SET_FAIL(tc);
	}

	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, 3, rs1->head->instance->num_fields);

	start_time = ion_time();

	while (iinq_next(rs1)) {
#if OUTPUT_QUERY_RESULTS
		printf("ID: %i, ", *iinq_get_int(rs1, 1));
		printf("CharValue: %s, ", iinq_get_string(rs1, 2));
		printf("IntValue: %d\n", *iinq_get_int(rs1, 3));
#endif
	}

#if OUTPUT_QUERY_RESULTS
	printf("\n");
#endif

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Iteration time taken: %lu\n", end_time - start_time);
#endif

	start_time = ion_time();

	iinq_close_result_set(rs1);

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Closing time taken: %lu\n\n", end_time - start_time);
#endif

	PLANCK_UNIT_ASSERT_TRUE(tc, NULL == rs1);
}

void
test_select_all_where_int_key_greater_than_table1(
	planck_unit_test_t *tc
) {
	volatile unsigned long start_time, end_time;

#if OUTPUT_SQL_STATEMENTS
	printf("SELECT * FROM Table1 WHERE IntValue > 50\n");
#endif

	start_time = ion_time();

/*  iinq_result_set_t *rs1 = SQL_select("SELECT * FROM Table1 WHERE IntValue > 50;"); */
	iinq_result_set_t *rs1 = iinq_init_result_set(iinq_projection_init(iinq_selection_init(iinq_dictionary_init(0, 3, predicate_all_records), 1, IINQ_CONDITION_LIST(IINQ_CONDITION(3, iinq_greater_than, IONIZE(50, int)))), 3, IINQ_PROJECTION_LIST(1, 2, 3)));

	end_time = ion_time();
#if OUTPUT_TIMES
	printf("Init time taken: %lu\n", end_time - start_time);
#endif

	if ((NULL == rs1) || (err_ok != rs1->status.error)) {
		PLANCK_UNIT_SET_FAIL(tc);
	}

	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, 3, rs1->head->instance->num_fields);

	start_time = ion_time();

	while (iinq_next(rs1)) {
#if OUTPUT_QUERY_RESULTS
		printf("ID: %i, ", *iinq_get_int(rs1, 1));
		printf("CharValue: %s, ", iinq_get_string(rs1, 2));
		printf("IntValue: %d\n", *iinq_get_int(rs1, 3));
#endif
	}

#if OUTPUT_QUERY_RESULTS
	printf("\n");
#endif

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Iteration time taken: %lu\n", end_time - start_time);
#endif

	start_time = ion_time();

	iinq_close_result_set(rs1);

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Closing time taken: %lu\n\n", end_time - start_time);
#endif

	PLANCK_UNIT_ASSERT_TRUE(tc, NULL == rs1);
}

void
test_select_all_where_int_key_greater_than__equal_table1(
	planck_unit_test_t *tc
) {
	volatile unsigned long start_time, end_time;

#if OUTPUT_SQL_STATEMENTS
	printf("SELECT * FROM Table1 WHERE IntValue >= 50\n");
#endif

	start_time = ion_time();

/*  iinq_result_set_t *rs1 = SQL_select("SELECT * FROM Table1 WHERE IntValue >= 50;"); */
	iinq_result_set_t *rs1 = iinq_init_result_set(iinq_projection_init(iinq_selection_init(iinq_dictionary_init(0, 3, predicate_all_records), 1, IINQ_CONDITION_LIST(IINQ_CONDITION(3, iinq_greater_than_equal_to, IONIZE(50, int)))), 3, IINQ_PROJECTION_LIST(1, 2, 3)));

	end_time = ion_time();
#if OUTPUT_TIMES
	printf("Init time taken: %lu\n", end_time - start_time);
#endif

	if ((NULL == rs1) || (err_ok != rs1->status.error)) {
		PLANCK_UNIT_SET_FAIL(tc);
	}

	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, 3, rs1->head->instance->num_fields);

	start_time = ion_time();

	while (iinq_next(rs1)) {
#if OUTPUT_QUERY_RESULTS
		printf("ID: %i, ", *iinq_get_int(rs1, 1));
		printf("CharValue: %s, ", iinq_get_string(rs1, 2));
		printf("IntValue: %d\n", *iinq_get_int(rs1, 3));
#endif
	}

#if OUTPUT_QUERY_RESULTS
	printf("\n");
#endif

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Iteration time taken: %lu\n", end_time - start_time);
#endif

	start_time = ion_time();

	iinq_close_result_set(rs1);

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Closing time taken: %lu\n\n", end_time - start_time);
#endif

	PLANCK_UNIT_ASSERT_TRUE(tc, NULL == rs1);
}

void
test_select_all_where_int_key_not__equal_table1(
	planck_unit_test_t *tc
) {
	volatile unsigned long start_time, end_time;

#if OUTPUT_SQL_STATEMENTS
	printf("SELECT * FROM Table1 WHERE IntValue <> 50\n");
#endif

	start_time = ion_time();

/*  iinq_result_set_t *rs1 = SQL_select("SELECT * FROM Table1 WHERE IntValue <> 50;"); */
	iinq_result_set_t *rs1 = iinq_init_result_set(iinq_projection_init(iinq_selection_init(iinq_dictionary_init(0, 3, predicate_all_records), 1, IINQ_CONDITION_LIST(IINQ_CONDITION(3, iinq_not_equal, IONIZE(50, int)))), 3, IINQ_PROJECTION_LIST(1, 2, 3)));

	end_time = ion_time();
#if OUTPUT_TIMES
	printf("Init time taken: %lu\n", end_time - start_time);
#endif

	if ((NULL == rs1) || (err_ok != rs1->status.error)) {
		PLANCK_UNIT_SET_FAIL(tc);
	}

	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, 3, rs1->head->instance->num_fields);

	start_time = ion_time();

	while (iinq_next(rs1)) {
#if OUTPUT_QUERY_RESULTS
		printf("ID: %i, ", *iinq_get_int(rs1, 1));
		printf("CharValue: %s, ", iinq_get_string(rs1, 2));
		printf("IntValue: %d\n", *iinq_get_int(rs1, 3));
#endif
	}

#if OUTPUT_QUERY_RESULTS
	printf("\n");
#endif

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Iteration time taken: %lu\n", end_time - start_time);
#endif

	start_time = ion_time();

	iinq_close_result_set(rs1);

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Closing time taken: %lu\n\n", end_time - start_time);
#endif

	PLANCK_UNIT_ASSERT_TRUE(tc, NULL == rs1);
}

void
test_select_all_where_int_order_by_asc_table1(
	planck_unit_test_t *tc
) {
	volatile unsigned long start_time, end_time, total_time;

#if OUTPUT_SQL_STATEMENTS
	printf("SELECT * FROM Table1 ORDER BY ID ASC\n");
#endif

	start_time = ion_time();

/*  iinq_result_set_t *rs1 = SQL_select("SELECT * FROM Table1 ORDER BY ID ASC;"); */
	iinq_result_set_t *rs1 = iinq_init_result_set(iinq_projection_init(iinq_external_sort_init(iinq_projection_init(iinq_dictionary_init(0, 3, predicate_all_records), 3, IINQ_PROJECTION_LIST(1, 2, 3)), 1, IINQ_ORDER_BY_LIST(IINQ_ORDER_BY(1, IINQ_ASC))), 3, IINQ_PROJECTION_LIST(1, 2, 3)));

	end_time = ion_time();
#if OUTPUT_TIMES
	printf("Init time taken: %lu\n", end_time - start_time);
#endif

	if ((NULL == rs1) || (err_ok != rs1->status.error)) {
		PLANCK_UNIT_SET_FAIL(tc);
	}

	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, 3, rs1->head->instance->num_fields);

	start_time = ion_time();

	while (iinq_next(rs1)) {
#if OUTPUT_QUERY_RESULTS
		printf("ID: %i, ", *iinq_get_int(rs1, 1));
		printf("CharValue: %s, ", iinq_get_string(rs1, 2));
		printf("IntValue: %d\n", *iinq_get_int(rs1, 3));
#endif
	}

#if OUTPUT_QUERY_RESULTS
	printf("\n");
#endif

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Iteration time taken: %lu\n", end_time - start_time);
#endif

	/* Skip order by for the rest of the tests if it takes longer than 20 minutes */
	if (end_time - start_time > 1200000) {
		skip_order_by = boolean_true;
	}

	start_time = ion_time();

	iinq_close_result_set(rs1);

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Closing time taken: %lu\n", end_time - start_time);
#endif

	PLANCK_UNIT_ASSERT_TRUE(tc, NULL == rs1);

#if EXECUTE_DICTIONARY_DIRECTLY

	ion_dictionary_t			dictionary;
	ion_dictionary_handler_t	handler;

	ion_init_master_table();
	ion_open_dictionary(&handler, &dictionary, 1);

	ion_dict_cursor_t	*cursor = NULL;
	ion_predicate_t		predicate;

	dictionary_build_predicate(&predicate, predicate_all_records);
	dictionary_find(&dictionary, &predicate, &cursor);

	ion_record_t record;

	record.key = malloc(dictionary.instance->record.key_size);

	ion_value_size_t value_size = dictionary.instance->record.value_size;

	record.value	= malloc(value_size);

	start_time		= ion_time();

	iinq_order_part_t orderby_order_part;

	orderby_order_part.pointer		= (char *) record.value + iinq_calculate_offset(0, 1);
	orderby_order_part.direction	= _ASCENDING_INDICATOR;
	orderby_order_part.size			= iinq_calculate_offset(0, 2) - iinq_calculate_offset(0, 1);

	switch (iinq_get_field_type(0, 1)) {
		case iinq_int:
			orderby_order_part.type = IINQ_ORDERTYPE_INT;
			break;

		case iinq_unsigned_int:
			orderby_order_part.type = IINQ_ORDERTYPE_UINT;
			break;

		case iinq_float:
			orderby_order_part.type = IINQ_ORDERTYPE_FLOAT;
			break;

		case iinq_null_terminated_string:
		case iinq_char_array:
		default:
			orderby_order_part.type = IINQ_ORDERTYPE_OTHER;
			break;
	}

	FILE *file					= fopen("orderby", "wb");

	int write_page_remaining	= IINQ_PAGE_SIZE;

	while (cs_cursor_active == cursor->next(cursor, &record) || cs_cursor_initialized == cursor->status) {
		if (write_page_remaining < (orderby_order_part.size + IINQ_BITS_FOR_NULL(3) + value_size)) {
			char x = 0;

			int i;

			for (i = 0; i < write_page_remaining; i++) {
				if (1 != fwrite(&x, 1, 1, file)) {
					break;
				}
			}

			write_page_remaining = IINQ_PAGE_SIZE;
		}

		if (1 != fwrite(orderby_order_part.pointer, orderby_order_part.size, 1, file)) {
			break;
		}
		else {
			write_page_remaining -= orderby_order_part.size;
		}

		if (1 != fwrite(record.value, IINQ_BITS_FOR_NULL(3), 1, file)) {
			break;
		}
		else {
			write_page_remaining -= IINQ_BITS_FOR_NULL(3);
		}

		int j;

		for (j = 1; j <= 3; j++) {
			size_t field_size = iinq_calculate_offset(0, j + 1) - iinq_calculate_offset(0, j);

			if (1 != fwrite((char *) record.value + iinq_calculate_offset(0, j), field_size, 1, file)) {
				break;
			}
			else {
				write_page_remaining -= field_size;
			}
		}
	}

	if (NULL != file) {
		fclose(file);
	}

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("External sort write time: %lu\n", end_time - start_time);
#endif

	start_time	= ion_time();

	file		= fopen("orderby", "rb");

	ion_external_sort_t es;

	iinq_sort_context_t context;

	context.parts	= &orderby_order_part;
	context.n		= 1;

	ion_err_t error = ion_external_sort_init(&es, file, &context, iinq_sort_compare, 0, value_size + orderby_order_part.size, IINQ_PAGE_SIZE, boolean_false, ION_FILE_SORT_FLASH_MINSORT);

	free(record.key);
	free(record.value);
	cursor->destroy(&cursor);
	ion_close_dictionary(&dictionary);

	uint16_t buffer_size					= ion_external_sort_bytes_of_memory_required(&es, 0, boolean_false);

	char *buffer							= malloc(buffer_size);
	/* recordbuf needs enough room for the sort field and the table tuple (sort field is stored twice) */
	char *record_buf						= malloc((orderby_order_part.size + value_size));

	ion_external_sort_cursor_t *sort_cursor = malloc(sizeof(ion_external_sort_cursor_t));

	error		= ion_external_sort_init_cursor(&es, sort_cursor, buffer, buffer_size);

	end_time	= ion_time();

#if OUTPUT_TIMES
	printf("External sort init time: %lu\n", end_time - start_time);
#endif

	start_time = ion_time();

	while (err_ok == sort_cursor->next(sort_cursor, record_buf) && cs_cursor_active == sort_cursor->status) {
#if OUTPUT_QUERY_RESULTS
		printf("ID: %i, ", NEUTRALIZE(record_buf + orderby_order_part.size + iinq_calculate_offset(0, 1), int));
		printf("CharValue: %s, ", record_buf + orderby_order_part.size + iinq_calculate_offset(0, 2));
		printf("IntValue: %d\n", NEUTRALIZE(record_buf + orderby_order_part.size + iinq_calculate_offset(0, 3), int));
#endif
	}

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("External sort sorting time: %lu\n", end_time - start_time);
#endif

	free(buffer);
	free(record_buf);
	fclose(es.input_file);
	fremove("orderby");
	free(sort_cursor);

#endif

	printf("\n");
}

void
test_select_all_where_int_order_by_desc_table1(
	planck_unit_test_t *tc
) {
	volatile unsigned long start_time, end_time;

#if OUTPUT_SQL_STATEMENTS
	printf("SELECT * FROM Table1 ORDER BY ID DESC\n");
#endif

	start_time = ion_time();

/*  iinq_result_set_t *rs1 = SQL_select("SELECT * FROM Table1 ORDER BY ID DESC;"); */
	iinq_result_set_t *rs1 = iinq_init_result_set(iinq_projection_init(iinq_external_sort_init(iinq_projection_init(iinq_dictionary_init(0, 3, predicate_all_records), 3, IINQ_PROJECTION_LIST(1, 2, 3)), 1, IINQ_ORDER_BY_LIST(IINQ_ORDER_BY(1, IINQ_DESC))), 3, IINQ_PROJECTION_LIST(1, 2, 3)));

	end_time = ion_time();
#if OUTPUT_TIMES
	printf("Init time taken: %lu\n", end_time - start_time);
#endif

	if ((NULL == rs1) || (err_ok != rs1->status.error)) {
		PLANCK_UNIT_SET_FAIL(tc);
	}

	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, 3, rs1->head->instance->num_fields);

	start_time = ion_time();

	while (iinq_next(rs1)) {
#if OUTPUT_QUERY_RESULTS
		printf("ID: %i, ", *iinq_get_int(rs1, 1));
		printf("CharValue: %s, ", iinq_get_string(rs1, 2));
		printf("IntValue: %d\n", *iinq_get_int(rs1, 3));
#endif
	}

#if OUTPUT_QUERY_RESULTS
	printf("\n");
#endif

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Iteration time taken: %lu\n", end_time - start_time);
#endif

	start_time = ion_time();

	iinq_close_result_set(rs1);

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Closing time taken: %lu\n\n", end_time - start_time);
#endif

	PLANCK_UNIT_ASSERT_TRUE(tc, NULL == rs1);
}

void
test_drop_table1(
	planck_unit_test_t *tc
) {
	volatile unsigned long start_time, end_time;

#if OUTPUT_SQL_STATEMENTS
	printf("DROP TABLE Table1;\n");
#endif

	start_time = ion_time();

	ion_err_t error =
/*		  SQL_execute("DROP TABLE Table1;"); */
		drop_table(0);

	end_time = ion_time();

#if OUTPUT_TIMES
	printf("Drop Table time taken: %lu\n\n", end_time - start_time);
#endif

	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, err_ok, error);

	ion_dictionary_t			dictionary;
	ion_dictionary_handler_t	handler;

	error = iinq_open_source(0, &dictionary, &handler);

	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, err_file_open_error, error);
}

void
test_performance_prepared_records_with_queries(
	planck_unit_test_t *tc
) {
	int num_records;

	for (num_records = MIN_RECORDS; num_records <= MAX_RECORDS; num_records += RECORD_INCREMENT) {
		test_create_table1(tc);
		test_insert_records_prep_table1(tc, num_records);
		test_select_all_records_table1(tc);
		test_select_all_where_equal_int_table1(tc);
		test_select_all_where_equal_int_key_table1(tc);
		test_select_all_where_range_int_table1(tc);
		test_select_all_where_range_int_key_table1(tc);
		test_select_field_list_table1(tc);
		test_select_all_where_str_equal_table1(tc);
		/* test_select_all_where_str_not_equal_table1(tc); */
		/* TODO: add tests to compare these tests with similar ones done on key (once new predicates have been added to IonDB) */
		/* test_select_all_where_int_key_less_than_table1(tc); */
		/* test_select_all_where_int_key_less_than__equal_table1(tc); */
		/* test_select_all_where_int_key_greater_than_table1(tc); */
		/* test_select_all_where_int_key_greater_than__equal_table1(tc); */
		/* test_select_all_where_int_key_not__equal_table1(tc); */

		/* order by is not very scalable at the moment */
		if (!skip_order_by) {
			test_select_all_where_int_order_by_asc_table1(tc);
			/* test_select_all_where_int_order_by_desc_table1(tc); */
		}

		test_drop_table1(tc);
		fremove(ION_MASTER_TABLE_FILENAME);
	}
}

void
test_performance_regular_records(
	planck_unit_test_t *tc
) {
	int num_records;

	for (num_records = MIN_RECORDS; num_records <= MAX_RECORDS; num_records += RECORD_INCREMENT) {
		test_create_table1(tc);
		test_insert_records_table1(tc, num_records);
		test_drop_table1(tc);
		fremove(ION_MASTER_TABLE_FILENAME);
	}
}

planck_unit_suite_t *
iinq_get_suite1(
) {
	planck_unit_suite_t *suite = planck_unit_new_suite();

	PLANCK_UNIT_ADD_TO_SUITE(suite, test_performance_prepared_records_with_queries);
	PLANCK_UNIT_ADD_TO_SUITE(suite, test_performance_regular_records);

	return suite;
}

void
run_all_tests_iinq_device(
) {
	planck_unit_suite_t *suite1 = iinq_get_suite1();

	planck_unit_run_suite(suite1);
	planck_unit_destroy_suite(suite1);

	fdeleteall();
	fremove(ION_MASTER_TABLE_FILENAME);
}