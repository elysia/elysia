#include "BrainPlugin.hpp"
namespace Elysia {
class Brain;
class CellComponent;
class GraphicsSystem;
class Neuron;
class Branch;
class Visualization:public BrainPlugin  {
    ///Neurons marked for death by the main plugin
    std::vector<Neuron*> mfd;
    std::tr1::shared_ptr<GraphicsSystem> mGraphics;
    static std::tr1::weak_ptr<GraphicsSystem> mGlobalGraphics;
    Brain * mBrain;
    float mNeuronSize;
    float mScale;
    Vector3f mOffset;
    bool mSynapseSnapToEdge;
    std::tr1::unordered_set<Neuron*> mSelected;
    std::tr1::unordered_set<Neuron*> mDetailed;
    Vector3f getNeuronLocation(Neuron*n)const;
    bool getNeuronWidthHeight(const std::string&text, float&width, float&hei, bool selected);
    bool getCurrentNeuronWidthHeight(Neuron*n, float&width, float&height);
    void getSynapseStartEnd(Neuron * start, bool startIsSelected, Neuron * end, bool endIsSelected, Vector3f& A, Vector3f &B);
    ///Returns top location from where dendrites can start branching out
    Vector3f drawNeuronBody(Neuron*n);
    void drawBranch(const Neuron * n, const Branch* dendrite, Vector3f top, float scale) ;
    void drawDendrites(const Neuron* n, const CellComponent* dendrite, Vector3f startLocation, float scale);
    void drawNeuron(Neuron*n);
    void doInput();
    typedef std::tr1::unordered_set<Neuron*> SelectedNeuronMap;
    SelectedNeuronMap mSelectedNeurons;
    void purgeMarkedForDeathNeurons();
    ///Returns if a single neuron may be selected by the given mouse coordinates
    bool click (Neuron*n,float mousex, float mousey);
    ///Returns if a single neuron may be selected by the given drag select box
    bool dragSelect(Neuron *n, float bbminx, float bbminy, float bbmaxx, float bbmaxy);
public:
    Visualization();
    void draw();
    void update();
    void initialize(Brain*b);
    void notifyNeuronDestruction(Neuron*n);
    ~Visualization();
    struct Event {
        enum {
            MOUSE_CLICK,
            MOUSE_UP,
            MOUSE_DRAG,
            MOUSE_MOVE,
            KEYBOARD,
            KEYBOARD_UP,
            KEYBOARD_SPECIAL,
            KEYBOARD_SPECIAL_UP
        } event;
        float mouseX;
        float mouseY;
        int button;
        int modCodes;
    };
    void postInputEvent(const Event&evt);
private:    
    //process a single event and make it adjust the input state machine
    
    std::vector<Event>mInputEvents;
    class Button {
        
        float minX;
        float maxX;
        float minY;
        float maxY;
        float mScale;//how big the text is
        std::string mText;
        std::tr1::function<void()> mClick;
        bool renderedOnce;
    public:
        /**
           buttons are referenced from the top left corner... in pixels
         */
        Button(float minX,
               float minY,
               float maxX,
               float maxY,
               const std::string &text,
               const std::tr1::function<void()> &click,
               float scale=.0625);
        void draw(Visualization * vis);
        bool click(Visualization * vis, const Visualization::Event&evt)const;
        void doClick(Visualization * vis, const Visualization::Event&evt)const;
    };
    class InputStateMachine {
        void drag(Visualization * vis, const Visualization::Event&evt);
        void click(Visualization * vis, const Visualization::Event&evt);
        std::vector<Button> mButtons;
    public:
        bool mActiveDrag;
        float mDragStartX;
        float mDragStartY;
        int mKeyDown[256];
        int mSpecialKeyDown[256];
        int mMouseButtons[5];
        float mMouseX;
        float mMouseY;
        void processPersistentState(const Visualization::Event&evt);
        void processEvent(Visualization*parent, const Visualization::Event&evt);
        InputStateMachine();
        
        void draw(Visualization*parent);
    }mInput;
};
BrainPlugin* makeVisualization(Brain*b);
}
