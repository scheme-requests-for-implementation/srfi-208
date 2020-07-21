# NaN

This is a tiny NaN library for Scheme based on a
[pre-SRFI](https://bitbucket.org/cowan/r7rs-wg1-infra/src/default/NaNMedernach.md).
It should be portable to all R7RS Schemes.

# Tests

Since the various kinds of NaNs can't be portably synthesized, the tests
for this library are made against bytevector representations of these NaNs.
Thus, the procedures exported for testing operate on bytevectors, while
those exported for normal use take actual NaN arguments.

To run the tests, load the nan-test.scm file into a Scheme REPL with
the feature identifier `test` defined.  This is often done by passing
an option such as `-D test` on the command line.
