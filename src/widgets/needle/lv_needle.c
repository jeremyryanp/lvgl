/**
 * @file lv_needle.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../../core/lv_obj_class_private.h"
#include "lv_needle_private.h"

#if LV_USE_NEEDLE != 0
#include "../../draw/lv_draw.h"
#include "../../misc/lv_assert.h"
#include "../../misc/lv_area.h"
#include "../../misc/lv_math.h"
#include "../../misc/lv_types.h"

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS (&lv_needle_class)
#define LV_NEEDLE_OVERINVALIDATE_PX 1
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void lv_needle_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_needle_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_needle_event(const lv_obj_class_t * class_p, lv_event_t * e);
static void lv_needle_get_points(const lv_needle_t * needle, lv_point_precise_t * p1, lv_point_precise_t * p2);
static void lv_needle_get_current_area(const lv_obj_t * obj, const lv_needle_t * needle, lv_area_t * area);
static void lv_needle_get_geometry_area(const lv_obj_t * obj, const lv_needle_t * needle, lv_area_t * area);
static void lv_needle_invalidate_area(lv_obj_t * obj, const lv_area_t * area);
static void lv_needle_refresh_value(lv_obj_t * obj);
static void lv_needle_refresh_geometry(lv_obj_t * obj);
static lv_value_precise_t lv_needle_map_value_to_angle(const lv_needle_t * needle);
static int32_t lv_needle_floor(lv_value_precise_t value);
static int32_t lv_needle_ceil(lv_value_precise_t value);
static int32_t lv_needle_floor_to_multiple(int32_t value, int32_t multiple);
static void lv_needle_get_parent_origin(const lv_obj_t * obj, int32_t * x, int32_t * y);
static int32_t lv_needle_get_draw_pad(const lv_obj_t * obj);

/**********************
 *  STATIC VARIABLES
 **********************/

#if LV_USE_OBJ_PROPERTY
static const lv_property_ops_t lv_needle_properties[] = {
    {
        .id = LV_PROPERTY_NEEDLE_BACK_LENGTH,
        .setter = lv_needle_set_back_length,
        .getter = lv_needle_get_back_length,
    },
    {
        .id = LV_PROPERTY_NEEDLE_END_ANGLE,
        .setter = lv_needle_set_end_angle,
        .getter = lv_needle_get_end_angle,
    },
    {
        .id = LV_PROPERTY_NEEDLE_END_X,
        .setter = lv_needle_set_end_x,
        .getter = lv_needle_get_end_x,
    },
    {
        .id = LV_PROPERTY_NEEDLE_END_Y,
        .setter = lv_needle_set_end_y,
        .getter = lv_needle_get_end_y,
    },
    {
        .id = LV_PROPERTY_NEEDLE_LENGTH,
        .setter = lv_needle_set_length,
        .getter = lv_needle_get_length,
    },
    {
        .id = LV_PROPERTY_NEEDLE_MAX_VALUE,
        .setter = lv_needle_set_max_value,
        .getter = lv_needle_get_max_value,
    },
    {
        .id = LV_PROPERTY_NEEDLE_MIN_VALUE,
        .setter = lv_needle_set_min_value,
        .getter = lv_needle_get_min_value,
    },
    {
        .id = LV_PROPERTY_NEEDLE_PIVOT_X,
        .setter = lv_needle_set_pivot_x,
        .getter = lv_needle_get_pivot_x,
    },
    {
        .id = LV_PROPERTY_NEEDLE_PIVOT_Y,
        .setter = lv_needle_set_pivot_y,
        .getter = lv_needle_get_pivot_y,
    },
    {
        .id = LV_PROPERTY_NEEDLE_SEGMENT,
        .setter = lv_needle_set_segment,
        .getter = lv_needle_get_segment,
    },
    {
        .id = LV_PROPERTY_NEEDLE_START_ANGLE,
        .setter = lv_needle_set_start_angle,
        .getter = lv_needle_get_start_angle,
    },
    {
        .id = LV_PROPERTY_NEEDLE_START_X,
        .setter = lv_needle_set_start_x,
        .getter = lv_needle_get_start_x,
    },
    {
        .id = LV_PROPERTY_NEEDLE_START_Y,
        .setter = lv_needle_set_start_y,
        .getter = lv_needle_get_start_y,
    },
    {
        .id = LV_PROPERTY_NEEDLE_VALUE,
        .setter = lv_needle_set_value,
        .getter = lv_needle_get_value,
    },
    {
        .id = LV_PROPERTY_NEEDLE_WIDTH,
        .setter = lv_needle_set_width,
        .getter = lv_needle_get_width,
    },
};
#endif
const lv_obj_class_t lv_needle_class = {
    .constructor_cb = lv_needle_constructor,
    .destructor_cb = lv_needle_destructor,
    .event_cb = lv_needle_event,
    .width_def = LV_SIZE_CONTENT,
    .height_def = LV_SIZE_CONTENT,
    .instance_size = sizeof(lv_needle_t),
    .base_class = &lv_obj_class,
    .name = "needle",
    LV_PROPERTY_CLASS_FIELDS(needle, NEEDLE)
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lv_needle_create(lv_obj_t * parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t * obj = lv_obj_class_create_obj(&lv_needle_class, parent);
    lv_obj_class_init_obj(obj);
    lv_needle_refresh_geometry(obj);

    return obj;
}

void lv_needle_set_pivot(lv_obj_t * obj, lv_value_precise_t x, lv_value_precise_t y)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->pivot_x = x;
    needle->pivot_y = y;
    lv_needle_refresh_geometry(obj);
}

