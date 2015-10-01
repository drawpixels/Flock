CC = c++
RM = rm

PKGDIR = $(HOME)/Packages
GLEWDIR = $(PKGDIR)/glew-1.12.0
GLFWDIR = $(PKGDIR)/glfw-3.1.1
GLMDIR = $(PKGDIR)/glm-0.9.6.3
X11DIR = /usr/X11R6
CFLAGS =  
CDEFS = 
IPATH = -I$(HOME)/Projects -I$(GLFWDIR)/include -I$(GLEWDIR)/include -I$(GLMDIR) 
LPATH = -L$(GLFWDIR)/lib -L$(GLEWDIR)/lib 
LFLAGS = -framework OpenGL -framework Cocoa -framework CoreVideo -framework IOKit
LIBS = -lglfw3 -lGLEWs $(HOME)/Projects/common/shader.o

all: flock

flock: flock.o 
	$(CC) -o $@ $^ $(LFLAGS) $(LPATH) $(LIBS)
	
.cpp.o: 
	$(CC) $(CDEFS) $(CFLAGS) -c -o $@ $^ $(IPATH)
	
clean:
	$(RM) -f *.o *.gch warp



