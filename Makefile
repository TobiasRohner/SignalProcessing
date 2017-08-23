CXX        = g++
CPP_VER    = c++14
CXXFLAGS   = -O3 -mavx -std=$(CPP_VER) -Wall -Wextra
CPPFLAGS   = -DNDEBUG
LINKFLAGS  = 
EXEC       = decoder.exe
SRCS       = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard src/**/**/*.cpp)
DOXYFILE   = Doxyfile
INC        = ../../../CppLibs include
LIBS       = -fopenmp
ODIR       = .obj
DOCDIR     = doc

OBJS       = $(foreach d, $(subst .cpp,.o,$(SRCS)), $(ODIR)/$d)
DEPS       = $(foreach d, $(subst .cpp,.d,$(SRCS)), $(ODIR)/$d)
DIRS       = $(sort $(foreach d, $(SRCS), $(ODIR)/$(dir $d)))
INC_PARAMS = $(foreach d, $(INC), -I$d)


program : decoder

doc : documentation
	
.PHONY: clean
clean :
	rm -rf $(ODIR)
	rm -f $(EXEC)
	rm -f $(EXEC).stackdump
	rm -f gmon.out
	

$(DIRS) :
	mkdir -p $@
	
$(ODIR)/%.o : %.cpp | $(SRCS) $(DIRS)
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(CPPFLAGS) $(LIBS) $(INC_PARAMS)
	
$(ODIR)/%.d : %.cpp | $(SRCS) $(DIRS)
	$(CXX) -M $< -o $@ -MT $(subst .d,.o,$@) -std=$(CPP_VER) $(INC_PARAMS) $(LIBS)

decoder : $(OBJS)
	$(CXX) -o $(EXEC) $(OBJS) $(LINKFLAGS)
	
	
documentation :
	doxygen $(DOXYFILE)
	
-include $(DEPS)