void lv_needle_set_pivot_x(lv_obj_t * obj, lv_value_precise_t x)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->pivot_x = x;
    lv_needle_refresh_geometry(obj);
}

void lv_needle_set_pivot_y(lv_obj_t * obj, lv_value_precise_t y)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->pivot_y = y;
    lv_needle_refresh_geometry(obj);
}

void lv_needle_set_length(lv_obj_t * obj, lv_value_precise_t length)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->length = length;
    lv_needle_refresh_geometry(obj);
}

void lv_needle_set_back_length(lv_obj_t * obj, lv_value_precise_t length)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->back_length = length;
    lv_needle_refresh_geometry(obj);
}

void lv_needle_set_line_width(lv_obj_t * obj, lv_value_precise_t width)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_obj_set_style_line_width(obj, width, 0);
    lv_needle_refresh_geometry(obj);
}

void lv_needle_set_color(lv_obj_t * obj, lv_color_t color)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_obj_set_style_line_color(obj, color, 0);
    lv_needle_refresh_value(obj);
}

void lv_needle_set_start_angle(lv_obj_t * obj, lv_value_precise_t angle)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->start_angle = angle;
    lv_needle_refresh_geometry(obj);
}

void lv_needle_set_end_angle(lv_obj_t * obj, lv_value_precise_t angle)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->end_angle = angle;
    lv_needle_refresh_geometry(obj);
}

void lv_needle_set_min_value(lv_obj_t * obj, lv_value_precise_t min_value)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->min_value = min_value;
    lv_needle_refresh_value(obj);
}

void lv_needle_set_max_value(lv_obj_t * obj, lv_value_precise_t max_value)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->max_value = max_value;
    lv_needle_refresh_value(obj);
}

void lv_needle_set_value(lv_obj_t * obj, lv_value_precise_t value)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->value = value;
    lv_needle_refresh_value(obj);
}

void lv_needle_set_start_x(lv_obj_t * obj, lv_value_precise_t x) { lv_needle_set_pivot_x(obj, x); }

void lv_needle_set_start_y(lv_obj_t * obj, lv_value_precise_t y) { lv_needle_set_pivot_y(obj, y); }

void lv_needle_set_end_x(lv_obj_t * obj, lv_value_precise_t x)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->end_x = x;
    needle->is_segment = true;
    lv_needle_refresh_geometry(obj);
}

void lv_needle_set_end_y(lv_obj_t * obj, lv_value_precise_t y)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->end_y = y;
    needle->is_segment = true;
    lv_needle_refresh_geometry(obj);
}

void lv_needle_set_segment(lv_obj_t * obj, bool segment)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->is_segment = segment;
    lv_needle_refresh_geometry(obj);
}

void lv_needle_set_width(lv_obj_t * obj, lv_value_precise_t width)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_obj_set_style_line_width(obj, width, 0);
    lv_needle_refresh_geometry(obj);
}

lv_value_precise_t lv_needle_get_width(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    return lv_obj_get_style_line_width(obj, 0);
}

lv_value_precise_t lv_needle_get_pivot_x(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    return needle->pivot_x;
}

