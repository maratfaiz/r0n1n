/**
 * @file text_input_i.h
 * GUI: TextInput, firmware-internal additions (R0N1N)
 */
#pragma once

#include "text_input.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Keep Cyrillic (UTF-8) in the result instead of transliterating it to
 * Latin on Save. Only for text that is not a file name (e.g. a search
 * query): FAT on the SD card can't store Cyrillic names. Reset by
 * text_input_reset().
 */
void text_input_set_allow_unicode(TextInput* text_input, bool allow);

#ifdef __cplusplus
}
#endif
