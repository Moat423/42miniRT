# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/22 15:55:27 by lmeubrin          #+#    #+#              #
#    Updated: 2025/05/07 15:27:24 by lmeubrin         ###   ########.fr        #
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

DIRS = $(addprefix $(OBJ_DIR)/, . util parse)

CC := clang
NAME := miniRT
INCLUDES := -I$(HDRS_DIR) -I$(LIBFT_DIR) -I$(MLX_DIR)/include

# Set MLX_FLAGS based on the operating system
ifeq ($(UNAME_S), Linux)
    MLX_FLAGS = $(LINUX_MLX)
else ifeq ($(UNAME_S), Darwin)
    MLX_FLAGS = $(MAC_MLX)
endif

LIBMLX := $(MLX_DIR)/build/libmlx42.a

#flags
CFLAGS := -Werror -Wall -Wextra #-g
LDFLAGS += -L./$(MLX_DIR)/build -lmlx42 #-g
# Ofast might instroduce race conditions in multithreading
# 03 is fast and safer
OPTIM_FLAGS := -O3 -march=native
LINUX_MLX := -ldl -lglfw -pthread -lm
MAC_MLX := -lglfw -framework Cocoa -framework OpenGL -framework IOKit

#TODO: IMPORTANT, clear the bonus flag when submitting mandatory
FINAL_CFLAGS = $(CFLAGS) $(OPTIM_FLAGS) -DBONUS
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

SRCS := $(addprefix $(SRCS_DIR)/,\
		main.c \
		camera.c \
		render.c \
		movement.c \
		key_movement.c \
		intersect.c \
		sphere.c \
		object.c \
		shading.c \
		plane.c \
		cylinder.c \
		cylinder_utils.c \
		light.c \
		circle.c \
		cone.c \
		cone_utils.c \
		aabb.c \
		window_utils.c \
		scene_fixing.c \
		checkers.c \
		sphere_checkers.c \
		$(addprefix util/, quaternion.c color.c util.c vec3_0.c vec3_1.c vec3_2.c \
		scene_malloc.c scene_destroy.c interval.c preprocess.c preprocess2.c \
		print_scene.c print_scene_utils_0.c print_scene_utils_1.c) \
		$(addprefix parse/, input_check.c parse_scene.c set_scene.c set_objs.c \
		set_utils.c set_bumpmap.c get_texture.c set_bumpmap_utils.c) \
		)

OBJS := $(SRCS:($SRCS_DIR)%.c=$(OBJ_DIR)/%.o)
HDRS := $(addprefix $(HDRS_DIR)/, miniRT.h miniRT_types.h parse.h)

#PRETTY
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

.PHONY: all clean fclean re sanitize asan ubsan leak debug clean_submodules

all: $(LIBFT) $(LIBMLX) $(NAME)

run: all
	./$(NAME)

# Main program
$(NAME): $(LIBFT) $(LIBMLX) $(OBJS) $(HDRS) Makefile
	@printf "\n$(BOLD)Linking $(NAME)$(RESET)\n"
	$(CC) $(FINAL_CFLAGS) $(OBJS) $(INCLUDES) $(LIBFT_FLAGS) $(MLX_FLAGS) $(FINAL_LDFLAGS) -o $@ && \
	printf "\n$(GREEN)$(BOLD)Build successful!$(RESET)\n" || \
	printf "$(RED)$(BOLD)Build failed!$(RESET)\n"

# Compile object files
$(OBJ_DIR)/%.o: $(SRCS_DIR)%.c $(LIBMLX) | $(DIRS)
	$(call update_progress)
	@$(CC) $(FINAL_CFLAGS) $(INCLUDES) -c $< -o $@

# Create obj directory
$(DIRS):
	@mkdir -p $@
# $(OBJ_DIR):
# 	@mkdir -p $@

#LIBRARIES
# Compiling MLX42. Clones from official repo if not present.
# Output of cloning / compiliation supressed via redirecting '>/dev/null 2>&1'.
$(LIBMLX):
	@mkdir -p lib
	@if [ ! -d "$(MLX_DIR)" ] || [ -z "$$(ls -A $(MLX_DIR))" ]; then \
		printf "$(BOLD)Initializing MLX42...$(RESET)\n"; \
		git submodule add -f https://github.com/codam-coding-college/MLX42.git $(MLX_DIR) >/dev/null 2>&1; \
		git submodule init >/dev/null 2>&1 && git submodule update >/dev/null 2>&1
	fi
	@printf "$(BOLD)Building MLX42...$(RESET)\n"
	# @make -s -C $(MLX_DIR) >/dev/null 2>&1;
	cd $(MLX_DIR) && \
	cmake -B build && \
	cmake --build build -j4

# Submodules in my own Libft from own repo if not present and compiles
$(LIBFT):
	@mkdir -p lib
	@if [ ! -d "$(LIBFT_DIR)" ] || [ -z "$$(ls -A $(LIBFT_DIR))" ]; then \
		printf "$(BOLD)Initializing libft...$(RESET)\n"; \
		git submodule add -f https://github.com/Moat423/Libft_full.git $(LIBFT_DIR); \
		git submodule update --init --recursive -- $(LIBFT_DIR); \
	else \
		printf "$(BOLD)Updating libft...$(RESET)\n"; \
		git submodule update --init --recursive -- $(LIBFT_DIR); \
	fi
	@printf "$(BOLD)Ensuring libft is at the correct commit...$(RESET)\n"
	@( cd $(LIBFT_DIR) && \
	  git fetch origin && \
	  git checkout a4c8c412c9d6ca51b20e4fb1d879c21201417c02 && \
	  printf "$(GREEN)Successfully checked out target commit.$(RESET)\n" ) || \
	  printf "$(RED)Failed to checkout specific commit. Manual intervention required.$(RESET)\n"
	@printf "$(BOLD)Building libft...$(RESET)\n"
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

debug: FINAL_CFLAGS += -g
debug: FINAL_LDFLAGS += -g
debug: fclean $(NAME)

bonus: FINAL_CFLAGS += -DBONUS
bonus: re

clean_submodules:
	@printf "$(BOLD)Cleaning submodule object files...$(RESET)\n"
	@rm -rf $(MLX_DIR)
	@make -s -C $(LIBFT_DIR) clean 2>&1

clean:
	@printf "$(BOLD)Cleaning object files...$(RESET)\n"
	@rm -dRf $(OBJ_DIR)
	@rm -f $(OBJ_DIR)/.flags

fclean: clean
	@printf "$(BOLD)Cleaning executables...$(RESET)\n"
	@rm -f $(NAME)
	@make -s -C $(LIBFT_DIR) fclean >/dev/null 2>&1

re: fclean all