lv_value_precise_t lv_needle_get_pivot_y(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    return needle->pivot_y;
}

lv_value_precise_t lv_needle_get_length(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    return needle->length;
}

lv_value_precise_t lv_needle_get_back_length(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    return needle->back_length;
}

lv_value_precise_t lv_needle_get_line_width(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    return lv_obj_get_style_line_width(obj, 0);
}

lv_value_precise_t lv_needle_get_start_angle(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    return needle->start_angle;
}

lv_value_precise_t lv_needle_get_end_angle(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    return needle->end_angle;
}

lv_value_precise_t lv_needle_get_value(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    return needle->value;
}

lv_value_precise_t lv_needle_get_min_value(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    return needle->min_value;
}

lv_value_precise_t lv_needle_get_max_value(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    return needle->max_value;
}

lv_color_t lv_needle_get_color(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    return lv_obj_get_style_line_color(obj, LV_PART_MAIN);
}

lv_value_precise_t lv_needle_get_start_x(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    return needle->pivot_x;
}

lv_value_precise_t lv_needle_get_start_y(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    return needle->pivot_y;
}

lv_value_precise_t lv_needle_get_end_x(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    return needle->end_x;
}

lv_value_precise_t lv_needle_get_end_y(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    return needle->end_y;
}

bool lv_needle_get_segment(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    return needle->is_segment;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void lv_needle_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->pivot_x = 0;
    needle->pivot_y = 0;
    needle->min_value = 0;
    needle->max_value = 100;
    needle->start_angle = 0;
    needle->end_angle = 360;
    needle->value = 0;
    needle->length = 100;
    needle->back_length = 0;
    needle->is_segment = false;
    needle->end_x = 0;
    needle->end_y = 0;
    lv_area_set(&needle->cached_area, 0, 0, 0, 0);
    needle->cached_area_valid = false;

    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);

    LV_TRACE_OBJ_CREATE("finished");
}

static void lv_needle_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_UNUSED(obj);
}

static void lv_needle_get_points(const lv_needle_t * needle, lv_point_precise_t * p1, lv_point_precise_t * p2)
{
    if(needle->is_segment) {
        p1->x = needle->pivot_x;
        p1->y = needle->pivot_y;
        p2->x = needle->end_x;
        p2->y = needle->end_y;
        return;
    }

    int32_t angle = (int32_t)lv_needle_map_value_to_angle(needle);
    float sin_val = lv_trigo_sin(angle) / 32768.0f;
    float cos_val = lv_trigo_cos(angle) / 32768.0f;

    p1->x = needle->pivot_x + needle->length * cos_val;
    p1->y = needle->pivot_y + needle->length * sin_val;

    /* Negative back_length intentionally moves the rear endpoint in front of the pivot. */
    p2->x = needle->pivot_x - needle->back_length * cos_val;
    p2->y = needle->pivot_y - needle->back_length * sin_val;
}

static void lv_needle_get_current_area(const lv_obj_t * obj, const lv_needle_t * needle, lv_area_t * area)
{
    lv_point_precise_t p1;
    lv_point_precise_t p2;
    int32_t parent_x;
    int32_t parent_y;
    lv_needle_get_points(needle, &p1, &p2);
    lv_needle_get_parent_origin(obj, &parent_x, &parent_y);

    int32_t x1 = parent_x + lv_needle_floor(LV_MIN(p1.x, p2.x));
    int32_t y1 = parent_y + lv_needle_floor(LV_MIN(p1.y, p2.y));
    int32_t x2 = parent_x + lv_needle_ceil(LV_MAX(p1.x, p2.x));
    int32_t y2 = parent_y + lv_needle_ceil(LV_MAX(p1.y, p2.y));

    lv_area_set(area, x1, y1, x2, y2);

    int32_t pad = lv_needle_get_draw_pad(obj);
    lv_area_increase(area, pad, pad);
}

