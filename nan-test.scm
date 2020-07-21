;;; Copyright (C) 2020 Wolfgang Corcoran-Mathe

;;; Permission is hereby granted, free of charge, to any person obtaining a
;;; copy of this software and associated documentation files (the
;;; "Software"), to deal in the Software without restriction, including
;;; without limitation the rights to use, copy, modify, merge, publish,
;;; distribute, sublicense, and/or sell copies of the Software, and to
;;; permit persons to whom the Software is furnished to do so, subject to
;;; the following conditions:

;;; The above copyright notice and this permission notice shall be included
;;; in all copies or substantial portions of the Software.

;;; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
;;; OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
;;; MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
;;; IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
;;; CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
;;; TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
;;; SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

(import (scheme base))
(import (scheme write))
(import (nan))

(cond-expand
  ((library (srfi 78))
   (import (srfi 78)))
  (else
    (begin
      (define *tests-failed* 0)
      (define-syntax check
        (syntax-rules (=>)
          ((check expr)
           (check expr => #t))
          ((check expr => expected)
           (if (equal? expr expected)
             (begin
               (display 'expr)
               (display " => ")
               (display expected)
               (display " ; correct")
               (newline))
             (begin
               (set! *tests-failed* (+ *tests-failed* 1))
               (display "FAILED: for ")
               (display 'expr)
               (display " expected ")
               (display expected)
               (display " but got ")
               (display expr)
               (newline))))))
      (define (check-report)
        (if (zero? *tests-failed*)
            (begin
             (display "All tests passed.")
             (newline))
            (begin
             (display "TESTS FAILED: ")
             (display *tests-failed*)
             (newline)))))))

;;;; Test NaN bytevectors

;;; All big-endian, since the exported NaN procedures convert their
;;; arguments to big-endian bytevectors.

(define quiet-negative (bytevector #xff #xf8 0 0 0 0 0 0))
(define quiet-positive (bytevector #x7f #xf8 0 0 0 0 0 0))
(define signaling-positive (bytevector #x7f #xf0 0 0 0 0 0 0))
(define signaling-negative (bytevector #xff #xf0 0 0 0 0 0 0))
(define all-ones-payload
  (bytevector #x7f #xff #xff #xff #xff #xff #xff #xff))

(define (check-all)
  (check (nan-bytevector-negative? quiet-negative)     => #t)
  (check (nan-bytevector-negative? signaling-negative) => #t)
  (check (nan-bytevector-negative? quiet-positive)     => #f)
  (check (nan-bytevector-negative? signaling-positive) => #f)

  (check (nan-bytevector-quiet? quiet-negative)     => #t)
  (check (nan-bytevector-quiet? signaling-negative) => #f)
  (check (nan-bytevector-quiet? quiet-positive)     => #t)
  (check (nan-bytevector-quiet? signaling-positive) => #f)

  (check (nan-bytevector-payload quiet-negative)   => 0)
  (check (nan-bytevector-payload all-ones-payload) => #x7ffffffffffff)

  (check (nan-bytevector= quiet-negative quiet-negative)     => #t)
  (check (nan-bytevector= quiet-negative quiet-positive)     => #f)
  (check (nan-bytevector= quiet-negative all-ones-payload)   => #f)
  (check (nan-bytevector= quiet-negative signaling-negative) => #f))

(check-all)
(newline)
(check-report)
