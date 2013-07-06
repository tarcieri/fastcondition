#include <pthread.h>
#include "ruby.h"

static VALUE cFastCondition = Qnil;

static VALUE FastCondition_allocate(VALUE klass);
static void FastCondition_mark(pthread_cond_t *cond);
static void FastCondition_free(pthread_cond_t *cond);

static VALUE FastCondition_signal(VALUE self);
static VALUE FastCondition_wait(VALUE self, VALUE mutex);

void Init_fastcondition_ext()
{
    cFastCondition = rb_define_class("FastCondition", rb_cObject);
    rb_define_alloc_func(cFastCondition, FastCondition_allocate);

    rb_define_method(cFastCondition, "signal", FastCondition_signal, 0);
    rb_define_method(cFastCondition, "wait", FastCondition_wait, 1);
}

static VALUE FastCondition_allocate(VALUE klass)
{
    pthread_cond_t *cond;

    cond = (pthread_cond_t *)xmalloc(sizeof(pthread_cond_t));
    pthread_cond_init(cond, NULL);

    return Data_Wrap_Struct(klass, FastCondition_mark, FastCondition_free, cond);
}

static void FastCondition_mark(pthread_cond_t *cond)
{
}

static void FastCondition_free(pthread_cond_t *cond)
{
    pthread_cond_destroy(cond);
    xfree(cond);
}

static VALUE FastCondition_signal(VALUE self)
{
    return Qnil;
}

static VALUE FastCondition_wait(VALUE self, VALUE mutex)
{
    return Qnil;
}
