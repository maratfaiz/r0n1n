#include "r0n1n_ui.h"

#include <furi.h>

void r0n1n_ui_fit_width(Canvas* canvas, FuriString* text, size_t width) {
    // Like r0n1n_ui_fit_width(), but never cuts a UTF-8 sequence in
    // half: Cyrillic letters are two bytes, and a stray lead byte renders as
    // garbage instead of the "...".
    if(canvas_string_width(canvas, furi_string_get_cstr(text)) <= width) return;
    const size_t ellipsis = canvas_string_width(canvas, "...");
    const size_t target = width > ellipsis ? width - ellipsis : 0;
    size_t len = furi_string_size(text);
    do {
        if(!len) break;
        len--;
        while(len && (furi_string_get_char(text, len) & 0xC0) == 0x80) {
            len--;
        }
        furi_string_left(text, len);
    } while(canvas_string_width(canvas, furi_string_get_cstr(text)) > target);
    furi_string_cat_str(text, "...");
}

void r0n1n_ui_multiline_centered(Canvas* canvas, int32_t y_center, const char* text) {
    canvas_set_font(canvas, FontSecondary);
    size_t lines = 1;
    for(const char* p = text; *p; p++) {
        if(*p == '\n') lines++;
    }
    const int32_t line_height = 10;
    int32_t y = y_center - (int32_t)(lines * line_height) / 2 + 7;
    FuriString* line = furi_string_alloc();
    const char* start = text;
    while(true) {
        const char* end = strchr(start, '\n');
        furi_string_set_strn(line, start, end ? (size_t)(end - start) : strlen(start));
        canvas_draw_str_aligned(
            canvas, 64, y, AlignCenter, AlignBottom, furi_string_get_cstr(line));
        if(!end) break;
        start = end + 1;
        y += line_height;
    }
    furi_string_free(line);
}

void r0n1n_ui_icon_centered(
    Canvas* canvas,
    int32_t x,
    int32_t y,
    uint8_t width,
    uint8_t height,
    const Icon* icon) {
    if(!icon) return;
    canvas_draw_icon(
        canvas,
        x + (width - icon_get_width(icon)) / 2,
        y + (height - icon_get_height(icon)) / 2,
        icon);
}

void r0n1n_ui_battery(Canvas* canvas, int32_t x_right, int32_t y, uint8_t pct) {
    int32_t bx = x_right - 13;
    canvas_draw_frame(canvas, bx, y, 12, 7);
    canvas_draw_box(canvas, bx + 12, y + 2, 1, 3);
    canvas_draw_box(canvas, bx + 2, y + 2, (8 * MIN(pct, 100) + 50) / 100, 3);

    char text[5];
    snprintf(text, sizeof(text), "%u%%", MIN(pct, 100));
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, bx - 2, y + 7, AlignRight, AlignBottom, text);
}

void r0n1n_ui_header(
    Canvas* canvas,
    const Icon* icon,
    const char* title,
    const char* right,
    int8_t battery_pct) {
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_box(canvas, 0, 0, canvas_width(canvas), R0N1N_UI_HEADER_HEIGHT);
    canvas_set_color(canvas, ColorWhite);
    canvas_set_font(canvas, FontSecondary);

    int32_t x = 2;
    if(icon) {
        canvas_draw_icon(canvas, 2, (R0N1N_UI_HEADER_HEIGHT - icon_get_height(icon)) / 2, icon);
        x += icon_get_width(icon) + 2;
    }

    int32_t right_x = canvas_width(canvas) - 2;
    if(battery_pct >= 0) {
        r0n1n_ui_battery(canvas, right_x, 1, battery_pct);
        right_x -= 34;
    }
    if(right) {
        canvas_draw_str_aligned(canvas, right_x, 9, AlignRight, AlignBottom, right);
        right_x -= canvas_string_width(canvas, right) + 4;
    }

    if(title) {
        FuriString* text = furi_string_alloc_set(title);
        r0n1n_ui_fit_width(canvas, text, right_x - x);
        canvas_draw_str(canvas, x, 8, furi_string_get_cstr(text));
        furi_string_free(text);
    }
    canvas_set_color(canvas, ColorBlack);
}

void r0n1n_ui_list_row(
    Canvas* canvas,
    int32_t y,
    uint8_t height,
    uint8_t width,
    const Icon* icon,
    const char* label,
    const char* right,
    const Icon* right_icon,
    bool selected) {
    canvas_set_font(canvas, FontSecondary);
    if(selected) {
        canvas_set_color(canvas, ColorBlack);
        canvas_draw_rbox(canvas, 0, y, width, height, 2);
        canvas_set_color(canvas, ColorWhite);
    } else {
        canvas_set_color(canvas, ColorBlack);
    }

    const int32_t baseline = y + (height + 7) / 2;
    if(icon) r0n1n_ui_icon_centered(canvas, 2, y, 10, height, icon);

    int32_t right_edge = width - 3;
    if(right_icon) {
        right_edge -= icon_get_width(right_icon);
        canvas_draw_icon(
            canvas, right_edge, y + (height - icon_get_height(right_icon)) / 2, right_icon);
        right_edge -= 3;
    } else if(right) {
        canvas_draw_str_aligned(canvas, right_edge, baseline, AlignRight, AlignBottom, right);
        right_edge -= canvas_string_width(canvas, right) + 3;
    }

    if(label) {
        FuriString* text = furi_string_alloc_set(label);
        r0n1n_ui_fit_width(canvas, text, right_edge - 15);
        canvas_draw_str(canvas, 15, baseline, furi_string_get_cstr(text));
        furi_string_free(text);
    }
    canvas_set_color(canvas, ColorBlack);
}

void r0n1n_ui_scrollbar(Canvas* canvas, int32_t y, uint8_t height, size_t pos, size_t total) {
    if(total < 2) return;
    const int32_t x = canvas_width(canvas) - 2;
    for(int32_t i = 0; i < height; i += 2) {
        canvas_draw_dot(canvas, x, y + i);
    }
    uint8_t thumb = MAX((size_t)4, height / total);
    int32_t thumb_y = y + (int32_t)(height - thumb) * (int32_t)pos / (int32_t)(total - 1);
    canvas_draw_box(canvas, x - 1, thumb_y, 3, thumb);
}

void r0n1n_ui_tile(
    Canvas* canvas,
    int32_t x,
    int32_t y,
    uint8_t width,
    uint8_t height,
    const Icon* icon,
    bool selected,
    bool marked) {
    canvas_set_color(canvas, ColorBlack);
    if(selected) {
        canvas_draw_rbox(canvas, x, y, width, height, 2);
        canvas_set_color(canvas, ColorWhite);
    } else {
        canvas_draw_rframe(canvas, x, y, width, height, 2);
    }
    r0n1n_ui_icon_centered(canvas, x, y, width, height, icon);
    if(marked) canvas_draw_box(canvas, x + width - 5, y + 2, 3, 3);
    canvas_set_color(canvas, ColorBlack);
}

void r0n1n_ui_caption(Canvas* canvas, int32_t sep_y, const char* text) {
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_line(canvas, 0, sep_y, canvas_width(canvas) - 1, sep_y);
    if(!text) return;
    canvas_set_font(canvas, FontSecondary);
    FuriString* str = furi_string_alloc_set(text);
    r0n1n_ui_fit_width(canvas, str, canvas_width(canvas) - 4);
    canvas_draw_str_aligned(
        canvas, canvas_width(canvas) / 2, 63, AlignCenter, AlignBottom, furi_string_get_cstr(str));
    furi_string_free(str);
}
