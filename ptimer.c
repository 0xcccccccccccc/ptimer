/*
 * Author: Will Eccles
 *
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org/>
 */

#include "ptimer.h"

#include <signal.h>
#include <time.h>
#include <errno.h>
#include <string.h>

int ptimer_create(clockid_t clockid, timer_t* timer, int tsig,
                  void(*callback)(int)) {
  if (tsig <= 0) {
    tsig = SIGALRM;
  }

  timer_t tid;
  int stat;
  sigset_t t_set;
  struct sigaction t_act;

  stat = sigemptyset(&t_set);
  if (stat != 0) {
    return -1;
  }

  stat = sigaddset(&t_set, tsig);
  if (stat != 0) {
    return -1;
  }

  memset(&t_act, 0, sizeof(struct sigaction));
  t_act.sa_flags = SA_RESTART;
  t_act.sa_mask = t_set;
  t_act.sa_handler = callback;

  stat = sigaction(tsig, &t_act, NULL);
  if (stat == -1) {
    return -1;
  }

  stat = timer_create(clockid, NULL, &tid);
  if (stat != 0) {
    return -1;
  }

  *timer = tid;

  return 0;
}

int ptimer_set(timer_t timer, const struct timespec* interval) {
  if (interval == NULL) {
    errno = EINVAL;
    return -1;
  }

  struct itimerspec itv = { *interval, *interval };
  return timer_settime(timer, 0, &itv, NULL);
}

int ptimer_stop(timer_t timer) {
  static struct itimerspec itv = {{0}, {0}};
  return timer_settime(timer, 0, &itv, NULL);
}

int ptimer_destroy(timer_t timer) {
  return timer_delete(timer);
}
