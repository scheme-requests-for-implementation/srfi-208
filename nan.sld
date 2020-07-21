(define-library (nan)
  (import (scheme base)
          (scheme inexact)
          (srfi 151))

  (cond-expand
    #;((library (scheme bytevector))
     (import (scheme bytevector)))
    (else
     (import (r6rs bytevectors))))

  (cond-expand
    ((library (srfi 145))
     (import (srfi 145)))
    (else
     (begin
      (define (assume . _) #t))))

  (export make-nan nan-negative? nan-quiet? nan-payload nan=)

  (include "nan.scm"))
