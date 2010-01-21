bin_PROGRAMS = aqua

AM_CPPFLAGS = -DDATADIR=\""$(pkgdatadir)"\" -DGL_GLEXT_PROTOTYPES
aqua_LDADD = -lGL -lGLU -lboost_filesystem -lSDL -lSDL_image 
aqua_SOURCES = aqua.cpp model.hpp model.cpp modellib.hpp modellib.cpp \
               fish.hpp fish.cpp aquarium.hpp aquarium.cpp
