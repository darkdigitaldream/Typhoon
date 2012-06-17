#include <GL/glut.h>
#include "hgrid.hpp"
#include "collision.hpp"
#include <cstdlib>
#include <ctime>

#define X_PIXELS 500
#define Y_PIXELS 500
#define Z_PIXELS 500

#define TO_RAD(x) ((x)/180.0 * 3.142592654f)
typedef Object Obj;
typedef boost::shared_ptr<Obj> PtrObj;
typedef vector<PtrObj> ListPtrObj;
typedef boost::shared_ptr<ListPtrObj> PtrListPtrObj;

float clocation[] = {0,0,0};
float rotation[] = {0,0,0};
float zoom = 45.0;
PtrObj FocusObj;
int lastx = 0;
int lasty = 0;

PtrListPtrObj MyObjects;
hgrid MyGrid;

PtrObj createRandObj(){

	double xratio = 1.0/X_PIXELS;
	double yratio = 1.0/Y_PIXELS;
	double zratio = 1.0/Z_PIXELS;

	int Location[3];
	int Dimensions[3];
	
	for(int i=0;i<3;i++){
		Dimensions[i] = (rand()%99)+1;
		Location[i] = (rand()%(X_PIXELS-Dimensions[i]));
	}
	PtrObj O(new AABB);
	O->Dimensions.push_back(Dimensions[0]*xratio);
	O->Dimensions.push_back(Dimensions[1]*yratio);
	O->Dimensions.push_back(Dimensions[2]*zratio);
	O->Location.push_back(Location[0]*xratio);
	O->Location.push_back(Location[1]*yratio);
	O->Location.push_back(Location[2]*zratio);
	O->debug = false;
	return O;
}

PtrListPtrObj createObjects(){
	PtrListPtrObj result(new ListPtrObj);

	srand((unsigned)time(0));
	double xratio = 1.0/X_PIXELS;
	double yratio = 1.0/Y_PIXELS;
	double zratio = 1.0/Z_PIXELS;

	for(int i=0;i<10;i++){
		result->push_back(createRandObj());
	}
	FocusObj = createRandObj();
	FocusObj->debug = false;
	result->push_back(FocusObj);

	return result;
}

bool drawPoly(){
	glBegin(GL_POLYGON);
	glVertex3d(0.0, 0.0, 0.0); 
	glVertex3d(0.0, 1.0, 0.0);
	glVertex3d(1.0, 1.0, 0.0);
	glVertex3d(1.0, 0.0, 0.0);
	glEnd();

}

void drawAABB(){
	
	typedef pair<Coordinate,Coordinate> Line;
	typedef vector<Line> LineList;
	typedef boost::shared_ptr<LineList> PtrLineList;
	
	ListPtrObj::iterator i = MyObjects->begin();
	PtrLineList Shape;

	//start drawin lines!
	glColor3f(0,0.5,0);
	glBegin(GL_LINES);

	while(i !=MyObjects->end()){
		Shape=(*i)->GetShape();

		LineList::iterator Si = Shape->begin();

		while(Si != Shape->end()){
			Coordinate Start = (*Si).first;
			Coordinate End = (*Si).second;
			glVertex3f(Start[0],Start[1],Start[2]);
			glVertex3f(End[0],End[1],End[2]);
			++Si;
		}
		++i;
	}

	glEnd();
}


void drawGrid(){
	typedef pair<Coordinate,Coordinate> Line;
	typedef vector<Line> LineList;
	typedef boost::shared_ptr<LineList> PtrLineList;
	
	PtrLineList Shape;
	typename hgrid::const_iterator i = MyGrid.begin();
	glColor3f(0,0,0.5);
	glBegin(GL_LINES);

	int cellcount = 0;
	while(i!=MyGrid.end()){
		cellcount++;
		//cout<<"Cell: "<<cellcount<<"\n";
		Shape = (*i).second->GetShape();
		LineList::iterator Si = Shape->begin();

		while(Si != Shape->end()){
			//cout<<"Lots of lines\n";
			Coordinate Start = (*Si).first;
			Coordinate End = (*Si).second;
			glVertex3f(Start[0],Start[1],Start[2]);
			glVertex3f(End[0],End[1],End[2]);
			++Si;
		}
		++i;
	}
	glEnd();
}

void draw(){
	//drawPoly();
	drawGrid();
	drawAABB();
}

void display(){
	//moveObjects();
	//updateCollisionGrid();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(zoom,1,0,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef( rotation[0], 1, 0, 0 );
	glRotatef( rotation[1], 0, 1, 0 );
	glRotatef( rotation[2], 0, 0, 1 );
	glTranslated(clocation[0],clocation[1],clocation[2]);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);
	//glShadeModel(GL_SMOOTH);
	//glEnable(GL_LIGHTING);
	draw();
	glutSwapBuffers();
}

void reshape(int x, int y){
	glViewport(0,0,x,(GLsizei)y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,0,x,y);
}

void keys(unsigned char key, int x, int y){
	if(key=='d'){
		rotation[1]+=1;
		if(rotation[1]>=360) rotation[1]-=360;
	}
	if(key=='s'){	
		float xrotradians = TO_RAD(rotation[0]);
		float yrotradians = TO_RAD(rotation[1]);
		clocation[0]+=float(sin(yrotradians))*0.2;
		clocation[1]-=float(sin(xrotradians))*0.2;
		clocation[2]-=float(cos(yrotradians))*0.2;
	}
	if(key=='a'){
		rotation[1]-=1;
		if(rotation[1]<0) rotation[1]+=360;
	}
	if(key=='w'){	
		float xrotradians = TO_RAD(rotation[0]);
		float yrotradians = TO_RAD(rotation[1]);
		clocation[0]-=float(sin(yrotradians))*0.2;
		clocation[1]+=float(sin(xrotradians))*0.2;
		clocation[2]+=float(cos(yrotradians))*0.2;
	}
	if(key=='q'){
		float yrotradians = TO_RAD(rotation[1]);
		clocation[0] += float(cos(yrotradians))*0.2;
		clocation[2] += float(sin(yrotradians))*0.2;
	}
	if(key=='e'){
		float yrotradians = TO_RAD(rotation[1]);
		clocation[0] -= float(cos(yrotradians))*0.2;
		clocation[2] -= float(sin(yrotradians))*0.2;
	}
	display();
}

void mouse(int button, int state, int x, int y){
	if(button == 3){
		zoom*=0.9;
		if(zoom<10) zoom = 10;
	}else if(button == 4){
		zoom*=1.1;
		if(zoom>190) zoom = 190;
	}
	display();
}

void mousemove(int x, int y){
	float dx = float(x-lastx);
	float dy = float(y-lasty);
	lastx=x;
	lasty=y;
	rotation[0] += dy/5.0;
	rotation[1] += dx/5.0;
	display();
}

int main(int argc, char **argv){
	//MyGrid.debug = true;
	//initialize window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(X_PIXELS, Y_PIXELS); 
	glutCreateWindow("Space Exploder");
	glOrtho(-1,1,-1,1,-1,1);
	
	//define our main functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keys);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mousemove);
	//initialize game elements
	MyObjects = createObjects();
	MyGrid.Add(*MyObjects);

	//run game
	glutMainLoop();
	return 0;
}
