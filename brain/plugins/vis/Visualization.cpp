#include "../../Platform.hpp"
#include "genome.pb.h"
#include "Visualization.hpp"
#include "GraphicsSystem.hpp"
#ifdef __APPLE__
#include "glut.h"
#else
#include "GL/glut.h"
//#include <GL/freeglut.h>
#endif
#include "Neuron.hpp"
#include "Brain.hpp"
namespace Elysia {
extern std::auto_ptr<boost::mutex >gRenderLock;
extern boost::condition_variable gRenderCondition;
extern boost::condition_variable gRenderCompleteCondition;

extern std::vector<Visualization*> gToRender;
std::tr1::weak_ptr<GraphicsSystem> Visualization::mGlobalGraphics;
BrainPlugin* makeVisualization(Brain*b) {
    BrainPlugin*v=new Visualization;
    v->initialize(b);
    return v;
}
Visualization::Visualization(){
    boost::unique_lock<boost::mutex> renderLock(*gRenderLock);
    if (std::find(gToRender.begin(),gToRender.end(),this)==gToRender.end()) {
        gToRender.push_back(this);
    }
    mNeuronSize=.25;
    mSynapseSnapToEdge=true;
    mScale=100.0;
    mOffset=Vector3f(0,0,0);
}
void Visualization::update() {
    {
        boost::unique_lock<boost::mutex> renderLock(*gRenderLock);    
        gRenderCondition.notify_one();
    }

    {
        boost::unique_lock<boost::mutex> renderLock(*gRenderLock);    
        gRenderCompleteCondition.wait(renderLock);
    }
}
void Visualization::initialize( Brain*b) {
    mGraphics=mGlobalGraphics.lock();
    if (!mGraphics) {
        std::tr1::shared_ptr<GraphicsSystem> tmp(new GraphicsSystem());
        mGraphics = tmp;
        mGlobalGraphics=tmp;
    }
    
    this->mBrain=b;
    BoundingBox3f3f bounds = b->getBounds();
    if (bounds==BoundingBox3f3f::null()) {
        bounds=BoundingBox3f3f(Vector3f(-128,-128,-1),Vector3f(128,128,1));
    }
    Vector3f diag=bounds.diag();
    float ratiox=diag.x/mGraphics->getWidth();
    float ratioy=diag.y/mGraphics->getHeight();
    float maxratio=ratiox>ratioy?ratiox:ratioy;
    mScale=1./maxratio;
    mOffset=-bounds.center();
}


static float selectiondefaultcol[4]={1,1,1,1};

static void arrow (float ox,float oy, float oz, float ex, float ey, float ez, float thickness) {
  thickness=fabs(thickness);
  float dx =ex-ox;
  float dy =ey-oy;
  float ldx=sqrt(dx*dx+dy*dy);
  dx*=thickness/ldx;
  dy*=thickness/ldx;
  float idx=-dy;
  float idy=dx;

  glVertex3f(ox-idx,oy-idy,oz);
  glVertex3f(ex-idx-3*dx,ey-idy-3*dy,ez);
  glVertex3f(ex+idx-3*dx,ey+idy-3*dy,ez);
  glVertex3f(ox+idx,oy+idy,oz);

  glVertex3f(ex-3*dx,ey-3*dy,ez);
  glVertex3f(ex-6*dx+3*idx,ey-6*dy+3*idy,ez);
  glVertex3f(ex,ey,ez);
  glVertex3f(ex-6*dx-3*idx,ey-6*dy-3*idy,ez);
}

static void selectionArrow(Vector3f start, Vector3f finish, float thickness, float *col=selectiondefaultcol) {

  float zero[4]={0,0,0,0};
  thickness=fabs(thickness);
  float dx =finish.x-start.x;
  float dy =finish.y-start.y;
  float ldx=sqrt(dx*dx+dy*dy);
  dx*=thickness*5/ldx;
  dy*=thickness*5/ldx;
  float idx=-dy;
  float idy=dx;
  Vector3f dd=Vector3f(dx,dy,0);
  finish=finish-dd*.75f;
  start=start+dd*.75f;
  glColor4fv(col);
  glVertex3f(start.x,start.y,start.z);
  glColor4fv(col);
  glVertex3f(finish.x,finish.y,finish.z);
  glColor4fv(zero);  
  Vector3f id=Vector3f(idx,idy,0);
  Vector3f tmp=finish+id;
  glVertex3f(tmp.x,tmp.y,tmp.z);
  tmp=start+id;
  glColor4fv(zero);  
  glVertex3f(tmp.x,tmp.y,tmp.z);

  glColor4fv(col);
  glVertex3f(finish.x,finish.y,finish.z);
  glColor4fv(col);
  glVertex3f(start.x,start.y,start.z); 
  glColor4fv(zero);
  tmp=start-id;
  glVertex3f(tmp.x,tmp.y,tmp.z);
  tmp=finish-id;
  glColor4fv(zero);
  glVertex3f(tmp.x,tmp.y,tmp.z);

  glColor4fv(zero);
  tmp=finish-id;
  glVertex3f(tmp.x,tmp.y,tmp.z);
  glColor4fv(zero);
  tmp=finish+dd*2;
  glVertex3f(tmp.x,tmp.y,tmp.z);
  glColor4fv(zero);
  tmp=finish+id;
  glVertex3f(tmp.x,tmp.y,tmp.z);
  glColor4fv(col);
  glVertex3f(finish.x,finish.y,finish.z);

  glColor4fv(zero);
  tmp=start-id;
  glVertex3f(tmp.x,tmp.y,tmp.z);
  glColor4fv(zero);
  tmp=start-dd*2;
  glVertex3f(tmp.x,tmp.y,tmp.z);
  glColor4fv(zero);
  tmp=start+id;
  glVertex3f(tmp.x,tmp.y,tmp.z);
  glColor4fv(col);
  glVertex3f(start.x,start.y,start.z);
}
void arrow(Vector3f start, Vector3f finish, float thickness) {
  arrow(start.x,start.y,start.z,finish.x,finish.y,finish.z,thickness);
}
void drawRect(Vector3f lower_left,Vector3f upper_right) {
    glVertex3f(lower_left.x,lower_left.y,upper_right.z);
    glVertex3f(lower_left.x,upper_right.y,upper_right.z);
    glVertex3f(upper_right.x,upper_right.y,upper_right.z);
    glVertex3f(upper_right.x,lower_left.y,upper_right.z);
    //printf ("drawing from %f %f to %f %f\n",lower_left.x,lower_left.y,upper_right.x,upper_right.y);
}
void drawRectOutline(Vector3f lower_left,Vector3f upper_right, float halfx,float halfy) {
    glVertex3f(lower_left.x,lower_left.y-halfy,upper_right.z);
    glVertex3f(lower_left.x,upper_right.y+halfy,upper_right.z);
    glVertex3f(lower_left.x-halfx,upper_right.y,upper_right.z);
    glVertex3f(upper_right.x+halfx,upper_right.y,upper_right.z);
    glVertex3f(upper_right.x,upper_right.y+halfy,upper_right.z);
    glVertex3f(upper_right.x,lower_left.y-halfy,upper_right.z);
    glVertex3f(upper_right.x+halfx,lower_left.y,upper_right.z);
    glVertex3f(lower_left.x-halfx,lower_left.y,upper_right.z);

}


int stringWidth(const std::string &dat, bool addspace, bool removespace) {
	int retval=addspace?1:0;
	static int space=glutBitmapWidth(GLUT_BITMAP_HELVETICA_10,' ');
	if (removespace) retval-=space-1;
	for (std::string::const_iterator i=dat.begin(),ie=dat.end();i!=ie;++i)
		retval+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_10,*i);
	return retval;
}
#define highest_special_char ((char)47)
#define lowest_special_char ((char)35)

