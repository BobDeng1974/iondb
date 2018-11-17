/******************************************************************************/
/**
@file		array_list.c
@author		Spencer MacBeth
@brief		Array List Implementation.
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

#include "array_list.h"
#include "../linear_hash/linear_hash_types.h"
#include <alloca.h>

ion_err_t
ion_array_list_init(int init_size, ion_array_list_t *array_list) {
    array_list->current_size = init_size;
    array_list->data = malloc(init_size * sizeof(int));
    memset(array_list->data, 0, sizeof(int) * init_size);

    if (NULL == array_list->data) {
        return err_out_of_memory;
    }

    return err_ok;
}

ion_err_t
ion_array_list_insert(int index, int value, ion_array_list_t *array_list) {
    /* case we need to expand array */
    if (index >= array_list->current_size) {
        int old_size = array_list->current_size;

        array_list->current_size = array_list->current_size * 2;

        ion_byte_t *bucket_map_cache = alloca(old_size * sizeof(int));

        memcpy(bucket_map_cache, array_list->data, old_size * sizeof(int));
        free(array_list->data);
        array_list->data = NULL;
        array_list->data = malloc(2 * old_size * sizeof(int));
        memset(array_list->data, 0, array_list->current_size * sizeof(int));
        memcpy(array_list->data, bucket_map_cache, old_size * sizeof(int));

        if (NULL == array_list->data) {
            free(array_list->data);
            return err_out_of_memory;
        }
    }

    array_list->data[index] = value;

    return err_ok;
}

int
ion_array_list_get(int index, ion_array_list_t *array_list) {
    /* case bucket_idx is outside of current size of array */
    if (index >= array_list->current_size) {
        return ARRAY_LIST_END_OF_LIST;
    }
        /* case bucket_idx is inside array */
    else {
        return array_list->data[index];
    }
}

ion_err_t
ion_array_list_save_to_file(FILE *file, ion_array_list_t *array_list) {
    if (1 != fwrite(&array_list->current_size, sizeof(int), 1, file)) {
        return err_file_write_error;
    }
    if (1 != fwrite(&array_list->data, array_list->current_size * sizeof(int), 1, file)) {
        return err_file_write_error;
    }
    return err_ok;
}


ion_err_t
ion_array_list_init_from_file(FILE *file, ion_array_list_t *array_list) {
    int size = 0;
    if (1 != (fread(&size, sizeof(int), 1, file))) {
        return err_file_read_error;
    }
    ion_array_list_init(size, array_list);

    if (1 != fread(array_list->data, sizeof(int) * size, 1, file)) {
        return err_file_read_error;
    }

    return 0;
}

void
ion_array_list_destroy(ion_array_list_t *array_list) {
    free(array_list->data);
}
