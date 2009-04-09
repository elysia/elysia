import Sprite;
import Lobe;
import Point;
import ButtonControl;
class Core {
    var lobe : Lobe;
    var modetogglebutton : ModeToggleButton;
    var makelobebutton : MakeLobeButton;
    var brain:Brain;
    static var app : Core;
    var counter;
    //==========================================================================
    function Core(mc:MovieClip) {
    
        //this.lobe=new Lobe();  //What does this line actually do?
        this.counter=0;
        // creates a 'tf' TextField size 800x600 at pos 0,0
        _root.createTextField("tf",0,0,0,800,600);
        // write some text into it
        _root.tf.text = "Elysia Brain Editor\nVersion 2.0\n";
        brain = new Brain();
        modetogglebutton=new ModeToggleButton(mc,new Point(0,40), new Point(100,70),"DRAWMODE");
        makelobebutton=new MakeLobeButton(mc,new Point(0,80), new Point(100,110),modetogglebutton);
        var keyListener = {};
        keyListener.onKeyDown = function()
        {
            var k = Key.getCode();
            if(k == Key.DELETEKEY){
                Brain.makenewlobe();
            }
        };

    Key.addListener( keyListener );
    }

    // entry point==============================================================
    static function main(root_mc:MovieClip) {
        app = new Core(root_mc);
    }
}