bool Visualization::getNeuronWidthHeight(const std::string &text, float&wid,float&hei,bool selected) {    
    float minh=10;
    bool drawText=text.length()&&(text[0]==' '||(text[0]>=lowest_special_char&&text[0]<=highest_special_char));
    float neuronsize=mNeuronSize*mScale;
    float neuronheight=neuronsize;
    float neuronwidth=neuronsize;
    float textwidth=0;
    if (drawText&&neuronheight<minh)
        neuronheight=minh;
    if (selected||neuronheight<mNeuronSize*mScale*1.625) {
        //we're fine
    }else {
        neuronheight=mNeuronSize*mScale;
        drawText=false;
    }
  
    if (drawText) textwidth=stringWidth(text,true,true);
    
    if (drawText&&neuronwidth<textwidth){
        neuronwidth=textwidth;
    }
    wid=neuronwidth;
    hei=neuronheight;
    return drawText;		
}
Vector3f Visualization::getNeuronLocation(Neuron* n)const {
    return (n->getLocation()+mOffset)*mScale;
}

void Visualization::getSynapseStartEnd(Neuron * start, bool startIsSelected, Neuron * end, bool endIsSelected, Vector3f& A, Vector3f &B) {
  float swid,shei,ewid,ehei;
  A=getNeuronLocation(start);
  B=getNeuronLocation(end);
  if (mSynapseSnapToEdge==false) {
    return;
  }

  getNeuronWidthHeight(start->getName(),swid,shei,startIsSelected);
  getNeuronWidthHeight(end->getName(),ewid,ehei,endIsSelected);
  Vector3f delta=B-A;
  Vector3f startdelta(0,0,0),enddelta(0,0,0);
  if (fabs(delta.x)>fabs(delta.y)){          
    if (delta.x>0) {
      startdelta.x+=swid;
      enddelta.x-=ewid;
    }else {
      startdelta.x-=swid;
      enddelta.x+=ewid;
    }
  }else {
    if (delta.y>0) {
      startdelta.y+=shei;
      enddelta.y-=ehei;
    }else {
      startdelta.y-=shei;
      enddelta.y+=ehei;
    }
  }
  if (start->getName().length()&&start->getName()[0]=='!')
    startdelta.x=startdelta.y=0;//not useful right now
  if (end->getName().length()&&end->getName()[0]=='!')
    enddelta.x=enddelta.y=0;//not useful right now
  
  A+=startdelta;
  B+=enddelta;
}

