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

// SCRIPT INSERT START
#if LV_USE_OBJ_PROPERTY
static const lv_property_ops_t properties[] = {
    {
        .id = LV_PROPERTY_NEEDLE_ANGLE,
        .setter = lv_needle_set_angle,
        .getter = lv_needle_get_angle,
    },
    {
        .id = LV_PROPERTY_NEEDLE_BACK_LENGTH,
        .setter = lv_needle_set_back_length,
        .getter = lv_needle_get_back_length,
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

void lv_needle_set_angle(lv_obj_t * obj, lv_value_precise_t angle)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->angle = angle;
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

lv_value_precise_t lv_needle_get_angle(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_needle_t * needle = (lv_needle_t *)obj;
    return needle->angle;
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

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void lv_needle_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    lv_needle_t * needle = (lv_needle_t *)obj;
    needle->pivot_x = 0;
    needle->pivot_y = 0;
    needle->angle = 0;
    needle->length = 100;
    needle->back_length = 0;
}

static void lv_needle_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_UNUSED(obj);
}

static void lv_needle_event(const lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_UNUSED(class_p);
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if (code == LV_EVENT_DRAW_MAIN)
    {
        lv_needle_t * needle = (lv_needle_t *)obj;
        lv_layer_t * layer = lv_event_get_layer(e);

        // should i resize the widget to fit the needle?

        lv_draw_line_dsc_t line_dsc;
        lv_draw_line_dsc_init(&line_dsc);
        lv_obj_init_draw_line_dsc(obj, LV_PART_MAIN, &line_dsc);

        // Calculate needle points based on pivot, angle and length
        lv_point_precise_t p1, p2;
        lv_value_precise_t rad = needle->angle * LV_PI / 180.0f;

        p1.x = needle->pivot_x + needle->length * lv_trigo_sin(rad);
        p1.y = needle->pivot_y + needle->length * lv_trigo_cos(rad);

        if (needle->back_length > 0)
        {
            p2.x = needle->pivot_x - needle->back_length * lv_trigo_cos(rad);
            p2.y = needle->pivot_y - needle->back_length * lv_trigo_sin(rad);
        }
        else
        {
            p2.x = needle->pivot_x;
            p2.y = needle->pivot_y;
        }

        line_dsc.p1.x = p1.x;
        line_dsc.p1.y = p1.y;

        line_dsc.p2.x = p2.x;
        line_dsc.p2.y = p2.y;

        lv_draw_line(layer, &line_dsc);
    }
}
#endif