static void lv_needle_get_geometry_area(const lv_obj_t * obj, const lv_needle_t * needle, lv_area_t * area)
{
    lv_value_precise_t min_x = needle->pivot_x;
    lv_value_precise_t max_x = needle->pivot_x;
    lv_value_precise_t min_y = needle->pivot_y;
    lv_value_precise_t max_y = needle->pivot_y;

    if(needle->is_segment) {
        min_x = LV_MIN(min_x, needle->end_x);
        max_x = LV_MAX(max_x, needle->end_x);
        min_y = LV_MIN(min_y, needle->end_y);
        max_y = LV_MAX(max_y, needle->end_y);
    }
    else {
        lv_value_precise_t min_angle = LV_MIN(needle->start_angle, needle->end_angle);
        lv_value_precise_t max_angle = LV_MAX(needle->start_angle, needle->end_angle);
        int32_t min_angle_i = lv_needle_floor(min_angle);
        int32_t max_angle_i = lv_needle_ceil(max_angle);
        int32_t angle = lv_needle_floor_to_multiple(min_angle_i, 90);

        for(; angle <= max_angle_i; angle += 90) {
            float sin_val = lv_trigo_sin(angle) / 32768.0f;
            float cos_val = lv_trigo_cos(angle) / 32768.0f;

            lv_value_precise_t front_x = needle->pivot_x + needle->length * cos_val;
            lv_value_precise_t front_y = needle->pivot_y + needle->length * sin_val;
            lv_value_precise_t back_x = needle->pivot_x - needle->back_length * cos_val;
            lv_value_precise_t back_y = needle->pivot_y - needle->back_length * sin_val;

            min_x = LV_MIN(min_x, LV_MIN(front_x, back_x));
            max_x = LV_MAX(max_x, LV_MAX(front_x, back_x));
            min_y = LV_MIN(min_y, LV_MIN(front_y, back_y));
            max_y = LV_MAX(max_y, LV_MAX(front_y, back_y));
        }

        {
            lv_value_precise_t endpoint_angles[2] = { needle->start_angle, needle->end_angle };
            uint32_t i;
            for(i = 0; i < 2; i++) {
                float sin_val = lv_trigo_sin((int32_t)endpoint_angles[i]) / 32768.0f;
                float cos_val = lv_trigo_cos((int32_t)endpoint_angles[i]) / 32768.0f;

                lv_value_precise_t front_x = needle->pivot_x + needle->length * cos_val;
                lv_value_precise_t front_y = needle->pivot_y + needle->length * sin_val;
                lv_value_precise_t back_x = needle->pivot_x - needle->back_length * cos_val;
                lv_value_precise_t back_y = needle->pivot_y - needle->back_length * sin_val;

                min_x = LV_MIN(min_x, LV_MIN(front_x, back_x));
                max_x = LV_MAX(max_x, LV_MAX(front_x, back_x));
                min_y = LV_MIN(min_y, LV_MIN(front_y, back_y));
                max_y = LV_MAX(max_y, LV_MAX(front_y, back_y));
            }
        }
    }

    lv_area_set(area,
                lv_needle_floor(min_x),
                lv_needle_floor(min_y),
                lv_needle_ceil(max_x),
                lv_needle_ceil(max_y));

    {
        int32_t pad = lv_needle_get_draw_pad(obj);
        lv_area_increase(area, pad, pad);
    }
}

static void lv_needle_invalidate_area(lv_obj_t * obj, const lv_area_t * area)
{
    if(area == NULL) {
        return;
    }

    lv_obj_invalidate_area(obj, area);
}

static void lv_needle_refresh_value(lv_obj_t * obj)
{
    lv_needle_t * needle = (lv_needle_t *)obj;
    lv_area_t invalidate_area;
    lv_area_t new_area;

    lv_needle_get_current_area(obj, needle, &new_area);

    if(needle->cached_area_valid) {
        invalidate_area.x1 = LV_MIN(needle->cached_area.x1, new_area.x1);
        invalidate_area.y1 = LV_MIN(needle->cached_area.y1, new_area.y1);
        invalidate_area.x2 = LV_MAX(needle->cached_area.x2, new_area.x2);
        invalidate_area.y2 = LV_MAX(needle->cached_area.y2, new_area.y2);
        lv_needle_invalidate_area(obj, &invalidate_area);
    }
    else {
        lv_needle_invalidate_area(obj, &new_area);
    }

    needle->cached_area = new_area;
    needle->cached_area_valid = true;
}

static void lv_needle_refresh_geometry(lv_obj_t * obj)
{
    lv_needle_t * needle = (lv_needle_t *)obj;
    lv_area_t geometry_area;

    lv_needle_get_geometry_area(obj, needle, &geometry_area);
    lv_obj_set_pos(obj, geometry_area.x1, geometry_area.y1);
    lv_obj_set_size(obj, lv_area_get_width(&geometry_area), lv_area_get_height(&geometry_area));

    lv_needle_refresh_value(obj);
}

