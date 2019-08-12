#DIR_ALLOC = $(SOURCEDIR)alloc/

MAIN_FILE = $(SOURCEDIR)main.c

# FILES_ALLOC = exit.c			\
# 			display.c			\
# 			ram.c				\
# 			model.c				\
# 			parser.c			\


SRC =		$(MAIN_FILE)									\
			# $(addprefix $(DIR_ALLOC), $(FILES_ALLOC))		\
			$(addprefix $(DIR_DISPLAY), $(FILES_DISPLAY))	\
			$(addprefix $(DIR_GEOMETRY), $(FILES_GEOMETRY))		\
			$(addprefix $(DIR_PARSING), $(FILES_PARSING)) \
			$(addprefix $(DIR_UTILS), $(FILES_UTILS))