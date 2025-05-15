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

/**********************
 *  STATIC VARIABLES
 **********************/

const lv_obj_class_t lv_needle_class = {
    .constructor_cb = lv_needle_constructor,
    .destructor_cb = lv_needle_destructor,
    .event_cb = lv_needle_event,
    .width_def = LV_SIZE_CONTENT,
    .height_def = LV_SIZE_CONTENT,
    .instance_size = sizeof(lv_needle_t),
    .base_class = &lv_obj_class,
    .name = "needle",
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
    lv_obj_invalidate(obj);
}

void lv_needle_set_pivot_x(lv_obj_t * obj, lv_value_precise_t x)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->pivot_x = x;
    lv_obj_invalidate(obj);
}

void lv_needle_set_pivot_y(lv_obj_t * obj, lv_value_precise_t y)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->pivot_y = y;
    lv_obj_invalidate(obj);
}

void lv_needle_set_length(lv_obj_t * obj, lv_value_precise_t length)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->length = length;
    lv_obj_invalidate(obj);
}

void lv_needle_set_back_length(lv_obj_t * obj, lv_value_precise_t length)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->back_length = length;
    lv_obj_invalidate(obj);
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
    lv_obj_invalidate(obj);
}

void lv_needle_set_end_angle(lv_obj_t * obj, lv_value_precise_t angle)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->end_angle = angle;
    lv_obj_invalidate(obj);
}

void lv_needle_set_value(lv_obj_t * obj, lv_value_precise_t value)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->value = value;
    lv_obj_invalidate(obj);
}

void lv_needle_set_start_x(lv_obj_t * obj, lv_value_precise_t x) { lv_needle_set_pivot_x(obj, x); }

void lv_needle_set_start_y(lv_obj_t * obj, lv_value_precise_t y) { lv_needle_set_pivot_y(obj, y); }

void lv_needle_set_end_x(lv_obj_t * obj, lv_value_precise_t x)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->end_x = x;
    needle->is_segment = true;
    lv_obj_invalidate(obj);
}

void lv_needle_set_end_y(lv_obj_t * obj, lv_value_precise_t y)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->end_y = y;
    needle->is_segment = true;
    lv_obj_invalidate(obj);
}

void lv_needle_set_segment(lv_obj_t * obj, bool segment)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->is_segment = segment;
    lv_obj_invalidate(obj);
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

    if (code == LV_EVENT_DRAW_MAIN)
    {
        lv_layer_t * layer = lv_event_get_layer(e);

        lv_draw_line_dsc_t line_dsc;
        lv_draw_line_dsc_init(&line_dsc);
        lv_obj_init_draw_line_dsc(obj, LV_PART_MAIN, &line_dsc);

        // Calculate needle points based on pivot, angle and length
        lv_point_precise_t p2;

        int32_t angle;
        angle = lv_map(needle->value, 0, 100, (int32_t)needle->start_angle, (int32_t)needle->end_angle);

        // Calculate current angle based on value range
        lv_value_precise_t angle_range = needle->end_angle - needle->start_angle;
        lv_value_precise_t current_angle = needle->start_angle + (needle->value * angle_range / 100);

        // Convert lv_trigo_sin output to proper sin value (-1 to 1)
        float sin_val = lv_trigo_sin(angle) / 32768.0;
        float cos_val = lv_trigo_cos(angle) / 32768.0;

        if (needle->back_length != 0)
        {
            p2.x = needle->pivot_x - needle->back_length * cos_val;
            p2.y = needle->pivot_y - needle->back_length * sin_val;
        }
        else
        {
            // use original pivot
            p2.x = needle->pivot_x;
            p2.y = needle->pivot_y;
        }

        line_dsc.p1.x = needle->pivot_x + needle->length * cos_val;
        line_dsc.p1.y = needle->pivot_y + needle->length * sin_val;

        line_dsc.p2.x = p2.x;
        line_dsc.p2.y = p2.y;

        if (needle->is_segment)
        {
            line_dsc.p1.x = needle->pivot_x;
            line_dsc.p1.y = needle->pivot_y;

            line_dsc.p2.x = needle->end_x;
            line_dsc.p2.y = needle->end_y;
        }
        else
        {
            // Calculate needle points based on pivot, angle and length
            lv_point_precise_t p2;

            int32_t angle;
            angle = lv_map(needle->value, 0, 100, (int32_t)needle->start_angle, (int32_t)needle->end_angle);

            // Calculate current angle based on value range
            lv_value_precise_t angle_range = needle->end_angle - needle->start_angle;
            lv_value_precise_t current_angle = needle->start_angle + (needle->value * angle_range / 100);

            // Convert lv_trigo_sin output to proper sin value (-1 to 1)
            float sin_val = lv_trigo_sin(angle) / 32768.0;
            float cos_val = lv_trigo_cos(angle) / 32768.0;

            if (needle->back_length != 0)
            {
                p2.x = needle->pivot_x - needle->back_length * cos_val;
                p2.y = needle->pivot_y - needle->back_length * sin_val;
            }
            else
            {
                // use original pivot
                p2.x = needle->pivot_x;
                p2.y = needle->pivot_y;
            }

            line_dsc.p1.x = needle->pivot_x + needle->length * cos_val;
            line_dsc.p1.y = needle->pivot_y + needle->length * sin_val;

            line_dsc.p2.x = p2.x;
            line_dsc.p2.y = p2.y;
        }

        lv_draw_line(layer, &line_dsc);
    }
}
#endif
