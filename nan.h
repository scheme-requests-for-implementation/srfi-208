bool nan_negative(double);
bool nan_quiet(double);
unsigned long nan_payload(double);
bool nan_equal(double, double);
double make_nan(bool, bool, unsigned long);

bool fnan_negative(float);
bool fnan_quiet(float);
unsigned int fnan_payload(float);
bool fnan_equal(float, float);
float fmake_nan(bool, bool, unsigned int);
