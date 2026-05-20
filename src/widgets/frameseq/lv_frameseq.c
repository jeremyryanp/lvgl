/**
 * @file lv_frameseq.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_frameseq.h"

#if LV_USE_FRAMESEQ

#include "../../core/lv_obj_class_private.h"
#include "../../misc/lv_assert.h"
#include "../../misc/lv_fs.h"
#include "../../misc/lv_log.h"
#include "../../misc/lv_math.h"
#include "../../misc/lv_timer_private.h"
#include "../../stdlib/lv_sprintf.h"
#include "../../stdlib/lv_string.h"
#include "../image/lv_image_private.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/*********************
 *      DEFINES
 *********************/

#define MY_CLASS (&lv_frameseq_class)
#define LV_FRAMESEQ_TIMER_PERIOD_MS 16
#define LV_FRAMESEQ_PATH_MAX 256

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_image_t img;
    char * src;
    char * format;
    char * current_path;
    int32_t frame_width;
    int32_t frame_height;
    int32_t frame_count;
    int32_t start_index;
    int32_t end_index;
    int32_t current_frame_index;
    int32_t mode;
    int32_t duration;
    uint32_t play_start_tick;
    lv_timer_t * timer;
    lv_draw_buf_t * draw_buf;
} lv_frameseq_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void lv_frameseq_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_frameseq_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_frameseq_timer_cb(lv_timer_t * timer);
static void lv_frameseq_refresh(lv_obj_t * obj);
static void lv_frameseq_restart(lv_obj_t * obj);
static void lv_frameseq_render_frame(lv_obj_t * obj, int32_t frame_index);
static bool lv_frameseq_ensure_draw_buf(lv_obj_t * obj);
static bool lv_frameseq_is_raw_format(const char * format);
static int32_t lv_frameseq_clamp_frame(const lv_frameseq_t * frameseq, int32_t frame_index);
static int32_t lv_frameseq_resolved_end_index(const lv_frameseq_t * frameseq);
static void lv_frameseq_format_path(const lv_frameseq_t * frameseq, int32_t frame_index, char * buf, size_t buf_size);
static void lv_frameseq_strlcat(char * dst, const char * src, size_t dst_size);
static void lv_frameseq_set_text(char ** target, const char * value);

/**********************
 *  STATIC VARIABLES
 **********************/

#if LV_USE_OBJ_PROPERTY
static const lv_property_ops_t lv_frameseq_properties[] = {
    {
        .id = LV_PROPERTY_FRAMESEQ_CURRENT_FRAME_INDEX,
        .setter = lv_frameseq_set_current_frame_index,
        .getter = lv_frameseq_get_current_frame_index,
    },
    {
        .id = LV_PROPERTY_FRAMESEQ_DURATION,
        .setter = lv_frameseq_set_duration,
        .getter = lv_frameseq_get_duration,
    },
    {
        .id = LV_PROPERTY_FRAMESEQ_END_INDEX,
        .setter = lv_frameseq_set_end_index,
        .getter = lv_frameseq_get_end_index,
    },
    {
        .id = LV_PROPERTY_FRAMESEQ_FORMAT,
        .setter = lv_frameseq_set_format,
        .getter = lv_frameseq_get_format,
    },
    {
        .id = LV_PROPERTY_FRAMESEQ_FRAME_COUNT,
        .setter = lv_frameseq_set_frame_count,
        .getter = lv_frameseq_get_frame_count,
    },
    {
        .id = LV_PROPERTY_FRAMESEQ_FRAME_HEIGHT,
        .setter = lv_frameseq_set_frame_height,
        .getter = lv_frameseq_get_frame_height,
    },
    {
        .id = LV_PROPERTY_FRAMESEQ_FRAME_WIDTH,
        .setter = lv_frameseq_set_frame_width,
        .getter = lv_frameseq_get_frame_width,
    },
    {
        .id = LV_PROPERTY_FRAMESEQ_MODE,
        .setter = lv_frameseq_set_mode,
        .getter = lv_frameseq_get_mode,
    },
    {
        .id = LV_PROPERTY_FRAMESEQ_SRC,
        .setter = lv_frameseq_set_src,
        .getter = lv_frameseq_get_src,
    },
    {
        .id = LV_PROPERTY_FRAMESEQ_START_INDEX,
        .setter = lv_frameseq_set_start_index,
        .getter = lv_frameseq_get_start_index,
    },
};
#endif
const lv_obj_class_t lv_frameseq_class = {
    .constructor_cb = lv_frameseq_constructor,
    .destructor_cb = lv_frameseq_destructor,
    .instance_size = sizeof(lv_frameseq_t),
    .base_class = &lv_image_class,
    .name = "lv_frameseq",
    LV_PROPERTY_CLASS_FIELDS(frameseq, FRAMESEQ)
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lv_frameseq_create(lv_obj_t * parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t * obj = lv_obj_class_create_obj(MY_CLASS, parent);
    lv_obj_class_init_obj(obj);
    return obj;
}

void lv_frameseq_set_src(lv_obj_t * obj, const char * src)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;

    lv_frameseq_set_text(&frameseq->src, src);
    frameseq->current_frame_index = -1;
    lv_frameseq_restart(obj);
}

