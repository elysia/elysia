import flash.geom.*;
import flash.geom.Point;

class Tuto {

    static var app : Tuto;
    var counter;
    function Tuto(mc:MovieClip) {
        this.counter=0;
        // creates a 'tf' TextField size 800x600 at pos 0,0
        _root.createTextField("tf",0,0,0,800,600);
        // write some text into it
        _root.tf.text = counter;//"Hello world !";
        var myPoint = new flash.geom.Point(5,5);
        mc.lineStyle(16,0xff0000);
        mc.moveTo(99,100);
        mc.lineTo(100,100);
        var local=this;
        var newmov=mc.createEmptyMovieClip("Actor",mc.getNextHighestDepth());
        mc.onMouseMove = function () { 
         
          _root.tf.text = local.counter;
          local.counter+=1;
          newmov.lineStyle(16,0xff0000);
          newmov.moveTo(mc._xmouse-1,mc._ymouse);
          newmov.lineTo(mc._xmouse,mc._ymouse);
          if (newmov.enabled)
           newmov.enabled=false;
          else
           newmov.enabled=false;
        }
        newmov.onMouseMove = function () { 
          newmov._x+=1;
          newmov._width=40;
          newmov._height=40;
          _root.tf.text = newmov.getDepth();
          newmov.lineStyle(16,0x00ff00);
          newmov.moveTo(mc._xmouse/2,mc._ymouse/2);
          newmov.lineTo(mc._xmouse/2+1,mc._ymouse/2);
          if (newmov.enabled)
           newmov.enabled=false;
          else
           newmov.enabled=false;
        }
    }

    // entry point
    static function main(root_mc:MovieClip) {
        app = new Tuto(root_mc);
    }
}
