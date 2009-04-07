import Sprite;
import Lobe;
import Point;
import ButtonControl;
class Core {
    var lobe : Lobe;
    var modetogglebutton : ModeToggleButton;
    var makelobebutton : MakeLobeButton;
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
        
        modetogglebutton=new ModeToggleButton(mc,new Point(0,40), new Point(100,70),"DRAWMODE");
        makelobebutton=new MakeLobeButton(mc,new Point(0,80), new Point(100,110),modetogglebutton);

        var local=this;
        
        //Draws a circle area that activates lobe
/*
        var newmov=(new Lobe(mc,mc.getNextHighestDepth())).mSurface;
        newmov.lineStyle(16,0xffff00);
        newmov.moveTo(110,100);
        newmov.lineTo(115,100);
        newmov.onMouseMove = function () { 
        //_root.tf.text = local.counter;
          local.counter+=1;
        }
*/
    }

    // entry point==============================================================
    static function main(root_mc:MovieClip) {
        app = new Core(root_mc);
    }
}
