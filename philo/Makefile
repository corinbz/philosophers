NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -g #-pthread -fsanitize=thread

OBJDIR = obj/

SRCS		= main.c \
			sim_init.c \
			sim_mutex.c \
			sim_monitor.c \
			philo_init.c \
			philo_status_check.c \
			philo_actions.c \
			philo_routine.c \
			philo_status.c \
			utils_time.c \
			utils_input.c \
			utils_memory.c \
			cleanup.c \
			cleanup_mutex.c \
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
