/**
 * @file lv_scale.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../../core/lv_obj_class_private.h"
#include "../../core/lv_obj_private.h"
#include "lv_scale_section_private.h"
#include "../scale/lv_scale_private.h"
#if LV_USE_SCALE != 0

#include "../../core/lv_group.h"
#include "../../draw/lv_draw_arc.h"
#include "../../misc/lv_assert.h"
#include "../../misc/lv_math.h"

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS (&lv_scale_section_class)
#define LV_SCALE_TICK_IDX_DEFAULT_ID ((uint32_t)255U)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void lv_scale_section_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_scale_section_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_scale_section_event(const lv_obj_class_t * class_p, lv_event_t * event);

/**********************
 *  STATIC VARIABLES
 **********************/

const lv_obj_class_t lv_scale_section_class = {
    .constructor_cb = lv_scale_section_constructor,
    .destructor_cb = lv_scale_section_destructor,
    .event_cb = lv_scale_section_event,
    .instance_size = sizeof(lv_scale_section_t),
    .editable = LV_OBJ_CLASS_EDITABLE_TRUE,
    .base_class = &lv_obj_class,
    .name = "scale_section",
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lv_scale_section_create(lv_obj_t * parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t * obj = lv_obj_class_create_obj(MY_CLASS, parent);
    lv_obj_class_init_obj(obj);
    return obj;
}

/*======================
 * Add/remove functions
 *=====================*/

/*
 * New object specific "add" or "remove" functions come here
 */

/*=====================
 * Setter functions
 *====================*/

void lv_scale_section_set_range(lv_scale_section_t * section, int32_t minor_range, int32_t major_range)
{
    if (NULL == section)
        return;

    section->minor_range = minor_range;
    section->major_range = major_range;
}

void lv_scale_section_set_range_min(lv_scale_section_t * section, int32_t range_min)
{
    if (NULL == section)
        return;

    section->minor_range = range_min;
}

void lv_scale_section_set_range_max(lv_scale_section_t * section, int32_t range_max)
{
    if (NULL == section)
        return;

    section->major_range = range_max;
}

void lv_scale_section_set_style(lv_scale_section_t * section, lv_part_t part, lv_style_t * section_part_style)
{
    if (NULL == section)
        return;

    switch (part)
    {
    case LV_PART_MAIN:
        section->main_style = section_part_style;
        break;
    case LV_PART_INDICATOR:
        section->indicator_style = section_part_style;
        break;
    case LV_PART_ITEMS:
        section->items_style = section_part_style;
        break;
    default:
        /* Invalid part */
        break;
    }
}

/*=====================
 * Getter functions
 *====================*/

/*=====================
 * Other functions
 *====================*/

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void lv_scale_section_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    LV_TRACE_OBJ_CREATE("begin");

    lv_scale_t * scale = (lv_scale_t *)obj->parent;
    lv_scale_section_t * section = (lv_scale_section_t *)obj;

    lv_ll_set_head(&scale->section_ll, section);

    LV_ASSERT_MALLOC(section);

    /* Section default values */
    section->main_style = NULL;
    section->indicator_style = NULL;
    section->items_style = NULL;
    section->minor_range = 0U;
    section->major_range = 0U;
    section->first_tick_idx_in_section = LV_SCALE_TICK_IDX_DEFAULT_ID;
    section->last_tick_idx_in_section = LV_SCALE_TICK_IDX_DEFAULT_ID;
    section->first_tick_idx_is_major = 0U;
    section->last_tick_idx_is_major = 0U;
    section->first_tick_in_section_width = 0U;
    section->last_tick_in_section_width = 0U;

    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

    LV_TRACE_OBJ_CREATE("finished");
}

static void lv_scale_section_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    // lv_scale_t * scale = (lv_scale_t *)obj;
    // lv_scale_section_t * section;
    // while (scale->section_ll.head)
    // {
    //     section = lv_ll_get_head(&scale->section_ll);
    //     lv_ll_remove(&scale->section_ll, section);
    //     lv_free(section);
    // }
    // lv_ll_clear(&scale->section_ll);

    // lv_free(obj);

    LV_TRACE_OBJ_CREATE("finished");
}

static void lv_scale_section_event(const lv_obj_class_t * class_p, lv_event_t * event)
{
    LV_UNUSED(class_p);

    // i dont think this needs to do anything as the drawing is handled by the scale.
    // this object exists for the purpose of using properties to configure the section.
}

#endif
