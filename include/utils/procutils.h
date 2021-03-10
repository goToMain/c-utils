/*
 * Copyright (c) 2020-2021 Siddharth Chandrasekaran <siddharth@embedjournal.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _UTIL_PROCUTILS_H_
#define _UTIL_PROCUTILS_H_

/*
 * read_pid() - Reads PID from `file`. The file's content must just be
 * be PID number, as written by a call to write_pid(). On success, copies
 * the PID read from file into `pid`.
 *
 * Returns:
 *  0 - Success
 * -1 - Failure
 */
int read_pid(const char *file, int *pid);

/*
 * write_pid() - Writes the calling process's PID to the file specified.
 *
 * Returns:
 *  0 - Success
 * -1 - Failure
 */
int write_pid(const char *file);

/*
 * o_redirect() - Redirects stdout/stderr to file specified.
 *
 * Params:
 *   mode - Bit mask.
 *           bit-0: STDOUT
 *           bit-1: STDERR
 *   file - Path to log file. if null, close() is called on the
 *          corresponding FDs (from mode above).
 *  Examples:
 *    o_redirect(1, "/tmp/my_log.txt"); // redirects stdout to "/tmp/my_log.txt"
 *    o_redirect(3, "/tmp/my_log.txt"); // Same as above; but redirects stdout and strerr
 *    o_redirect(3, NULL); // close both stdout and strerr. Same as `> /dev/null 1>&2`
 */
int o_redirect(int mode, const char *file);

#endif /* _UTIL_PROCUTILS_H_ */