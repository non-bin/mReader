/**
 * mReader is a micro e-reader
 * Copyright (C) 2026  Alice Jacka
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * \copyright 2026 by Alice Jacka <https://github.com/non-bin/mReader>
 */

#include <stdint.h>
#include <stdbool.h>
#include "fatfs.h"

char *format_data_size(char *output, uint32_t input, uint32_t length);
bool attempt_mount_flash(FATFS *fatfs_work_area, bool fail_gracefully);
bool read_file_at_offset(FIL *file, uint32_t offset, char *buffer, uint32_t buffer_size);
