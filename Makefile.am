## makefile.am
## Copyright 2017 Mac Radigan
## All Rights Reserved

AM_LDFLAGS  = -L/usr/lib/x86_64-linux-gnu -lgflags                            

AM_CPPFLAGS = -I$(top_srcdir)/include                                        \
              -std=c++1z
AM_FCFLAGS  = -Jmod

#myenv_ldadd              = libmyenv.la                                        

bin_PROGRAMS             = myenv                                              

myenv_SOURCES            = src/myenv.cc
#myenv_LDADD              = $(myenv_ldadd)

#lib_LTLIBRARIES          = libmyenv.la
#libmyenv_la_SOURCES      =                                                    

#libmyenv_la_LIBADD       =                                                    

#libmyenv_la_LDFLAGS      = -export-symbols-regex '^myenv_[^_]' -fPIC         \
#                           -shared -pthread                                   

#pkginclude_HEADERS       =

## *EOF*
