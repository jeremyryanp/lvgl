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
#include "../../misc/lv_math.h"
#include "../../misc/lv_math.h" // For LV_PI constant
#include "../../misc/lv_types.h"
#include <stdio.h>

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS (&lv_needle_class)
#define LV_PI 3.14159265358979323846f
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
static void lv_needle_refresh_geometry(lv_obj_t * obj);

/**********************
 *  STATIC VARIABLES
 **********************/

// SCRIPT INSERT START
#if LV_USE_OBJ_PROPERTY
static const lv_property_ops_t properties[] = {
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
// SCRIPT INSERT END
const lv_obj_class_t lv_needle_class = {
    .constructor_cb = lv_needle_constructor,
    .destructor_cb = lv_needle_destructor,
    .event_cb = lv_needle_event,
    .width_def = LV_SIZE_CONTENT,
    .height_def = LV_SIZE_CONTENT,
    .instance_size = sizeof(lv_needle_t),
    .base_class = &lv_obj_class,
    .name = "needle",
// SCRIPT INSERT START
#if LV_USE_OBJ_PROPERTY
    .prop_index_start = LV_PROPERTY_NEEDLE_START,
    .prop_index_end = LV_PROPERTY_NEEDLE_END,
    .properties = properties,
    .properties_count = sizeof(properties) / sizeof(properties[0]),
#if LV_USE_OBJ_PROPERTY_NAME
    .property_names = lv_needle_property_names,
    .names_count = sizeof(lv_needle_property_names) / sizeof(lv_property_name_t),
#endif
#endif
// SCRIPT INSERT END
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
    lv_obj_set_size(obj, lv_obj_get_width(parent), lv_obj_get_height(parent));

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
    lv_obj_invalidate(obj);
}

void lv_needle_set_color(lv_obj_t * obj, lv_color_t color)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_obj_set_style_line_color(obj, color, 0);
    lv_obj_invalidate(obj);
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

void lv_needle_set_value(lv_obj_t * obj, lv_value_precise_t value)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->value = value;
    lv_needle_refresh_geometry(obj);
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
    lv_needle_t * needle = (lv_needle_t *)obj;
    lv_obj_set_style_line_width(obj, width, 0);
    lv_obj_invalidate(obj);
}

lv_value_precise_t lv_needle_get_width(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
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
    lv_needle_t * needle = (lv_needle_t *)obj;
    return 0;
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

lv_value_precise_t lv_needle_get_segment(lv_obj_t * obj)
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
    needle->start_angle = 0;
    needle->end_angle = 360;
    needle->value = 0;
    needle->length = 100;
    needle->back_length = 0;

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

    int32_t angle = lv_map(needle->value, 0, 100, (int32_t)needle->start_angle, (int32_t)needle->end_angle);
    float sin_val = lv_trigo_sin(angle) / 32768.0f;
    float cos_val = lv_trigo_cos(angle) / 32768.0f;

    p1->x = needle->pivot_x + needle->length * cos_val;
    p1->y = needle->pivot_y + needle->length * sin_val;

    if(needle->back_length != 0) {
        p2->x = needle->pivot_x - needle->back_length * cos_val;
        p2->y = needle->pivot_y - needle->back_length * sin_val;
    }
    else {
        p2->x = needle->pivot_x;
        p2->y = needle->pivot_y;
    }
}

static void lv_needle_refresh_geometry(lv_obj_t * obj)
{
    lv_obj_refresh_self_size(obj);
    lv_obj_invalidate(obj);
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
        if(*s < line_width) *s = line_width;
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

        lv_draw_line_dsc_t line_dsc;
        lv_draw_line_dsc_init(&line_dsc);
        lv_obj_init_draw_line_dsc(obj, LV_PART_MAIN, &line_dsc);
        lv_needle_get_points(needle, &line_dsc.p1, &line_dsc.p2);

        lv_draw_line(layer, &line_dsc);
    }
}
#endif
