### Makefile for tracer ###

PROJECT = tracer
TARGET = tracer

LEXER = $(GENSRC)/lexer.c
PARSER = $(GENSRC)/parser.c
EXTRA_OBJS = $(OBJDIR)/lexer.o $(OBJDIR)/parser.o

ifndef MAKEDIR
  MAKEDIR = ../make
endif

include $(MAKEDIR)/main.gmk

CFLAGS_lexer.o = -Wno-unused-function
LDLIBS_tracer = -lm -lfl
LDLIBS_test-parser = -lm -lfl
LDLIBS_test-vector = -lm

TESTTRACE = $(TESTDIR)/trace.ppm

$(TESTTRACE): $(TARGET) examples/test.pov $(TESTDIR)/test-bitmap.testresult
	@$(ECHO) Tracing $(notdir $(TESTTRACE))
	@./$(TARGET) -x512 -y512 -w1 -r16 -o $(TESTTRACE) -i examples/test.pov

test: $(TESTTRACE)

test-show: test
	$(XV) $(TESTTRACE) &

# Extra source files to print
paper: $(SRC)/lexer.l $(SRC)/parser.y

### Dependencies

## Link dependencies are all object files for the executable

$(addprefix $(BUILDDIR)/,$(TARGET)): $(OBJFILES)

## The first dependency for object files must be their source file

$(OBJDIR)/bitmap.o: $(SRC)/bitmap.c $(SRC)/bitmap.h $(SRC)/color.h
$(OBJDIR)/camera.o: $(SRC)/camera.c $(SRC)/camera.h $(SRC)/vector.h $(SRC)/types.h
$(OBJDIR)/color.o: $(SRC)/color.c $(SRC)/color.h $(SRC)/types.h
$(OBJDIR)/hitdata.o: $(SRC)/hitdata.c $(SRC)/hitdata.h $(SRC)/color.h $(SRC)/vector.h $(SRC)/types.h
$(OBJDIR)/light.o: $(SRC)/light.c $(SRC)/light.h $(SRC)/color.h $(SRC)/vector.h $(SRC)/types.h
$(OBJDIR)/main.o: $(SRC)/main.c $(SRC)/options.h $(SRC)/world.h $(SRC)/camera.h $(SRC)/types.h
$(OBJDIR)/object.o: $(SRC)/object.c $(SRC)/object.h $(SRC)/types.h $(SRC)/sphere.h $(SRC)/plane.h
$(OBJDIR)/options.o: $(SRC)/options.c $(SRC)/options.h $(SRC)/types.h $(SRC)/defaults.h
$(OBJDIR)/plane.o: $(SRC)/plane.c $(SRC)/plane.h $(SRC)/world.h $(SRC)/types.h $(SRC)/color.h $(SRC)/surface.h $(SRC)/hitdata.h $(SRC)/ray.h
$(OBJDIR)/ray.o: $(SRC)/ray.c $(SRC)/ray.h $(SRC)/types.h $(SRC)/vector.h
$(OBJDIR)/sphere.o: $(SRC)/sphere.c $(SRC)/sphere.h $(SRC)/types.h $(SRC)/vector.h $(SRC)/surface.h $(SRC)/ray.h $(SRC)/hitdata.h
$(OBJDIR)/surface.o: $(SRC)/surface.c $(SRC)/surface.h $(SRC)/color.h $(SRC)/bitmap.h $(SRC)/types.h
$(OBJDIR)/trace.o: $(SRC)/trace.c $(SRC)/trace.h $(SRC)/bitmap.h $(SRC)/types.h $(SRC)/world.h $(SRC)/ray.h $(SRC)/hitdata.h $(SRC)/camera.h $(SRC)/options.h
$(OBJDIR)/vector.o: $(SRC)/vector.c $(SRC)/vector.h $(SRC)/types.h
$(OBJDIR)/world.o: $(SRC)/world.c $(SRC)/world.h $(SRC)/hitdata.h $(SRC)/color.h $(SRC)/ray.h $(SRC)/object.h $(SRC)/light.h $(SRC)/options.h $(SRC)/types.h $(SRC)/defaults.h

$(OBJDIR)/lexer.o: $(GENSRC)/lexer.c $(GENSRC)/parser.h
$(OBJDIR)/parser.o: $(GENSRC)/parser.c $(SRC)/sphere.h $(SRC)/plane.h $(SRC)/vector.h $(SRC)/types.h $(SRC)/color.h $(SRC)/world.h $(SRC)/camera.h $(SRC)/ray.h

$(GENSRC)/lexer.c: $(SRC)/lexer.l
$(GENSRC)/parser.c $(GENSRC)/parser.h: $(SRC)/parser.y $(SRC)/vector.h $(SRC)/color.h $(SRC)/world.h $(SRC)/camera.h $(SRC)/types.h $(SRC)/defaults.h

## Test objects

$(TESTDIR)/test-bitmap.o: $(TOPDIR)/test/test-bitmap.c $(SRC)/bitmap.h $(SRC)/color.h
$(TESTDIR)/test-parser.o: $(TOPDIR)/test/test-parser.c $(GENSRC)/parser.h $(SRC)/plane.h $(SRC)/sphere.h
$(TESTDIR)/test-vector.o: $(TOPDIR)/test/test-vector.c $(SRC)/types.h
$(TESTDIR)/test-color.o: $(TOPDIR)/test/test-color.c $(SRC)/color.h
$(TESTDIR)/test-surface.o: $(TOPDIR)/test/test-surface.c $(SRC)/surface.h $(SRC)/defaults.h $(SRC)/types.h

## Test executables

$(TESTDIR)/test-bitmap: $(TESTDIR)/test-bitmap.o $(OBJDIR)/bitmap.o $(OBJDIR)/color.o
$(TESTDIR)/test-parser: $(TESTDIR)/test-parser.o $(filter-out %/main.o %/trace.o,$(OBJFILES))
$(TESTDIR)/test-vector: $(TESTDIR)/test-vector.o $(OBJDIR)/vector.o
$(TESTDIR)/test-color: $(TESTDIR)/test-color.o $(OBJDIR)/color.o
$(TESTDIR)/test-surface:  $(TESTDIR)/test-surface.o $(OBJDIR)/surface.o $(OBJDIR)/bitmap.o $(OBJDIR)/color.o

## Tests that use artifacts from other tests

$(TESTDIR)/test-surface.testresult: $(TESTDIR)/test-bitmap.testresult
$(TESTDIR)/test-parser.testresult: $(TESTDIR)/test-bitmap.testresult
