OBJDIR := obj
SRCDIR := src
OBJS := $(addprefix $(OBJDIR)/,CPGM.o main.o)
CFLAGS :=
#CFLAGS := -Wall -Wextra -pedantic

main: $(OBJS)
	@gcc -o icp1102_01 $(OBJS)
	@echo Building icp1102_01

$(OBJDIR)/%.o:  $(SRCDIR)/%.c | $(OBJDIR)
	@gcc -c $(CFLAGS) $(SRCDIR)/$*.c -o $(OBJDIR)/$*.o
	@echo Building $*.o

$(OBJDIR):
	@test -d $(OBJDIR) || mkdir $(OBJDIR)
	@echo Create $(OBJDIR) directory
