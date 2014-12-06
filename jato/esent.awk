#
# awk script to extract error codes from esent.h and format them as an initializer list
#
# use: awk -f esent.awk <esent.h >esent_errors.h
#
/^#define JET_(err|wrn)/	{ print "{ " $2 ", \"" $2 "\" }," }
