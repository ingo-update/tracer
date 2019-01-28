### Makefile for tracer ###

TARGET = tracer
LEXER = $(GENSRC)/lexer.c
PARSER = $(GENSRC)/parser.c
EXTRA_OBJS = $(OBJDIR)/lexer.o $(OBJDIR)/parser.o

include make/build.gmk

CFLAGS_lexer.c = -Wno-unused-function
LDLIBS = -lm -lfl
LDLIBS_test-parser = -lm -lfl

TESTTRACE = $(TESTDIR)/trace.ppm
TESTPPM = $(TESTDIR)/test_asc.ppm

$(TESTPPM): $(TESTDIR)/test-bitmap

$(TESTTRACE): $(TARGET) $(TESTPPM)
	@$(ECHO) Tracing $(notdir $(TESTTRACE))
	@./$(TARGET) -x512 -y512 -w1 -r10 -o $(TESTTRACE) -i examples/test.pov

test-show: $(TESTTRACE)
	$(XV) $(TESTTRACE)

test: $(TESTTRACE)

# Extra source files to print
paper: $(SRC)/lexer.l $(SRC)/parser.y

### Dependencies

## Link dependencies are all object files for the executable

$(addprefix $(BUILDDIR)/,$(TARGET)): $(OBJFILES)

## The first dependency for object files must be their source file

$(OBJDIR)/bitmap.o: $(SRC)/bitmap.c $(SRC)/bitmap.h $(SRC)/color.h
$(OBJDIR)/camera.o: $(SRC)/camera.c $(SRC)/camera.h $(SRC)/vector.h
$(OBJDIR)/color.o: $(SRC)/color.c $(SRC)/color.h $(SRC)/real.h
$(OBJDIR)/hitdata.o: $(SRC)/hitdata.c $(SRC)/hitdata.h $(SRC)/color.h $(SRC)/vector.h $(SRC)/real.h
$(OBJDIR)/light.o: $(SRC)/light.c $(SRC)/light.h $(SRC)/color.h $(SRC)/vector.h
$(OBJDIR)/main.o: $(SRC)/main.c $(SRC)/options.h $(SRC)/world.h $(SRC)/camera.h
$(OBJDIR)/object.o: $(SRC)/object.c $(SRC)/object.h $(SRC)/real.h $(SRC)/sphere.h $(SRC)/plane.h $(SRC)/triangle.h
$(OBJDIR)/options.o: $(SRC)/options.c $(SRC)/options.h $(SRC)/real.h $(SRC)/defaults.h
$(OBJDIR)/plane.o: $(SRC)/plane.c $(SRC)/plane.h $(SRC)/world.h $(SRC)/real.h $(SRC)/color.h $(SRC)/surface.h
$(OBJDIR)/ray.o: $(SRC)/ray.c $(SRC)/ray.h $(SRC)/real.h $(SRC)/vector.h
$(OBJDIR)/sphere.o: $(SRC)/sphere.c $(SRC)/sphere.h $(SRC)/real.h $(SRC)/vector.h $(SRC)/surface.h $(SRC)/ray.h $(SRC)/hitdata.h
$(OBJDIR)/surface.o: $(SRC)/surface.c $(SRC)/surface.h $(SRC)/color.h $(SRC)/bitmap.h $(SRC)/real.h
$(OBJDIR)/trace.o: $(SRC)/trace.c $(SRC)/trace.h $(SRC)/bitmap.h $(SRC)/real.h $(SRC)/world.h $(SRC)/ray.h $(SRC)/hitdata.h $(SRC)/camera.h $(SRC)/options.h
$(OBJDIR)/triangle.o: $(SRC)/triangle.c $(SRC)/triangle.h $(SRC)/world.h $(SRC)/real.h $(SRC)/ray.h $(SRC)/hitdata.h $(SRC)/surface.h
$(OBJDIR)/vector.o: $(SRC)/vector.c $(SRC)/vector.h $(SRC)/real.h
$(OBJDIR)/world.o: $(SRC)/world.c $(SRC)/world.h $(SRC)/hitdata.h $(SRC)/color.h $(SRC)/ray.h $(SRC)/object.h $(SRC)/light.h $(SRC)/options.h $(SRC)/real.h $(SRC)/defaults.h

$(OBJDIR)/lexer.o: $(GENSRC)/lexer.c $(GENSRC)/parser.h
$(OBJDIR)/parser.o: $(GENSRC)/parser.c

$(GENSRC)/lexer.c: $(SRC)/lexer.l
$(GENSRC)/parser.c: $(SRC)/parser.y
$(GENSRC)/parser.h: $(SRC)/parser.y

$(TESTDIR)/test-bitmap.o: $(TOPDIR)/test/test-bitmap.c $(SRC)/bitmap.h $(SRC)/color.h
$(TESTDIR)/test-parser.o: $(TOPDIR)/test/test-parser.c $(GENSRC)/parser.h

$(TESTDIR)/test-bitmap: $(OBJDIR)/bitmap.o $(OBJDIR)/color.o $(TESTDIR)/test-bitmap.o
$(TESTDIR)/test-parser: $(filter-out %/main.o,$(OBJFILES)) $(TESTDIR)/test-parser.o
