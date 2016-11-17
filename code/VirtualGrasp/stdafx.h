// GL extensions
#include <GL/glew.h>
#define GLEE_ARB_vertex_buffer_object GLEW_ARB_vertex_buffer_object

#include "Macros.h"

// Shape Analysis
#include "ShapeAnalysis/Agent/HumanHand.h"
extern HumanHand * hand;

//#include "ShapeAnalysis/Agent/HumanBody.h"
//extern HumanBody * actor;

// Our classes
#include "Mesh.h"
extern Mesh * mesh;

// UI classes
#include "virtualgrasp.h"
extern VirtualGrasp * mainWindow;