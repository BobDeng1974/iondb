/********************************************************************/
/*			  Code generated by IinqExecute.java				  */
/********************************************************************/

#if !defined(IINQ_USER_FUNCTIONS_H_)
#define IINQ_USER_FUNCTIONS_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <limits.h>
#include "../../../../dictionary/dictionary_types.h"
#include "../../../../dictionary/dictionary.h"
#include "../../../iinq.h"
#include "../../../iinq_interface/iinq_functions.h"

#define IINQ_DEBUG 1

typedef struct iinq_dictionary_operator iinq_dictionary_operator_t;

struct iinq_dictionary_operator {
	iinq_query_operator_parent_t	super;
	ion_dictionary_t				dictionary;
	ion_dictionary_handler_t		handler;
	ion_dict_cursor_t				*cursor;
	ion_predicate_t					predicate;
	ion_record_t					record;
};

typedef struct iinq_selection iinq_selection_t;

struct iinq_selection {
	iinq_query_operator_parent_t	super;
	unsigned int					num_conditions;
	iinq_where_params_t				*conditions;
};

typedef struct iinq_projection iinq_projection_t;

struct iinq_projection {
	iinq_query_operator_parent_t	super;
	iinq_field_num_t				*input_field_nums;
};

size_t
iinq_calculate_offset(
	iinq_table_id_t		table_id,
	iinq_field_num_t	field_num
);

iinq_query_operator_t *
iinq_selection_init(
	iinq_query_operator_t	*input_operator,
	unsigned int			num_conditions,
	iinq_where_params_t		*conditions
);

ion_boolean_t
iinq_projection_next(
	iinq_query_operator_t *query_operator
);

ion_err_t
iinq_execute_prepared(
	iinq_prepared_sql *p
);

size_t
iinq_calculate_key_offset(
	iinq_table_id_t		table_id,
	iinq_field_num_t	field_num
);

ion_boolean_t
iinq_dictionary_operator_next(
	iinq_query_operator_t *query_operator
);

iinq_query_operator_t *
iinq_projection_init(
	iinq_query_operator_t	*input_operator,
	iinq_field_num_t		num_fields,
	iinq_field_num_t		*field_nums
);

void
iinq_projection_destroy(
	iinq_query_operator_t **query_operator
);

ion_err_t
iinq_print_keys(
	iinq_table_id_t table_id
);

ion_err_t
drop_table(
	iinq_table_id_t table_id
);

void
iinq_set_param(
	iinq_prepared_sql	*p,
	iinq_field_num_t	field_num,
	ion_value_t			val
);

iinq_query_operator_t *
iinq_dictionary_init(
	iinq_table_id_t			table_id,
	iinq_field_num_t		num_fields,
	ion_predicate_type_t	predicate_type,
	...
);

ion_boolean_t
iinq_is_key_field(
	iinq_table_id_t		table_id,
	iinq_field_num_t	field_num
);

void
iinq_selection_destroy(
	iinq_query_operator_t **query_operator
);

iinq_field_t
iinq_get_field_type(
	iinq_table_id_t		tableId,
	iinq_field_num_t	field_num
);

ion_err_t
iinq_print_table(
	iinq_table_id_t table_id
);

ion_err_t
create_table(
	iinq_table_id_t		table_id,
	ion_key_type_t		keyType,
	ion_key_size_t		keySize,
	ion_value_size_t	value_size
);

void
iinq_dictionary_operator_destroy(
	iinq_query_operator_t **query_operator
);

iinq_prepared_sql *
iinq_insert_0(
	int		*value_1,
	char	*value_2,
	int		*value_3
);

ion_boolean_t
iinq_selection_next(
	iinq_query_operator_t *query_operator
);

#if defined(__cplusplus)
}
#endif

#endif
