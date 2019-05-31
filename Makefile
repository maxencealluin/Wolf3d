# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ydonse <ydonse@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/04/23 10:03:18 by ydonse            #+#    #+#              #
#    Updated: 2019/05/31 15:08:38 by malluin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = wolf3d

SRC_PATH = src

SRC_NAME =	main.c \
			parsing.c \
			errors.c \
			walls_checking.c \
			minimap.c \
			minimap_2.c \
			debug.c \
			structs_initialize.c \
			actions.c \
			controls.c \
			image.c \
			raycasting.c \
			raycasting_2.c \
			raycasting_setup.c \
			raycasting_draw.c \
			utils_trig.c \
			collisions.c \
			parser_tga.c \
			sounds.c \
			free.c \
			menu.c

CPPFLAGS = -I libft/includes/ -I /usr/local/include/ -MMD

LDFLAGS = -L libft/ -lft  -L /usr/local/include/

CC = gcc

CFLAGS = -Wall -Wextra -Werror

OBJ_NAME = $(SRC_NAME:.c=.o)
OBJ_PATH = obj

HEADER_PATH = includes/
HEADER_NAME = wolf3d.h

ID = $(shell id -un)

SDLINCL = $(shell $(ABSOLUTE_DIR)/SDL2/bin/sdl2-config --cflags)
SDLFLAGS =  $(SDLINCL) $(shell $(ABSOLUTE_DIR)/SDL2/bin/sdl2-config --libs)

MIXINCL = -I ./SDL2/include/SDL2/
MIXFLAGS = $(MIXINCL) -L./SDL2/lib -lSDL2_MIXER

OBJ = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))
SRC = $(addprefix $(SRC_PATH)/,$(SRC_NAME))

HEADER = $(addprefix $(HEADER_PATH)/,$(HEADER_NAME))

ABSOLUTE_DIR = $(shell pwd)
SDL2_SRC = $(ABSOLUTE_DIR)/SDL2-2.0.9
SDL2_MIX_SRC = $(ABSOLUTE_DIR)/SDL2_mixer-2.0.4

all: $(NAME)

sdl :
	if test ! -f SDL_2.0.9.tar.gz; \
	then curl http://libsdl.org/release/SDL2-2.0.9.tar.gz -o SDL_2.0.9.tar.gz; \
	fi
	if test ! -d SDL2-2.0.9; \
	then tar xvf SDL_2.0.9.tar.gz; \
	fi
	mkdir -p SDL2 2> /dev/null || true
	mkdir -p SDL2/build 2> /dev/null || true
	cd SDL2/build; \
		$(SDL2_SRC)/configure --prefix $(ABSOLUTE_DIR)/SDL2; \
		@make -j6; \
		make install;
	cd $(ABSOLUTE_DIR)
	if test ! -f SDL2_mixer-2.0.4.tar.gz; \
	then curl https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-2.0.4.tar.gz -o SDL2_mixer-2.0.4.tar.gz; \
	fi
	if test ! -d SDL2_mixer-2.0.4; \
	then tar xvf SDL2_mixer-2.0.4.tar.gz; \
	fi
	cd SDL2/build; \
		$(SDL2_MIX_SRC)/configure --prefix $(ABSOLUTE_DIR)/SDL2; \
		@make -j6; \
		make install;

$(NAME): $(OBJ)
	make -C libft/
	$(CC) $(CFLAGS) $(LDFLAGS) $(SDLFLAGS) $(MIXFLAGS) $(MIXINCL) $(LDLIBS) $(SDLFMK) $^ -o $@

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
	$(CC) $(CFLAGS) $(CPPFLAGS) -I $(HEADER_PATH) $(SDLINCL) $(MIXINCL) -o $@ -c $<

$(OBJ_PATH):
	@mkdir $(OBJ_PATH) 2> /dev/null || true

sdlclean:
	rm -rf $(SDL2_SRC) $(SDL2_MIX_SRC) SDL_2.0.9.tar.gz SDL2_mixer-2.0.4.tar.gz SDL2

clean:
	make clean -C libft/
	rm -f $(OBJ) $(OBJ:.o=.d)
	@rmdir $(OBJ_PATH) 2> /dev/null || true

fclean: clean
	make fclean -C libft/
	rm -f $(NAME)

re: fclean
	$(MAKE) all

.PHONY: make clean fclean re sdl sdlclean

-include $(OBJ:.o=.d)
