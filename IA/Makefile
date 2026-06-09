##
## EPITECH PROJECT, 2026
## Zappy AI
## File description:
## Makefile
##

NAME	=	zappy_ai

all:	$(NAME)

$(NAME):
	@echo "#!/bin/bash" > $(NAME)
	@echo "python3 main.py \"\$$@\"" >> $(NAME)
	@chmod +x $(NAME)

clean:
	@rm -rf __pycache__
	@rm -rf src/__pycache__
	@rm -rf src/agent_submodules/__pycache__

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
