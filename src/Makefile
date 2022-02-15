CC			:= gcc
CFLAGS		:= -O2 -Wall -MP -MMD -I../include 
SRC			:= main.c apply_status.c color.c draw_object.c locus_list.c matrix.c solve_equation.c judge_condition.c
OBJS		:= $(patsubst %.c,%.o,$(SRC))
DEPS		:= $(patsubst %.c,%.d,$(SRC))
OUTPUT		:= hogemon_ranger
UNAME		:= $(shell uname | tr '[:lower:]' '[:upper:]')

ifeq ($(UNAME),DARWIN)
FRAMEWORKS	:= -framework OpenGL -framework GLUT
else ifeq ($(findstring CYGWIN, $(UNAME)),CYGWIN)
FRAMEWORKS	:= -lopengl32 -lglu32 -lglut
else
FRAMEWORKS	:= -lopengl32 -lglu32 -lfreeglut
endif

.PHONY: all
all: $(OUTPUT)

-include $(DEPS)

$(OUTPUT): $(OBJS)
	$(CC) $^ $(FRAMEWORKS) -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	$(RM) -f $(DEPS) $(OBJS) $(OUTPUT) $(addsuffix .exe,$(OUTPUT))
