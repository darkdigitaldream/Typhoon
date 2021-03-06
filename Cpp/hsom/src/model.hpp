//3d SOM model
#include "somIII.hpp"

//simplified adapter/wrapper for SDL library window creation & 2d drawing functions
//SDL.h and SDL_image.h included internally, which includes SDL_Event for our controller routines
#include "render.hpp"

//library for rendering an openGL context to an SDL window
#include "SDL/SDL_opengl.h"

//3d viewing model
#include "camera.hpp"

//import SOM training vectors during initialization
#include "tspFileReader.hpp"

class model{
	public:
		//self-organizing map model is our focus
		som mySOM;

		//camera class for viewing som model
		camera* cam;

		//sdl event pipeline handled by controller
		bool terminateProgram;	
		SDL_Event event;

		//this helps our picking logic find the selected object. 
		//it should be populated on each mouse up event
		int mouseX;
		int mouseY;

		void init(){
			mouseX = 0;
			mouseY = 0;
			selectedIndex = -1;	
			//use time to get more random initial SOM arrangement
			srand(time(NULL));

			TSPFileReader trainingVectorFactory;
			vector<neuron> TrainingNeurons;

			//data from file
			//TrainingNeurons = trainingVectorFactory.retrieveTrainingVectors("./maps/RGB.tsp");

			//data randomly generated
			for(int i=0; i<100; i++){
				neuron myNewNeuron;
				for int(j=0;j<3;j++){
					myNewNeuron.push_back(randomFloatInRange(0,255)); 
				}
				TrainingNeurons.push_back(myNewNeuron);
			}

			//clustered data added
			for(int i=0; i<100; i++){
				neuron myNewNeuron;
				for int(j=0;j<3;j++){
					//add only high-blue values
					if(j==3)
						myNewNeuron.push_back(randomFloatInRange(127,255)); 
					else
						myNewNeuron.push_back(0); 
				}
				TrainingNeurons.push_back(myNewNeuron);
			}

			cam = new camera();
			cam->translateRight(0.75);
			cam->translateUp(0.75);
			cam->translateForward(-1.75);

			//colourcube: 3d data, 3d mesh(10x10x10), 1000 training iterations
			mySOM.init(TrainingNeurons,8,8,8,100);

			terminateProgram = false;
		}
};
