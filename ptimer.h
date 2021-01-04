/*
 * Author: Will Eccles
 *
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org/>
 */

#ifndef PTIMER_WRAPPER_H_
#define PTIMER_WRAPPER_H_

#include <signal.h>
#include <time.h>

/*
 * Creates a timer. Specify a clock ID (such as CLOCK_MONOTONIC). The initialized
 * timer is stored in the timer_t pointed to by 'timer'. The process will
 * receive the specified signal (tsig). A signal value less than or equal to
 * zero will default the timer to using SIGALRM.
 *
 * The callback function specified for the timer will receive tsig as an
 * argument, as it is a signal handler.
 *
 * On success, 0 is returned and the value pointed to by 'timer' is updated.
 * On error, -1 is returned, the value pointed to by 'timer' is unchanged, and
 * errno is set accordingly.
 *
 * Wrapper for timer_create(2). If you wish to change this behavior, call
 * timer_create(2) directly.
 */
int ptimer_create(clockid_t clockid, timer_t* timer, int tsig,
                  void(*callback)(int));

/*
 * Sets a timer's value, starting the timer or restarting it if necessary. The
 * timeout should be set in 'interval'. The interval will be used for the
 * initial value as well, meaning that the timer will not fire until after the
 * interval is completed at least once. It will not fire immediately.
 *
 * On success, 0 is returned and the timer is started. On error, -1 is returned
 * and errno is set accordingly.
 *
 * This is a wrapper for timer_settime(2).
 */
int ptimer_set(timer_t timer, const struct timespec* interval);

/*
 * Stops a timer.
 *
 * On success, 0 is returned. Else, -1 is returned and errno is updated
 * accordingly.
 *
 * This is a wrapper around timer_settime(2) with a zeroed itimerspec.
 */
int ptimer_stop(timer_t timer);

/*
 * Destroy a timer created with ptimer_create. If the timer is armed, it will be
 * disarmed before it is destroyed.
 *
 * On success, 0 is returned. On error, -1 is returned and errno is updated.
 *
 * This is a wrapper around timer_delete(2).
 */
int ptimer_destroy(timer_t timer);

#endif // PTIMER_WRAPPER_H_