char * lv_frameseq_get_src(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;
    return frameseq->src;
}

void lv_frameseq_set_format(lv_obj_t * obj, const char * format)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;

    lv_frameseq_set_text(&frameseq->format, format && format[0] ? format : "image");
    frameseq->current_frame_index = -1;
    if(!lv_frameseq_is_raw_format(frameseq->format) && frameseq->draw_buf) {
        lv_draw_buf_destroy(frameseq->draw_buf);
        frameseq->draw_buf = NULL;
    }
    lv_frameseq_refresh(obj);
}

char * lv_frameseq_get_format(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;
    return frameseq->format;
}

void lv_frameseq_set_frame_width(lv_obj_t * obj, int32_t frame_width)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;
    frameseq->frame_width = LV_MAX(frame_width, 0);
    if(frameseq->draw_buf && frameseq->draw_buf->header.w != (uint32_t)frameseq->frame_width) {
        lv_draw_buf_destroy(frameseq->draw_buf);
        frameseq->draw_buf = NULL;
        frameseq->current_frame_index = -1;
    }
    lv_frameseq_refresh(obj);
}

int32_t lv_frameseq_get_frame_width(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;
    return frameseq->frame_width;
}

void lv_frameseq_set_frame_height(lv_obj_t * obj, int32_t frame_height)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;
    frameseq->frame_height = LV_MAX(frame_height, 0);
    if(frameseq->draw_buf && frameseq->draw_buf->header.h != (uint32_t)frameseq->frame_height) {
        lv_draw_buf_destroy(frameseq->draw_buf);
        frameseq->draw_buf = NULL;
        frameseq->current_frame_index = -1;
    }
    lv_frameseq_refresh(obj);
}

int32_t lv_frameseq_get_frame_height(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;
    return frameseq->frame_height;
}

void lv_frameseq_set_frame_count(lv_obj_t * obj, int32_t frame_count)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;
    frameseq->frame_count = LV_MAX(frame_count, 0);
    if(frameseq->frame_count > 0 && frameseq->current_frame_index >= frameseq->frame_count) {
        frameseq->current_frame_index = -1;
    }
    lv_frameseq_restart(obj);
}

int32_t lv_frameseq_get_frame_count(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;
    return frameseq->frame_count;
}

void lv_frameseq_set_duration(lv_obj_t * obj, int32_t duration)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;
    frameseq->duration = LV_MAX(duration, 0);
    lv_frameseq_restart(obj);
}

int32_t lv_frameseq_get_duration(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;
    return frameseq->duration;
}

void lv_frameseq_set_mode(lv_obj_t * obj, int32_t mode)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;
    frameseq->mode = mode == 0 ? 0 : 1;
    lv_frameseq_restart(obj);
}

int32_t lv_frameseq_get_mode(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;
    return frameseq->mode;
}

void lv_frameseq_set_start_index(lv_obj_t * obj, int32_t start_index)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;
    frameseq->start_index = LV_MAX(start_index, 0);
    frameseq->current_frame_index = -1;
    lv_frameseq_restart(obj);
}

int32_t lv_frameseq_get_start_index(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;
    return frameseq->start_index;
}

void lv_frameseq_set_end_index(lv_obj_t * obj, int32_t end_index)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;
    frameseq->end_index = end_index;
    frameseq->current_frame_index = -1;
    lv_frameseq_restart(obj);
}

int32_t lv_frameseq_get_end_index(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;
    return frameseq->end_index;
}

void lv_frameseq_set_current_frame_index(lv_obj_t * obj, int32_t current_frame_index)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_frameseq_render_frame(obj, current_frame_index);
}

