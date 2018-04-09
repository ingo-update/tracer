# Makefile for tracer

CC = gcc -c
LD = gcc
RM = rm -rf
ECHO = echo
MKDIR = mkdir -p
CP = cp
LEX = flex
YACC = bison
CD = cd
XV = gpicview

CFLAGS = -Wall -Werror
LDFLAGS = -Wall -Werror
LDLIBS = -lm -lfl

OPT_CFLAGS = -O4
DBG_CFLAGS = -g

CFLAGS_lexer.c = -I$(GENSRC) -Wno-unused-function

TARGET = tracer

TOPDIR = $(shell pwd)
SRC = $(TOPDIR)/src
BUILDDIR = $(TOPDIR)/build
OBJDIR = $(BUILDDIR)/objs
GENSRC = $(BUILDDIR)/gensrc
TEST = $(BUILDDIR)/test

LEXER = $(GENSRC)/lexer.c
PARSER = $(GENSRC)/parser.c

OBJFILES = $(sort \
               $(patsubst $(SRC)/%.c,$(OBJDIR)/%.o,$(wildcard $(SRC)/*.c)) \
               $(patsubst $(GENSRC)/%.c,$(OBJDIR)/%.o,$(LEXER) $(PARSER)) \
               )

TESTOBJS = $(patsubst $(TOPDIR)/test/%.c,$(TEST)/%.o,$(wildcard $(TOPDIR)/test/*.c))

default: $(TARGET)
$(TARGET): $(addprefix $(BUILDDIR)/,$(TARGET))
	@$(CP) $^ $(TOPDIR)

OBJ_CMD = $(CC) -I$(SRC) $(CFLAGS) $(OPT_CFLAGS) $(DBG_CFLAGS) $(CFLAGS_$(notdir $<)) $< -o $@
$(OBJFILES):
	@$(MKDIR) $(dir $@)
	@$(ECHO) Compiling $(notdir $<)
	@$(ECHO) '$(OBJ_CMD)' > $@.cmdline
	@$(OBJ_CMD) 2> $@.log
	@[ -s $@.log ] || $(RM) $@.log

TARGET_CMD = $(LD) -o $@ $(LDFLAGS) $^ $(LDLIBS)
$(addprefix $(BUILDDIR)/,$(TARGET)):
	@$(MKDIR) $(dir $@)
	@$(ECHO) Linking $(notdir $@)
	@$(ECHO) '$(TARGET_CMD)' > $@.cmdline
	@$(TARGET_CMD) 2> $@.log
	@[ -s $@.log ] || $(RM) $@.log

clean:
	@$(RM) $(BUILDDIR) $(TARGET)

sweep: clean
	@$(RM) $(SRC)/*~ $(TEST)/*~ *~

test: test_bitmap test_trace

test_bitmap: $(TEST)/test_bitmap
	@$(CD) $(dir $<) && $<

test_trace: $(TARGET) test_bitmap
	@./$(TARGET) -x512 -y512 -w1 -r10 -o build/test/a.ppm -i examples/test.pov
	@$(XV) build/test/a.ppm &

TESTOBJ_CMD = $(CC) -I$(SRC) $(CFLAGS) $(OPT_CFLAGS) $(DBG_CFLAGS) $(CFLAGS_$(notdir $<)) $< -o $@
$(TESTOBJS): 
	@$(MKDIR) $(dir $@)
	@$(ECHO) Compiling $(notdir $<)
	@$(ECHO) '$(TESTOBJ_CMD)' > $@.cmdline
	@$(TESTOBJ_CMD) 2> $@.log
	@[ -s $@.log ] || $(RM) $@.log

$(TEST)/test_bitmap: $(OBJDIR)/bitmap.o $(OBJDIR)/color.o $(TEST)/test_bitmap.o
	@$(LD) -o $@ $(LDFLAGS) $^

LEX_CMD = $(LEX) -t $< > $@
$(GENSRC)/lexer.c:
	@$(MKDIR) $(dir $@)
	@$(ECHO) Generating $(notdir $@)
	@$(ECHO) '$(LEX_CMD)' > $@.cmdline
	@$(LEX_CMD) 2> $@.log
	@[ -s $@.log ] || $(RM) $@.log

YACC_CMD = $(YACC) -d -o $(GENSRC)/parser.c $<
$(GENSRC)/parser.c $(GENSRC)/parser.h:
	@$(MKDIR) $(dir $@)
	@$(ECHO) Generating parser.h, parser.c
	@$(ECHO) '$(YACC_CMD)' > $@.cmdline
	@$(YACC_CMD) 2> $@.log
	@[ -s $@.log ] || $(RM) $@.log

## Dependencies
$(addprefix $(BUILDDIR)/,$(TARGET)): $(OBJFILES)

$(GENSRC)/lexer.c: $(SRC)/lexer.l
$(GENSRC)/parser.c: $(SRC)/parser.y
$(GENSRC)/parser.h: $(SRC)/parser.y

$(OBJDIR)/bitmap.o: $(SRC)/bitmap.c $(SRC)/bitmap.h $(SRC)/color.h 
$(OBJDIR)/camera.o: $(SRC)/camera.c $(SRC)/camera.h $(SRC)/vector.h
$(OBJDIR)/color.o: $(SRC)/color.c $(SRC)/color.h $(SRC)/real.h
$(OBJDIR)/hitdata.o: $(SRC)/hitdata.c $(SRC)/hitdata.h $(SRC)/color.h $(SRC)/vector.h $(SRC)/surface.h $(SRC)/real.h
$(OBJDIR)/light.o: $(SRC)/light.c $(SRC)/light.h $(SRC)/color.h $(SRC)/vector.h
$(OBJDIR)/main.o: $(SRC)/main.c $(SRC)/options.h $(SRC)/world.h $(SRC)/camera.h
$(OBJDIR)/object.o: $(SRC)/object.c $(SRC)/object.h $(SRC)/real.h $(SRC)/sphere.h $(SRC)/plane.h $(SRC)/triangle.h $(SRC)/pgram.h
$(OBJDIR)/options.o: $(SRC)/options.c $(SRC)/options.h $(SRC)/real.h
$(OBJDIR)/pgram.o: $(SRC)/pgram.c $(SRC)/pgram.h $(SRC)/world.h $(SRC)/real.h $(SRC)/ray.h $(SRC)/hitdata.h $(SRC)/surface.h
$(OBJDIR)/plane.o: $(SRC)/plane.c $(SRC)/plane.h $(SRC)/world.h $(SRC)/real.h $(SRC)/color.h $(SRC)/surface.h
$(OBJDIR)/ray.o: $(SRC)/ray.c $(SRC)/ray.h $(SRC)/real.h $(SRC)/vector.h
$(OBJDIR)/sphere.o: $(SRC)/sphere.c $(SRC)/sphere.h $(SRC)/real.h $(SRC)/vector.h $(SRC)/surface.h $(SRC)/ray.h $(SRC)/hitdata.h
$(OBJDIR)/surface.o: $(SRC)/surface.c $(SRC)/surface.h $(SRC)/color.h $(SRC)/bitmap.h $(SRC)/real.h
$(OBJDIR)/trace.o: $(SRC)/trace.c $(SRC)/trace.h $(SRC)/bitmap.h $(SRC)/real.h $(SRC)/world.h $(SRC)/ray.h $(SRC)/hitdata.h $(SRC)/camera.h $(SRC)/options.h
$(OBJDIR)/triangle.o: $(SRC)/triangle.c $(SRC)/triangle.h $(SRC)/world.h $(SRC)/real.h $(SRC)/ray.h $(SRC)/hitdata.h $(SRC)/surface.h
$(OBJDIR)/vector.o: $(SRC)/vector.c $(SRC)/vector.h $(SRC)/real.h
$(OBJDIR)/world.o: $(SRC)/world.c $(SRC)/world.h $(SRC)/hitdata.h $(SRC)/color.h $(SRC)/ray.h $(SRC)/object.h $(SRC)/light.h $(SRC)/options.h $(SRC)/real.h

$(OBJDIR)/lexer.o: $(GENSRC)/lexer.c $(GENSRC)/parser.h
$(OBJDIR)/parser.o: $(GENSRC)/parser.c

$(TEST)/test_bitmap.o: $(TOPDIR)/test/test_bitmap.c $(SRC)/bitmap.h $(SRC)/color.h
