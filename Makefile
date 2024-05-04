NAME = philo

CFLAGS = -Wall -Wextra -Werror -g -pthread

OBJDIR = ./obj/

SRCS = $(shell find ./src -name "*.c")


#Colors

GREEN=\033[1;32m
BLUE=\033[1;36m
YELLOW=\033[1;33m
RED=\033[1;31m
NC=\033[0m # No Color

OBJS = $(addprefix $(OBJDIR), $(notdir $(SRCS:.c=.o)))

$(OBJDIR)%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)Compiled $< successfully!$(NC)"

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@
	@echo "$(BLUE)$(NAME) created successfully!$(NC)"

clean:
	@rm -rf $(OBJDIR)
	@echo "$(YELLOW)$(NAME) -> Object files removed!$(NC)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)$(NAME) executable removed!$(NC)"

re: fclean all

.PHONY: all clean fclean re