int32_t lv_frameseq_get_current_frame_index(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;
    return frameseq->current_frame_index;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void lv_frameseq_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);

    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;
    frameseq->format = lv_strdup("image");
    frameseq->frame_count = 0;
    frameseq->start_index = 0;
    frameseq->end_index = -1;
    frameseq->current_frame_index = -1;
    frameseq->mode = 1;
    frameseq->duration = 0;
    frameseq->timer = lv_timer_create(lv_frameseq_timer_cb, LV_FRAMESEQ_TIMER_PERIOD_MS, obj);
    if(frameseq->timer) {
        lv_timer_pause(frameseq->timer);
    }
}

static void lv_frameseq_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;

    if(frameseq->timer) {
        lv_timer_delete(frameseq->timer);
        frameseq->timer = NULL;
    }
    if(frameseq->draw_buf) {
        lv_draw_buf_destroy(frameseq->draw_buf);
        frameseq->draw_buf = NULL;
    }

    lv_free(frameseq->src);
    lv_free(frameseq->format);
    lv_free(frameseq->current_path);
    frameseq->src = NULL;
    frameseq->format = NULL;
    frameseq->current_path = NULL;
}

static void lv_frameseq_timer_cb(lv_timer_t * timer)
{
    lv_obj_t * obj = (lv_obj_t *)lv_timer_get_user_data(timer);
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;

    if(frameseq == NULL || frameseq->mode == 0 || frameseq->duration <= 0 || frameseq->frame_count <= 0) {
        lv_timer_pause(timer);
        return;
    }

    int32_t start = lv_frameseq_clamp_frame(frameseq, frameseq->start_index);
    int32_t end = lv_frameseq_resolved_end_index(frameseq);
    if(end < start) {
        lv_timer_pause(timer);
        return;
    }

    int32_t frame_span = end - start + 1;
    uint32_t elapsed = lv_tick_elaps(frameseq->play_start_tick);
    uint32_t position = elapsed % (uint32_t)frameseq->duration;
    int32_t frame = start + (int32_t)(((uint64_t)frame_span * position) / (uint32_t)frameseq->duration);
    if(frame > end) {
        frame = end;
    }

    lv_frameseq_render_frame(obj, frame);
}

static void lv_frameseq_refresh(lv_obj_t * obj)
{
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;
    int32_t frame = frameseq->current_frame_index >= 0 ? frameseq->current_frame_index : frameseq->start_index;
    lv_frameseq_render_frame(obj, frame);
}

static void lv_frameseq_restart(lv_obj_t * obj)
{
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;

    if(frameseq->mode == 0 || frameseq->duration <= 0 || frameseq->frame_count <= 0) {
        if(frameseq->timer) {
            lv_timer_pause(frameseq->timer);
        }
        lv_frameseq_refresh(obj);
        return;
    }

    frameseq->play_start_tick = lv_tick_get();
    lv_frameseq_refresh(obj);
    if(frameseq->timer) {
        lv_timer_set_period(frameseq->timer, LV_FRAMESEQ_TIMER_PERIOD_MS);
        lv_timer_resume(frameseq->timer);
        lv_timer_reset(frameseq->timer);
    }
}

static void lv_frameseq_render_frame(lv_obj_t * obj, int32_t frame_index)
{
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;

    if(frameseq->src == NULL || frameseq->src[0] == '\0') {
        return;
    }

    frame_index = lv_frameseq_clamp_frame(frameseq, frame_index);
    if(frame_index == frameseq->current_frame_index) {
        return;
    }

    char path[LV_FRAMESEQ_PATH_MAX];
    lv_frameseq_format_path(frameseq, frame_index, path, sizeof(path));

    if(lv_frameseq_is_raw_format(frameseq->format)) {
        if(!lv_frameseq_ensure_draw_buf(obj)) {
            return;
        }

        lv_fs_file_t file;
        if(lv_fs_open(&file, path, LV_FS_MODE_RD) != LV_FS_RES_OK) {
            LV_LOG_WARN("frameseq frame open failed: %s", path);
            return;
        }

        uint32_t expected_bytes = frameseq->draw_buf->header.stride * frameseq->draw_buf->header.h;
        uint32_t bytes_read = 0;
        lv_fs_res_t read_result = lv_fs_read(&file, frameseq->draw_buf->data, expected_bytes, &bytes_read);
        lv_fs_close(&file);

        if(read_result != LV_FS_RES_OK || bytes_read != expected_bytes) {
            LV_LOG_WARN("frameseq frame read failed: %s", path);
            return;
        }

        lv_draw_buf_flush_cache(frameseq->draw_buf, NULL);
        lv_obj_invalidate(obj);
        frameseq->current_frame_index = frame_index;
        return;
    }

    lv_frameseq_set_text(&frameseq->current_path, path);
    lv_image_set_src(obj, frameseq->current_path);
    lv_obj_invalidate(obj);
    frameseq->current_frame_index = frame_index;
}

