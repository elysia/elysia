
import flash.geom.*;
import flash.geom.Point;
class Test {
function Test(scope:MovieClip) {
//local function: drawPoint
  var drawPoint = function(p:flash.geom.Point, size:Number, tgt:MovieClip, color: Number){
    if (!color) color = 0x000000; //black
        tgt.lineStyle(size, color);
        tgt.moveTo(p.x-1, p.y);
        tgt.lineTo(p.x+1, p.y);
        tgt.moveTo(p.x, p.y-1);
        tgt.lineTo(p.x, p.y+1);
    }

//Draw a diagonal line of Points 10 pixels apart
    for(var i = 10; i < 200; i += 10){
         var myPoint = new flash.geom.Point(i,i);
        drawPoint(myPoint, 5, scope);
    }
}
//Main Entry Point
static function main() { 
  var test:Test = new Test(_root);
}
}
/*
class Tuto {

    static var app : Tuto;

    function Tuto() {
        // creates a 'tf' TextField size 800x600 at pos 0,0
        _root.createTextField("tf",0,0,0,800,600);
        // write some text into it
        _root.tf.text = "Hello world !";
    }

    // entry point
    static function main(mc) {
        app = new Tuto();
    }
}
*/