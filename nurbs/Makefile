

LLARGS = -framework OpenCL -framework OpenGL -L/usr/local/homebrew/Cellar/glfw3/3.1.1/lib -L/usr/local/homebrew/Cellar/glew/1.11.0/lib -L/usr/local/homebrew/Cellar/imagemagick/6.9.0-10/lib -lglfw3 -lGLEW -lMagick++-6.Q16 -lMagickCore-6.Q16
CCARGS = -g -framework OpenCL -framework OpenGL  -I/usr/local/homebrew/Cellar/glew/1.11.0/include -I/usr/local/include -I/usr/local/homebrew/Cellar/imagemagick/6.9.0-10/include/ImageMagick-6

all: mygl

mygl: object.o mygl.o texture.o camera.o scene.o loader.o technique.o render_target.o fx_linedraw.o protocol.o input_handler.o triangulator.o
	g++ -g $^ $(LLARGS) -o $@

object.o: object.cpp object.h
	g++ -c $< -o $@ $(CCARGS)

scene.o: scene.cpp scene.h
	g++ -c $< -o $@ $(CCARGS)

mygl.o: mygl.cpp
	g++ -c $< -o $@ $(CCARGS)

camera.o: camera.cpp
	g++ -c $< -o $@ $(CCARGS)

texture.o: texture.cpp texture.h
	g++ -c $< -o $@ $(CCARGS)

loader.o: loader.cpp loader.h
	g++ -c $< -o $@ $(CCARGS)

technique.o: technique.cpp technique.h
	g++ -c $< -o $@ $(CCARGS)

render_target.o: render_target.cpp render_target.h
	g++ -c $< -o $@ $(CCARGS)

fx_linedraw.o: fx_linedraw.cpp fx_linedraw.h
	g++ -c $< -o $@ $(CCARGS)

protocol.o: protocol.cpp protocol.h
	g++ -c $< -o $@ $(CCARGS)

input_handler.o: input_handler.cpp input_handler.h
	g++ -c $< -o $@ $(CCARGS)

triangulator.o: triangulator.cpp triangulator.h
	g++ -c $< -o $@ $(CCARGS)