static bool lv_frameseq_ensure_draw_buf(lv_obj_t * obj)
{
    lv_frameseq_t * frameseq = (lv_frameseq_t *)obj;

    int32_t width = frameseq->frame_width > 0 ? frameseq->frame_width : lv_obj_get_width(obj);
    int32_t height = frameseq->frame_height > 0 ? frameseq->frame_height : lv_obj_get_height(obj);

    if(width <= 0 || height <= 0) {
        LV_LOG_WARN("frameseq raw format requires width and height");
        return false;
    }

    if(frameseq->draw_buf &&
       (frameseq->draw_buf->header.w != (uint32_t)width ||
        frameseq->draw_buf->header.h != (uint32_t)height ||
        frameseq->draw_buf->header.cf != LV_COLOR_FORMAT_RGB565)) {
        lv_draw_buf_destroy(frameseq->draw_buf);
        frameseq->draw_buf = NULL;
    }

    if(frameseq->draw_buf == NULL) {
        frameseq->draw_buf = lv_draw_buf_create((uint32_t)width,
                                                (uint32_t)height,
                                                LV_COLOR_FORMAT_RGB565,
                                                LV_STRIDE_AUTO);
        if(frameseq->draw_buf == NULL) {
            LV_LOG_WARN("frameseq draw buffer allocation failed");
            return false;
        }
        lv_image_set_src(obj, frameseq->draw_buf);
    }

    return true;
}

static bool lv_frameseq_is_raw_format(const char * format)
{
    if(format == NULL) {
        return false;
    }

    char lowered[16];
    size_t len = strlen(format);
    if(len >= sizeof(lowered)) {
        return false;
    }

    for(size_t i = 0; i <= len; i++) {
        lowered[i] = (char)tolower((unsigned char)format[i]);
    }

    return strcmp(lowered, "rgb565") == 0 || strcmp(lowered, "raw565") == 0;
}

static int32_t lv_frameseq_clamp_frame(const lv_frameseq_t * frameseq, int32_t frame_index)
{
    if(frame_index < 0) {
        frame_index = 0;
    }
    if(frameseq->frame_count > 0 && frame_index >= frameseq->frame_count) {
        frame_index = frameseq->frame_count - 1;
    }
    return frame_index;
}

static int32_t lv_frameseq_resolved_end_index(const lv_frameseq_t * frameseq)
{
    if(frameseq->frame_count <= 0) {
        return LV_MAX(frameseq->end_index, frameseq->start_index);
    }

    if(frameseq->end_index < 0) {
        return frameseq->frame_count - 1;
    }

    return LV_MIN(frameseq->end_index, frameseq->frame_count - 1);
}

static void lv_frameseq_format_path(const lv_frameseq_t * frameseq, int32_t frame_index, char * buf, size_t buf_size)
{
    const char * pattern = frameseq->src ? frameseq->src : "";

    if(strchr(pattern, '%') != NULL) {
        lv_snprintf(buf, buf_size, pattern, (int)frame_index);
        return;
    }

    lv_strlcpy(buf, pattern, buf_size);

    char index_buf[16];
    lv_snprintf(index_buf, sizeof(index_buf), "%" LV_PRId32, frame_index);

    char * pos = strstr(buf, "{index}");
    size_t token_len = 7;
    if(pos == NULL) {
        pos = strstr(buf, "{}");
        token_len = 2;
    }
    if(pos == NULL) {
        return;
    }

    char suffix[LV_FRAMESEQ_PATH_MAX];
    lv_strlcpy(suffix, pos + token_len, sizeof(suffix));
    *pos = '\0';
    lv_frameseq_strlcat(buf, index_buf, buf_size);
    lv_frameseq_strlcat(buf, suffix, buf_size);
}

static void lv_frameseq_strlcat(char * dst, const char * src, size_t dst_size)
{
    size_t len = lv_strlen(dst);
    if(len >= dst_size) {
        return;
    }

    lv_strlcpy(dst + len, src, dst_size - len);
}

static void lv_frameseq_set_text(char ** target, const char * value)
{
    char * copy = value ? lv_strdup(value) : NULL;
    if(value && copy == NULL) {
        LV_LOG_WARN("frameseq string allocation failed");
        return;
    }

    lv_free(*target);
    *target = copy;
}

#endif /*LV_USE_FRAMESEQ*/
