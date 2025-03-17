# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lmeubrin <lmeubrin@student.42berlin.       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/22 15:55:27 by lmeubrin          #+#    #+#              #
#    Updated: 2025/03/17 13:26:02 by lmeubrin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL := /bin/bash
.ONESHELL:
# change the Makefile for mlx
MAKEFILES := lib/libft/Makefile lib/mlx/Makefile
MAKEFLAGS += --no-print-directory --warn-undefined-variables
# Detect the operating system
UNAME_S := $(shell uname -s)

#headers directories
SRCS_DIR	:= srcs
OBJ_DIR		:= objs
HDRS_DIR	:= include
LIBFT_DIR	:= lib/libft
MLX_DIR		:= lib/mlx

CC := cc
NAME := miniRT
INCLUDES := -I$(HDRS_DIR) -I$(LIBFT_DIR) -I$(MLX_DIR)

# Set MLX_FLAGS based on the operating system
ifeq ($(UNAME_S), Linux)
    MLX_FLAGS = $(LINUX_MLX)
else ifeq ($(UNAME_S), Darwin)
    MLX_FLAGS = $(MAC_MLX)
endif

#flags
CFLAGS := -Werror -Wall -Wextra -g
LDFLAGS :=
OPTIM_FLAGS := -Ofast
LINUX_MLX := -ldl -lglfw(3) -pthread -lm
MAC_MLX := -lglfw(3) -framework Cocoa -framework OpenGL -framework IOKit

FINAL_CFLAGS = $(CFLAGS)
FINAL_LDFLAGS = $(LDFLAGS)

# Different sanitizer configurations
ASAN_FLAGS := -fsanitize=address
UBSAN_FLAGS := -fsanitize=undefined
LEAK_FLAGS := -fsanitize=leak

#Libft
LIBFT := $(LIBFT_DIR)/libft.a
LIBFT_FLAGS := -L$(LIBFT_DIR) -lft

#MiniLibX
# replace MLX_FLAGS with LINUX_MLX or MAC_MLX depending on your OS
# MLX_FLAGS := -L$(MLX_DIR) -lmlx_Linux -lXext -lX11 -lm -lz
LIBMLX := $(MLX_DIR)/libmlx.a

SRCS := main.c

OBJS := $(SRCS:%.c=$(OBJ_DIR)/%.o)
HDRS := $(HDRS_DIR)/fractol.h

#PRETTY
# BOLD := \033[1m
# RESET := \033[0m
# RED := \033[31;2m
# GREEN := \033[32m
# YELLOW := \033[33m
# CLEAR_LINE := \033[2K
# CURSOR_UP := \033[A
BOLD := $(shell printf '\033[1m')
RESET := $(shell printf '\033[0m')
RED := $(shell printf '\033[31;2m')
GREEN := $(shell printf '\033[32m')
YELLOW := $(shell printf '\033[33m')
CLEAR_LINE := $(shell printf '\033[2K')
CURSOR_UP := $(shell printf '\033[1A')

# Additional pretty printing variables
TOTAL_FILES := $(words $(SRCS))
CURRENT_FILE := 0
PROGRESS_BAR_WIDTH := 40

define update_progress
    @printf "$(CLEAR_LINE)"
    @printf "\r$(YELLOW)Compiling: $(RESET)["
    @$(eval CURRENT_FILE=$(shell echo $$(($(CURRENT_FILE) + 1))))
    @$(eval PERCENTAGE=$(shell echo $$(($(CURRENT_FILE) * 100 / $(TOTAL_FILES)))))
    @$(eval FILLED=$(shell echo $$(($(PERCENTAGE) * $(PROGRESS_BAR_WIDTH) / 100))))
    @$(eval EMPTY=$(shell echo $$(($(PROGRESS_BAR_WIDTH) - $(FILLED)))))
    @printf "%*s" ${FILLED} "" | tr ' ' '='
    @printf "%*s" ${EMPTY} "" | tr ' ' ' '
    @printf "] %3d%% %s" $(PERCENTAGE) "$<"
endef

.PHONY: all clean fclean re sanitize asan ubsan leak

all: $(LIBFT) $(LIBMLX) $(NAME)

