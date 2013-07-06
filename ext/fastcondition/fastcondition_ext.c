#include "ruby.h"

#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#include <assert.h>

#ifdef HAVE_RUBY_THREAD_H
#include "ruby/thread.h"
#endif

struct fast_condition_struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

static VALUE cFastCondition = Qnil;

static VALUE FastCondition_allocate(VALUE klass);
static void FastCondition_mark(struct fast_condition_struct *cond);
static void FastCondition_free(struct fast_condition_struct *cond);

static VALUE FastCondition_signal(VALUE self);
static VALUE FastCondition_wait(VALUE self, VALUE mutex);

#ifdef HAVE_RUBY_THREAD_H
static void *FastCondition_unlocked_wait(void *ptr);
#else
static VALUE FastCondition_unlocked_wait(void *ptr);
#endif

void Init_fastcondition_ext()
{
    cFastCondition = rb_define_class("FastCondition", rb_cObject);
    rb_define_alloc_func(cFastCondition, FastCondition_allocate);

    rb_define_method(cFastCondition, "signal", FastCondition_signal, 0);
    rb_define_method(cFastCondition, "wait", FastCondition_wait, 1);
}

static VALUE FastCondition_allocate(VALUE klass)
{
    struct fast_condition_struct *cond;

    cond = (struct fast_condition_struct *)xmalloc(sizeof(struct fast_condition_struct));
    pthread_mutex_init(&cond->mutex, NULL);
    pthread_cond_init(&cond->cond, NULL);

    return Data_Wrap_Struct(klass, FastCondition_mark, FastCondition_free, cond);
}

static void FastCondition_mark(struct fast_condition_struct *cond)
{
}

static void FastCondition_free(struct fast_condition_struct *cond)
{
    pthread_mutex_destroy(&cond->mutex);
    pthread_cond_destroy(&cond->cond);
    xfree(cond);
}

static VALUE FastCondition_signal(VALUE self)
{
    struct fast_condition_struct *cond;
    Data_Get_Struct(self, struct fast_condition_struct, cond);

    pthread_cond_signal(&cond->cond);
    return self;
}

static VALUE FastCondition_wait(VALUE self, VALUE mutex)
{
    struct fast_condition_struct *cond;
    Data_Get_Struct(self, struct fast_condition_struct, cond);

    /* Assume the mutex is uncontended and try a nonblocking operation to
       aquire it. This sidesteps the need to release the GVL first. This is
       written with the assumption that this mutex is uncontended, but that
       may not be the case. If this assumption is violated, the assertion will
       blow up */
    assert(pthread_mutex_trylock(&cond->mutex) == 0);
    rb_mutex_unlock(mutex);

    /* FIXME: RUBY_UBF_IO is probably not appropriate here. We should signal the pthread_cond_t
       in order to unblock it. This might cause problems inside signal handlers */
#ifdef HAVE_RUBY_THREAD_H
    rb_thread_call_without_gvl(FastCondition_unlocked_wait, (void *)cond, RUBY_UBF_IO, 0);
#else
    rb_thread_blocking_region(FastCondition_unlocked_wait, (void *)cond, RUBY_UBF_IO, 0);
#endif

    rb_mutex_lock(mutex);
    assert(pthread_mutex_unlock(&cond->mutex) == 0);

    return Qnil;
}

#ifdef HAVE_RUBY_THREAD_H
static void *FastCondition_unlocked_wait(void *ptr)
#else
static VALUE FastCondition_unlocked_wait(void *ptr)
#endif
{
    struct fast_condition_struct *cond = (struct fast_condition_struct *)ptr;

    assert(pthread_cond_wait(&cond->cond, &cond->mutex) == 0);

#ifdef HAVE_RUBY_THREAD_H
    return NULL;
#else
    return Qnil;
#endif
}

#else
/* Noop if we don't have pthreads */
void Init_fastcondition_ext()
{
}
#endif