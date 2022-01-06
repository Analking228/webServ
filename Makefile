NAME = webserv

SOURCES_FOLDER = ./../
SOURCES =  ... .cpp

INCLUDES_FOLDER = ./../
INCLUDES =  ... .hpp

OBJECTS_FOLDER = ./../
OBJECT = $(SOURCES:.cpp=.o)
OBJECTS = $(addprefix $(OBJECTS_FOLDER), $(OBJECT))

FLAGS = -Wall -Wextra -Werror -std=c++98

$(NAME): $(OBJECTS)
        @clang++ $(FLAGS) $(OBJECTS) -o $(NAME)

all: $(NAME)

clean:
        @rm -rf $(OBJECTS_FOLDER)

fclean: clean
        @rm -rf $(NAME)

re: fclean all
