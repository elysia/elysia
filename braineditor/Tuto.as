import flash.geom.*;
import flash.geom.Point;
import Sprite;
import Lobe;
class Tuto {
    var lobe:Lobe;
    static var app : Tuto;
    var counter;
    function Tuto(mc:MovieClip) {
        this.lobe=new Lobe();
        this.counter=0;
        // creates a 'tf' TextField size 800x600 at pos 0,0
        _root.createTextField("tf",0,0,0,800,600);
        // write some text into it
        _root.tf.text = counter;//"Hello world !";
        mc.lineStyle(16,0xff0000);
        mc.moveTo(99,100);
        mc.lineTo(100,100);
        var local=this;
        var newmov=(new Lobe(mc,mc.getNextHighestDepth())).mSurface;
        newmov.lineStyle(16,0xffff00);
        newmov.moveTo(110,100);
        newmov.lineTo(115,100);

        newmov.onMouseMove = function () { 
         
         //          _root.tf.text = local.counter;
          local.counter+=1;
        }

    }

    // entry point
    static function main(root_mc:MovieClip) {
        app = new Tuto(root_mc);
    }
}
