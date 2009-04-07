import Sprite;
import Lobe;
import Point;
import ButtonControl;
class Core {
    var lobe : Lobe;
    var controlbutton : ButtonControl;
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
        _root.tf.text = "Check it out, this is how you should do a....\n newline";
        
        controlbutton=new ButtonControl(mc,new Point(40,60),new Point(120,80),"AWESOME");
        modetogglebutton=new ModeToggleButton(mc,new Point(130,90), new Point(200,150),"DRAWMODE");
        makelobebutton=new MakeLobeButton(mc,new Point(200,100), new Point(250,150),modetogglebutton);

        mc.lineStyle(16,0xff0000);
        mc.moveTo(99,100);
        mc.lineTo(100,100);
        var local=this;
        
        //Draws a circle area that activates lobe

        var newmov=(new Lobe(mc,mc.getNextHighestDepth())).mSurface;
        newmov.lineStyle(16,0xffff00);
        newmov.moveTo(110,100);
        newmov.lineTo(115,100);
        newmov.onMouseMove = function () { 
        //_root.tf.text = local.counter;
          local.counter+=1;
        }

    }

    // entry point==============================================================
    static function main(root_mc:MovieClip) {
        app = new Core(root_mc);
    }
}
