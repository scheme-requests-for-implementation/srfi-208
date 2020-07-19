;;; Copyright (C) 2020 Wolfgang Corcoran-Mathe
;;;
;;; Permission is hereby granted, free of charge, to any person obtaining a
;;; copy of this software and associated documentation files (the
;;; "Software"), to deal in the Software without restriction, including
;;; without limitation the rights to use, copy, modify, merge, publish,
;;; distribute, sublicense, and/or sell copies of the Software, and to
;;; permit persons to whom the Software is furnished to do so, subject to
;;; the following conditions:
;;;
;;; The above copyright notice and this permission notice shall be included
;;; in all copies or substantial portions of the Software.
;;;
;;; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
;;; OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
;;; MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
;;; IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
;;; CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
;;; TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
;;; SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

(define (%nan->bytevector nan)
  (let ((bvec (make-bytevector 8)))
    (bytevector-ieee-double-set! bvec 0 nan (endianness 'big))
    bvec))

(define (nan-negative? nan)
  (assume (nan? nan))
  (let ((bvec (%nan->bytevector nan)))
    (bit-set? 7 (bytevector-u8-ref bvec 0))))

(define (nan-quiet? nan)
  (assume (nan? nan))
  (let ((bvec (%nan->bytevector nan)))
    (bit-set? 3 (bytevector-u8-ref bvec 1))))

(define (nan-payload nan)
  (assume (nan? nan))
  (let ((whole-nan (%nan->bytevector nan))
        (payload (make-bytevector 7 0)))
    (bytevector-u8-set! payload 0 (bitwise-and #x7
                                               (bytevector-u8-ref whole-nan 1)))
    (bytevector-copy! payload 1 whole-nan 2)
    (bytevector-sint-ref payload 0 (endianness 'big) 7)))

(define (nan= nan1 nan2)
  (assume (nan? nan1))
  (assume (nan? nan2))
  (bytevector=? (%nan->bytevector nan1) (%nan->bytevector nan2)))
