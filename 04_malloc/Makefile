#◉────▣───▢◇▢───▣────◉•◉───▣───▢    Colors:    ▢───▣───◉•◉───▣────▢◇▢───▣───◉#

CURRENT_COLOR   := \033[0;34m
RESET           := \033[0m

#◉───▣───▢◇▢───▣────◉•◉───▣───▢ malloc Variables: ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME            = libft_malloc_$(HOSTTYPE).so
LIB_NAME        = libft_malloc.so
CC              = gcc
CFLAGS          = -Wall -Wextra -Werror -fPIC
FLAGS_LIB       = -shared
FLAGS_THREDS    = -lpthread
RM              = rm -rf

# Directories
SRC_DIR     := src
OBJ_DIR     := obj
INC_DIR     := inc

# Test Variables
TEST_SRC    := ./test/src/main.c
TEST_BIN    := ./test/test_malloc

#◉───▣───▢◇▢───▣────◉•◉───▣───▢ valgrind Variables ▢───▣───▣───◉•◉───▣───◉#

VALGRIND    := valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=all --error-limit=no --log-file=test/valgrind_outputs/valgrind_output_$(shell date +%Y%m%d_%H%M%S).log -s

#◉───▣───▢◇▢───▣────◉•◉───▣───▢ strace Variables ▢───▣───▣───◉•◉───▣───◉#

STRACE      := strace -e trace=mmap,munmap -o test/strace_outputs/strace_output_$(shell date +%Y%m%d_%H%M%S).log

#◉───▣───▢◇▢───▣────◉•◉───▣───▢ Libft Variables ▢───▣───◉•◉───▣───▣───◉#

LIBFT_DIR   := libft
LIBFT       := $(LIBFT_DIR)/libft.a

#◉───▣───▢◇▢───▣────◉•◉───▣───▢ Includes ▢───▣───▣───▣──▢◇▢───▣───◉#

INCLUDES        := -I$(INC_DIR)
INCLUDES_LIBFT  := -I$(LIBFT_DIR)

#◉───▣───▢◇▢───▣────◉•◉───▣───▢ malloc srcs ▢───▣───▣───◉•◉───▣───▢◇▢───◉#

SRCS            := \
                free.c malloc.c realloc.c \
                show_alloc_mem.c show_alloc_mem_ex.c

# Full .c paths
SRC_PATHS   := $(addprefix $(SRC_DIR)/,$(SRCS))

# Matching .o paths in OBJ_DIR
OBJS        := $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

#◉───▣───▢◇▢───▣────◉•◉───▣───▢ rules ▢───▣───▣───▣───▢◇▢───▣───◉#

all: $(LIBFT) $(NAME) $(LIB_NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(FLAGS_LIB) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft $(FLAGS_THREDS)
	@echo "$(CURRENT_COLOR) $(NAME) created.\n$(RESET)"

$(LIB_NAME): $(NAME)
	ln -sf $(NAME) $(LIB_NAME)
	@echo "$(CURRENT_COLOR) Symlink $(LIB_NAME) -> $(NAME) created.\n$(RESET)"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(INCLUDES_LIBFT) -c $< -o $@

$(TEST_BIN): all $(TEST_SRC)
	$(CC) -Wall -Wextra -Werror $(INCLUDES) $(INCLUDES_LIBFT) $(TEST_SRC) -L. -lft_malloc -Wl,-rpath,. -o $(TEST_BIN)

clean:
	$(RM) $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@echo "$(CURRENT_COLOR) 🧹 All object files have been cleaned.\n$(RESET)"

fclean: clean clean_valgrind clean_strace
	$(RM) $(NAME) $(LIB_NAME) $(TEST_BIN)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(CURRENT_COLOR) 🧹 $(NAME) & $(LIB_NAME): have been cleaned.\n$(RESET)"

re: fclean all

#◉───▣───▢◇▢───▣────◉•◉───▣───▢ Valgrind ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

valgrind: $(TEST_BIN)
	@echo "$(CURRENT_COLOR)➵⤐──╌╌➣⋆➣╌╌──< Valgrind Test >──╌╌➣⋆➣╌╌──$(RESET)"
	@mkdir -p test/valgrind_outputs
	$(VALGRIND) $(TEST_BIN)
	@-test/sh_files/./open_valgrind_log.sh
	@echo "$(CURRENT_COLOR)➵⤐╌╌➣⋆➣╌─< Valgrind Completed >─╌➣⋆➣╌╌──$(RESET)"

clean_valgrind:
	test/sh_files/./clean_valgrind.sh

#◉───▣───▢◇▢───▣────◉•◉───▣───▢ Strace mem test ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

test_strace: all $(TEST_BIN)
	@echo "$(CURRENT_COLOR)➵⤐──╌╌➣⋆➣╌╌──< Strace Test >──╌╌➣⋆➣╌╌──$(RESET)"
	@mkdir -p test/strace_outputs
	$(STRACE) $(TEST_BIN)
	@-test/sh_files/./open_strace_log.sh
	@echo "$(CURRENT_COLOR)➵⤐╌╌➣⋆➣╌─< Strace Completed >─╌➣⋆➣╌╌──$(RESET)"

clean_strace:
	test/sh_files/./clean_strace.sh

#◉───▣───▢◇▢───▣────◉•◉───▣───▢ Test ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

test: $(TEST_BIN)
	@echo "$(CURRENT_COLOR)➵⤐──╌╌➣⋆➣╌╌── Test──╌╌➣⋆➣╌╌──$(RESET)"
	@$(TEST_BIN)
	@-test/sh_files/./run_test.sh
	@echo "$(CURRENT_COLOR)➵⤐╌╌➣⋆➣── End ─╌➣⋆➣╌─$(RESET)"

#◉───▣───▢◇▢───▣────◉•◉───▣───▢ Norminette ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

norm:
	@echo "$(CURRENT_COLOR)➵⤐──╌╌➣⋆➣╌╌── Checking Norminette ──╌╌➣⋆➣╌╌──$(RESET)"
	@norminette $(SRC_PATHS) $(INC_DIR)/*.h > file_norminette.log || true
	@code file_norminette.log

#◉───▣───▢◇▢───▣────◉•◉───▣───▢ Format ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

format:
	c_formatter_42 $(SRC_PATHS)

#◉───▣───▢◇▢───▣────◉•◉───▣───▢ Phony targets ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

.PHONY: all clean fclean re valgrind clean_valgrind test_strace clean_strace test norm format