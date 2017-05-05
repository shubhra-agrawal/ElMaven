TEMPLATE = subdirs
CONFIG += ordered qt thread

#Faster build + c++14 ++ OpenMP
#QMAKE_CXXFLAGS += -Ofast

SUBDIRS +=	\
		libneural \
        libcdfread \
		libcsvparser\
		pugixml/src \
		libmaven \
        libpillow\
        libpls \
    	mzroll \
        peakdetector \
        MavenTests \
        CrashReporter
