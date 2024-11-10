NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -pthread
MODE = none
ifeq ($(MODE), debug)
	CFLAGS	+= -fsanitize=thread -g
endif

-fsanitize=thread

OBJDIR = obj/

SRCS		= main.c \
			cleanup.c \
			print_utils.c \
			init_data.c \
			init_philos.c \
			memory_utils.c \
			monitor_routine.c \
			parsing_utils.c \
			philo_routine.c \
			start_sim.c \
			time_utils.c \
# Colors
GREEN = \033[1;32m
BLUE = \033[1;36m
YELLOW = \033[1;33m
RED = \033[1;31m
NC = \033[0m # No Color

OBJS = $(addprefix $(OBJDIR), $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@
	@echo "$(BLUE)$(NAME) created successfully!$(NC)"

$(OBJDIR)%.o: %.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)Compiled $< successfully!$(NC)"

clean:
	@rm -rf $(OBJDIR)
	@echo "$(YELLOW)$(NAME) -> Object files removed!$(NC)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)$(NAME) executable removed!$(NC)"

re: fclean all

.PHONY: all clean fclean re