run: all
	./$(NAME)

# Main program
$(NAME): $(LIBFT) $(LIBMLX) $(OBJS)
	@printf "\n$(BOLD)Linking $(NAME)$(RESET)\n"
	$(CC) $(FINAL_CFLAGS) $(OBJS) $(LIBFT_FLAGS) $(MLX_FLAGS) $(FINAL_LDFLAGS) -o $@
	printf "\n$(GREEN)$(BOLD)Build successful!$(RESET)\n" || \
	printf "$(RED)$(BOLD)Build failed!$(RESET)\n"

# Compile object files
$(OBJ_DIR)/%.o: %.c $(LIBMLX) | $(OBJ_DIR)
	$(call update_progress)
	@$(CC) $(FINAL_CFLAGS) $(INCLUDES) -c $< -o $@

# Create obj directory
$(OBJ_DIR):
	@mkdir -p $@

#LIBRARIES
# Compiling MLX42. Clones from official repo if not present.
# Output of cloning / compiliation supressed via redirecting '>/dev/null 2>&1'.
$(LIBMLX):
	@mkdir -p lib
	@if [ ! -d "$(MLX_DIR)" ] || [ -z "$$(ls -A $(MLX_DIR))" ]; then \
		printf "$(BOLD)Initializing MLX42...$(RESET)\n"; \
		git submodule add https://github.com/codam-coding-college/MLX42.git $(MLX_DIR) >/dev/null 2>&1; \
		git submodule init >/dev/null 2>&1 && git submodule update >/dev/null 2>&1
	fi
	@printf "$(BOLD)Building MLX42...$(RESET)\n"
	# @make -s -C $(MLX_DIR) >/dev/null 2>&1;
	cd $(MLX_DIR) && \
	cmake -B . # build here refers to the outputfolder.
	cmake --build . -j4 # or do make -C build -j4

# Submodules in my own Libft from own repo if not present and compiles
$(LIBFT):
	@mkdir -p lib
	@if [ ! -d "$(LIBFT_DIR)" ] || [ -z "$$(ls -A $(LIBFT_DIR))" ]; then \
		printf "$(BOLD)Initializing libft...$(RESET)\n"; \
		git submodule add https://github.com/Moat423/Libft_full.git $(LIBFT_DIR); \
		git submodule update --init --recursive -- $(LIBFT_DIR); \
	else \
		printf "$(BOLD)Updating libft...$(RESET)\n"; \
		git submodule update --init --recursive -- $(LIBFT_DIR); \
	fi
	@make -s -C $(LIBFT_DIR) > /dev/null 2>&1

# Targets for different sanitizer builds
asan: FINAL_CFLAGS = $(CFLAGS) $(ASAN_FLAGS)
asan: FINAL_LDFLAGS = $(LDFLAGS) $(ASAN_FLAGS)
asan: fclean $(NAME)

ubsan: FINAL_CFLAGS = $(CFLAGS) $(UBSAN_FLAGS)
ubsan: FINAL_LDFLAGS = $(LDFLAGS) $(UBSAN_FLAGS)
ubsan: fclean $(NAME)

leak: FINAL_CFLAGS = $(CFLAGS) $(LEAK_FLAGS)
leak: FINAL_LDFLAGS = $(LDFLAGS) $(LEAK_FLAGS)
leak: fclean $(NAME)

# Combined sanitizer (original flags + sanitizer flags)
sanitize: FINAL_CFLAGS = $(CFLAGS) $(SANITIZE_FLAGS)
sanitize: FINAL_LDFLAGS = $(LDFLAGS) $(SANITIZE_FLAGS)
sanitize: fclean $(NAME)

clean:
	@printf "$(BOLD)Cleaning object files...$(RESET)\n"
	@rm -rf $(MLX_DIR)
	@rm -dRf $(OBJ_DIR)
	@rm -f $(OBJ_DIR)/.flags
	@make -s -C $(LIBFT_DIR) clean 2>&1

fclean: clean
	@printf "$(BOLD)Cleaning executables...$(RESET)\n"
	@rm -f $(NAME)
	@make -s -C $(LIBFT_DIR) fclean >/dev/null 2>&1

re: fclean all
