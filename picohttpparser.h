/*
 * Copyright (c) 2009-2014 Kazuho Oku, Tokuhiro Matsuno, Daisuke Murase
 *
 * The software is licensed under either the MIT License (below) or the Perl
 * license.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef picohttpparser_h
#define picohttpparser_h

/* $Id$ */

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* contains name and value of a header (name == NULL if is a continuing line
 * of a multiline header */
struct phr_header {
  const char* name;
  size_t name_len;
  const char* value;
  size_t value_len;
};

/* returns number of bytes cosumed if successful, -2 if request is partial,
 * -1 if failed */
int phr_parse_request(const char* buf, size_t len, const char** method,
                      size_t* method_len, const char** path,
                      size_t* path_len, int* minor_version,
                      struct phr_header* headers, size_t* num_headers,
                      size_t last_len);

/* ditto */
int phr_parse_response(const char* _buf, size_t len, int *minor_version,
              int *status, const char **msg, size_t *msg_len,
              struct phr_header* headers, size_t* num_headers,
              size_t last_len);

/* ditto */
int phr_parse_headers(const char* buf, size_t len, struct phr_header* headers,
                      size_t* num_headers, size_t last_len);

/* should be zero-filled before start */
struct phr_chunked_decoder {
  size_t pos;
  size_t chunk_data_size;
  int state;
};

/* removes the headers of the chunked encoding from the buffer.  Users should
 * repeatedly call this function while it returns -2 with newly arrived data
 * appended to the end of the buffer.  Upon success, the function returns the
 * length of the decoded content (which starts at `buf`), or returns -1 if
 * failed. */
ssize_t phr_decode_chunked(struct phr_chunked_decoder *decoder, char *buf,
                           size_t *bufsz);

#ifdef __cplusplus
}
#endif

#endif