static lv_value_precise_t lv_needle_map_value_to_angle(const lv_needle_t * needle)
{
    lv_value_precise_t span = needle->max_value - needle->min_value;
    if(span == 0) {
        return needle->start_angle;
    }

    return needle->start_angle +
           ((needle->value - needle->min_value) * (needle->end_angle - needle->start_angle)) / span;
}

static int32_t lv_needle_floor(lv_value_precise_t value)
{
    int32_t truncated = (int32_t)value;
    if(value < (lv_value_precise_t)truncated) {
        truncated--;
    }

    return truncated;
}

static int32_t lv_needle_ceil(lv_value_precise_t value)
{
    int32_t truncated = (int32_t)value;
    if(value > (lv_value_precise_t)truncated) {
        truncated++;
    }

    return truncated;
}

static int32_t lv_needle_floor_to_multiple(int32_t value, int32_t multiple)
{
    int32_t remainder = value % multiple;
    if(remainder < 0) {
        remainder += multiple;
    }

    return value - remainder;
}

static int32_t lv_needle_get_draw_pad(const lv_obj_t * obj)
{
    const int32_t line_width = lv_obj_get_style_line_width((lv_obj_t *)obj, LV_PART_MAIN);
    /* Leave one extra pixel for AA/diagonal rasterization so moving needles don't clip at some angles. */
    return LV_MAX((line_width + 1) / 2, 1) + LV_NEEDLE_OVERINVALIDATE_PX;
}

static void lv_needle_get_parent_origin(const lv_obj_t * obj, int32_t * x, int32_t * y)
{
    lv_obj_t * parent = lv_obj_get_parent((lv_obj_t *)obj);
    if(parent == NULL) {
        *x = 0;
        *y = 0;
        return;
    }

    lv_area_t parent_coords;
    lv_obj_get_coords(parent, &parent_coords);
    *x = parent_coords.x1;
    *y = parent_coords.y1;
}

static void lv_needle_event(const lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_UNUSED(class_p);

    lv_result_t res;

    /*Call the ancestor's event handler*/
    res = lv_obj_event_base(MY_CLASS, e);
    if (res != LV_RESULT_OK)
        return;

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_current_target(e);

    lv_needle_t * needle = (lv_needle_t *)obj;

    if(code == LV_EVENT_REFR_EXT_DRAW_SIZE) {
        /*The corner of the skew lines is out of the intended area*/
        int32_t line_width = lv_obj_get_style_line_width(obj, LV_PART_MAIN);
        int32_t * s = lv_event_get_param(e);
        const int32_t ext_draw_size = line_width + LV_NEEDLE_OVERINVALIDATE_PX;
        if(*s < ext_draw_size) *s = ext_draw_size;
    }
    else if(code == LV_EVENT_GET_SELF_SIZE) {
        lv_point_t * p = lv_event_get_self_size_info(e);
        lv_point_precise_t p1;
        lv_point_precise_t p2;
        lv_needle_get_points(needle, &p1, &p2);

        p->x = LV_MAX(p->x, (int32_t)LV_MAX(p1.x, p2.x));
        p->y = LV_MAX(p->y, (int32_t)LV_MAX(p1.y, p2.y));
    }
    else if(code == LV_EVENT_DRAW_MAIN) {
        lv_layer_t * layer = lv_event_get_layer(e);
        int32_t parent_x;
        int32_t parent_y;
        lv_point_precise_t p1;
        lv_point_precise_t p2;
        lv_needle_get_points(needle, &p1, &p2);
        lv_needle_get_parent_origin(obj, &parent_x, &parent_y);

        lv_draw_line_dsc_t line_dsc;
        lv_draw_line_dsc_init(&line_dsc);
        lv_obj_init_draw_line_dsc(obj, LV_PART_MAIN, &line_dsc);
        line_dsc.p1.x = parent_x + p1.x;
        line_dsc.p1.y = parent_y + p1.y;
        line_dsc.p2.x = parent_x + p2.x;
        line_dsc.p2.y = parent_y + p2.y;

        lv_draw_line(layer, &line_dsc);
    }
}
#endif
