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
#include "Branch.hpp"
#include "Neuron.hpp"
#include "Brain.hpp"
#include "Synapse.hpp"
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
    memset(mKeyDown,0,sizeof(mKeyDown));
    memset(mSpecialKeyDown,0,sizeof(mSpecialKeyDown));
    memset(mMouseButtons,0,sizeof(mMouseButtons));
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
void Visualization::postInputEvent(const Event&evt){
    mInputEvents.push_back(evt);
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
//    printf("Drawing arrow from %f %f %f to %f %f %f t %f\m",           start.x,start.y,start.z,finish.x,finish.y,finish.z,thickness);
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
    if (neuronheight<1) {
        neuronheight=1;
        neuronwidth=1;
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

Vector3f Visualization::drawNeuronBody(Neuron*n) {
    Vector3f center=n->getLocation();
    center.z=0;
    float wid=0;
    float hei=0;
    bool text=getNeuronWidthHeight(n->getName(), wid,hei,mSelected.find(n)!=mSelected.end());
    //printf ("aaawing from %f %f to %f %f\n",((center-Vector3f(wid/2,hei/2,0))).x,((center-Vector3f(wid/2,hei/2,0))).y,((center+Vector3f(wid/2,hei/2,0))).x,(center+Vector3f(wid/2,hei/2,0)).y);
    Vector3f scaledCenter=getNeuronLocation(n);
    drawRect(scaledCenter-Vector3f(wid/2,hei/2,0),scaledCenter+Vector3f(wid/2,hei/2,0));
    return scaledCenter+Vector3f(0,hei/2,0);
}

void drawParallelogramLineSegment(const Vector3f &source, const Vector3f &dest, double width) {
    width*=.5;
    glVertex3f(source.x-width,source.y,source.z);
    glVertex3f(dest.x-width,dest.y,dest.z);
    glVertex3f(dest.x+width,dest.y,dest.z);
    glVertex3f(source.x+width,source.y,source.z);
}

void Visualization::drawBranch(const Neuron * n, const Branch* dendrite, Vector3f top, float scale) {
    for (Branch::SynapseConstIterator i=dendrite->childSynapseBegin(),ie=dendrite->childSynapseEnd();i!=ie;++i) {
        Neuron * destination = (*i)->mRecipientNeuron;
        if (destination) {
            float wid=0;
            float hei=0;
            bool text=getNeuronWidthHeight(destination->getName(), wid,hei,mSelected.find(destination)!=mSelected.end());
            Vector3f scaledDestination = getNeuronLocation(destination);
            arrow(scaledDestination-Vector3f(0,hei/2,0),top,1);
        }
    }
}
void Visualization::drawDendrites(const Neuron * n, const CellComponent* dendrite, Vector3f top, float scale) {
    CellComponent::ChildIterator i=dendrite->childBegin(),ie=dendrite->childEnd(),b;
    size_t size = ie-i;
    b=i;    
    if (scale<10.0) scale=0.0;
    float width = scale*.125/size;
    float height = mScale*5;
    if (height<1.0) height=1.0;
    if (width<2.0) width=2.0;
    for (;i!=ie;++i) {
        Vector3f dest = Vector3f(top.x-scale*.25+scale*(i-b)/((double)size),
                                 top.y+height,
                                 top.z);
        if (scale)
            drawParallelogramLineSegment(top,dest,width);
        const CellComponent *nextInLine = *i;
        drawDendrites(n,nextInLine,scale?dest:top,scale*.5);
    }
    {
        const Branch * b = dynamic_cast<const Branch*>(dendrite);
        if (b) {
            drawBranch(n,b,top,scale);
        }
    }

}

void Visualization::drawNeuron(Neuron*n) {
    Vector3f top = drawNeuronBody(n);
    bool drawDendrites=true;
    if (drawDendrites) {
        this->drawDendrites(n, n, top, mScale*20);
    }
    
}
void Visualization::processEvent(struct Event&evt) {
}


void Visualization::doInput() {
    std::vector<Event> inputEvents;
    mInputEvents.swap(inputEvents);
    for (std::vector<Event>::iterator i=inputEvents.begin(),ie=inputEvents.end();i!=ie;++i) {
        switch(i->event) {
          case Event::MOUSE_CLICK:
            if (i->button<sizeof(mMouseButtons)/sizeof(mMouseButtons[0])) {
                mMouseButtons[i->button]=1;
            }
            break;
          case Event::MOUSE_UP:
            if (i->button<sizeof(mMouseButtons)/sizeof(mMouseButtons[0])) {
                mMouseButtons[i->button]=0;
            }
            break;
          case Event::KEYBOARD:
            //amKeyDown[i->button]=1;
            break;
          case Event::KEYBOARD_UP:
            //mKeyDown[i->button]=0;
            break;
          case Event::KEYBOARD_SPECIAL:
            if (i->button<256)
                mSpecialKeyDown[i->button]=1;
            break;
          case Event::KEYBOARD_SPECIAL_UP:
            if (i->button<256)
                mSpecialKeyDown[i->button]=0;
            break;
          case Event::MOUSE_DRAG:
            break;
          case Event::MOUSE_MOVE:
            break;
        }
        processEvent(*i);
    }
    float speed=20;
    if (mKeyDown['d']) {
        mOffset.x-=speed/mScale;
    }
    if (mKeyDown['a']) {
        mOffset.x+=speed/mScale;
    }
    if (mKeyDown['w']) {
        mOffset.y-=speed/mScale;
    }
    if (mKeyDown['s']) {
        mOffset.y+=speed/mScale;
    }
    if (mKeyDown['q']) {
        mScale*=.95;
    }
    if (mKeyDown['e']) {
        mScale*=1.05;
    }
}
void Visualization::purgeMarkedForDeathNeurons() {
   
    for (std::vector<Neuron*>::iterator i=mfd.begin(),ie=mfd.end();i!=ie;++i) {
        SelectedNeuronMap::iterator where =mSelectedNeurons.find(*i);
        if (where!=mSelectedNeurons.end()) {
            mSelectedNeurons.erase(where);
        }
    }
}

void Visualization::draw() {
    purgeMarkedForDeathNeurons();
    doInput();
    glBegin(GL_QUADS);
    for (Brain::NeuronSet::iterator i=mBrain->mAllNeurons.begin(),
             ie=mBrain->mAllNeurons.end();
         i!=ie;
         ++i) {
        drawNeuron(*i);
    }
    glEnd(); 
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
void Visualization::notifyNeuronDestruction(Neuron*n){
    mfd.push_back(n);
}

}