void Visualization::drawNeuronBody(Neuron*n) {
    Vector3f center=n->getLocation();
    center.z=0;
    float wid=0;
    float hei=0;
    bool text=getNeuronWidthHeight(n->getName(), wid,hei,mSelected.find(n)!=mSelected.end());
    //printf ("aaawing from %f %f to %f %f\n",((center-Vector3f(wid/2,hei/2,0))).x,((center-Vector3f(wid/2,hei/2,0))).y,((center+Vector3f(wid/2,hei/2,0))).x,(center+Vector3f(wid/2,hei/2,0)).y);
    drawRect((center+mOffset-Vector3f(wid/2,hei/2,0))*mScale,(center+mOffset+Vector3f(wid/2,hei/2,0))*mScale);
}

void Visualization::drawNeuron(Neuron*n) {
    drawNeuronBody(n);
}

void Visualization::draw() {
    // Anti-Clockwise Winding
    printf ("start draw\n");
    glBegin(GL_QUADS);
    for (Brain::NeuronSet::iterator i=mBrain->mAllNeurons.begin(),
             ie=mBrain->mAllNeurons.end();
         i!=ie;
         ++i) {
        drawNeuron(*i);
    }
    glEnd(); 
    printf ("estart draw\n");   
}
Visualization::~Visualization() {
    {
        boost::unique_lock<boost::mutex> renderLock(*gRenderLock);
        std::vector<Visualization*>::iterator where=std::find(gToRender.begin(),gToRender.end(),this);
        if (where!=gToRender.end()) {
            gToRender.erase(where);
        }
        
    }
    mGraphics=std::tr1::shared_ptr<GraphicsSystem>();

}
}
