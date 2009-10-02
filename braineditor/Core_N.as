import Sprite;
import Lobe;
import Point;
import ButtonControl;
import Scroll;
class Core_N {
    var lobe : Lobe;
    var modetogglebutton : ModeToggleButton;
    var makelobebutton : MakeLobeButton;
    var brain:Brain;
    var scroller : Scroll;
    static var app : Core_N;
    var counter;
    //==========================================================================
    function Core_N(mc:MovieClip) {
    
        //this.lobe=new Lobe();  //What does this line actually do?
        this.counter=0;
        // creates a 'tf' TextField size 800x600 at pos 0,0
        _root.createTextField("tf",0,0,0,800,600);
        // write some text into it
        _root.tf.text = "Nia's' Brain Editor\nVersion 2.0\n";
        brain = new Brain(mc);
        modetogglebutton=new ModeToggleButton(mc,new Point(0,40), new Point(100,70),"DRAWMODE");
        makelobebutton=new MakeLobeButton(mc,new Point(0,80), new Point(100,110),modetogglebutton);
        scroller = new Scroll(mc,new Point(20,20),new Point(40,120));
        scroller.translate(new Point(100,200));
    }

    // entry point==============================================================
    static function main(root_mc:MovieClip) {
        app = new Core_N(root_mc);
    }
}
